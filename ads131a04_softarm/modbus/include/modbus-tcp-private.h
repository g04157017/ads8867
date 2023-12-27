/***************************************************************************
 * Copyright (C), suxin, All rights reserved.
 * File        :modbus-tcp-private.h
 * Decription  :
 *
 * Author      :  liuch
 * Version     :  v1.0
 * Date        :  2018/6/22
 * Histroy	   :
 **************************************************************************/

#ifndef _MODBUS_TCP_PRIVATE_H_
#define _MODBUS_TCP_PRIVATE_H_

#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0

typedef struct _modbus_tcp {
    /* TCP port */
    int port;
    /* IP address */
    char ip[16];
} modbus_tcp_t;

#define _MODBUS_TCP_PI_NODE_LENGTH    1025
#define _MODBUS_TCP_PI_SERVICE_LENGTH   32

typedef struct _modbus_tcp_pi {
    /* TCP port */
    int port;
    /* Node */
    char node[_MODBUS_TCP_PI_NODE_LENGTH];
    /* Service */
    char service[_MODBUS_TCP_PI_SERVICE_LENGTH];
} modbus_tcp_pi_t;

#endif /* _MODBUS_TCP_PRIVATE_H_ */
