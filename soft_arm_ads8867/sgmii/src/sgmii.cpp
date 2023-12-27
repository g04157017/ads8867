#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include<unistd.h>

#include "CJsonObject.h"
#include "iniext.h"

#include "def.h"
#include "def.h"
#include "config.h"


extern "C" {
#include "ql_sgmii.h"
}

int Set_EtherNet_Enable()
{
	if(ql_sgmii_enable()) 
	{
		printf("\nEnable SGMII Module failure");
		return ERR;
	}
		
	if(ql_sgmii_autoneg_set(QL_SGMII_AUTONEG_ON)) 
	{
		printf("\nset autoneg failure");
		return ERR;
	}

	return OK;
}

int Set_EtherNet_brige()
{
	int ret = ERR;
	char cfgkey[200] = {0};
	pcstr syscfg = NULL;

	std::string msg_ip;
	std::string msg_mask;
	std::string msg_gate;
	char cmd[200] = {0};
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");
	CHK_ERR(ret, ERR);

	sprintf(cfgkey, CFG_SEC_ETHERNET CFG_SEC_ETHERNET_NAME);
	syscfg = pIni->GetString(cfgkey);
	if (NULL != syscfg)
    {
		std::string strmsg(syscfg);
        CJsonObject sysjson(strmsg);
		ret = sysjson.Get(CFG_ETH_IF_IPADDR,msg_ip);
		CHK_ERR(ret, ERR);

		ret = sysjson.Get(CFG_ETH_IF_NETMASK,msg_mask);
		CHK_ERR(ret, ERR);

		ret = sysjson.Get(CFG_ETH_IF_GATE,msg_gate);
		CHK_ERR(ret, ERR);
	}
	pIni->UnLoadIni();
	sprintf(cmd,"ifconfig bridge0 %s netmask %s",msg_ip.c_str(),msg_mask.c_str());
	while (system(cmd) < 0) 
	{
		printf("\nifconfig bridge0 IP fail !");
		return ERR;
	}
	memset(cmd,0,200);
	sprintf(cmd,"route add default gw %s dev bridge0",msg_gate.c_str());
	while (system(cmd) < 0) 
	{
		printf("\nifconfig bridge0 Route fail !");
		return ERR;
	}
	
	return OK;
}

int Set_EtherNet_Eth()
{
	int ret = ERR;
	char cfgkey[200] = {0};
	pcstr syscfg = NULL;
	char cmd[200] = {0};

	std::string msg_ip;
	std::string msg_mask;
	
	IniExt* pIni = IniExt::GetInstance();
	CHK_NULL_PTR(pIni, ERR);
	ret = pIni->LoadIni("config.ini");
	CHK_ERR(ret, ERR);

	sprintf(cfgkey, CFG_SEC_ETHERNET CFG_ETH_NAME);
	syscfg = pIni->GetString(cfgkey);
	if (NULL != syscfg)
    {
		std::string strmsg(syscfg);
        CJsonObject sysjson(strmsg);
		ret = sysjson.Get(CFG_ETH_IF_IPADDR,msg_ip);
		CHK_ERR(ret, ERR);

		ret = sysjson.Get(CFG_ETH_IF_NETMASK,msg_mask);
		CHK_ERR(ret, ERR);
	}
	pIni->UnLoadIni();

	memset(cmd,0,200);
	sprintf(cmd,"ifconfig eth0:1 %s netmask %s",msg_ip.c_str(),msg_mask.c_str());
	while (system(cmd) < 0) 
	{
		printf("\nifconfig eth0:1 IP fail !");
		return ERR;
	}
	
	return OK;
}

int main(int argc, char **argv)
{
	int ret = ERR;
	while(1) {

		if(Set_EtherNet_Enable() !=OK)
			return 0;
		sleep(1);
		if(Set_EtherNet_brige() !=OK)
			return 0;
		sleep(1);
		if(Set_EtherNet_Eth() !=OK)
			return 0;
		
		printf("\nset autoneg Success !\n");
		return 0;
	}
}

