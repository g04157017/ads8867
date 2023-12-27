#ifndef __SYSM_AUTHORRIZATION_H
#define __SYSM_AUTHORRIZATION_H
#include <stdio.h>
#include <stdlib.h>

#include "keyExtend.h"
#include"encryption.h"
#include"decrypt.h"


#include<iostream>
#include<bitset>
 

#include "sysm_log.h"
#include "comm.h"
#include "iniext.h"
#include "mqttcmd.h"
#include "utils.h"
#include "sm2.h"
#include "tommath.h"


#ifdef DEBUG_FLAG
#define Authorization_log(format, ...) Authorization_dbg(format, ##__VA_ARGS__)
#else
#define Authorization_log(format, ...)
#endif

typedef int (*AutOutCallback) (byte* data, uint datalen);

typedef struct ATRZT
{
	bool State;

	unsigned char boxStatus;
	
	ulong DeadLine;
	
	uint ToDay;
	uint LastDay;
	std::string DevID;
	ulong AuthStartData;
}ATRZT_T;

class Authorization
{
public:
	static Authorization* GetInstance();
	int Init();
	int DeInit();
	
public:
	int regSendCallback(AutOutCallback callback);
	int PutAutQueMsg(int opCode,  byte* msg, uint len);
	int SendAutOpRsp(byte* msg, uint len);
    int SendAutOpRsp(std::string msg);
	static void* AutMsgProc(void* data);

	int ReadConfig();
	int SaveSystemCfg();
	int AesDecryptionAutMsg();
	int DecryptionAutMsg();
	int DetermineExpirationDate();
	int ReportAuthorizationError(std::string msg);


	int get_time(char * time);
	
	int NotifyExit();

private:
	Authorization();
	virtual ~Authorization();
	
private:
	pthread_t       			m_nAuthThrs;
	bool           				m_bStart;

	AutOutMsg_T*                m_pAutOutMsg;	
	AutOutCallback 				m_pAutOutCallback;
	static Authorization*		m_pAuthorization;
	
public:
	ATRZT_T						m_pATRZT_T;
};

int Aut_Init();

#endif