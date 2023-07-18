#ifndef SINGLESERIALPORT_H
#define SINGLESERIALPORT_H

/* https://blog.csdn.net/weixin_43194657/article/details/126148729 */

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "publicdata.h"
#include "mycrc.h"

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:

    explicit SerialPort();

    void serialPortInit(const QString& name);

    qint64 sendInitData(uint8_t type);
    qint64 sendControlData(uint8_t type, uint8_t aux);
    qint64 sendParamData(uint8_t type, uint8_t aux, const P_DATA_s& p_data);

    void sendUpgradeData(uint16_t len, const char* data);

    //receivers返回的是该信号的连接数，如果大于0则为信号有连接
    inline bool checkSignalStatus()
    {
        return receivers(SIGNAL(readyRead())) > 0;
    }

signals:

public slots:

    void recvSerialPortData();

    void sendSerialPortData();

private:

    void showSerialInfo();

};

#endif // SINGLESERIALPORT_H
