#include "sysm_LedAndHeart.h"
static Enum_PinName m_GpioPin_sysled = PINNAME_I2C1_SDA; // OPEN EVB J0203 GPIO42

void sysm_LedAndHeart_dbg(const char* format, ...)
{
	printf  ("[sysm_LedAndHeart_dbg]: ");
	va_list vp;
	va_start(vp, format);
	vprintf (format, vp);
	va_end  (vp);
	printf  ("\n");
	return;
}

int SysmLedAndHeartSendRspStub(byte* data, uint datalen)
{
    sysm_LedAndHeart_log("SysmLedAndHeartSendRspStub:%s,len:%d",data,datalen);
    return 0;
}

sysm_LedAndHeart* sysm_LedAndHeart::m_psysm_LedAndHeart = NULL_PTR;
sysm_LedAndHeart::sysm_LedAndHeart():
	m_nDataThrs(INVALID_THREAD_FD),
	m_bStart(false)
{
	m_pSysmLedAndHeartOutCallback = SysmLedAndHeartSendRspStub;
}
	
sysm_LedAndHeart::~sysm_LedAndHeart()
{
	DeInit();
}

int sysm_LedAndHeart::DeInit()
{  
    m_bStart = false;
    usleep(CFG_SYS_WAIT_USECOND);
    return OK;
}

sysm_LedAndHeart* sysm_LedAndHeart::GetInstance()
{
    if (NULL_PTR == m_psysm_LedAndHeart)
    {
        m_psysm_LedAndHeart = new sysm_LedAndHeart();
    }
    
    return (m_psysm_LedAndHeart == NULL_PTR) ? NULL_PTR : m_psysm_LedAndHeart;
}

int sysm_LedAndHeart::regSendCallback(SysmLedAndHeartOutCallback callback)
{
	m_pSysmLedAndHeartOutCallback = callback;
	return OK;
}

int sysm_LedAndHeart::Init()
{
	int ret = ERR;
	ret = Ql_GPIO_Init(m_GpioPin_sysled, PINDIRECTION_OUT, PINLEVEL_LOW, PINPULLSEL_DISABLE);
	CHK_ERR(ret, ERR);
//	ret = Ql_GPIO_SetLevel(m_GpioPin_sysled,PINLEVEL_LOW);
//	CHK_ERR(ret, ERR);
//	sysm_LedAndHeart_log("the return value is = %d",ret);
	
	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, 1024);
    
    m_bStart = true;
    ret = pthread_create(&m_nDataThrs, &thread_attr, sysm_LedAndHeart::sysm_LedAndHeartMsgProc, this);
    if(ret < 0)
    {
        m_bStart = false;
        sysm_LedAndHeart_log("sysm_LedAndHeart::Init sysm_LedAndHeart thread create fail.");
        APP_LOG(SYSLAH, LOG_FAULT, "sysm_LedAndHeart::Init sysm_LedAndHeart thread create fail.");
        return ERR;
    }

    ret = pthread_attr_destroy(&thread_attr); 
    if(ret != 0)
    {
        return ERR;
    }
 
    return OK;
}
int sysm_LedAndHeart::HeartbeatTimingTask()
{
	CJsonObject resjson;
	std::string msg;

	char data[100];

	int data_leng = 0;
	
	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);

	resjson.Add(OPC,MisTopic_OPCODE_HeartbeatTiming);
	
	resjson.Add(Dev_P_ID,pMosquittoPub->Dev_ID);

	msg = resjson.ToString();

	strcpy(data,&msg[0]);
	data_leng = msg.length();

	pMosquittoPub->publish_MIS_msg(data, data_leng);

	return OK;
}

int sysm_LedAndHeart::FristHeartbeatTimingTask()
{
	CJsonObject resjson;
	std::string msg;

	char data[100];

	int data_leng = 0;
	
	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);

	resjson.Add(OPC,MisTopic_OPCODE_FristHeartbeatTiming);
	
	resjson.Add(Dev_P_ID,pMosquittoPub->Dev_ID);

	msg = resjson.ToString();

	strcpy(data,&msg[0]);
	data_leng = msg.length();

	pMosquittoPub->publish_MIS_msg(data, data_leng);

	return OK;
}


void* sysm_LedAndHeart::sysm_LedAndHeartMsgProc(void* data)
{
	int ret = ERR;
	int wait_time =0 ;
	int NumberOfUploads = 3;
	sysm_LedAndHeart* pThis =(sysm_LedAndHeart*)data;
	
	while(pThis->m_bStart)
	{
		ret = Ql_GPIO_SetLevel(m_GpioPin_sysled,PINLEVEL_LOW);
		sleep(1);
		ret = Ql_GPIO_SetLevel(m_GpioPin_sysled,PINLEVEL_HIGH);
		sleep(1);
		wait_time++;
		NumberOfUploads--;
		if(wait_time>=30)
		{
			ret = pThis->HeartbeatTimingTask();
			
			wait_time = 0;
		}
		if(NumberOfUploads>0)
		{
			ret = pThis->FristHeartbeatTimingTask();
		}
	}
	pThis->m_nDataThrs = INVALID_THREAD_FD;
    return NULL;
}
int sysm_LedAndHeart_Init()
{
	int ret = ERR;
	sysm_LedAndHeart_log("sysm_LedAndHeart Init called.");

	sysm_LedAndHeart* SysmLedAndHeart = sysm_LedAndHeart::GetInstance();
	CHK_NULL_PTR(SysmLedAndHeart, ERR);

	ret = SysmLedAndHeart->Init();
	CHK_ERR(ret, ERR);

	sysm_LedAndHeart_log("sysm_LedAndHeart Init ended.");
	return OK;
}
