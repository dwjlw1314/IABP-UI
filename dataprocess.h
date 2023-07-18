#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QObject>
#include <QThread>

#include "publicdata.h"
#include "IABP_ECG.h"
#include "IABP_ABP.h"
#include "EcgFilter.h"
#include "mycrc.h"

class DataProcess : public QThread
{
    Q_OBJECT
public:
    explicit DataProcess();

    void setBaseDataPointer(int fd, QSharedPointer<PublicData> obj);

signals:

    void thread_close();

private slots:

    /** @brief 串口数据完整包拼接 */
    void ecg_ap_data_splicing(QByteArray& rdata);

    void ecg_ap_data_splicing_2(QByteArray& array);

    //6222030200021423086
    void thread_run() { start(); }

private:

    virtual void run();

    /** @brief 判断导联脱落是否影响显示 */
    bool checkLeadStatus(uint8_t source, uint8_t lead);

    /** @brief 处理球囊增益变化 */
    void setBalloonGainData(uint8_t trigger_status, int value);

    /** @brief 串口数据二次处理 */
    void ecg_ap_data_process(const ECG_ANALYSIS_DATA& rdata);

    void data_analysis(const uint8_t combuff[]);

    /** @brief 串口数据解析 */
    void data_analysis(const QByteArray& rdata);

private:

    int rfd;
    int arterial_gain[2];
    int balloonMax[2];
    int balloonMin[2];
    int balloonCount;

    bool isShowEcg;
    uint8_t ecg_trigger_source;

    QuickR QuickR1;
    NorQRS NorQRS1;

    QSharedPointer<PublicData> publicData;
};

#endif // DATAPROCESS_H
