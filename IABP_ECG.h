#ifndef IABP_ECG_H
#define IABP_ECG_H

#define IABP_VSS 1//1在上位机，0在下位机


#define uint8_Alg unsigned char
#define uint16_Alg unsigned short
#define uint32_Alg unsigned int
#define uint64_Alg unsigned long
#define int8_Alg char//
#define int16_Alg int//short 待后续调整
#define int32_Alg int
#define int64_Alg long
#define double_Alg float



#define SAMPLE 250

#define MS2000 SAMPLE*2
#define MS1500 SAMPLE*3/2
#define MS150 SAMPLE*3/20
#define MS110 SAMPLE/9
#define MS142 SAMPLE/7
#define MS500 SAMPLE/2
#define MS333 SAMPLE/3
#define MS375 SAMPLE*3/8
#define MS250 SAMPLE/4
#define MS200 SAMPLE/5
#define MS180 SAMPLE*9/50
#define MS100 SAMPLE/10
#define MS80 SAMPLE*2/25
#define MS63 SAMPLE/16
#define MS20 SAMPLE/50
#define MS520 SAMPLE/2+MS20
#define MS266 SAMPLE/4+SAMPLE/25
#define MS285 SAMPLE*2/7
#define MS4 1
#define MS1600 SAMPLE*8/5
#define MAX16 32767
#define MIN16 -32767

#define MINThr 10
//#define MinThrLearn 200
#define Num_mean 10
#define SNR_SIZE SAMPLE
#define Num_Algmv MS63
#define Num_Algfilter 7
#define Num_ThrLearn 4
#define Num_processHR 10

#define MV1 4000


#define ThrTrans MV1/4


typedef struct 
{

    //预处理过程变量及缓冲区
    int16_Alg Mem_mean[Num_mean];
    int16_Alg xAlgabs;
    int32_Alg yAlgabs;
    int32_Alg movedata;
    int16_Alg Mem_Algfilter[Num_Algfilter];
    int16_Alg Prodata;

    int16_Alg Mem_Algmv[Num_Algmv];

    int16_Alg signal_buf[SNR_SIZE];
    int16_Alg noise_buf[SNR_SIZE];
    int16_Alg transMem[SNR_SIZE];
    int16_Alg towards ;

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


    int16_Alg qual;

}QuickR;

typedef struct 
{


    int16_Alg FlagR;
    int16_Alg AveRR;
    int16_Alg DelayR;
}NorQRS;
extern QuickR* QuickR1 ;
extern NorQRS* NorQRS1 ;


#define NormalQT MS375
#define NormalPR SAMPLE/10

extern void sort(int16_Alg* mem, int16_Alg* memsort, int num);
extern void Init_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs);
extern void Main_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs, int16_Alg data);

#endif





