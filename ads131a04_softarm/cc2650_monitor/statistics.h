#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <errno.h>
#include "log.h"
#include "debug.h"
#include "cc2650_mainuart_protocol.h"
#include "util.h"


int store_cc2650_reboot_ec20_cnt(unsigned short reboot_cnt);

int store_cc2650_boot_cnt(unsigned short reboot_cnt);


#endif
