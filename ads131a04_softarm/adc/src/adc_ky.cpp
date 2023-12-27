#include "adc_ky.h"

static Enum_PinName m_GpioPin = PINNAME_GPIO2; // OPEN EVB J0203 GPIO2
static Enum_PinName m_GpioPin_adcRST = PINNAME_I2C1_SCL; // OPEN EVB J0203 GPIO41

int Adc_Ky_dbg(const char* strlog, ...)
{
	printf  ("[Adc_Ky]: ");
	va_list vp;
	va_start(vp, strlog);
	vprintf (strlog, vp);
	va_end  (vp);
	printf  ("\n");
	return OK;
}
void print_for_adc_data_dbg(unsigned char *data, int leng)
{
	for (int i = 1; i <= leng; i++)
		{
			printf("%02x  ", data[i-1]);
			if(i % 12 == 0)
			{
				printf("\n");
			}
		}
	printf("\n\n");
}

Adc_Ky* Adc_Ky::m_pAdc_Ky = NULL_PTR;
Adc_Ky* Adc_Ky::GetInstance()
{
    if (NULL_PTR == m_pAdc_Ky)
    {
        m_pAdc_Ky = new Adc_Ky();
    }
    
    return (m_pAdc_Ky == NULL_PTR) ? NULL_PTR : m_pAdc_Ky;
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
int Adc_Ky::insmod_spi_module(unsigned char bits_per_word, unsigned int speed_hz)
{
    char cmd[512];
    struct timeval to;

    Fd_Spi = -1;
	
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

	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"insmod /lib/modules/3.18.20/kernel/drivers/spi/spidev.ko busnum=6 chipselect=0 bufsiz=%d",MAX_SPI_BUF_SIZE);

    Adc_Ky_log("cmd: %s\n",cmd);

	while (system(cmd) < 0) {
        Adc_Ky_log("cmd: %s\t error: %s", cmd, strerror(errno));
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
*********************************************************************/
int Adc_Ky::open_interface_spi()
{
	if (Fd_Spi != -1) {
        Adc_Ky_log("spi interface has been opened! %d\n", Fd_Spi);
		return -1;
	}

	Adc_Ky_log("spi: speed_hz: %d, bits_per_word: %d, len: %d\n",tr.speed_hz,tr.bits_per_word);
	Fd_Spi = Ql_SPI_Init((char*)SPI_DEVICE_FILE,  SPIMODE1,  tr.bits_per_word, S_4_8M);
	if (-1 == Fd_Spi) {
        Adc_Ky_log("Ql_SPI_Init fail!\n");
		return ERR;
	}

	return OK;
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
*********************************************************************/
int Adc_Ky::close_interface_spi(void)
{
	if (Fd_Spi != -1) {
		Ql_SPI_DeInit(Fd_Spi);
		Fd_Spi = -1;
	}
	return OK;
}
int Adc_Ky::Init_interface_spi(void)
{
	int ret = 0;
    /* 打开SPI接口 */
    close_interface_spi();
    ret = open_interface_spi();
    if (ret) 
	{
        Adc_Ky_log("open interface spi fail!\n");
        return ERR;
    }
	return OK;
}
Adc_Ky::Adc_Ky() : 
    m_bStart(false)    
{
	Fd_Spi = -1;
	spi_buf_len = 0;
	memset(write_buf,0,100);
	memset(read_buf,0,100);
}
Adc_Ky::~Adc_Ky()
{
    DeInit();
}
int Adc_Ky::DeInit()
{
    m_bStart = false;
	spi_buf_len = 0;
	close_interface_spi();
    usleep(CFG_SYS_WAIT_USECOND);
    return OK;
} 
int Adc_Ky::Init()
{
	int iRet;
/* 加载SPI模块, 根据配置来决定位数和采样率 */
/* 读取位数和采样率 */
	Adc_Ky_log("begain to init spi module !");
	
    insmod_spi_module(8, 4800000);

	Adc_Ky_log("begain to init spi interface !");
	Init_interface_spi();

//配置GPIO2作为Dready信号输入端。
	iRet = Ql_GPIO_Init(m_GpioPin, PINDIRECTION_IN, PINLEVEL_HIGH, PINPULLSEL_PULLUP);
    Adc_Ky_log("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", m_GpioPin, PINDIRECTION_IN, PINLEVEL_HIGH, iRet);
//配置GPIO65作为RST信号输出端。
	iRet = Ql_GPIO_Init(m_GpioPin_adcRST, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
    Adc_Ky_log("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", m_GpioPin_adcRST, PINDIRECTION_OUT, PINLEVEL_HIGH, iRet);
	if(iRet == RES_OK){
	Adc_Ky_log("init the RST IO success !");
	}
	else{
		Adc_Ky_log("init the rst return err =  %d",iRet);
	}
    return OK;
} 
/*
返回值：value->读到的ADC0数值，单位mv。
*/
int Adc_Ky::Get_Ec20ADC0Value()
{
	int value;
	value = ql_adc_show(ADC0);
	return value*2;
}
/*
返回值：value->读到的ADC1数值，单位mv。
*/
int Adc_Ky::Get_Ec20ADC1Value()
{
	int value;
	value = ql_adc_show(ADC1);
	return value*2;
}
int Adc_Ky::Read_Data(unsigned char* p_read_buf,int len)
{
	int ret;
	ret = read(Fd_Spi, p_read_buf,len);
	if(ret<0){
		Adc_Ky_log("spi Read_Data err !");
		return ERR;
	}
	return OK;
}
int Adc_Ky::Write_Data(unsigned char* p_write_buf,unsigned char* p_read_buf,int len)
{
	int ret;
	ret = Ql_SPI_Write_Read(Fd_Spi,p_write_buf,p_read_buf,len);
		if(ret<0){
		Adc_Ky_log("spi Read_Data err !");
		return ERR;
	}
	return OK;
}
int Adc_Ky::Write_Data(unsigned char* p_write_buf,int len)
{
	int ret;
	unsigned char p_read_buf[len];
	ret = Ql_SPI_Write_Read(Fd_Spi,p_write_buf,p_read_buf,len);
		if(ret<0){
		Adc_Ky_log("spi Read_Data err !");
		return ERR;
	}
	return OK;
}

int Adc_Ky::Init_ADS131A0()
{
	int receive=0x0000;
	int ret=0;
	int count=0;
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
	write_buf[1]= 0X11;
	write_buf[0]= 0X00;
	Write_Data(write_buf,2);
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[0])<<8;
		receive |= read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0xff04)&&(count<10));
	if(count>=10) return ERR;
//set the UNLOCK
	write_buf[1]= 0X55;
	write_buf[0]= 0X06;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[0])<<8;
		receive |= read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0655)&&(count<10));
	if(count>=10) return ERR;
//	A_SYS_CFG  enable EXternal reference volatage (2.5v)
	write_buf[1]= 0X70;
	write_buf[0]= 0X40|0x0B;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x70)&&(count<10));
	if(count>=10) return ERR;
//	D_SYS_CFG_ADDRESS  set DOUT Data Frame, Fixed six device words per frame
	write_buf[1]= 0X3E;
	write_buf[0]= 0X40|0x0C;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x3E)&&(count<10));
	if(count>=10) return ERR;
//ADC CLK1  fICLK = fCLKIN(16.384mhz) / 2  
	write_buf[1]= 0X02;
	write_buf[0]= 0X40|0x0D;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[1];
		Adc_Ky_log("spi read data was %x %x",receive,read_buf[1]);
		sleep(1);
		count++;
	}while((receive !=0X02)&&(count<10));
	if(count>=10) return ERR;
//fMOD = fICLK / 2  fICLK = fCLKIN / 2048 ****** now is 500hz *****0x21
	write_buf[1]= 0X2D;
	write_buf[0]= 0X40|0x0E;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X2D)&&(count<10));
	if(count>=10) return ERR;
//ADC CHANNEL ENABLE ALL
	write_buf[1]= 0X0f;
	write_buf[0]= 0X40|0x0F;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0X0f)&&(count<10));
	if(count>=10) return ERR;
//WAKEUP ADS131 
	write_buf[1]= 0X33;
	write_buf[0]= 0x00;
	Write_Data(write_buf,2);
	receive = 0;
	count = 0;
	do{
		Read_Data(read_buf,2);
		receive = (read_buf[0])<<8;
		receive |= read_buf[1];
		Adc_Ky_log("spi read data was %x",receive);
		sleep(1);
		count++;
	}while((receive !=0x0033)&&(count<10));
	if(count>=10) return ERR;
	Adc_Ky_log("Init_ADS131A04  end !");
	read_all_ads131a04_Register();
	return OK;
}

int Adc_Ky::Get_ADS131A0Value(unsigned char * buff)
{
	struct timeval tpstart,tpend;     
	float timeuse;     
	
	memset(write_buf,0,100);

	write_buf[0] = 0x00;
	write_buf[1] = 0x00;
#ifdef WORD_LENGTH_24BIT
    write_buf[5*WORD_LENGTH] = 0x4E;
    write_buf[5*WORD_LENGTH + 1] = 0xC3;

#elif defined WORD_LENGTH_16BIT
     write_buf[5*WORD_LENGTH] = 0x1D;
     write_buf[5*WORD_LENGTH + 1] = 0x0F;

#elif defined WORD_LENGTH_32BIT
       write_buf[5*WORD_LENGTH] = 0xF6;
       write_buf[5*WORD_LENGTH + 1] = 0xB8;

#endif
// Send NULL word, receive response word
	int i =0;
	gettimeofday(&tpstart,NULL); // 开始时间
	for(i=0;i<1000;i++)
	{
		Write_Data(write_buf,&buff[6*WORD_LENGTH*i],6*WORD_LENGTH);
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
	return OK;
}

int Adc_Ky::read_all_ads131a04_Register()
{
	int receive=0x0000;
	write_buf[1]= 0X00;
	write_buf[0]= 0X20|0x0B;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[1]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0B value was %x",receive);

	write_buf[1]= 0X00;
	write_buf[0]= 0X20|0x0C;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[1]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0C value was %x",receive);

	write_buf[1]= 0X00;
	write_buf[0]= 0X20|0x0D;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[1]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0D value was %x",receive);
	
	write_buf[1]= 0X00;
	write_buf[0]= 0X20|0x0E;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[1]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0E value was %x",receive);

	write_buf[1]= 0X00;
	write_buf[0]= 0X20|0x0f;
	Write_Data(write_buf,2);
	Read_Data(read_buf,2);
	receive = (read_buf[1]);
	Adc_Ky_log("read_all_ads131a04_Register 0x0f value was %x",receive);
	return OK;
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
int Adc_Ky::signExtend(const uint8_t dataBytes[])
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
int Ads131a04_init()
{
	int ret = ERR;
	int i = 0;
	Adc_Ky_log("Ads131a04_init called.");
	Adc_Ky * pAdc_Ky = Adc_Ky::GetInstance();
	CHK_NULL_PTR(pAdc_Ky, ERR);

	ret = pAdc_Ky->Init();
	ret = pAdc_Ky->Init_ADS131A0();
	if(ret == ERR)
	{
		for(i=0;i<3;i++)
		{
			ret = pAdc_Ky->Init_ADS131A0();
			if(ret != ERR)
			break;
		}
		if(i>=3)
		{
			SYSM_LOG_ERROR(ADS131,"ADC init three times ERR !");
			Adc_Ky_log("ADC init three times ERR !");
			return ERR;
		}
	}
	Adc_Ky_log("Ads131a04_init ended.");
	return OK;
}

