#include "serialport.h"

SerialPort::SerialPort()
{
    showSerialInfo();

    //serialPortInit("/dev/pts/3");
}

void SerialPort::showSerialInfo()
{
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        // 返回串行端口的名称
        qDebug() << "portName: " << info.portName();
        // 返回串行端口的系统位置
        qDebug() << "systemLocation: " << info.systemLocation();
        // 返回串行端口的描述字符串（如果可用）；否则返回空字符串
        qDebug() << "description: " << info.description();
        // 返回串行端口的制造商字符串(如果可用)；否则返回空字符串
        qDebug() << "manufacturer: " << info.manufacturer();
        // 返回串行端口的序列号字符串（如果可用）；否则返回空字符串
        qDebug() << "serialNumber: " << info.serialNumber();
        // 返回目标平台支持的可用标准波特率列表
        qDebug() << "vendorIdentifier: " << info.vendorIdentifier();
        // 返回串行端口的16位供应商编号（如果可用）；否则返回零
        qDebug() << "productIdentifier: " << info.productIdentifier();
        // 返回串行端口的16位产品号（如果可用）；否则返回零
        qDebug() << "standardBaudRates: " << info.standardBaudRates();
    }
}

// 串口初始化（参数配置）
void SerialPort::serialPortInit(const QString& name)
{
    setPortName(name);

    // 参数配置
    setBaudRate(QSerialPort::Baud115200);

    // 校验，校验默认选择无
    setParity(QSerialPort::NoParity);

    // 数据位，数据位默认选择8位
    setDataBits(QSerialPort::Data8);

    // 停止位，停止位默认选择1位
    setStopBits(QSerialPort::OneStop);

    // 控制流，默认选择无
    setFlowControl(QSerialPort::NoFlowControl);

    // 接受数据信号
    //connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(recvSerialPortData()));

    if(isOpen()) close();

    if(!open(QIODevice::ReadWrite))
    {
        qDebug() << "openError: " << error();
    }
    clear();
}

void SerialPort::recvSerialPortData()
{
    qDebug() << "signal readyRead --> recvSerialPortData";

    QByteArray info = readAll();
    QByteArray hexData = info.toHex();
    QString t0 = QString(hexData);
    qDebug() << "recvSerialPortData";
}

void SerialPort::sendSerialPortData()
{
    qDebug() << "signal readyWrite --> sendSerialPortData";

    write("test", 4);
    qDebug() << "123";
}

qint64 SerialPort::sendInitData(uint8_t type)
{
    const qint64 buff_size = 6;
    uint8_t buff[buff_size];

    buff[0] = HEAD_1;
    buff[1] = HEAD_2;
    buff[2] = type & '\x0F';
    buff[3] = buff_size;
    buff[4] = 0x01;
    buff[5] = MyCrc::my_crc8_check(buff, 0, buff_size-1);
    //buff[5] = '\n';

//    QByteArray tmp((const char*)buff, buff_size);
//    qDebug() << tmp.toHex(' ').toUpper();

    //发送串口数据到下位机
    return write((const char*)buff, buff_size);
}

qint64 SerialPort::sendParamData(uint8_t type, uint8_t aux, const P_DATA_s& p_data)
{
    qint64 buff_size = sizeof(uint8_t);
    uint8_t buff[32];

    buff[0] = HEAD_1;
    buff[1] = HEAD_2;
    buff[2] = type & '\x0F';

    if (aux == AUX_All)
    {
        buff_size = sizeof(P_DATA_s) - sizeof(Additional);
        ::memcpy(buff+5, &p_data, buff_size);
    }
    else if (aux == AUX_RM_TYPE)
        buff[5] = p_data.run_mode;
    else if (aux == AUX_IT_TYPE)
        buff[5] = p_data.time >> 8;
    else if (aux == AUX_DT_TYPE)
        buff[5] = p_data.time & 0xff;
    else if (aux == AUX_IP_TYPE)
        buff[5] = p_data.aug_data;
    else if (aux == AUX_AP_TYPE)
        buff[5] = p_data.threshold;
    else if (aux == AUX_RT_TYPE)
        buff[5] = p_data.rtrac_mode;
    else if (aux == AUX_TR_TYPE)
        buff[5] = p_data.trigger_rate;
    else if (aux == AUX_TM_TYPE)
        buff[5] = p_data.trigger_mode;
    else if (aux == AUX_TS_TYPE)
        buff[5] = p_data.trigger_source;

    buff_size += 7;
    buff[3] = buff_size;
    buff[4] = aux;

    buff[buff_size-2] = 0x01;
    buff[buff_size-1] = MyCrc::my_crc8_check(buff, 0, buff_size-1);
    //buff[5] = '\n';

    //发送串口数据到下位机
    return write((const char*)buff, buff_size);
}

qint64 SerialPort::sendControlData(uint8_t type, uint8_t aux)
{
    const qint64 buff_size = 7;
    uint8_t buff[buff_size];

    buff[0] = HEAD_1;
    buff[1] = HEAD_2;
    buff[2] = type & '\x0F';
    buff[3] = buff_size;
    buff[4] = aux;
    buff[5] = 0x01;
    buff[6] = MyCrc::my_crc8_check(buff, 0, buff_size-1);
    //buff[5] = '\n';

    //发送串口数据到下位机
    return write((const char*)buff, buff_size);
}

//4811012055020008d5510008f5410008d15100085d0800089561000800000000000
void SerialPort::sendUpgradeData(uint16_t len, const char* data)
{
    qint64 buff_size = 6 + len;
    uint8_t buff[256] = {0};

    buff[0] = HEAD_1;
    buff[1] = HEAD_2;
    buff[2] = '\x0E';
    buff[3] = buff_size;

    ::memcpy(buff+4, data, len);
    buff[buff_size-2] = 0x01;
    buff[buff_size-1] = MyCrc::my_crc8_check(buff, 0, buff_size-1);

//    for(uint16_t i = 0; i < buff_size; i++)
//    {
//        qDebug() << "left-> " << (buff[i] >> 4 & 0xf) << "right-> " << (buff[i] & 0xf);
//    }

    //发送串口数据到下位机
    write(data, buff_size);
}
