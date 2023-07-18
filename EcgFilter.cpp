#include "EcgFilter.h"
#include "stdlib.h"


int32_Alg cofflp40[lenlp40] = { 21, -52, -26, 10, 39, 25, -33, -75, -28, 120, 288, 363, 288, 120, -28, -75, -33, 25, 39, 10, -26, -52, 21 };
int32_Alg cofflp18[lenlp18] = { 1,1,1,1,1 };




ABPPlot ABPC;
ECG* EcgC;
ECG EcgA[7];

void InitHpFiter(struct ECG* Ecg1)
{
	for (int16_Alg i = 0; i < LenMemHp; i++)
	{
		Ecg1->MemHp[i] = 0;
	}
	Ecg1->Flaginit = 0;
	Ecg1->idxMemHp = 0;
	Ecg1->sumMemHp = 0;

}
void InitLpFiter(struct ECG* Ecg1)
{
	for (int16_Alg i = 0; i < lenMemlp; i++)
	{
		Ecg1->Memlp[i] = 0;
	}
}
void InitFiter()
{
    for(int i = 0; i < sizeof(EcgA)/sizeof(ECG); i++)
    {
        InitHpFiter(&EcgA[i]);
        InitLpFiter(&EcgA[i]);

        EcgA[i].Timer = 0;
        EcgA[i].xmaxGain = 0;
        EcgA[i].xminGain = 0;

        EcgA[i].sGain = 100;
    }

	ABPC.Ymax = 150*GAIN_ABP;
	ABPC.Ymin = 50 * GAIN_ABP;

	ABPC.tmax=0*GAIN_ABP;
	ABPC.tmin = 300 * GAIN_ABP;

	ABPC.Timer = 0;

}

int32_Alg LpFiter(ECG* Ecg1, int32_Alg data, int32_Alg* coff, int32_Alg lencoff, int32_Alg Gain)
{

	int32_Alg result = 0;
	int32_Alg tidx = 0;

	for (tidx = lenMemlp - 1; tidx > 0; tidx--)
	{
		Ecg1->Memlp[tidx] = Ecg1->Memlp[tidx - 1];
	}
	Ecg1->Memlp[0]= data;

	result = 0;
	for (tidx = 0; tidx < lencoff; tidx++)
	{
		result+= Ecg1->Memlp[tidx]* coff[tidx];
	}
	result /= Gain;

	return result;
}



int32_Alg HpFiter(ECG* Ecg1, int32_Alg data)
{
	int32_Alg tmp = 0;
	int32_Alg v_cur = 0;
	int32_Alg num = 0;
	int32_Alg tidx = 0;
	Ecg1->sumMemHp -= Ecg1->MemHp[Ecg1->idxMemHp];
	Ecg1->MemHp[Ecg1->idxMemHp] = data;
	Ecg1->sumMemHp += Ecg1->MemHp[Ecg1->idxMemHp];

	Ecg1->idxMemHp += 1;
	if (Ecg1->idxMemHp== LenMemHp)
	{
		Ecg1->idxMemHp = 0;
		Ecg1->Flaginit = 1;
	}



	if (Ecg1->Flaginit == 1)
	{
		if (Ecg1->idxMemHp >= LenMemHp / 2)
		{
			tidx = Ecg1->idxMemHp - LenMemHp / 2;
		}
		else
		{
			tidx = Ecg1->idxMemHp + LenMemHp / 2;
		}
		
		num = LenMemHp;
		v_cur = data;// Ecg1->MemHp[tidx];
		
	}
	else
	{

		tidx = Ecg1->idxMemHp / 2;
		
		num = Ecg1->idxMemHp;
		v_cur = data;// Ecg1->MemHp[tidx];
	}



	tmp = v_cur * num;
	tmp = tmp- Ecg1->sumMemHp;
	tmp = tmp / num;
	
    if (tmp > 32767)
        tmp = 32767;
    else if (tmp < -32767)
        tmp = -32768;
		


	return tmp;
}

void GainFiter(int index, int32_Alg data)
{
    ECG* ecg = &EcgA[index];

    int32_Alg tmp = 0;
    if (data > ecg->xmaxGain)
        ecg->xmaxGain = data;
    if (data < ecg->xminGain)
        ecg->xminGain = data;
    if (ecg->Timer% SEC2 == SEC2 - 1)
    {
        ecg->Timer = SEC2;

        tmp = ecg->xmaxGain - ecg->xminGain;
        if (tmp < Gain_Ecg_1mv / 2)
            ecg->sGain = 200;
        if (tmp > Gain_Ecg_1mv * 2)
            ecg->sGain = 50;

        ecg->xminGain = 0;
        ecg->xmaxGain = 0;
    }
}

int32_Alg MainFiter(int flag, int32_Alg data, int16_Alg Keyhp , int16_Alg Keylp, int16_Alg Keynotch50)
{
	int32_Alg tdata = data;

	if (Keynotch50 == 1)
	{

	}
	if (Keyhp == 1)
	{
        tdata = HpFiter(&EcgA[flag], tdata);
	}
	if (Keylp == 1)
	{
        tdata = LpFiter(&EcgA[flag], tdata, cofflp40, lenlp40, Gainlp40);
	}
	else if (Keylp == 2)
	{
        tdata = LpFiter(&EcgA[flag], tdata, cofflp18, lenlp18, Gainlp18);
	}
    EcgA[flag].Timer++;
    if (EcgA[flag].Timer >= SEC20)
	{
        EcgA[flag].Timer = SEC2;
	}

    if (0 == flag) EcgC = &EcgA[flag];

	return tdata;
}


void MainAbp(int32_Alg data)
{
	int32_Alg tmean = 0;
	int32_Alg tdet = 0;

	if (data > 300 * GAIN_ABP)
		data = 300 * GAIN_ABP;
	else if (data < 0)
		data = 0;

	if (ABPC.tmin > data)
	{
		ABPC.tmin = data;
	}
	if (ABPC.tmax < data)
	{
		ABPC.tmax = data;
	}


	ABPC.Timer++;
	if (ABPC.Timer >= SEC5)
	{
		ABPC.Timer = 0;

		tmean = (ABPC.tmax + ABPC.tmin) / 2;
		tdet = (ABPC.tmax - ABPC.tmin);

		ABPC.Ymax = tmean+ tdet;
		ABPC.Ymin = tmean - tdet;

		if(ABPC.Ymax> 300 * GAIN_ABP)
			ABPC.Ymax = 300 * GAIN_ABP;
		if (ABPC.Ymin < 0 )
			ABPC.Ymin = 0 ;

		ABPC.tmax = 0;
		ABPC.tmin = 300 * GAIN_ABP;
	}
	
}
