#ifndef KY_COMM_H
#define KY_COMM_H

#include "def.h"
#include "queue.h"
#include "list.h"

#define TSMsgLeng 10
#define TSMsgDat 50*1024  //max data deep is 50k

typedef struct SMsgHead_S
{
    socket_t socket;
    uint ipaddress;
    uint port;
}SMsgHead_T;

typedef enum MQTT_STAT_E
{
	NORMAL   =  0,
	ALARM 	 =  1,
}mqtt_stat_t;
typedef struct SensorStatus
{
	bool TS1_S_STA;
	bool TS2_S_STA;
	bool S1_STA;
	bool S2_STA;
	bool TS1_T_STA;
	bool TS2_T_STA;
	bool ALL_STA;
}SensorStatus_T;
typedef struct MqttMsg_S
{
	queue_handle qh;
	mqtt_stat_t stat;
	SensorStatus_T STA;
	int TS1Msg_tLen;//温振一体机温度数据1长度
	int TS2Msg_tLen;//温振一体机温度数据2长度
	int TS1Msg_sLen;//温振一体机振动数据1长度
	int TS2Msg_sLen;//温振一体机振动数据2长度
	int S1_Len;//震动数据1长度
	int S2_Len;//震动数据2长度
	int TS1Msg_t[TSMsgLeng];//温振一体机温度数据1
	int TS2Msg_t[TSMsgLeng];//温振一体机温度数据2
	int TS1Msg_s[TSMsgDat];//温振一体机振动数据1
	int TS2Msg_s[TSMsgDat];//温振一体机振动数据2
	int S1[TSMsgDat];//震动数据1
	int S2[TSMsgDat];//震动数据2
}MqttMsg_T;

typedef struct AutOutMsg_S
{
	queue_handle qh;
	uint nLen;
	int  opCode;
    char pMsg[0];
}AutOutMsg_T;

typedef struct AppOutMsg_S
{
    queue_handle qh;
    uint nLen;
    char pMsg[0];
}AppOutMsg_T;

typedef struct QueueMsg_S
{
    queue_handle qh;
    SMsgHead_T  head;
    int  opCode;
    uint nLen;
    char pMsg[0];
}QueueMsg_T;

typedef struct TunnelMsg_S
{
    uint nTag;
    uint nLen;
    byte pMsg[0];
}TnlMsg_T;

typedef TnlMsg_T NetCapMsg_T;

typedef struct ProcStat_S
{
    int  opCode;
    int  action;
    uint ErrNo;
    std::string sequence;
}ProcStat_T;


#endif
