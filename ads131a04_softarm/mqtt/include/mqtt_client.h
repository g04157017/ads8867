#ifndef mqtt_client_h
#define mqtt_client_h

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

#ifdef DEBUG_FLAG
#define MosquittoClient_log(format, ...) MosquittoPub_dbg(format, ##__VA_ARGS__)
#else
#define MosquittoClient_log(format, ...)
#endif

typedef enum SSOCKET_STAT_E
{
    MISUNCONNECT_ALARMUNCONNECT  = 0,
    WAITCONNECT  = 1,
    MOSQUTTCONNECTED = 2,
    MISCONNECT_ALARMUNCONNECT =3,
    MISUNCONNECT_ALARMCONNECT =4,
}MosquittoPubSTAT;

#define SSOCKET_Client_BUFF_LEN 1*1024

class MosquittoClient
{
public:
	static MosquittoClient* GetInstance();
	virtual ~MosquittoClient();
			MosquittoClient();
			
	int MqttClientInit();
    int MqttClientDeInit();


	int get_conn_net_status();

	int set_ipadress();
	int mosq_config_init(void);
	int client_MIS_connect();
	int client_ALARM_connect();
	void client_disconnect(void);
	
	int Send_mqtt_msg( char* msg,uint Len);
	int MosquittoClientStat();

	
	static void* MqttClientProc(void* data);

	int socket_resolver(const char *domain, char* ipaddr);

public:
	 int             m_nState;
private:
	 bool            m_bStart;
	 pthread_t       m_Thread;
	 
	 char*            p_sendBuff;
	 char*            adressGet;
	 
	 char			 m_MisClientId[30];
	 char			 m_AlarmClientId[30];
	 
	 char            m_MisTopic[20];
	 char            m_AlarmTopic[20];

	 MQTTClient      m_Misclient;
	 MQTTClient      m_Alarmclient;
	 
	 char			 m_DevID[20];
	 char 			 m_RealmName[25];

	 int 			 Port;
	 int 			 Qos;
	 int             TimeOut;

	 static MosquittoClient* m_MosquittoClient;
public:
	
};

int Mosquittoclient_init();



#endif

