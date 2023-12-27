#include <stdio.h>
#include <pthread.h>
#include <sys/select.h> 
#include <sys/time.h> 
#include <errno.h>
#include "cc2650_mainuart_protocol.h"
#include "suxin_database.h"
#include "debug_and_log.h"
#include "util.h"
#include "common.h"

/*********************************************************************
* @fn	    send_get_temphum_msg
* @brief    发送采集温湿度传感器数据的信息
* @param    void     
* @return   0: success -1: fail
* @history:
**********************************************************************/
int send_get_temphum_msg(void)
{
	int ret = 0;
    unsigned char payload[2];
    int payload_len;
    unsigned char transmit_type;


    transmit_type = SINK_DOWNSTREAM_COLLECT_SENSOR;
    payload[0] = SINK_COLLECT_SENSOR;
    payload[1] = 0x00;
    payload_len = 2;

    /* 包含3次重试 */

    ret = build_packet_and_send_uart_msg(payload, payload_len, transmit_type);
    if(ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "build_packet_and_send_uart_msg fail!\n");
        return -1;
    }
	
	return 0;
}


/*********************************************************************
* @fn	    collect_temphum_thread_fn
* @brief    采集温湿度传感器的线程函数
* @param    args[in]: 线程参数     
* @return   void
* @history:
**********************************************************************/
void *collect_temphum_thread_fn(void *arg)
{
    int ret;


    while(1) {

        send_get_temphum_msg(); 
        
        safe_sleep(10, 0);
    }

}


/*********************************************************************
* @fn	    collect_temphum_sensor_thread
* @brief    创建采集温湿度传感器的线程
* @param    void     
* @return   0: success -1: fail
* @history:
**********************************************************************/
int collect_temphum_sensor_thread(void)
{
    int ret;
    pthread_t collect_temphum_sensor_tid;
	pthread_attr_t collect_temphum_sensor_attr;
    
    /* 读取数据库查询有无传感器通道, 有无温湿度设备 */


    pthread_attr_init(&collect_temphum_sensor_attr);
	pthread_attr_setdetachstate(&collect_temphum_sensor_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&collect_temphum_sensor_tid,  &collect_temphum_sensor_attr,  collect_temphum_thread_fn, NULL);
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create temphum sensor thread error: %s\n", strerror(errno));
		return -1;
	}
}