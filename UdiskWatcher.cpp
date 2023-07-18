#include "UdiskWatcher.h"

// 定义设备挂载U盘的最大个数
#define USB_COUNT 10

USBDeviceInfo::USBDeviceInfo()
    : strDev(""), strUUID(""), strLABEL("")
    , strTYPE(""), isBusy(false)
{

}

UdiskWatcher::UdiskWatcher(QObject *parent)
    : QObject(parent), m_systemWatcher(nullptr)
{
    m_systemWatcher = new QFileSystemWatcher(this);
    if (!m_systemWatcher->addPath("/dev"))
    {
        qDebug() << "fileSystemWatcher invalid path!";
    }

    // 由于QFileSystemWathcher初次开机时检测不到已经存在的U盘设备节点，所以不会挂载U盘；
    // 所以选择在UdiskWatcher构造的时候尝试检测是否插入U盘，挂载一次U盘
    udiskHandle();

    connect(m_systemWatcher, &QFileSystemWatcher::directoryChanged, this, &UdiskWatcher::udiskHandle);
}

void UdiskWatcher::udiskHandle()
{
    refreshUSBdeviceList();

    int count = 0;

    int devListSize = m_usbDevList.size();
    for (int i = 0; i < devListSize; i++)
    {
        if (m_usbDevListMounted.contains(m_usbDevList.at(i)))
            count++;
    }

    qDebug("count = %d , devListSize = %d , m_usbDevListMounted.size() = %d", count, devListSize, m_usbDevListMounted.size());

    if ((count == m_usbDevListMounted.size()) && (count == devListSize))        
        return; // 两个list相等，不需要操作

    // 刷新U盘设备节点列表后，新增的U盘设备列表
    QList<USBDeviceInfo> usbDevListAdded;
    // 刷新U盘设备节点列表后，移除的U盘设备列表
    QList<USBDeviceInfo> usbDevListRemoved;

    usbDevListAdded.clear();
    usbDevListRemoved.clear();

    for (int j = 0; j < m_usbDevListMounted.size(); j++)
    {
        if (!m_usbDevList.contains(m_usbDevListMounted.at(j)))
        {
            usbDevListRemoved.append(m_usbDevListMounted.at(j));
        }
    }

    //qDebug("need to umount usb count = %d", usbDevListRemoved.size());
    // 卸载已经移除的U盘设备
    for (int j = 0; j < usbDevListRemoved.size(); j++)
        umountUsb(usbDevListRemoved.at(j));

    if (m_usbDevListMounted.size() < USB_COUNT)
    {
        for (int i = 0; i < devListSize; i++)
        {
            if (!m_usbDevListMounted.contains(m_usbDevList.at(i)))
            {
                usbDevListAdded.append(m_usbDevList.at(i));
            }
        }

        qDebug("need to mount usb count = %d", usbDevListAdded.size());

        // 挂载新增的U盘设备
        for (int i = 0; i < usbDevListAdded.size(); i++)
        {
            mountUsb(usbDevListAdded.at(i));
        }
    }
}

bool UdiskWatcher::mountUsb(USBDeviceInfo dev)
{
    qDebug("DeviceManager::mountUsb\n");

#ifdef Q_OS_LINUX
    QString output;
    QString usbName;

    // 只有U盘是ntfs类型的时候，中文卷标才能正常显示；所以当U盘类型是fat32或者其他类型的时候，显示UUID;这里与ubuntu的处理是一样的；
    if (dev.strTYPE == "ntfs" && !dev.strLABEL.isEmpty())
    {
        usbName = "/mnt/" + dev.strLABEL;
    }
    else
    {
        usbName = "/mnt/" + dev.strUUID;
    }

    if (!QFile::exists(usbName))
    {
        if (!myProcess("mkdir", QStringList() << "-p" << usbName, output))
        {
            qDebug() << "mkdir usb derectory failed!";
            return false;
        }
    }

    if (!myProcess("/bin/mount", QStringList() << "-o" << "sync" << "-o" << "shortname=mixed" << "-o" << "utf8=yes" << dev.strDev << usbName, output))
    {
        qDebug() << "mount usb failed!";
        return false;
    }

//    if (!myProcess("touch", QStringList() << usbName, output))
//    {
//        qDebug() << "touch usb derectory failed!";
//        return false;
//    }

    m_usbDevListMounted.append(dev);

    movePdfFileToUdisk(usbName);

    qDebug("DeviceManager::mountUsb success! m_usbDevListMounted.size = %d\n", m_usbDevListMounted.size());
    return true;
#else
    Q_UNUSED(dev)
    return false;
#endif
}

void UdiskWatcher::movePdfFileToUdisk(const QString& dst)
{
    QString output;
    QDir dir("/opt/pdf");
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo f, fileList)
    {
        // "."和".."跳过
        if (f.baseName() == "")
            continue;

        if (f.suffix() == "pdf")
        {
            myProcess("/bin/mv", QStringList() << f.absoluteFilePath() << dst, output);
            dir.remove(f.absoluteFilePath());
        }
    }
}

bool UdiskWatcher::umountUsb(USBDeviceInfo dev)
{
    qDebug("DeviceManager::umountUsb\n");

#ifdef Q_OS_LINUX

    QString output;
    QString usbName;
    if (dev.strTYPE == "ntfs" && !dev.strLABEL.isEmpty())
    {
        usbName = "/mnt/" + dev.strLABEL;
    }
    else
    {
        usbName = "/mnt/" + dev.strUUID;
    }
    if (!QFile::exists(usbName))
    {
        qDebug() << "derectory has not exist!";
        return true;
    }
    if (!myProcess("/bin/umount", QStringList() << usbName, output))
    {
        qDebug() << "umount usb failed!";
        return false;
    }
    if (!myProcess("rmdir", QStringList() << usbName, output))
    {
        qDebug() << "rmdir usb derectory failed!";
        return false;
    }

    m_usbDevListMounted.removeAll(dev);
    qDebug("DeviceManager::umountUsb success! m_usbDevListMounted.size = %d\n", m_usbDevListMounted.size());
    return true;

#else
    return false;
#endif
}

void UdiskWatcher::refreshUSBdeviceList()
{
#ifdef Q_OS_LINUX

    m_usbDevList.clear();

    QString output;
    //myProcess("blkid", QStringList() << "-d" << "-c" << "/dev/null", output);
    myProcess("blkid", output);

    // 正则表达式，取出U盘设备节点列表
    QStringList itemList;
    qDebug() << "==============" << output;

    myRegExp("/dev/sd[a-z].*\\n", output, itemList);

    for (int i = 0; i < itemList.size(); i++)
    {
        USBDeviceInfo info;
        QStringList tempList;
        if (myRegExp("/dev/sd[a-z][1-9]", itemList.at(i), tempList) > 0)
        {
            info.strDev = tempList.at(0);
            tempList.clear();
        }
        if (myRegExp("LABEL=\".*\"", itemList.at(i), tempList) > 0)
        {
            int size = tempList.at(0).size();
            info.strLABEL = tempList.at(0).mid(7, (size-8));
            tempList.clear();
        }
        if (myRegExp("UUID=\".*\"", itemList.at(i), tempList) > 0)
        {
            int size = tempList.at(0).size();
            info.strUUID = tempList.at(0).mid(6, (size-7));
            tempList.clear();
        }
        if (myRegExp("TYPE=\".*\"", itemList.at(i), tempList) > 0)
        {
            int size = tempList.at(0).size();
            info.strTYPE = tempList.at(0).mid(6, (size-7));
            tempList.clear();
        }
        m_usbDevList.append(info);
    }
    if (m_usbDevList.size() < 1)
    {
        qDebug() << "No USB Device Detected!";
        return;
    }
#endif
}

int UdiskWatcher::myRegExp(const QString pattern, const QString &text, QStringList &list)
{
    QRegExp regExp;
    int capCount = 0;
    int pos = 0;

    if (pattern.isEmpty() || text.isEmpty())
    {
        return -1;
    }
    regExp.setMinimal(true);
    regExp.setPattern(pattern);
    regExp.indexIn(text);

    list.clear();
    while ((pos = regExp.indexIn(text, pos)) != -1)
    {
        list.push_back(regExp.cap(0));
        pos += regExp.matchedLength();
        capCount++;
    }

    return capCount;
}

bool UdiskWatcher::myProcess(QString cmd, QString &output)
{
    QProcess process;
    process.start(cmd);
    // 等待进程启动
    if (!process.waitForStarted())
    {
        qDebug() <<  "command execution failed to start!";
        return false;
    }
    process.closeWriteChannel();

    while (!process.waitForFinished(3000))
    {
        qDebug() <<  "Command execution end failed!";
        return false;
    }

    // 进程的控制台输出
    output = process.readAll();

    return true;
}

bool UdiskWatcher::myProcess(QString cmd, QStringList param, QString &output)
{
    QProcess process;
    process.start(cmd, param);
    // 等待进程启动
    if (!process.waitForStarted())
    {
        qDebug() <<  "command execution failed to start!";
        return false;
    }
    process.closeWriteChannel();

    while (!process.waitForFinished(300000))
    {
        qDebug() <<  "Command execution end failed!";
        return false;
    }

    // 进程的控制台输出
    output = process.readAll();

    return true;
}
