#include "lkt.h"
#include "sm2.h"



/*******************************************
*	用来做SM2国密算法的应用开发
*时间：2020.08.17
*作者：刘鹏
********************************************/
/*
*说明：打印调式
*/
void sm2_cpu_dbg(const char* strlog, ...)
{
	printf	("[sm2_cpu dbg]: ");
		va_list vp;
		va_start(vp, strlog);
		vprintf (strlog, vp);
		va_end	(vp);
		printf	("\n");
		return;
}
/*
*说明：sm2 加密函数调用
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
int sm2_cpu_encryption( unsigned char *encData, unsigned long &encLen, unsigned char *data,unsigned long dataleng,unsigned char *public_key )
{
	u8 ret = 0;
	unsigned long temp;
	u8 buf[1024]={0};



	unsigned char buff[64] = {0};unsigned long buffLen = 64;
	char * pubkey_B_XY = "3226E57052AC93E0F50E05AFFA4757CAAFE37C197328FDB0F3125777E3878277E2886908C8497D1270EA99F4A7144992C50B423BCCBD28F1B800CACD84ED6125";
	
	ret = hexCharStr2unsignedCharStr(pubkey_B_XY, strlen(pubkey_B_XY), 0, buff, &buffLen);
	CHECK_RET(ret);
	print_for_data_log((unsigned char *)pubkey_B_XY,buffLen*2);
	print_for_data_log(buff,buffLen);
	
	sm2_cpu_dbg("----------------------****************************-------------------------");
	ret = GM_SM2Encrypt(buf, &temp, data,dataleng, public_key, (unsigned long)64);
	print_for_data_log(buf,temp);
	memcpy(encData,buf,temp);
	print_for_data_log(encData,temp);
	encLen=temp;
	sm2_cpu_dbg("************temp= %d****************",temp);

END:
	if(ret != 0)
		{
			sm2_cpu_dbg("sm2_cpu_encryption fail !\n");
			return -1;
		}
	else
	sm2_cpu_dbg("sm2_cpu_encryption success !\n");
	return 0;
}

/*
*说明：sm2 解密函数调用
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
int sm2_cpu_decryption( unsigned char *decData, unsigned long  &decLen, unsigned char *data, int  dataleng )
{
	u8 ret = 0;
	unsigned long temp;
	u8 buf[1024]={0};


	unsigned char prikeyBuff[200] = {0};unsigned long priLen = 200;
	char * prikey = "CCE813F8CC0CB16E3EFA7D9CC87BDF9A5AD0782FC5802BA2F60E8E2A66D1F0A8";
	ret = hexCharStr2unsignedCharStr(prikey, strlen(prikey),0, prikeyBuff, &priLen);
	CHECK_RET(ret);


	ret = GM_SM2Decrypt(buf, &temp, data, dataleng, _public_private_certificate._private_key, 0x20);

	print_for_data_log(buf,temp);
	memcpy(decData,buf,temp);
	print_for_data_log(decData,temp);
	decLen=temp;
	sm2_cpu_dbg("************temp= %d****************",temp);

END:	
	if(ret != 0)
		{
			sm2_cpu_dbg("sm2_cpu_decryption fail !\n");
			return -1;
		}
	else
		sm2_cpu_dbg("sm2_cpu_decryption success !\n");
	return 0;
}

/*
*说明：sm2 签名函数调用
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
int sm2_cpu_signature( unsigned char *sigData, unsigned long  &sigcLen, unsigned char *data, int  dataleng )
{

	char *uid = "ALICE123@YAHOO.COM";
	int lenUid = strlen(uid);	
	u8 ret = 0;
	unsigned char sig[1024] = {0};
	unsigned long ulSigLen = 0;

	
	unsigned char prikeyBuff[200] = {0};unsigned long priLen = 200;
    char*    prikey      = "CCE813F8CC0C0E8E2A66DA2F6B16E3EFA7D9CC87BDF1F0A89A5AD0782FC5802B";
	ret = hexCharStr2unsignedCharStr(prikey, strlen(prikey),0, prikeyBuff, &priLen);
	CHECK_RET(ret);

	
	ret = GM_SM2Sign(sig, &ulSigLen, data, dataleng, (unsigned char *)uid, lenUid, _public_private_certificate._private_key, 0x20);
	print_for_data_log(sig,ulSigLen);
	memcpy(sigData,sig,ulSigLen);
	print_for_data_log(sigData,ulSigLen);
	sigcLen=ulSigLen;
	sm2_cpu_dbg("************sigcLen= %d****************",sigcLen);
	if(ret != 0)
	{
		sm2_cpu_dbg("signature failed !\n");
		return -1;
	}
	else
		sm2_cpu_dbg("signature success !\n");
END:
	return 0;
}

/*
*说明：sm2 验签函数调用
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
int sm2_cpu_verify( unsigned char *verfyData, unsigned long vecLen, unsigned char *data, unsigned long dataleng, unsigned char *public_key )
{
	u8 ret = 0;
	char *uid = "ALICE123@YAHOO.COM";
	int lenUid = strlen(uid);	



	unsigned char buff[64] = {0};unsigned long buffLen = 64;
	char *pubkey_A_XY = "3226E57052AC93E0F50E05AFFA4757CAAFE37C194ED6125FDB0F3125777E3878277E2886908C8497D1270EA97328992C50B423BCCBD28F1B800CACD89F4A7145";
	ret = hexCharStr2unsignedCharStr(pubkey_A_XY, strlen(pubkey_A_XY), 0, buff, &buffLen);
	CHECK_RET(ret);



	ret = GM_SM2VerifySig( data, dataleng, verfyData, vecLen, (unsigned char *)uid, lenUid, public_key, 64);
	if(ret != 0)
	{
		sm2_cpu_dbg("verify failed !\n");
		return -1;
	}
	else
	sm2_cpu_dbg("verify success !\n");
END:
	return 0;
}
void cpu_get_random(  )
{
	long ret=0;
	srand( (unsigned)time( NULL ) );
	ret = rand();
	_get_random.random_digit[0]=ret%10;
	_get_random.random_digit[1]=ret/10%10;
	_get_random.random_digit[2]=ret/10/10%10;
	_get_random.random_digit[3]=ret/10/10/10%10;
	_get_random.random_digit[4]=ret/10/10/10/10%10;
	_get_random.random_digit[5]=ret/10/10/10/10/10%10;
	_get_random.random_digit[6]=ret/10/10/10/10/10/10%10;
	_get_random.random_digit[7]=ret/10/10/10/10/10/10/10%10;
	sm2_cpu_dbg("****************ret= %ld**************",ret);
	print_for_data_log(_get_random.random_digit,8);
}
/*
*说明：sm2 设备端发起双向身份认证
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
u8 cpu_buff[0xFF];
int terminal_cpu_authentication(int &g_terminal,unsigned char *data,int &datalen)
{
	u8 singn[200];
	unsigned long sign_leng=0;
	if(g_terminal>2)
		g_terminal=0;
	switch(g_terminal)
		{
		case 0:
			lkt_init();
			sm2_cpu_dbg("da yin gong yao !");

			
			print_for_data_log(_public_private_certificate._public_key,_public_private_certificate._date_leng[0]);
			memcpy(cpu_buff,_public_private_certificate._public_key,_public_private_certificate._date_leng[0]);//保存一份证书公钥
			print_for_data_log(cpu_buff,0x40);
			sm2_cpu_dbg("da yin si yao !");
			print_for_data_log(_public_private_certificate._private_key,_public_private_certificate._date_leng[1]);

			
			sm2_cpu_dbg("***********************terminal case 0******************************\n");

			sm2_cpu_dbg("*****************datalen = %d******************\n",datalen);
			print_for_data_log(data,datalen);	
			
			cpu_get_random();//终端获取随机数
			
			memcpy(data,_get_random.random_digit,0x08);//copy随机数到buff里面
			datalen=0x08;
			
			g_terminal++;
			sm2_cpu_dbg("************************g_terminal= %d*****************************\n",g_terminal);
			sm2_cpu_dbg("********************************data value **********************************\n");
			print_for_data_log(data,datalen);
			return g_terminal;
		case 1:

			sm2_cpu_dbg("***********************terminal case 1*****************************\n");
			sm2_cpu_dbg("*****************datalen = %d******************\n",datalen);
			print_for_data_log(data,datalen);

			//获取终端证书的公钥值
			memcpy(_public_private_certificate._public_key,&data[datalen-(_public_private_certificate._date_leng[5])],0x40);//copy 对方的公钥文件到本设备的公钥文件中
			_public_private_certificate._date_leng[0]=0x40;
			sm2_cpu_dbg("************************pritf cert public key !********************\n");
			print_for_data_log(_public_private_certificate._public_key,_public_private_certificate._date_leng[0]);

			if(sm2_cpu_verify(_get_random.random_digit,0x08,&data[8],datalen-0x08-0x40,_public_private_certificate._public_key)<0)
				return -1;

			if(sm2_cpu_signature(singn,sign_leng,data,0x08)<0)
				return -1;//签名失败时，返回-1
			
			
			memcpy(data,singn,sign_leng);//copy Sign2到data中
			memcpy(&data[sign_leng],cpu_buff,0x40);//copy签名结果和公钥值到s_buff中
			
			datalen=sign_leng+0x40;
			
			g_terminal++;
			sm2_cpu_dbg("************************g_terminal= %d*****************************\n",g_terminal);
			sm2_cpu_dbg("********************************data value **********************************\n");
			print_for_data_log(data,datalen);
			return g_terminal;
		default :
			;
		}
}
/*
*说明：sdk端接受来自设备端的双向身份认证
*返回值：0->成功，-1->失败
*时间：2020.08.17
*作者：刘鹏
*单位：江苏创博信息科技有限公司
*/
int sdk_cpu_authentication(int &g_sdk,unsigned char *data,int &datalen)
{
	u8 singn1[200];
	unsigned long sign_leng1=0;
	if(g_sdk>2)
		g_sdk=0;
	switch (g_sdk)
		{
		case 0:
			lkt_init();
			sm2_cpu_dbg("da yin gong yao !");
			print_for_data_log(_public_private_certificate._public_key,_public_private_certificate._date_leng[0]);
			sm2_cpu_dbg("da yin si yao !");
			print_for_data_log(_public_private_certificate._private_key,_public_private_certificate._date_leng[1]);
			sm2_cpu_dbg("********************************sdk case 0**********************************\n");
			sm2_cpu_dbg("*****************datalen = %d******************\n",datalen);
			print_for_data_log(data,datalen);
			
			cpu_get_random();//SDK获取随机数

			if(sm2_cpu_signature(singn1,sign_leng1,data,datalen)<0)
				return -1;//签名失败时，返回-1

			
			memcpy(data,_get_random.random_digit,0x08);//copy R2到data中
			memcpy(&data[8],singn1,sign_leng1);//copy Sign1到data中
			memcpy(&data[8+_public_private_certificate._date_leng[5]],_public_private_certificate._public_key,0x40);//copy 公钥到data中
				
			datalen=sign_leng1+0x08+0x40;
				
			g_sdk++;
			sm2_cpu_dbg("************************g_sdk= %d*****************************\n",g_sdk);

			sm2_cpu_dbg("********************************data value **********************************\n");
			print_for_data_log(data,datalen);
			return g_sdk;
		case 1:
			sm2_cpu_dbg("********************************sdk case 1**********************************\n");
			sm2_cpu_dbg("*****************datalen = %d******************\n",datalen);
			print_for_data_log(data,datalen);

			//获取终端证书的公钥值
			memcpy(_public_private_certificate._public_key,&data[(datalen-_public_private_certificate._date_leng[5])],0x40);//copy 对方的公钥文件到本设备的公钥文件中
			sm2_cpu_dbg("************************pritf cert public key !********************\n");
			_public_private_certificate._date_leng[0]=0x40;
			print_for_data_log(_public_private_certificate._public_key,_public_private_certificate._date_leng[0]);

			if(sm2_cpu_verify(_get_random.random_digit,0x08,data,datalen-0x40,_public_private_certificate._public_key)<0)
			return -1;
				
			g_sdk++;
			sm2_cpu_dbg("************************g_sdk= %d*****************************\n",g_sdk);
			return g_sdk;
		default :
			;
		}
}
