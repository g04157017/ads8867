#include "otaload.h"

void OTA_cfg_dbg(const char* format, ...)
{
	printf  ("[OTA_CFG]: ");
	va_list vp;
	va_start(vp, format);
	vprintf (format, vp);
	va_end  (vp);
	printf  ("\n");
	return;
}

OTACfg* OTACfg::m_pOTACfgCfg = NULL_PTR;

OTACfg* OTACfg::GetInstance()
{
    if (NULL_PTR == m_pOTACfgCfg)
    {
        m_pOTACfgCfg = new OTACfg();
    }
    
    return (m_pOTACfgCfg == NULL_PTR) ? NULL_PTR : m_pOTACfgCfg;
}

OTACfg::~OTACfg()
{
	DeInit();
}

int OTACfg::regSendCallback(OTAOutCallback callback)
{
	m_OTACfgOutCallback = callback;
	return OK;	
}

int OTACfg::Init()
{
	int ret =  0;
	
    return OK;
}

int OTACfg::DeInit()
{
    return OK;
}

int OTASendRspStub(byte* data, uint len)
{
    OTA_cfg_log("OTACfgSendRspStub:%s,len:%d",data,len);
    return 0;
}
OTACfg::OTACfg()
{

	
	DevID					= {0};
	m_OTACfgOutCallback 	= OTASendRspStub;
}

int OTACfg::OTACfgConfig_Proc(int opCode, byte * Msg, uint len)
{
	int ret = ERR;
	
    switch(opCode)
    {
		case OTAQueryConfiguration:
			ret = OTAQueryCFG();
		break;
		case OTAUpdateConfiguration:
			ret = OTAUpdateCFG(Msg);
		break;
			
		default:

		break;
    }
	return OK;
}

int OTACfg::OTAQueryCFG()
{
	int ret = ERR;
	bool res = false;
    int cfgval = 0.00;

	CJsonObject resjson;
	
    CJsonObject recobj;
	
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
	
    ret = pIni->LoadIni("config.ini");
    CHK_ERR(ret, ERR);
	
    std::string systemStr = "";
	
    ret = pIni->GetString(CFG_MSQT CFG_MSQT_NAME, systemStr);
  

	recobj.Add("mosqcfg", systemStr);

	ret = pIni->GetString(CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME, systemStr);

	recobj.Add("system", systemStr);

	ret = pIni->GetString(CFG_AUT CFG_AUT_NAME, systemStr);

	recobj.Add("aut", systemStr);

	ret = pIni->GetString(CFG_SAMP CFG_SAMP_NAME, systemStr);

	recobj.Add("samp", systemStr);

	ret = pIni->GetString(CFG_ALARM CFG_ALARM_NAME, systemStr);

	recobj.Add("alarm", systemStr);

	ret = pIni->GetString(CFG_SENSORDATATYPE CFG_SENSORDATATYPE_NAME, systemStr);

	recobj.Add("sensordatatype", systemStr);

	ret = pIni->GetString(CFG_SEC_ETHERNET CFG_SEC_ETHERNET_NAME, systemStr);

	recobj.Add("bridge0", systemStr);

	ret = pIni->GetString(CFG_SEC_ETHERNET CFG_ETH_NAME, systemStr);

	recobj.Add("eth0:1", systemStr);

	resjson.AddEmptySubArray(REC);
    resjson[REC].Add(recobj);
	
	pIni->UnLoadIni();

	resjson.Add(RES, 0);
    resjson.Add(MSG, std::string("Remote QueryCFG operation success!"));
	
	SendOTACfgOpRsp(resjson.ToString());
	
    return OK;
}

int OTACfg::OTAUpdateCFG(byte * Msg)
{
	int ret = ERR;

	ret = UpdateMosq(Msg);
	CHK_ERR(ret, ERR);

	ret = UpdateAlarm(Msg);
	CHK_ERR(ret, ERR);

	ret = UpdateSystem(Msg);
	CHK_ERR(ret, ERR);
	
	ret = UpdateSamp(Msg);
	CHK_ERR(ret, ERR);

	ret = UpdateSenSorDT(Msg);
	CHK_ERR(ret, ERR);

	ret = UpdateETH(Msg);
	CHK_ERR(ret, ERR);
	
	while (system("reboot") < 0) {
        OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}
	
	return OK;
}
int	OTACfg::UpdateMosq(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char mosq[CFG_KEY_MAX_LEN] = {0};
    CJsonObject recobj;

	std :: string p_msg;

	pcstr mosqcfg = NULL;
	
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");  
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(mosq, CFG_MSQT CFG_MSQT_NAME);        
	mosqcfg = pIni->GetString(mosq);

//	OTA_cfg_log("UpdateMosq come in !");
	
	if (NULL != mosqcfg)
	{
		std::string strmsg(mosqcfg);
		CJsonObject CheckMSG(strmsg);

//		OTA_cfg_log("UpdateMosq come in becaus UpdateMosq");

		p_msg.clear();
		ret = resetJson.Get("RealmName",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			OTA_cfg_log("p_msg = %s",p_msg.c_str());
			OTA_cfg_log("UpdateMosq come in becaus p_msg !=	0 && !p_msg.empty()");
			recobj.Add("RealmName",p_msg);
		}else{
				ret = CheckMSG.Get("RealmName",		 p_msg);
				CHK_ERR(ret, ERR);
				OTA_cfg_log("p_msg = %s",p_msg.c_str());
				recobj.Add("RealmName", p_msg);
				OTA_cfg_log("UpdateMosq come in becaus p_msg ==	0 && p_msg.empty()");
			}
		p_msg.clear();
		ret = resetJson.Get("port",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
//			OTA_cfg_log("p_msg = %s",p_msg.c_str());
//			OTA_cfg_log("UpdateMosq come in becaus p_msg !=	0 && !p_msg.empty()");
			recobj.Add("port",p_msg);
		}else{
				ret = CheckMSG.Get("port",		 p_msg);
				CHK_ERR(ret, ERR);
//				OTA_cfg_log("p_msg = %s",p_msg.c_str());
				recobj.Add("port", p_msg);
//				OTA_cfg_log("UpdateMosq come in becaus p_msg ==	0 && p_msg.empty()");
			}
		p_msg.clear();
		ret = resetJson.Get("Topic",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("Topic",p_msg);
		}else{
				ret = CheckMSG.Get("Topic",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("Topic", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("MisTopic",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("MisTopic",p_msg);
		}else{
				ret = CheckMSG.Get("MisTopic",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("MisTopic", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("AlarmTopic",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("AlarmTopic",p_msg);
		}else{
				ret = CheckMSG.Get("AlarmTopic",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("AlarmTopic", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("Qos",p_msg);
		CHK_ERR(ret, ERR);
		
		if(!p_msg.empty()){
			recobj.Add("Qos",p_msg);
		}else{
				ret = CheckMSG.Get("Qos",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("Qos", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("UserName",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("UserName",p_msg);
		}else{
				ret = CheckMSG.Get("UserName",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("UserName", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("Password",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("Password",p_msg);
		}else{
				ret = CheckMSG.Get("Password",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("Password", p_msg);
			}
		p_msg.clear();
		ret = resetJson.Get("TimeOut",p_msg);
		CHK_ERR(ret, ERR);

		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("TimeOut",p_msg);
		}else{
				ret = CheckMSG.Get("TimeOut",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("TimeOut", p_msg);
			}
	}

	pIni->SetVal(mosq, recobj.ToString().c_str());
	
    ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
    if(ERR == ret)
    {
        OTA_cfg_log("OTACfg::UpdateMosq: Save to ini fail.");
    }
    pIni->UnLoadIni();
	
	while (system("sync") < 0) {
        OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}

	return OK;
}
int	OTACfg::UpdateSystem(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char sysname[CFG_KEY_MAX_LEN] = {0};
    CJsonObject recobj;

	std :: string p_msg;

	pcstr syscfg = NULL;
	
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");  
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(sysname, CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME);        
	syscfg = pIni->GetString(sysname);

	OTA_cfg_log("now come in UpdateSystem");
	
	if (NULL != syscfg)
	{
		std::string strmsg(syscfg);
		CJsonObject CheckMSG(strmsg);

		
		p_msg.clear();
		ret = resetJson.Get("Dev_ID",p_msg);
		CHK_ERR(ret, ERR);

		OTA_cfg_log("now come in UpdateSystem NULL != syscfg");
		
		if(p_msg !=	"0" && !p_msg.empty()){
			OTA_cfg_log("now come in 11111");
			OTA_cfg_log("Dev_ID = %s",p_msg.c_str());
			recobj.Add("Dev_ID",p_msg);
		}else{
				ret = CheckMSG.Get("Dev_ID",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("Dev_ID", p_msg);
				
				OTA_cfg_log("now come in 22222");
				OTA_cfg_log("Dev_ID = %s",p_msg.c_str());
			}

		p_msg.clear();
		ret = resetJson.Get("SendTimeDlyMin",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			
			OTA_cfg_log("now come in 11111");
			OTA_cfg_log("SendTimeDlyMin = %s",p_msg.c_str());
			recobj.Add("SendTimeDlyMin",p_msg);
		}else{
				ret = CheckMSG.Get("SendTimeDlyMin",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("SendTimeDlyMin", p_msg);

				OTA_cfg_log("now come in 22222");
				OTA_cfg_log("SendTimeDlyMin = %s",p_msg.c_str());
			}
	}

	pIni->SetVal(sysname, recobj.ToString().c_str());
	
    ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
    if(ERR == ret)
    {
        OTA_cfg_log("OTACfg::UpdateSys: Save to ini fail.");
    }
    pIni->UnLoadIni();
	
	while (system("sync") < 0) {
        OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}

	return OK;
}

int	OTACfg::UpdateSamp(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char sampname[CFG_KEY_MAX_LEN] = {0};
	CJsonObject recobj;

	std :: string p_msg;

	pcstr syscfg = NULL;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");	
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(sampname, CFG_SAMP CFG_SAMP_NAME);		 
	syscfg = pIni->GetString(sampname);
		
	if (NULL != syscfg)
	{
		std::string strmsg(syscfg);
		CJsonObject CheckMSG(strmsg);

		p_msg.clear();
		ret = resetJson.Get("SampTime",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("SampTime",p_msg);
		}else{
				ret = CheckMSG.Get("SampTime",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("SampTime", p_msg);
			}

		p_msg.clear();
		ret = resetJson.Get("SampFreq",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("SampFreq",p_msg);
		}else{
				ret = CheckMSG.Get("SampFreq",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("SampFreq", p_msg);
			}
	}

	pIni->SetVal(sampname, recobj.ToString().c_str());
	
	ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
	if(ERR == ret)
	{
		OTA_cfg_log("OTACfg::UpdateSamp: Save to ini fail.");
	}
	pIni->UnLoadIni();
	
	while (system("sync") < 0) {
		OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}

	return OK;
}
int	OTACfg::UpdateSenSorDT(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char SenSorname[CFG_KEY_MAX_LEN] = {0};
	CJsonObject recobj;

	std :: string p_msg;

	pcstr syscfg = NULL;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");	
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(SenSorname, CFG_SENSORDATATYPE CFG_SENSORDATATYPE_NAME);		 
	syscfg = pIni->GetString(SenSorname);
		
	if (NULL != syscfg)
	{
		std::string strmsg(syscfg);
		CJsonObject CheckMSG(strmsg);

		p_msg.clear();
		ret = resetJson.Get("sensordatatype",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("sensordatatype",p_msg);
		}else{
				ret = CheckMSG.Get("sensordatatype",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("sensordatatype", p_msg);
			}
	}

	pIni->SetVal(SenSorname, recobj.ToString().c_str());
	
	ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
	if(ERR == ret)
	{
		OTA_cfg_log("OTACfg::UpdateSenSorTYPE: Save to ini fail.");
	}
	pIni->UnLoadIni();
	
	while (system("sync") < 0) {
		OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}
	return OK;
}
int	OTACfg::UpdateETH(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char ethname[CFG_KEY_MAX_LEN] = {0};
	CJsonObject recobj;

	std :: string p_msg;

	pcstr syscfg = NULL;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");	
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(ethname, CFG_SEC_ETHERNET CFG_SEC_ETHERNET_NAME);		 
	syscfg = pIni->GetString(ethname);
		
	if (NULL != syscfg)
	{
		std::string strmsg(syscfg);
		CJsonObject CheckMSG(strmsg);

		p_msg.clear();
		ret = resetJson.Get("brgipaddress",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("ipaddress",p_msg);
		}else{
				ret = CheckMSG.Get("ipaddress",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("ipaddress", p_msg);
			}

		p_msg.clear();
		ret = resetJson.Get("brgnetmask",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("netmask",p_msg);
		}else{
				ret = CheckMSG.Get("netmask",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("netmask", p_msg);
			}

		p_msg.clear();
		ret = resetJson.Get("gate",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("gate",p_msg);
		}else{
				ret = CheckMSG.Get("gate",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("gate", p_msg);
			}
	}

	pIni->SetVal(ethname, recobj.ToString().c_str());

	recobj.Clear();
	
	sprintf(ethname, CFG_SEC_ETHERNET CFG_ETH_NAME);		 
	syscfg = pIni->GetString(ethname);

	if (NULL != syscfg)
	{
		std::string strmsg(syscfg);
		CJsonObject CheckMSG1(strmsg);

		p_msg.clear();
		ret = resetJson.Get("eth1ipaddress",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("ipaddress",p_msg);
		}else{
				ret = CheckMSG1.Get("ipaddress",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("ipaddress", p_msg);
			}

		p_msg.clear();
		ret = resetJson.Get("eth1netmask",p_msg);
		CHK_ERR(ret, ERR);
		
		if(p_msg !=	"0" && !p_msg.empty()){
			recobj.Add("netmask",p_msg);
		}else{
				ret = CheckMSG1.Get("netmask",		 p_msg);
				CHK_ERR(ret, ERR);
				recobj.Add("netmask", p_msg);
			}

	}

	pIni->SetVal(ethname, recobj.ToString().c_str());
	
	ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
	if(ERR == ret)
	{
		OTA_cfg_log("OTACfg::UpdateEth: Save to ini fail.");
	}
	pIni->UnLoadIni();
	
	while (system("sync") < 0) {
		OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}

	return OK;
}

int	OTACfg::UpdateAlarm(byte * Msg)
{
	float cfgval = 0;
	int ret = ERR;
	
	char alarm[CFG_KEY_MAX_LEN] = {0};
    CJsonObject recobj;
	pcstr alarmcfg = NULL;
   
    IniExt* pIni = IniExt::GetInstance();
    CHK_NULL_PTR(pIni, ERR);
    ret = pIni->LoadIni("config.ini");  
	CHK_ERR(ret, ERR);
	
	std::string reqmsg((const char*)Msg);
	CJsonObject resetJson(reqmsg);

	sprintf(alarm, CFG_ALARM CFG_ALARM_NAME);        
	alarmcfg = pIni->GetString(alarm);
	
	if (NULL != alarmcfg)
    {
		std::string strmsg(alarmcfg);
        CJsonObject alarmjson(strmsg);

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC1_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
    		recobj.Add(SPEED_EFFECTIVE_AC1_H, (int)100*cfgval);
		}else{
				ret = alarmjson.Get(SPEED_EFFECTIVE_AC1_H,		 cfgval);
				CHK_ERR(ret, ERR);
				recobj.Add(SPEED_EFFECTIVE_AC1_H, (int)cfgval);
			}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC1_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC1_L, (int)100*cfgval);	
		}else{
				ret = alarmjson.Get(SPEED_EFFECTIVE_AC1_L,		 cfgval);
				CHK_ERR(ret, ERR);
				recobj.Add(SPEED_EFFECTIVE_AC1_L, (int)cfgval);
			}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC2_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC2_H, (int)100*cfgval);
		}else{
				ret = alarmjson.Get(SPEED_EFFECTIVE_AC2_H,		 cfgval);
				CHK_ERR(ret, ERR);
				recobj.Add(SPEED_EFFECTIVE_AC2_H, (int)cfgval);
			}
		
		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC2_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC2_L, (int)100*cfgval);
		}else{
				ret = alarmjson.Get(SPEED_EFFECTIVE_AC2_L,		 cfgval);
				CHK_ERR(ret, ERR);
				recobj.Add(SPEED_EFFECTIVE_AC2_L, (int)cfgval);
			}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC3_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(SPEED_EFFECTIVE_AC3_H,(int)100*cfgval);
		}else{
				ret = alarmjson.Get(SPEED_EFFECTIVE_AC3_H,		 cfgval);
				CHK_ERR(ret, ERR);
				recobj.Add(SPEED_EFFECTIVE_AC3_H, (int)cfgval);
			}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC3_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC3_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(SPEED_EFFECTIVE_AC3_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(SPEED_EFFECTIVE_AC3_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC4_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC4_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(SPEED_EFFECTIVE_AC4_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(SPEED_EFFECTIVE_AC4_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(SPEED_EFFECTIVE_AC4_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
			recobj.Add(SPEED_EFFECTIVE_AC4_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(SPEED_EFFECTIVE_AC4_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(SPEED_EFFECTIVE_AC4_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(TEMPERATURE_TP1_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(TEMPERATURE_TP1_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(TEMPERATURE_TP1_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(TEMPERATURE_TP1_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(TEMPERATURE_TP1_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(TEMPERATURE_TP1_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(TEMPERATURE_TP1_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(TEMPERATURE_TP1_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(TEMPERATURE_TP2_H,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(TEMPERATURE_TP2_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(TEMPERATURE_TP2_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(TEMPERATURE_TP2_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(TEMPERATURE_TP2_L,		 cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(TEMPERATURE_TP2_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(TEMPERATURE_TP2_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(TEMPERATURE_TP2_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC1_H,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC1_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC1_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC1_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC1_L,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC1_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC1_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC1_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC2_H,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC2_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC2_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC2_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC2_L,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC2_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC2_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC2_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC3_H,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC3_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC3_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC3_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC3_L,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC3_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC3_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC3_L, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC4_H,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC4_H, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC4_H,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC4_H, (int)cfgval);
		}

		cfgval	=	0;
		ret = resetJson.Get(PEAKTOPEAK_AC4_L,		cfgval);
		CHK_ERR(ret, ERR);
		
		if(cfgval	!=	0){
		recobj.Add(PEAKTOPEAK_AC4_L, (int)100*cfgval);
		}else{
			ret = alarmjson.Get(PEAKTOPEAK_AC4_L,		 cfgval);
			CHK_ERR(ret, ERR);
			recobj.Add(PEAKTOPEAK_AC4_L, (int)cfgval);
		}
	}

	pIni->SetVal(alarm, recobj.ToString().c_str());

	
    ret = pIni->SaveIni("config.ini"); 
	CHK_ERR(ret, ERR);
    if(ERR == ret)
    {
        OTA_cfg_log("AlarmCfg::DwnAddArp: Save to ini fail.");
    }
    pIni->UnLoadIni();
	
	while (system("sync") < 0) {
        OTA_cfg_log("cmd: sync\t error: %s", strerror(errno));
		return ERR;
	}

	return OK;
}


int OTACfg::OTACfg_Send_Msg()
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
	SendOTACfgOpRsp(resjson.ToString());
		
	return OK;
}
int OTACfg::get_time(char * time)
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
int OTACfg::SendOTACfgOpRsp(std :: string msg)
{
	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);

	pMosquittoPub->publish_MIS_msg((char*)&msg[0], msg.length());
	OTA_cfg_log("SendAlarmOpRsp:%s,%d", (char*)&msg[0], msg.length());
	return OK;
}
int OTACfgCfg_Init()
{
	int ret = ERR;
	OTACfg * pOTACfgCfg = OTACfg::GetInstance();
	CHK_NULL_PTR(pOTACfgCfg, ERR);

	ret = pOTACfgCfg->Init();
	CHK_ERR(ret, ERR);
	
	OTA_cfg_log("OTACfg  Init ended.");
	
	return OK;
}


