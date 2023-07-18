#include "senderdata.h"

SenderData::SenderData(QObject *parent) : QObject(parent)
{
    /** initializer */
}

void SenderData::fillThreadObj(QSharedPointer<LowSpeedData> ls)
{
    m_ls_obj = ls;

    connect(this, SIGNAL(startECG(bool)), m_ls_obj.data(), SLOT(balloon_status_change(bool)));
    connect(this, SIGNAL(stopECG(bool)), m_ls_obj.data(), SLOT(balloon_status_change(bool)));

    connect(this, SIGNAL(sensorCalibrate()), m_ls_obj.data(), SLOT(sensor_calibrate()));

    connect(this, SIGNAL(setTriggerMode(QString, int)), m_ls_obj.data(), SLOT(set_paramer_data(QString, int)));

    connect(this, SIGNAL(setIABFill()), m_ls_obj.data(), SLOT(set_iab_fill()));

    connect(this, SIGNAL(setWriteFile(bool)), m_ls_obj.data(), SLOT(set_write_file(bool)));

    connect(this, SIGNAL(modifyAugAlarmData(int)), m_ls_obj.data(), SLOT(set_aug_alarm_data(int)));

    connect(this, SIGNAL(setEcgGainDim(int)), m_ls_obj.data(), SLOT(set_ecg_gain_dim(int)));

    connect(this, SIGNAL(setEcgSpeedGain(int)), m_ls_obj.data(), SLOT(set_ecg_speed_gain(int)));

    connect(this, SIGNAL(clearFeedback(int)), m_ls_obj.data(), SLOT(clear_feedback(int)));

    connect(this, SIGNAL(setWriteFile(bool)), m_ls_obj.data(), SLOT(printer_ecg_wave(bool)));

    connect(this, SIGNAL(setSystemVolume(int)), m_ls_obj.data(), SLOT(set_system_volume(int)));

    connect(this, SIGNAL(setShowBrightness(int)), m_ls_obj.data(), SLOT(set_show_brightness(int)));

    connect(this, SIGNAL(setControlBrightness(int)), m_ls_obj.data(), SLOT(set_control_brightness(int)));

    connect(this, SIGNAL(setShowMoreEcgWave(int)), m_ls_obj.data(), SLOT(set_show_more_ecgwave(int)));


/** @brief test signal */

    //connect(this, SIGNAL(setIABFill()), m_ls_obj.data(), SLOT(set_iab_fill()));
}
