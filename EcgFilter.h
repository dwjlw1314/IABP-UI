#ifndef EcgFilter_H
#define EcgFilter_H

#define int16_Alg short
#define int32_Alg int
#define int64_Alg long
#define double_Alg float

#define SampleFilter 250
#define SEC2 SampleFilter*2
#define SEC20 SEC2*10
#define SEC5 SampleFilter*5

#define LenMemHp 250l
#define Gain_Ecg_1mv 1000l

#define lenMemlp 50

#define lenlp40 23
#define Gainlp40 1000

#define lenlp18 5
#define Gainlp18 5


typedef struct ECG
{
    int32_Alg MemHp[LenMemHp];
    int16_Alg idxMemHp;
    int32_Alg sumMemHp;
    int16_Alg Flaginit;


    int32_Alg Memlp[lenMemlp];


    int32_Alg xmaxGain;
    int32_Alg xminGain;

    int16_Alg sGain;


    int16_Alg Timer;
}ECG_s;

extern ECG* EcgC;

#define GAIN_ABP 10 //10???1mmgh
typedef struct ABPPlot
{
    int32_Alg Ymax;
    int32_Alg Ymin;

    int32_Alg tmax;
    int32_Alg tmin;

    int16_Alg Timer;
}ABPPlot_s;

extern ABPPlot ABPC;

//??????????? ABPC.Ymax;
//??????????? ABPC.Ymin;
void MainAbp(int32_Alg data);

extern void InitFiter();

//data ????ECG????
//Keyhp ???????? 1?????
//Keylp ???????? 1?????40Hz??2?????
//Keynotch50 ???????????? 1?????
//AutoGain ?????????????? 1?????
extern int32_Alg MainFiter(int flag, int32_Alg data, int16_Alg Keyhp, int16_Alg Keylp, int16_Alg Keynotch50);

#endif
