#include "statistics.h"
#include "debug_and_log.h"


#define FILE_CC2650_REBOOT_EC20_CNT "/home/root/stats/cc2650_reboot_ec20_cnt" //EC20被CC2650重启的次数
#define FILE_CC2650_BOOT_CNT        "/home/root/stats/cc2650_boot_cnt" //CC2650的启动次数
#define FILE_EC20_BOOT_CNT          "/home/root/stats/ec20_boot_cnt" //EC20的启动次数


/*********************************************************************
* @fn	    store_cc2650_reboot_ec20_cnt
* @brief    存储CC2650重启EC20的次数
* @param    reboot_cnt[in]: 重启次数
* @return   0: success -1: fail
* @history:
**********************************************************************/
int store_cc2650_reboot_ec20_cnt(unsigned short reboot_cnt)
{
	FILE *fp = NULL;
	char *path = NULL;
	char reboot_cnt_str[8] = {0};

	path = FILE_CC2650_REBOOT_EC20_CNT;

	snprintf(reboot_cnt_str,sizeof(reboot_cnt_str),"%hu",reboot_cnt);

	if (NULL == (fp = fopen(path,"w"))) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "fopen %s fail!\n",path);
		return -1;
	} else {
		if (fwrite(reboot_cnt_str,sizeof(reboot_cnt_str),1,fp) != 1) {
            DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "fwrite %s fail! %s\n", path, strerror(errno));
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}


/*********************************************************************
* @fn	    store_cc2650_boot_cnt
* @brief    存储CC2650启动的次数
* @param    reboot_cnt[in]: 重启次数
* @return   0: success -1: fail
* @history:
**********************************************************************/
int store_cc2650_boot_cnt(unsigned short reboot_cnt)
{
	FILE *fp = NULL;
	char *path = NULL;
	char reboot_cnt_str[8] = {0};

	path = FILE_CC2650_BOOT_CNT;
	
	snprintf(reboot_cnt_str,sizeof(reboot_cnt_str),"%hu",reboot_cnt);

	if (NULL == (fp = fopen(path,"w"))) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "fopen %s fail!\n", path);
		return -1;
	} else {
		if (fwrite(reboot_cnt_str,sizeof(reboot_cnt_str),1,fp) != 1) {
            DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "fwrite %s fail! %s\n", path, strerror(errno));
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}


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

	if (NULL == (fp = fopen(path,"r+"))) {
        DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "open %s fail!\n", path);
		return -1;
	} else {
		fread(boot_cnt_str,sizeof(boot_cnt_str),1,fp);
		boot_cnt = atoi(boot_cnt_str);
        DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "####EC20 boot times: %d, %s\n", ++boot_cnt, boot_cnt_str);
		snprintf(boot_cnt_str,sizeof(boot_cnt_str),"%d",boot_cnt);
		fseek(fp,0,SEEK_SET);
		fwrite(boot_cnt_str,sizeof(boot_cnt_str),1,fp);
	}

	fclose(fp);
	fp = NULL;
	return 0;
}


/*********************************************************************
* @fn	    get_lte_offline_stats_info
* @brief    获取4G离线统计信息
* @param    stats[in]: 统计信息 
* @param    len[in]: 长度
* @return   0: success -1: fail
* @history:
**********************************************************************/
int get_lte_offline_stats_info(unsigned char **stats,int *len)
{

}


/*********************************************************************
* @fn	    get_plat_offline_stats_info
* @brief    获取平台断连统计信息
* @param    stats[in]: 统计信息 
* @param    len[in]: 长度
* @return   0: success -1: fail
* @history:
**********************************************************************/
int get_plat_offline_stats_info(unsigned char **stats,int *len)
{

}


/*********************************************************************
* @fn	    get_reboot_cnt_stats_info
* @brief    获取EC20重启次数统计信息
* @param    stats[in]: 统计信息 
* @param    len[in]: 长度
* @return   0: success -1: fail
* @history:
**********************************************************************/
int get_reboot_cnt_stats_info(unsigned char **stats,int *len)
{

}


/*********************************************************************
* @fn	    get_process_crash_stats_info
* @brief    获取进程挂起统计信息
* @param    stats[in]: 统计信息 
* @param    len[in]: 长度
* @return   0: success -1: fail
* @history:
**********************************************************************/
int get_process_crash_stats_info(unsigned char **stats,int *len)
{

}


/*********************************************************************
* @fn	    get_system_status_stats_info
* @brief    获取系统状态统计信息
* @param    stats[in]: 统计信息 
* @param    len[in]: 长度
* @return   0: success -1: fail
* @history:
**********************************************************************/
int get_system_status_stats_info(unsigned char **stats,int *len)
{

}


