#ifndef JSCB_MUTEX
#define JSCB_MUTEX

class Mutex
{
public:
    Mutex()
    {
#ifdef LINUX
    pthread_mutex_init(&mutex_lock, NULL);
#endif
    
#ifdef WIN32
    InitializeCriticalSection(&mutex_lock);
#endif
    }
    
    ~Mutex()
    {
        DelLock();
    }

    void Lock()
    {
#ifdef LINUX
    pthread_mutex_lock(&mutex_lock);
#endif

#ifdef WIN32
    EnterCriticalSection(&mutex_lock);
#endif    
    }

    void UnLock()
    {
#ifdef LINUX
    pthread_mutex_unlock(&mutex_lock);
#endif

#ifdef WIN32
    LeaveCriticalSection(&mutex_lock);
#endif    
    }

    void DelLock()
    {
#ifdef LINUX
    pthread_mutex_destroy(&mutex_lock);
#endif
#ifdef WIN32
    DeleteCriticalSection(&mutex_lock);
#endif    
    }

private:

#ifdef LINUX
    pthread_mutex_t   mutex_lock;
#endif

#ifdef WIN32
    CRITICAL_SECTION  mutex_lock;
#endif
};

class FwdSyncServ
{
public:
    static FwdSyncServ* GetInstance();
    void SM1Lock();
    void SM1UnLock();
    void SM2Lock();
    void SM2UnLock();
    void AuthLock();
    void AuthUnLock();
    void DeInitServ();
private:
    FwdSyncServ();
    ~FwdSyncServ();
private:
    static FwdSyncServ* m_pFwdSyncServ;
    Mutex m_sm1Lock;
    Mutex m_sm2Lock;
    Mutex m_tnlAuthLock;
};

#endif
