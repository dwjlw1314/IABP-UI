#ifndef RECEIVEDATA_H
#define RECEIVEDATA_H

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QtDebug>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "publicdata.h"

class ReceiveData : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveData();

    bool initNameFifoObj(const QString& fifo_name, QSharedPointer<PublicData> obj);

signals:

    void ecgControlSignal(bool);

public slots:

private:

    virtual void run();

    void ParseFifoData(const FIFO_DATA& fifo_data);

    QSharedPointer<PublicData> publicData;
};

#endif // RECEIVEDATA_H
