#include "app.h"

#define IS_64bit() (sizeof(long) == 8) 
#define IS_32bit() (sizeof(long) == 4)

int main()
{
    int ret = 1;
    if (IS_64bit()) 
    {
        printf("64Bit CPU.\n");
    } 
    else 
    {
        printf("32Bit CPU.\n");
    }
	 
    /*
        主进程初始化步骤：
        1. 申请内存空间
        2. 创建消息队列
        3. 初始化主线程
        3. 初始化子线程
        4. 其他任务
    */
	SYSM_LOG_FAULT(APP,"At the time the program begain restart !");
/*    ret = app_pfm_init();
    if (OK != ret) 
    {   
        Adc_Ky_log("app prm init fail.");
        return ERR;
    }
    
    ret = app_reset_init();
    if (OK != ret) 
    { 
        Adc_Ky_log("app reset init fail.");
        return ERR;
    }*/

    ret = app_module_init();
    if (OK != ret)
    {  
        printf("app module init fail.\n");
        return ERR;
    }
	
    ret = app_main_init();
    if (OK != ret) 
    {   
        printf("app main init fail.\n");
        return ERR;
    }

    ret = app_module_start();
    if (OK != ret) 
    {   
        printf("app module start fail.\n");
        return ERR;
    }

    ret = app_main_start();
    if (OK != ret) 
    {   
        printf("app main process start fail.\n");
        return ERR;
    }
    
	printf("app initial end.\n");
	
	return 0;
}




