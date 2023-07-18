#ifndef SENDERDATA_H
#define SENDERDATA_H

#include <QObject>
#include <QSharedPointer>
#include "serialport.h"
#include "lowspeeddata.h"

#include <QFileInfo>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

class SenderData : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(int m_number READ number WRITE setNumber NOTIFY numberChanged)

public:
    explicit SenderData(QObject *parent = nullptr);

    //通过Q_INVOKABLE宏标记的public函数可以在QML中访问
    //Q_INVOKABLE void sendSignal();

    void fillThreadObj(QSharedPointer<LowSpeedData> ls);

signals:
    //信号可以在QML中访问

    /** @brief parm start=true  */
    void startECG(bool);
    void stopECG(bool);

    /** @brief 传感器校准 */
    void sensorCalibrate();

    void setTriggerMode(QString name, int value);

    /** @brief 设置血压观察线 */
    void setMoveLineMode(bool flag);

    /** @brief 设置血压观察线数据 */
    void modifyMoveLineData(int value);

    /** @brief 设置充气强度告警值 */
    void modifyAugAlarmData(int value);

    /** @brief 设置气体填充 */
    void setIABFill();

    /** @brief 设置是否保存串口数据 */
    void setWriteFile(bool status);

    /** @brief 设置心电比例尺 */
    void setEcgGainDim(int value);

    /** @brief 设置心电波形绘制速度 */
    void setEcgSpeedGain(int value);

    /** @brief 走纸速度清空反馈 */
    void clearFeedback(int value);

    /** @brief 设置字体 */
    void setLanguageStyle(int value);

    /** @brief 打印数据 */
    void printWaveData(bool flag);

    /** @brief 切换全导联波形显示界面 */
    void setShowMoreEcgWave(int index);

    /** @brief 调节系统音量大小, 0~100 */
    void setSystemVolume(int value);

    /** @brief 控制背光亮度, 0~255 */
    void setShowBrightness(int value);

    /** @brief 调节系统音量大小, 0~255 */
    void setControlBrightness(int value);

public slots:

    //public槽函数可以在QML中访问

private:

    QSharedPointer<LowSpeedData> m_ls_obj;

};

#endif // SENDERDATA_H
