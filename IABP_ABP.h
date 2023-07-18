#ifndef IABP_ABP_H
#define IABP_ABP_H
#include "IABP_ECG.h"

#define GAIN_ABP 10 //100???1mmgh
#define MinABPThrLearn 200

#define MINThr_ABP 50

#define Num_ABPfilter 28//23
#define Num_ABPpeak 20
#define Num_ABPThrLearn 10
#define Num_ABPprocessHR 10
#define Gain_ABPfilter 125


#define DefaultThrAP 12*GAIN_ABP

#define MinThrAP 7*GAIN_ABP
#define MaxThrAP 30*GAIN_ABP
#define MaxTimeRise MS200
typedef struct
{

    //????????????????????

    int16_Alg Mem_filter[Num_ABPfilter];

    int16_Alg Mem_peak[Num_ABPpeak];
    int16_Alg Prodata;
    int16_Alg Peak;
    //????????????
    int16_Alg tmaxThrLearn;
    int16_Alg Mem_ThrLearn[Num_ABPThrLearn];
    int16_Alg ThrPeak;
    int16_Alg AvePeak;

    int16_Alg tmaxNoise;
    int16_Alg Mem_Noise[Num_ABPThrLearn];
    int16_Alg AveNoise;
    int16_Alg Snr;




    //?????R????????????
    int16_Alg FlagThrLearn;

    int16_Alg TimerSleep;
    int16_Alg cntRR;


    int16_Alg numRR;
    int16_Alg AveHR;
    int16_Alg Mem_processHR[Num_ABPprocessHR];
    int16_Alg Mem_sortHR[Num_ABPprocessHR];


    int16_Alg Mem_Sys[Num_ABPprocessHR];
    int16_Alg Mem_Dia[Num_ABPprocessHR];
    int16_Alg t_Sys;
    int16_Alg t_Dia;

    int16_Alg Sys;
    int16_Alg Dia;
    int16_Alg Map;

    int16_Alg Flag_SysDia;


    int16_Alg FlagAlg;//0???????1????????

    int16_Alg FlagR;
    int16_Alg AveRR;
    int16_Alg DelayR;

    int16_Alg Arrhy;//??????????????2????????



    int16_Alg Flag_PV ;
    int16_Alg Point_V ;
    int16_Alg ThrAP;
    int16_Alg CntRise;

}ABP;

extern ABP* ABP1;
extern void Init_ABP(ABP* ABPs);
extern void main_ABP(ABP* ABPs, int16_Alg data);
extern void main_ABP2(ABP* ABPs, int16_Alg data);
#endif


