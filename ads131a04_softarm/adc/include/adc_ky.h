#ifndef __adc_ky_h
#define __adc_ky_h

#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/time.h> 
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "def.h"
#include "config.h"
#include "sysm_log.h"

extern "C" {
#include "ql_adc.h"
#include "ql_spi.h"
#include "ql_gpio.h"
#include "ql_error.h"
}

/* 每次SPI采集的第一个数据可能不准确，需要丢弃第一个数据 */
#define IGNORE_SPI_WORD 1 

#define ACC_BYTES_PER_SPIWORD 2

#define ACC_SAMP_FREQ 30000 

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



/* 采样加速度数据是可以设置的 */
 
#define ACC_SAMPLE_NUM_SN 4096                                  //传感节点
#define SAMPLE_BYTES_SN (ACC_SAMPLE_NUM_SN*BYTES_PER_SPIWORD)   

#define MAX_ACC_SAMP_TIME 25000                                 //加速度数据最大采样时长（毫秒）
#define MOTION_DATA_TIME 500                                    //运动数据最小时长（毫秒）
#define ACC_VALUES_NUM_RN (ACC_SAMP_FREQ*MOTION_DATA_TIME/1000) //加速度数据个数
#define ACC_VALUES_LEN_RN (ACC_VALUES_NUM_RN*BYTES_PER_SPIWORD) //加速度数据长度


#define SPI_DEVICE_FILE   "/dev/spidev6.0"
#define MAX_SPI_BUF_SIZE  4000000


#ifdef DEBUG_FLAG
#define Adc_Ky_log(format, ...) Adc_Ky_dbg(format, ##__VA_ARGS__)
#else
#define Adc_Ky_log(format, ...)
#endif

#ifdef DEBUG_FOR_FLAG
#define print_for_adc_data_log(data, leng)  print_for_adc_data_dbg(data, leng)
#else
#define print_for_adc_data_log(data, leng)
#endif

typedef struct
{
    int channel1;
    int channel2;
    int channel3;
    int channel4;
    unsigned short response;
    unsigned short crc;
} adc_data_struct;

class Adc_Ky
{
public:
    static Adc_Ky* GetInstance()  ;
    int Init();
    int DeInit(); 
	int Get_Ec20ADC0Value();
	int Get_Ec20ADC1Value();
	
	
	int Init_ADS131A0();
	int Get_ADS131A0Value(unsigned char * buff);
	int read_all_ads131a04_Register();

/*spi 接口调用初始化*/
	int insmod_spi_module(unsigned char bits_per_word, unsigned int speed_hz);
	int close_interface_spi(void);
	int Init_interface_spi(void);
	int open_interface_spi();
	int Write_Data(unsigned char* p_write_buf,unsigned char* p_read_buf,int len);
	int Write_Data(unsigned char* p_write_buf,int len);
	int Read_Data(unsigned char* p_read_buf,int len);
	int signExtend(const uint8_t dataBytes[]);
private:
    Adc_Ky();
    virtual ~Adc_Ky();

private:
    static Adc_Ky*     m_pAdc_Ky;
    bool               m_bStart;
	int 				Fd_Spi;
	struct spi_ioc_transfer tr;
	unsigned int spi_buf_len;
	unsigned char			write_buf[100];
	adc_data_struct dummy_data;
public:
	unsigned char			read_buf[100];
};

int Ads131a04_init();

#endif
