#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include<unistd.h>


extern "C" {
#include "ql_sgmii.h"
}

int main(int argc, char **argv)
{
	while(1) {
		if(ql_sgmii_enable()) {
			printf("\nEnable SGMII Module failure");
			return 0;
		}
		printf("\nEnable SGMII Module Success");
		
		if(ql_sgmii_autoneg_set(QL_SGMII_AUTONEG_ON)) {
			printf("\nset autoneg failure");
			return 0;
		}
		/* set the bridge0 gw and mask*/		
		while (system("ifconfig bridge0 192.168.88.1 netmask 255.255.255.0") < 0) {
		printf("\nifconfig bridge0 fail !");
		}
		sleep(1);
		
		while (system("ifconfig eth0 down") < 0) {
		printf("\nifconfig eth0 down fail !");
		}
		sleep(1);

		
		while (system("ifconfig eth0:1 192.168.88.8") < 0) {
		printf("\nifconfig eth0 down fail !");
		}
		sleep(1);

		while (system("ifconfig eth0 down") < 0) {
		printf("\nifconfig eth0 down fail !");
		}
		sleep(1);
		printf("\nset autoneg Success");
		return 0;
	}
}

