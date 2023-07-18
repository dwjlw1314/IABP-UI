#ifndef DATASHOW_H
#define DATASHOW_H

#include <QObject>
#include <QThread>

#include "publicdata.h"

class DataShow : public QObject
{
    Q_OBJECT
public:
    explicit DataShow(QObject *parent = nullptr);

    ~DataShow() { if (data_refresh_time) delete data_refresh_time; }

    void setBaseDataPointer(QSharedPointer<PublicData> obj);

signals:

public slots:

    void data_refresh_timeout();

    void show_timer_change();

    void show_balloon_change(bool flag);

    void reset_point_num();

#ifdef TEST_TIMER
    void test_timeout();
#endif

private:

    int point_num;

    int axisX;

    int once_points;

    int placeholder;

    QThread thread;

    QTimer *data_refresh_time;

    QSharedPointer<PublicData> publicData;

    volatile bool show_balloon_value;
};

#endif // DATASHOW_H
