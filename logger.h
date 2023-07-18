#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QTextStream>
#include <QDateTime>

static QMutex fileMutex;
static QFile logFile;

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

    // 检查日志文件的大小
    void checkLogFilesSize() = delete;

    void setDeleteDays(int value) { days = value; }

    static void myMessageOutput(QtMsgType, const QMessageLogContext&, const QString&);

signals:

public slots:

    // 自动删除N天前日志
    void autoDeleteLog();

private:

    int days;
};

#endif // LOGGER_H
