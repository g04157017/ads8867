/*
 * SPI testing utility (using spidev driver)
 * for example: insmod /usr/lib/modules/3.18.48/kernel/drivers/spi/spidev.ko busnum=6 chipselect=0
 */

#include <stdint.h>
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h> 
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "ql_spi.h"
#include "ql_gpio.h"
#include "ql_error.h"



#define device   "/dev/spidev6.0"
#define MAX_SPI_BUF_SIZE  4000000
//#define USE_32  1
static Enum_PinName m_GpioPin_adcRST = PINNAME_I2C1_SCL; // OPEN EVB J0203 GPIO41

static int fd_spi = -1;
static struct spi_ioc_transfer tr;

#define WORD_LENGTH_BITS		((uint8_t) 16)
//#define WORD_LENGTH_24BIT
#define WORD_LENGTH_16BIT
//#define WORD_LENGTH_32BIT

#define WORD_LENGTH_16BIT_TRUNCATED


// Determine number of bytes per command based on WORD_LENGTH
#define WORD_LENGTH             (WORD_LENGTH_BITS >> 3)

// Fetch the upper byte of a 16-bit word and return it as an 8-bit value
#define UPPER_BYTE(x)           ((uint8_t) ((0xFF00 & x) >> 8))

// Fetch the lower byte of a 16-bit word and return it as an 8-bit value
#define LOWER_BYTE(x)           ((uint8_t)  (0x00FF & x))

// Combine two 8-bit values into a 16-bit word
#define COMBINE_BYTES(x, y)     (((uint16_t) x << 8) | ((uint16_t) y & 0x00FF))	


int Adc_Ky_log(const char* strlog, ...)
{
	printf  ("[Adc_Ky]: ");
	va_list vp;
	va_start(vp, strlog);
	vprintf (strlog, vp);
	va_end  (vp);
	printf  ("\n");
	return 0;
}
void print_for_adc_data_dbg(unsigned char *data, int leng)
{
	int i = 0;
	int p_data = 0;
	for (i = 0; i < leng; i++)
		{
			if(data[i*2]<=127){
				p_data = 100*4*2*((2*2.5*(signExtend(&data[i*2])))/65536);
//				printf("+%02x+%02x  ", data[i*2],data[i*2+1]);
				printf("+%02d  ", p_data);
			}
			else{
				p_data = -(100*4*2*(2*2.5*(65535.0-signExtend(&data[i*2]))/65536));
				printf("%02d  ", p_data);
			}
			if(i==0) continue;
			else{
				if(i % 24 == 0)
				{
					printf("\n");
				}
			}
		}
	printf("\n\n");
}



int Init_ADS131A0()
{
	int receive=0x0000;
	int ret=0;
	int count=0;
#ifdef USE_32
	unsigned char write_buf[4] = {0};
	unsigned char read_buf[4] = {0};
//硬复位
	ret = Ql_GPIO_SetLevel(m_GpioPin_adcRST,PINLEVEL_LOW);
	if(ret == RES_OK)
		Adc_Ky_log("set the RST IO success !");
	ret = Ql_GPIO_GetLevel(m_GpioPin_adcRST);
	Adc_Ky_log("m_GpioPin_adcRST = %d value is = %d",m_GpioPin_adcRST,ret);
	sleep(1);
	ret = Ql_GPIO_SetLevel(m_GpioPin_adcRST,PINLEVEL_HIGH);
	if(ret == RES_OK){
	Adc_Ky_log("set the RST IO PINLEVEL_HIGH success !");
	}
	else{
		Adc_Ky_log("Ql_GPIO_SetLevel %d return err %d= ",PINLEVEL_HIGH,ret);
	}
	ret = Ql_GPIO_GetLevel(m_GpioPin_adcRST);
	Adc_Ky_log("m_GpioPin_adcRST = %d value is = %d",m_GpioPin_adcRST,ret);
	sleep(1);

//set the RESET
	write_buf[2]= 0X11;
	write_buf[3]= 0X00;
	Write_Data(write_buf,4);
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = (read_buf[3])<<8;
		receive |= read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0xff04)&&(count<10));
	if(count>=10) return -1;
//set the UNLOCK
	write_buf[2]= 0X55;
	write_buf[3]= 0X06;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = (read_buf[3])<<8;
		receive |= read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0655)&&(count<10));
	if(count>=10) return -1;
//	A_SYS_CFG  enable EXternal reference volatage (2.5v)
	write_buf[2]= 0X70;
	write_buf[3]= 0X40|0x0B;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x70)&&(count<10));
	if(count>=10) return -1;
//	D_SYS_CFG_ADDRESS  set DOUT Data Frame, Fixed six device words per frame
	write_buf[2]= 0X3C;
	write_buf[3]= 0X40|0x0C;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x3C)&&(count<10));
	if(count>=10) return -1;

//ADC CLK1  fICLK = fCLKIN(16.384mhz) / 2  
	write_buf[2]= 0X02;
	write_buf[3]= 0X40|0x0D;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X02)&&(count<10));
	if(count>=10) return -1;
//fMOD = fICLK / 2  fICLK = fCLKIN / 2048 ****** now is 500hz *****0x21
	write_buf[2]= 0X2D;
	write_buf[3]= 0X40|0x0E;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X2D)&&(count<10));
	if(count>=10) return -1;
//ADC CHANNEL ENABLE ALL
	write_buf[2]= 0X0f;
	write_buf[3]= 0X40|0x0F;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X0f)&&(count<10));
	if(count>=10) return -1;
//WAKEUP ADS131 
	write_buf[2]= 0X33;
	write_buf[3]= 0x00;
	Write_Data(write_buf,4);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,4);
		receive = (read_buf[3])<<8;
		receive |= read_buf[2];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0033)&&(count<10));
	if(count>=10) return -1;
	Adc_Ky_log("Init_ADS131A04  end !");
	read_all_ads131a04_Register();

#else
	unsigned char write_buf[2] = {0};
	unsigned char read_buf[2] = {0};
//硬复位
	ret = Ql_GPIO_SetLevel(m_GpioPin_adcRST,PINLEVEL_LOW);
	if(ret == RES_OK)
		Adc_Ky_log("set the RST IO success !");
	ret = Ql_GPIO_GetLevel(m_GpioPin_adcRST);
	Adc_Ky_log("m_GpioPin_adcRST = %d value is = %d",m_GpioPin_adcRST,ret);
	sleep(1);
	ret = Ql_GPIO_SetLevel(m_GpioPin_adcRST,PINLEVEL_HIGH);
	if(ret == RES_OK){
	Adc_Ky_log("set the RST IO PINLEVEL_HIGH success !");
	}
	else{
		Adc_Ky_log("Ql_GPIO_SetLevel %d return err %d= ",PINLEVEL_HIGH,ret);
	}
	ret = Ql_GPIO_GetLevel(m_GpioPin_adcRST);
	Adc_Ky_log("m_GpioPin_adcRST = %d value is = %d",m_GpioPin_adcRST,ret);
	sleep(1);

//set the RESET
	write_buf[0]= 0X11;
	write_buf[1]= 0X00;
	Write_Data(write_buf,2);
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[1])<<8;
		receive |= read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0xff04)&&(count<10));
	if(count>=10) return -1;
//set the UNLOCK
	write_buf[0]= 0X55;
	write_buf[1]= 0X06;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[1])<<8;
		receive |= read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0655)&&(count<10));
	if(count>=10) return -1;
//	A_SYS_CFG  enable EXternal reference volatage (2.5v)
	write_buf[0]= 0X70;
	write_buf[1]= 0X40|0x0B;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x70)&&(count<10));
	if(count>=10) return -1;
//	D_SYS_CFG_ADDRESS  set DOUT Data Frame, Fixed six device words per frame
	write_buf[0]= 0X3C;
	write_buf[1]= 0X40|0x0C;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x3C)&&(count<10));
	if(count>=10) return -1;

//ADC CLK1  fICLK = fCLKIN(16.384mhz) / 2  
	write_buf[0]= 0X02;
	write_buf[1]= 0X40|0x0D;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[0];
		Adc_Ky_log("spi read data was %x %x",receive,read_buf[0]);
		sleep(1);
		count++;
	}while((receive !=0X02)&&(count<10));
	if(count>=10) return -1;
//fMOD = fICLK / 2  fICLK = fCLKIN / 2048 ****** now is 500hz *****0x21
	write_buf[0]= 0X2D;
	write_buf[1]= 0X40|0x0E;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X2D)&&(count<10));
	if(count>=10) return -1;
//ADC CHANNEL ENABLE ALL
	write_buf[0]= 0X0f;
	write_buf[1]= 0X40|0x0F;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X0f)&&(count<10));
	if(count>=10) return -1;
//WAKEUP ADS131 
	write_buf[0]= 0X33;
	write_buf[1]= 0x00;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[1])<<8;
		receive |= read_buf[0];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0033)&&(count<10));
	if(count>=10) return -1;
	Adc_Ky_log("Init_ADS131A04  end !");
	read_all_ads131a04_Register();
#endif
	return 0;
}

int Get_ADS131A0Value(unsigned char * buff,int Fd_spi)
{
	struct timeval tpstart,tpend;     
	float timeuse;     
	unsigned char* write_buf;
	write_buf = (unsigned char *)calloc(10,sizeof(unsigned char));	
	memset(write_buf,0,100);

	write_buf[0] = 0x00;
	write_buf[1] = 0x00;
#ifdef WORD_LENGTH_24BIT
    write_buf[5*WORD_LENGTH] = 0x4E;
    write_buf[5*WORD_LENGTH + 1] = 0xC3;

#elif defined WORD_LENGTH_16BIT
     write_buf[5*WORD_LENGTH + 1] = 0x1D;
     write_buf[5*WORD_LENGTH] = 0x0F;

#elif defined WORD_LENGTH_32BIT
       write_buf[5*WORD_LENGTH] = 0xF6;
       write_buf[5*WORD_LENGTH + 1] = 0xB8;

#endif
// Send NULL word, receive response word
	memset(buff,0,1000);
	int i =0;
	gettimeofday(&tpstart,NULL); // 开始时间
	for(i=0;i<2;i++){
//		while(!Ql_GPIO_GetLevel(m_GpioPin));
		Write_Data_P(write_buf,&buff[6*WORD_LENGTH*i],6*WORD_LENGTH*10);
	}
	gettimeofday(&tpend,NULL);   // 结束时间  // 计算执行时间，以微秒为单位进行计算
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;       
	timeuse/=1000000;
	Adc_Ky_log("used time:%f sec\n",timeuse);
	
#ifdef CRC_EN
    uint16_t crcWordOut = calculateCRC(&dataRx[0], 6*WORD_LENGTH, 0xFFFF);
#else
    // (OPTIONAL) Ignore CRC error checking (CRC = 0x00 -> indicates a valid SPI frame)
    uint16_t crcWordOut = 0x00;
#endif	
	return 0;
}

int read_all_ads131a04_Register()
{
	int receive=0x0000;
#ifdef USE_32
	unsigned char write_buf[4] = {0};
	unsigned char read_buf[4] = {0};
	write_buf[2]= 0X00;
	write_buf[3]= 0X20|0x0B;
	Write_Data(write_buf,4);
	Read_Data(read_buf,4);
	receive = (read_buf[2]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0B value was %x",receive);

	write_buf[2]= 0X00;
	write_buf[3]= 0X20|0x0C;
	Write_Data(write_buf,4);
	Read_Data(read_buf,4);
	receive = (read_buf[2]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0C value was %x",receive);

	write_buf[2]= 0X00;
	write_buf[3]= 0X20|0x0D;
	Write_Data(write_buf,4);
	Read_Data(read_buf,4);
	receive = (read_buf[2]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0D value was %x",receive);
	
	write_buf[2]= 0X00;
	write_buf[3]= 0X20|0x0E;
	Write_Data(write_buf,4);
	Read_Data(read_buf,4);
	receive = (read_buf[3]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0E value was %x",receive);

	write_buf[2]= 0X00;
	write_buf[3]= 0X20|0x0f;
	Write_Data(write_buf,4);
	Read_Data(read_buf,4);
	receive = (read_buf[2]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0f value was %x",receive);

#else
	unsigned char write_buf[2] = {0};
	unsigned char read_buf[2] = {0};
	write_buf[0]= 0X00;
	write_buf[1]= 0X20|0x0B;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[0]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0B value was %x",receive);

	write_buf[0]= 0X00;
	write_buf[1]= 0X20|0x0C;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[0]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0C value was %x",receive);

	write_buf[0]= 0X00;
	write_buf[1]= 0X20|0x0D;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[0]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0D value was %x",receive);
	
	write_buf[0]= 0X00;
	write_buf[1]= 0X20|0x0E;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[2]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0E value was %x",receive);

	write_buf[0]= 0X00;
	write_buf[1]= 0X20|0x0f;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[0]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0f value was %x",receive);
#endif
	return 0;
}
//*****************************************************************************
//
//! Internal function used by readData() to convert ADC data from multiple unsigned
//! bytes into a single signed 32-bit word.
//!
//! \fn int32_t signExtend(const uint8_t dataBytes[])
//!
//! \param dataBytes is a pointer to uint8_t[] where the first element is the MSB.
//!
//! \return Returns the signed-extend 32-bit result.
//
//*****************************************************************************
int signExtend(const uint8_t dataBytes[])
{
#ifdef WORD_LENGTH_24BIT

    int upperByte   = ((int) dataBytes[0] << 24);
    int middleByte  = ((int) dataBytes[1] << 16);
    int lowerByte   = ((int) dataBytes[2] << 8);

    // NOTE: This right-shift operation on signed data maintains the signed bit,
    // and provides for the sign-extension from 24 to 32 bits.
    return (((int) (upperByte | middleByte | lowerByte)) >> 8);

#elif defined WORD_LENGTH_32BIT_SIGN_EXTEND

    int signByte    = ((int) dataBytes[0] << 24);
    int upperByte   = ((int) dataBytes[1] << 16);
    int middleByte  = ((int) dataBytes[2] << 8);
    int lowerByte   = ((int) dataBytes[3] << 0);

    return (signByte | upperByte | middleByte | lowerByte);

#elif defined WORD_LENGTH_32BIT_ZERO_PADDED

    int upperByte   = ((int) dataBytes[0] << 24);
    int middleByte  = ((int) dataBytes[1] << 16);
    int lowerByte   = ((int) dataBytes[2] << 8);

    // NOTE: This right-shift operation on signed data maintains the signed bit,
    // and provides for the sign-extension from 24 to 32 bits.
    return (((int) (upperByte | middleByte | lowerByte)) >> 8);     // Right-shift of signed data maintains signed bit

#elif defined WORD_LENGTH_16BIT_TRUNCATED

    int upperByte   = ((int) dataBytes[0] << 8);
    int lowerByte   = ((int) dataBytes[1]);

    // NOTE: This right-shift operation on signed data maintains the signed bit,
    // and provides for the sign-extension from 16 to 32 bits.
    return (((int) (upperByte | lowerByte)));

#endif
}



/*********************************************************************
* @fn	   insmod_spi_module
*
* @brief   加载SPI模块
*
* @param   bits_per_word[in]: 位数
*          speed_hz[in]:      SPI CLOCK速率
*
* @return  -1: 失败 0: 成功
*
*/
int insmod_spi_module(unsigned char bits_per_word, unsigned int speed_hz)
{
    char cmd[512];
    struct timeval to;

    fd_spi = -1;
	
	tr.tx_buf = (unsigned long)NULL;
	tr.rx_buf = (unsigned long)NULL;
	tr.delay_usecs = 0;
	tr.cs_change = 1;
    tr.speed_hz = speed_hz;
    tr.bits_per_word = bits_per_word;

	while (system("insmod /lib/modules/3.18.20/kernel/drivers/spi/spi_qsd.ko") < 0) {
			Adc_Ky_log("cmd: insmod /lib/modules/3.18.20/kernel/drivers/spi/spi_qsd.ko error: %s", strerror(errno));
			to.tv_sec = 1;
			to.tv_usec = 0;
			select(0,NULL,NULL,NULL,&to);
		}
	Adc_Ky_log("insmod /lib/modules/3.18.20/kernel/drivers/spi/spidev.ko");
    //DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "spi: speed_hz: %d, bits_per_word: %d, len: %d\n",tr.speed_hz,tr.bits_per_word);

	memset(cmd,0,sizeof(cmd));
//	snprintf(cmd,sizeof(cmd),"insmod /lib/modules/3.18.20/kernel/drivers/spi/quec_spi_chn.ko speed_hz=19200000 gpiomodemready=34 gpiomcuready=52 busnum=6 chipselect=0 spimode=1 maxspeed=%u bufsiz=%d",tr.speed_hz,MAX_SPI_BUF_SIZE);
//	snprintf(cmd,sizeof(cmd),"insmod /lib/modules/3.18.20/kernel/drivers/spi/quec_spi_chn.ko speed_hz=19200000 gpiomodemready=75 gpiomcuready=10");
    snprintf(cmd,sizeof(cmd),"insmod /lib/modules/3.18.20/kernel/drivers/spi/spidev.ko busnum=6 chipselect=0 bufsiz=%d",MAX_SPI_BUF_SIZE);

   // DBG_PRINT_AND_LOG(SX_DEBUG_EMERG, SX_LOG_EMERG, "cmd: %s\n",cmd);
	printf("cmd: %s\n",cmd);
	while (system(cmd) < 0) {
        //DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "cmd: %s\t error: %s", cmd, strerror(errno));
        printf("cmd: %s\t error: %s", cmd, strerror(errno));
        to.tv_sec = 1;
	    to.tv_usec = 0;
	    select(0,NULL,NULL,NULL,&to);
	}

	to.tv_sec = 2;
	to.tv_usec = 0;
	select(0,NULL,NULL,NULL,&to);
}

/*********************************************************************
* @fn	   open_interface_spi
*
* @brief   打开SPI接口
*
* @param   buf[in]: spi缓冲区
*          len[in]: spi缓冲区长度
*
* @return  -1: 失败 0: 成功
*
*/
int open_interface_spi(unsigned char *buf, unsigned int len)
{
	if (fd_spi != -1) {
     //   DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "spi interface has been opened! %d\n", fd_spi);
     printf("spi interface has been opened! %d\n",fd_spi);
		return -1;
	}

	tr.rx_buf = (unsigned long)(buf);
	tr.len = len;

	fd_spi = Ql_SPI_Init(device,  SPIMODE1,  tr.bits_per_word, tr.speed_hz);
	//fd_spi = open(device, O_RDWR | O_NONBLOCK);
	if (-1 == fd_spi) {
     //   DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Ql_SPI_Init fail!\n");
     printf("Ql_SPI_Init fail!\n");
		return -1;
	}

	return 0;
}
int open_interface_spi_next(unsigned char *buf, unsigned int len)
{
	if (fd_spi != -1) {
     //   DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "spi interface has been opened! %d\n", fd_spi);
     printf("spi interface has been opened! %d\n",fd_spi);
		return -1;
	}

	tr.rx_buf = (unsigned long)(buf);
	tr.len = len;

	fd_spi = Ql_SPI_Init(device,  SPIMODE1,  32, tr.speed_hz);
	//fd_spi = open(device, O_RDWR | O_NONBLOCK);
	if (-1 == fd_spi) {
     //   DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "Ql_SPI_Init fail!\n");
     printf("Ql_SPI_Init fail!\n");
		return -1;
	}

	return 0;
}


/*********************************************************************
* @fn	   close_interface_spi
*
* @brief   关闭SPI接口
*
* @param   void
*
* @return  -1: 失败 0: 成功
*
*/
int close_interface_spi(void)
{
	if (fd_spi != -1) {
		Ql_SPI_DeInit(fd_spi);
	//	close(fd_spi);
		fd_spi = -1;
	}
}

/*********************************************************************
* @fn	   collect_spi_data
*
* @brief   采集SPI接口数据
*
* @param   void
*
* @return  -1: 失败 0: 成功
*
*/
int collect_spi_data(void)
{
    int ret;
    ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
      //  DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "can't send spi message. ioctl error: %s!\n", strerror(errno));
      printf("can't send spi message. ioctl error: %s!\n", strerror(errno));
		return -1;
	}
    return 0;
}




int Write_Data(unsigned char* p_write_buf,int len)
{
	int ret;
	unsigned char p_read_buf[len];
	ret = Ql_SPI_Write_Read(fd_spi,p_write_buf,p_read_buf,len);
	//ret = write(Fd_Spi,p_write_buf,len);
		if(ret<0){
		printf("spi Read_Data err !");
		return -1;
	}
	return 0;
}
int Write_Data_P(unsigned char* p_write_buf,unsigned char* p_read_buf,int len)
{
	int ret;
	ret = Ql_SPI_Write_Read(fd_spi,p_write_buf,p_read_buf,len);
	//ret = write(Fd_Spi,p_write_buf,len);
		if(ret<0){
		Adc_Ky_log("spi Read_Data err !");
		return -1;
	}
	return 0;
}


int Read_Data(unsigned char* p_read_buf,int len)
{
	int ret;
	ret = read(fd_spi, p_read_buf,len);
	if(ret<0){
		printf("spi Read_Data err !");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	int iRet = 0;
	int receive=0x0000;
	uint8_t  writebuf[1024]={};
	uint8_t  readbuf[1024] ={};
	unsigned char *spi_buf;

//配置GPIO65作为RST信号输出端。
//	iRet = Ql_GPIO_Init(PINNAME_I2C1_SCL, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
//	printf("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", PINNAME_I2C1_SCL, PINDIRECTION_OUT, PINLEVEL_HIGH, iRet);
//	if(iRet == RES_OK){
//	printf("init the RST IO success !");
//	}
//	else{
//		printf("init the rst return err =  %d",iRet);
//	}
//硬复位
//	iRet = Ql_GPIO_SetLevel(PINNAME_I2C1_SCL,PINLEVEL_LOW);
//	if(iRet == RES_OK)
//		printf("set the RST IO success !");
//	iRet = Ql_GPIO_GetLevel(PINNAME_I2C1_SCL);
//	printf("m_GpioPin_adcRST = %d value is = %d",PINNAME_I2C1_SCL,iRet);
//	sleep(1);
//	iRet = Ql_GPIO_SetLevel(PINNAME_I2C1_SCL,PINLEVEL_HIGH);
//	if(iRet == RES_OK){
//	printf("set the RST IO PINLEVEL_HIGH success !");
//	}
//	else{
//		printf("Ql_GPIO_SetLevel %d return err %d= ",PINLEVEL_HIGH,iRet);
//	}
//	iRet = Ql_GPIO_GetLevel(PINNAME_I2C1_SCL);
//	printf("m_GpioPin_adcRST = %d value is = %d",PINNAME_I2C1_SCL,iRet);
//	sleep(1);

    /* 加载SPI模块, 根据配置来决定位数和采样率 */
    /* 读取位数和采样率 */
    insmod_spi_module(16, 195000);

	spi_buf = (unsigned char *)calloc(1000,sizeof(unsigned char));	

	/* 打开SPI接口 */
	close_interface_spi();
	int ret = open_interface_spi(spi_buf, 1000);
	if (ret){
			printf("open interface spi fail!\n");
	}
	 
//	Init_ADS131A0();
//	Get_ADS131A0Value(spi_buf,fd_spi);
//	print_for_adc_data_dbg(spi_buf,12*2);
//	close_interface_spi();
//	open_interface_spi_next(spi_buf, 1000);
	ret = collect_spi_data();
		 if (ret < 0){
			  //  DBG_PRINT_AND_LOG(SX_DEBUG_ERROR, SX_LOG_ERROR, "collect spi data failed!\n");
			 return -1;
		  }
	print_for_adc_data_dbg(spi_buf,500);

//	if(count>=10) return -1;
	
	puts("");
	
	return  0 ;
}
