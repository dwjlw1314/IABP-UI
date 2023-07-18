#include "IABP_ABP.h"
#include "stdlib.h"





#if IABP_VSS ==1
	ABP* ABP1 = (ABP*)malloc(sizeof(ABP));
#else 
	ABP* ABP1 ;
#endif
int32_Alg TimerAbp = 0;

//int32_Alg cofffilter[Num_ABPfilter] = { 1, 3, 6, 10, 15, 18, 19, 18, 15, 10, 5, 0, -5, -10, -15, -18, -19, -18, -15, -10, -6, -3, -1};
int32_Alg cofffilter[Num_ABPfilter] =
{
	1, 3, 6, 10, 15, 19, 22, 24, 25, 25, 23, 19, 13, 5, -5, -13, -19, -23, -25, -25, -24, -22, -19, -15, -10, -6, -3, -1,
};

void Init_ABP(ABP* ABPs)
{
	int16_Alg i;


	//预处理过程变量及缓冲区
	
	ABPs->Prodata = 0;
	ABPs->Peak = 0;
	for (i = 0; i < Num_ABPfilter; i++)
		ABPs->Mem_filter[i] = 0;
	for (i = 0; i < Num_ABPpeak; i++)
		ABPs->Mem_peak[i] = 0;



	//阈值学习过程变量
	ABPs->tmaxThrLearn = MIN16;
	ABPs->ThrPeak = MINThr_ABP;
	ABPs->AvePeak = MINThr_ABP;
	for (i = 0; i < Num_ABPThrLearn; i++)
		ABPs->Mem_ThrLearn[i] = 0;

	ABPs->tmaxNoise = MIN16;
	ABPs->AveNoise = MINThr;
	for (i = 0; i < Num_ABPThrLearn; i++)
		ABPs->Mem_Noise[i] = 0;
	ABPs->Snr = 0;



	//识别到R峰，更新输出状态
	ABPs->FlagThrLearn = 0;

	ABPs->TimerSleep = 0;
	ABPs->cntRR = 0;



	ABPs->FlagR = 0;
	ABPs->numRR = 0;
	ABPs->AveRR = 0;
	ABPs->AveHR = 0;

	ABPs->FlagAlg = 0;
	ABPs->DelayR = MS142;//待后期确认算法实际延迟，再调整


	for (i = 0; i < Num_ABPprocessHR; i++)
	{
		ABPs->Mem_processHR[i] = 0;
		ABPs->Mem_sortHR[i] = 0;
		ABPs->Mem_Sys[i] = 0;
		ABPs->Mem_Dia[i] = 0;
	}
	ABPs->t_Sys = 0;
	ABPs->t_Dia = 0;
	ABPs->Flag_SysDia = 0;


	ABPs->Sys = 0;
	ABPs->Dia = 0;
	ABPs->Map = 0;



	ABPs->Arrhy = 0;


	ABPs->ThrAP = DefaultThrAP;
	ABPs->Flag_PV = 0;
	ABPs->Point_V = MAX16;
	ABPs->CntRise = 0;

}
int16_Alg Filter_ABP(ABP* ABPs, int16_Alg data)
{
	int32_Alg result = 0;
	int16_Alg i;

	for ( i = Num_ABPfilter - 1; i > 0; i--)
	{
		ABPs->Mem_filter[i] = ABPs->Mem_filter[i - 1];
	}
	ABPs->Mem_filter[0] = data;

	for ( i = 0; i < Num_ABPfilter; i++)
	{
		result = result+ ABPs->Mem_filter[i] * cofffilter[i];
	}
	result /= Gain_ABPfilter;
	if (result > 32767)
	{
		result = 32767;
	}
	else if (result < - 32768)
	{
		result = -32768;
	}
	return result;
}
int16_Alg Peak_ABP(ABP* ABPs, int16_Alg data)
{
	int32_Alg result = 0;
	int16_Alg i;
	for (i = Num_ABPpeak - 1; i > 0; i--)
	{
		ABPs->Mem_peak[i] = ABPs->Mem_peak[i - 1];
	}
	ABPs->Mem_peak[0] = data;

	result = ABPs->Mem_peak[Num_ABPpeak / 2];
	for (i = 0; i < Num_ABPpeak; i++)
	{
		if (i != Num_ABPpeak / 2)
		{
			if (result < ABPs->Mem_peak[i]|| result<=0)
			{
				result = 0;
				break;
			}
		}
	}
	
	return result;
}
int16_Alg process_ABP(ABP* ABPs, int16_Alg data)
{
	int16_Alg result;
	result = Filter_ABP(ABPs,  data);
	ABPs->Prodata = result;
	result = Peak_ABP(ABPs, result);
	return result;
}
static void ThrLearn(ABP* ABPs)
{
	int32_Alg tmp = 0;
	if (ABPs->tmaxThrLearn < ABPs->Prodata)
		ABPs->tmaxThrLearn = ABPs->Prodata;
	if (TimerAbp % (SAMPLE * 2) == 0 && ABPs->tmaxThrLearn > MinABPThrLearn)
	{
		if (ABPs->FlagAlg == 0)//zqj这块要再优化下，控制标志位不能放到检波模块里
			ABPs->FlagAlg = 1;
		for (int16_Alg i = Num_ThrLearn - 1; i > 0; i--)
		{
			ABPs->Mem_ThrLearn[i] = ABPs->Mem_ThrLearn[i - 1];
		}
		ABPs->Mem_ThrLearn[0] = ABPs->tmaxThrLearn;
		ABPs->tmaxThrLearn = MIN16;

		ABPs->AvePeak = 0;
		for (int16_Alg i = 0; i < Num_ThrLearn; i++)
		{
			ABPs->AvePeak += ABPs->Mem_ThrLearn[i];
		}
		ABPs->AvePeak /= Num_ThrLearn;


	}
	if (ABPs->FlagThrLearn == 0)
	{
		if (TimerAbp >= SAMPLE * 6)
		{
			ABPs->FlagThrLearn = 1;
		}
	}
	else
	{
		if (ABPs->cntRR > MS500)
		{
			ABPs->ThrPeak = ABPs->AvePeak / 2;
		}
		else if (ABPs->cntRR > MS250)
		{
			tmp = (MS500 - ABPs->cntRR);
			tmp = tmp * ABPs->AvePeak / 4;
			tmp = tmp / (MS500 - MS250);
			ABPs->ThrPeak = ABPs->AvePeak / 2 + tmp;
		}/*
		else
		{
			ThrPeak = MAX16;
		}*/
	}




}
static void FindR(ABP* ABPs)
{
	if (ABPs->FlagThrLearn == 1)
	{
		if (ABPs->TimerSleep <= 0)
		{
			if (ABPs->Prodata > ABPs->ThrPeak)
			{
				ABPs->TimerSleep = MS250;

				ABPs->FlagR = 1;
	/*			if (TimerAbp < 8 * SAMPLE + SAMPLE / 4)
				{
					ABPs->FlagR = 0;
				}*/
			}
		}
		else
		{
			ABPs->TimerSleep--;
		}
	}
}
static void processHR(ABP* ABPs, int16_Alg data)
{
	int16_Alg result = 0;
	int16_Alg tnum = 0;
	int32_Alg tmp = 0;
	if (ABPs->numRR > 0)
	{
		if (ABPs->numRR == 1)
		{
			ABPs->Mem_processHR[0] = ABPs->cntRR;
			ABPs->Mem_processHR[1] = ABPs->cntRR;
			
		}
		else
		{

			for (int16_Alg i = Num_ABPprocessHR - 1; i > 0; i--)
			{
				ABPs->Mem_processHR[i] = ABPs->Mem_processHR[i - 1];
			}
			ABPs->Mem_processHR[0] = ABPs->cntRR;
			
		}
	}

	if (ABPs->numRR < Num_ABPprocessHR)
	{
		ABPs->numRR++;
	}

	if (ABPs->numRR >= Num_ABPprocessHR / 2)
	{

		//QuickRs->numRR = Num_processHR;
		sort(ABPs->Mem_processHR, ABPs->Mem_sortHR, ABPs->numRR);
		tmp = ABPs->Mem_sortHR[ABPs->numRR / 2];

		for (int16_Alg i = 0; i < Num_ABPprocessHR; i++)
		{
			if (abs(tmp - ABPs->Mem_processHR[i]) < tmp / 5)
			{
				result += ABPs->Mem_processHR[i];
				tnum++;
			}
		}
		if (tnum > Num_ABPprocessHR / 3)
		{
			ABPs->AveRR = result / tnum;
			tmp = 60 * SAMPLE * tnum;
			tmp /= result;
			ABPs->AveHR = tmp;
			ABPs->FlagAlg = 1;
		}
		else
		{
			ABPs->AveHR = 0;
			ABPs->AveRR = 0;
		}
	}
	ABPs->cntRR = 0;
}

void calculateBloodPressure(ABP* ABPs) 
{
	int16_Alg i = 0;

	int16_Alg num = 0;
	
	ABPs->Sys = 0;
	ABPs->Dia = 0;
	
	
	for (i = 0; i < Num_ABPprocessHR; i++)
	{
		if (ABPs->Mem_Dia[i] > 0 && ABPs->Mem_Sys[i] > 0)
		{
			num++;
		}
		else
		{
			break;
		}
	}
	if (num >= 4)
	{
		ABPs->Dia = (ABPs->Mem_Dia[num / 2]+ GAIN_ABP/2)/ GAIN_ABP;
		ABPs->Sys = (ABPs->Mem_Sys[num / 2] + GAIN_ABP / 2) / GAIN_ABP;
		ABPs->Map = (ABPs->Sys + 2*ABPs->Dia) / 3;
	}
}
void FindSysDia(ABP* ABPs) {

	int16_Alg tdata = 0;
	int16_Alg i = 0;
	tdata = (ABPs->Mem_filter[0] + ABPs->Mem_filter[1] + ABPs->Mem_filter[2] + ABPs->Mem_filter[3]) / 4;
	if (ABPs->FlagR == 1)
	{

		for (i = Num_ABPprocessHR - 1; i > 0; i--)
		{
			ABPs->Mem_Dia[i] = ABPs->Mem_Dia[i - 1];
		}
		ABPs->Mem_Dia[0] = ABPs->t_Dia;


		ABPs->Flag_SysDia = 1;//处于搜寻收缩压阶段
		ABPs->t_Sys = tdata;
	}

	if (ABPs->Flag_SysDia == 1)
	{
		if (tdata >= ABPs->t_Sys) {
			ABPs->t_Sys = tdata;
		}
		else
		{
			for (i = Num_ABPprocessHR - 1; i > 0; i--)
			{
				ABPs->Mem_Sys[i] = ABPs->Mem_Sys[i - 1];
			}
			ABPs->Mem_Sys[0] = ABPs->t_Sys;

			ABPs->Flag_SysDia = 0;//处于搜寻舒张压阶段
			ABPs->t_Dia = tdata;
		}

	}
	else
	{
		if (tdata <= ABPs->t_Dia) 
		{
			ABPs->t_Dia = tdata;
		}

	}




}

void main_ABP(ABP* ABPs, int16_Alg data)
{
	TimerAbp++;
	ABPs->cntRR++;
	ABPs->FlagR = 0;


	ABPs->Prodata = process_ABP(ABPs, data);

	//阈值学习
	ThrLearn(ABPs);

	//R峰识别
	FindR(ABPs);

	FindSysDia(ABPs);

	if (ABPs->FlagR == 1)
	{
		processHR(ABPs, ABPs->cntRR);
		calculateBloodPressure(ABPs);
	}
}



































































int16_Alg Filter_ABP2(ABP* ABPs, int16_Alg data)
{
	int32_Alg result = 0;
	int16_Alg i;

	for (i = Num_ABPfilter - 1; i > 0; i--)
	{
		ABPs->Mem_filter[i] = ABPs->Mem_filter[i - 1];
		result += ABPs->Mem_filter[i];
	}
	ABPs->Mem_filter[0] = data;
	result += ABPs->Mem_filter[0];

	
	result /= Num_ABPfilter;
	if (result > 32767)
	{
		result = 32767;
	}
	else if (result < -32768)
	{
		result = -32768;
	}
	return result;
}

void FindR2(ABP* ABPs, int16_Alg data)
{
	if (ABPs->TimerSleep>0)
	{
		ABPs->TimerSleep--;
	}
	else
	{
		if (ABPs->Flag_PV == 0)//下降阶段
		{
			if (ABPs->Point_V > data)
			{
				ABPs->Point_V = data;
			}
			else 
			{
				ABPs->Flag_PV = 1;
				ABPs->CntRise = 0;
				
			}
		}
		else//上升
		{
			ABPs->CntRise++;
			if (ABPs->CntRise > MaxTimeRise)
			{
				ABPs->CntRise = 0;
				ABPs->Flag_PV = 0;
				ABPs->Point_V = data;
			}
			if (ABPs->Mem_filter[2] >data)
			{
				ABPs->Flag_PV = 0;
				ABPs->Point_V = data;
			}
			else if (data > ABPs->Point_V + ABPs->ThrAP)
			{
				ABPs->FlagR = 1;
				ABPs->TimerSleep = MS250;
			}
		}
	}

}


void main_ABP2(ABP* ABPs, int16_Alg data)
{
	TimerAbp++;
	ABPs->cntRR++;
	ABPs->FlagR = 0;


	ABPs->Prodata = Filter_ABP2(ABPs, data);
	

	//状态识别识别
	FindR2(ABPs,data);

	if (ABPs->FlagR == 1)
	{
		processHR(ABPs, ABPs->cntRR);
	}
}
