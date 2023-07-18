#ifndef LOWSPEEDDATA_H
#define LOWSPEEDDATA_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QDataStream>

#include "UdiskWatcher.h"
#include "serialport.h"
#include "publicdata.h"
#include "mycrc.h"

/** @brief LowSpeedData 和 HighSpeedData 可以合并优化为基类  */

class LowSpeedData : public QObject
{
    Q_OBJECT
public:
    explicit LowSpeedData();

    bool shake_init();
    bool check_init();
    bool param_init();
    bool machine_init();

    void bind_signals();
    void unbind_signals();

    /* 设置程序运行各项指标参数 */
    void setBaseDataPointer(QSharedPointer<PublicData> obj);

signals:

    void ecg_start(bool flag);

    void balloon_show(bool flag);

    void reset_point_num();

    void resetMainThreadAff(int core_nums);

private slots:

    void init();

    void balloon_status_change(bool flag);

    void read_data_process();

    void sensor_calibrate();

    void base_data_refresh();

    void set_paramer_data(QString, int);

    void set_iab_fill();

    void set_write_file(bool status);

    void set_aug_alarm_data(int value);

    void set_ecg_gain_dim(int value);

    void set_ecg_speed_gain(int value);

    /** @brief 走纸速度显示反馈动作清空判断 */
    void clear_feedback(int value);

    /** @brief 打印心电波形 */
    void printer_ecg_wave(bool flag);

    void start_printer_process(QString file);

    void get_printer_result(int ret_id = 0);

    void set_system_volume(int value);

    void set_show_brightness(int value);

    void set_control_brightness(int value);

    void set_show_more_ecgwave(int index);

private:

    qint64 shakeHandsProcess(const QByteArray& rdata = "");
    qint64 checkProcess(const QByteArray& rdata = "");
    qint64 machineProcess(const QByteArray& rdata = "");
    qint64 paramerProcess(const QByteArray& rdata = "");

    void calibrateProcess(const uint8_t *rdata);
    void pressureProcess(const uint8_t* rdata);
    void heliumProcess(const uint8_t* rdata);
    void batteryProcess(const uint8_t* rdata);
    void machineStatusProcess(const uint8_t* rdata);
    void oneParamerProcess(const uint8_t* rdata);
    void iabFillProcess(const uint8_t *rdata);

    void get_pressure_data();
    void get_helium_data();
    void get_battery_data();
    void get_machine_status_data();

    bool balloon_control(bool flag);

    void ass_data_process(QByteArray& rdata);

    void printDataWriteFile(const QQueue<ECG_SHOW_DATA>& buf);

    void frame_data_process(size_t real_size, uint8_t *fdata);

    /* true -> write */
    void operation_paramer_file(bool rw);
    void qml_paramer_loading();

    /** @brief 升级下位机程序 */
    void upgradeLowerMachine();

    QString getPrinterFileName();

private:

    QThread thread;

    QProcess *myProcess;

    QTimer *base_data_timer;

    SerialPort *ass_serialport;

    UdiskWatcher *disk_watcher;

    QSharedPointer<PublicData> publicData;

    P_DATA_s parameter_data;

    uint8_t clear_feedback_flag;

    volatile bool first_run;
    volatile bool send_par;
};

#endif // LOWSPEEDDATA_H
