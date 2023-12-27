#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "debug_and_log.h"
#include "cc2650_mainuart_protocol.h"
#include "watchdog.h"
#include "util.h"
#include "message_queue.h"
#include "suxin_database.h"
#include "common.h"
#include "mainuart.h"
#include "data_store.h"
#include "statistics.h"


/* 接收温湿度特征数据的最大时间，单位秒 */
#define MAX_RECV_SINK_FEATURE_MSG_TIME         10

/* 发送获取温湿度特征数据命令的重传最大次数 */
#define MAX_RESEND_SINK_GET_FEATURE_MSG_TIMES   3

/* 串口select超时时间，单位秒 */
#define SERIAL_SELECT_TIME_OUT 1	

#define MAIN_UART_RECV_DATA_LEN		10000

/**
 * CC2650通信报文个数
 * @start: 开始位置
 * @len: 报文长度
 */ 
struct cc2650_comm_msg {
    int start;
    int len;
};

#define MAX_COMM_MSG_NUM 10


/*********************************************************************
* @fn	    store_temphum_msg_data
* @brief    存储温湿度数据
* @param    msg[in]: 串口报文
* @return   0: succes, -1: fail
* @history:
**********************************************************************/
int store_temphum_msg_data(unsigned char *msg)
{
    int ret = 0;
    int row = 0;
    short hdc_temp = 0, hdc_humidity = 0, ntc_temp = 0;
    sql_param_t param;
    struct timeval tv;

    if(msg == NULL) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid params: %p\n", msg);
        return -1;
    }

    struct sink_temphum_msg *temphum_msg = (struct sink_temphum_msg *)msg;

    //存储开机次数
    ret = store_cc2650_reboot_ec20_cnt(COMBINE16(temphum_msg->system_stats.reboot_ec20_value));
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "store_cc2650_reboot_ec20_cnt fail!\n");
        return -1;
    }
	ret = store_cc2650_boot_cnt(COMBINE16(temphum_msg->system_stats.cc2650_boot_value));
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "store_cc2650_boot_cnt fail!\n");
        return -1;
    }

    // 检查设备ID
    ret = check_device_id(temphum_msg->net_params.device_id, DEVICE_ID_LEN);
    if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "check device id fail\n");
		return -1;
    }

    hdc_temp = COMBINE16(temphum_msg->temphum_value.hdc_temp);
    hdc_humidity = COMBINE16(temphum_msg->temphum_value.hdc_humidity);
    ntc_temp = COMBINE16(temphum_msg->temphum_value.ntc_temp);

    // 如果为生产模式，打印温湿度以及NTC信息　
    if(get_hw_mode() == PRODUCTION_MODE) {
        printf("\nHWTI_BEG:HTU20D_AND_NTC success, hdc_temp: %hd, hdc_humidity: %hd, ntc_temp: %hd.:HWTI_END\n",
		hdc_temp, hdc_humidity, ntc_temp);
    }

    // 将温湿度和NTC数据存储到数据库 
    gettimeofday(&tv, NULL);
    put_data_to_db("DT0001", &hdc_temp, 2, &tv);
    put_data_to_db("DT0002", &hdc_humidity, 2, &tv);
    put_data_to_db("DT0003", &ntc_temp, 2, &tv);

    return 0;
}


/*********************************************************************
* @fn	    send_keepalive_msg
* @brief    发送保活报文给CC2650
* @param    void
* @return   0: succes, -1: fail
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
        case SINK_UPSTREAM_HEARTBEAT:
            /**
             * 检查心跳报文正文, 返回EC20心跳保活报文 
             */
            ret =  check_heartbeat_msg_context(msg, len);
            if(ret) {
                DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "check heartbeat message context error\n");
                return -1;
            }

            DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "receive upstream heartbeat message\n");

            ret =  send_keepalive_msg();
            if(ret) {
                DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "send keepalive message fail\n");
                return -1;
            }        
            break;
        default:
            DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid uart message transmit type\n");
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
                DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "invalid packet length\n");
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
        /**
         * 没有通信报文，只有调试信息， 
         * 生产模式下，打印CC2650的调试信息
         */
        if(get_hw_mode() == PRODUCTION_MODE) {
            printf("\n");
            for(j = 0; j < len; j++) {
			    printf("%c", buffer[j]);
		    }
            printf("\n");
        }
        /* todo: 存储到测试日志 */
	} else  {
        /**
         * 有通信报文，也有调试信息，
         * 生产模式下，打印CC2650的调试信息 
         */
		for(i = 0; i < comm_msg_num; i++) {
			if ((i == 0) && (comm_msg[i].start > 0)) {

                /* todo: 生产模式下打印，存储到测试日志 */
			} else if (comm_msg[i].start > 0) {

                /* todo: 生产模式下打印，存储到测试日志 */
		    }
		}	
	}

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
                    DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "read uart data fail\n");
					continue;
				}

				len = ret;

				/* 解析CC2650串口发送的消息 */
				ret = handle_cc2650_uart_msg(buffer, len);
				if (ret) {
                    DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, 
                    "parse cc2650 uart msg error:%s\n", strerror(errno));
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

    /* 读取数据库获取温湿度数据的数据ID */
    

    /* 创建处理串口接收数据的线程 */
    DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create handle_mainuart thread!\n");
	pthread_attr_init(&handle_mainuart_thread_attr);
	pthread_attr_setdetachstate(&handle_mainuart_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&handle_mainuart_tid, &handle_mainuart_thread_attr, handle_mainuart_thread_fn, NULL);
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create handle_mainuart thread error: %s\n", strerror(errno));
		return -1;
	}	
    return 0;
}
    