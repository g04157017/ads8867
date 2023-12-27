#ifndef CONFIG_H
#define CONFIG_H

/* 全局配置 */
#define CFG_SEC_GLOBAL              "global:"
#define CFG_SEC_GLOBAL_NAME         "global"


/* 系统控制配置 */
#define CFG_SEC_SYSC                "sysc:"
#define CFG_SEC_SYSC_NAME           "sysc"

#define CFG_SEC_BACKUP              "backup:"
#define CFG_SEC_BACKUP_NAME         "backup"

#define CFG_SEC_RESET               "reset:"
#define CFG_SEC_RESET_NAME          "reset"
#define CFG_SYSC_RESET_CODE         "resetcode"         /* 重启代码 */    
#define CFG_SYSC_RESET_REASON       "resetreason"       /* 重启原因     */ 
#define CFG_SYSC_RESET_LASTBAK      "lastbakup"         /* 最新备份     */ 
#define CFG_SYSC_RESET_BAKTIME      "bakuptime"         /* 备份时间     */
#define CFG_SYSC_RESET_TIMES        "resettimes"        /* 重启次数     */

#define CFG_SYSC_CFG_FILE           "config_file"        /* 当前配置文件名,config.ini */
#define CFG_SYSC_DEF_FILE           "default_file"       /* 默认配置文件名,default.ini */

#define CFG_SYSC_CONFIG_FILE        "config.ini"         /* 当前配置文件名,config.ini */
#define CFG_SYSC_CFG_FILE_TMP       "config.ini.bak"     /* 配置文件备份文件名 */
#define CFG_SYSC_DEFAULT_FILE       "default.ini"        /* 默认配置文件名,default.ini */
#define CFG_SYSC_BACKUP_FILE        "bakcup.ini"         /* 默认配置恢复文件名,bakcup.ini */

/* 系统管理配置 */
#define CFG_SEC_SYSM                "sysm:"
#define CFG_SEC_SYSM_NAME           "sysm"
#define CFG_SYSM_LOG_LEVEL          "loglevel"           /* 日志级别 */

#define CFG_SYSM_LOG_LEVEL_D        "error"              /* 日志级别 */
#define CFG_SYSM_LOG_FILE           "syslog.log"         /* 日志文件名 */
#define CFG_SYSM_OPER_FILE          "sysoper.log"        /* 操作日志文件名 */

/* 接口配置 */
#define CFG_SEC_IF                  "interface:"
#define CFG_SEC_IF_NAME             "interface"

#define CFG_IF_NUM                  "eth_num"
#define CFG_IF_SUB_NUM              "eth_sub_num"
#define CFG_IF_ROUTE_NUM            "eth_route_num"

#define CFG_IF_ETH0                 "eth0"
#define CFG_IF_ETH0_0               "eth0:0"
#define CFG_IF_ETH0_1               "eth0:1"
#define CFG_IF_ETH1                 "eth1"
#define CFG_IF_ETH1_0               "eth1:0"
#define CFG_IF_ETH1_1               "eth1:1"

/* 串口配置 */
#define CFG_SERIAL					"serialcfg:"
#define CFG_SERIAL_NAME				"serial"
#define CFG_SEC_USART				"serialcfg"
/*mosquto 配置*/
#define CFG_MSQT					"MosqCFG:"
#define CFG_MSQT_NAME				"mosqcfg"

/* 系统配置 */
#define CFG_SEC_SYSTEM              "System:"
#define CFG_SEC_SYSTEM_NAME         "system"
#define CFG_DEVICE_ID				"Dev_ID"
/*    aut        */
#define CFG_AUT						"Aut:"
#define CFG_AUT_NAME				"aut"
#define AUTHSTARTDATA				"AuthStartData"

/*	samp	*/
#define CFG_SAMP					"Samp:"
#define CFG_SAMP_NAME				"samp"
#define	SAMP_TIME					"SampTime"
#define SAMP_FREQ					"SampFreq"

/*	alarm 	*/
#define CFG_ALARM					"Alarm:"
#define CFG_ALARM_NAME				"alarm"
#define	SPEED_EFFECTIVE_AC1_H			"SpeedEffectiveValueThreshold_AC1_H"
#define	SPEED_EFFECTIVE_AC2_H			"SpeedEffectiveValueThreshold_AC2_H"
#define	SPEED_EFFECTIVE_AC3_H			"SpeedEffectiveValueThreshold_AC3_H"
#define	SPEED_EFFECTIVE_AC4_H			"SpeedEffectiveValueThreshold_AC4_H"
#define PEAKTOPEAK_AC1_H				"PeakToPeakThreshold_AC1_H"
#define PEAKTOPEAK_AC2_H				"PeakToPeakThreshold_AC2_H"
#define PEAKTOPEAK_AC3_H				"PeakToPeakThreshold_AC3_H"
#define PEAKTOPEAK_AC4_H				"PeakToPeakThreshold_AC4_H"
#define TEMPERATURE_TP1_H				"TemperatureValue_TP1_H"
#define TEMPERATURE_TP2_H				"TemperatureValue_TP2_H"

#define	SPEED_EFFECTIVE_AC1_L			"SpeedEffectiveValueThreshold_AC1_L"
#define	SPEED_EFFECTIVE_AC2_L			"SpeedEffectiveValueThreshold_AC2_L"
#define	SPEED_EFFECTIVE_AC3_L			"SpeedEffectiveValueThreshold_AC3_L"
#define	SPEED_EFFECTIVE_AC4_L			"SpeedEffectiveValueThreshold_AC4_L"
#define PEAKTOPEAK_AC1_L				"PeakToPeakThreshold_AC1_L"
#define PEAKTOPEAK_AC2_L				"PeakToPeakThreshold_AC2_L"
#define PEAKTOPEAK_AC3_L				"PeakToPeakThreshold_AC3_L"
#define PEAKTOPEAK_AC4_L				"PeakToPeakThreshold_AC4_L"
#define TEMPERATURE_TP1_L				"TemperatureValue_TP1_L"
#define TEMPERATURE_TP2_L				"TemperatureValue_TP2_L"

/*  SensorDataType   */
#define	CFG_SENSORDATATYPE				"SensorDataType:"
#define	CFG_SENSORDATATYPE_NAME			"sensordatatype"



/* 网络配置 */
#define CFG_SEC_ETHERNET            "EtherNet:"
#define CFG_SEC_ETHERNET_NAME       "bridge0"
#define CFG_ETH_NAME                "eth0:1"

#define CFG_ETH_IF_NAME             "ifname"
#define CFG_ETH_IF_TYPE             "iftype"
#define CFG_ETH_IF_IPADDR           "ipaddress"
#define CFG_ETH_IF_NETMASK          "netmask"
#define CFG_ETH_IF_GATE             "gate"
#define CFG_ETH_IF_VLANID           "vlanid"
#define CFG_ETH_IF_DESC             "desc"

#define CFG_ETH_IF_PUBLIC           "PUBLIC"
#define CFG_ETH_IF_PRIVATE          "PRIVATE"
#define CFG_ETH_SGW_IN              "eth0"           /* eth0作为对内隧道接口地址所在网卡                     */
#define CFG_ETH_SGW_BIP             "eth0:0"         /* 配置在eth0:0上的加密网关服务器内网地址                  */  
#define CFG_ETH_SGW_OUT             "eth1"           /* eth1作为对外接口地址所在网卡                     */
#define CFG_ETH_SGW_MIP             "eth1:0"         /* 配置在eth1:0上的加密网关服务器外网地址                  */
                                                     /* eth*:1-eth*:11 用于建立隧道                */ 
#define CFG_ETH_SGW_IIP_DEF         "192.168.100.1"
#define CFG_ETH_SGW_BIP_DEF         "192.168.100.2"
#define CFG_ETH_SGW_OIP_DEF         "192.168.101.1"
#define CFG_ETH_SGW_MIP_DEF         "192.168.101.2"

/* 日志配置 */
#define CFG_SEC_LOG                 "log:"
#define CFG_SEC_LOG_NAME            "log"
#define CFG_LOG_LEVEL               "loglevel"
#define CFG_LOG_SIZE                "logsize"
#define CFG_LOG_DAYS                "logdays"
#define CFG_LOG_LEVEL_DEFAULT       "error"

/* 配置默认值 */
#define CFG_SYS_WAIT_SECOND         1
#define CFG_SYS_WAIT_MSECOND        500000
#define CFG_SYS_WAIT_USECOND         10000
#define CFG_LOG_SIZE_DEF            10240000
#define CFG_LOG_DAYS_DEF            90
#define CFG_KEY_MAX_LEN             512


#endif

