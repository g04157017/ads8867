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


/*********************************************************************
* @fn	    node_version_up_thread_fn
* @brief    节点版本信息上传线程函数
* @param    arg[in]: 线程参数
* @return   void
* @history:
**********************************************************************/
void *node_version_up_thread_fn(void *arg)
{	
    int ret = -1;

    /* 触发一个版本信息上传的事件, 或者周期性上传 */
    
	while (1) {


        safe_sleep(300, 0);
        /* 将版本信息写入数据库 */

	}

	return (void *)0;
}


/*********************************************************************
* @fn	    node_version_up_thread
* @brief    创建节点版本信息上传的线程，版本信息应该周期性上传
* @param    void
* @return   0: success, -1: fail
* @history:
**********************************************************************/
int node_version_up_thread(void)
{
    int ret = 0;
    pthread_t      node_version_up_tid;
	pthread_attr_t node_version_up_thread_attr;

    /* 创建版本信息上传的线程 */
    DBG_PRINT_AND_LOG(SX_DEBUG_TRACE, SX_LOG_TRACE, "create node version up thread!\n");
	pthread_attr_init(&node_version_up_thread_attr);
	pthread_attr_setdetachstate(&node_version_up_thread_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&node_version_up_tid, &node_version_up_thread_attr, node_version_up_thread_fn, NULL);
	if (ret) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "create node version up thread error: %s\n", strerror(errno));
		return -1;
	}	
    return 0;
}





