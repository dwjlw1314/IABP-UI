#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QObject>

typedef struct ADDITIONAL
{
    /* 参数写入文件时间 */
    uint32_t update_time;

    /* 心电数据算法参数 */
    int16_t Keyhp;
    int16_t Keylp;
    int16_t Keynotch50;

    /* 心电波形高度增益 */
    uint16_t ecgHGain;

    /* 心电波形宽度增益 */
    uint16_t speedGain;

    /* 心电血压数据保存基于当前时间前N秒 */
    uint16_t preSec;

}Additional;

typedef struct
{
    /* 板载血压数据 */
    uint16_t ap_p_sensor;
    /* 外部接口心电+血压 */
    uint16_t ecg_p_data;
    uint16_t ap_p_data;
    /* 球囊压力 */
    int16_t ball_press_p_data;
    /* 板载心电数据 */
    /* 1byte 说明 低5位是导联状态  低6-7位是触发状态   低8位是起搏信号 */
    uint8_t ecg_lead_state 		: 5;	//导联脱落状态
    uint8_t ecg_trigger_state	: 2;    //有无触
    uint8_t pacer_state         : 1;    //起搏信号

    //触发类型
    /* 1byte 说明 低四位是触发源  高四位是触发源下子类型 */
    uint8_t trigger_type       : 4;
    uint8_t trigger_type_son   : 4;

    /* 21byte */
    int32_t ecg_p_lead_i		: 24;
    int32_t ecg_p_lead_ii		: 24;
    int32_t ecg_p_lead_v		: 24;
    int32_t ecg_p_lead_iii		: 24;
    int32_t	ecg_p_lead_avr		: 24;
    int32_t	ecg_p_lead_avl		: 24;
    int32_t	ecg_p_lead_avf		: 24;
}ECG_COM_DATA;

/* 保存串口数据解析结构 */
typedef struct
{
    int ecg_I;                      //I  导联数据
    int ecg_II;                     //II 导联数据
    int ecg_III;                    //III导联数据
    int ecg_aVR;                    //IV 导联数据
    int ecg_aVL;                    //V  导联数据
    int ecg_aVF;                    //VI 导联数据
    int ecg_V1;                     //VII导联数据
    int ecg_out_data;               //外部导联数据

    int ecg_cur_data;               //当前导联数据

    uint8_t ecg_lead_state;         //导联状态
    uint8_t trigger_source;         //触发源
    uint8_t trigger_source_son;     //触发源子类型

    uint8_t trigger_state;          //心电触发状态
    uint8_t pacer_state;            //起搏信号
    int16_t ap_in_press;            //内部血压数据
    int16_t ap_out_press;           //外部血压数据
    int16_t ball_data;              //球囊压力数据
}ECG_ANALYSIS_DATA;

/* 界面心电血压数据结构 */
typedef struct
{
    int ecgData;                    //单导联数据
    int ecg_I;                      //I  导联数据
    int ecg_II;                     //II 导联数据
    int ecg_III;                    //III导联数据
    int ecg_aVR;                    //IV 导联数据
    int ecg_aVL;                    //V  导联数据
    int ecg_aVF;                    //VI 导联数据
    int ecg_V1;                     //VII导联数据
    bool ecg_All;                   //所有导联标记

    uint8_t trigger_state;          //心电触发状态
    uint8_t pacer_state;            //起搏信号
    int16_t ap_press;               //血压数据
    int16_t ball_data;              //球囊压力数据
}ECG_SHOW_DATA;

/** @brief 打印基础数据 */
typedef struct
{
    int AveHR;                      //心率
    int Amax;                       //高压
    int Amin;                       //低压
    int Atype;                      //血压类型
    int Bmax;                       //球囊高压
    int Bmin;                       //球囊低压

}PRINTER_BASE_DATA;

/* 参数数据结构 */
typedef struct P_DATA
{
    /*
    * 低两位代表模式 触发模式
    * 00	auto_matic
    * 01	semi_auto_matic
    * 10	manual_matic
    */
    uint8_t run_mode;

    /*
    * 低1位代表模式 算法模式
    * 0	rtrac_off
    * 1	rtrac_on
    */
    uint8_t rtrac_mode;

    /*
    * 低三位代表模式 触发方式
    * 000	ecg_trigger
    * 001	ap_trigger
    * 010	a_av_trigger
    * 011	a_trigger
    * 100	internal_trigger
    */
    uint8_t trigger_mode;

    /*
    * 触发源选择 低四位用来选择心电  高四位选择血压
    * 0000（L）	ecg_i
    * 0001（L）	ecg_ii
    * 0010（L）	ecg_iii
    * 0011（L）	ecg_avr
    * 0100（L）	ecg_avl
    * 0101（L）	ecg_avf
    * 0110（L）	ecg_v
    * 0111（L）	ecg_exit
    * 0001（H）	ap_internal
    * 0010（H）	ap_exit
    */
    uint8_t trigger_source;

    /*
    * 低两位代表速率 触发比率
    * 000	rate_1：1
    * 001	rate_1：2
    * 010	rate_1：3
    */
    uint8_t trigger_rate;

    /* 充气强度 */
    uint8_t aug_data;

    /* 时序调整 高八位 充气时序 低八位 放气时序 */
    uint16_t time;

    /* 血压阈值设置 */
    uint8_t threshold;

    /* 附加值，非指令下发参数 */
    Additional aux_value;
}P_DATA_s;

Q_DECLARE_METATYPE(P_DATA_s);

enum ALARM {
    TipsType,    //提示信息
    ErrorType,   //错误信息
    MessageType  //常规信息
};

Q_ENUMS(ALARM)
//Q_DECLARE_METATYPE(ALARM)

/** @brief 打印编号结构体 */
typedef struct
{
    int num;
    QString days;
}PRINTER_NUMBER;

#endif // DATASTRUCT_H
