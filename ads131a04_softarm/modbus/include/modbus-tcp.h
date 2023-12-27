/***************************************************************************
 * Copyright (C), suxin, All rights reserved.
 * File        :modbus-tcp.h
 * Decription  :
 *
 * Author      :  liuch
 * Version     :  v1.0
 * Date        :  2018/6/22
 * Histroy	   :
 **************************************************************************/


#ifndef _MODBUS_TCP_H_
#define _MODBUS_TCP_H_

#include "modbus.h"

MODBUS_BEGIN_DECLS

#if defined(_WIN32) && !defined(__CYGWIN__)
/* Win32 with MinGW, supplement to <errno.h> */
#include <winsock2.h>
#define ECONNRESET   WSAECONNRESET
#define ECONNREFUSED WSAECONNREFUSED
#define ETIMEDOUT    WSAETIMEDOUT
#define ENOPROTOOPT  WSAENOPROTOOPT
#endif

#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_TCP_SLAVE         0xFF

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MODBUS_TCP_MAX_ADU_LENGTH  260

modbus_t* modbus_new_tcp(const char *ip_address, int port);
int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
int modbus_tcp_accept(modbus_t *ctx, int *socket);

modbus_t* modbus_new_tcp_pi(const char *node, const char *service);
int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection);
int modbus_tcp_pi_accept(modbus_t *ctx, int *socket);

MODBUS_END_DECLS

#endif /* _MODBUS_TCP_H_ */
