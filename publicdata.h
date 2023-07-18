#ifndef PUBLICDATA_H
#define PUBLICDATA_H

#include <QtGlobal>

#include <QObject>
#include <QQueue>
#include <QDebug>
#include <QMutex>
#include <QTimer>
#include <QMetaType>
#include <QSemaphore>
#include <QByteArray>
#include <QFile>
#include <QProcess>
#include <QRegExp>

#ifdef Q_OS_WIN
#include <WS2tcpip.h>
#endif

#ifdef Q_OS_LINUX
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#endif

#include "macro.h"
#include "logger.h"
#include "datastruct.h"

class PublicData : public QObject
{
    Q_OBJECT

    //Q_DECLARE_METATYPE(ALARM)
    //Q_PROPERTY(QString indications READ Indications NOTIFY indicationsChange)
    Q_PROPERTY(bool ecg_queue_status READ ecgQueueStatus)

public:

    static QByteArray protocol_head;
    static QSemaphore semaphore;
    static QMutex mutex;
    static Logger logger;

    Q_INVOKABLE int getParamerValue(int type, P_DATA_s value);
    //Q_INVOKABLE bool postMessage(const QString &msg);

    explicit PublicData(QObject *parent = nullptr);

    int getReadSocketfd();
    int getWriteSocketfd();

    bool ecgQueueStatus() const { return m_ecg_queue_status; }
    void setEcgQueueStatus(bool value) { m_ecg_queue_status = value; }

    qreal getEcgWidthRatio();
    void initHeightGainEcg(qreal dimValue);
    void initWidthGainEcg(qreal speedValue);

    qreal getBalloonHeightRatio();
    void initGainBalloon(qreal realValue);

public:

    QQueue<ECG_SHOW_DATA> ecg_pre_sec_queue;
    QQueue<ECG_SHOW_DATA> ecg_printer_queue;
    QQueue<ECG_SHOW_DATA> ecg_show_queue;
    QQueue<QByteArray> ecg_com_queue;
    PRINTER_BASE_DATA printer_base_data;

    /* m_plot[0] = width */
    qreal plotArea[2];
    qreal plotArea_balloon[2];
    qreal h_gain_ecg;
    qreal w_gain_ecg;

    int point_num = 1;
    int aug_alarm_threshod;

    int16_t Keyhp;  //是否打开高通 1是打开
    int16_t Keylp;  //是否打开低通 1是打开40Hz，2是电刀模式
    int16_t Keynotch50;  //是否打开工频干扰 1是打开

    int16_t ecg_height_gain;

    uint16_t savePrePoint;  //保存前N秒的点数

    bool ecg_gain_mode;  //auto=true
    bool print_flag;    //default false

    volatile bool show_all_lead_wave;

    volatile bool freeze_display_status;
    volatile bool enable_write_file;
    volatile bool internal_press;

signals:

    /* 血压接口 */
    void arterialQueueChange(QList<qreal> arterial_value);
    void arterialValueChange(QList<int> ap_value);

    /* 球囊接口 */
    void balloonQueueChange(QList<qreal> balloon_value);
    void balloonStatusChange(qreal progressBarValue);
    void balloonMaxValue(QString maxValue);

    /* 心电接口 */
    void ecgQueueChange(QList<qreal> ecg_value);
    void indicationsChange();
    void ecgHeartBeatChange(QString heart_rate);

    void ecgAllQueueChange(QList<qreal> ecg_value);

    /* 信息显示 */
    void sendAlarmMessage(enum ALARM alarm_type, const QString& msg);

    /* 还原菜单栏启动按钮状态 */
    void restoreMenuStatus(bool flag);

    /* 氦气值变更 */
    void heliumValueChange(qreal he_value);

    /* 控制界面参数统一加载 */
    void paramerLoading(P_DATA_s value);

    void setRunMode(int value);
    void setTriggerMode(int value);
    void setTriggerSource(int value);
    void setTriggerRate(int value);
    void setAugmentData(int value);
    void setThresholdData(int value);
    void setTimeData(int value);
    void setRTracData(int value);

    /* 修改显示界面相关参数信号 */
    void modifyTriggerMode(int value);
    void modifyETriggerSource(int value);
    void modifyATriggerSource(int value);
    void modifyAugmentData(int value);
    void modifyTriggerRate(int value);
    void modifyThresholdData(int value);
    void modifyInflationData(int value);
    void modifyDeflationData(int value);
    //void modifyRTracData(int value);

    /* 心电波形显示增益修改 */
    void setEcgGainData(QString value, int gain);
    void setEcgSpeedData(int value);
    void modifyEcgGainData(int value);

    /* 血压波形显示增益修改 */
    void setArterialGainData(int top, int avg, int bottom);

    /* 球囊波形显示增益修改 */
    void setBalloonGainData(int top, int bottom);

    void setLeadStatus(int value);

    void setDebugLeftValue(int value);
    void setDebugRightValue(int value);

    /** @brief 清空波形数据 */
    void clearLineSeries();

    void playSound(int type);

public slots:

    void initGainEcg(bool flag);

    void setEcgPlotAttr(int w, int h);

    void setAllEcgPlotAttr(int w, int h);

    void setBalloonPlotAttr(int w, int h);

    void setArterialPlotAttr(int w, int h);

    void setFreezeDisplay(bool value);

    void setEcgHarFilter(int value);
    void setEcgLowFilter(int value);
    void setEcgHeightFilter(int value);

    void setCpuAffinity(int core_nums);

    /* level + msg */
    QList<QString> getAlarmMessage(int page_index)
    {
        qDebug() << "getAlarmMessage->" << page_index;
        return QList<QString>();
    }

private:

    qreal pixel_1mv;

    qreal pixel_1mmHg;
    qreal h_gain_balloon;

    qreal ecg_plotArea[2];
    qreal ecg_all_plotArea[2];

#ifdef Q_OS_WIN
    qDebug()<<"windows";
#endif

#ifdef Q_OS_MAC
    qDebug()<<"mac";
#endif

#ifdef Q_OS_ANDROID
    qDebug()<<"android";
#endif

#ifdef Q_PROCESSOR_ARM
    //qDebug()<<"arm";
#endif

#ifdef Q_OS_LINUX
    int sync_fd[2];
#endif

    int placeholder;

    bool m_ecg_queue_status;
};

#endif // PUBLICDATA_H
