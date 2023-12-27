#ifndef _SYSM_LOG_H
#define _SYSM_LOG_H
#include "comm.h"
#include "config.h"
#include "utils.h"

#define  SYSM_LOG_FILE           "/usr/local/log/sysm_log.log"        /* 错误日志默认文件名 */
#define  SYSM_AUT_FILE			 "/usr/local/cfg/AuthorizationCode/AuthorizationCode.txt"
#define  P_SYSM_AUT_FILE		 "/usr/local/cfg/AuthorizationCode/P_AuthorizationCode.txt"

#define  SYS_LOG_BUFF_LEN        512
#define  SYS_LOG_FILE_LEN        1024*1024*10

#define  APP    "APP"
#define  ADS131 "ADS131"
#define  AutCFG	"AutCFG"
#define  CFG    "CFG"
#define  SYSLAH	"SYSLAH"
#define  ROUTE  "ROUTE"
#define  ARP    "ARP"
#define  NAT    "NAT"
#define  MAC    "MAC"
#define  POLICY "POLICY"
#define  TUNNEL "TUNNEL"
#define  SECURE "SECURE"
#define  NET    "NET"
#define  SYSC   "SYSC"
#define  SYSM   "SYSM"
#define  FWD    "FWD"
#define  SDKS   "SDKS"
#define  SDKC   "SDKC"
#define  SOCK   "SOCK"
#define  USER   "USER"
#define  AGENT  "AGENT"
#define  UTILS  "UTILS"
#define  FWDC   "FWDC"
#define  BACKUP "BACKUP"
#define  RESET  "RESET"
#define  EXPORT "FILE"

typedef enum E_Log_Level
{
    LOG_DEBUG = 0,
    LOG_ERROR = 1,
    LOG_FAULT = 2,
    LOG_NOLOG = 3,
}LOG_LEVEL_E;

class Log
{
public: 
    std::string modulename;     //模块名称
    std::string level;          //日志级别
    std::string datetime;       //日志时间
    std::string desc;           //日志描述
};

class SysMLogMng
{
public:
    static SysMLogMng* GetInstance();
    int Init();
    int LogSetLevel(std::string logLevel);
    int LogCHK(ProcStat_T* stat, char* msg, SMsgHead_T* head);
    int LogUpdate(Log* pLogUpdate);
    int LogQuery(Log* pLogQuery);
    int LogDawnFile();
    inline int GetLogLevel() { return m_nLogLevel; }
private:
    uint m_nLogLevel;
    uint m_nLogSize;
    uint m_nLogDays;
    SysMLogMng();
    ~SysMLogMng();
    static SysMLogMng* m_pSysMLogMng;
};

/***** WARNING 一条日志长度不能超过512字符 WARNING *****/
int APP_LOG(pcstr module, uint loglevel, pcstr strlog, ...);
int SYSM_LOG_ERROR(pcstr module, pcstr strlog, ...);
int SYSM_LOG_DEBUG(pcstr module, pcstr strlog, ...);
int SYSM_LOG_FAULT(pcstr module, pcstr strlog, ...);

#endif
