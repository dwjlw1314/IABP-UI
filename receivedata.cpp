#include "receivedata.h"

ReceiveData::ReceiveData()
{
    /** @brief NO INIT */
}

/** @todo 处理控制界面指令 */
void ReceiveData::run()
{
    qDebug() << "ReceiveData ThreadID: " << QThread::currentThreadId();

    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0)
    {
        qDebug() << "read fifo open err";
        return;
    }

    qDebug() << "read fifo open success";

    while(1)
    {
        char readbuf[32]={0};

        FIFO_DATA fifo_data;

        ssize_t ret = read(fd,readbuf,sizeof(readbuf));
        if(ret < 0)
        {
            qDebug() << "readbuf error";
            continue;
        }

        ::memcpy(&fifo_data, readbuf, sizeof(FIFO_DATA));

        qDebug() << "recv fifo buf->" << fifo_data.cmd_type;

        ParseFifoData(fifo_data);
    }

}

void ReceiveData::ParseFifoData(const FIFO_DATA& fifo_data)
{
    fifo_data.cmd_type & 0x01 ? emit ecgControlSignal(true) : emit ecgControlSignal(false);
}

bool ReceiveData::initNameFifoObj(const QString& fifo_name, QSharedPointer<PublicData> obj)
{
    publicData = obj;

    QFileInfo fileInfo(fifo_name);
    if(fileInfo.exists())
    {
        return true;
    }

    /* fifo_name 文件名字 ; 0777 文件权限 */
    int ret = mkfifo(fifo_name.toStdString().c_str(), 0777);
    if(ret < 0)
    {
        obj->setIndications("creat myfifo error");
        return false;
    }

    //创建管道文件成功
    qDebug() << "creat myfifo success";
    return true;
}
