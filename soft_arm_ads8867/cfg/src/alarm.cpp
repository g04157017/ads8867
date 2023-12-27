#include "alarm.h"

void Alarm_cfg_dbg(const char* format, ...)
{
	printf  ("[Alarm_CFG]: ");
	va_list vp;
	va_start(vp, format);
	vprintf (format, vp);
	va_end  (vp);
	printf  ("\n");
	return;
}

int AlarmSendRspStub(byte* data, uint len)
{
    Alarm_cfg_log("CfgSendRspStub:%s,len:%d",data,len);
    return 0;
}

AlarmCfg* AlarmCfg::m_pAlarmCfg = NULL_PTR;

AlarmCfg* AlarmCfg::GetInstance()
{
    if (NULL_PTR == m_pAlarmCfg)
    {
        m_pAlarmCfg = new AlarmCfg();
    }
    
    return (m_pAlarmCfg == NULL_PTR) ? NULL_PTR : m_pAlarmCfg;
}

AlarmCfg::AlarmCfg()
{
	SpeedEffectiveValueThreshold_AC1_H	=	0;
	SpeedEffectiveValueThreshold_AC2_H	=	0;
	SpeedEffectiveValueThreshold_AC3_H	=	0;
	SpeedEffectiveValueThreshold_AC4_H	=	0;
	TemperatureValue_TP1_H				=	0;
	TemperatureValue_TP2_H				=	0;
	PeakToPeakThreshold_AC1_H				=	0;
	PeakToPeakThreshold_AC2_H				=	0;
	PeakToPeakThreshold_AC3_H				=	0;
	PeakToPeakThreshold_AC4_H				=	0;

	SpeedEffectiveValueThreshold_AC1_L	=	0;
	SpeedEffectiveValueThreshold_AC2_L	=	0;
	SpeedEffectiveValueThreshold_AC3_L	=	0;
	SpeedEffectiveValueThreshold_AC4_L	=	0;
	TemperatureValue_TP1_L				=	0;
	TemperatureValue_TP2_L				=	0;
	PeakToPeakThreshold_AC1_L				=	0;
	PeakToPeakThreshold_AC2_L				=	0;
	PeakToPeakThreshold_AC3_L				=	0;
	PeakToPeakThreshold_AC4_L				=	0;
	
	DevID								= {0};
	m_pAlarmOutCallback 	= AlarmSendRspStub;

}

AlarmCfg::~AlarmCfg()
{
	DeInit();
}

int AlarmCfg::regSendCallback(AlarmOutCallback callback)
{
	m_pAlarmOutCallback = callback;
	return OK;	
}

int AlarmCfg::Init()
{
	int ret =  0;
    ret = LoadConfig();
	CHK_ERR(ret,ERR);

    return OK;
}

int AlarmCfg::AlarmConfig_Proc(int opCode, byte * Msg, uint len)
{
	float cfgval = 0;
	int ret = ERR;
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);
	
    switch(opCode)
    {
		case DevidTopic_OPCODE_ThresholdAlarm:
			
			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC1_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC1_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC1_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC1_L= (int)100*cfgval;			
	
			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC2_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC2_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC2_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC2_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC3_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC3_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC3_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC3_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC4_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC4_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(SPEED_EFFECTIVE_AC4_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			SpeedEffectiveValueThreshold_AC4_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(TEMPERATURE_TP1_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			TemperatureValue_TP1_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(TEMPERATURE_TP1_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			TemperatureValue_TP1_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(TEMPERATURE_TP2_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			TemperatureValue_TP2_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(TEMPERATURE_TP2_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			TemperatureValue_TP2_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC1_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC1_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC1_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC1_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC2_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC2_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC2_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC2_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC3_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC3_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC3_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC3_L= (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC4_H,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC4_H = (int)100*cfgval;

			cfgval	=	0;
			ret = resetJson.Get(PEAKTOPEAK_AC4_L,       cfgval);
			CHK_ERR(ret, ERR);
			if(cfgval	!=	0)
			PeakToPeakThreshold_AC4_L= (int)100*cfgval;

			ret = DwnAddAlarm();
			CHK_ERR(ret, ERR);

			ret = Alarm_Send_Msg();
			CHK_ERR(ret, ERR);
		break;
			
		default:

		break;
    }
	return OK;
}


int AlarmCfg::DeInit()
{
    return OK;
}

int AlarmCfg::LoadConfig()
{
	int ret = ERR;
	bool res = false;
    int cfgval = 0.00;
    
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
	
    ret = pIni->LoadIni("config.ini");
    CHK_ERR(ret, ERR);
	
    std::string systemStr = "";
	
    ret = pIni->GetString(CFG_ALARM CFG_ALARM_NAME, systemStr);
    CJsonObject alarmJson(systemStr);

	res = alarmJson.Get(SPEED_EFFECTIVE_AC1_H,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC1_H	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC1_L,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC1_L	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC2_H,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC2_H	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC2_L,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC2_L	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC3_H,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC3_H	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC3_L,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC3_L	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC4_H,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC4_H	= cfgval;

	res = alarmJson.Get(SPEED_EFFECTIVE_AC4_L,       cfgval);
    CHK_ERR(ret, ERR);
	SpeedEffectiveValueThreshold_AC4_L	= cfgval;
	
	res = alarmJson.Get(PEAKTOPEAK_AC1_H,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC1_H				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC1_L,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC1_L				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC2_H,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC2_H				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC2_L,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC2_L				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC3_H,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC3_H				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC3_L,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC3_L				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC4_H,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC4_H				= cfgval;

	res = alarmJson.Get(PEAKTOPEAK_AC4_L,       cfgval);
    CHK_ERR(ret, ERR);
	PeakToPeakThreshold_AC4_L				= cfgval;

	res = alarmJson.Get(TEMPERATURE_TP1_H,       cfgval);
	CHK_ERR(ret, ERR);
	Alarm_cfg_log("LoadConfig TemperatureValue_TP1_H = %f",TemperatureValue_TP1_H);
	TemperatureValue_TP1_H				= cfgval;

	res = alarmJson.Get(TEMPERATURE_TP1_L,       cfgval);
	CHK_ERR(ret, ERR);
	Alarm_cfg_log("LoadConfig TemperatureValue_TP1_L = %f",TemperatureValue_TP1_L);
	TemperatureValue_TP1_L				= cfgval;

	res = alarmJson.Get(TEMPERATURE_TP2_H,       cfgval);
    CHK_ERR(ret, ERR);
	Alarm_cfg_log("LoadConfig TemperatureValue_TP2_H = %f",TemperatureValue_TP2_H);
	TemperatureValue_TP2_H				= cfgval;

	res = alarmJson.Get(TEMPERATURE_TP2_L,       cfgval);
    CHK_ERR(ret, ERR);
	Alarm_cfg_log("LoadConfig TemperatureValue_TP2_L = %f",TemperatureValue_TP2_L);
	TemperatureValue_TP2_L				= cfgval;

	ret = pIni->GetString(CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME, systemStr);
	CHK_ERR(ret, ERR);
	CJsonObject GetIDJson(systemStr);

    res = GetIDJson.Get(CFG_DEVICE_ID,       DevID);
    CHK_TRUE(res, ERR);
	
	pIni->UnLoadIni();
    
    return OK;
}

int AlarmCfg::DwnAddAlarm()
{
    int ret = OK;
    char alarm[CFG_KEY_MAX_LEN] = {0};
    CJsonObject recobj;
    
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");  
	CHK_ERR(ret, ERR);
        
    sprintf(alarm, CFG_ALARM CFG_ALARM_NAME);        
    recobj.Add(SPEED_EFFECTIVE_AC1_H,        SpeedEffectiveValueThreshold_AC1_H);
	    recobj.Add(SPEED_EFFECTIVE_AC1_L,        SpeedEffectiveValueThreshold_AC1_L);
	recobj.Add(SPEED_EFFECTIVE_AC2_H,        SpeedEffectiveValueThreshold_AC2_H);
		recobj.Add(SPEED_EFFECTIVE_AC2_L,        SpeedEffectiveValueThreshold_AC2_L);
	recobj.Add(SPEED_EFFECTIVE_AC3_H,        SpeedEffectiveValueThreshold_AC3_H);
		recobj.Add(SPEED_EFFECTIVE_AC3_L,        SpeedEffectiveValueThreshold_AC3_L);
	recobj.Add(SPEED_EFFECTIVE_AC4_H,        SpeedEffectiveValueThreshold_AC4_H);
		recobj.Add(SPEED_EFFECTIVE_AC4_L,        SpeedEffectiveValueThreshold_AC4_L);

    recobj.Add(PEAKTOPEAK_AC1_H,          PeakToPeakThreshold_AC1_H);
	    recobj.Add(PEAKTOPEAK_AC1_L,          PeakToPeakThreshold_AC1_L);
	recobj.Add(PEAKTOPEAK_AC2_H,          PeakToPeakThreshold_AC2_H);
		recobj.Add(PEAKTOPEAK_AC2_L,          PeakToPeakThreshold_AC2_L);
	recobj.Add(PEAKTOPEAK_AC3_H,          PeakToPeakThreshold_AC3_H);
		recobj.Add(PEAKTOPEAK_AC3_L,          PeakToPeakThreshold_AC3_L);
	recobj.Add(PEAKTOPEAK_AC4_H,          PeakToPeakThreshold_AC4_H);
		recobj.Add(PEAKTOPEAK_AC4_L,          PeakToPeakThreshold_AC4_L);

	recobj.Add(TEMPERATURE_TP1_H,          TemperatureValue_TP1_H);
		recobj.Add(TEMPERATURE_TP1_L,          TemperatureValue_TP1_L);
	recobj.Add(TEMPERATURE_TP2_H,          TemperatureValue_TP2_H);
		recobj.Add(TEMPERATURE_TP2_L,          TemperatureValue_TP2_L);
    pIni->SetVal(alarm, recobj.ToString().c_str());
    ret = pIni->SaveIni("config.ini") ;      
    if(ERR == ret)
    {
        Alarm_cfg_log("AlarmCfg::DwnAddArp: Save to ini fail.");
    }
    
    pIni->UnLoadIni();
	while (system("sync") < 0) {
        Alarm_cfg_log("cmd: sync\t error: %s", strerror(errno));
	}
    return ret;
}

int AlarmCfg::Alarm_Send_Msg()
{
	CJsonObject resjson;
	int ret = ERR;
	char time[13]={0};
	long P_Time=0;
	long  MP_Time = 0;
	ulong P_TimeRemainder;
	ret = get_time(time);
	sscanf(time,"%ld",&P_Time);
	
	resjson.Add(OPC,MisTopic_OPCODE_Answer);
	resjson.Add(Dev_P_ID,DevID);
	resjson.Add(SEQ,111111111);
	resjson.Add(TMS,time);
	resjson.Add(REC,"00");		
	SendAlarmOpRsp(resjson.ToString());
		
	return OK;
}
int AlarmCfg::get_time(char * time)
{
	struct timeval tv;
	char time_s[10];
	std::string mtime;
	gettimeofday(&tv, NULL);
	mtime = std::to_string(tv.tv_sec);
	strcpy(time_s,mtime.c_str());
	sprintf(time,"%s",time_s);
	return OK;
}
int AlarmCfg::SendAlarmOpRsp(std :: string msg)
{
	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);

	pMosquittoPub->publish_MIS_msg((char*)&msg[0], msg.length());
	Alarm_cfg_log("SendAlarmOpRsp:%s,%d", (char*)&msg[0], msg.length());
	return OK;
}
int AlarmCfg_Init()
{
	int ret = ERR;
	AlarmCfg * pAlarmCfg = AlarmCfg::GetInstance();
	CHK_NULL_PTR(pAlarmCfg, ERR);

	ret = pAlarmCfg->Init();
	CHK_ERR(ret, ERR);
	
	Alarm_cfg_log("AlarmCfg  Init ended.");
	
	return OK;
}


