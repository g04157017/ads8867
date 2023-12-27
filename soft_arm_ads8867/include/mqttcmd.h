#ifndef mqttcmd_h_
#define mqttcmd_h_

/* APP opCode and Action defined here */

/* Users opCode */

#define APP_OPCODE_DATA_NOMOUT    					"001"
#define APP_OPCODE_DATA_ABNOMOUT  					"002"

/*   Heartbeat  opCode  */
#define MisTopic_OPCODE_HeartbeatTiming  			"001"
#define MisTopic_OPCODE_FristHeartbeatTiming  		"002"


/* System Authorization opCode */
#define APP_OPCODE_SYSTEM_AUTHORIZATION_BEGAIN    	001
#define DevidTopic_AUTHORIZATION_ISSUED    			005

#define APP_OPCODE_SYSTEM_AUTHORIZATION_REPORT   	"004"
#define APP_OPCODE_SYSTEM_AUTHORIZATION_EXPIRE   	"006"
#define APP_OPCODE_SYSTEM_AUTHORIZATION_ERR		 	"009"

#define APP_OPCODE_SYSTEM_AUTHORIZATION_END       	010


/*	define cfg here */
#define DevidTopic_cfg_begain					  	11

#define DevidTopic_OPCODE_ThresholdAlarm			14

#define MisTopic_OPCODE_Answer						"005"

#define AlarmTopic_OPCODE_AlarmMessage				"001"


#define DevidTopic_cfg_End						  	20

#define OTA_Load_CFG_Begain							21

#define OTAQueryConfiguration						22
#define OTAUpdateConfiguration						23


#define OTA_Load_CFG_End							24



/* Config opCode */
#define APP_OPCODE_CFG_BEGIN     19
#define APP_OPCODE_CFG_QUERIF    20
#define APP_OPCODE_CFG_REMOTE    21
#define APP_OPCODE_CFG_NETCFG    22
#define APP_OPCODE_CFG_ROUTE     23
#define APP_OPCODE_CFG_BRIDGE    24
#define APP_OPCODE_CFG_TUNNEL    25
#define APP_OPCODE_CFG_POLICY    26
#define APP_OPCODE_CFG_NATCFG    27
#define APP_OPCODE_CFG_ARPCFG    28
#define APP_OPCODE_CFG_MACCFG    29

/* System Monitor opCode */
#define APP_OPCODE_SYSM_BEGIN    30
#define APP_OPCODE_SYSM_LOGS     31
#define APP_OPCODE_SYSM_ALARMS   32
#define APP_OPCODE_SYSM_OPERS    33
#define APP_OPCODE_SYSM_TUNNEL   34

/* System Control opCode */
#define APP_OPCODE_SYSC_BEGIN    40
#define APP_OPCODE_SYSC_BAKUPOP  41
#define APP_OPCODE_SYSC_FILEOP   42
#define APP_OPCODE_SYSC_RSTOP    43
#define APP_OPCODE_SYSC_SAVECFG  44

/* System Security opCode */
#define APP_OPCODE_SYSS_BEGIN    50
#define APP_OPCODE_SYSS_CAUPL    51
#define APP_OPCODE_SYSS_DISSU    52
#define APP_OPCODE_SYSS_PWOFF    53
#define APP_OPCODE_SYSS_BOOOM    54

/* Usart And Ec20T control opCode */
#define APP_OPCODE_USART_BEGAIN  59
#define APP_OPCODE_SERIAL        60


#define APP_OPCODE_EC200T_BEGAIN 65
#define APP_OPCODE_EC200T_CFG	 66

#define APP_OPCODE_END           70


/* APP Action code */
#define APP_ACTION_CREATE        1
#define APP_ACTION_DELETE        2
#define APP_ACTION_MODIFY        3
#define APP_ACTION_QUERY         4
#define APP_ACTION_QUERY_ALL     0

#define OPC                     "opcode"
#define Dev_P_ID                "DevID"
#define SEQ                     "sequence"
#define TMS						"TimeStamp"
#define RES                     "result"
#define BOXSTA					"boxStatus"
#define STADAT					"startDate"
#define AUTHCODE				"authCode"
#define RTS						"rts"
#define REC                     "record"
#define SK1					    "tempvibonevib"  //温振一体机震动数据1
#define SK2					    "tempvibtwovib"  //温振一体机震动数据2
#define SK3					    "vib1"  //振动数据3
#define SK4					    "vib2"  //振动数据4
#define TM1					    "tempvibonetemp"  //温振一体机温度1
#define TM2					    "tempvibtwotemp"  //温振一体机温度2
#define DT					    "data"
#define SSDT					"SensorDataType"

#define VIB1TIM					"Vib1Tim"
#define VIB2TIM					"Vib2Tim"
#define VIB3TIM					"Vib3Tim"
#define VIB4TIM					"Vib4Tim"





#define NM                      "tunelname"
#define TP						"type"
#define VIB						"Vib"
#define TMP						"Tmp"
#define UNIT					"unit"




#define RECNUM                  "recordnum"
#define PCT                     "percent"
#define MSG                     "message"
#define ERRNO                   "errno"


/* ErrNO temply define here */
#define ERR_NO_OPC_ERR                  0x00000001
#define ERR_NO_ACT_ERR                  0x00000002
#define ERR_NO_SEQ_ERR                  0x00000003
#define ERR_NO_MEM_ERR                  0x0000000F

#define ERR_USER_ERR                    0x00010000
#define ERR_USER_NAME_ERR               0x00010001
#define ERR_USER_PSWD_ERR               0x00010002
#define ERR_USER_LONGIN_TYPE_ERR        0x00010003
#define ERR_USER_PSWD_MISS_MATCH        0x00010004
#define ERR_USER_OP_NO_IP_ERR           0x00010005
#define ERR_USER_PSWD_SHORT_ERR         0x00010006
#define ERR_USER_PSWD_SLONG_ERR         0x00010007

#define ERR_CFG_SYS_SGW_ERR             0x00020000
#define ERR_CFG_SYS_SGW_NAME_ERR        0x00020001
#define ERR_CFG_SYS_SGW_IP_ERR          0x00020002
#define ERR_CFG_SYS_MNG_IP_ERR          0x00020003
#define ERR_CFG_SYS_KEY_ERR             0x00020004
#define ERR_CFG_SYS_CERT_ERR            0x00020005
#define ERR_CFG_SYS_MNG_TYPE_ERR        0x00020006

#define ERR_CFG_NET_ERR                 0x0002000A
#define ERR_CFG_NET_ETHNAME_ERR         0x0002000B
#define ERR_CFG_NET_DEST_IP_ERR         0x0002000C
#define ERR_CFG_NET_MASK_ERR            0x0002000D
#define ERR_CFG_NET_IP_TYPE_ERR         0x0002000E
#define ERR_CFG_NET_GATE_ERR            0x0002000F
#define ERR_CFG_NET_VlAN_ERR            0x00020010
#define ERR_CFG_NET_RECORD_NUM_ERR      0x00020011
#define ERR_CFG_NET_RECORD_ERR          0x00020012
#define ERR_CFG_NET_ROUTE_NAME_ERR      0x00020013
#define ERR_CFG_NET_ROUTE_NAME_LEN      0x00020014
#define ERR_CFG_NET_IP_PROTECTED_ERR    0x00020015

#define ERR_CFG_PLC_ERR                 0x00020020
#define ERR_CFG_PLC_NAME_ERR            0x00020021
#define ERR_CFG_PLC_IN_ADDR_ERR         0x00020022
#define ERR_CFG_PLC_OUT_ADDR_ERR        0x00020023
#define ERR_CFG_PLC_IN_PORT_ERR         0x00020024
#define ERR_CFG_PLC_OUT_PORT_ERR        0x00020025
#define ERR_CFG_PLC_PROTO_ERR           0x00020026
#define ERR_CFG_PLC_DIRECTION_ERR       0x00020027
#define ERR_CFG_PLC_MODE_ERR            0x00020028
#define ERR_CFG_PLC_INUSE_ERR           0x00020029

#define ERR_CFG_TNL_ERR                 0x00020030
#define ERR_CFG_TNL_NAME_ERR            0x00020031
#define ERR_CFG_TNL_PLC_IN_ERR          0x00020032
#define ERR_CFG_TNL_PLC_OUT_ERR         0x00020033
#define ERR_CFG_TNL_LOCAL_ADD_ERR       0x00020034
#define ERR_CFG_TNL_PEER_MADD_ERR       0x00020035
#define ERR_CFG_TNL_PEER_MCER_ERR       0x00020036
#define ERR_CFG_TNL_PEER_BADD_ERR       0x00020037
#define ERR_CFG_TNL_PEER_BCER_ERR       0x00020038
#define ERR_CFG_TNL_MODE_ERR            0x00020039
#define ERR_CFG_TNL_ROLE_ERR            0x0002003a
#define ERR_CFG_TNL_PLC_ROUTE_ERR       0x0002003b

#define ERR_CFG_SEC_ERR                 0x00020040
#define ERR_CFG_SEC_PEM_TYPE_ERR        0x00020041
#define ERR_CFG_SEC_PEM_STAT_ERR        0x00020042
#define ERR_CFG_SEC_KEY_FILE_ERR        0x00020043
#define ERR_CFG_SEC_KEY_LEN_ERR         0x00020044
#define ERR_CFG_SEC_KEY_SHA_ERR         0x00020045
#define ERR_CFG_SEC_CERT_FILE_ERR       0x00020046
#define ERR_CFG_SEC_CERT_LEN_ERR        0x00020047
#define ERR_CFG_SEC_CERT_SHA_ERR        0x00020048
#define ERR_CFG_SEC_KEY_ERR             0x00020049
#define ERR_CFG_SEC_CERT_ERR            0x0002004A

#define ERR_CFG_MAC_ERR                 0x00020050
#define ERR_CFG_MAC_ADDR_ERR            0x00020051
#define ERR_CFG_ARP_ERR                 0x00020052
#define ERR_CFG_ARP_ADDR_ERR            0x00020053

#define ERR_CFG_NAT_ERR                 0x00020060
#define ERR_CFG_NAT_ADDR_ERR            0x00020061
#define ERR_CFG_NAT_MODE_ERR            0x00020062
#define ERR_CFG_NAT_PROTO_ERR           0x00020063
#define ERR_CFG_NAT_PORT_ERR            0x00020064
#define ERR_CFG_NAT_NAME_ERR            0x00020065

#define ERR_SYSC_SYSC_ERR               0x00020070
#define ERR_SYSC_BACKUP_NAME_ERR        0x00020071
#define ERR_SYSC_FILEOP_NAME_ERR        0x00020072
#define ERR_SYSC_RESET_TYPE_ERR         0x00020073
#define ERR_SYSC_FILEOP_TYPE_ERR        0x00020074


/* Reset Code and Reason temply define here */
#define RST_CODE_INITIA                 0
#define RST_CODE_SYSTEM_NORMAL          1
#define RST_CODE_SGW_NORMAL             2
#define RST_CODE_SGW_UPDATE_CFG         3
#define RST_CODE_CFG_RESTORE            4
#define RST_CODE_CFG_DEFAULT            5

#define RST_REASON_INITIA               "system initia."
#define RST_REASON_SYSTEM_NORMAL        "system normal reboot."
#define RST_REASON_SGW_NORMAL           "sgw normal reboot."
#define RST_REASON_SGW_UPDATE_CFG       "sgw update config."
#define RST_REASON_CFG_RESTORE          "system config restore."
#define RST_REASON_CFG_DEFAULT          "system config default."

#define SYSC_CMD_RESTART_NETWORK_M      "service network-manager restart"
#define SYSC_CMD_RESTART_NETWORK_S      "service networking restart"
#define SYSC_CMD_RESTART_NETWORK_D      "/etc/init.d/networking restart"
#define SYSC_CMD_RESTART_NETWORK_UP     "ifup -a"
#define SYSC_CMD_RESTART_NETWORK_DW     "ifdown -a"
#define SYSC_CMD_RESET                  "reset"
#define SYSC_CMD_SYSCMD                 "syscmd"

#endif
