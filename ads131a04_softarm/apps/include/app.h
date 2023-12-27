#ifndef __app_h
#define __app_h

#include<stdio.h>
#include<stdlib.h>

#include<signal.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include <algorithm>


#include "utils.h"
#include "comm.h"
#include "tommath.h"


#include "sysm_log.h"
#include "def.h"
#include "config.h"
#include "mqtt.h"
#include "kydatabase.h"
#include "adc_ky.h"
#include "iniext.h"
#include "sysm_authorization.h"
#include "sysm_LedAndHeart.h"
#include "alarm.h"




extern "C" {
#include "ql_adc.h"
#include "ql_spi.h"
#include "ql_gpio.h"
#include "ql_error.h"
}

#ifdef DEBUG_FLAG
#define App_log(format, ...) App_dbg(format, ##__VA_ARGS__)
#else
#define App_log(format, ...)
#endif
#ifdef DEBUG_FOR_FLAG
#define print_for_App_data_log(data, leng)  print_for_App_data_dbg(data, leng)
#else
#define print_for_App_data_log(data, leng)
#endif

#define app_ReadBufLen  1024*12*2

class App
{
public:
    static App* GetInstance()  ;
    int Init();
    int DeInit(); 
    int ModuleInit();
    int Start();
    int Stop();

	static void* AppProc(void* data);
	static	int AppInMsgProc(byte* msg, uint len);
	static int AppOutMsgProc(byte* msg, uint len);
	int SendMsgProc(std :: string msg);
	int AppMainProc();

	int NotifyExit();
    int get_adc_Value();
	int get_ads131a04_Value();
	static void safe_sleep(long sec,long usec);
	int AlarmJudgment();

	int CalcRMS(int * Data, int Num);
	int CalcPeakToPeak(int *Data,int Num);
	
private:
    App();
    virtual ~App();

private:
	pthread_t       m_nAppThrs;
    static App*     m_pApp;
	
    MosquittoPub*   m_pMosquittoPub;

	AlarmCfg*		m_pAlarmCfg;

	Authorization*   m_pAuthorization;
	
    bool            m_bStart;
	int 			m_sendTime;

	AppOutMsg_T*    m_pAppOutMsg;

public:
	MqttMsg_T* 		pMqttMsg;
	unsigned char*  readbuf;
	Adc_Ky*         m_pAdc_Ky;
	
};


int app_module_init();
int app_main_init();
int app_module_start();
int app_main_start();

#endif
