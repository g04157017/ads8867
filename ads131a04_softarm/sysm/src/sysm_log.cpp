#include <stdio.h>
#include "def.h"
#include "sysm_log.h"

#define LOG_DEBUG_LEVEL  "debug"
#define LOG_ERROR_LEVEL  "error"
#define LOG_FAULT_LEVEL  "fault"
#define LOG_NOLOG_LEVEL  "nolog"

/* log
{
    "opcode":31,"sequence":"2020052012345678","action":0,"recordnum":1,"record":
    [{
         "modulename":"app",                         //模块名称
         "datetime":"2020-08-25 00:00:00.0000",      //日志时间
         "level":"error",                            //日志级别
         "logdesc":"something error."                //日志描述
    }]
}
*/
SysMLogMng* SysMLogMng::m_pSysMLogMng = NULL_PTR;

SysMLogMng* SysMLogMng::GetInstance()   
{
    if (NULL_PTR == m_pSysMLogMng)
    {
        m_pSysMLogMng = new SysMLogMng();
    }
    
    return (m_pSysMLogMng == NULL_PTR) ? NULL_PTR : m_pSysMLogMng;
}

SysMLogMng::SysMLogMng()
{
    m_nLogLevel = LOG_ERROR;
    m_nLogSize  = CFG_LOG_SIZE_DEF;
    m_nLogDays  = CFG_LOG_DAYS_DEF;
}

SysMLogMng::~SysMLogMng()
{
}

int SysMLogMng::Init()
{
    int  ret = ERR;
    bool res = false;
    
    std::string     loglevel = "error";
    uint            logsize   = 0;
    uint            logdays   = 1;

    LogSetLevel(loglevel);
    m_nLogSize   =   logsize;
    m_nLogDays   =   logdays;
    
    return OK;
}

int SysMLogMng::LogSetLevel(std::string logLevel)
{
    if (logLevel == LOG_DEBUG_LEVEL)
    {
        m_nLogLevel = LOG_DEBUG;
    }
    else if (logLevel == LOG_ERROR_LEVEL)
    {
        m_nLogLevel = LOG_ERROR;
    }
    else if (logLevel == LOG_FAULT_LEVEL)
    {
        m_nLogLevel = LOG_FAULT;
    }
    else
    {
        m_nLogLevel = LOG_NOLOG;
    }    
    
    return OK;
}
    
int SysMLogMng::LogCHK(ProcStat_T* stat, char* msg, SMsgHead_T* head)
{
    return OK;
}

int SysMLogMng::LogUpdate(Log* pLogUpdate)
{
    Log* pLog = new Log();
    CHK_NULL_PTR(pLog, ERR);
    
    pLog->modulename = pLogUpdate->modulename;
    pLog->datetime   = pLogUpdate->datetime;
    pLog->level      = pLogUpdate->level;
    pLog->desc       = pLogUpdate->desc;
    
    static char log[SYS_LOG_BUFF_LEN];
    sprintf(log,"[%s:%s] %s: %s\n", pLogUpdate->modulename.c_str(), pLogUpdate->datetime.c_str(), 
                                 pLogUpdate->level.c_str(), pLogUpdate->desc.c_str());
    writefile(SYSM_LOG_FILE, log, strlen(log));
     
    return OK;
}

int SysMLogMng::LogQuery(Log* pLogQuery)
{
    return OK;
}


int SysMLogMng::LogDawnFile()
{
    return OK;
}

#ifdef DEBUG
int APP_LOG(pcstr module, uint loglevel, pcstr logformat, ...) {    return OK;  }
#else 
int APP_LOG(pcstr module, uint loglevel, pcstr logformat, ...)
{
    SysMLogMng* pSysMLog = SysMLogMng::GetInstance();
    CHK_NULL_PTR(pSysMLog, ERR);
    
    static char  desc[SYS_LOG_BUFF_LEN];
    static pcstr level[LOG_NOLOG] = {LOG_DEBUG_LEVEL, LOG_ERROR_LEVEL, LOG_FAULT_LEVEL};
    
	va_list vp;
	va_start(vp, logformat);
	vsprintf (desc, logformat, vp);
	va_end(vp);
    
    Log log;
    log.datetime   = get_stime(); 
    log.modulename = module; 
    log.desc       = desc;
    log.level      = level[loglevel];
    
    return pSysMLog->LogUpdate(&log);
}
#endif

#ifdef DEBUG
int SYSM_LOG_DEBUG(pcstr module, uint loglevel, pcstr logformat, ...) {    return OK;  }
#else
int SYSM_LOG_DEBUG(pcstr module, pcstr logformat, ...)
{
    SysMLogMng* pSysMLog = SysMLogMng::GetInstance();
    CHK_NULL_PTR(pSysMLog, ERR);
    
    
    static char desc[SYS_LOG_BUFF_LEN];
	va_list vp;
	va_start(vp, logformat);
	vsprintf (desc, logformat, vp);
	va_end(vp);
    
    Log log;
    log.datetime   = std::string(get_stime()); 
    log.modulename = std::string(module); 
    log.desc       = std::string(desc);
    log.level      = LOG_DEBUG_LEVEL;
    
    return pSysMLog->LogUpdate(&log);
}
#endif

#ifdef DEBUG
int SYSM_LOG_ERROR(pcstr module, uint loglevel, pcstr logformat, ...) {    return OK;  }
#else
int SYSM_LOG_ERROR(pcstr module, pcstr logformat, ...)
{
    SysMLogMng* pSysMLog = SysMLogMng::GetInstance();
    CHK_NULL_PTR(pSysMLog, ERR);
    
    static char desc[SYS_LOG_BUFF_LEN];
	va_list vp;
	va_start(vp, logformat);
	vsprintf (desc, logformat, vp);
	va_end(vp);
    
    Log log;
    log.datetime   = std::string(get_stime()); 
    log.modulename = std::string(module); 
    log.desc       = std::string(desc);
    log.level      = LOG_ERROR_LEVEL;
    
    return pSysMLog->LogUpdate(&log);
}
#endif

#ifdef DEBUG
int SYSM_LOG_FAULT(pcstr module, uint loglevel, pcstr logformat, ...) {    return OK;  }
#else
int SYSM_LOG_FAULT(pcstr module, pcstr logformat, ...)
{
    SysMLogMng* pSysMLog = SysMLogMng::GetInstance();
    CHK_NULL_PTR(pSysMLog, ERR);
    
    static char desc[SYS_LOG_BUFF_LEN];
	va_list vp;
	va_start(vp, logformat);
	vsprintf (desc, logformat, vp);
	va_end(vp);
    
    Log log;
    log.datetime   = std::string(get_stime()); 
    log.modulename = std::string(module); 
    log.desc       = std::string(desc);
    log.level      = LOG_FAULT_LEVEL;
    
    return pSysMLog->LogUpdate(&log);
}
#endif




