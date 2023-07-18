#ifndef HIGHSPEEDDATA_H
#define HIGHSPEEDDATA_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFile>
#include <QTextCodec>
#include <QFileInfo>
#include <QSharedPointer>

#include "serialport.h"

class HighSpeedData : public QObject
{
    Q_OBJECT
public:
    explicit HighSpeedData();

    ~HighSpeedData();

    void setBaseDataPointer(int fd, QSharedPointer<PublicData> obj);

    void rawDataWriteFile(const QByteArray& buf);

signals:

    void invoke_signal();

    void thread_status_change();

    void ass_serial_init();

private slots:

    bool ecg_control(bool flag);

    bool shake_init();

    void read_data_process();

private:

    qint64 shakeHandsProcess(bool flag, const QByteArray& rdata = "");

    qint64 ecgControlProcess(bool flag, const QByteArray& rdata = "");

private:

    int wfd;

    QThread thread;

    SerialPort *ecg_serialport;

    QSharedPointer<PublicData> publicData;

    QFile file;

};

#endif // HIGHSPEEDDATA_H
