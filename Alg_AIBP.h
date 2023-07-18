#ifndef ALG_AIBP_H
#define ALG_AIBP_H

#define int16_Alg int//short
#define int32_Alg int
#define int64_Alg long
#define double_Alg float



#define SAMPLE 250

#define MS2000 SAMPLE*2
#define MS1500 SAMPLE*3/2
#define MS150 SAMPLE*3/20
#define MS110 SAMPLE/9
#define MS500 SAMPLE/2
#define MS333 SAMPLE/3
#define MS250 SAMPLE/4
#define MS200 SAMPLE/5
#define MS180 SAMPLE*9/50
#define MS100 SAMPLE/10
#define MS80 SAMPLE*2/25
#define MS20 SAMPLE/50

#define MAX16 32767
#define MIN16 -32767
#define MINThr 10
#define MinThrLearn 200
#define Num_mean 5
#define Num_Algmv MS80
#define Num_Algfilter 7
#define Num_ThrLearn 4
#define Num_processHR 10
typedef struct 
{

    //预处理过程变量及缓冲区
    int16_Alg Mem_mean[Num_mean];
    int16_Alg xAlgabs;
    int32_Alg yAlgabs;
    int16_Alg Mem_Algfilter[Num_Algfilter];
    int16_Alg Prodata;

    int16_Alg Mem_Algmv[Num_Algmv];


    //阈值学习过程变量
    int16_Alg tmaxThrLearn;
    int16_Alg Mem_ThrLearn[Num_ThrLearn];
    int16_Alg ThrPeak;
    int16_Alg AvePeak;

    int16_Alg tmaxNoise;
    int16_Alg Mem_Noise[Num_ThrLearn];
    int16_Alg AveNoise;
    int16_Alg Snr;

    int16_Alg maxPeak;
    int16_Alg timeSinceMaxPeak;
    int16_Alg lastPdataPeak;


    //识别到R峰，更新输出状态
    int16_Alg FlagThrLearn;
    int16_Alg FlagR_trac;
    int16_Alg TimerSleep;
    int16_Alg cntRR;

    
    int16_Alg numRR;
    int16_Alg AveHR;
    int16_Alg Mem_processHR[Num_processHR];
    int16_Alg Mem_sortHR[Num_processHR];

    int16_Alg FlagAlg;//0是学习中，1是学习结束

    int16_Alg FlagR;
    int16_Alg AveRR;
    int16_Alg DelayR;

    int16_Alg Arrhy;//低一位心率异常，低2位心率过速


}QuickR;

typedef struct 
{


    int16_Alg FlagR;
    int16_Alg AveRR;
    int16_Alg DelayR;
}NorQRS;
extern QuickR* QuickR1 ;
extern NorQRS* NorQRS1 ;


#define NormalQT SAMPLE*2/5
#define NormalPR SAMPLE/5


extern void Init_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs);
extern void Main_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs, int16_Alg data);

#endif
