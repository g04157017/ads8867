/*
*****************************************************
* Copyright (c) 2020,江苏创博
* All rights reserved.
* 程序名称：	led.c
* 版本号：		1.0
* 功能：		led 驱动
* 原始开发人及时间：
* 刘鹏	20020-06-03
* 修改人员、时间及修改内容：     
* 其他： 
*****************************************************
*/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>

#include "led.h"

static unsigned char s_led_init_state=0;//指示LED灯是否被初始化过
//打印信息的并显示
static void pabort(const char *s)
{
	perror(s);
	abort();
}

/**	
* led2_open
* 功 能：打开led2灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led2_open()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_BUSY_GPIO5_IO6_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO6_PIN_H, sizeof(SYSFS_RST_GPIO5_IO6_PIN_H));	//设置为高电平
	close(fd);
}

/**	
* led2_close
* 功 能：关闭led2灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led2_close()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_BUSY_GPIO5_IO6_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO6_PIN_L, sizeof(SYSFS_RST_GPIO5_IO6_PIN_L));	//设置为低电平
	close(fd);
}

/**	
* led3_open
* 功 能：打开led3灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led3_open()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO5_IO8_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO8_PIN_H, sizeof(SYSFS_RST_GPIO5_IO8_PIN_H));	//设置为高电平
	close(fd);
}

/**	
* led3_close
* 功 能：关闭led3灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led3_close()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO5_IO8_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO8_PIN_L, sizeof(SYSFS_RST_GPIO5_IO8_PIN_L));	//设置为低电平
	close(fd);
}

/**	
* led4_open
* 功 能：打开led4灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led4_open()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO4_IO16_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO4_IO16_PIN_H, sizeof(SYSFS_RST_GPIO4_IO16_PIN_H));	//设置为高电平
	close(fd);
}

/**	
* led4_close
* 功 能：关闭led4灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led4_close()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO4_IO16_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO4_IO16_PIN_L, sizeof(SYSFS_RST_GPIO4_IO16_PIN_L));	//设置为低电平
	close(fd);
}

/**	
* led5_open
* 功 能：打开led5灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led5_open()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO5_IO4_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO4_PIN_H, sizeof(SYSFS_RST_GPIO5_IO4_PIN_H));	//设置为高电平
	close(fd);
}

/**	
* led5_close
* 功 能：关闭led5灯
* 开发人员：刘鹏
* 开发日期：2020-06-04
**/
void led5_close()
{
	int fd;
	if(!s_led_init_state)//判断led的引脚是否被初始化过
	{
		led_pin_init();
	}
    fd = open(SYSFS_RST_GPIO5_IO4_PIN_VAL, O_RDWR);	//打开引脚设备
    if(fd == -1)
    {
		return;
    }
	write(fd, SYSFS_RST_GPIO5_IO4_PIN_L, sizeof(SYSFS_RST_GPIO5_IO4_PIN_L));	//设置为低电平
	close(fd);
}

/**	
* led pin init 
* 功 能：初始化led1、led2、led3、led4、led5 引脚
* 开发人员：刘鹏
* 开发日期：2020-06-03
**/
void led_pin_init()
{
	int fd;
	s_led_init_state=1;	//指示LED灯是被初始化过
	
/*	led2	*/
//先卸载SYSFS_BUSY_GPIO5_IO6_PIN
	fd = open(SYSFS_GPIO_UNEXPORT, O_WRONLY);    //不判断返回值
	write(fd, SYSFS_BUSY_GPIO5_IO6_PIN ,sizeof(SYSFS_BUSY_GPIO5_IO6_PIN));   //卸载对应的参数
	close(fd); 

//打开端口/sys/class/gpio# SYSFS_BUSY_GPIO5_IO6_PIN > export
    fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
    if(fd == -1)
    {
		printf("ERR: Radio hard busy pin open error.\n");
    }
    if( write(fd, SYSFS_BUSY_GPIO5_IO6_PIN ,sizeof(SYSFS_BUSY_GPIO5_IO6_PIN))<0)    
    printf("write error!\n"); 
    close(fd); 	 
		 
//设置端口方向/sys/class/gpio/gpio4# SYSFS_BUSY_GPIO5_IO6_PIN > direction
    fd = open(SYSFS_BUSY_GPIO5_IO6_PIN_DIR, O_WRONLY);
    if(fd == -1)
    {
		printf("ERR: Radio hard busy pin direction open error.\n");
    }     
    write(fd, SYSFS_BUSY_GPIO5_IO6_PIN_DIR_VAL, sizeof(SYSFS_BUSY_GPIO5_IO6_PIN_DIR_VAL)); 
    close(fd); 
	
/*	led3	*/
//先卸载SYSFS_RST_GPIO5_IO8_PIN
	fd = open(SYSFS_GPIO_UNEXPORT, O_WRONLY);	 //不判断返回值
	write(fd, SYSFS_RST_GPIO5_IO8_PIN ,sizeof(SYSFS_RST_GPIO5_IO8_PIN));	 //卸载对应的参数
	close(fd); 
	
//打开端口/sys/class/gpio# SYSFS_BUSY_GPIO5_IO6_PIN > export
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin open error.\n");
	}
	if( write(fd, SYSFS_RST_GPIO5_IO8_PIN ,sizeof(SYSFS_RST_GPIO5_IO8_PIN))<0)	
	printf("write error!\n"); 
	close(fd);	 
			 
//设置端口方向/sys/class/gpio/gpio4# SYSFS_BUSY_GPIO5_IO6_PIN > direction
	fd = open(SYSFS_RST_GPIO5_IO8_PIN_DIR, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin direction open error.\n");
	}	  
	write(fd, SYSFS_RST_GPIO5_IO8_PIN_DIR_VAL, sizeof(SYSFS_RST_GPIO5_IO8_PIN_DIR_VAL)); 
	close(fd); 
	
/*	led4	*/
//先卸载SYSFS_RST_GPIO4_IO16_PIN
	fd = open(SYSFS_GPIO_UNEXPORT, O_WRONLY);	 //不判断返回值
	write(fd, SYSFS_RST_GPIO4_IO16_PIN ,sizeof(SYSFS_RST_GPIO4_IO16_PIN));	 //卸载对应的参数
	close(fd); 
		
//打开端口/sys/class/gpio# SYSFS_BUSY_GPIO5_IO6_PIN > export
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin open error.\n");
	}
	if( write(fd, SYSFS_RST_GPIO4_IO16_PIN ,sizeof(SYSFS_RST_GPIO4_IO16_PIN))<0)	
	printf("write error!\n"); 
	close(fd);	 
				 
//设置端口方向/sys/class/gpio/gpio4# SYSFS_BUSY_GPIO5_IO6_PIN > direction
	fd = open(SYSFS_RST_GPIO4_IO16_PIN_DIR, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin direction open error.\n");
	}	  
	write(fd, SYSFS_RST_GPIO4_IO16_PIN_DIR_VAL, sizeof(SYSFS_RST_GPIO4_IO16_PIN_DIR_VAL)); 
	close(fd); 

/*	led5	*/
//先卸载SYSFS_RST_GPIO4_IO16_PIN
	fd = open(SYSFS_GPIO_UNEXPORT, O_WRONLY);	 //不判断返回值
	write(fd, SYSFS_RST_GPIO5_IO4_PIN ,sizeof(SYSFS_RST_GPIO5_IO4_PIN));	 //卸载对应的参数
	close(fd); 
			
//打开端口/sys/class/gpio# SYSFS_BUSY_GPIO5_IO6_PIN > export
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin open error.\n");
	}
	if( write(fd, SYSFS_RST_GPIO5_IO4_PIN ,sizeof(SYSFS_RST_GPIO5_IO4_PIN))<0)	
	printf("write error!\n"); 
	close(fd);	 
					 
//设置端口方向/sys/class/gpio/gpio4# SYSFS_BUSY_GPIO5_IO6_PIN > direction
	fd = open(SYSFS_RST_GPIO5_IO4_PIN_DIR, O_WRONLY);
	if(fd == -1)
	{
		printf("ERR: Radio hard busy pin direction open error.\n");
	}	  
	write(fd, SYSFS_RST_GPIO5_IO4_PIN_DIR_VAL, sizeof(SYSFS_RST_GPIO5_IO4_PIN_DIR_VAL)); 
	close(fd); 
}

