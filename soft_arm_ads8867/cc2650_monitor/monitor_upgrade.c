#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "debug_and_log.h"
#include "watchdog.h"
#include "util.h"
#include "message_queue.h"
#include "suxin_database.h"
#include "common.h"
#include "system_monitor.h"

/* 版本信息长度 */
// #define MAX_HW_VERSION_LEN 30
// #define MAX_KERNEL_VERSION_LEN 30
// #define MAX_CUR_SW_VERSION_LEN 30
// #define MAX_NEW_SW_VERSION_LEN 30
// #define MAX_SN_STR_LEN 10

/*********************************************************************
* @fn	    send_dfota_rsp
* @brief    发送dfota响应信息
* @param    status[in]:　0:升级成功 1:升级失败
* @param    description[in]: 描述信息
* @param    sn[in]: 升级报文的SN
* @return   0: success, -1: fail
* @history:
**********************************************************************/
// int send_dfota_rsp(unsigned char status, char *description, unsigned short sn)
// {
//     int ret = -1;
// 	unsigned char pktbuf[128] = {0};
// 	int pktbuf_offset = 0;
// 	unsigned char *p = pktbuf;
// 	pkt_data_item_t* pdata_item = NULL;
// 	unsigned char data_item_buf[128] = {0};
// 	int data_item_len = 0; 
// 	unsigned short u_value_16 = 0;
//     char deviceid[DEVICE_ID_LEN + 1];
//     mqtt_topic_t topic;
//     mqtt_packet_params_t params;


//     ret = get_device_id_str(deviceid, ROUTER);
// 	if (ret) {
// 		dbg_print(DEBUG_ERROR,__FILE__,__func__,__LINE__,"get_device_id fail!\n"); 
// 		LOG(0,"get_device_id fail!\n"); 
// 		return (void *)-1;
// 	}
	
//     /* 升级状态赋值　*/
//     memset(pktbuf, 0, sizeof(pktbuf));
//     memset(data_item_buf, 0, sizeof(data_item_buf));
//     pdata_item = (pkt_data_item_t*)data_item_buf;
// 	pdata_item->type = RET_CODE;
//     u_value_16 = SWAP16(1);
//     memcpy(pdata_item->len, &u_value_16, PACKET_PAYLOAD_LEN_LEN);
//     memcpy(pdata_item->value, &status, 1);
//     data_item_len = sizeof(pkt_data_item_t) + 1;
// 	memcpy(p + pktbuf_offset, pdata_item, data_item_len);
// 	pktbuf_offset += data_item_len;

//     /*　升级失败或成功时返回描述信息　*/
//     if(description == NULL) {
//         dbg_print(DEBUG_ERROR, __FILE__, __func__, __LINE__, "invalid description!\n"); 
// 		LOG(0, "invalid description!\n"); 
// 		return (void *)-1;
//     }
//     memset(data_item_buf, 0, sizeof(data_item_buf));
//     pdata_item = (pkt_data_item_t*)data_item_buf;
// 	pdata_item->type = RET_DESC;
//     u_value_16 = SWAP16(strlen(description));
//     memcpy(pdata_item->len, &u_value_16, PACKET_PAYLOAD_LEN_LEN);
//     memcpy(pdata_item->value, description, strlen(description));
//     data_item_len = sizeof(pkt_data_item_t) + strlen(description);
// 	memcpy(p + pktbuf_offset, pdata_item, data_item_len);       
//     pktbuf_offset += data_item_len;    


//     /* 
//     * 升级RSP报文不存库，直接通过消息队列传给mosquitto_pub进程，
//     * 存库没有什么意义，下次再发的时候已经早就超时了
//     */
//     strcpy(topic.device_type, "Combo");
//     strcpy(topic.device_id, deviceid);
//     strcpy(topic.channel, "CMD");

//     params.payload = pktbuf;
//     params.payload_len = pktbuf_offset;
//     params.cmd = UPGRADE_DFOTA_RSP;
//     /* 升级响应信息的sn与接收到的升级报文的sn保持一致　*/
//     params.sn = sn;
//     params.compr_flag = 0;
    
// 	ret = build_packet_and_send_publish_msg(params, topic, NULL, SYSTEM_MONITOR_PUB);
// 	if(ret == 0) {
//         LOG(0, "####send dfota ack msg success(sn: %hu)!\n", params.sn);
// 	} else {
//         LOG(0, "####send dfota ack msg failed(sn: %hu)!\n", params.sn);
// 	}

// }


/*********************************************************************
* @fn	    monitor_upgrade_thread_fn
* @brief    监测升级状态的线程函数
* @param    arg[in]: 线程参数
* @return   void
* @history:
**********************************************************************/
void * monitor_upgrade_thread_fn(void *arg)
{
	int ret = -1;
    unsigned char ack_status = 0;
    unsigned short sn;
    char sn_str[10];
    FILE *fp = NULL;
    char new_version[MAX_NEW_SW_VERSION_LEN] = {0};
    char node_cur_sw_version[MAX_CUR_SW_VERSION_LEN] = {0};

    DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "monitor upgrade thread is starting\n");
	
    /* 判断升级文件是否存在　*/
	if (access(DFOTA_STATUS, F_OK) == -1) {
		return (void *)0;
	}

    /*　判断routernode_deviceid文件是否存在, 升级成功与升级失败之后这个文件是否还存在, 需要实测一下 */
	while (access(ROUTERNODE_DEVICEID_FILE, F_OK)) {
        DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "routenode_device file not exist\n");
		safe_sleep(5, 0);
	}

    /* 
    * 这里要判断要升级的版本与现在的版本是否一致，如果不一致认定为升级失败，
    * 判断是否升级包版本未改过来还是发生了系统还原　
    */
    fp = fopen(DFOTA_STATUS, "r");
	if (fp == NULL) {
		DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "fopen %s fail! %s\n", DFOTA_STATUS, strerror(errno));
		return (void *)-1;
	}
    fseek(fp, 0, SEEK_SET );
    if(fgets(sn_str, MAX_SN_STR_LEN, fp) != NULL) {
        sn = atoi(sn_str);
        printf("sn: %d\n", sn);
    } else {
        fclose(fp);
        return (void *)-1;
    }
    if(fgets(new_version, MAX_NEW_SW_VERSION_LEN, fp) != NULL) {
        printf("new_version:%s\n", new_version);
    } else {
        fclose(fp);
        return (void *)-1;
    }
    fclose(fp);

    get_node_version("Software Ver(EC20)", node_cur_sw_version);

    /* 升级成功与失败时发送响应信息给平台　*/
    if(strcmp(new_version, node_cur_sw_version) != 0) {
        // send_dfota_rsp(1, "升级未成功，系统还原", sn);

        DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "####dfota upgrade fail! Trigger a UPGRADE_DFOTA_RSP fail event!\n");
        /* 将事件信息写入数据库 */

        remove(DFOTA_STATUS);
	    remove(DFOTA_FILE);
        remove(DFOTA_FILE_MD5);
        return (void *)-1;
    } else {
        /* 升级成功，返回升级成功响应信息，为空格 */
        // send_dfota_rsp(0, "升级成功", sn);

        DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "####dfota upgrade success! Trigger a UPGRADE_DFOTA_RSP success event!\n");
        /* 将事件信息写入数据库 */

        remove(DFOTA_STATUS);
	    remove(DFOTA_FILE);
        remove(DFOTA_FILE_MD5);
    }

	return (void *)0;
}


/*********************************************************************
* @fn	    monitor_upgrade_thread
* @brief    创建监测升级状态的线程
* @param    void
* @return   0: success, -1: fail
* @history:
**********************************************************************/
int monitor_upgrade_thread(void)
{
    int ret = 0;
    pthread_t      monitor_upgrade_tid;
	pthread_attr_t monitor_upgrade_thread_attr;

    /* 创建升级监控线程来处理，升级的ACK应该是一个事件 */
    DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create monitor upgrade thread!\n");
	pthread_attr_init(&monitor_upgrade_thread_attr);
	pthread_attr_setdetachstate(&monitor_upgrade_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&monitor_upgrade_tid, &monitor_upgrade_thread_attr, monitor_upgrade_thread_fn, NULL);
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create monitor upgrade thread error!\n");
		return -1;
	}	
    
    return 0;
}

