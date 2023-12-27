#include "app.h"

int App_dbg(const char* strlog, ...)
{
	printf  ("[APP]: ");
	va_list vp;
	va_start(vp, strlog);
	vprintf (strlog, vp);
	va_end  (vp);
	printf  ("\n");
	return OK;
}
void print_for_App_data_dbg(int *data, int leng)
{
	for (int i = 1; i <= leng; i++)
		{
			printf("%02d  ", data[i-1]);
			if(i % 16 == 0)
			{
				printf("\n");
			}
		}
	printf("\n\n");
}
void app_print_for_adc_data_dbg(unsigned char *data, int leng)
{
	for (int i = 1; i <= leng; i++)
		{
			printf("%02x  ", data[i-1]);
			if(i % 12 == 0)
			{
				printf("\n");
			}
		}
	printf("\n\n");
}


App* App::m_pApp = NULL_PTR;
App* App::GetInstance()
{
    if (NULL_PTR == m_pApp)
    {
        m_pApp = new App();
    }
    
    return (m_pApp == NULL_PTR) ? NULL_PTR : m_pApp;
}

App::App() : 
	m_nAppThrs(INVALID_THREAD_FD),
    m_bStart(false)
{
    m_pMosquittoPub  				= 	NULL_PTR;
    m_pAdc_Ky  						= 	NULL_PTR;
	m_pAuthorization				=	NULL_PTR;
	pMqttMsg 						= 	NULL_PTR;
	m_pAlarmCfg						=   NULL_PTR;
	m_pAppOutMsg					= 	NULL;
	m_sendTime 						= 	30;
}
	
App::~App()
{
    DeInit();
}

int App::DeInit()
{
    m_bStart = false;
	free(pMqttMsg);//释放内存，防止内存泄漏
	free(readbuf);//释放内存，防止内存泄漏
    usleep(CFG_SYS_WAIT_USECOND);

	NotifyExit();
	QUEUE_FREE(m_pAppOutMsg);
    return OK;
}
int App::NotifyExit()
{
    AppOutMsg_T* pAppMsg = (AppOutMsg_T*)malloc(sizeof(AppOutMsg_T));
    CHK_NULL(pAppMsg, ERR);
    
    pAppMsg->nLen = 0;

    QUEUE_PUSH(m_pAppOutMsg, pAppMsg);
    
    return OK;
}

int App::ModuleInit()
{
	m_pMosquittoPub 		= 	MosquittoPub::GetInstance();
	m_pAdc_Ky 				= 	Adc_Ky::GetInstance();
	m_pAuthorization		= 	Authorization::GetInstance();
	m_pAlarmCfg				=	AlarmCfg::GetInstance();
	return OK;
}

int App::Init()
{  
    int  ret = ERR;
	
	char cfgkey[200] = {0};
	pcstr syscfg = NULL;
	std::string            JsendTime;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");
	CHK_ERR(ret, ERR);
	
	pMqttMsg = (MqttMsg_T*)malloc(sizeof(MqttMsg_T));//申请内存
	if(pMqttMsg == NULL){
		App_log("malloc MqttMsg_T ERR !");
	}
	App_log("MQTTMsg_T = %d",sizeof(MqttMsg_T));


	readbuf = (unsigned char *)malloc(app_ReadBufLen*sizeof(unsigned char));//申请内存
	if(readbuf == NULL){
		App_log("malloc readbuf err !");
	}

	
	sprintf(cfgkey, CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME);
	syscfg = pIni->GetString(cfgkey);
	if (NULL != syscfg)
    {
		std::string strmsg(syscfg);
        CJsonObject sysjson(strmsg);
		ret = sysjson.Get("SendTimeDlyMin",JsendTime);
		CHK_ERR(ret, ERR);
		sscanf(JsendTime.c_str(),"%d",&m_sendTime);
	}
	pIni->UnLoadIni();

	QUEUE_INIT(AppOutMsg_T, m_pAppOutMsg);
    if (NULL == m_pAppOutMsg)
    {
		App_log("AppOutMsg_T Init Fail.");
        SYSM_LOG_ERROR(APP, "AppOutMsg_T Init Fail.");
        return ERR;
    }
	
	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, 10*1024);

    m_bStart = true;
    ret = pthread_create(&m_nAppThrs, &thread_attr, App::AppProc, this);
    if(ret < 0)
    {
        m_bStart = false;
        App_log("App thread create fail.");
        return ERR;
    }	
    return OK;
}
int App::Start()
{
	int ret = ERR;
	m_pMosquittoPub->regRecvCallback(AppInMsgProc);
	m_pAuthorization->regSendCallback(AppOutMsgProc);

	return OK;
}
int App::Stop()
{
	m_bStart = false;
	m_pMosquittoPub->MqttDeInit();
	m_pAdc_Ky->DeInit();
	m_pAuthorization->DeInit();
	m_pAlarmCfg->DeInit();
    usleep(CFG_SYS_WAIT_USECOND);

	this->DeInit();
    return OK;
}

//Process received messages
int App::AppInMsgProc(byte * msg, uint len)
{
	int ret = ERR;
	std::string opCode;
	int m_opCode=0;
	//int opCode = 0;
	CHK_NULL(msg, ERR);
    CHK_VAL(len, MAX_CTRL_MSG_LEN);
    App_log("-----------------------------Start---------------------------");
    App_log("App::AppInMsgProc Start Parser JSON Msg:%s,%d", (char*)msg, len);

    App   *app = GetInstance();
    CHK_NULL_PTR(app, ERR);

    std::string strjson((char*)msg);
    CJsonObject jsonObj(strjson);

    jsonObj.Get(OPC, opCode);
	
	m_opCode =  atoi(opCode.c_str());

	
	if(m_opCode > APP_OPCODE_SYSTEM_AUTHORIZATION_BEGAIN && m_opCode < APP_OPCODE_SYSTEM_AUTHORIZATION_END)
    {
        ret = app->m_pAuthorization->PutAutQueMsg(m_opCode, msg, len);
		CHK_ERR(ret,ERR);
    }


	if(m_opCode > DevidTopic_cfg_begain && m_opCode < DevidTopic_cfg_End)
    {
        ret = app->m_pAlarmCfg->AlarmConfig_Proc(m_opCode, msg, len);
		CHK_ERR(ret,ERR);
    }
	
}
//读取ADC0和ADC1的数据，每个通道1个数值
//根据2021年12月6号开会讨论，调整为一个值
int App::get_adc_Value()
{
	int count=0;
	
	pMqttMsg->TS1Msg_t[count]= 100*(32.0*(m_pAdc_Ky->Get_Ec20ADC0Value())/1000-40);//100*
	pMqttMsg->TS2Msg_t[count]= 100*(32.0*(m_pAdc_Ky->Get_Ec20ADC1Value())/1000-40);
	pMqttMsg->TS1Msg_tLen = 1;
	pMqttMsg->TS2Msg_tLen = 1;
	return OK;
}
int App::get_ads131a04_Value()
{
	int count = 0 ;
	int err_Cunt = 0;
	int ret = 0;
loop:
		int i =0 ;
		err_Cunt=0;
//		memset(readbuf,0,app_ReadBufLen);
		memset(pMqttMsg->S1,0,TSMsgLeng);
		memset(pMqttMsg->S2,0,TSMsgLeng);
		memset(pMqttMsg->TS1Msg_s,0,TSMsgLeng);
		memset(pMqttMsg->TS2Msg_s,0,TSMsgLeng);
//		memset(pMqttMsg->TS1Msg_t,0,TSMsgLeng);
//		memset(pMqttMsg->TS2Msg_t,0,TSMsgLeng);	
		
		m_pAdc_Ky->Get_ADS131A0Value(readbuf);
		
		do{
			pMqttMsg->S1[i]      = 4*100*(2*(2.5*(m_pAdc_Ky->signExtend(&(readbuf[i*12+1*WORD_LENGTH]))))/32768);
		   	if(pMqttMsg->S1[i]>2000)
			pMqttMsg->S1[i] = -(4*100*2*2.5*((65535.0 - m_pAdc_Ky->signExtend(&(readbuf[i*12+1*WORD_LENGTH])))/32768));
			
			pMqttMsg->TS1Msg_s[i]= 4*100*2*2.5*(m_pAdc_Ky->signExtend(&(readbuf[i*12+2*WORD_LENGTH])))/32768;
			 if(pMqttMsg->TS1Msg_s[i]>2000)
			 pMqttMsg->TS1Msg_s[i] = -(4*100*2*2.5*((65535.0-m_pAdc_Ky->signExtend(&(readbuf[i*12+2*WORD_LENGTH])))/32768));
			 
			pMqttMsg->S2[i]		 = 4*100*2*2.5*(m_pAdc_Ky->signExtend(&(readbuf[i*12+3*WORD_LENGTH])))/32768;
			  if(pMqttMsg->S2[i]>2000)
			 pMqttMsg->S2[i] = -(4*100*2*2.5*((65535.0-m_pAdc_Ky->signExtend(&(readbuf[i*12+3*WORD_LENGTH])))/32768));
			  
			pMqttMsg->TS2Msg_s[i] = 4*100*2*2.5*(m_pAdc_Ky->signExtend(&(readbuf[i*12+4*WORD_LENGTH])))/32768;
			  if(pMqttMsg->TS2Msg_s[i]>2000)
			 pMqttMsg->TS2Msg_s[i] = -(4*100*2*2.5*((65535.0-m_pAdc_Ky->signExtend(&(readbuf[i*12+4*WORD_LENGTH])))/32768));
			 i++;
		}while(i<1000);
		
		pMqttMsg->S1_Len=1000;
		pMqttMsg->S2_Len=1000; 
		pMqttMsg->TS1Msg_sLen=1000; 
		pMqttMsg->TS2Msg_sLen=1000; 
		
		if(((pMqttMsg->S1[1]||pMqttMsg->TS1Msg_s[1]||pMqttMsg->S2[1]||pMqttMsg->TS2Msg_s[1]) == 0)&&(err_Cunt<4))
		{
			err_Cunt++;
			sleep(1);
			ret = m_pAdc_Ky->Init_ADS131A0();
			if(ret == ERR)
			{
				for(count=0;count<3;count++)
				{
					ret = m_pAdc_Ky->Init_ADS131A0();
					if(ret != ERR)
					break;
				}
				if(count>=3)
					SYSM_LOG_ERROR(APP,"ADC init three times ERR !");	
			}
			if(err_Cunt>3)
			{
				App_log("read ADC has count three times ! now begain countinue without read adc !");
				SYSM_LOG_ERROR(APP,"ADC Read ERR !");
			}
goto loop;
		}
	return OK;
}
int App::CalcRMS(int * Data, int Num)
{
	int fSum = 0;
	for(int i = 0;i<Num; ++i)
	{
		fSum += Data[i]*Data[i];
	}
	return sqrt(fSum/Num);
}
int App::CalcPeakToPeak(int *Data,int Num)
{
	int Dat_P = *max_element(Data,Data+Num);
	int Dat_N = *min_element(Data,Data+Num);
	int sum = abs(Dat_P)+abs(Dat_N);
	return sum;
}
int App::AlarmJudgment()
{
	pMqttMsg->STA.ALL_STA			=	false;
	pMqttMsg->STA.S1_STA			=	false;
	pMqttMsg->STA.S2_STA			=	false;
	pMqttMsg->STA.TS1_S_STA			=	false;
	pMqttMsg->STA.TS1_T_STA			=	false;
	pMqttMsg->STA.TS2_S_STA			=	false;
	pMqttMsg->STA.TS2_T_STA			=	false;
	pMqttMsg->stat					=	NORMAL;

	if(m_pAlarmCfg->SpeedEffectiveValueThreshold_AC1_L > CalcRMS(pMqttMsg->S1,pMqttMsg->S1_Len) || CalcRMS(pMqttMsg->S1,pMqttMsg->S1_Len) > m_pAlarmCfg->SpeedEffectiveValueThreshold_AC1_H)
		pMqttMsg->STA.S1_STA = true;
	App_log("CalcRMS(pMqttMsg->S1,pMqttMsg->S1_Len) = %d",CalcRMS(pMqttMsg->S1,pMqttMsg->S1_Len));
	
	if(m_pAlarmCfg->SpeedEffectiveValueThreshold_AC2_L > CalcRMS(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen) || CalcRMS(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen) > m_pAlarmCfg->SpeedEffectiveValueThreshold_AC2_H)
		pMqttMsg->STA.TS1_S_STA = true;
	App_log("CalcRMS(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen) = %d",CalcRMS(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen));

	if(m_pAlarmCfg->SpeedEffectiveValueThreshold_AC3_L > CalcRMS(pMqttMsg->S2,pMqttMsg->S2_Len) ||  CalcRMS(pMqttMsg->S2,pMqttMsg->S2_Len) > m_pAlarmCfg->SpeedEffectiveValueThreshold_AC3_H)
		pMqttMsg->STA.S2_STA = true;
	App_log("CalcRMS(pMqttMsg->S2,pMqttMsg->S2_Len) = %d",CalcRMS(pMqttMsg->S2,pMqttMsg->S2_Len));

	if(m_pAlarmCfg->SpeedEffectiveValueThreshold_AC4_L > CalcRMS(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen) || CalcRMS(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen)> m_pAlarmCfg->SpeedEffectiveValueThreshold_AC4_H)
		pMqttMsg->STA.TS2_S_STA = true;
	App_log("CalcRMS(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen) = %d",CalcRMS(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen));

	

	if(m_pAlarmCfg->TemperatureValue_TP1_L > pMqttMsg->TS1Msg_t[0] || pMqttMsg->TS1Msg_t[0]> m_pAlarmCfg->TemperatureValue_TP1_H)
		pMqttMsg->STA.TS1_T_STA = true;
	App_log("pMqttMsg->TS1Msg_t[0] = %d  pMqttMsg->STA.TS1_T_STA = %d  m_pAlarmCfg->TemperatureValue_TP1_H=%d  m_pAlarmCfg->TemperatureValue_TP1_L=%d",pMqttMsg->TS1Msg_t[0],pMqttMsg->STA.TS1_T_STA,m_pAlarmCfg->TemperatureValue_TP1_H,m_pAlarmCfg->TemperatureValue_TP1_L);

	if(m_pAlarmCfg->TemperatureValue_TP2_L > pMqttMsg->TS2Msg_t[0] || pMqttMsg->TS2Msg_t[0]> m_pAlarmCfg->TemperatureValue_TP2_H)
	pMqttMsg->STA.TS2_T_STA = true;
	App_log("pMqttMsg->TS2Msg_t[0] = %d  pMqttMsg->STA.TS2_T_STA = %d  m_pAlarmCfg->TemperatureValue_TP2_H=%d  m_pAlarmCfg->TemperatureValue_TP2_L=%d",pMqttMsg->TS2Msg_t[0],pMqttMsg->STA.TS2_T_STA,m_pAlarmCfg->TemperatureValue_TP2_H,m_pAlarmCfg->TemperatureValue_TP2_L);


	

	if(m_pAlarmCfg->PeakToPeakThreshold_AC1_L > CalcPeakToPeak(pMqttMsg->S1,pMqttMsg->S1_Len) || CalcPeakToPeak(pMqttMsg->S1,pMqttMsg->S1_Len)> m_pAlarmCfg->PeakToPeakThreshold_AC1_H)
	pMqttMsg->STA.S1_STA = true;
	App_log("CalcPeakToPeak(pMqttMsg->S1,pMqttMsg->S1_Len) = %d",CalcPeakToPeak(pMqttMsg->S1,pMqttMsg->S1_Len));

	if(m_pAlarmCfg->PeakToPeakThreshold_AC2_L > CalcPeakToPeak(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen) || CalcPeakToPeak(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen)> m_pAlarmCfg->PeakToPeakThreshold_AC2_H)
	pMqttMsg->STA.TS1_S_STA = true;
	App_log("CalcPeakToPeak(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen) = %d",CalcPeakToPeak(pMqttMsg->TS1Msg_s,pMqttMsg->TS1Msg_sLen));

	if(m_pAlarmCfg->SpeedEffectiveValueThreshold_AC3_L > CalcPeakToPeak(pMqttMsg->S2,pMqttMsg->S2_Len) || CalcPeakToPeak(pMqttMsg->S2,pMqttMsg->S2_Len)> m_pAlarmCfg->SpeedEffectiveValueThreshold_AC3_H)
	pMqttMsg->STA.S2_STA = true;
	App_log("CalcPeakToPeak(pMqttMsg->S2,pMqttMsg->S2_Len) = %d",CalcPeakToPeak(pMqttMsg->S2,pMqttMsg->S2_Len));

	if( m_pAlarmCfg->SpeedEffectiveValueThreshold_AC4_L > CalcPeakToPeak(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen) || CalcPeakToPeak(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen) > m_pAlarmCfg->SpeedEffectiveValueThreshold_AC4_H)
	pMqttMsg->STA.TS2_S_STA = true;
	App_log("CalcPeakToPeak(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen) = %d",CalcPeakToPeak(pMqttMsg->TS2Msg_s,pMqttMsg->TS2Msg_sLen));

	if(pMqttMsg->STA.S1_STA||pMqttMsg->STA.TS1_S_STA||pMqttMsg->STA.S2_STA||pMqttMsg->STA.TS2_S_STA||pMqttMsg->STA.TS1_T_STA||pMqttMsg->STA.TS2_T_STA)
	pMqttMsg->STA.ALL_STA = true;
}
void App::safe_sleep(long sec,long usec)
{
	struct timeval to;

	to.tv_sec = sec;
	to.tv_usec = usec;

	select(0,NULL,NULL,NULL,&to);
}
int App::SendMsgProc(std :: string msg)
{
	AppOutMsgProc((byte*)&msg[0], msg.length());
}

int App::AppOutMsgProc(byte* msg, uint len)
{
	CHK_VAL(len,  MAX_CTRL_MSG_LEN);
    CHK_NULL(msg, ERR);
	
	App* pApp   = App::GetInstance();
    CHK_NULL_PTR(pApp, ERR);
    
    AppOutMsg_T* pAppOutMsg = (AppOutMsg_T*)malloc(sizeof(AppOutMsg_T) + len);
    CHK_NULL(pAppOutMsg, ERR);

    pAppOutMsg->nLen = len;
    memcpy(pAppOutMsg->pMsg , msg, len);

	App_log("AppOutMsgProc  come in ");
    
    QUEUE_PUSH(pApp->m_pAppOutMsg, pAppOutMsg);

    return OK;
}

void* App::AppProc(void* data)
{
	int ret = ERR;
	int count =0 ;
	int err_Cunt=0;
	float test =0;
	int tetst =0;
	int Sum_Tim = 0;
    App* pThis = (App*)data;
	Sum_Tim	=	pThis->m_sendTime +1;
    while(pThis->m_bStart)
    {
		pThis->get_adc_Value();		
		ret = pThis->get_ads131a04_Value();
		if(ret == ERR)
		{
			App_log("get_ads131a04_Value err !");
		}
		pThis->AlarmJudgment();
		if(pThis->pMqttMsg->STA.ALL_STA == true)
		{
			if(pThis->m_pAuthorization->m_pATRZT_T.State)
			{
				pThis->pMqttMsg->stat					=	ALARM;
				pThis->m_pMosquittoPub->MqttProc_send(pThis->pMqttMsg);	
			}
		}
		
		if(Sum_Tim > pThis->m_sendTime)
		{
//封装完数据，进行发送mqtt处理,send the message before ,judge the authorization whether exceed the time limit.
			pThis->m_pAuthorization->DetermineExpirationDate();
			if(pThis->m_pAuthorization->m_pATRZT_T.State)
			{
				pThis->pMqttMsg->stat					=	NORMAL;
				pThis->m_pMosquittoPub->MqttProc_send(pThis->pMqttMsg);			
			}
			App_log("The Transmission Time Interval is = %d min",pThis->m_sendTime);
			Sum_Tim = 0;
		}
		safe_sleep(60,0);
		Sum_Tim++;
    }
	pThis->m_nAppThrs = INVALID_THREAD_FD;
    return NULL_PTR;
}
int App::AppMainProc()
{
	AppOutMsg_T* pAppOutMsg  = NULL;
	while(1)
	{
		 QUEUE_POP(m_pAppOutMsg, pAppOutMsg, AppOutMsg_T);
		App_log("AppMainProc message come in one !");
		 if (NULL != pAppOutMsg && 0 != pAppOutMsg->nLen)
         {
           m_pMosquittoPub->publish_mqtt_msg(pAppOutMsg->pMsg , pAppOutMsg->nLen);//send message
         }

        if (NULL != pAppOutMsg)
         {
            free(pAppOutMsg);
            pAppOutMsg = NULL;
         }
	}

	return OK;
}

int app_module_init()
{
	int ret = ERR;
	App_log("app module init .");
	App* pApp = App::GetInstance();
	CHK_NULL_PTR(pApp, ERR);
	App_log("app module init start.");

/* init other modules  */
	ret = Ads131a04_init();
	if (OK != ret) 
    {  
       App_log("app module init fail.");
       return ERR;
    }
	App_log("app module init Ads131a04_init success.");

	ret = MosquittoPub_init();
	if (OK != ret) 
    {  
       App_log("app  module init  MosquittoPub fail.");
       return ERR;
    }
	ret = Aut_Init();
	if (OK != ret) 
    {  
       App_log("app module init Aut fail.");
       return ERR;
    }
	ret = sysm_LedAndHeart_Init();
	if (OK != ret) 
    {  
       App_log("app module init sysm_LedAndHeart_Init fail.");
       return ERR;
    }

	ret = AlarmCfg_Init();
	if (OK != ret) 
    {  
       App_log("app module init AlarmCfg_Init fail.");
       return ERR;
    }
		
	ret = pApp->ModuleInit();
	if (OK != ret) 
    {  
       App_log("app module init ModuleInit fail.");
       return ERR;
    }
	App_log("app module init ModuleInit success.");
	return OK;
}
int app_main_init()
{
	int ret = ERR;
	App* PApp= App::GetInstance();
	CHK_NULL_PTR(PApp, ERR);

	ret = PApp->Init();
	if(ERR == ret)
	{
		APP_LOG(APP, LOG_FAULT, "app main init fail.");
		return ERR;
	}
	else
	{
		APP_LOG(APP, LOG_DEBUG, "app main init ok.");
	}

	App_log("app_main_init OK.");

	return OK;
}
int app_module_start()
{
	int ret = ERR;

	App* PApp = App::GetInstance();
	CHK_NULL_PTR(PApp, ERR);

	ret = PApp->Start();
	if(ERR == ret)
	{
		APP_LOG(APP,LOG_DEBUG,"app module start fail.");
        return ERR;
	}
	else
	{
		 APP_LOG(APP,LOG_DEBUG,"app module start ok.");
	}
	 App_log("app_module_start OK.");

    return OK;
}
int app_main_start()
{
	int ret = ERR;

	App* pApp = App::GetInstance();
	CHK_NULL_PTR(pApp, ERR);
	App_log("app_main_start started OK.");
    
    SYSM_LOG_ERROR(APP, "app started ok.");

	ret = pApp->AppMainProc();
    CHK_ERR(ret, ERR);

    App_log("app_main_start finished OK.");
    
    APP_LOG(APP, LOG_DEBUG, "app finished ok.");

    return OK;
}



