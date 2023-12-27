/***************************************************************************
 * Copyright (C), suxin, All rights reserved.
 * File        :modbus-rtu.h
 * Decription  :
 *
 * Author      :  liuch
 * Version     :  v1.0
 * Date        :  2018/6/22
 * Histroy	   :
 **************************************************************************/


#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_

#include "modbus.h"

MODBUS_BEGIN_DECLS

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#define MODBUS_RTU_MAX_ADU_LENGTH  256

modbus_t* modbus_new_rtu(const char *device, int baud, char parity,
                         int data_bit, int stop_bit);

#define MODBUS_RTU_RS232 0
#define MODBUS_RTU_RS485 1

int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode);
int modbus_rtu_get_serial_mode(modbus_t *ctx);

MODBUS_END_DECLS

#endif /* _MODBUS_RTU_H_ */
