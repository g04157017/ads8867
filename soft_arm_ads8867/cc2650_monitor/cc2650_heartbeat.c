#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "debug_and_log.h"
#include "cc2650_mainuart_protocol.h"
#include "util.h"
#include "suxin_database.h"
#include "common.h"
#include "mainuart.h"
#include "system_monitor.h"


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
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "build_packet_and_send_uart_msg fail!\n");
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

        DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_DEBUG_EMERG, ">>>>time to send a keepalive msg to cc2650!\n");
		
		if (send_keepalive_msg() < 0) {
            DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send_keepalive_msg fail!\n");	
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
    
    DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create cc2650 heartbeat thread!\n");
	pthread_attr_init(&cc2650_heartbeat_thread_attr);
	pthread_attr_setdetachstate(&cc2650_heartbeat_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&cc2650_heartbeat_tid, &cc2650_heartbeat_thread_attr, cc2650_heartbeat_thread_fn, NULL);
	if (ret) {
		DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create cc2650 heartbeat thread error: %s\n", strerror(errno));
        return -1;
	}	
    return 0;
}


