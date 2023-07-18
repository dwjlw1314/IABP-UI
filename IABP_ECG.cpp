
#include "IABP_ECG.h"
#include "stdlib.h"
#include <stdio.h>
#include <math.h>




const int16_Alg coff_Algfilter[Num_Algfilter] = { 3, 2, 1, 0, -1, -2, -3 };

int32_Alg TimerEcg = 0;

#if IABP_VSS ==1
    QuickR* QuickR1 = (QuickR*)malloc(sizeof(QuickR));
#else 
    QuickR* QuickR1;
#endif

NorQRS* NorQRS1 ;

void Init_NorQRS(NorQRS* NorQRSs)
{
    int16_Alg i;

    NorQRSs = (NorQRS*)malloc(sizeof(NorQRS));

}

void Init_QuickR(QuickR* QuickRs)
{
    int16_Alg i;


		
    //预处理过程变量及缓冲区
    QuickRs->xAlgabs = 0;
    QuickRs->yAlgabs = 0;
    QuickRs->Prodata = 0;
    for (i = 0; i < Num_mean; i++)
        QuickRs->Mem_mean[i] = 0;
    for (i = 0; i < Num_Algfilter; i++)
        QuickRs->Mem_Algfilter[i] = 0;
    for (i = 0; i < Num_Algmv; i++)
        QuickRs->Mem_Algmv[i] = 0;
    
    for (i = 0; i < SNR_SIZE; i++)
    {
        QuickRs->signal_buf[i] = 0;
        QuickRs->noise_buf[i] = 0;
        QuickRs->transMem[i] = 0;
    }
    QuickRs->towards = 0;

    QuickRs->movedata = 0;

    //阈值学习过程变量
    QuickRs->tmaxThrLearn = MIN16;
    QuickRs->ThrPeak = MINThr;
    QuickRs->AvePeak = MINThr;
    for (i = 0; i < Num_ThrLearn; i++)
        QuickRs->Mem_ThrLearn[i] = 0;

    QuickRs->tmaxNoise = MIN16;
    QuickRs->AveNoise = MINThr;
    for (i = 0; i < Num_ThrLearn; i++)
        QuickRs->Mem_Noise[i] = 0;
    QuickRs->Snr = 0;

    QuickRs->maxPeak = 0;
    QuickRs->timeSinceMaxPeak = 0;
    QuickRs->lastPdataPeak = 0;

    //识别到R峰，更新输出状态
    QuickRs->FlagThrLearn = 0;
    QuickRs->FlagR_trac = 0;
    QuickRs->TimerSleep = 0;
    QuickRs->cntRR = 0;

    QuickRs->qual = 0;

    QuickRs->FlagR = 0;
    QuickRs->numRR = 0;
    QuickRs->AveRR = 0;
    QuickRs->AveHR = 0;

    QuickRs->FlagAlg = 0;
    QuickRs->DelayR = MS20;//待后期确认算法实际延迟，再调整
    ;

    for (i = 0; i < Num_processHR; i++)
        QuickRs->Mem_processHR[i] = 0;
    for (i = 0; i < Num_processHR; i++)
        QuickRs->Mem_sortHR[i] = 0;

    QuickRs->Arrhy = 0;

}

void Init_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs)
{
    Init_QuickR(QuickRs);
    Init_NorQRS(NorQRSs);
    TimerEcg = 0;

}
/*
#define LPBUFFER_LGTH 13
int lpfilt(int datum, int init)
{
    static long y1 = 0, y2 = 0;
    static int data[LPBUFFER_LGTH], ptr = 0;
    long y0;
    int output, halfPtr;
    if (init==0)
    {
        for (ptr = 0; ptr < LPBUFFER_LGTH; ++ptr)
            data[ptr] = 0;
        y1 = y2 = 0;
        ptr = 0;
    }
    halfPtr = ptr - (LPBUFFER_LGTH / 2);	// Use halfPtr to index
    if (halfPtr < 0)							// to x[n-6].
        halfPtr += LPBUFFER_LGTH;
    y0 = (y1 << 1) - y2 + datum - (data[halfPtr] << 1) + data[ptr];
    y2 = y1;
    y1 = y0;
    output = y0 / ((LPBUFFER_LGTH * LPBUFFER_LGTH) / 4);
    data[ptr] = datum;			// Stick most recent sample into
    if (++ptr == LPBUFFER_LGTH)	// the circular buffer and update
        ptr = 0;					// the buffer pointer.
    return(output);
}
*/

int16_Alg Algmean(QuickR* QuickRs,int16_Alg data)
{
    int16_Alg result =0;
    for (int16_Alg i = Num_mean-1; i >0; i--)
    {
        QuickRs->Mem_mean[i]= QuickRs->Mem_mean[i-1];
        result+= QuickRs->Mem_mean[i];
    }
    QuickRs->Mem_mean[0] = data;
    result+= QuickRs->Mem_mean[0];
    result =(result+Num_mean/2)/Num_mean;
    return result;
}

int16_Alg Algabs(QuickR* QuickRs, int16_Alg data)
{
    int16_Alg t_tmp = data - QuickRs->xAlgabs;
    if (t_tmp < 0)
    {
        t_tmp = -t_tmp;
    }
    
    QuickRs->yAlgabs = QuickRs->yAlgabs+ t_tmp;
    if (QuickRs->yAlgabs > 20000)
    {
       
        for (int16_Alg i = 0; i < Num_Algfilter; i++)
        {
            QuickRs->Mem_Algfilter[i] -= 10000;
        }
        QuickRs->yAlgabs -= 10000;
    }
    QuickRs->xAlgabs = data;
    return QuickRs->yAlgabs;
}

int16_Alg Algfilter(QuickR* QuickRs, int16_Alg data)
{

    for (int16_Alg i = Num_Algfilter - 1; i > 0; i--)
    {
        QuickRs->Mem_Algfilter[i] = QuickRs->Mem_Algfilter[i - 1];
    }
    QuickRs->Mem_Algfilter[0] = data;
    
    int32_Alg result = 0;
    for (int16_Alg i = 0; i <Num_Algfilter; i++)
    {
        result+= QuickRs->Mem_Algfilter[i]*coff_Algfilter[i];
    }
    result = (result + 5) / 10;
    return result;
}

int16_Alg Algmv(QuickR* QuickRs, int16_Alg data)
{
    int32_Alg result = 0;
    for (int16_Alg i = Num_Algmv - 1; i > 0; i--)
    {
        QuickRs->Mem_Algmv[i] = QuickRs->Mem_Algmv[i - 1];
        result += QuickRs->Mem_Algmv[i];
    }
    QuickRs->Mem_Algmv[0] = data;
    result += QuickRs->Mem_Algmv[0];

    result /= Num_Algmv;
    return result;
}





int16_Alg Process(QuickR* QuickRs, int16_Alg data)
{
    data=Algmean(QuickRs,data);
    QuickRs->movedata = data;
    data = Algabs(QuickRs, data);
    data = Algfilter(QuickRs, data);
    if (0)
    {
        
    }
    else
    {
        data = Algmv(QuickRs, data);
    }
    return data;
}



static void ThrLearn(QuickR* QuickRs)
{
    int32_Alg tmp = 0;
    if (QuickRs->tmaxThrLearn < QuickRs->Prodata)
        QuickRs->tmaxThrLearn = QuickRs->Prodata;
    if (TimerEcg % (MS1600) == 0&& QuickRs->tmaxThrLearn> MINThr)
    {
        if (QuickRs->FlagAlg ==0)//zqj这块要再优化下，控制标志位不能放到检波模块里
            QuickRs->FlagAlg =1;
        for (int16_Alg i = Num_ThrLearn - 1; i > 0; i--)
        {
            QuickRs->Mem_ThrLearn[i] = QuickRs->Mem_ThrLearn[i - 1];
        }
        QuickRs->Mem_ThrLearn[0] = QuickRs->tmaxThrLearn;
        QuickRs->tmaxThrLearn = MIN16;

        QuickRs->AvePeak = 0;
        for (int16_Alg i = 0; i < Num_ThrLearn; i++)
        {
            QuickRs->AvePeak += QuickRs->Mem_ThrLearn[i];
        }
        QuickRs->AvePeak /= Num_ThrLearn;

        
    }
    if (QuickRs->FlagThrLearn == 0)
    {
        if (TimerEcg >= MS1600 * 2)
        {
            QuickRs->FlagThrLearn = 1;
        }
    }
    else
    {
        if (QuickRs->cntRR > MS500)
        {
            QuickRs->ThrPeak = QuickRs->AvePeak / 2;
        }
        else if (QuickRs->cntRR > MS180)
        {
            tmp = (MS500 - QuickRs->cntRR);
            tmp = tmp * QuickRs->AvePeak / 3;
            tmp = tmp / (MS500 - MS180);
            QuickRs->ThrPeak = QuickRs->AvePeak / 2 + tmp;
        }/*
        else
        {
            ThrPeak = MAX16;
        }*/
    }
    

    

}

int16_Alg Peak(QuickR* QuickRs)
{
    
    int16_Alg pk = 0;

    QuickRs->timeSinceMaxPeak+=1;

    if ((QuickRs->Prodata > QuickRs->lastPdataPeak) && (QuickRs->Prodata > QuickRs->maxPeak))
    {
        QuickRs->maxPeak = QuickRs->Prodata;
        if (QuickRs->maxPeak > 2)
            QuickRs->timeSinceMaxPeak = 1;
    }

    else if (QuickRs->Prodata < (QuickRs->maxPeak /2))
    {
        pk = QuickRs->maxPeak;
        QuickRs->maxPeak = 0;
        QuickRs->timeSinceMaxPeak = 0;
    }

    else if (QuickRs->timeSinceMaxPeak > MS100)
    {
        pk = QuickRs->maxPeak;
        QuickRs->maxPeak = 0;
        QuickRs->timeSinceMaxPeak = 0;
    }
    QuickRs->lastPdataPeak = QuickRs->Prodata;
    return(pk);
}
void NoiseR(QuickR* QuickRs)
{
    double_Alg tmp=0,tmp2=0;

    tmp=Peak(QuickRs);

    if (QuickRs->tmaxNoise < tmp&& QuickRs->cntRR>MS200)
        QuickRs->tmaxNoise = tmp;
    if (TimerEcg % (SAMPLE * 2) == 0)
    {
       
        for (int16_Alg i = Num_ThrLearn - 1; i > 0; i--)
        {
            QuickRs->Mem_Noise[i] = QuickRs->Mem_Noise[i - 1];
        }
        QuickRs->Mem_Noise[0] = QuickRs->tmaxNoise;
        QuickRs->tmaxNoise = MIN16;

        QuickRs->AveNoise = 0;
        for (int16_Alg i = 0; i < Num_ThrLearn; i++)
        {
            QuickRs->AveNoise += QuickRs->Mem_Noise[i];
        }
        QuickRs->AveNoise /= Num_ThrLearn;

        tmp = QuickRs->AveNoise;
        if (tmp != 0)
        {
            tmp2 = QuickRs->AvePeak;
            tmp2 = tmp2 * 100 / tmp;
            QuickRs->Snr = tmp2;
        }
    }


}

void sort(int16_Alg* mem, int16_Alg* memsort, int num)
{
    int16_Alg tmp = 0;
    for (int16_Alg i = 0; i < num; i++)
    {
        memsort[i] = mem[i];
    }

    for (int16_Alg i = 0; i < num; i++)
    {
        for (int16_Alg j = 0; j < num; j++)
        {
            if (memsort[i] < memsort[j])
            {
                tmp = memsort[i];
                memsort[i] = memsort[j];
                memsort[j] = tmp;
            }
        }
    }
}
static void processHR(QuickR* QuickRs, int16_Alg data)
{
    int16_Alg result=0 ;
    int16_Alg tnum =0;
    int32_Alg tmp=0;

    if (QuickRs->numRR > 0)
    {
        if (QuickRs->numRR == 1)
        {
            QuickRs->Mem_processHR[0] = QuickRs->cntRR;
            QuickRs->Mem_processHR[1] = QuickRs->cntRR;
            
        }
        else
        {
        
            for (int16_Alg i = Num_processHR - 1; i > 0; i--)
            {
                QuickRs->Mem_processHR[i] = QuickRs->Mem_processHR[i - 1];
            }
            QuickRs->Mem_processHR[0] = QuickRs->cntRR;
           
         }
    }

    if (QuickRs->numRR < Num_processHR)
    {
        QuickRs->numRR++;
    }

    if (QuickRs->numRR >= 3)
    {

        //QuickRs->numRR = Num_processHR;
        sort(QuickRs->Mem_processHR, QuickRs->Mem_sortHR, QuickRs->numRR);
        tmp = QuickRs->Mem_sortHR[QuickRs->numRR / 2];

        for (int16_Alg i = 0;i<Num_processHR ; i++)
        {
            if (abs(tmp - QuickRs->Mem_processHR[i])< tmp/5)
            {
                result += QuickRs->Mem_processHR[i];
                tnum++;
            }
        }
        if (tnum > Num_processHR / 3)
        {
            QuickRs->AveRR = result/tnum;
            tmp = 60* SAMPLE * tnum;
            tmp /= result;
            QuickRs->AveHR = tmp;
            QuickRs->FlagAlg = 1;
        }
        else
        {
            QuickRs->AveHR = 0;
            QuickRs->AveRR = 0;
        }
    }
    QuickRs->cntRR = 0;
}
static void FindR(QuickR* QuickRs)
{
    if (QuickRs->FlagThrLearn == 1)
    {
        if (QuickRs->TimerSleep <= 0)
        {
            if (QuickRs->Prodata > QuickRs->ThrPeak)
            {
                QuickRs->TimerSleep = MS180;

                QuickRs->FlagR = 1;
            /*  if (TimerEcg < 8 * SAMPLE + SAMPLE / 4)
                {
                    QuickRs->FlagR = 0;
                }*/
                
                if (QuickRs->cntRR > MS333 && QuickRs->FlagR == 1)
                {
                    QuickRs->FlagR_trac = 1;
                }
                
            }
        }
        else
        {
            QuickRs->TimerSleep--;
            
        }
    }
}
void arrhythmia(QuickR* QuickRs)
{
    int16_Alg RR1 = 0.;
    int16_Alg RR2 = 0;
    
    if (QuickRs->AveRR < SAMPLE / 2)
    {
        QuickRs->Arrhy |= 0x01;
    }

    RR1 = QuickRs->Mem_processHR[0];
    for (int16_Alg i = 1; i < Num_processHR-1; i++)
    {
        RR2 = QuickRs->Mem_processHR[i];
        if (abs(RR2- RR1) > RR1 / 5)
        {
            QuickRs->Arrhy |= 0x02;
            break;
        }
        RR1 = RR2;
    }
}




// 计算均值
double mean(int* data, int length)
{
    double sum = 0.0;
    for (int i = 0; i < length; i++)
    {
        sum += data[i];
    }
    return sum / length;
}

// 计算标准差
double stddev(int* data, int length, double avg)
{
    double sum = 0.0;
    for (int i = 0; i < length; i++)
    {
        sum += pow(data[i] - avg, 2);
    }
    return sqrt(sum / length);
}


int transition(QuickR* QuickRs, NorQRS* NorQRSs) {
    int i;
    int flag = 0;
    int result = 0;
    if (TimerEcg > 37)
    {
        int sdc = 0;
    }



    for (i = SNR_SIZE - 1; i > 0; i--)
    {
        QuickRs->transMem[i] = QuickRs->transMem[i-1];
        result += QuickRs->transMem[i];
    }
    QuickRs->transMem[0] = 0;
    if (abs(QuickRs->Mem_mean[0]- QuickRs->Mem_mean[1]) > ThrTrans) 
    {
                if (QuickRs->Mem_mean[0] > QuickRs->Mem_mean[1])
                {
                    flag = 1;
                }
                if (QuickRs->Mem_mean[0] < QuickRs->Mem_mean[1])
                {
                    flag = -1;
                }
                if (QuickRs->towards* flag <= 0)
                {
                    QuickRs->transMem[0] = 1;
                    QuickRs->towards = flag;
                }

    }
    else
    {
        QuickRs->towards = 0;
    }
    result += QuickRs->transMem[0];



    return result;
}

int NoiseAnaly(QuickR* QuickRs, NorQRS* NorQRSs)
{

    double SNR = 0;
    
    int i = 0;


    for (i = SNR_SIZE - 1; i > 0; i--)
    {
        QuickRs->signal_buf[i] = QuickRs->signal_buf[i - 1];
        QuickRs->noise_buf[i] = QuickRs->noise_buf[i - 1];
    }
    QuickRs->signal_buf[0] = QuickRs->movedata;
    QuickRs->noise_buf[0] = QuickRs->movedata - QuickRs->Mem_mean[Num_mean / 2];


    // 计算信号的平均值和标准差
    double signalAvg = mean(&(QuickRs->signal_buf[0]), SNR_SIZE);
    double signalStddev = stddev(&(QuickRs->signal_buf[0]), SNR_SIZE, signalAvg);

    // 计算噪声的标准差
    double noiseStddev = stddev(&(QuickRs->noise_buf[0]), SNR_SIZE, 0.0);

    // 计算信噪比
    SNR = 20.0 * log10(signalStddev / noiseStddev);

    return SNR;
      

}





void Main_EcgAibp(QuickR* QuickRs, NorQRS* NorQRSs,int16_Alg data)
{
    
    QuickRs->cntRR++;
    QuickRs->FlagR = 0;
    QuickRs->FlagR_trac = 0;
    
    if (TimerEcg >SAMPLE*20)
    {
        TimerEcg = SAMPLE * 10+1;
        if (QuickRs->cntRR > SAMPLE * 6)
        {
            Init_EcgAibp(QuickRs, NorQRSs); 
        }
    }
    
    

    //预处理
    QuickRs->Prodata=Process(QuickRs,data);

    QuickRs->qual = NoiseAnaly(QuickRs, NorQRSs);
    QuickRs->qual = transition(QuickRs, NorQRSs);
    //阈值学习
    ThrLearn(QuickRs);

    //R峰识别
    FindR(QuickRs);
    if (QuickRs->FlagR == 1)
    {
        processHR(QuickRs, QuickRs->cntRR);
        if (QuickRs->numRR == Num_processHR)
        {
            arrhythmia( QuickRs);
        }
    }


    //伪R峰信号评估
    NoiseR(QuickRs);
    TimerEcg++;
}
