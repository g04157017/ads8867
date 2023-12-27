#include "modbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
extern "C" {
#include "ql_uart.h"
#include "ql_gpio.h"
#include "ql_error.h"
#include <pthread.h>
}


#define device_rtu   "/dev/ttyHS1"
static int fd_uart = -1;

static pthread_t thrd_uart_rcv;
static void* UartRecv_Proc(void* arg);

static Enum_PinName m_RS485_RD = PINNAME_GPIO3; // OPEN EVB J0203 GPIO3
static Enum_PinName m_RS485_232_SL = PINNAME_GPIO4; // OPEN EVB J0203 PINNAME_GPIO4
void print_for_adc_data_dbg(uint16_t *data, int leng)
{
	for (int i = 1; i <= leng; i++)
		{
			printf("%02x  ", data[i-1]);
			if(i % 10 == 0)
			{
				printf("\n");
			}
		}
	printf("\n\n");
}

int gpio_init()
{
 int iRet = 0;
	//配置GPIO3作为RS485_RD信号输出端。
	iRet = Ql_GPIO_Init(m_RS485_RD, PINDIRECTION_OUT, PINLEVEL_LOW, PINPULLSEL_DISABLE);
    printf("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", m_RS485_RD, PINDIRECTION_OUT, PINLEVEL_HIGH, iRet);
	if(iRet == RES_OK){
	printf("init the m_RS485_RD IO success !\n");
	}
	else{
		printf("init the m_RS485_RD return err =  %d\n",iRet);
		return -1;
	}
//配置GPIO4作为RS485_232_SL信号输出端。
	iRet = Ql_GPIO_Init(m_RS485_232_SL, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
    printf("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", m_RS485_232_SL, PINDIRECTION_OUT, PINLEVEL_HIGH, iRet);
	if(iRet == RES_OK){
	printf("init the m_RS485_232_SL IO success !\n");
	}
	else{
		printf("init the m_RS485_232_SL return err =  %d\n",iRet);
		return -1;
	}

	return 0;
}
//1:RS-485 mode ;0:RS-232 mode,set this mode ,please Synchronous set modbus-rtu modbus_rs485_use
int SP330EEY_SET_MODE(int mode)
{
	int ret = 0;
	switch (mode)
		{
			case 0:
				ret = Ql_GPIO_SetLevel(m_RS485_232_SL,PINLEVEL_HIGH);
				if(ret == RES_OK)
				printf("set the m_RS485_RD IO success !\n");
				ret = Ql_GPIO_GetLevel(m_RS485_232_SL);
				printf("m_RS485_RD = %d value is = %d\n",m_RS485_232_SL,ret);
			break;
			
			case 1:
				ret = Ql_GPIO_SetLevel(m_RS485_232_SL,PINLEVEL_LOW);
				if(ret == RES_OK)
				printf("set the m_RS485_RD IO success !\n");
				ret = Ql_GPIO_GetLevel(m_RS485_232_SL);
				printf("m_RS485_RD = %d value is = %d\n",m_RS485_232_SL,ret);
			break;
			
			default:
			printf("该层不存在, 请重新输入\n");
		}

}
int RS485_Write()
{
	int ret = 0;
	char strTmp[] = "uart test, =+-_0)9(8*7&6^5%4$3#2@1!`~)\n";
	
	ret = Ql_GPIO_SetLevel(m_RS485_RD,PINLEVEL_LOW);
	if(ret == RES_OK)
	ret = Ql_GPIO_GetLevel(m_RS485_RD);
	printf("m_RS485_RD = %d value is = %d\n",m_RS485_RD,ret);
	usleep(2100);
	ret = Ql_UART_Write(fd_uart, (const char*)strTmp, sizeof(strTmp));
          printf("< write(fd=%d)=%d\n", fd_uart, ret);
	usleep(2100);
	ret = Ql_GPIO_SetLevel(m_RS485_RD,PINLEVEL_HIGH);
	if(ret == RES_OK)
	ret = Ql_GPIO_GetLevel(m_RS485_RD);
	printf("m_RS485_RD = %d value is = %d\n",m_RS485_RD,ret);

	return 0;
}
/*int main(int argc, char* argv[])
{
        int iRet;
        int baudRate;
		char strTmp[] = "uart test, =+-_0)9(8*7&6^5%4$3#2@1!`~)\n";

/* Start: If need, to modify uart dcb config */

 /*       ST_UARTDCB dcb ;
	  	dcb.baudrate = B_115200;
	  	dcb.databit = DB_CS8;
	  	dcb.flowctrl = FC_NONE;
	  	dcb.parity = PB_NONE;
	  	dcb.stopbit = SB_1;
		gpio_init();
		SP330EEY_SET_MODE(1);
        

        printf("< OpenLinux: UART example >\n");

        fd_uart = Ql_UART_Open(device_rtu, B_115200, FC_NONE);
        printf("< open(\"%s\", %d)=%d\n", device_rtu, baudRate, fd_uart);

        iRet = Ql_UART_SetDCB(fd_uart, &dcb);
        printf("SET DCB ret: %d\n", iRet);
        iRet = Ql_UART_GetDCB(fd_uart, &dcb);
        printf("GET DCB ret: %d: baudrate: %d, flowctrl: %d, databit: %d, stopbit: %d, paritybit: %d\n",
                                iRet, dcb.baudrate, dcb.flowctrl, dcb.databit, dcb.stopbit, dcb.parity);
        /* End: if need, to modify uart dcb config */


        /*
        *  Create a thread to handle uart rx data
        */
/*        if (pthread_create(&thrd_uart_rcv, NULL, UartRecv_Proc, NULL) != 0)
        {
                printf("Fail to create thread!\n");
        }

        while (fd_uart >=0)
        {
//          iRet = Ql_UART_Write(fd_uart, (const char*)strTmp, sizeof(strTmp));
//          printf("< write(fd=%d)=%d\n", fd_uart, iRet);
        RS485_Write();
          sleep(10);
		}
				
		return 0;
}
				
static void* UartRecv_Proc(void* arg)
{
	int iRet;
	fd_set fdset;
	struct timeval timeout = {3, 0};		// timeout 3s
	char buffer[100] = {0};
				
	while (fd_uart >=0)
	{
		FD_ZERO(&fdset);
		FD_SET(fd_uart, &fdset);
		iRet = select(fd_uart + 1, &fdset, NULL, NULL, &timeout);
		if (-1 == iRet)
			{
				printf("< failed to select >\n");
				exit(-1);
			}
		else if (0 == iRet)
			{// no data in Rx buffer
				printf("< no data >\n");
				timeout.tv_sec	= 3;
				timeout.tv_usec = 0;
			}
		else
			{// data is in Rx data
				if (FD_ISSET(fd_uart, &fdset))
					{
						do {
								memset(buffer, 0x0, sizeof(buffer));
								iRet = Ql_UART_Read(fd_uart, buffer, 100);
								
								printf("> read(uart)=%d:%s\n", iRet, buffer);
						}while (100 == iRet);
					}
			}
	}
	return (void *)1;
}
*/
int main(int argc, char *argv[])
{
	int ret = -1;
	modbus_t *mb = NULL;
	uint16_t tab_reg[100] = {0};
	int retry_times = 3;
	gpio_init();
	SP330EEY_SET_MODE(1);
	mb = modbus_new_rtu(device_rtu, 115200, 'N', 8, 1);
	if (mb == NULL) 
    {
        printf("Unable to allocate libmodbus context\n");
        return -1;
    }
	ret = modbus_set_slave(mb, 1);//set slave address

	while ((-1 == modbus_connect(mb)) && ((retry_times--) > 0))
	{
        printf("Connection failed\n");
        sleep(1);
    }
	
	if(retry_times < 0)
		{
			printf("Connection failed begain three times !\n");
			return -1;
		}
	
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = 1000000;//set modbus time 1000ms
	modbus_set_response_timeout(mb, &t);




	while(1)
	{
		printf("now begain to read registers !\n");
		int reg = modbus_read_registers(mb, 0, 10, tab_reg);

		print_for_adc_data_dbg(tab_reg,10);
		sleep(3);
	}
	modbus_close(mb);
	modbus_free(mb);
}
