#include "mqtt.h"

const char* FILE_test_NAME = "/usr/local/log/send.log";
MqttMsg_T* m_pMqttMsg;
volatile MQTTClient_deliveryToken deliveredtoken;


void MosquittoPub_dbg(const char* format, ...)
{
	printf  ("[MosquittoPub]: ");
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

int MosquittoPubRecvStub(byte* data, uint datalen)
{        
    MosquittoPub_log("stub received data:%s\n", (char*)data);
    MosquittoPub_log("stub received datalen = %d\n", datalen);
    return OK;
}
MosquittoPub* MosquittoPub::m_MosquittoPub = NULL_PTR;

MosquittoPub* MosquittoPub::GetInstance()
{
	if (NULL_PTR == m_MosquittoPub)
		{
			m_MosquittoPub = new MosquittoPub();
		}
	return (m_MosquittoPub == NULL_PTR) ? NULL_PTR : m_MosquittoPub;
}

MosquittoPub::MosquittoPub():
				m_bStart(false),
				m_Thread(INVALID_THREAD_FD),
				m_nState(UNCONNECT),
				m_nUnconnect(UNCONNECT),
				m_nRecvLen(0),
				m_nSendLen(0)
{
	Adress = NULL;
	p_sendBuff = NULL;
	Qos = 1;
	TimeOut = 10;
	SampTime = 0;
	m_pMqttMsg = NULL_PTR;
	MosquittoPubRecv = MosquittoPubRecvStub;
}

MosquittoPub::~MosquittoPub()
{
	MqttDeInit();
}

int MosquittoPub::MqttDeInit()
{
	m_bStart = false;
    usleep(CFG_SYS_WAIT_USECOND);
    m_nState = UNCONNECT;
	QUEUE_FREE(m_pMqttMsg);
	pub_client_disconnect();
	free(p_sendBuff);//释放内存，防止内存泄漏
	free(adressGet);
    MosquittoPubRecv = MosquittoPubRecvStub;

	return OK;
}

int MosquittoPub::regRecvCallback(MosquittoPubCallback callback)
{
	MosquittoPubRecv = callback;
}

int MosquittoPub::get_conn_net_status()
{
	if(MQTTClient_isConnected(client) == true)   /* 检测连接状态 */
      {
        MosquittoPub_log("alive \n");
      }
	  else{
        MosquittoPub_log(" no alive \n");
        return ERR;
      }
	return OK;
}
// 注：如需获取网站IP地址，参数填写域名即可，不需加"http://"
int MosquittoPub::socket_resolver(const char *domain, char* ipaddr)
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
int MosquittoPub::set_ipadress()
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
	Adress = adressGet;

//	MosquittoPub_log("MQTT set the ip was %s",Adress);
	return ret;
}

int MosquittoPub::mosq_config_init(void)
{
	int ret =0;
	bool res		 = false;
	int i = 0;
	char cfgkey[200] = {0};
	pcstr mosqtcfg = NULL;
	pcstr syscfg = NULL;
	std :: string msg;
	MosqutoCFG PMosqutoCFG;
	
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
		
		res = mosqutjson.Get("RealmName",PMosqutoCFG.JRealmName);
		CHK_TRUE(res, ERR);
		strcpy( m_RealmName,PMosqutoCFG.JRealmName.c_str());
		
		res = mosqutjson.Get("Topic",PMosqutoCFG.JTopic);
		CHK_TRUE(res, ERR);
		strcpy( m_Topic,PMosqutoCFG.JTopic.c_str());

		res = mosqutjson.Get("MisTopic",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_MisTopic,msg.c_str());

		res = mosqutjson.Get("AlarmTopic",msg);
		CHK_TRUE(res, ERR);
		strcpy( m_AlarmTopic,msg.c_str());
		
		res = mosqutjson.Get("port",PMosqutoCFG.JMPort);
		CHK_TRUE(res, ERR);
		sscanf(PMosqutoCFG.JMPort.c_str(),"%d",&Port);

		res = mosqutjson.Get("Qos",PMosqutoCFG.JQos);
		CHK_TRUE(res, ERR);
		sscanf(PMosqutoCFG.JQos.c_str(),"%d",&Qos);

		res = mosqutjson.Get("TimeOut",PMosqutoCFG.JTimeOut);
		CHK_TRUE(res, ERR);
		sscanf(PMosqutoCFG.JTimeOut.c_str(),"%d",&TimeOut);

		res = mosqutjson.Get("UserName",msg);
		CHK_TRUE(res, ERR);
		strcpy(m_username,msg.c_str());
		
		res = mosqutjson.Get("Password",msg);
		CHK_TRUE(res, ERR);
		strcpy(m_password,msg.c_str());
	}
	
	sprintf(cfgkey, CFG_SEC_SYSTEM CFG_SEC_SYSTEM_NAME);
	syscfg = pIni->GetString(cfgkey);
	if (NULL != syscfg)
    {
		std::string strmsg(syscfg);
        CJsonObject sysjson(strmsg);
		res = sysjson.Get("Dev_ID",PMosqutoCFG.JDev_ID);
		CHK_TRUE(res, ERR);
		strcpy( m_DevID,PMosqutoCFG.JDev_ID.c_str());
	}

	sprintf(m_ClientId, "ClientPub""%s", m_DevID);
	sprintf(m_SubId, "%s""KYMQTT", m_DevID);

	
	sprintf(cfgkey, CFG_SENSORDATATYPE CFG_SENSORDATATYPE_NAME);
		syscfg = pIni->GetString(cfgkey);
		if (NULL != syscfg)
		{
			std::string strmsg(syscfg);
			CJsonObject sysjson(strmsg);
			res = sysjson.Get("sensordatatype",msg);
			CHK_TRUE(res, ERR);
			strcpy(m_SensorDataType,msg.c_str());
		}

	sprintf(cfgkey, CFG_SAMP CFG_SAMP_NAME);
		syscfg = pIni->GetString(cfgkey);
		if (NULL != syscfg)
		{
			std::string strmsg(syscfg);
			CJsonObject sysjson(strmsg);
			res = sysjson.Get(SAMP_TIME,msg);
			CHK_TRUE(res, ERR);
			sscanf(msg.c_str(),"%d",&SampTime);
		}

	set_ipadress();
	MosquittoPub_log("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	MosquittoPub_log("Mosquitt was = %s,%s",m_RealmName,m_Topic);
	MosquittoPub_log("Mosquitt was = %s,%s",m_ClientId,m_SubId);
	MosquittoPub_log("Mosquitt was = %d,%d,%d,%s",Port,Qos,TimeOut,m_DevID);
	MosquittoPub_log("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

	ClientId = (char *)m_ClientId;
	Dev_ID   = (char *)m_DevID;
	Sub_Id   = (char *)m_SubId;
	Topic    = (char *)m_Topic;
	Payload = (char *)"Hello World!";

	pIni->UnLoadIni();
    CHK_ERR(ret, ERR);
	
}
void MqttConnlost(void *context, char *cause)
{
	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	
	MosquittoPub_log("\nConnection lost\n");
    MosquittoPub_log("     cause: %s\n", cause);
	pMosquittoPub->m_nState = UNCONNECT;
}

int MqttMsgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;

	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);
	
	if(topicName != NULL)                          /* 滤掉心跳包 */
      {
	    MosquittoPub_log("Message arrived\n");
	    MosquittoPub_log("     topic: %s\n", topicName);
	    MosquittoPub_log("   message: ");

		mosq_print_for_adc_data_dbg((char *)message->payload,message->payloadlen);
		 
		 pMosquittoPub->MosquittoPubRecv((byte*)message->payload, message->payloadlen);//call back by app
         if(strcmp((char *)message->payload,"ESC") == 0)
          {
            MosquittoPub_log("ESC \n");
         }
      }
	
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void MqttDelivered(void *context, MQTTClient_deliveryToken dt)
{
//   printf("Message with token value %d delivery confirmed\n", dt);
   deliveredtoken = dt;
}


int MosquittoPub::MQttConnectInit()
{
	int ret=0;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	
	conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
	conn_opts.username = m_username;
	conn_opts.password = m_password;
//	conn_opts.username = "ahb";    //E	yunwei
//	conn_opts.password = "Kunyue@123";	//E yunwei
//	conn_opts.username = "kunyue";
//	conn_opts.password = "kunyue@123";

    if ((ret = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        MosquittoPub_log("Failed to connect, return code %d\n", ret);
        return ERR;
    }
	MosquittoPub_log("pub_client_connect success !");

	MosquittoPub_log("Sub_Id = %s",Sub_Id);
	if((ret = MQTTClient_subscribe(client, Sub_Id, 1)) != MQTTCLIENT_SUCCESS) /* 订阅一个客户端 的一个话题*/
	{
		MosquittoPub_log("Failed to connect subscribe, return code %d\n", ret);
        return ERR;
	}
	MosquittoPub_log("pub_client_subscribe_connect success !  Sub_Id = %s",Sub_Id);

	usleep(1000); 
	m_nState = MOSQUTTCONNECTED;
	return OK;
}

int MosquittoPub::MqttInit()
{
	int ret = ERR;
	int max_ID;
	MosquittoPub_log("MqttInit start init !");
	
	adressGet = (char *)malloc(30*sizeof(char));
	if(adressGet == NULL){
		MosquittoPub_log("malloc adressGet err !");
	}
	
	mosq_config_init();
	p_sendBuff = (char *)malloc(SSOCKET_BUFF_LEN*sizeof(char));
	if(p_sendBuff == NULL){
		MosquittoPub_log("malloc p_sendBuff err !");
	}

	
	set_ipadress();
	//	MosquittoPub_log("pub_client_connect  adress is %s",Adress);
		
	MQTTClient_create(&client, Adress, ClientId,MQTTCLIENT_PERSISTENCE_NONE, NULL);//set connect sesion
	//	MosquittoPub_log("pub_client_connect  adress is %s",Adress);
	
	 MQTTClient_setCallbacks(client, NULL, MqttConnlost, MqttMsgarrvd, MqttDelivered);// callback by message send and lost
	
	MQttConnectInit();
	
	QUEUE_INIT(MqttMsg_T,m_pMqttMsg);
	
	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, MAX_TH_STACK_LEN);

    m_bStart = true;
    ret = pthread_create(&m_Thread, &thread_attr, MosquittoPub::MqttProc, this);
    if(ret < 0)
    {
        m_bStart = false;
        MosquittoPub_log("MosquittoPub thread create fail.");
        return ERR;
    }

    ret = pthread_attr_destroy(&thread_attr); 
    if(ret != 0)
    {
        return ERR;
    }

    MosquittoPub_log("MosquittoPub create server theread success.");
 
    return OK;  
}

int MosquittoPub::pub_client_connect()
{
	int ret;

	MQTTClient_disconnect(client, 10000);
//    MQTTClient_destroy(&client);

	ret = MQttConnectInit();
	CHK_ERR(ret,ERR);
//	MosquittoPub_log("Sub_Id = %s",Sub_Id);
//	if((ret = MQTTClient_subscribe(client, Sub_Id, 1)) != MQTTCLIENT_SUCCESS) /* 订阅一个客户端 的一个话题*/
//	{
//		MosquittoPub_log("Failed to connect subscribe, return code %d\n", ret);
//        return ERR;
//	}
//	MosquittoPub_log("pub_client_subscribe_connect success !  Sub_Id = %s",Sub_Id);
    return OK;
}

int MosquittoPub::get_time(char * time)
{
	struct timeval tv;
	char time_s[10];
	std::string mtime;
	gettimeofday(&tv, NULL);
	mtime = std::to_string(tv.tv_sec);
	strcpy(time_s,mtime.c_str());
	sprintf(time,"%s%s",time_s,"000");
	return 0;
}
int MosquittoPub::get_time_S(char * time)
{
	struct timeval tv;
	char time_s[10];
	std::string mtime;
	gettimeofday(&tv, NULL);
	mtime = std::to_string(tv.tv_sec);
	strcpy(time_s,mtime.c_str());
	sprintf(time,"%s",time_s);
	return 0;
}
int MosquittoPub::DataCompression(std::string msg)
{

	 unsigned long originalSize = msg.length();
	 std::cout << "original data leng ：" << originalSize << " byte" << std::endl;
	 
// 压缩后的数据
   uLong compressedSize = compressBound(originalSize); // 获取压缩后的最大可能大小
   Bytef* compressedData = new Bytef[compressedSize];

   
   // 压缩
	  int compressionResult = compress(compressedData, &compressedSize, (const Bytef*)&msg[0], originalSize);
	  if (compressionResult != Z_OK) {
		  std::cerr << "compression fail !" << std::endl;
		  delete[] compressedData;
		  return ERR;
	  }
   
	  std::cout << "compression data leng ：" << compressedSize << " byte" << std::endl;
	  delete[] compressedData;

	 return OK;
}
void MosquittoPub::MosquittoPaload2Json(MqttMsg_T* pMqttMsg)
{
    CJsonObject resjson;
	MqttMsg_T* ppMqttMsg;
	std::string msg;
	int DataTmp;
	int ret;
	int count;
	char time[13];
	ret = get_time(time);
	
	if(m_nState == MOSQUTTCONNECTED){
		resjson.Add(OPC,APP_OPCODE_DATA_NOMOUT);
	}else{
		resjson.Add(OPC,APP_OPCODE_DATA_ABNOMOUT);
	}
	
	resjson.Add(Dev_P_ID,Dev_ID);
	resjson.Add(SEQ,111111111);
	resjson.Add(TMS,time);
	MosquittoPub_log("time is = %s",time);
	resjson.Add(SSDT,m_SensorDataType);
	
	resjson.Add(VIB1TIM,SampTime);
	resjson.Add(VIB2TIM,SampTime);
	resjson.Add(VIB3TIM,SampTime);
	resjson.Add(VIB4TIM,SampTime);

	CJsonObject recobj;
	recobj.Clear();
	recobj.Add(NM,	SK1);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->TS1Msg_s[count]);
		count++;
		}while(count<pMqttMsg->TS1Msg_sLen);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);

	recobj.Clear();
	recobj.Add(NM,	SK2);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->TS2Msg_s[count]);
		count++;
		}while(count<pMqttMsg->TS2Msg_sLen);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);

	recobj.Clear();
	recobj.Add(NM,	SK3);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->S1[count]);
		count++;
	}while(count<pMqttMsg->S1_Len);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);
	
	recobj.Clear();
	recobj.Add(NM,	SK4);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->S2[count]);
		count++;
	}while(count<pMqttMsg->S2_Len);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);

	recobj.Clear();
	recobj.Add(NM,	TM1);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->TS1Msg_t[count]);	
		count++;
	}while(count<pMqttMsg->TS1Msg_tLen);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);

	recobj.Clear();
	recobj.Add(NM,	TM2);
	count = 0;
	do{
		recobj.AddEmptySubArray(DT);
		recobj[DT].Add(pMqttMsg->TS2Msg_t[count]);
		count++;
	}while(count<pMqttMsg->TS2Msg_tLen);
	resjson.AddEmptySubArray(REC);
	resjson[REC].Add(recobj);
	
	msg = resjson.ToString();
	strcpy(p_sendBuff,&msg[0]);
	m_nSendLen = msg.length();
//	DataCompression(msg);
}

void MosquittoPub::AlarmMosquittoPaload2Json(MqttMsg_T* pMqttMsg)
{
    CJsonObject resjson;
	MqttMsg_T* ppMqttMsg;
	std::string msg;
	int DataTmp;
	int ret;
	int count;
	char time[13];
	ret = get_time(time);
	
	if(m_nState == MOSQUTTCONNECTED){
		resjson.Add(OPC,APP_OPCODE_DATA_NOMOUT);
	}else{
		resjson.Add(OPC,APP_OPCODE_DATA_ABNOMOUT);
	}
	
	resjson.Add(Dev_P_ID,Dev_ID);
	resjson.Add(SEQ,111111111);
	resjson.Add(TMS,time);
	MosquittoPub_log("time is = %s",time);
	resjson.Add(SSDT,m_SensorDataType);
	CJsonObject recobj;
	if(pMqttMsg->STA.TS1_S_STA)
	{
		resjson.Add(VIB1TIM,SampTime);
		recobj.Clear();
		recobj.Add(NM,	SK1);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->TS1Msg_s[count]);
			count++;
			}while(count<pMqttMsg->TS1Msg_sLen);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	if(pMqttMsg->STA.TS2_S_STA)
	{
		resjson.Add(VIB2TIM,SampTime);
		recobj.Clear();
		recobj.Add(NM,	SK2);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->TS2Msg_s[count]);
			count++;
			}while(count<pMqttMsg->TS2Msg_sLen);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	if(pMqttMsg->STA.S1_STA)
	{
		resjson.Add(VIB3TIM,SampTime);
		recobj.Clear();
		recobj.Add(NM,	SK3);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->S1[count]);
			count++;
		}while(count<pMqttMsg->S1_Len);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	if(pMqttMsg->STA.S2_STA)
	{
		resjson.Add(VIB4TIM,SampTime);
		recobj.Clear();
		recobj.Add(NM,	SK4);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->S2[count]);
			count++;
		}while(count<pMqttMsg->S2_Len);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	if(pMqttMsg->STA.TS1_T_STA)
	{
		recobj.Clear();
		recobj.Add(NM,	TM1);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->TS1Msg_t[count]);	
			count++;
		}while(count<pMqttMsg->TS1Msg_tLen);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	if(pMqttMsg->STA.TS2_T_STA)
	{
		recobj.Clear();
		recobj.Add(NM,	TM2);
		count = 0;
		do{
			recobj.AddEmptySubArray(DT);
			recobj[DT].Add(pMqttMsg->TS2Msg_t[count]);
			count++;
		}while(count<pMqttMsg->TS2Msg_tLen);
		resjson.AddEmptySubArray(REC);
		resjson[REC].Add(recobj);
	}
	msg = resjson.ToString();
	strcpy(p_sendBuff,&msg[0]);
	m_nSendLen = msg.length();
}


int MosquittoPub::publish_mqtt_msg( char* msg,uint Len)
{
	int rc;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)msg;
    pubmsg.payloadlen = Len;

    pubmsg.qos = Qos;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, Topic, &pubmsg, &token); /* 发送消息 */
	rc = MQTTClient_waitForCompletion(client, token, TimeOut);
	
    return rc;
}

int MosquittoPub::publish_MIS_msg( char* msg,uint Len)
{
	int rc;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)msg;
    pubmsg.payloadlen = Len;

    pubmsg.qos = Qos;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, m_MisTopic, &pubmsg, &token); /* 发送消息 */
	rc = MQTTClient_waitForCompletion(client, token, TimeOut);
	
    return rc;
}

int MosquittoPub::publish_ALARM_msg( char* msg,uint Len)
{
	int rc;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)msg;
    pubmsg.payloadlen = Len;

    pubmsg.qos = Qos;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, m_AlarmTopic, &pubmsg, &token); /* 发送消息 */
	rc = MQTTClient_waitForCompletion(client, token, TimeOut);
	
    return rc;
}

int MosquittoPub::publish_mqtt_msg()
{
	int rc;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = (void *)p_sendBuff;
    pubmsg.payloadlen = m_nSendLen;

    pubmsg.qos = Qos;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, Topic, &pubmsg, &token); /* 发送消息 */
	MosquittoPub_log("publish_mqtt_msg rc1 = %d",rc);
	rc = MQTTClient_waitForCompletion(client, token, TimeOut);
	MosquittoPub_log("publish_mqtt_msg rc2 = %d",rc);
    return rc;
}

void MosquittoPub::pub_client_disconnect(void)
{
	 MQTTClient_unsubscribe(client,Sub_Id);
	 MQTTClient_disconnect(client, 10000);
     MQTTClient_destroy(&client);
	 m_nState = UNCONNECT;
}

void* MosquittoPub::MqttProc(void* data)
{
	int ret = ERR;
	int nrow=0;
	char **azResult;
	MosquittoPub* pThis = (MosquittoPub*)data;
	while(pThis->m_bStart)
		{
			switch(pThis->m_nState)
        	{
            	case UNCONNECT:
            	{
            		ret = pThis->pub_client_connect();
					if(ERR == ret)
					{
						MosquittoPub_log("MosquittoPub connect fail, retry after 10 second ...");
						sleep(10);
					}
					pThis->m_nUnconnect = UNCONNECT;	
					break;
            	}
				case MOSQUTTCONNECTED:
				default:
				{	
					sleep(60);
					ret = pThis->MosquittoPubDataProc();
					if(ERR == ret)
					{	
						MosquittoPub_log("MosquittoPub client connection was disconnected.");
					}
					if(pThis->m_nUnconnect == UNCONNECT)
					{
						//建立连接时，查询数据库中有多少条失连时存储的数据，然后进行数据从发处理，发送完成后要清空已经发送完成的数据
						DataBase * pDataBase = DataBase::GetInstance();
						
						ret = pDataBase->LoadDatabase("/home/data.db");
						
						MosquittoPub_log("MqttProc  connect , get out the database !");

						ret = pDataBase->GetLargestRow("SenSorTable", &nrow);
						
						if(nrow != 0)
						{
							ret = pDataBase->QueryAPieceOfDataFromATable("SenSorTable", &azResult, nrow);
							
							strcpy(pThis->p_sendBuff,azResult[4]);

							sscanf(azResult[5],"%d",&pThis->m_nSendLen);

							sqlite3_free_table(azResult);

							pThis->Asterisk2DoubleQuotationMark();
							
//							MosquittoPub_log("----------------%s -------------------",pThis->p_sendBuff);
//							MosquittoPub_log("----------------%d -------------------",pThis->m_nSendLen);

							ret = pThis->publish_mqtt_msg(pThis->p_sendBuff, pThis->m_nSendLen);
							
							ret = pDataBase->DeleteARowOfDataFromTheTable("SenSorTable",nrow);
						}else
							pThis->m_nUnconnect = MOSQUTTCONNECTED;

						pDataBase->UnLoadDatabase();
					}
				}
				break;
			}
		}
	pThis->m_Thread = INVALID_THREAD_FD;
    pThis->MqttDeInit();
    return NULL_PTR;
}

int MosquittoPub::subRcv_mqtt_msg(void)
{
	int rc=0;
	int i=0;
	char* topicName_rev = NULL;
	int   topicLen_rev=0;
	MQTTClient_message *receivemsg = NULL ;
	 rc = MQTTClient_receive(client,&topicName_rev, &topicLen_rev, &receivemsg,5000); /* 接收消息 */
	if(rc == MQTTCLIENT_SUCCESS)
    {
//      MosquittoPub_log("Message REv  %d delivered\n", rc);
//      MosquittoPub_log("topicName: %s  topicName_LEN: %d \n", topicName_rev,topicLen_rev);
      if(topicName_rev != NULL)                          /* 滤掉心跳包 */
      {
        MosquittoPub_log("topicName: ");
        for(i=0;i<topicLen_rev;i++)
         {
           MosquittoPub_log(" %c ", topicName_rev[i]);
        }
         MosquittoPub_log("\n");
         MosquittoPub_log("Data: %s  len:%d msgid: %d \n",(char *)receivemsg->payload,receivemsg->payloadlen,receivemsg->msgid);
		 MosquittoPubRecv((byte*)receivemsg->payload, receivemsg->payloadlen);//call back by app
         if(strcmp((char *)receivemsg->payload,"ESC") == 0)
          {
            MosquittoPub_log("ESC \n");
         }
      }
    }
	usleep(10000); 
    usleep(100000);
	return OK;
}

int MosquittoPub::MosquittoPubDataProc()
{
	int ret = ERR;
	ret = get_conn_net_status();
    if(ret == ERR) {
     	m_nState = UNCONNECT;
	 	return ERR;
    } 
//	subRcv_mqtt_msg();
	return OK;
}

int MosquittoPub::Asterisk2DoubleQuotationMark()
{
	int i =0;
	do{
		if(p_sendBuff[i]=='*')
			p_sendBuff[i] = '"';
		i++;
	}while(i<m_nSendLen);
}

int MosquittoPub::DoubleQuotationMark2Asterisk()
{
	int i =0;
	do{
		if(p_sendBuff[i]=='"')
			p_sendBuff[i] = '*';
		i++;
	}while(i<m_nSendLen);
	return OK;
}

int MosquittoPub::MqttProc_send(MqttMsg_T* pMqttMsg)
{
	int ret = ERR;
	int nrow = 0;
	char time[13] = {0};
	unsigned long MinimumTime = 0;
	if(pMqttMsg->stat == NORMAL)
		MosquittoPaload2Json(pMqttMsg);
	else
		AlarmMosquittoPaload2Json(pMqttMsg);
//	MosquittoPub_log("%s",p_sendBuff);
	MosquittoPub_log("MqttProc_send one message !");

	ret = get_conn_net_status();
    if(ret == ERR) 
	{
     	m_nState = UNCONNECT;
		
		get_time_S(time);

		sscanf(time,"%d",&MinimumTime);
		if(MinimumTime>1671085508)
		{
			DataBase * pDataBase = DataBase::GetInstance();
			ret = pDataBase->LoadDatabase("/home/data.db");
			CHK_ERR(ret,ERR);
			MosquittoPub_log("+++++++++++++++++++++++++++++++MqttProc  unconnect , store in the database !++++++++++++++++++++++++++++++++");

			DoubleQuotationMark2Asterisk();
			ret = pDataBase->AddData2Table(p_sendBuff,"SenSorTable",m_nSendLen);
			CHK_ERR(ret,ERR);

			pDataBase->UnLoadDatabase();
			
		 	return ERR;
		}else
		{
			MosquittoPub_log("MqttProc unconnect onece , NoT store !");
			return ERR;
		}
    }
	if(pMqttMsg->stat == NORMAL)
	{
		for(int i = 3;i>0;i--)
		{
			ret = publish_mqtt_msg();
			if(ret == MQTTCLIENT_SUCCESS)
				break;
			if(i==1)
			{
				DataBase * pDataBase = DataBase::GetInstance();
				ret = pDataBase->LoadDatabase("/home/data.db");
				CHK_ERR(ret,ERR);
				MosquittoPub_log("+++++++++++++++++++++++++++++++MqttProc  unconnect , store in the database !++++++++++++++++++++++++++++++++");

				DoubleQuotationMark2Asterisk();
				ret = pDataBase->AddData2Table(p_sendBuff,"SenSorTable",m_nSendLen);
				CHK_ERR(ret,ERR);

				pDataBase->UnLoadDatabase();
			}
		}

	}
	else
		ret = publish_ALARM_msg(p_sendBuff,m_nSendLen);
	return ret;
}


int MosquittoPub_init()
{
	int ret = ERR;
	MosquittoPub_log("MosquittoPub_init called .");

	MosquittoPub* pMosquittoPub = MosquittoPub::GetInstance();
	CHK_NULL_PTR(pMosquittoPub, ERR);
	
	ret = pMosquittoPub->MqttInit();
	
	MosquittoPub_log("MosquittoPub_init ended .");
	return OK;
}
