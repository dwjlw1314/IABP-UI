#include "logger.h"
#include "publicdata.h"

Logger::Logger(QObject *parent) : QObject(parent)
{
    days = 5;  //defalut

    QDir dir;
    if(!dir.exists("/opt/log"))
        dir.mkdir("/opt/log");

    if(!dir.exists("/opt/pdf"))
        dir.mkdir("/opt/pdf");

    autoDeleteLog();
#ifndef CONSOLE
    qInstallMessageHandler(Logger::myMessageOutput);
#endif
}

void Logger::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    fileMutex.lock();
    QString text = "";
    switch (type)
    {
        case QtInfoMsg:
            text = QString("Info:");
            break;
        case QtDebugMsg:
            text = QString("Debug:");
            break;
        case QtWarningMsg:
            text = QString("Warning:");
            break;
        case QtCriticalMsg:
            text = QString("Critical:");
            break;
        case QtFatalMsg:
            text = QString("Fatal:");
            break;
        default:
            text = QString("...");
            break;
    }
    QString context_info = QString("%1:%2").arg(QString(context.file)).arg(context.line);
    QString current_dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_data = QString("%1").arg(current_dateTime);
    QString message = QString("%1(%2) %3 %4").arg(current_data).arg(text).arg(context_info).arg(msg);
    QString filename = QString("/opt/log/%1.log").arg(current_dateTime.left(10));

    if (!QFile::exists(filename))
    {
        if (logFile.isOpen())
            logFile.close();
        logFile.setFileName(filename);
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    {
        if (!logFile.isOpen())
        {
            logFile.setFileName(filename);
            logFile.open(QIODevice::WriteOnly | QIODevice::Append);
        }

        QTextStream text_stream(&logFile);
        text_stream << message << "\r\n";
    }

    logFile.flush(); //操作日志比较重要，每次刷新缓存

    fileMutex.unlock();
}

void Logger::autoDeleteLog()
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime time1 = now.addDays(0-days);
    QDateTime time2;
    QDir dir("/opt/log");
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo f, fileList)
    {
        // "."和".."跳过
        if (f.baseName() == "")
            continue;

        time2 = QDateTime::fromString(f.baseName(), "yyyy-MM-dd");
        if (time2 < time1)
        {
            //只要日志时间小于前days天的时间就删除
            dir.remove(f.absoluteFilePath());
        }
    }
}
