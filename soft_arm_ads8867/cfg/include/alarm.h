#ifndef __ALARM_H
#define __ALARM_H

#include<time.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include "comm.h"
#include "mqttcmd.h"
#include "mqtt.h"


#include "CJsonObject.h"
#include "iniext.h"

#include "def.h"
#include "def.h"
#include "config.h"




#ifdef DEBUG_FLAG
#define Alarm_cfg_log(format, ...) Alarm_cfg_dbg(format, ##__VA_ARGS__)
#else
#define Alarm_cfg_log(format, ...)
#endif

typedef int (*AlarmOutCallback) (byte* data, uint datalen);


class AlarmCfg
{
public:
    static AlarmCfg* GetInstance();
    int Init();
    int DeInit();

	int regSendCallback(AlarmOutCallback callback);
	int AlarmConfig_Proc(int opCode, byte * Msg, uint len);

	int LoadConfig();
    int DwnAddAlarm();

	int Alarm_Send_Msg();
	int get_time(char * time);

	int SendAlarmOpRsp(std :: string msg);
	
private:
    AlarmCfg();
    virtual ~AlarmCfg();
    
private:
	AlarmOutCallback   m_pAlarmOutCallback;
	std::string 	   DevID;

public:
	static AlarmCfg*   m_pAlarmCfg;

public:
	int SpeedEffectiveValueThreshold_AC1_H;	//S1	
	int SpeedEffectiveValueThreshold_AC2_H;	//TS1Msg_s
	int SpeedEffectiveValueThreshold_AC3_H;	//S2
	int SpeedEffectiveValueThreshold_AC4_H;	//TS2Msg_s
	int TemperatureValue_TP1_H;	//TS1Msg_t
	int TemperatureValue_TP2_H;	//TS2Msg_t
	int PeakToPeakThreshold_AC1_H;		//S1	
	int PeakToPeakThreshold_AC2_H;		//TS1Msg_s
	int PeakToPeakThreshold_AC3_H;		//S2
	int PeakToPeakThreshold_AC4_H;		//TS2Msg_s

	int SpeedEffectiveValueThreshold_AC1_L;	//S1	
	int SpeedEffectiveValueThreshold_AC2_L;	//TS1Msg_s
	int SpeedEffectiveValueThreshold_AC3_L;	//S2
	int SpeedEffectiveValueThreshold_AC4_L;	//TS2Msg_s
	int TemperatureValue_TP1_L;	//TS1Msg_t
	int TemperatureValue_TP2_L;	//TS2Msg_t
	int PeakToPeakThreshold_AC1_L;		//S1	
	int PeakToPeakThreshold_AC2_L;		//TS1Msg_s
	int PeakToPeakThreshold_AC3_L;		//S2
	int PeakToPeakThreshold_AC4_L;		//TS2Msg_s
};

int AlarmCfg_Init();


#endif


