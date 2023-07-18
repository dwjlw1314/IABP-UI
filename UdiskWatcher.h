#ifndef UDISKWATCHER_H
#define UDISKWATCHER_H

#include <QFileSystemWatcher>

#include "publicdata.h"

class USBDeviceInfo
{

public:

    USBDeviceInfo();

    bool operator==(const USBDeviceInfo &value)
    {
        return (strUUID == value.strUUID);
    }

public :
    QString strDev;
    QString strUUID;
    QString strLABEL;
    QString strTYPE;
    QString isBusy;
};

class UdiskWatcher : public QObject
{
    Q_OBJECT

public:

    explicit UdiskWatcher(QObject *parent = nullptr);

    void udiskHandle();

    void refreshUSBdeviceList();

    bool mountUsb(USBDeviceInfo dev);

    bool umountUsb(USBDeviceInfo dev);

    void movePdfFileToUdisk(const QString& dst);

    bool myProcess(QString cmd, QStringList param, QString &output);
    bool myProcess(QString cmd, QString &output);

    int myRegExp(const QString pattern, const QString &text, QStringList &list);

    QList<USBDeviceInfo> getCurrentUSBDevice()
    {
        return m_usbDevListMounted;
    }

    void setBaseDataPointer(QSharedPointer<PublicData> obj)
    {
        publicData = obj;
    }

signals:

private :

    QFileSystemWatcher *m_systemWatcher;

    QList<USBDeviceInfo> m_usbDevList;

    QList<USBDeviceInfo> m_usbDevListMounted;

    QSharedPointer<PublicData> publicData;
};

#endif // UDISKWATCHER_H
