#ifndef _UTILS_H
#define _UTILS_H

#include "def.h"
#include "CJsonObject.h"
#include "mutex.h"
#include "list.h"
#include "hash.h"
#include "queue.h"
#include "shm_queue.h"
#include "pkt_queue.h"
#include "ringbuf.h"


void utils_dbg(const char* strlog, ...);
int net_aton(const char *cp, double *nip);
uint get_netmask(const char* mask);
char *get_netmask(uint masklen);
int Readfile(const char* src, char* des,ulong &enclen);
int copyfile(const char* src, const char* des);
int writefile(const char* filename, const char* data, size_t nLen);
int updatefile(const char* filename, const char* data, size_t nLen);
ulong getfilesize(pcstr filename);
int getmd5str(byte* data, uint datalen, std::string& strmd5);
int getsha256str(byte* data, uint datalen, std::string& strsha256);
uint get_timespan();

void debug_natime();
void debug_data(byte* data, uint len);
char* get_stime(void);
char* get_filename(pcstr prefix);

bool IsValidIPString(const char * pcIPAddress);
void IPDeleteZero(char *pcIPAddress);
void IPchar2Int(const char *pcIPAddress, long long & ll);
bool IPAddressIsValid(const char *pcIPAddress, long long& llIp);
bool IsIPmaskValid(const char *pcIPMask, long long& llMask);
int IpAndMaskIsValid(const char * pcIPAddress, const char * pcIPMask);
bool InTheSameLAN(const char * pcIPAddr1, const char * pcIPMask1,
                   const char * pcIPAddr2, const char * pcIPMask2);
bool IsInvalidPort(const char* pcPort);

#define CHK_IPADDR(ip) if (false == IsValidIPString(ip)) { return ERR; }
#define CHK_PORT(port) if (false == IsInvalidPort(port)) { return ERR; }

#ifdef DEBUG_FLAG
#define utils_log(format, ...) utils_dbg(format, ##__VA_ARGS__)
#define print_natime()         debug_natime()
#define print_data(data, len)  debug_data(data, len)
#else
#define utils_log(format, ...)
#define print_natime()
#define print_data(data, len)
#endif

#endif

