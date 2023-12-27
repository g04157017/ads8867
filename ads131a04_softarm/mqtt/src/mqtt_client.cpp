#include "mqtt_client.h"

void MosquittoPub_dbg(const char* format, ...)
{
	printf  ("[Mosquitto_client]: ");
	va_list vp;
	va_start(vp, format);
	vprintf (format, vp);
	va_end  (vp);
	printf  ("\n");
}

void mosq_print_for_adc_data_dbg(char *data, int leng)
{
	for (int i = 1; i <= leng; i++)
		{
			printf("%c", data[i-1]);
			if(i % 140 == 0)
			{
				printf("\n");
			}
		}
	printf("\n\n");
}

MosquittoClient* MosquittoClient::m_MosquittoClient = NULL_PTR;
MosquittoClient* MosquittoClient::GetInstance()
{
	if (NULL_PTR == m_MosquittoClient)
		{
			m_MosquittoClient = new MosquittoClient();
		}
	return (m_MosquittoClient == NULL_PTR) ? NULL_PTR : m_MosquittoClient;
}

MosquittoClient::MosquittoClient():
				m_bStart(false),
				m_Thread(INVALID_THREAD_FD),
				m_nState(MISUNCONNECT_ALARMUNCONNECT)
{
	p_sendBuff = NULL;
	Qos = 1;
	TimeOut = 10;
}

MosquittoClient::~MosquittoPub()
{
	MqttClientDeInit();
}

int MosquittoClient::MqttClientDeInit()
{
	m_bStart = false;
    usleep(CFG_SYS_WAIT_USECOND);
    m_nState = MISUNCONNECT_ALARMUNCONNECT;
	client_disconnect();
	free(p_sendBuff);//释放内存，防止内存泄漏
	free(adressGet);

	return OK;
}

int MosquittoClient::get_conn_net_status()
{
	if(MQTTClient_isConnected(client) == true)   /* 检测连接状态 */
      {
        MosquittoClient_log("alive \n");
      }
	  else{
        MosquittoClient_log(" no alive \n");
        return ERR;
      }
	return OK;
}
// 注：如需获取网站IP地址，参数填写域名即可，不需加"http://"
int MosquittoClient::socket_resolver(const char *domain, char* ipaddr)
{
    if (!domain || !ipaddr) return -1;

    struct hostent* host=gethostbyname(domain);
    if (!host)
    {
        return ERR;
    }
    // 获取第一个IP地址
    strncpy(ipaddr, inet_ntoa(*(in_addr*)host->h_addr), 16);
    return OK;
}
int MosquittoClient::set_ipadress()
{
	char ip[16] = {0};
	int ret;

	ret = socket_resolver(m_RealmName, ip);
	if(ret == ERR){
		for(int i =0;i<3;i++){
			ret = socket_resolver(m_RealmName, ip);
			if(ret == OK)  break;
		}
	}
	sprintf(adressGet,"%s:%d",ip,Port);

	return OK;
}

int MosquittoClient::mosq_config_init(void)
{
	int ret 		=	0;
	bool res		 = false;
	char cfgkey[200] = {0};
	
	pcstr mosqtcfg 	= NULL;
	
	std::string msg;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");
	CHK_ERR(ret, ERR);

	sprintf(cfgkey, CFG_MSQT CFG_MSQT_NAME);
	mosqtcfg = pIni->GetString(cfgkey);
	if (NULL != mosqtcfg)
    {
		std::string strmsg(mosqtcfg);
        CJsonObject mosqutjson(strmsg);
		
		res = mosqutjson.Get("RealmName",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_RealmName,msg.c_str());
		
		res = mosqutjson.Get("MisTopic",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_MisTopic,msg);

		res = mosqutjson.Get("AlarmTopic",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_AlarmTopic,msg);
		
		res = mosqutjson.Get("port",msg);
		CHK_TRUE(res, ERR);
		sscanf(msg.c_str(),"%d",&Port);

		res = mosqutjson.Get("Qos",msg);
		CHK_TRUE(res, ERR);
		sscanf(msg.c_str(),"%d",&Qos);

		res = mosqutjson.Get("TimeOut",msg);
		CHK_TRUE(res, ERR);
		sscanf(msg.c_str(),"%d",&TimeOut);
	}
	
	sprintf(cfgkey, CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME);
	mosqtcfg = pIni->GetString(cfgkey);
	if (NULL != mosqtcfg)
    {
		std::string strmsg(mosqtcfg);
        CJsonObject sysjson(strmsg);
		
		res = sysjson.Get("Dev_ID",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_DevID,msg.c_str());
	}

	sprintf(m_MisClientId, "%s""Mis", m_DevID);
	sprintf(m_AlarmClientId, "s%""Alarm", m_DevID);

	MosquittoClient_log("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	MosquittoClient_log("Mosquitt was = %s,%s,%s",m_RealmName,m_MisTopic,m_AlarmTopic);
	MosquittoClient_log("Mosquitt was = %s , %s",m_MisClientId,m_AlarmClientId);
	MosquittoClient_log("Mosquitt was = %d,%d,%d,%s",Port,Qos,TimeOut,m_DevID);
	MosquittoClient_log("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

	pIni->UnLoadIni();
    CHK_ERR(ret, ERR);

	return OK;
	
}

int MosquittoClient::MqttClientInit()
{
	int ret = ERR;
	int max_ID;
	MosquittoClient_log("MqttClientInit start !");
	
	adressGet = (char *)malloc(30*sizeof(char));
	CHK_NULL(adressGet,ERR);
	
	ret = mosq_config_init();
	CHK_ERR(ret, ERR);
	
	p_sendBuff = (char *)malloc(SSOCKET_Client_BUFF_LEN*sizeof(char));
	CHK_NULL(p_sendBuff,ERR);
	
	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, MAX_TH_STACK_LEN);

    m_bStart = true;
    ret = pthread_create(&m_Thread, &thread_attr, MosquittoClient::MqttClientProc, this);
    if(ret < 0)
    {
        m_bStart = false;
        MosquittoClient_log("MosquittoPub thread create fail.");
        return ERR;
    }

    ret = pthread_attr_destroy(&thread_attr); 
    if(ret != 0)
    {
        return ERR;
    }

    MosquittoClient_log("MosquittoPub create server theread success.");
 
    return OK;  
}

int MosquittoClient::client_MIS_connect()
{
	int ret;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	ret = set_ipadress();
	MosquittoClient_log("pub_client_connect  adress is %s",adressGet);
	
	MQTTClient_create(&m_Misclient, adressGet, m_MisClientId,MQTTCLIENT_PERSISTENCE_NONE, NULL);

	conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
	conn_opts.username = "kunyue";
	conn_opts.password = "kunyue@123";
	
    if ((ret = MQTTClient_connect(m_Misclient, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        MosquittoClient_log("Failed to client_MIS_connect, return code %d\n", ret);
        return ERR;
    }
	
	MosquittoClient_log("client_MIS_connect success !");
	usleep(10000); 
	m_nState = MOSQUTTCONNECTED;
    return OK;
}

int MosquittoClient::client_ALARM_connect()
{
	int ret;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	ret = set_ipadress();
	MosquittoClient_log("pub_client_connect  adress is %s",adressGet);
	
	MQTTClient_create(&m_Alarmclient, adressGet, m_AlarmClientId,MQTTCLIENT_PERSISTENCE_NONE, NULL);

	conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
	conn_opts.username = "kunyue";
	conn_opts.password = "kunyue@123";
	
    if ((ret = MQTTClient_connect(m_Alarmclient, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        MosquittoClient_log("Failed to client_ALARM_connect, return code %d\n", ret);
        return ERR;
    }
	
	MosquittoClient_log("client_ALARM_connect success !");
	usleep(10000); 
	m_nState = MOSQUTTCONNECTED;
    return OK;
}

void MosquittoClient::client_disconnect(void)
{
	 MQTTClient_disconnect(client, 10000);
     MQTTClient_destroy(&client);
	 m_nState = MISUNCONNECT_ALARMUNCONNECT;
}

void* MosquittoClient::MqttClientProc(void* data)
{
	int ret = ERR;
	MosquittoClient* pThis = (MosquittoClient*)data;
	while(pThis->m_bStart)
		{
			switch(pThis->m_nState)
        	{
            	case MISUNCONNECT_ALARMUNCONNECT:
            	{
            		ret = pThis->client_connect();
					if(ERR == ret)
					{
						MosquittoClient_log("MosquittoPub connect fail, retry after 4 second ...");
						sleep(4);
					}
					break;
            	}
				case MOSQUTTCONNECTED:
				default:
				{	
					sleep(4);
					ret = pThis->MosquittoClientStat();
					if(ERR == ret)
					{	
						MosquittoClient_log("MosquittoPub client connection was disconnected.");
					}
				}
				break;
			}
		}
	pThis->m_Thread = INVALID_THREAD_FD;
    pThis->MqttClientDeInit();
    return NULL_PTR;
}

int MosquittoClient::MosquittoClientStat()
{
	int ret = ERR;
	ret = get_conn_net_status();
    if(ret == ERR) {
     	m_nState = MISUNCONNECT_ALARMUNCONNECT;
	 	return ERR;
    } 
	return OK;
}
int MosquittoClient::Send_mqtt_MIS_msg( char* msg,uint Len)
{
	int ret;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)msg;
	MosquittoClient_log("m_sendBuff = s%",msg);
    pubmsg.payloadlen = Len;

    pubmsg.qos = Qos;
	MosquittoClient_log("Qos = %d",Qos);
    pubmsg.retained = 0;
    ret = MQTTClient_publishMessage(m_Misclient, m_MisTopic, &pubmsg, &token); /* 发送消息 */
	MosquittoClient_log("MQTTClient_publishMessage return rc = %d",ret);
	MosquittoClient_log("Waiting for up to %d seconds for publication of %s\n on topic %s for client with ClientID: %s\n",	(int)(TimeOut/1000), Payload, Topic, ClientId);
	ret = MQTTClient_waitForCompletion(m_Misclient, token, TimeOut);
	
    return 0;
}

int MosquittoClient::Send_mqtt_ALARM_msg( char* msg,uint Len)
{
	int ret;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)msg;
	MosquittoClient_log("m_sendBuff = s%",msg);
    pubmsg.payloadlen = Len;

    pubmsg.qos = Qos;
	MosquittoClient_log("Qos = %d",Qos);
    pubmsg.retained = 0;
    ret = MQTTClient_publishMessage(client, Topic, &pubmsg, &token); /* 发送消息 */
	MosquittoClient_log("MQTTClient_publishMessage return rc = %d",ret);
	MosquittoClient_log("Waiting for up to %d seconds for publication of %s\n on topic %s for client with ClientID: %s\n",	(int)(TimeOut/1000), Payload, Topic, ClientId);
	ret = MQTTClient_waitForCompletion(client, token, TimeOut);
	
    return 0;
}


int Mosquittoclient_init()
{
	int ret = ERR;
	MosquittoClient_log("MosquittoPub_init called .");

	MosquittoClient* pMosquittoPub = MosquittoClient::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);
	
	ret = pMosquittoPub->MqttClientInit();
	if(ret != OK)
	{
		MosquittoClient_log("MosquittoPub_init faile  .");
		return ERR;
	}else
	MosquittoClient_log("MosquittoPub_init ended .");
	
	return OK;
}
