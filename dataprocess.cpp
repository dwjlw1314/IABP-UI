#include "dataprocess.h"

DataProcess::DataProcess()
    :arterial_gain{0, 150},
    balloonMax{750, 0}, balloonMin{-750, 0}
{
    ecg_trigger_source = 8;
    Init_EcgAibp(&QuickR1, &NorQRS1);
    InitFiter();
    Init_ABP(ABP1);
    balloonCount = 0;
}

void DataProcess::setBaseDataPointer(int fd, QSharedPointer<PublicData> obj)
{
    rfd = fd;
    publicData = obj;
}

/** @todo 处理实时心电数据 */
void DataProcess::run()
{
    char bufs[2] = {0};

    qDebug() << "DataProcess ThreadID: " << QThread::currentThreadId();

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(DP_CPU,&mask);

    if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &mask) < 0)
        qDebug() << "DataProcess->pthread_setaffinity_np error!";

    while(true)
    {
        //if (!publicData->ecg_com_queue.isEmpty())
        if (read(rfd, bufs, 1))
        {
            //if (bufs[0] == 'S') break;

            if (publicData->ecg_com_queue.isEmpty())
            {
                qDebug() << "publicData->ecg_com_queue.isEmpty";
                msleep(20);
                continue;
            }

            //publicData->setDebugLeftValue(publicData->ecg_com_queue.size());

            PublicData::mutex.lock();
            QByteArray buf = publicData->ecg_com_queue.dequeue();
            PublicData::mutex.unlock();

            //qDebug() << "ecg_com_queue->" << publicData->ecg_com_queue.size();

            //qDebug() << "rdata.size() -> " << buf.size() << "rdata.data() ->" << buf.toHex(' ').toUpper();

            //ecg_ap_data_splicing(buf);
            ecg_ap_data_splicing_2(buf);

            continue;
        }
        else
        {
            qDebug() << "DataProcess read block";
            msleep(20);
        }
    }

    //qDebug() << "HighSpeedData Thread Exit";
}

/** @return true -> show */
bool DataProcess::checkLeadStatus(uint8_t source, uint8_t lead)
{
    bool status = false;

    if (!lead) return status;

    //Leads fall off prompt
    publicData->setLeadStatus(lead);

    if (source == ECG_I)
    {
        if (lead & RA_LEAD || lead & LA_LEAD || lead & RL_LEAD)
            status = true;
        else
            status = false;
    }
    else if (source == ECG_II)
    {
        if (lead & RA_LEAD || lead & LL_LEAD || lead & RL_LEAD)
            status = true;
        else
            status = false;
    }
    else if (source == ECG_III || source == ECG_AVR || source == ECG_AVF || source == ECG_AVL)
    {
        if (lead & RA_LEAD || lead & LL_LEAD || lead & RL_LEAD || lead & LA_LEAD)
            status = true;
        else
            status = false;
    }
    else if (source == ECG_V)
    {
        if (lead & 0x1f) status = true;
    }

    return status;
}

void DataProcess::setBalloonGainData(uint8_t trigger_status, int value)
{
    if (value > balloonMax[1])
        balloonMax[1] = value;

    if (value < balloonMin[1])
        balloonMin[1] = value;

    if (trigger_status & '\x02')
        balloonCount++;

    if (balloonCount > 5)
    {
        int max_increase = balloonMax[0]*0.1;
        int min_increase = balloonMin[0]*0.1;
        if (balloonMax[1] >  balloonMax[0] || balloonMax[1] < balloonMax[0] - max_increase)
            balloonMax[0] = balloonMax[1] * 1.1;

        if (balloonMin[1] < balloonMin[0] || balloonMin[1] > balloonMin[0] - min_increase)
            balloonMin[0] = balloonMin[1] * 1.1;

        balloonMax[1] = 0;
        balloonMin[1] = 0;
        balloonCount = 0;

        //qDebug() << "balloonMax -> " << balloonMax[0] << " balloonMin -> " << balloonMin[0];

        emit publicData->setBalloonGainData(balloonMax[0], balloonMin[0]);
        publicData->printer_base_data.Bmax = balloonMax[0];
        publicData->printer_base_data.Bmin = balloonMin[0];
    }
}

/**
 * @brief COM实时数据处理
 * eg: AA AA 8A 9C 11 96 00 73 3D 00 A7 43 ... AA AA
 */
void DataProcess::ecg_ap_data_splicing(QByteArray& array)
{
    thread_local QByteArray plobalbyte;

    plobalbyte.append(array);

    while(plobalbyte.contains(PublicData::protocol_head))
    {
        int posAA = plobalbyte.indexOf(PublicData::protocol_head);
        plobalbyte.remove(0,posAA);
        int countbyte = (uint8_t)plobalbyte.at(3);

        if(plobalbyte.size() >= countbyte)
        {
            QByteArray array = plobalbyte.left(countbyte);
            uint8_t  crcnum = MyCrc::my_crc8_check((uint8_t*)array.data(), 0, countbyte-1);

            if((uint8_t)array[array.length()-1] != crcnum)
            {
                qDebug()<<"crc校验值不对";
                plobalbyte.remove(0,2);
                continue;
            }
            else
            {
                data_analysis(array);
            }
            plobalbyte.remove(0,2);
        }
        else
        {
            break;
        }
    }
}

void DataProcess::ecg_ap_data_splicing_2(QByteArray& rdata)
{
    thread_local uint8_t combuff[256];
    thread_local size_t combuff_size = 0;
    thread_local bool half_package = true;

    int index = 0;
    int cur_index;

    while(-1 != (cur_index = rdata.indexOf(PublicData::protocol_head, index)))
    {
        if (half_package)
        {
            combuff_size = 2;
            half_package = false;
            ::memcpy(combuff, PublicData::protocol_head.data(), combuff_size);

            rdata.remove(0, cur_index+2);
            continue;
        }

        if (combuff_size+cur_index > 161)
        {
            half_package = true;
            rdata.remove(0, cur_index);
            continue;
        }

        ::memcpy(combuff+combuff_size, rdata.data(), cur_index);
        combuff_size += cur_index;

        rdata.remove(0, cur_index);

        uint8_t len = combuff[3];
        if (combuff_size < len)
        {
            ::memcpy(combuff+combuff_size, rdata.data(), 2);
            combuff_size += 2;
            rdata.remove(0, 2);
            qDebug() << "ECG: Incomplete package!" <<endl;
            continue;
        }

        half_package = true;
        uint8_t crc = MyCrc::my_crc8_check(combuff, 0, len-1);
        if(crc != combuff[len-1])
        {
            qDebug() << "ECG: Received packet verification failed!" <<endl;
            continue;
        }

        if (len == COMMAND_LEN && rdata[2] == '\x8A' && rdata[4] | '\x00')
        {
            qDebug() << "ECG: Abnormal acquisition of ECG data!" << endl;
            continue;
        }

        data_analysis(combuff);
    }

    if (rdata.size() <= 159 && combuff_size + rdata.length() <= 161)
    {
        ::memcpy(combuff+combuff_size, rdata.data(), rdata.length());
        combuff_size += rdata.length();
    }
    else
    {
        half_package = true;
    }
}

void DataProcess::data_analysis(const uint8_t combuff[])
{
    PublicData::semaphore.acquire();

    ECG_ANALYSIS_DATA ecg_analysis_data;

    for(int i = 0; i < 5; i++)
    {
        /* 获取内部血压数据 */
        ecg_analysis_data.ap_in_press = combuff[5+31*i]<<8 | combuff[4+31*i];

        /* 获取外部血压数据 */
        ecg_analysis_data.ap_out_press = combuff[9+31*i]<<8 | combuff[8+31*i];

        /* 获取球囊压力数据 */
        ecg_analysis_data.ball_data = combuff[11+31*i]<<8 | combuff[10+31*i];

        /* 获取心电状态数据 */
        ecg_analysis_data.ecg_lead_state = combuff[12+31*i] & 0x1f;
        ecg_analysis_data.trigger_state = combuff[12+31*i]>>6 & 0x03;

        /* 获取起搏信号 */
        ecg_analysis_data.pacer_state = combuff[12+31*i]>>7 & 0x01;

        /* 获取触发类型 */
        ecg_analysis_data.trigger_source = combuff[13+31*i] & 0x0f;

        /* 获取触发子类型 */
        ecg_analysis_data.trigger_source_son = combuff[13+31*i] >> 4 & 0x0f;

        /* 获取心电 1 通道数据 */
        ecg_analysis_data.ecg_I = combuff[16+31*i]<<24 | combuff[15+31*i]<<16 | combuff[14+31*i]<<8;
        ecg_analysis_data.ecg_I >>= 8;

        /* 获取心电 2 通道数据 */
        ecg_analysis_data.ecg_II = combuff[19+31*i]<<24 | combuff[18+31*i]<<16 | combuff[17+31*i]<<8;
        ecg_analysis_data.ecg_II >>= 8;

        /* 获取心电 3 通道数据 */
        ecg_analysis_data.ecg_III = combuff[25+31*i]<<24 | combuff[24+31*i]<<16 | combuff[23+31*i]<<8;
        ecg_analysis_data.ecg_III >>= 8;

        /* 获取心电 4 通道数据 */
        ecg_analysis_data.ecg_aVR = combuff[28+31*i]<<24 | combuff[27+31*i]<<16 | combuff[26+31*i]<<8;
        ecg_analysis_data.ecg_aVR >>= 8;

        /* 获取心电 5 通道数据 */
        ecg_analysis_data.ecg_aVL = combuff[31+31*i]<<24 | combuff[30+31*i]<<16 | combuff[29+31*i]<<8;
        ecg_analysis_data.ecg_aVL >>= 8;

        /* 获取心电 6 通道数据 */
        ecg_analysis_data.ecg_aVF = combuff[34+31*i]<<24 | combuff[33+31*i]<<16 | combuff[32+31*i]<<8;
        ecg_analysis_data.ecg_aVF >>= 8;

        /* 获取心电 7 通道数据 */
        ecg_analysis_data.ecg_V1 = combuff[22+31*i]<<24 | combuff[21+31*i]<<16 | combuff[20+31*i]<<8;
        ecg_analysis_data.ecg_V1 >>= 8;

        /* 获取外部心电数据 */
        ecg_analysis_data.ecg_out_data = combuff[7+31*i]<<24 | combuff[6+31*i]<<16;
        ecg_analysis_data.ecg_out_data >>= 8;

        if (ECG_I == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_I;
        }
        else if (ECG_II == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_II;
        }
        else if (ECG_III == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_III;
        }
        else if (ECG_AVR == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVR;
        }
        else if (ECG_AVL == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVL;
        }
        else if (ECG_AVF == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVF;
        }
        else if (ECG_V == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_V1;
        }
        else if (ECG_EXT == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_out_data;
        }

        ecg_ap_data_process(ecg_analysis_data);
    }

    PublicData::semaphore.release();

}

void DataProcess::data_analysis(const QByteArray& rdata)
{
    PublicData::semaphore.acquire();

    uint8_t combuff[256] = {0};
    ECG_ANALYSIS_DATA ecg_analysis_data;
    ::memcpy(combuff, rdata.data(), rdata.size());

    for(int i = 0; i < 5; i++)
    {
        /* 获取内部血压数据 */
        ecg_analysis_data.ap_in_press = combuff[5+31*i]<<8 | combuff[4+31*i];

        /* 获取外部血压数据 */
        ecg_analysis_data.ap_out_press = combuff[9+31*i]<<8 | combuff[8+31*i];

        /* 获取球囊压力数据 */
        ecg_analysis_data.ball_data = combuff[11+31*i]<<8 | combuff[10+31*i];

        /* 获取心电状态数据 */
        ecg_analysis_data.ecg_lead_state = combuff[12+31*i] & 0x1f;
        ecg_analysis_data.trigger_state = combuff[12+31*i]>>6 & 0x03;

        /* 获取起搏信号 */
        ecg_analysis_data.pacer_state = combuff[12+31*i]>>7 & 0x01;

        /* 获取触发类型 */
        ecg_analysis_data.trigger_source = combuff[13+31*i] & 0x0f;

        /* 获取触发子类型 */
        ecg_analysis_data.trigger_source_son = combuff[13+31*i] >> 4 & 0x0f;

        /* 获取心电 1 通道数据 */
        ecg_analysis_data.ecg_I = combuff[16+31*i]<<24 | combuff[15+31*i]<<16 | combuff[14+31*i]<<8;
        ecg_analysis_data.ecg_I >>= 8;

        /* 获取心电 2 通道数据 */
        ecg_analysis_data.ecg_II = combuff[19+31*i]<<24 | combuff[18+31*i]<<16 | combuff[17+31*i]<<8;
        ecg_analysis_data.ecg_II >>= 8;

        /* 获取心电 3 通道数据 */
        ecg_analysis_data.ecg_III = combuff[25+31*i]<<24 | combuff[24+31*i]<<16 | combuff[23+31*i]<<8;
        ecg_analysis_data.ecg_III >>= 8;

        /* 获取心电 4 通道数据 */
        ecg_analysis_data.ecg_aVR = combuff[28+31*i]<<24 | combuff[27+31*i]<<16 | combuff[26+31*i]<<8;
        ecg_analysis_data.ecg_aVR >>= 8;

        /* 获取心电 5 通道数据 */
        ecg_analysis_data.ecg_aVL = combuff[31+31*i]<<24 | combuff[30+31*i]<<16 | combuff[29+31*i]<<8;
        ecg_analysis_data.ecg_aVL >>= 8;

        /* 获取心电 6 通道数据 */
        ecg_analysis_data.ecg_aVF = combuff[34+31*i]<<24 | combuff[33+31*i]<<16 | combuff[32+31*i]<<8;
        ecg_analysis_data.ecg_aVF >>= 8;

        /* 获取心电 7 通道数据 */
        ecg_analysis_data.ecg_V1 = combuff[22+31*i]<<24 | combuff[21+31*i]<<16 | combuff[20+31*i]<<8;
        ecg_analysis_data.ecg_V1 >>= 8;

        /* 获取外部心电数据 */
        ecg_analysis_data.ecg_out_data = combuff[7+31*i]<<24 | combuff[6+31*i]<<16;
        ecg_analysis_data.ecg_out_data >>= 8;

        if (ECG_I == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_I;
        }
        else if (ECG_II == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_II;
        }
        else if (ECG_III == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_III;
        }
        else if (ECG_AVR == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVR;
        }
        else if (ECG_AVL == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVL;
        }
        else if (ECG_AVF == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_aVF;
        }
        else if (ECG_V == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_V1;
        }
        else if (ECG_EXT == ecg_analysis_data.trigger_source_son)
        {
            ecg_analysis_data.ecg_cur_data = ecg_analysis_data.ecg_out_data;
        }

        ecg_ap_data_process(ecg_analysis_data);
    }

    PublicData::semaphore.release();
}

void DataProcess::ecg_ap_data_process(const ECG_ANALYSIS_DATA& rdata)
{
    ECG_SHOW_DATA ecg_show_data;

    if (publicData->internal_press)
    {
        /* 获取内部血压数据 */
        ecg_show_data.ap_press = rdata.ap_in_press;
    }
    else
    {
        /* 获取外部血压数据 */
        ecg_show_data.ap_press = rdata.ap_out_press;
    }

    main_ABP(ABP1, ecg_show_data.ap_press);
    ecg_show_data.ap_press /= 10;

    /* 获取血压高度增益*/
    MainAbp(ecg_show_data.ap_press);
    if (ABPC.Ymax != arterial_gain[0] || ABPC.Ymin != arterial_gain[1])
    {
        arterial_gain[0] = ABPC.Ymax;
        arterial_gain[1] = ABPC.Ymin;
        emit publicData->setArterialGainData(ABPC.Ymax, (ABPC.Ymax + ABPC.Ymin)/2, ABPC.Ymin);
        publicData->printer_base_data.Amax = ABPC.Ymax;
        publicData->printer_base_data.Amin = ABPC.Ymin;
        publicData->printer_base_data.Atype = publicData->internal_press;
    }

    if (ABP1->FlagR)
    {
        QList<int> ap_value;
        ap_value << ABP1->Sys << ABP1->Dia << ABP1->Map;
        emit publicData->arterialValueChange(ap_value);
    }

    /* 获取球囊压力数据 */
    ecg_show_data.ball_data = rdata.ball_data / 10;

    /* 获取心电状态数据 */
    uint8_t lead_state = rdata.ecg_lead_state;
    ecg_show_data.trigger_state = rdata.trigger_state;

    setBalloonGainData(ecg_show_data.trigger_state, ecg_show_data.ball_data);

    Main_EcgAibp(&QuickR1, &NorQRS1, rdata.ecg_cur_data);

    if (QuickR1.FlagR)
    {
        QString heart_rate = "--";
        if (QuickR1.AveHR > 30 && QuickR1.AveHR < 300)
            heart_rate = QString::number(QuickR1.AveHR);

        publicData->printer_base_data.AveHR = QuickR1.AveHR;
        emit publicData->ecgHeartBeatChange(heart_rate);
    }

    if (publicData->show_all_lead_wave)
    {
        ecg_show_data.ecg_I = MainFiter(0, rdata.ecg_I, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_I = ecg_show_data.ecg_I / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_II = MainFiter(1, rdata.ecg_II, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_II = ecg_show_data.ecg_II / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_III = MainFiter(2, rdata.ecg_III, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_III = ecg_show_data.ecg_III / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_V1 = MainFiter(3, rdata.ecg_V1, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_V1 = ecg_show_data.ecg_V1 / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_aVF = MainFiter(4, rdata.ecg_aVF, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_aVF = ecg_show_data.ecg_aVF / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_aVL = MainFiter(5, rdata.ecg_aVL, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_aVL = ecg_show_data.ecg_aVL / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_aVR = MainFiter(6, rdata.ecg_aVR, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecg_aVR = ecg_show_data.ecg_aVR / 28.0f * publicData->h_gain_ecg;

        ecg_show_data.ecg_All = true;
    }
    else
    {
        ecg_show_data.ecg_All = false;
        ecg_show_data.ecgData = MainFiter(0, rdata.ecg_cur_data, publicData->Keyhp, publicData->Keylp, publicData->Keynotch50);
        ecg_show_data.ecgData = ecg_show_data.ecgData / 28.0f * publicData->h_gain_ecg;
    }

    //qDebug() << "AveHR ->" << QuickR1.AveHR;
    //qDebug() << "FlagR ->" << QuickR1.FlagR;
    //qDebug() << "ecg_state ->" << ecg_show_data.ecg_state;
    //qDebug() << "ball_data ->" << ecg_show_data.ball_data;

    if (publicData->ecg_gain_mode && publicData->ecg_height_gain != EcgC->sGain)
    {
        publicData->ecg_height_gain = EcgC->sGain;
        publicData->initHeightGainEcg(EcgC->sGain/100.0);
        QString gain = QString::number(EcgC->sGain/100.0,'f',1) + " mv";
        qInfo() << "EcgGain Change->" << gain;
        emit publicData->setEcgGainData(gain, EcgC->sGain);
    }

    if (ecg_trigger_source != rdata.trigger_source_son)
    {
        InitFiter();
        ecg_trigger_source = rdata.trigger_source_son;
        emit publicData->setTriggerSource(ecg_trigger_source);
        emit publicData->modifyETriggerSource(ecg_trigger_source);
    }

    if (checkLeadStatus(ecg_trigger_source, lead_state))
    {
        qDebug() << "checkLeadStatus ->" << lead_state;
        ecg_show_data.ecgData = 0;
    }

    publicData->ecg_show_queue.enqueue(ecg_show_data);

    if (publicData->ecg_pre_sec_queue.size() > publicData->savePrePoint)
        publicData->ecg_pre_sec_queue.dequeue();

    publicData->ecg_pre_sec_queue.enqueue(ecg_show_data);

    if (publicData->print_flag) publicData->ecg_printer_queue.enqueue(ecg_show_data);
}
