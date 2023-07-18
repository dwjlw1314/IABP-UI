#ifndef MACRO_H
#define MACRO_H

#define ARM
//#define CONSOLE

#ifdef ARM
    /* 可以在嵌入式ARM开发板运行的代码 */

#ifdef T507
    #define ECG_SERIAL_NAME "/dev/ttyS2"
    #define ASS_SERIAL_NAME "/dev/ttyS3"
#else
    #define ECG_SERIAL_NAME "/dev/ttyS5"
    #define ASS_SERIAL_NAME "/dev/ttyS4"
#endif

#else
    /* 在PC机的linux系统上运行的代码 */

    #define ECG_SERIAL_NAME "/dev/ttyUSB0"
    #define ASS_SERIAL_NAME "/dev/ttyUSB1"

#endif

#define USED_CRYPTO         1

#define ERROR               -1
#define SUCCESS             0

#define MAIN_CPU            0   //cpu kernel
#define HS_CPU              1
#define DP_CPU              2
#define DS_CPU              3

#define REFRESH_TIME        20       // ms
#define BASE_REFRESH_TIME   60000    // ms

#define GAIN_ECG_DEV        140
#define GAIN_BALLOON_DEV    210
#define GAIN_ARTERIAL_DEV   90
#define GAIN_25MM_PIXEL     156      //25mm/s = ? pix
#define GAIN_ECG_SAM        250
#define BASE_LINE_ECG       0

#define ECG_TRIGGER         0
#define AP_TRIGGER          1
#define AV_TRIGGER          2
#define A_TRIGGER           3
#define INTERNAL_TRIGGER    4

#define ECG_I           0x0
#define ECG_II          0x1
#define ECG_III         0x2
#define ECG_AVR         0x3
#define ECG_AVL         0x4
#define ECG_AVF         0x5
#define ECG_V           0x6
#define ECG_EXT         0x7

#define RL_LEAD         0x1
#define LA_LEAD         0x2
#define LL_LEAD         0x4
#define V_LEAD          0x8
#define RA_LEAD         0x10

#define MAX_DIM_ECG static_cast<qreal>(2.0)


#define     HEAD_1                  0XAA
#define     HEAD_2                  0XAA

/** ==============指令类型=============== */

#define     COMMAND_HS                0X11
#define     COMMAND_CK                0X12
#define     COMMAND_MA                0X13
#define     COMMAND_PA                0X17
#define     COMMAND_IF                0X19      //充放气控制
#define     COMMAND_EG                0X1A      //心电
#define     COMMAND_CL                0X1D      //球囊泵
#define     COMMAND_SC                0X1C      //传感器校准
#define     COMMAND_PD                0X1B      //压力数据
#define     COMMAND_HE                0X16      //氦气数据
#define     COMMAND_BA                0X15      //电池数据
#define     COMMAND_MS                0X14      //机器状态

/** ==============指令长度=============== */

#define     COMMAND_LEN               0X07

/** ==============辅助指令=============== */

#define     AUX_All                   0X00      //整包参数 / 开始心电读取 / 传感器校准
#define     AUX_B_START               0X00      //开始球囊反搏
#define     AUX_B_STOP                0X01      //停止球囊反搏
#define     AUX_P_TYPE                0X08      //压力数据类型

#define     AUX_IP_TYPE               1      //设置充气压力
#define     AUX_EC_TYPE               2      //设置心电参考  //cancel
#define     AUX_AP_TYPE               3      //设置血压阈值
#define     AUX_IT_TYPE               4      //设置充气时序
#define     AUX_DT_TYPE               5      //设置放气时序
#define     AUX_SP_TYPE               6      //待机PWM设置
#define     AUX_RP_TYPE               7      //运行PWM设置
#define     AUX_RT_TYPE               8      //设置R-Trac
#define     AUX_TR_TYPE               9      //设置充气频率
#define     AUX_RM_TYPE               10     //仪器工作模式
#define     AUX_TM_TYPE               11     //设置触发方式
#define     AUX_TS_TYPE               12     //触发源选择

/** ==============声音类别=============== */
#define     INIT_VOLUME_TYPE          0      //启动初始化声音
#define     ADJUST_VOLUME_TYPE        1      //音量调节声音

#endif // MACRO_H
