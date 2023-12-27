#ifndef mqtt_h
#define mqtt_h

#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <termios.h>
#include <iostream>
#include <fstream>


#include "def.h"
#include "config.h"
#include "MQTTClient.h"
#include "comm.h"
#include "mqttcmd.h"

#include "CJsonObject.h"
#include "iniext.h"

#include "kydatabase.h"
#include "zlib.h"


extern MqttMsg_T* m_pMqttMsg;

#ifdef DEBUG_FLAG
#define MosquittoPub_log(format, ...) MosquittoPub_dbg(format, ##__VA_ARGS__)
#else
#define MosquittoPub_log(format, ...)
#endif

typedef enum SSOCKET_STAT_E
{
    UNCONNECT    = 0,
    WAITCONNECT  = 1,
    MOSQUTTCONNECTED = 2,
}MosquittoPubSTAT;
	
typedef struct
{
	std::string            JSub_Id;
	std::string            JDev_ID;
	std::string			   JRealmName;
	std::string			   JMPort;
	std::string			   JClientId;
	std::string			   JTopic;
	std::string			   JPayload;
	std::string			   JQos;
	std::string			   JTimeOut;
}MosqutoCFG;


#define SSOCKET_BUFF_LEN 5*500*1024

typedef int (*MosquittoPubCallback)(byte* msg, uint len);

class MosquittoPub
{
public:
	static MosquittoPub* GetInstance();
	virtual ~MosquittoPub();
			MosquittoPub();
	int regRecvCallback(MosquittoPubCallback callback);

	int MQttConnectInit();
	int MqttInit();
    int MqttDeInit();
	
	int get_conn_net_status();

	int set_ipadress();
	int mosq_config_init(void);
	int pub_client_connect();
	
	int publish_mqtt_msg();
	int publish_mqtt_msg( char* msg,uint Len);
	int publish_MIS_msg( char* msg,uint Len);
	int publish_ALARM_msg( char* msg,uint Len);
	
	int subRcv_mqtt_msg(void);
	void pub_client_disconnect(void);
	int MosquittoPubDataProc();
	void MosquittoPaload2Json(MqttMsg_T* pMqttMsg);
	void AlarmMosquittoPaload2Json(MqttMsg_T* pMqttMsg);
	int MqttProc_send(MqttMsg_T* pMqttMsg);

	
	static void* MqttProc(void* data);
	int DataCompression(std::string msg);

	int get_time(char * time);
	int get_time_S(char * time);
	int DoubleQuotationMark2Asterisk();
	int Asterisk2DoubleQuotationMark();
	int socket_resolver(const char *domain, char* ipaddr);

public:
	 int             m_nState;
	 int 			 m_nUnconnect;
 	 char*			 p_sendBuff;
	 int			 m_nRecvLen;
	 int			 m_nSendLen;


private:
	 bool            m_bStart;
	 pthread_t       m_Thread;

	 char*            adressGet;
	 char* 			 Adress;
	 char*			 ClientId;
	 char*           Sub_Id;
	 char*			 Topic;
	 char*			 Payload;
	 char			 m_ClientId[30];
	 char			 m_SubId[30];
	 char			 m_DevID[20];
	 char 			 m_RealmName[25];
	 char            m_Topic[100];
	 char            m_MisTopic[100];
	 char            m_AlarmTopic[100];
	 char            m_SensorDataType[5];
	 char			 m_username[20];
	 char 			 m_password[40];
	 int 			 Port;
	 int 			 Qos;
	 int             TimeOut;
	 int			 SampTime;  //ms
	 MQTTClient      client;
	 static MosquittoPub* m_MosquittoPub;
public:
	 MosquittoPubCallback MosquittoPubRecv;
	 char*           Dev_ID;
};

int MosquittoPub_init();



#endif

