#include "mqtt.h"
//(0-50)*100 g
int randSData()
{
	int data;
	data = (rand() % 50)*100;
	return data;
}
//(-40-125)*100摄氏度
int randTData()
{
	int data;
	data = ((rand() % 166 - 40))*100;
	return data;
}

int data_generate(MqttMsg_T *pMqttMsg)
{
	int count =0;
	do{
			pMqttMsg->TS1Msg_s[count] = randSData();
			count++;
		}while(count<pMqttMsg->TS1Msg_sLen);
	count = 0;
	do{
			pMqttMsg->TS1Msg_t[count] = randTData();
			count++;
		}while(count<pMqttMsg->TS1Msg_tLen);
	count = 0;
	do{
			pMqttMsg->S1[count] = randSData();
			count++;
		}while(count<pMqttMsg->S1_Len);
	count = 0;
	do{
			pMqttMsg->S2[count] = randSData();
			count++;
		}while(count<pMqttMsg->S2_Len);

		count = 0;
	do{
			pMqttMsg->TS2Msg_s[count] = randSData();
			count++;
		}while(count<pMqttMsg->TS2Msg_sLen);
		count = 0;
	do{
			pMqttMsg->TS2Msg_t[count] = randTData();
			count++;
		}while(count<pMqttMsg->TS2Msg_tLen);
	return 0;
}
int main(int argc, char *argv[])
{
	int ret = 0;
	char ip[16] = {0};
	int count =0;
	char time[13];
	int datanum;
	int nrow=0;
	int i =0;
	int ncolumn = 0;
	char **azResult;
	char data_leng[10]={0};
	MosquittoPub test_mosquitto;
	printf("pub client test !\n");
	test_mosquitto.MqttInit();
	test_mosquitto.socket_resolver("www.baidu.com", ip);
	printf("dddd: %s\n", ip);

	test_mosquitto.socket_resolver("172.18.18.168", ip);
	printf("dddd: %s\n", ip);

	MqttMsg_T* pMqttMsg = (MqttMsg_T*)malloc(sizeof(MqttMsg_T));

	pMqttMsg->stat = NORMAL;
	pMqttMsg->S1_Len = 100;
	pMqttMsg->S2_Len = 100;
	pMqttMsg->TS1Msg_sLen = 100;
	pMqttMsg->TS1Msg_tLen = 100;
	pMqttMsg->TS2Msg_sLen = 100;
	pMqttMsg->TS2Msg_tLen = 100;
	memset(pMqttMsg->TS1Msg_s,0,sizeof(pMqttMsg->TS1Msg_s));
	memset(pMqttMsg->TS2Msg_s,0,sizeof(pMqttMsg->TS2Msg_s));
	test_mosquitto.get_time(time);
	printf("time is = %s\n",time);

	//建立连接时，查询数据库中有多少条失连时存储的数据，然后进行数据从发处理，发送完成后要清空已经发送完成的数据
/*	DataBase * pAppDataBase;
	pAppDataBase = DataBase::GetInstance();
	pAppDataBase->Init();
	datanum = pAppDataBase->db_get_maxID(TabName, &datanum);
	if(datanum>1){
	ret = pAppDataBase->db_Query_ALLTable(TabName, &azResult, &ncolumn, &nrow);
	printf("nrow=%d ncolumn=%d\n",nrow,ncolumn);
	printf("the result is:\n");
	i=10;
	do{
		strcpy(data_leng,azResult[i+4]);
		test_mosquitto.m_nSendLen = atoi(data_leng);
		printf("test_mosquitto->m_nSendLen is = %d",test_mosquitto.m_nSendLen);
		memcpy(test_mosquitto.p_sendBuff,azResult[i+2],test_mosquitto.m_nSendLen);
								
		int j =0;
		//耗时操作，转换jasoN格式数据
			do{
				if(test_mosquitto.p_sendBuff[j]=='*')
				test_mosquitto.p_sendBuff[j] = '"';
				j++;
			}while(j<test_mosquitto.m_nSendLen);
			printf("test_mosquitto->p_sendBuff = %s",test_mosquitto.p_sendBuff);
		//发送jason报文数据
/*			ret = test_mosquitto->publish_mqtt_msg(pMqttMsg);
			if(ret == OK){
				 MosquittoPub_log("send id = %d mesg success !",azResult[i]);
			}*/
/*			i = i+5;
			}while(i<(nrow+1)*ncolumn);
			sqlite3_free_table(azResult);
			//发送完成所有的数据后,清空表中的所有数据
			pAppDataBase->db_delete_all_data_InTab(TabName);
			test_mosquitto.get_time(time);
			pAppDataBase->db_Insert_Table(TabName,time,"00",1);//插入一条数据，做记录使用
		}*/
	while(1)
		{
			sleep(10);
			data_generate(pMqttMsg);
			test_mosquitto.MqttProc_send(pMqttMsg);
		}
	free(pMqttMsg);
	return 0;
}

