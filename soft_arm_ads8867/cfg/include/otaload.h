#ifndef __OTALOAD_H
#define __OTALOAD_H

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
#define OTA_cfg_log(format, ...) OTA_cfg_dbg(format, ##__VA_ARGS__)
#else
#define OTA_cfg_log(format, ...)
#endif

typedef int (*OTAOutCallback) (byte* data, uint datalen);


class OTACfg
{
public:
    static OTACfg* GetInstance();
    int Init();
    int DeInit();

	int regSendCallback(OTAOutCallback callback);
	int OTACfgConfig_Proc(int opCode, byte * Msg, uint len);

	int OTAQueryCFG();
	int OTAUpdateCFG(byte * Msg);

	int	UpdateAlarm(byte * Msg);
	int	UpdateMosq(byte * Msg);
	int	UpdateSystem(byte * Msg);
	int	UpdateSamp(byte * Msg);
	int	UpdateSenSorDT(byte * Msg);
	int	UpdateETH(byte * Msg);

	int OTACfg_Send_Msg();
	int get_time(char * time);

	int SendOTACfgOpRsp(std :: string msg);
	
private:
    OTACfg();
    virtual ~OTACfg();
    
private:
	OTAOutCallback   m_OTACfgOutCallback;
	std::string 	   DevID;

public:
	static OTACfg*   m_pOTACfgCfg;

};

int OTACfgCfg_Init();


#endif


