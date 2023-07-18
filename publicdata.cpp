#include "publicdata.h"

QMutex PublicData::mutex;
Logger PublicData::logger;
QSemaphore PublicData::semaphore(1);
QByteArray PublicData::protocol_head;

PublicData::PublicData(QObject *parent) : QObject(parent)
{
    /** @todo init public object */
    protocol_head[0] = '\xAA';
    protocol_head[1] = '\xAA';

    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sync_fd) < 0)
        qDebug() << "sync socker create error";

    //获取文件的flags值
    int fla= fcntl(sync_fd[1], F_GETFL, 0);
    //设置成非阻塞模式
    fcntl(sync_fd[1], F_SETFL, fla | O_NONBLOCK);

    print_flag = false;
    internal_press = true;
    ecg_gain_mode = true;
    ecg_height_gain = 0;
    enable_write_file = false;
    show_all_lead_wave = false;
    freeze_display_status = false;
}

void PublicData::setCpuAffinity(int core_nums)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    for(int i = 0; i < core_nums; i++)
        CPU_SET(i, &mask);

    if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) < 0)
        qDebug() << "sched_setaffinity error!";

    //usleep(500000); /* 让当前的设置有足够时间生效*/
}

int PublicData::getParamerValue(int type, P_DATA_s value)
{
    qDebug() << type << "--" << value.aug_data;
    return value.aug_data;
}

int PublicData::getReadSocketfd()
{
    return sync_fd[0];
}

int PublicData::getWriteSocketfd()
{
    return sync_fd[1];
}

void PublicData::setFreezeDisplay(bool value)
{
    freeze_display_status = value;
}

void PublicData::initGainEcg(bool flag)
{
    if (flag)
    {
        plotArea[0] = ecg_all_plotArea[0];
        plotArea[1] = ecg_all_plotArea[1]/7;
    }
    else
    {
        plotArea[0] = ecg_plotArea[0];
        plotArea[1] = ecg_plotArea[1];
    }

    pixel_1mv = plotArea[1] / MAX_DIM_ECG;

    initHeightGainEcg(2.0); //m_ecg_dim_value
    initWidthGainEcg(1.0); //m_ecg_speed_value
}

void PublicData::setEcgPlotAttr(int w, int h)
{
    ecg_plotArea[0] = w;
    ecg_plotArea[1] = h;
}

void PublicData::setAllEcgPlotAttr(int w, int h)
{
    ecg_all_plotArea[0] = w;
    ecg_all_plotArea[1] = h;
}

void PublicData::setBalloonPlotAttr(int w, int h)
{
    plotArea_balloon[0] = w;
    plotArea_balloon[1] = h;

    pixel_1mmHg = plotArea_balloon[1] / (GAIN_BALLOON_DEV * 2);

    //initGainBalloon(380.0); //m_balloon_value > 380mmHg run once
    //initGainBalloon(27000.0); //test
}

void PublicData::setArterialPlotAttr(int w, int h)
{
    qDebug() << "setArterialPlotAttr->" << w << h;
    return;
}

void PublicData::initGainBalloon(qreal realValue)
{
    h_gain_balloon = GAIN_BALLOON_DEV / realValue;
}

void PublicData::initHeightGainEcg(qreal dimValue)
{
    qDebug() << "initHeightGainEcg->" << dimValue;
    h_gain_ecg = MAX_DIM_ECG / dimValue;
}

void PublicData::initWidthGainEcg(qreal speedValue)
{
    qDebug() << "initWidthGainEcg->" << speedValue;
    w_gain_ecg = GAIN_25MM_PIXEL * speedValue / GAIN_ECG_SAM;
}

qreal PublicData::getBalloonHeightRatio()
{
    return h_gain_balloon * pixel_1mmHg;
}

void PublicData::setEcgHeightFilter(int value)
{
    Keyhp = value ? 1 : 0;
}

void PublicData::setEcgLowFilter(int value)
{
    Keylp = value;
}

void PublicData::setEcgHarFilter(int value)
{
    Keynotch50 = value ? 1 : 0;
}
