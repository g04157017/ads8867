#ifndef __sysm_LedAndHeart_h_
#define __sysm_LedAndHeart_h_
extern "C" {
#include "ql_gpio.h"
#include "ql_error.h"
}
#include "def.h"
#include "comm.h"
#include "config.h"
#include "sysm_log.h"

#include "CJsonObject.h"
#include "iniext.h"

#include "mqtt.h"

typedef int (*SysmLedAndHeartOutCallback) (byte* data, uint datalen);


#ifdef DEBUG_FLAG
#define sysm_LedAndHeart_log(format, ...) sysm_LedAndHeart_dbg(format, ##__VA_ARGS__)
#else
#define sysm_LedAndHeart_log(format, ...)
#endif

class sysm_LedAndHeart
{
public:
	static sysm_LedAndHeart* GetInstance()  ;
    int Init();
    int DeInit(); 
public:
	 static void* sysm_LedAndHeartMsgProc(void* data);
	 int HeartbeatTimingTask();
	 int FristHeartbeatTimingTask();
	 int regSendCallback(SysmLedAndHeartOutCallback callback);
private:
	sysm_LedAndHeart();
	virtual ~sysm_LedAndHeart();

private:
	pthread_t       m_nDataThrs;
	static sysm_LedAndHeart*     m_psysm_LedAndHeart;
    bool               			 m_bStart;
	SysmLedAndHeartOutCallback   m_pSysmLedAndHeartOutCallback;
};

int sysm_LedAndHeart_Init();
#endif

