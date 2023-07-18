#include "highspeeddata.h"

HighSpeedData::HighSpeedData() : ecg_serialport(nullptr)
{
    this->moveToThread(&thread);
    thread.setObjectName("HighSpeedData");
    thread.start();

    //Turn it on immediately after startup
    file.setFileName("/opt/ecg.txt");
    //file.open(QIODevice::WriteOnly|QFile::Truncate);

    connect(this, SIGNAL(invoke_signal()), this, SLOT(shake_init()));
}

HighSpeedData::~HighSpeedData()
{
    /** Release source */
}

bool HighSpeedData::shake_init()
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(HS_CPU,&mask);

    if(pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
        qDebug() << "HighSpeedData->pthread_setaffinity_np error!";

    if (!ecg_serialport)
    {
        /** @todo 构造 心电,血压,球囊压力 串口 */
        ecg_serialport = new SerialPort;
        ecg_serialport->serialPortInit(ECG_SERIAL_NAME);
    }

    for(int i = 0, j = 10; i < 3; i++)
    {
        ecg_serialport->clear();
        /** @todo 处理心电串口握手 */
        if (ERROR == shakeHandsProcess(true))
            continue;

        QByteArray buf;
        while (j--)
        {
            if (0 == j)
            {
                publicData->sendAlarmMessage(ErrorType, "ECG Running");
                return false;
            }

            if (ecg_serialport->waitForReadyRead(3000))
                buf += ecg_serialport->readAll();
            else  //timeout
            {
                publicData->sendAlarmMessage(ErrorType, "ECG ShakeHandsProcess Timeout");
                return false;
            }

            if (ERROR == shakeHandsProcess(false, buf))
                continue;

            emit ass_serial_init();

            return true;
        }
    }

    return false;
}

void HighSpeedData::setBaseDataPointer(int fd, QSharedPointer<PublicData> obj)
{
    wfd = fd;
    publicData = obj;
}

qint64 HighSpeedData::shakeHandsProcess(bool flag, const QByteArray& rdata)
{
    if (flag)
    {
        /** @todo sender */
        return ecg_serialport->sendInitData(COMMAND_HS);
    }
    else
    {
        /** @todo recv proc */
        qDebug() << "HighSpeedData::shakeHandsProcess: " << rdata.toHex(' ').toUpper();

        if (rdata.size() != COMMAND_LEN)
        {
            publicData->sendAlarmMessage(ErrorType, "ECG_HS_LEN ERROR");
            return ERROR;
        }

        if (rdata[4] == 0x01)
        {
            publicData->sendAlarmMessage(ErrorType, "ECG_HS STATUS ERROR");
            return ERROR;
        }

        return COMMAND_LEN;
    }
}

bool HighSpeedData::ecg_control(bool flag)
{
    /** @todo 处理心电数据控制 */
    for(int i = 0; i < 3; i++)
    {
        if (flag)
        {
            if(ERROR == ecg_serialport->sendControlData(COMMAND_EG, AUX_All))
                continue;
            else
                break;
        } 
    }

    if (!flag) return true;

    emit thread_status_change();

    connect(ecg_serialport, SIGNAL(readyRead()), this, SLOT(read_data_process()), Qt::UniqueConnection);

    return true;
}

void HighSpeedData::read_data_process()
{
    QByteArray buf;

    buf = ecg_serialport->readAll();
    PublicData::mutex.lock();
    publicData->ecg_com_queue.enqueue(buf);
    PublicData::mutex.unlock();

    //qDebug() << "send.size() -> " << buf.size();

    rawDataWriteFile(buf);

    //将同步数据写入到sync_fd[0]
    write(wfd, "A", 1);

    return;
}

void HighSpeedData::rawDataWriteFile(const QByteArray& buf)
{
    if (!publicData->enable_write_file)
    {
        if (file.isOpen())
        {
            file.flush();
            file.close();
            QString current_dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd.hh:mm:ss");
            QString filename = QString("/opt/log/%1.ecg.txt").arg(current_dateTime);
            file.copy(filename);
        }
        return;
    }
    else
    {
        if (!file.isOpen())
            file.open(QFile::WriteOnly|QFile::Truncate);
    }

    QTextStream out(&file);  //创建写入流
    out << buf.toHex(' ').toUpper();//写入数据
    out << ' ';
}

