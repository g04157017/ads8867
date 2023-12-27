#include "sysm_authorization.h"
#include "base64.h"


Authorization* Authorization::m_pAuthorization = NULL_PTR;

void Authorization_dbg(const char* format, ...)
{
	printf  ("[Authorization]: ");
	va_list vp;
	va_start(vp, format);
	vprintf (format, vp);
	va_end  (vp);
	printf  ("\n");
	return;
}
void print_for_Authorization_dbg(unsigned char *data, int leng)
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

int AutSendRspStub(byte* data, uint len)
{
    Authorization_log("CfgSendRspStub:%s,len:%d",data,len);
    return 0;
}

Authorization* Authorization::GetInstance()
{
    if (NULL_PTR == m_pAuthorization)
    {
        m_pAuthorization = new Authorization();
    }
    
    return (m_pAuthorization == NULL_PTR) ? NULL_PTR : m_pAuthorization;
}

Authorization::Authorization():
m_bStart(false)
{

	m_pAutOutCallback 	= AutSendRspStub;
	m_pAuthorization	= NULL_PTR;
	m_pAutOutMsg 		= NULL_PTR;
	m_pATRZT_T.State	= false;
	m_pATRZT_T.LastDay	= 0;
	m_pATRZT_T.boxStatus=0;
}

Authorization::~Authorization()
{
	DeInit();
}
int Authorization::regSendCallback(AutOutCallback callback)
{
	m_pAutOutCallback = callback;
	return OK;	
}
int Authorization::NotifyExit()
{
    AutOutMsg_T* pAutMsg = (AutOutMsg_T*)malloc(sizeof(AutOutMsg_T));
    CHK_NULL(pAutMsg, ERR);
    
    pAutMsg->nLen = 0;
    pAutMsg->opCode = 0;

    QUEUE_PUSH(m_pAutOutMsg, pAutMsg);
    
    return OK;
}
int Authorization::DeInit()
{
	m_bStart = false;
	m_pAutOutCallback 	= AutSendRspStub;
	m_pAuthorization	= NULL_PTR;
	m_pAutOutMsg 		= NULL_PTR;
	m_pATRZT_T.State	= false;
	m_pATRZT_T.LastDay	= 0;
	m_pATRZT_T.boxStatus=0;
    usleep(CFG_SYS_WAIT_USECOND);
	NotifyExit();
	QUEUE_FREE(m_pAutOutMsg);
	return OK;
}

int Authorization::Init()
{
	int  ret = ERR;
	/* set Authorization   */
	ret = ReadConfig();//Get ID
	CHK_ERR(ret, ERR);
	ret = DecryptionAutMsg();
//	ret = AesDecryptionAutMsg();

	QUEUE_INIT(AutOutMsg_T, m_pAutOutMsg);
    if (NULL == m_pAutOutMsg)
    {
        SYSM_LOG_ERROR(AutCFG, "AutOutMsg_T Init Fail.");
        return ERR;
    }
	
	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, 10*1024);
    
    m_bStart = true;
    ret = pthread_create(&m_nAuthThrs, &thread_attr, Authorization::AutMsgProc, this);
    if(ret < 0)
    {
        m_bStart = false;
        Authorization_log("Authorization::Init Cfg thread create fail.");
        SYSM_LOG_ERROR(AutCFG, "Authorization::Init Cfg thread create fail.");
        return ERR;
    }

    ret = pthread_attr_destroy(&thread_attr); 
    if(ret != 0)
    {
        return ERR;
    }
    
    return OK;
}
 int Authorization::SaveSystemCfg()
{
	int ret = OK;
	
    char syscfg[CFG_KEY_MAX_LEN] = {0};
    CJsonObject recobj;
    char p_AuthStartData[20];
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");
    CHK_ERR(ret, ERR);
	
    sprintf(syscfg, CFG_AUT "%s", CFG_AUT_NAME);
	
	sprintf(p_AuthStartData,"%ld",m_pATRZT_T.AuthStartData);

    recobj.Add(AUTHSTARTDATA , p_AuthStartData);

    pIni->SetVal(syscfg, recobj.ToString().c_str());

    ret = pIni->SaveIni("config.ini") ;  
    if(ERR == ret)
    {
        Authorization_log("Cfg::SaveSystemCfg: Save to ini fail.");
        SYSM_LOG_ERROR(AutCFG, "Cfg::SaveSystemCfg: Save to ini fail.");
    }
    pIni->UnLoadIni();
	while (system("sync") < 0) {
        Authorization_log("cmd: sync\t error: %s", strerror(errno));
	}
    return ret;

}
int Authorization::ReadConfig()
{
	int ret = ERR;
	bool res = false;
    std::string cfgval = "";
	ulong p_cfgval=0;
    
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");
    CHK_ERR(ret, ERR);
    std::string systemStr = "";
    ret = pIni->GetString(CFG_AUT CFG_AUT_NAME, systemStr);
    CJsonObject resetJson(systemStr);

	res = resetJson.Get(AUTHSTARTDATA,       cfgval);
    CHK_TRUE(res, ERR);
	sscanf(cfgval.c_str(),"%d",&p_cfgval);
    m_pATRZT_T.AuthStartData = p_cfgval;
    ret = pIni->GetString(CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME, systemStr);
    CJsonObject p_resetJson(systemStr);

    res = p_resetJson.Get(CFG_DEVICE_ID,       cfgval);
    CHK_TRUE(res, ERR);
    m_pATRZT_T.DevID = cfgval;

	pIni->UnLoadIni();
    CHK_ERR(ret, ERR);
	return OK;
}

int Authorization::DecryptionAutMsg()
{
	int ret = ERR;
	unsigned long encLen = 0;
	unsigned char base64_data[100]={0};
	unsigned long base64_size=0;
	unsigned char decryt[100] = {0};
	unsigned long declen = 0;
	

	unsigned char prikeyBuff[200] = {0};
	unsigned long priLen = 200;

	char P_DevId[14]={0};
	char P_Time[10]={0};

	memset(base64_data,0,100);
	ret = Readfile(SYSM_AUT_FILE,(char*)base64_data,base64_size);
	if(ERR == ret)
	{
		ReportAuthorizationError("AuthorizationCode.txt is null !");
		m_pATRZT_T.boxStatus=0;//no authorization
		return ERR;
	}
	//Authorization_log("Readfile = %s",base64_data);
    base64_decode((const char*)base64_data,base64_size,(unsigned char*)decryt,&declen);

	memcpy(P_DevId,decryt,14);
	Authorization_log("P_DevId = %s",P_DevId);
	ret = strcmp(P_DevId,m_pATRZT_T.DevID.c_str());
	if(OK!= ret)
	{
		ReportAuthorizationError("the id was not match this device !");
		m_pATRZT_T.boxStatus=1;//no not match this device
		return ERR;
	}
	memcpy(P_Time,&decryt[15],10);
	sscanf(P_Time,"%ld",&m_pATRZT_T.DeadLine);
	m_pATRZT_T.boxStatus=2;//authorization the device
END:
	return ret;
}
int Authorization::DetermineExpirationDate()
{
	CJsonObject resjson;
	int ret = ERR;
	char time[13]={0};
//	char encryt[1000] = {0};
	ulong encLen = 0;
	long P_Time=0;
	long  MP_Time = 0;
	ulong P_TimeRemainder;
	ret = get_time(time);
	sscanf(time,"%ld",&P_Time);
	switch(m_pATRZT_T.boxStatus){
		case 0://no authorization
		resjson.Add(OPC,APP_OPCODE_SYSTEM_AUTHORIZATION_REPORT);
		resjson.Add(Dev_P_ID,m_pATRZT_T.DevID);
		resjson.Add(SEQ,111111111);
		resjson.Add(TMS,time);
		resjson.Add(BOXSTA,0);		
		SendAutOpRsp(resjson.ToString());
		m_pATRZT_T.State = false;
	break;
		case 1://no not match this device
		ReportAuthorizationError("001");
		m_pATRZT_T.State = false;
	break;
	case 2://authorization the device
	
	MP_Time = m_pATRZT_T.DeadLine-P_Time;

	if(MP_Time > 2592000)
	{
		resjson.Add(OPC,APP_OPCODE_SYSTEM_AUTHORIZATION_REPORT);
		resjson.Add(Dev_P_ID,m_pATRZT_T.DevID);
		resjson.Add(SEQ,111111111);
		resjson.Add(TMS,time);
		resjson.Add(BOXSTA,1);
		resjson.Add(STADAT,m_pATRZT_T.AuthStartData);
		resjson.Add(AUTHCODE,m_pATRZT_T.DeadLine);
		P_TimeRemainder= (m_pATRZT_T.DeadLine-P_Time)/86400;//unit: day
		resjson.Add(RTS,P_TimeRemainder);
		SendAutOpRsp(resjson.ToString());
		m_pATRZT_T.State = true;
	}
	if( 0< MP_Time && MP_Time<=2592000 )
	{
		resjson.Add(OPC,APP_OPCODE_SYSTEM_AUTHORIZATION_REPORT);
		resjson.Add(Dev_P_ID,m_pATRZT_T.DevID);
		resjson.Add(SEQ,111111111);
		resjson.Add(TMS,time);
		resjson.Add(BOXSTA,1);
		resjson.Add(STADAT,m_pATRZT_T.AuthStartData);
		resjson.Add(AUTHCODE,m_pATRZT_T.DeadLine);
		P_TimeRemainder= (m_pATRZT_T.DeadLine-P_Time)/86400;//unit: day
		resjson.Add(RTS,P_TimeRemainder);
		SendAutOpRsp(resjson.ToString());
		m_pATRZT_T.State = true;
	}
	if( MP_Time < 0 )//Authorization deadline
	{
		resjson.Add(OPC,APP_OPCODE_SYSTEM_AUTHORIZATION_REPORT);
		resjson.Add(Dev_P_ID,m_pATRZT_T.DevID);
		resjson.Add(SEQ,111111111);
		resjson.Add(TMS,time);
		resjson.Add(BOXSTA,3);
		resjson.Add(STADAT,m_pATRZT_T.AuthStartData);
		resjson.Add(AUTHCODE,m_pATRZT_T.DeadLine);
		resjson.Add(RTS,0);
		SendAutOpRsp(resjson.ToString());
		m_pATRZT_T.State = false;
	}		
	break;
		default:
		
	break;
	}
	return OK;
}
int Authorization::ReportAuthorizationError(std::string msg)
{
	CJsonObject resjson;
	char time[13];
	int ret = ERR;
	ret = get_time(time);
	CHK_ERR(ret, ERR);
	  
	resjson.Add(OPC,APP_OPCODE_SYSTEM_AUTHORIZATION_ERR);
	resjson.Add(Dev_P_ID,m_pATRZT_T.DevID);
	resjson.Add(SEQ,111111111);
	resjson.Add(TMS,time);
	resjson.Add(REC,msg);

	SendAutOpRsp(resjson.ToString());
	SYSM_LOG_ERROR(AutCFG,resjson.ToString().c_str());
	return OK;
}
int Authorization::get_time(char * time)
{
	struct timeval tv;
	char time_s[10];
	std::string mtime;
	gettimeofday(&tv, NULL);
	mtime = std::to_string(tv.tv_sec);
	strcpy(time_s,mtime.c_str());
	sprintf(time,"%s",time_s);
	//sprintf(time,"%s%s",time_s,"000");
	return 0;
}
void* Authorization::AutMsgProc(void* data)
{
	int ret = ERR;
	char time[13]={0};
	AutOutMsg_T* pAutOutMsg =NULL;
	std::string cfgval = "";
	std :: string msg;
	char p_msg[500];
    Authorization* pThis = (Authorization*)data;

    int opCode    = 0;

	while(pThis->m_bStart)
	{
		QUEUE_POP(pThis->m_pAutOutMsg, pAutOutMsg, AutOutMsg_T);
        CHK_CONTINUE(pAutOutMsg, NULL);
		
        std::string reqmsg(pAutOutMsg->pMsg);
		CJsonObject resetJson(reqmsg);
		
        switch(pAutOutMsg->opCode)
        {
			case DevidTopic_AUTHORIZATION_ISSUED:
				Authorization_log("now came in APP_OPCODE_SYSTEM_AUTHORIZATION_ISSUED");


				ret = resetJson.Get(REC,       cfgval);
				msg = cfgval.c_str();
				memset(p_msg,0,500);
				memcpy(p_msg,cfgval.c_str(),msg.length());
				ret = updatefile(SYSM_AUT_FILE,p_msg,msg.length());
	
				ret = pThis->get_time(time);
				sscanf(time,"%ld",&(pThis->m_pATRZT_T.AuthStartData));
				pThis->SaveSystemCfg();
				ret = pThis->DecryptionAutMsg();	
				ret = pThis->DetermineExpirationDate();	
			break;
				
			default:

			break;
        }
		if(NULL != pAutOutMsg)
        {
            free(pAutOutMsg);
            pAutOutMsg = NULL;
        }
	}
	pThis->m_nAuthThrs = INVALID_THREAD_FD;
    return NULL;
}
int Authorization::PutAutQueMsg(int opCode, byte * msg, uint len)
{
	AutOutMsg_T* pAutMsg = (AutOutMsg_T*)malloc(sizeof(AutOutMsg_T) + len);
    CHK_NULL(pAutMsg, ERR);
    
    pAutMsg->nLen = len;
    pAutMsg->opCode = opCode;
    memcpy(pAutMsg->pMsg , msg, len);
    
    QUEUE_PUSH(m_pAutOutMsg, pAutMsg);
    return OK;

}
int Authorization::SendAutOpRsp(std :: string msg)
{	
	m_pAutOutCallback((byte*)&msg[0], msg.length());
	Authorization_log("SendAutOpRsp:%s,%d", (char*)&msg[0], msg.length());
	return OK;
}

int Authorization::SendAutOpRsp(byte * msg, uint len)
{
	m_pAutOutCallback(msg,len);
	Authorization_log("SendAutOpRsp:%s", (char*)msg);
	return OK;
}

int Aut_Init()
{
	int ret = ERR;
	Authorization_log("Aut Init called.");

	Authorization* pAuthorization = Authorization::GetInstance();
	CHK_NULL_PTR(pAuthorization, ERR);

	ret = pAuthorization->Init();
	CHK_ERR(ret, ERR);

	Authorization_log("Aut Init ended.");
	return OK;
}
