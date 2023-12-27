#ifndef JSCB_DEF_H
#define JSCB_DEF_H

#ifdef LINUX
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <linux/spi/spidev.h>
#include <linux/if_packet.h>
#include <linux/rtnetlink.h>     
#include <dirent.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
#else
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "Iphlpapi")
#endif

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>
#include <time.h>
#include <string>
#include <cstdio>
#include <ctime>
#include <cmath>

#include <openssl/evp.h>
#include <openssl/x509.h>
//#include <openssl/x509err.h>
//#include <openssl/x509v3.h>
//#include <openssl/x509v3err.h>
#include <openssl/x509_vfy.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

typedef unsigned int  uint;
typedef unsigned char byte;
typedef const char*   pcstr;
typedef unsigned long ulong;

#ifdef LINUX
typedef int socket_t;
#else
typedef SOCKET socket_t;
typedef int socklen_t;
#endif

typedef int thread_t;
typedef int eproll_t;



#ifndef NULL  
#define NULL (0)
#endif

#ifndef NULL_PTR  
#define NULL_PTR nullptr
#endif

#define INVALID_VALUE     (0xffffffff)
#define INVALID_HANDLE    (-1)
#define INVALID_SOCKET_FD (-1)
#define INVALID_THREAD_FD (-1)
#define INVALID_EPROLL_FD (-1)
#define INVALID_POLICY_FD (-1)
#define INVALID_TUNNEL_FD (-1)

#define OK       (0)
#define ERR      (-1)
#define NOK      (~0)

#define MAX_CTRL_MSG_LEN  4096
#define MAX_DATA_MSG_LEN  4096
#define MAX_TH_STACK_LEN  8*1024

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif 

#define CHK_EQU(var, val) if((var) != (val))      {  return ERR; }
#define CHK_VAL(var, val) if((var) > (val))       {  return ERR; }
#define CHK_ERR(ret, err) if((ret) == ERR)        {  return err; }
#define CHK_TRUE(res, err) if((res) != true)      {  return err; }
#define CHK_NULL(x, err) if((x) == NULL)          {  return err; }
#define CHK_NULL_PTR(x, err) if((x) == NULL_PTR)  {  return err; }
#define CHK_CONTINUE(var, val) if((var) == (val)) {  continue;   }

#endif
