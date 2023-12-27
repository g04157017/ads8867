#include "adc_ky.h"

static Enum_PinName m_A_SEL = PINNAME_GPIO2; // OPEN EVB J0203 GPIO2
static Enum_PinName m_A_SEL12 = PINNAME_SD_INT_DET; // OPEN EVB J0203 GPIO23
static Enum_PinName m_A_SEL34 = PINNAME_I2C1_SCL; // OPEN EVB J0203 GPIO41


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
int Adc_Ky::insmod_spi_module(unsigned char bits_per_word, int  speed_hz)
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
	while (system("insmod /lib/modules/3.18.20/kernel/drivers/spi/spi_qsd.ko") < 0) 
	{
        Adc_Ky_log("cmd: insmod /lib/modules/3.18.20/kernel/drivers/spi/spi_qsd.ko error: %s", strerror(errno));
        to.tv_sec = 1;
	    to.tv_usec = 0;
	    select(0,NULL,NULL,NULL,&to);
	}
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
int Adc_Ky::open_spi(unsigned char *buf, unsigned int len,SPI_SPEED  speed)
{
	if (Fd_Spi != -1) {
        Adc_Ky_log("spi interface has been opened! %d\n", Fd_Spi);
		return -1;
	}

	tr.rx_buf = (unsigned long)(buf);
	tr.len = len;
	Adc_Ky_log("spi: speed_hz: %d, bits_per_word: %d, len: %d\n",tr.speed_hz,tr.bits_per_word);
	Fd_Spi = Ql_SPI_Init((char*)SPI_DEVICE_FILE,  SPIMODE1,  tr.bits_per_word, speed);
	if (-1 == Fd_Spi) {
        Adc_Ky_log("Ql_SPI_Init fail!\n");
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
*********************************************************************/
int Adc_Ky::close_spi(void)
{
	if (Fd_Spi != -1) {
		Ql_SPI_DeInit(Fd_Spi);
		Fd_Spi = -1;
	}
	return OK;
}
int Adc_Ky::Init_CTL_IO(void)
{
	int iRet = 0;

	iRet = Ql_GPIO_Init(m_A_SEL, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	CHK_ERR(iRet, ERR);

	iRet = Ql_GPIO_Init(m_A_SEL12, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	CHK_ERR(iRet, ERR);

	iRet = Ql_GPIO_Init(m_A_SEL34, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	CHK_ERR(iRet, ERR);
	
	Adc_Ky_log("Init the IO ctl set success !");

	return OK;

}
SampFreq Adc_Ky::char2enum(const char* str)
{
	 Adc_Ky_log("char2enum:const char* str is  %s",str);
	 if(strcmp(str, "SW_1K") == 0){

		return SW_1K;
	 }
	 if(strcmp(str, "SW_5K") == 0){

		return SW_5K;
	 }
	 if(strcmp(str, "SW_10K") == 0){
		return SW_10K;
	 }
	  if(strcmp(str, "SW_20K") == 0){
		return SW_20K;
	 }
}
/*
	Author	:	PengLiu
	Time	:	2022.11.9
	Sampling Rate was 10.67kSPS	SPI word was fix two words
*/
int Adc_Ky::Init_spi(void)
{
	int ret = 0;
	int Fre;
	SampFreq PSampFreq = SW_10K;
	SPI_SPEED Pspeed = S_195K;

    int samp_time = 0;
	int samp_freq = 0;
	int samp_word_leng = 0;
	
	char cfgkey[200] = {0};
	std::string sampStr = "";
	std::string cfgval = "";
	
	insmod_spi_module(16,390000);


	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	
	ret = pIni->LoadIni("config.ini");
	CHK_ERR(ret, ERR);

	ret = pIni->GetString(CFG_SAMP CFG_SAMP_NAME, sampStr);
	CHK_ERR(ret, ERR);
	pIni->UnLoadIni();

	CJsonObject resetJson(sampStr);

	ret = resetJson.Get(SAMP_TIME,       cfgval);
	CHK_TRUE(ret, ERR);
	sscanf(cfgval.c_str(),"%d",&samp_time);
	if(samp_time>5000)		samp_time = 5000; //max samp_time is 5s = 5000ms

	ret = resetJson.Get(SAMP_FREQ,       cfgval);
	CHK_TRUE(ret, ERR);
	PSampFreq = char2enum(cfgval.c_str());

	switch (PSampFreq)
	{
		case SW_1K:
			goto result;
		break;
		
		case SW_5K:
		goto result;
		
		break;
		
		case SW_10K:
			Pspeed = S_195K;
			samp_freq = SW_10K;
			goto result;
		break;

		case SW_20K:
			Pspeed = S_390K;
			samp_freq = SW_20K;
			goto result;
		break;
		
		default:
			Adc_Ky_log("now came in default !");
		break;
	}
	
result:
//	insmod_spi_module(16,Pspeed);

/* 获取加速度数据采样时长(ms)               读取采样率(KHZ)*/
    samp_word_leng = (float)samp_time/1000 * samp_freq*1000;
    spi_buf_len = 2*samp_word_leng +2;

	Adc_Ky_log("samp_word_leng was = %d, spi_buf_len is = %d",samp_word_leng,spi_buf_len);

    spi_buf = (unsigned char *)malloc(spi_buf_len);	
    if (NULL == spi_buf) {
        Adc_Ky_log("malloc Init_spi fail! %s\n", strerror(errno));
		free(spi_buf);
		spi_buf = NULL;
		return ERR;
    }

    ret = close_spi();
	CHK_ERR(ret, ERR);
	
    ret = open_spi(spi_buf, spi_buf_len,Pspeed);
    if (ret) {
        Adc_Ky_log("open interface spi fail!\n");
		free(spi_buf);
		spi_buf = NULL;
		return ERR;
    }
	return OK;
}
Adc_Ky::Adc_Ky() : 
    m_bStart(false)    
{
	Fd_Spi = -1;
	spi_buf = NULL;
	spi_buf_len = 0;
}
Adc_Ky::~Adc_Ky()
{
    DeInit();
}
int Adc_Ky::DeInit()
{
    m_bStart = false;
	free(spi_buf);
	spi_buf = NULL;
	spi_buf_len = 0;
	close_spi();
    usleep(CFG_SYS_WAIT_USECOND);
    return OK;
}
int Adc_Ky::Init()
{
	int iRet;
	iRet = Init_spi();
	if(iRet == OK){
		Adc_Ky_log("Init_spi() success !");
	}
	else 
		Adc_Ky_log("Init_spi() fail !");
	CHK_ERR(iRet, ERR);

	iRet = Init_CTL_IO();
	if(iRet == OK){
		Adc_Ky_log("Init_CTL_IO() success !");
	}
	else
		Adc_Ky_log("Init_CTL_IO() fail !");
	CHK_ERR(iRet, ERR);

	pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, MAX_TH_STACK_LEN);

    m_bStart = true;
    iRet = pthread_create(&m_Thread, &thread_attr, Adc_Ky::AdcProc, this);
    if(iRet < 0)
    {
        m_bStart = false;
        Adc_Ky_log("MosquittoPub thread create fail.");
        return ERR;
    }

    iRet = pthread_attr_destroy(&thread_attr); 
    if(iRet != 0)
    {
        return ERR;
    }

    Adc_Ky_log("Adc_Ky create server theread success.");
	
    return OK;
} 
void* Adc_Ky::AdcProc(void* data)
{
	int ret = ERR;
	int nrow=0;
	char **azResult;
	Adc_Ky* pThis = (Adc_Ky*)data;
	while(pThis->m_bStart)
	{
		sleep(10);
    }
	
	pThis->m_Thread = INVALID_THREAD_FD;
    pThis->DeInit();
    return NULL_PTR;
}


int Adc_Ky::Get_ADS8867Value(unsigned char SWitchSelect)
{
    int ret;
	switch (SWitchSelect)
		{
			case 0:
				ret = Ql_GPIO_SetLevel(m_A_SEL,PINLEVEL_LOW);
				
				CHK_EQU(ret, RES_OK);

				ret = Ql_GPIO_SetLevel(m_A_SEL12,PINLEVEL_LOW);
				
				CHK_EQU(ret, RES_OK);
			break;

			case 1:
				ret = Ql_GPIO_SetLevel(m_A_SEL,PINLEVEL_LOW);
				CHK_EQU(ret, RES_OK);

				ret = Ql_GPIO_SetLevel(m_A_SEL12,PINLEVEL_HIGH);
				CHK_EQU(ret, RES_OK);
			break;

			case 2:
				ret = Ql_GPIO_SetLevel(m_A_SEL,PINLEVEL_HIGH);
				CHK_EQU(ret, RES_OK);

				ret = Ql_GPIO_SetLevel(m_A_SEL34,PINLEVEL_LOW);
				CHK_EQU(ret, RES_OK);
			break;

			case 3:
				ret = Ql_GPIO_SetLevel(m_A_SEL,PINLEVEL_HIGH);
				CHK_EQU(ret, RES_OK);

				ret = Ql_GPIO_SetLevel(m_A_SEL34,PINLEVEL_HIGH);
				CHK_EQU(ret, RES_OK);
			break;

			default:
						
			break;
		}
	sleep(1);
    ret = ioctl(Fd_Spi, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
        Adc_Ky_log("can't read spi message. ioctl error: %s!\n", strerror(errno));
		return ERR;
	}
	ret = Ql_GPIO_SetLevel(m_A_SEL,PINLEVEL_LOW);
	
	CHK_EQU(ret, RES_OK);

	ret = Ql_GPIO_SetLevel(m_A_SEL12,PINLEVEL_LOW);
	
	CHK_EQU(ret, RES_OK);
	
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

    int upperByte   = ((int) dataBytes[1] << 8);
    int lowerByte   = ((int) dataBytes[0]);

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
	CHK_ERR(ret, ERR);
	
	Adc_Ky_log("Ads131a04_init ended.");
	return OK;
}

