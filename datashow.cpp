#include "datashow.h"
#include <QTime>

//#define TEST_TIMER

DataShow::DataShow(QObject *parent) : QObject(parent),
    point_num(0), once_points(0), data_refresh_time(nullptr)
{
    this->moveToThread(&thread);
    thread.setObjectName("DataShow");
    thread.start();

    show_balloon_value = false;
}

void DataShow::setBaseDataPointer(QSharedPointer<PublicData> obj)
{
    publicData = obj;
}

void DataShow::show_timer_change()
{
    qDebug() << "DataShow ECG ThreadID: " << QThread::currentThreadId();

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(DS_CPU,&mask);

    if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &mask) < 0)
        qDebug() << "DataShow->pthread_setaffinity_np error!";

    /* 高精度对cpu资源消耗高 */
    //data_refresh_time.setTimerType(Qt::PreciseTimer);
    if (!data_refresh_time)
        data_refresh_time = new QTimer;

    /* 信号槽函数连接,只连接一次 */
#ifndef TEST_TIMER
    connect(data_refresh_time, SIGNAL(timeout()), this, SLOT(data_refresh_timeout()));
#else
    connect(&data_refresh_time, SIGNAL(timeout()), this, SLOT(test_timeout()));
    data_refresh_time.start(20);
#endif

    if (!data_refresh_time->isActive())
        data_refresh_time->start(REFRESH_TIME);
}

void DataShow::show_balloon_change(bool flag)
{
    qDebug() << "show_balloon_change->" << flag;

    publicData->balloonStatusChange(0.0);
    publicData->balloonMaxValue("--");
    show_balloon_value = flag;
}

void DataShow::reset_point_num()
{
    point_num = 0;
    publicData->freeze_display_status = false;
}

/* QTime::currentTime()  */
void DataShow::data_refresh_timeout()
{
    //qDebug() << "DataShow ECG ThreadID: " << QThread::currentThreadId();
    thread_local bool show_all_wave = false;
    thread_local int pre_index = -1;
    thread_local QList<qreal> ecg_value;
    thread_local QList<qreal> balloon_value;
    thread_local QList<qreal> arterial_value;

    ecg_value.clear();
    balloon_value.clear();
    arterial_value.clear();

    //qreal balloon_height_ratio = publicData->getBalloonHeightRatio();
    //qDebug() << "DataShow ECG HW_ratio: " << height_ratio << ":" << width_ratio;

    PublicData::semaphore.acquire();

    while(true)
    {
        if (once_points++ >= 5)
        {
            once_points = 0;
            break;
        }

        if (!publicData->ecg_show_queue.empty())
        {
            /* 出队一个数据 */
            ECG_SHOW_DATA ecg_data = publicData->ecg_show_queue.dequeue();

            if (ecg_data.ecg_All)
            {
                show_all_wave = true;
                ecg_value << publicData->plotArea[1] * 3 + ecg_data.ecg_I;
                ecg_value << publicData->plotArea[1] * 2 + ecg_data.ecg_II;
                ecg_value << publicData->plotArea[1] + ecg_data.ecg_III;
                ecg_value << ecg_data.ecg_V1;
                ecg_value << ecg_data.ecg_aVF - publicData->plotArea[1];
                ecg_value << ecg_data.ecg_aVL - publicData->plotArea[1] * 2;
                ecg_value << ecg_data.ecg_aVR - publicData->plotArea[1] * 3;
//                ecg_value << publicData->plotArea[1] * 3 + ecg_data.ecg_II;
//                ecg_value << publicData->plotArea[1] * 2 + ecg_data.ecg_II;
//                ecg_value << publicData->plotArea[1] + ecg_data.ecg_II;
//                ecg_value << ecg_data.ecg_II;
//                ecg_value << ecg_data.ecg_II - publicData->plotArea[1];
//                ecg_value << ecg_data.ecg_II - publicData->plotArea[1] * 2;
//                ecg_value << ecg_data.ecg_II - publicData->plotArea[1] * 3;
            }
            else
            {
                show_all_wave = false;
                ecg_value << ecg_data.trigger_state << ecg_data.ecgData;
            }

            arterial_value << ecg_data.ap_press;

            //balloon_value << ecg_data.ball_data;
            balloon_value << ecg_data.ball_data;

            if (!show_balloon_value) ;
            else if (ecg_data.trigger_state & '\x02')  //fill
            {
                qDebug() << "trigger_state ->" << ecg_data.trigger_state;

                publicData->balloonStatusChange(1.0);
                publicData->balloonMaxValue(QString::number(ecg_data.ball_data));
            }
            else if (ecg_data.trigger_state & '\x01')  //clear
            {
                qDebug() << "trigger_state ->" << ecg_data.trigger_state;

                publicData->balloonStatusChange(0.0);
            }

            //publicData->setDebugRightValue(ecg_data.ball_data);
        }
        else
        {
            ecg_value << 0.0 << 0.0;
            balloon_value << GAIN_BALLOON_DEV;
            arterial_value << GAIN_ARTERIAL_DEV;
        }

        if (publicData->freeze_display_status)
            continue;

        axisX = static_cast<int>(publicData->w_gain_ecg * point_num);

        bool draw_method = true;
        int start_end = 2;
        if (0 == point_num && 0 == axisX)
        {
            start_end = 0; //0->start
        }
        else if (axisX >= publicData->plotArea[0])
        {
            axisX = static_cast<int>(publicData->plotArea[0]-1);
            start_end = 1; //1->end
            point_num = -1;
        }

        if (pre_index != axisX)
            pre_index = axisX;
        else
            draw_method = false;  //replace

        point_num++;

        //qDebug() << "axisX ->" << axisX << "draw_method->" << draw_method;

        ecg_value << start_end << draw_method << axisX;
        balloon_value << start_end << draw_method << axisX;
        arterial_value << start_end << draw_method << axisX;
    }

    PublicData::semaphore.release();

    if (publicData->freeze_display_status)
        return;

    /** @todo sender ECG pointer To QML */
    if (show_all_wave)
    {
        publicData->ecgAllQueueChange(ecg_value);
    }
    else
    {
        publicData->ecgQueueChange(ecg_value);
        publicData->arterialQueueChange(arterial_value);
        publicData->balloonQueueChange(balloon_value);
    }
}
