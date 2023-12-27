#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include<sys/resource.h>
#include "ql_oe.h"


/***********************************************************
* MACROS
*/


#define DEBUG_LEVEL_FILE     "/home/root/debug/debug_system_monitor"
#define LOG_LEVEL_FILE       "/home/root/debug/log_system_monitor"
#define LOG_FILE             "/home/root/log/system_monitor.log"
#define LOG_MAX_SIZE         1000000

#define QL_MAINUART_BAUDRATE  				115200
#define QL_MAINUART_DEV                   "/dev/ttyHS0"

#define FILE_EC20_BOOT_CNT          "/home/root/stats/ec20_boot_cnt" //EC20的启动次数


/* device file name */
#define WATCHDOG           "/dev/watchdog"

#define TIMEOUT            1


/*
**Protocol V1.0
**preamble(4 bytes) + transmit_type(1 byte) + protocol version(2 bytes) + data length(2 bytes) + data(data1,data2,...) + crc16(2 bytes) + [endcode]
**
**(1)dataN: <data_type(1 byte)> + <data_len(2 bytes)> + <data_value>
**(2)endcode: not necessary. Downstream packets need it.
*/

#define PACKET_HEAD_PREAMBLE_LEN       0x04
#define PREAMBLE_FIRST_BYTE			   0x7B
#define PREAMBLE_SECOND_BYTE		   0x07
#define PREAMBLE_THIRD_BYTE		       0xAF
#define PREAMBLE_FOURTH_BYTE		   0xEC

/* protocol version */
#define  VERSION_LEN                   0x02

/* transmit type */
#define  TRANSMIT_TYPE_LEN             0x01

/* total data length */
#define TOTAL_DATA_LEN_LEN             0x02

/* single data */
#define SINGLE_DATA_TYPE_LEN           0x01
#define SINGLE_DATA_LEN_LEN            0x01

/* net parameters length */
#define  DATA_PACKET_ID_LEN            0x03
#define  UART_DEVICE_ID_LEN            0x09
#define  NODE_HOPS_LEN                 0x01
#define  PARENT_NODE_ID_LEN            0x02
#define  SINK_NODE_ID_LEN              0x08

/* temperature and humidity features length */
#define  HDC_TEMP_LEN                  0x02
#define  HDC_HUMIDITY_LEN              0x02
#define  NTC_TEMP_LEN                  0x02

/* acc features length */
#define  ACC_SAMPLING_RATE_LEN         0x04
#define  ACC_SKEW_LEN                  0x04
#define  ACC_KURTOSIS_LEN              0x04
#define  ACC_CREST_FACTOR_LEN          0x04
#define  ACC_SHAPE_FACTOR_LEN          0x04
#define  ACC_VARIANCE_LEN              0x04
#define  ACC_CLEARANCE_FACTOR_LEN      0x04
#define  ACC_ABS_MEAN_LEN              0x04
#define  ACC_SQUARE_ROOT_AMPLITUDE_LEN 0x04
#define  ACC_IMPULSE_FACTOR_LEN        0x04

/* acc origin packet id(not subpacket!!!) */
#define  SENDER_ORIG_DATA_PACKET_ID_LEN              0x01
#define  SENDER_ORIG_DATA_SUBPACKET_NUM_LEN          0x01
#define  SENDER_ORIG_DATA_SUBPACKET_NO_LEN           0x01


/* acc origin number */
#define  ACC_ORIGIN_VALUE_NUM          0x40

/* acc origin length */
#define  ACC_ORIGIN_VALUE_LEN          0x80

/* other sensor value length */
#define  NOISE_VALUE_LEN               0x02
#define  ROTATION_SPEED_LEN            0x02

#define  REBOOT_EC20_LEN               0x02
#define  CC2650_BOOT_LEN               0x02

#define  CC2650_HEARTBEAT_LEN          0x00


/* downstream data length(single data type) */
#define  SINK_COLLECT_SENSOR_LEN       0x00
#define  SINK_EC20_HEARTBEAT_LEN       0x00
#define  SET_DEVICE_ID_LEN             0x09
#define  SET_PAN_ID_LEN                0x02
#define  SET_DEBUG_LEVEL_LEN           0x01
#define  READ_DEVIE_ID_LEN             0x00
#define  READ_PAN_ID_LEN               0x00
#define  READ_DEBUG_LEVEL_LEN          0x00

/* crc */
#define CRC_LEN                        0x02 //crc16

/* endcode */
#define PACKET_ENDCODE_LEN             0x04
#define PACKET_ENDCODE_FIRST_BYTE	   0x85
#define PACKET_ENDCODE_SECOND_BYTE	   0xf9
#define PACKET_ENDCODE_THIRD_BYTE	   0x51
#define PACKET_ENDCODE_FOURTH_BYTE	   0x14

#define SENDER_ORIG_DATA_FIRST_SUBPACKET_ID 0x01

#define UART_PACKET_LEN_WITHOUT_DATA    PACKET_HEAD_PREAMBLE_LEN + VERSION_LEN + TRANSMIT_TYPE_LEN + TOTAL_DATA_LEN_LEN + CRC_LEN + PACKET_ENDCODE_LEN

/* 串口select超时时间，单位秒 */
#define SERIAL_SELECT_TIME_OUT 1	

#define MAIN_UART_RECV_DATA_LEN		10000

#define MAX_COMM_MSG_NUM 10


#define TEST


/****************************** DATA TYPE ******************************/
enum TRANSMIT_TYPE {
	SENDER_UPSTREAM_FEATURE           = 0x01,
	SENDER_UPSTREAM_ORIGIN            = 0x02,
	SINK_UPSTREAM_TEMPHUM             = 0x03,
	SENDER_DOWNSTREAM_CONFIG          = 0x04,
	SINK_DOWNSTREAM_COLLECT_SENSOR    = 0x05,
	SENDER_DOWNSTREAM_KEEPALIVE       = 0x06,
	DOWNSTREAM_FLASH_INFO             = 0x07,  
	DOWNSTREAM_READ_INFO              = 0x08,  
	SINK_UPSTREAM_HEARTBEAT           = 0x09,
    DOWNSTREAM_LED_CTRL               = 0x0A
};

/* upstream data type */
enum  DATA_UPSTREAM_TYPE {
	/* net parameters */
	DATA_PACKET_ID                  =   0x00,
	DEVICE_ID                       =   0x01,
	NODE_HOPS                       =   0x02,
	PARENT_NODE_ID                  =   0x03,
	SINK_NODE_ID                    =   0x04,

	/* sender temperature and humidity features */
	SENDER_HDC_TEMP                 =   0x10,
	SENDER_HDC_HUMIDITY             =   0x11,
	SENDER_NTC_TEMP                 =   0x12,

	/* sink temperature and humidity features */
	SINK_HDC_TEMP                   =   0x20,
	SINK_HDC_HUMIDITY               =   0x21,
	SINK_NTC_TEMP                   =   0x22,

	/* sender acc features */
	ACC_SAMPLING_RATE               =   0x30,
	ACC_SKEW                        =   0x31,
	ACC_KURTOSIS                    =   0x32,
	ACC_CREST_FACTOR                =   0x33,
	ACC_SHAPE_FACTOR                =   0x34,
	ACC_VARIANCE                    =   0x35,
	ACC_CLEARANCE_FACTOR             =   0x36,
	ACC_ABS_MEAN                    =   0x37,
	ACC_SQUARE_ROOT_AMPLITUDE       =   0x38,
	ACC_IMPULSE_FACTOR              =   0x39,

	/* sender acc origin value */
	ACC_ORIGIN                      =   0x50,

	/* other sensor features */
	NOISE_VALUE                     =   0x60,
	ROTATION_SPEED                  =   0x61,

	/*REBOOT EC20 TIMES*/  
	REBOOT_EC20_TIMES               =   0x70,  
	CC2650_BOOT_TIMES               =   0x71,

	/* sink heartbeat */  
	CC2650_HEARTBEAT_TYPE           =   0x80  
};


/* downstream data type */
enum  DATA_DOWMSTREAM_TYPE {
	SENDER_DEVICE_ID                = 0x90,
	SENDER_UPSTREAM_INTERVAL        = 0x91,
	SINK_COLLECT_SENSOR             = 0x92,
	SINK_EC20_HEARTBEAT             = 0x93,
    RINGLIGHT_COLOR                 = 0x94,
    RINGLIGHT_DURATION              = 0x95,
    RINGLIGHT_PERIOD                = 0x96,
    SYS_LED_STATUS                  = 0x97,
	SET_DEVICE_ID                   = 0xA0,
	SET_PAN_ID                      = 0xA1,
	SET_DEBUG_LEVEL                 = 0xA2,
  	READ_DEVIE_ID                   = 0xA3,
  	READ_PAN_ID                     = 0xA4,
  	READ_DEBUG_LEVEL                = 0xA5
};

/* SINK_GET_FEATURE消息状态 */
// enum SINK_GET_FEATURE_MSG_STATUS {
// 	INIT_STATUS,
// 	ACC_DATA_READY, //acc data has been sampled
// 	SEND_SINK_GET_FEATURE_MSG, //has sent SINK_GET_FEATURE msg
// 	RETRY_GET_MOTION_DATA, //为了获取运动数据重新发送
// 	SEND_SINK_GET_FEATURE_MSG_REAL, //has sent SINK_GET_FEATURE msg to get realtime original data
// 	RECV_SINK_FEATURE_MSG, //has recved SINK_FEATURE msg
// };


typedef struct {
	unsigned char  preamble[PACKET_HEAD_PREAMBLE_LEN];
	unsigned char  transmit_type;
	unsigned char  software_version[VERSION_LEN];
	unsigned char  length[TOTAL_DATA_LEN_LEN];
}uart_packet_header_t;

typedef struct {
    unsigned char crc[2];
}uart_packet_tail_t;


struct net_params {
	unsigned char   data_packet_id_type;
	unsigned char   data_packet_id_len;
	unsigned char   data_packet_id[DATA_PACKET_ID_LEN];
	unsigned char   device_id_type;
	unsigned char   device_id_len;
	unsigned char   device_id[UART_DEVICE_ID_LEN];
	unsigned char   node_hops_type;
	unsigned char   node_hops_len;
	unsigned char   node_hops;
	unsigned char   parent_node_id_type;
	unsigned char   parent_node_id_len;
	unsigned char   parent_node_id[PARENT_NODE_ID_LEN];
	unsigned char   sink_node_id_type;
	unsigned char   sink_node_id_len;
	unsigned char   sink_node_id[SINK_NODE_ID_LEN];
};


struct sink_system_stats {  
	unsigned char   reboot_ec20_type;  
	unsigned char   reboot_ec20_len;  
	unsigned char   reboot_ec20_value[REBOOT_EC20_LEN];   
	unsigned char   cc2650_boot_type;  
	unsigned char   cc2650_boot_len;  
	unsigned char   cc2650_boot_value[CC2650_BOOT_LEN];
};


struct sink_temphum_value {
	unsigned char  hdc_temp_type;
	unsigned char  hdc_temp_len;
	unsigned char  hdc_temp[HDC_TEMP_LEN];
	unsigned char  hdc_humidity_type;
	unsigned char  hdc_humidity_len;
	unsigned char  hdc_humidity[HDC_HUMIDITY_LEN];
	unsigned char  ntc_temp_type;
	unsigned char  ntc_temp_len;
	unsigned char  ntc_temp[NTC_TEMP_LEN];
};


// struct sink_get_feature_status {
// 	int send_msg_time; //发送SINK_GET_FEATURE报文的时间
// 	int resend_msg_cnt; //由于超时而重发报文的次数
// 	enum SINK_GET_FEATURE_MSG_STATUS status; //消息状态
// };


struct sink_heartbeat_value {  
	unsigned char cc2650_heartbeat_type;  
	unsigned char cc2650_heartbeat_len;
};



struct sink_temphum_msg {
	/* data frame header */
	uart_packet_header_t  header;

	/* net params */
	struct net_params net_params;

	/* system statistics */  
	struct sink_system_stats system_stats;

	/* sensor data */
	struct sink_temphum_value temphum_value;

	/* data frame crc */
	unsigned char crchl[CRC_LEN];
};


struct sink_heartbeat_msg {   
	/* data frame header */  
	uart_packet_header_t  header;  
	
	/* sensor data */  
	struct sink_heartbeat_value heartbeat_value;  
	
	/* data frame crc */  
	unsigned char crchl[2];
};


struct downstream_msg {
	/* data frame header */
	uart_packet_header_t  header;

	/* data type */
	unsigned char data_type;

	/* data len */
	unsigned char data_len;

	/* data frame crc */
	unsigned char crchl[CRC_LEN];

	/* end code */
	unsigned char endcode[PACKET_ENDCODE_LEN];
};

/**
 * CC2650通信报文个数
 * @start: 开始位置
 * @len: 报文长度
 */ 
struct cc2650_comm_msg {
    int start;
    int len;
};



/*********************************************************************
* GLOBAL VARIABLES
*/

static int fd_watchdog = -1;
static int watchdog_opened = 0; //whether watchdog is opened
static int feed_intv = -1;


//app_log_t log_monitor;

static int fd_mainuart = -1;

const unsigned char PACKET_HEAD_PREAMBLE[PACKET_HEAD_PREAMBLE_LEN] = {PREAMBLE_FIRST_BYTE, PREAMBLE_SECOND_BYTE, PREAMBLE_THIRD_BYTE, PREAMBLE_FOURTH_BYTE};
const unsigned char VERSION[VERSION_LEN] = {0x01, 0x00};
const unsigned char PACKET_ENDCODE[PACKET_ENDCODE_LEN] = {PACKET_ENDCODE_FIRST_BYTE, PACKET_ENDCODE_SECOND_BYTE, PACKET_ENDCODE_THIRD_BYTE, PACKET_ENDCODE_FOURTH_BYTE};



/*********************************************************************
* FUNCTIONS
*/

const unsigned short crctab[16] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001,0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01,0x8801, 0x4400
};

unsigned short crc16(unsigned char *buf, int len) {
    unsigned short crc = 0xffff;
    unsigned int Temp, i;
    for (i = 0; i < (len << 1); i++) {
        Temp = (i & 0x1) ? (buf[i >> 1] >> 4) : (buf[i >> 1] & 0x0f);
        crc = (crc >> 4) ^ crctab[(crc & 0x0f) ^ Temp];
    }
   return crc;
}


static void safe_sleep(long sec,long usec)
{
	struct timeval to;

	to.tv_sec = sec;
	to.tv_usec = usec;

	select(0,NULL,NULL,NULL,&to);
}


/*********************************************************************
* @fn	   mainuart_open
*
* @brief   open main uart
*
* @param   void
*
* @return  0:success 1:false
*
*/
int mainuart_open()
{	
	int iRet;
	int baudRate = QL_MAINUART_BAUDRATE;

	if(!(baudRate == B_9600 || baudRate == B_19200  || baudRate == B_38400  || baudRate == B_57600 || 
		baudRate == B_115200  || baudRate == B_230400  || baudRate == B_460800 || baudRate == B_921600 )) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Usage: baud rate :9600/19200/38400/57600/115200/230400/460800/921600/\n");
        printf("Usage: baud rate :9600/19200/38400/57600/115200/230400/460800/921600/\n");
		return -1;
	}

	fd_mainuart = Ql_UART_Open(QL_MAINUART_DEV, baudRate, FC_NONE);
	//DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "< open(\"%s\", %d)=%d\n", QL_MAINUART_DEV, baudRate, fd_mainuart);
	
	printf("< open(\"%s\", %d)=%d\n", QL_MAINUART_DEV, baudRate, fd_mainuart);

	if(fd_mainuart < 0) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Ql_UART_Open fail!\n");
        printf("Ql_UART_Open fail!\n");
		return -1;
	}

	/* Start: If need, to modify uart dcb config */
	ST_UARTDCB dcb = {
		.flowctrl = FC_NONE,	//none flow control
		.databit = DB_CS8,	    //databit: 8
		.stopbit = SB_1,	    //stopbit: 1
		.parity = PB_NONE,	    //parity check: none
		.baudrate = baudRate	//baudrate: 115200
	};

	iRet = Ql_UART_SetDCB(fd_mainuart, &dcb);
	iRet = Ql_UART_GetDCB(fd_mainuart, &dcb);

    /*DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "GET DCB ret: %d: baudrate: %d, flowctrl: %d, databit: %d, stopbit: %d, paritybit: %d\n", 
				iRet, dcb.baudrate, dcb.flowctrl, dcb.databit, dcb.stopbit, dcb.parity);*/
	printf("GET DCB ret: %d: baudrate: %d, flowctrl: %d, databit: %d, stopbit: %d, paritybit: %d\n", 
				iRet, dcb.baudrate, dcb.flowctrl, dcb.databit, dcb.stopbit, dcb.parity);
	/* End: if need, to modify uart dcb config */

	return 0;
}



/*********************************************************************
* @fn	   mainuart_close
*
* @brief     close main uart
*
* @param   
*
* @return  0:success -1:fail
*
*/
int mainuart_close(void)
{
	close(fd_mainuart);
	fd_mainuart = -1;
	
	return 0;
}

/*********************************************************************
* @fn	   get_mainuart_fd
*
* @brief     get fd of main uart
*
* @param   
*
* @return  fd of main uart
*
*/
int get_mainuart_fd(void)
{
	return fd_mainuart;
}

/*********************************************************************
* @fn	   open_interface_main_uart
*
* @brief   初始化主串口
*
* @param    
*
* @return  
*
*/
void open_interface_main_uart(void)
{
    struct timeval to;

	while (0 != mainuart_open()) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "mainuart_open fail\n");
        printf("mainuart_open fail\n");
        to.tv_sec = 1;
	    to.tv_usec = 0;
		select(0,NULL,NULL,NULL,&to);
	}
}


/*********************************************************************
* @fn	   send_mainuart_msg
*
* @brief   主串口发送信息
*
* @param   output[in]: 信息内容
*          len[in]: 信息长度
*
* @return  -1: 失败，0: 成功
*
*/
int send_mainuart_msg(unsigned char *output, unsigned short len)
{
    int ret;

	ret = write(fd_mainuart, output, len);
	
	if (ret == len) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "send uart packet success\n");
        printf("send uart packet success\n");
    } else {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send uart packet fail! ret = %d\n", ret);
        printf("send uart packet fail! ret = %d\n", ret);
		return -1;
    }
    return 0;
}



/*********************************************************************
* @fn	   set_uart_packet_head
*
* @brief   串口报文头赋值
*
* @param   head[out]: 串口报文头
*          payload_len[in]: 报文负载长度
*          transimit_type[in]: 报文类型
*
* @return  0: success  -1: false
*
*/
static int set_uart_packet_head(uart_packet_header_t *head, int payload_len, unsigned char transmit_type)
{
    if(head == NULL) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid params: %p\n", head);
        printf("invalid params: %p\n", head);
        return -1;
    }

    memcpy(head->preamble, PACKET_HEAD_PREAMBLE, PACKET_HEAD_PREAMBLE_LEN);
    head->transmit_type = transmit_type;
    memcpy(head->software_version, VERSION, VERSION_LEN);
    head->length[0] = (payload_len >> 8) & 0xFF;
    head->length[1] = payload_len & 0xFF;

    return 0;
}

/*********************************************************************
* @fn	   set_uart_packet_tail
*
* @brief   串口报文尾部赋值
*
* @param   tail[out]: 串口报文尾
*          crc[in]: crc16值
*
* @return  void
*
*/
static void set_uart_packet_tail(uart_packet_tail_t *tail, unsigned short crc)
{
    tail->crc[0] = (crc >> 8) & 0xFF;
    tail->crc[1] = crc & 0xFF;
}

/*********************************************************************
* @fn	   build_uart_packet_with_data
*
* @brief   用负载数据构建串口报文
*
* @param   packet[out]: 报文
*          payload[out]: 负载数据指针
*          payload_len[in]: 负载数据长度
*          transmit_type[in]: 报文传输类型
*
* @return  0: success  -1: false
*
*/
int build_uart_packet_with_data(void *packet, unsigned char *payload, int payload_len, unsigned char transmit_type)
{
    int ret = 0;
	int body_len = 0;
	unsigned char *p = NULL;
	uart_packet_header_t *head = NULL;
    uart_packet_tail_t *tail = NULL;
    unsigned char crc_array[2];
	unsigned short crc = 0;
	
	if ((NULL == packet) || (NULL == payload)) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid param! %p, %p, %p\n", packet, payload);
        printf("Invalid param! %p, %p, %p\n", packet, payload);
		return -1;
	}

    p = packet;

	/* 报文头 */
	head = (uart_packet_header_t *)packet;
	ret = set_uart_packet_head(head, payload_len, transmit_type);
	if (ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "set_uart_packet_head fail!\n");
        printf("set_uart_packet_head fail!\n");
        goto out;
	}
	p += sizeof(uart_packet_header_t);

	/* 报文体 */
	memcpy(p, payload, payload_len);
	p += payload_len;

	/* CRC校验 */
    tail = (uart_packet_tail_t *)p;
	crc = crc16(packet, sizeof(uart_packet_header_t) + payload_len); 
    set_uart_packet_tail(tail, crc);

    p+=CRC_LEN;

    memcpy(p, PACKET_ENDCODE, PACKET_ENDCODE_LEN);

out:
	return ret;	


}


/*********************************************************************
* @fn	   build_packet_and_send_uart_msg
*
* @brief   构建报文并发送串口消息
*
* @param   payload[in]: 报文DATA
*          payload_len[in]: 报文DATA长度
*          transmit_type[in]: 串口消息类型
*
* @return  0: 成功 -1: 失败
*
*/
int build_packet_and_send_uart_msg(unsigned char* payload, int payload_len, unsigned char transmit_type)
{
    int ret = 0;
    unsigned char *packet = NULL;
	int packet_len = 0;

    packet_len = payload_len + UART_PACKET_LEN_WITHOUT_DATA;
    packet = (unsigned char *)calloc(packet_len, 1);
    ret =  build_uart_packet_with_data(packet, payload, payload_len, transmit_type);
    if(ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "build_packet_with_data fail!\n");
        printf("build_packet_with_data fail!\n");
		goto out;
    }

    ret =  send_mainuart_msg(packet, packet_len);
    if(ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send mainuart message fail!\n");
        printf("send mainuart message fail!\n");
    }

out:
    if (packet) {
		free(packet);
		packet = NULL;
	}
	return ret;
}



/*********************************************************************
* @fn	    send_keepalive_msg
* @brief    发送保活信息给CC2650
* @param    void
* @return   0: success -1: fail
* @history:
**********************************************************************/
static int send_keepalive_msg(void)
{
    int ret = 0;
    unsigned char payload[2];
    int payload_len;
    unsigned char transmit_type;


    transmit_type = SENDER_DOWNSTREAM_KEEPALIVE;
    payload[0] = SINK_EC20_HEARTBEAT;
    payload[1] = 0x00;
    payload_len = 2;

    ret = build_packet_and_send_uart_msg(payload, payload_len, transmit_type);
    if(ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "build_packet_and_send_uart_msg fail!\n");
        printf("build_packet_and_send_uart_msg fail!\n");
        return -1;
    }

    /* 报文打印出来 */
	
	return 0;
}


/*********************************************************************
* @fn	    cc2650_heartbeat_thread_fn
* @brief    心跳线程函数
* @param    arg[in]: 线程参数
* @return   void
* @history:
**********************************************************************/
void *cc2650_heartbeat_thread_fn(void *arg)
{
	/* 从数据库读取CC2650心跳时间 */
    
	
	while (1) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_DEBUG_EMERG, ">>>>time to send a keepalive msg to cc2650!\n");
        printf(">>>>time to send a keepalive msg to cc2650!\n");
		
		if (send_keepalive_msg() < 0) {
            //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send_keepalive_msg fail!\n");	
			printf("send_keepalive_msg fail!\n");	
		}	

        safe_sleep(60, 0);
		// safe_sleep(configs.ec20_heartbeat_intv, 0);
	}

	return (void *)0;
}


/*********************************************************************
* @fn	    cc2650_heartbeat_thread
* @brief    创建与CC2650之间的心跳线程
* @param    void
* @return   0: success, -1: fail
* @history:
**********************************************************************/
int cc2650_heartbeat_thread(void)
{
    int ret = 0;
    pthread_t      cc2650_heartbeat_tid;
	pthread_attr_t cc2650_heartbeat_thread_attr;
    
    //DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create cc2650 heartbeat thread!\n");
    printf("create cc2650 heartbeat thread!\n");
	pthread_attr_init(&cc2650_heartbeat_thread_attr);
	pthread_attr_setdetachstate(&cc2650_heartbeat_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&cc2650_heartbeat_tid, &cc2650_heartbeat_thread_attr, cc2650_heartbeat_thread_fn, NULL);
	if (ret) {
		//DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create cc2650 heartbeat thread error: %s\n", strerror(errno));
		printf("create cc2650 heartbeat thread error: %s\n", strerror(errno));
        return -1;
	}	
    return 0;
}




#ifndef TEST
/********************************************************************************
 * Function:       check_set_lan_config
 * Description:    读取并设置lan配置
 * Prototype:      int check_set_lan_config()
 * Param:          void
 * Return:         0:success; -1: failed
 * Other:
********************************************************************************/
int check_set_lan_config(void)
{
	char lan_ip[16] = {0};
	char lan_mask[16] = {0};
	char lan_gateway[16] = {0};
	char cmd[128] = {0};

	//读取盒子IP地址配置
	if(db_get_config_data_safe("lan_ip", lan_ip, sizeof(lan_ip), "192.168.225.1") != 0) {
		DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "get device IP address config value failed\n");
		return -1;
	}

	//读取盒子子网掩码配置
	if(db_get_config_data_safe("lan_mask", lan_mask, sizeof(lan_mask), "192.168.255.0") != 0) {
		DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "get device lan mask config value failed\n");
		return -1;
	}

	//盒子网关配置
	if(db_get_config_data_safe("lan_gateway", lan_gateway, sizeof(lan_gateway), "192.168.1.1") != 0) {
		DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "get device lan gateway config value failed\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "/etc/init.d/set_local_ip.sh \"%s\" \"%s\" \"%s\"", lan_ip,
			lan_mask, lan_gateway);

    DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "run set ip cmd:%s\n", cmd);

	do_system(cmd);

	return 0;
}

/********************************************************************************
 * Function:       check_set_uart1_config
 * Description:    读取并设置uart1配置
 * Prototype:      int check_set_uart1_config()
 * Param:          void
 * Return:         0:success; 1: failed
 * Other:
********************************************************************************/
int check_set_uart1_config(void)
{
	int ret = 0;
	//设置串口1工作模式
	ret = uart1_set_mode_db_config();
	LOG(SX_LOG_EMERG, "set uart1 mode with db config %s\n", ret == 0 ? "success" : "failed");

    //  在需要使用uart1的地方，如下所示使用数据库配置参数配置并打开uart1
    //	ST_UARTDCB dcb;
    //	uart1_set_params_db_config(&dcb);
    //	uart1_open(dcb);
    //	uart1_close();

	return ret;
}
#endif

/*********************************************************************
* @fn	    resource_clean
* @brief    进程资源回收
* @param    void
* @return   void
* @history:
**********************************************************************/
static void resource_clean(void)
{
    //DBG_CLOSE();
    //log_close(&log_monitor);
	mainuart_close();
	//db_close_database(DATABASE_PATH_DATA);
    //watchdog_close();
}


/*********************************************************************
* @fn	    sig_catch
* @brief    信号处理函数
* @param    sig_num[in]: 信号
* @return   void
* @history:
**********************************************************************/
static void sig_catch(int signum)
{
	int ret =0;

    printf(">>>>system monitor process catches a signal [%d]\n", signum);

	switch (signum) {
		case SIGINT:
			resource_clean();
			exit(0);
		case SIGUSR1:
#ifndef TEST
			ret = watchdog_close();
			if(ret != 0 ) {
                printf("soft_wdt: failed close\n");
			} else {
                printf("watchdog is closed!\n");
			}
			break;
#endif
		case SIGUSR2:
			break;

		default:
			break;
	}
}


void set_core_limit(int size_kb)
{
	struct rlimit rlim = {0, 0};

	rlim.rlim_cur = size_kb * 1024;
	rlim.rlim_max = rlim.rlim_cur;
	setrlimit(RLIMIT_CORE, &rlim);
}

void print_core_limit()
{
	struct rlimit rlim = {0, 0};

	getrlimit(RLIMIT_CORE, &rlim);
	//LOG(SX_LOG_INFO, "process core file size: %lu %lu\n", rlim.rlim_cur, rlim.rlim_max);
	printf("process core file size: %lu %lu\n", rlim.rlim_cur, rlim.rlim_max);
}

int resource_init_common()
{
	int ret = 0;

#ifndef TEST
	ret = log_impt_init();
	if (ret) {
		printf("log_impt_init fail!\n");
		return -1;
	}
#endif
	set_core_limit(5*1024); //5MB

	return ret;
}


/*********************************************************************
* @fn	    resource_init
* @brief    进程资源初始化
* @param    void
* @return   0: success -1: fail
* @history:
**********************************************************************/
static int resource_init(void)
{
    int ret = 0;
#ifndef TEST

    ret = DBG_OPEN(DEBUG_LEVEL_FILE, SX_DEBUG_OFF);
	if (ret) {
		printf("system_monitor DBG_OPEN fail!\n");
		return -1;
	}
#endif

	ret = resource_init_common();
	if (ret) {
		printf("resource_init_common fail!\n");
		return -1;
	}

#ifndef TEST

    ret = log_open(&log_monitor, LOG_FILE, LOG_LEVEL_FILE, LOG_MAX_SIZE, SX_LOG_ERROR);
	if (ret) {
		printf("system_monitor LOG_OPEN fail!\n");
		return -1;
	}

	print_core_limit();
#endif

    /* 打开测试日志模块，是否有测试日志? */

#ifndef TEST
    ret = db_open_database(DATABASE_PATH_DATA);
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "db_open_database fail!\n");
        resource_clean();
		return -1;
	}
#endif

	open_interface_main_uart();

    /* 初始化消息队列 */

    signal(SIGINT,  sig_catch);
	signal(SIGUSR1, sig_catch);
	signal(SIGUSR2, sig_catch);

#ifndef TEST
    /* 读取数据库，读取配置表，主要是看门狗的超时时间 */

    ret = watchdog_open_ex(300, "sys_monitor", 0);
	if(ret !=0) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "open watchdog fail!\n");
        printf("open watchdog fail!\n");
        resource_clean();
		return -1;
    }
#endif

    return 0;
}

#ifndef TEST

/*********************************************************************
* @fn	        send_get_deviceid_msg
* @brief        发送采集温湿度传感器信息获取设备ID
* @param        void
* @return       0: success -1: fail
* @history:
**********************************************************************/
int send_get_deviceid_msg(void)
{
	int ret = 0;
    unsigned char payload[2];
    int payload_len;
    unsigned char transmit_type;


    transmit_type = SINK_DOWNSTREAM_COLLECT_SENSOR;
    payload[0] = SINK_COLLECT_SENSOR;
    payload[1] = 0x00;
    payload_len = 2;

    ret = build_packet_and_send_uart_msg(payload, payload_len, transmit_type);
    if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "build_packet_and_send_uart_msg fail!\n");
        return -1;
    }

	return 0;
}
#endif

/*********************************************************************
* @fn	    store_ec20_boot_cnt
* @brief    存储EC20启动的次数
* @param    void
* @return   0: success -1: fail
* @history:
**********************************************************************/
int store_ec20_boot_cnt(void)
{
	FILE *fp = NULL;
	char *path = NULL;
	char boot_cnt_str[5] = {0};
	int boot_cnt = 0;

	path = FILE_EC20_BOOT_CNT;
	memset(boot_cnt_str,0,sizeof(boot_cnt_str));

	if (NULL == (fp = fopen(path,"rw+"))) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "open %s fail!\n", path);
        printf("open %s fail!\n", path);
		return -1;
	} else {
		fread(boot_cnt_str,sizeof(boot_cnt_str),1,fp);
		boot_cnt = atoi(boot_cnt_str);
        //DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "####EC20 boot times: %d, %s\n", ++boot_cnt, boot_cnt_str);
        printf("####EC20 boot times: %d, %s\n", ++boot_cnt, boot_cnt_str);
		snprintf(boot_cnt_str,sizeof(boot_cnt_str),"%d",boot_cnt);
		fseek(fp,0,SEEK_SET);
		fwrite(boot_cnt_str,sizeof(boot_cnt_str),1,fp);
	}

	fclose(fp);
	fp = NULL;
	return 0;
}



/*********************************************************************
* @fn	   check_crc
*
* @brief   check whether the crc is valid
*
* @param   p[in]: pointer to the buffer to calc crc
*		   len[in]: length of the buffer
*
* @return  0: success -1: false
*
*/
int check_crc(unsigned char *p, int len)
{
	unsigned short crc = 0;
    unsigned short cur_crc16 = 0;

    if ((NULL == p) || (0 == len)) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid param! %p,%d\n", p, len);
        printf("Invalid param! %p,%d\n", p, len);
		return -1;
	}

    cur_crc16 = ((*(p+len-2)) << 8) | (*(p+len-1));
	
	crc = crc16(p, len - 2);

	if (crc != cur_crc16) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid crc16!\n");
        printf("Invalid crc16!\n");
		return -1;
	}

	return 0;
}



/*********************************************************************
* @fn	   check_preamble
*
* @brief   check whether the preamble is valid
*
* @param   p[in]: pointer to the preamble
*
* @return  0: success  -1: false
*
*/
int check_preamble(unsigned char *p)
{
	if (NULL == p) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid param! %p\n", p);
        printf("Invalid param! %p\n", p);
		return -1;
	}

	if (0 != memcmp(p, PACKET_HEAD_PREAMBLE, PACKET_HEAD_PREAMBLE_LEN)) {
        // DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid preamble!\n");
		return -1;
	}

	return 0;
}

/*********************************************************************
* @fn	   check_protocol_version
*
* @brief   check whether the protocol version is valid
*
* @param   p[in]: pointer to the protocol version
*
* @return  0:success -1:false
*
*/
int check_protocol_version(unsigned char *p)
{
	if (NULL == p) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid param!%p\n", p);
        printf("Invalid param!%p\n", p);
		return -1;
	}

	if (0 != memcmp(p, VERSION, VERSION_LEN)) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid protocol version!\n");
        printf("Invalid protocol version!\n");
		return -1;
	}

	return 0;
}



/*********************************************************************
* @fn	   check_heartbeat_msg_context
*
* @brief   检查串口心跳消息正文
*
* @param   msg[in]: 报文
*          len[in]: 报文长度
*
* @return  0：成功 -1: 失败
*
*/
int check_heartbeat_msg_context(unsigned char *msg, int len)
{
    int ret = 0;

    if(sizeof(struct sink_heartbeat_msg) != len) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Invalid packet len: %d\n", len);
        printf("Invalid packet len: %d\n", len);
        return -1;
    }

    struct sink_heartbeat_msg *hb_msg = (struct sink_heartbeat_msg *)msg;

    if(hb_msg->heartbeat_value.cc2650_heartbeat_type != CC2650_HEARTBEAT_TYPE) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid heartbeat type! %d\n", hb_msg->heartbeat_value.cc2650_heartbeat_type);
        printf("invalid heartbeat type! %d\n", hb_msg->heartbeat_value.cc2650_heartbeat_type);
        return -1;
    }

    if(hb_msg->heartbeat_value.cc2650_heartbeat_len != CC2650_HEARTBEAT_LEN) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid heartbeat len! %d\n", hb_msg->heartbeat_value.cc2650_heartbeat_len);
        printf("invalid heartbeat len! %d\n", hb_msg->heartbeat_value.cc2650_heartbeat_len);
        return -1;
    }

    return 0;
}



/*********************************************************************
* @fn	    parse_cc2650_comm_msg
* @brief    解析与CC2650串口通信报文信息
* @param    msg[in]: 串口报文内容
* @param    len[in]: 串口报文长度
* @return   0: succes, -1: fail
* @history:
**********************************************************************/
int parse_cc2650_comm_msg(unsigned char *msg, int len) 
{
    int ret = 0;
    uart_packet_header_t  head = {0};
    int context_len;


    /* 检查CRC */
    ret = check_crc(msg, len);

    /* 复制报文头 */
    memcpy(&head, msg, sizeof(uart_packet_header_t));

    /* 检查协议版本 */
    ret = check_protocol_version(head.software_version);

    switch(head.transmit_type) {
#ifndef TEST
        case SINK_UPSTREAM_TEMPHUM:
            /**
             * 检查串口特征信息报文正文
             */
            ret = check_temphum_msg_context(msg, len);
            if(ret) {
                DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "check temphum message context error\n");
                return -1;
            }

            /**
             * 存储报文中数据，当DEVICEID不存在时设置DEVICEID 
             */
            ret = store_temphum_msg_data(msg);
            break;
#endif
        case SINK_UPSTREAM_HEARTBEAT:
            /**
             * 检查心跳报文正文, 返回EC20心跳保活报文 
             */
            ret =  check_heartbeat_msg_context(msg, len);
            if(ret) {
                //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "check heartbeat message context error\n");
                printf("check heartbeat message context error\n");
                return -1;
            }

            //DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "receive upstream heartbeat message\n");
            printf("receive upstream heartbeat message\n");

            ret =  send_keepalive_msg();
            if(ret) {
                //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send keepalive message fail\n");
                printf("send keepalive message fail\n");
                return -1;
            }        
            break;
        default:
            //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid uart message transmit type\n");
            printf("invalid uart message transmit type\n");
            return -1;
    }

    return 0; 
}



/*********************************************************************
* @fn	    handle_cc2650_uart_msg
* @brief    解析cc2650串口发送过来的消息，通信报文有可能和调试信息混在一起，
*           要将通信报文和调试信息分开
* @param    buffer[in]: 缓冲区指针
* @param    len[in]: 缓冲区长度
* @return   0: succes, -1: fail
* @history:
**********************************************************************/
int handle_cc2650_uart_msg(unsigned char *buffer, int len)
{
	int ret = -1;
	int i = 0;
    int j = 0;
    int comm_msg_index = 0;
    int offset = 0;

	struct cc2650_comm_msg comm_msg[MAX_COMM_MSG_NUM] = {0};
    /* CC2650通信报文个数 */
    int comm_msg_num = 0;
	
	while((i+4) < len) {	
        ret = check_preamble(buffer + i);
        if(ret) {
            i++;
        } else {
            if(comm_msg_num >= MAX_COMM_MSG_NUM) {
                break;
            }
            comm_msg[comm_msg_num].start = i;
            comm_msg[comm_msg_num].len = (buffer[i+7]<<8 | buffer[i+8]) + sizeof(uart_packet_header_t)
                                            + sizeof(uart_packet_tail_t);

            i += comm_msg[comm_msg_num].len;

            if((comm_msg[comm_msg_num].start + comm_msg[comm_msg_num].len) > len) {
                //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid packet length\n");
                printf("invalid packet length\n");
                break;
            }
            
            /* todo: 报文存到测试日志 */

            /* 处理CC2650通信报文 */
            ret = parse_cc2650_comm_msg(&buffer[comm_msg[comm_msg_num].start], comm_msg[comm_msg_num].len);

            comm_msg_num++;
        }
    }

    /* 打印和存储调试信息到日志 */
    if (comm_msg_num == 0) {
            printf("\n");
            for(j = 0; j < len; j++) {
			    printf("%c", buffer[j]);
		    }
            printf("\n");
        }
        /* todo: 存储到测试日志 */

	return 0;

}

/*********************************************************************
* @fn	    handle_mainuart_thread_fn
* @brief    处理主串口接收数据的线程函数
* @param    arg[in]: 线程参数
* @return   void
* @history:
**********************************************************************/
void *handle_mainuart_thread_fn(void *arg)
{
    int ret = 0;
    int fd_mainuart = -1;
    fd_set fdset;
    struct timeval timeout = {SERIAL_SELECT_TIME_OUT, 0};
    unsigned char buffer[MAIN_UART_RECV_DATA_LEN] = {0};
    int len;

    fd_mainuart = get_mainuart_fd();

    while(1) {

        /** 
         * 想办法检查接收特征数据的报文是否超时
         * 超时需要重新发送获取特征数据报文
         */
        // check_recv_sink_feature_msg_timeout();

	    FD_ZERO(&fdset); 
	    FD_SET(fd_mainuart, &fdset); 
				
		ret = select(fd_mainuart+1, &fdset, NULL, NULL, &timeout);
		if (-1 == ret) {
			continue;
		} else if (0 == ret) {
			timeout.tv_sec  = SERIAL_SELECT_TIME_OUT;
			timeout.tv_usec = 0;
			continue;
		} else {
			if (FD_ISSET(fd_mainuart, &fdset))  {	
				
                memset(buffer, 0x0, sizeof(buffer));
				ret = read(fd_mainuart, buffer, MAIN_UART_RECV_DATA_LEN);
				
				if (ret < 0) {
                    //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "read uart data fail\n");
                    printf("read uart data fail\n");
					continue;
				}

				len = ret;

				/* 解析CC2650串口发送的消息 */
				ret = handle_cc2650_uart_msg(buffer, len);
				if (ret) {
                    //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "parse cc2650 uart msg error:%s\n", strerror(errno));
                    printf("parse cc2650 uart msg error:%s\n", strerror(errno));
				}		
			}
		}
    }

}



/*********************************************************************
* @fn	    handle_mainuart_thread
* @brief    创建处理主串口接收数据的线程
* @param    void
* @return   0: success, -1: fail
* @history:
**********************************************************************/
int handle_mainuart_thread(void)
{
    int ret = 0;
    pthread_t      handle_mainuart_tid;
	pthread_attr_t handle_mainuart_thread_attr; 

    /* 创建处理串口接收数据的线程 */
    //DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create handle_mainuart thread!\n");
    printf("create handle_mainuart thread!\n");
	pthread_attr_init(&handle_mainuart_thread_attr);
	pthread_attr_setdetachstate(&handle_mainuart_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&handle_mainuart_tid, &handle_mainuart_thread_attr, handle_mainuart_thread_fn, NULL);
	if (ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create handle_mainuart thread error: %s\n", strerror(errno));
        printf("create handle_mainuart thread error: %s\n", strerror(errno));
		return -1;
	}	
    return 0;
}


/*********************************************************************
* @fn	    main
* @brief    进程主函数
* @param    argc[in]: 参数个数
* @param    argv[in]: 参数列表
* @return   0: success -1: fail
* @history:
**********************************************************************/
int main(int argc, char *argv[])
{
	int ret = 0;

    // 系统监测进程一直存在，可以作为判断EC20复位成功的依据
    printf("\nHWTI_BEG:EC20 RST success:HWTI_END\n");
	printf("Process system_monitor is starting!\n");

    ret = resource_init();
    if(ret) {
        resource_clean();
        printf("resource init failed，clean resource\n");
        return -1;
    }

	// 存储开机次数
	store_ec20_boot_cnt();

#ifndef TEST
	printf("system monitor set ip address with db config\n");
	LOG(SX_LOG_EMERG, "set ip address\n");
	check_set_lan_config();

	printf("system monitor set uart1 work mode\n");
	LOG(SX_LOG_EMERG, "set uart1 work mode\n");
	check_set_uart1_config();

	ret = create_wdog_feed_thread(60);
	if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create watchdog thread fail!\n");
        resource_clean();
		return -1;
	}
#endif

    ret = cc2650_heartbeat_thread();
    if(ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create cc2650 heartbeat thread fail!\n");
        printf("create cc2650 heartbeat thread fail!\n");
        resource_clean();
		return -1;
	}

    ret = handle_mainuart_thread();
    if(ret) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create handle mainuart thread fail!\n");
        printf("create handle mainuart thread fail!\n");
        resource_clean();
		return -1;
	}

    //睡眠一段时间确保主串口接收线程已开启
    safe_sleep(5, 0);

#ifndef TEST
    //开机发送获取设备ID的报文
    send_get_deviceid_msg();


    wait_for_valid_time("system_monitor");

    ret = monitor_upgrade_thread();
    if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create monitor upgrade thread fail!\n");
        resource_clean();
		return -1;
	}

    ret = collect_temphum_sensor_thread();
    if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create temphum sensor thread fail!\n");
        resource_clean();
		return -1;
	}

    /**
     * 升级监控应该是一个事件，平台下行的命令应该做成可配置的，关联某一个具体的函数或动作
     * 创建节点版本上传的心跳线程
     */
    // ret = node_version_up_thread();
    // if(ret) {
    //     DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create node version up thread fail!\n");
    //     resource_clean();
	// 	return -1;
	// }
#endif


	while (1) {
		safe_sleep(60, 0);
	}
}
