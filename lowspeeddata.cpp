#include "lowspeeddata.h"

LowSpeedData::LowSpeedData() : myProcess(nullptr), base_data_timer(nullptr), ass_serialport(nullptr),
    disk_watcher(nullptr), clear_feedback_flag(0), first_run(true), send_par(false)
{
    /** @brief 构造非心电串口 */
    this->moveToThread(&thread);
    thread.setObjectName("LowSpeedData");
    thread.start();

    parameter_data.run_mode = 1;
    parameter_data.rtrac_mode = 0;
    parameter_data.trigger_mode = 0;
    parameter_data.trigger_source = 1;
    parameter_data.trigger_rate = 0;
    parameter_data.aug_data = 0;
    parameter_data.time = 12850; //0x3232
    parameter_data.threshold = 15;

    parameter_data.aux_value.Keyhp = 1;
    parameter_data.aux_value.Keylp = 0;
    parameter_data.aux_value.preSec = 10;
    parameter_data.aux_value.Keynotch50 = 0;
    parameter_data.aux_value.ecgHGain = 0; //0=auto,50,100=1mv,200
    parameter_data.aux_value.speedGain = 2; //1=12.5mm/s ; 2=25mm/s ; 4=50mm/s

    /* 读取参数文件配置 */
    operation_paramer_file(false);
}

void LowSpeedData::operation_paramer_file(bool rw)
{
    QFile file("/opt/SG1000.dat");

    QDateTime timeDate = QDateTime::currentDateTime();

    if (rw)
    {
        char buf[128] = {0};
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        parameter_data.aux_value.update_time = timeDate .toTime_t();
        memcpy(buf, &parameter_data, sizeof(P_DATA_s));
        out.writeRawData(buf, sizeof(P_DATA_s));
    }
    else
    {
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen()) return;
        QByteArray by = file.readAll();

        P_DATA_s real_value;
        memset(&real_value, 0, sizeof(P_DATA_s));
        memcpy(&real_value, by.data(), sizeof(P_DATA_s));

        if (timeDate.toTime_t() - real_value.aux_value.update_time < 1800)
            parameter_data = real_value;
    }
    file.close();
}

void LowSpeedData::init()
{
    qDebug() << "LowSpeedData ThreadID: " << QThread::currentThreadId();

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(HS_CPU,&mask);

    if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &mask) < 0)
        qDebug() << "LowSpeedData->pthread_setaffinity_np error!";

    if (!ass_serialport)
    {
        ass_serialport = new SerialPort;
        ass_serialport->serialPortInit(ASS_SERIAL_NAME);
    }

    qml_paramer_loading();

    if (!shake_init()) return;
    if (!check_init()) return;

    if (!base_data_timer) base_data_timer = new QTimer;

    if (!disk_watcher) disk_watcher = new UdiskWatcher;

    connect(base_data_timer, SIGNAL(timeout()), this, SLOT(base_data_refresh()));

    //base_data_timer->start(BASE_REFRESH_TIME);

    bind_signals();

    emit publicData->playSound(INIT_VOLUME_TYPE);

    emit ecg_start(true);
}

void LowSpeedData::qml_paramer_loading()
{
    emit publicData->setRunMode(parameter_data.run_mode);
    emit publicData->setTriggerMode(parameter_data.trigger_mode);
    emit publicData->setTriggerSource(parameter_data.trigger_source);
    emit publicData->setTriggerRate(parameter_data.trigger_rate);
    emit publicData->setAugmentData(parameter_data.aug_data);
    emit publicData->setThresholdData(parameter_data.threshold);
    emit publicData->setTimeData(parameter_data.time);
    emit publicData->setRTracData(parameter_data.rtrac_mode);

    /* 使用双函数，目的是防止界面反向信号 */
    emit publicData->modifyTriggerMode(parameter_data.trigger_mode);
    emit publicData->modifyETriggerSource(parameter_data.trigger_source);
    emit publicData->modifyAugmentData(parameter_data.aug_data);
    emit publicData->modifyTriggerRate(parameter_data.trigger_rate);
    emit publicData->modifyThresholdData(parameter_data.threshold);
    emit publicData->modifyInflationData(parameter_data.time >> 8 & 0xf);
    emit publicData->modifyDeflationData(parameter_data.time & 0xf);
    //emit publicData->modifyRTracData(parameter_data.rtrac_mode);
    emit publicData->modifyEcgGainData(parameter_data.aux_value.ecgHGain);

    publicData->savePrePoint = parameter_data.aux_value.preSec * 250;
}

void LowSpeedData::setBaseDataPointer(QSharedPointer<PublicData> obj)
{
    publicData = obj;
    publicData->Keyhp = parameter_data.aux_value.Keyhp;
    publicData->Keylp = parameter_data.aux_value.Keylp;
    publicData->Keynotch50 = parameter_data.aux_value.Keynotch50;

    publicData->setEcgSpeedData(parameter_data.aux_value.speedGain);
    publicData->initWidthGainEcg(parameter_data.aux_value.speedGain/2.0);

    uint16_t gain_dim = parameter_data.aux_value.ecgHGain;
    if (gain_dim)
        publicData->ecg_gain_mode = false;
    else
        gain_dim = 200;

    QString gain = QString::number(gain_dim/100.0, 'f', 1) + " mv";
    emit publicData->setEcgGainData(gain, gain_dim);
    publicData->initHeightGainEcg(gain_dim/100.0);
}

bool LowSpeedData::shake_init()
{
    qDebug() << "LowSpeedData ThreadID: " << QThread::currentThreadId();

    for(int i = 0; i < 3; i++)
    {
        ass_serialport->clear();
        /** @todo 处理辅助串口握手 */
        if (ERROR == shakeHandsProcess())
            continue;

        QByteArray buf;
        while (1)
        {
            if (ass_serialport->waitForReadyRead(6000))
                buf += ass_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "AUX ShakeHandsProcess Timeout");
                return false;
            }

            if (ERROR == shakeHandsProcess(buf))
                continue;

            return true;
        }
    }

    return false;
}

void LowSpeedData::bind_signals()
{
    connect(ass_serialport, SIGNAL(readyRead()), this, SLOT(read_data_process()), Qt::UniqueConnection);
}

void LowSpeedData::unbind_signals()
{
    ass_serialport->disconnect(SIGNAL(readyRead()));
}

void LowSpeedData::read_data_process()
{
    QByteArray buf;

    buf = ass_serialport->readAll();

    ass_data_process(buf);

    return;
}

qint64 LowSpeedData::shakeHandsProcess(const QByteArray& rdata)
{
    if (rdata.isEmpty())
    {
        /** @todo sender */
        return ass_serialport->sendInitData(COMMAND_HS);
    }
    else
    {
        /** @todo recv proc */
        qDebug() << "LowSpeedData::shakeHandsProcess: " << rdata.toHex(' ').toUpper();

        if (rdata.size() != COMMAND_LEN)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_HS_LEN ERROR");
            return ERROR;
        }

        if (rdata[4] != 0x00)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_HS ERROR");
            return ERROR;
        }

        return COMMAND_LEN;
    }
}

bool LowSpeedData::check_init()
{
    for(int i = 0; i < 3; i++)
    {
        ass_serialport->clear();
        /** @todo 处理机器自检,时间比较长 */
        if (ERROR == checkProcess())
            continue;

        QByteArray buf;
        while(1)
        {
            if (ass_serialport->waitForReadyRead(20000))
                buf += ass_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "AUX CheckProcess Timeout");
                return false;
            }

            if (ERROR == checkProcess(buf))
                continue;
            else
                return true;
        }
    }

    return false;
}

qint64 LowSpeedData::checkProcess(const QByteArray& rdata)
{
    uint8_t combuff[256] = {0};
    if (rdata.isEmpty())
    {
        /** @todo sender */
        return ass_serialport->sendInitData(COMMAND_CK);
    }
    else
    {
        /** @todo recv proc */
        qDebug() << "LowSpeedData::checkProcess: " << rdata.toHex(' ').toUpper();

        if (rdata.size() != COMMAND_LEN)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_CK_LEN ERROR");
            return ERROR;
        }

        int length = rdata.size();
        ::memcpy(combuff, rdata.data(), length);
        uint8_t crc = MyCrc::my_crc8_check(combuff, 0, length-1);

        if (crc != combuff[length-1] || rdata[2] != '\x82' /*|| rdata[4] | '\x00'*/)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_CK ERROR");
            return ERROR;
        }

        return COMMAND_LEN;
    }
}

bool LowSpeedData::machine_init()
{
    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return false;
    }

    for(int i = 0; i < 3; i++)
    {
        ass_serialport->clear();
        /** @todo 处理机器初始化 */
        if (ERROR == machineProcess())
            continue;

        QByteArray buf;
        while(1)
        {
            if (ass_serialport->waitForReadyRead(60000))
                buf += ass_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "AUX MachineProcess Timeout");
                return false;
            }

            if (ERROR == machineProcess(buf))
                continue;
            else
                return true;
        }
    }

    return false;
}

qint64 LowSpeedData::machineProcess(const QByteArray& rdata)
{
    uint8_t combuff[256] = {0};
    if (rdata.isEmpty())
    {
        /** @todo sender */
        return ass_serialport->sendInitData(COMMAND_MA);
    }
    else
    {
        /** @todo recv proc */
        qDebug() << "LowSpeedData::machineProcess: " << rdata.toHex(' ').toUpper();

        if (rdata.size() != COMMAND_LEN)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_MA_LEN ERROR");
            return ERROR;
        }

        int length = rdata.size();
        ::memcpy(combuff, rdata.data(), length);
        uint8_t crc = MyCrc::my_crc8_check(combuff, 0, length-1);

        if (crc != combuff[length-1] || rdata[2] != '\x83' || rdata[4] | '\x00')
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_MA ERROR");
            return ERROR;
        }

        return COMMAND_LEN;
    }
}

bool LowSpeedData::param_init()
{
    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return false;
    }

    for(int i = 0; i < 3; i++)
    {
        ass_serialport->clear();
        /** @todo 默认参数下发 */
        if (ERROR == paramerProcess())
            continue;

        QByteArray buf;
        while(1)
        {
            if (ass_serialport->waitForReadyRead(3000))
                buf += ass_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "AUX ParamProcess Timeout");
                return false;
            }

            if (ERROR == paramerProcess(buf))
                continue;
            else
                return true;
        }
    }

    return true;
}

qint64 LowSpeedData::paramerProcess(const QByteArray& rdata)
{
    if (rdata.isEmpty())
    {
        /** @todo sender */
        return ass_serialport->sendParamData(COMMAND_PA, AUX_All, parameter_data);
    }
    else
    {
        /** @todo recv proc */
        qDebug() << "LowSpeedData::paramProcess: " << rdata.toHex(' ').toUpper();

        if (rdata.size() != COMMAND_LEN)
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_PA_LEN ERROR");
            return ERROR;
        }

        if (rdata[2] != '\x87' || rdata[4] | '\x00')
        {
            publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_PA ERROR");
            return ERROR;
        }

        return COMMAND_LEN;
    }
}

void LowSpeedData::set_paramer_data(QString name, int value)
{
    qDebug() << "set_paramer_data->" << name << "---" << value;

    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return;
    }

    uint8_t aux = AUX_All;  //default all par

    /* 设置相关参数 */
    if (!name.compare("run_mode", Qt::CaseInsensitive))
    {
        aux = AUX_RM_TYPE;
        parameter_data.run_mode = value & '\x07';
    }
    else if (!name.compare("trigger_mode", Qt::CaseInsensitive))
    {
        aux = AUX_TM_TYPE;
        parameter_data.trigger_mode = value & '\x07';
        emit publicData->modifyTriggerMode(parameter_data.trigger_mode);
    }
    else if (!name.compare("trigger_rate", Qt::CaseInsensitive))
    {
        aux = AUX_TR_TYPE;
        parameter_data.trigger_rate = value & '\x07';
        emit publicData->modifyTriggerRate(value);
    }
    else if (!name.compare("ECG", Qt::CaseInsensitive)) //心电信号源
    {
        aux = AUX_TS_TYPE;
        uint8_t tmp = parameter_data.trigger_source & '\xF0';
        parameter_data.trigger_source = tmp | (value & '\x0F');
        emit publicData->modifyETriggerSource(value);
    }
    else if (!name.compare("Pressure", Qt::CaseInsensitive))  //血压源
    {
        aux = AUX_TS_TYPE;
        parameter_data.trigger_source &= '\x0F';
        parameter_data.trigger_source |= value << 4;
        emit publicData->modifyATriggerSource(value);  // 0 ~ 1
        publicData->internal_press = value ? false : true;
    }
    else if (!name.compare("aug_data", Qt::CaseInsensitive))
    {
        aux = AUX_IP_TYPE;
        int index = 0;
        for(int i = 9; i > 0; i--)
        {
            if (!(value >> i & 0x01))
                index++;
        }
        parameter_data.aug_data = index & '\x0F';
        emit publicData->modifyAugmentData(parameter_data.aug_data);
    }
    else if (!name.compare("Inflation", Qt::CaseInsensitive))  //充气时序参考值
    {
        aux = AUX_IT_TYPE;
        parameter_data.time = value << 8 & 0x7F00;
        emit publicData->modifyInflationData(value);  //0~100
    }
    else if (!name.compare("Deflation", Qt::CaseInsensitive))  //放气时序参考值
    {
        aux = AUX_DT_TYPE;
        parameter_data.time = value & 0x7F;
        emit publicData->modifyDeflationData(value);  //0~100
    }
    else if (!name.compare("Thresholds", Qt::CaseInsensitive))  //血压阈值
    {
        aux = AUX_AP_TYPE;
        parameter_data.threshold = value & 0xFF;
        emit publicData->modifyThresholdData(value);
    }
    else if (!name.compare("RTrac", Qt::CaseInsensitive))  //R-Trac
    {
        aux = AUX_RT_TYPE;
        parameter_data.rtrac_mode = value & 0xFF;
    }

    /* 直接下发用户操作参数 */
    //if (!send_par) ass_serialport->sendParamData(COMMAND_PA, aux, parameter_data);
    ass_serialport->sendParamData(COMMAND_PA, aux, parameter_data);

    /* 运行中无法变更参数，需确认需求 */
    //operation_paramer_file(true);
}

bool LowSpeedData::balloon_control(bool flag)
{
    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return false;
    }

    /** @todo 处理球囊泵数据控制 */
    for(int i = 0; i < 3; i++)
    {
        ass_serialport->clear();
        //flag -> true 处理球囊泵启动逻辑  flag -> false 处理球囊泵关闭逻辑
        if(ERROR == ass_serialport->sendControlData(COMMAND_CL, flag ? AUX_B_START : AUX_B_STOP))
            continue;

        QByteArray buf;
        while(1)
        {
            if (ass_serialport->waitForReadyRead(9000))
                buf += ass_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "AUX BalloonProcess Timeout");
                return false;
            }

            /** @todo recv proc */
            qDebug() << "LowSpeedData::balloonProcess: " << buf.toHex(' ').toUpper();

            if (buf.size() < COMMAND_LEN)
                continue;

            if (buf[2] != '\x8D' || buf[4] | '\x00')
            {
                publicData->sendAlarmMessage(ErrorType, "Auxiliary AUX_BA ERROR");
                return false;
            }

            /* 后期增加校验位 验证，为了测试暂时不添加 */

            /* 球囊操作成功 发送显示动作 */
            emit balloon_show(flag);

            return true;
        }
    }

    publicData->sendAlarmMessage(ErrorType, "AUX Command Sender Fail");

    return false;
}

void LowSpeedData::balloon_status_change(bool flag)
{
    if (first_run)
    {
        /* 下发用户设置相关参数 */
        if (!param_init())
        {
            publicData->restoreMenuStatus(flag);
            return;
        }

        /* 下发机器初始化指令 */
        if (!machine_init())
        {
            publicData->restoreMenuStatus(flag);
            return;
        }

        first_run = false;
    }

    unbind_signals();

    /* 下发开始反搏指令 */
    if (balloon_control(flag))
    {
        send_par = flag ? true : false;
        /* 配置信息写入参数文件 */
        operation_paramer_file(true);
    }
    else
        publicData->restoreMenuStatus(flag);

    bind_signals();
}

void LowSpeedData::sensor_calibrate()
{
    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return;
    }

    /** @todo 处理传感器校准,时间比较长 */
    publicData->sendAlarmMessage(MessageType, "sensor calibrate start");
    if (ERROR == ass_serialport->sendControlData(COMMAND_SC, AUX_All))
        publicData->sendAlarmMessage(ErrorType, "sensor calibrate sending fail");
}

void LowSpeedData::get_pressure_data()
{
    /** @todo 获取压力数据 */
    if (ERROR == ass_serialport->sendControlData(COMMAND_PD, AUX_P_TYPE))
        publicData->sendAlarmMessage(ErrorType, "pressure_data sending fail");
}

void LowSpeedData::get_helium_data()
{
    /** @todo 获取氦气数据 */
    if (ERROR == ass_serialport->sendInitData(COMMAND_HE))
        publicData->sendAlarmMessage(ErrorType, "helium sending fail");
}

void LowSpeedData::get_battery_data()
{
    /** @todo 获取电池数据 */
    if (ERROR == ass_serialport->sendInitData(COMMAND_BA))
        publicData->sendAlarmMessage(ErrorType, "battery sending fail");
}

void LowSpeedData::get_machine_status_data()
{
    /** @todo 获取机器状态数据 */
    if (ERROR == ass_serialport->sendInitData(COMMAND_MS))
        publicData->sendAlarmMessage(ErrorType, "machine_status sending fail");
}

void LowSpeedData::calibrateProcess(const uint8_t* rdata)
{
    /** @todo 通过状态位判断是否重置成功 */
    if (rdata[4] | '\x00')
    {
        publicData->sendAlarmMessage(ErrorType, "sensor calibrate fail");
        return;
    }

    publicData->sendAlarmMessage(MessageType, "sensor calibrate success");
}

void LowSpeedData::pressureProcess(const uint8_t* rdata)
{
    /** @todo 压力值扩大100倍 */
}

void LowSpeedData::heliumProcess(const uint8_t* rdata)
{
    /** @todo 氦气值 百分比 -> 小数 */
    qreal value = rdata[4] / 100.0;

    emit publicData->heliumValueChange(value);
}

void LowSpeedData::batteryProcess(const uint8_t* rdata)
{
    /** @todo
     * 电池电流（2byte）+续航时间（2byte）+百分比（1byte）
     * 传输数据低位在前 高位在后，电池电流最高位1 电池放电 0 电池充电，单位是mA
    */

    //emit battery;
}

void LowSpeedData::machineStatusProcess(const uint8_t* rdata)
{
    /** @todo 机器运行过程的状态解析 */
}

void LowSpeedData::oneParamerProcess(const uint8_t* rdata)
{
    /** @todo 单一参数处理结果解析 */
}

void LowSpeedData::base_data_refresh()
{
    if (!ass_serialport || !ass_serialport->isOpen())
        return;

    qDebug() << "---base_data_refresh---";

    /* 下位机单方面取消 */
    //get_pressure_data();

    get_helium_data();

    get_battery_data();

    get_machine_status_data();

    /* 配置信息写入参数文件 */
    operation_paramer_file(true);
}

/** @brief COM实时一帧数据处理 */
void LowSpeedData::ass_data_process(QByteArray& rdata)
{
    thread_local uint8_t combuff[256];
    thread_local size_t combuff_size = 0;
    thread_local bool half_package = true;

    int index = 0;
    int cur_index;

    qDebug() << "ass com.size() -> " << rdata.size() << "data() -> " << rdata.toHex(' ').toUpper();

    while(ERROR != (cur_index = rdata.indexOf(PublicData::protocol_head, index)))
    {
        if (half_package)
        {
            combuff_size = 2;
            half_package = false;
            ::memcpy(combuff, PublicData::protocol_head.data(), combuff_size);

            rdata.remove(0, cur_index+2);
            continue;
        }

        ::memcpy(combuff+combuff_size, rdata.data(), cur_index);
        combuff_size += cur_index;
        rdata.remove(0, cur_index);

        frame_data_process(combuff_size, combuff);
        half_package = true;
    }

    ::memcpy(combuff+combuff_size, rdata.data(), rdata.length());
    combuff_size += rdata.length();

    frame_data_process(combuff_size, combuff);
}

void LowSpeedData::set_iab_fill()
{
    if (!ass_serialport || !ass_serialport->isOpen())
    {
        publicData->sendAlarmMessage(ErrorType, "AUX Uninitialized");
        return;
    }

    /** @todo 充放气控制 */
    if (ERROR == ass_serialport->sendControlData(COMMAND_IF, AUX_IP_TYPE))
        publicData->sendAlarmMessage(ErrorType, "IAB Fill Command Send Fail");

    publicData->sendAlarmMessage(MessageType, "IAB Fill Start");
}

void LowSpeedData::iabFillProcess(const uint8_t *rdata)
{
    /** @todo 通过状态位判断是否成功 */
    if (rdata[4] | '\x00')
    {
        publicData->sendAlarmMessage(ErrorType, "IAB Fill Fail");
        return;
    }

    publicData->sendAlarmMessage(MessageType, "IAB Fill Success");
}

void LowSpeedData::frame_data_process(size_t real_size, uint8_t *fdata)
{
    if (!fdata) return;

    uint8_t len = fdata[3];
    if (real_size > 2 && real_size >= len)
    {
        if (fdata[len-1] != MyCrc::my_crc8_check(fdata, 0, len-1))
            return; /* 校验错误 */

        switch(fdata[2] & '\x0F')
        {
            /* 传感器校准 */
            case '\x0C' : calibrateProcess(fdata); break;
            /* 压力数据处理 */
            case '\x0B' : pressureProcess(fdata); break;
            /* 氦气数据处理 */
            case '\x06' : heliumProcess(fdata); break;
            /* 电池数据处理 */
            case '\x05' : batteryProcess(fdata); break;
            /* 机器状态数据处理 */
            case '\x04' : machineStatusProcess(fdata); break;
            /* 特定参数状态处理 */
            case '\x07' : oneParamerProcess(fdata); break;
            /* 充放气控制处理 */
            case '\x09' : iabFillProcess(fdata); break;
        }
    }
}

void LowSpeedData::set_system_volume(int value)
{
    QString program = "/usr/bin/amixer";
    QStringList arguments;
    QProcess myProcess;

    arguments << "sset" << "Master" << "Playback";
    arguments << QString::number(value);

    myProcess.start(program,arguments);

    if(!myProcess.waitForFinished(3000))
        return;

    publicData->playSound(ADJUST_VOLUME_TYPE);
}

void LowSpeedData::set_show_brightness(int value)
{
    int fd_mips = ::open("/sys/class/backlight/dsi1-backlight/brightness",O_RDWR|O_NONBLOCK);

    char buf[20];

    if (fd_mips <= 0) return;

    sprintf(buf, "%d", value);
    ::write(fd_mips, buf, sizeof(buf));

    ::close(fd_mips);
}

void LowSpeedData::set_control_brightness(int value)
{
    int fd_lvds = ::open("/sys/class/backlight/lvds-backlight/brightness",O_RDWR|O_NONBLOCK);

    char buf[20];

    if (fd_lvds <= 0) return;

    sprintf(buf, "%d", value);
    ::write(fd_lvds, buf, sizeof(buf));

    ::close(fd_lvds);
}

void LowSpeedData::set_write_file(bool flag)
{
    publicData->enable_write_file = flag;
}

void LowSpeedData::printer_ecg_wave(bool status)
{
    if (status)  //start print
    {
        publicData->ecg_printer_queue.clear();

        PublicData::semaphore.acquire();

        publicData->print_flag = true;
        publicData->ecg_printer_queue = publicData->ecg_pre_sec_queue;

        PublicData::semaphore.release();

        return;
    }

    publicData->print_flag = false;

    printDataWriteFile(publicData->ecg_printer_queue);
}

void LowSpeedData::set_show_more_ecgwave(int index)
{
    qDebug() << "set_show_more_ecgwave -> " << index;

    publicData->freeze_display_status = true;

    publicData->initGainEcg(index ? true : false);

    publicData->show_all_lead_wave = index ? true : false;

    emit resetMainThreadAff(index ? 4 : 1);

    emit publicData->clearLineSeries();
}

void LowSpeedData::printDataWriteFile(const QQueue<ECG_SHOW_DATA>& buf)
{
    QString file_name = getPrinterFileName();
    QFile file(file_name);

    if (file.isOpen())
        return;

    file.open(QFile::WriteOnly|QFile::Truncate);

    uint64_t count = publicData->ecg_printer_queue.size();
    uint64_t len = sizeof(ECG_SHOW_DATA) * count;

    len += sizeof(PRINTER_BASE_DATA);

    char *array = new char[len+1];
    memset(array, 0, len+1);

    char *cur_index = array;

    memcpy(cur_index, &publicData->printer_base_data, sizeof(PRINTER_BASE_DATA));
    cur_index += sizeof(PRINTER_BASE_DATA);
    for(uint64_t i = 0; i < count; i++)
    {
        auto node = publicData->ecg_printer_queue.dequeue();
        memcpy(cur_index, &node, sizeof(ECG_SHOW_DATA));
        cur_index += sizeof(ECG_SHOW_DATA);
    }

    QDataStream out(&file);
    out.writeRawData(array, len);

    file.flush();
    file.close();

    if (array)
    {
        delete[] array;
        array = nullptr;
    }

    //printer pdf of start process
    start_printer_process(file_name);
}

void LowSpeedData::start_printer_process(QString file)
{
    QString program = "/opt/XJ-VAD-PRINTER";
    QStringList arguments;
    arguments << file;

    if (!myProcess) myProcess = new QProcess(this);

    connect(myProcess, SIGNAL(finished(int)), this, SLOT(get_printer_result(int)), Qt::UniqueConnection);

    myProcess->start(program,arguments);
    //myProcess->startDetached(program,arguments);
}

void LowSpeedData::get_printer_result(int ret_id)
{
    if (!myProcess)
        return;

    if (0 == ret_id)
    {
        /** @brief 正确执行返回值  */
    }

    myProcess->close();
}


void LowSpeedData::set_aug_alarm_data(int value)
{
    publicData->aug_alarm_threshod = value;
}

void LowSpeedData::set_ecg_gain_dim(int value)
{
    if (value)
    {
        publicData->initHeightGainEcg(value/100.0);
        publicData->ecg_gain_mode = false;
        QString gain = QString::number(value/100.0, 'f', 1) + " mv";
        emit publicData->setEcgGainData(gain, value);
    }
    else
    {
        publicData->ecg_gain_mode = true;
        publicData->ecg_height_gain = 0;
    }

    parameter_data.aux_value.ecgHGain = value;
}

void LowSpeedData::set_ecg_speed_gain(int value)
{
    qDebug() << "set_ecg_speed_gain -> " << value;

    publicData->freeze_display_status = true;

    publicData->initWidthGainEcg(value/2.0);
    parameter_data.aux_value.speedGain = value;

    emit publicData->clearLineSeries();
}

void LowSpeedData::clear_feedback(int value)
{
    clear_feedback_flag |= value;
    if (0x7 == clear_feedback_flag)
        emit reset_point_num();
}

void LowSpeedData::upgradeLowerMachine()
{
    QFile iabp_file;
    QList<int> iabp_list;

    iabp_file.setFileName("/opt/IABP_Process.bin");
    if (iabp_file.isOpen()) return;

    iabp_file.open(QIODevice::ReadOnly);

    QByteArray data = iabp_file.readAll();

    //qDebug() << "size-> " << data.size() << " date -> " << data.data();

    while(data.size())
    {
        uint16_t len = 249;  //'\xFF'

        if (len > data.size()) len = data.size();

        ass_serialport->sendUpgradeData(len, data.left(len).data());

        data.remove(0, len);
    }

    iabp_file.close();
}

QString LowSpeedData::getPrinterFileName()
{
    PRINTER_NUMBER printer_number;

    QDateTime data_time = QDateTime::currentDateTime();
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString current_time = QDateTime::currentDateTime().toString("HH-mm-ss");

    QFile fileInfo("/opt/print_num.txt");
    fileInfo.open(QFile::ReadWrite);

    QByteArray data = fileInfo.readAll();
    fileInfo.seek(0);
    printer_number.num = QString(data.data()[0]).toInt()+1;
    printer_number.days = data.data()+1;

    if (current_date != printer_number.days)
    {
        printer_number.num = 1;
        printer_number.days = current_date;
    }

    QTextStream out(&fileInfo);
    out << printer_number.num << printer_number.days;

    fileInfo.close();

    QString file_name = QString("/opt/pdf/printer_wave_%1_%2_%3.txt").arg(current_date).arg(current_time).arg(printer_number.num);

    return file_name;
}
