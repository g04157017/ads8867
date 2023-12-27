#include "utils.h"

FwdSyncServ* FwdSyncServ::m_pFwdSyncServ = NULL_PTR;

FwdSyncServ* FwdSyncServ::GetInstance()
{
    if (NULL_PTR == m_pFwdSyncServ)
    {
        m_pFwdSyncServ = new FwdSyncServ();
    }
    
    return (m_pFwdSyncServ == NULL_PTR) ? NULL_PTR : m_pFwdSyncServ;    
}

void FwdSyncServ::SM1Lock()
{
    m_sm1Lock.Lock();
}

void FwdSyncServ::SM1UnLock()
{
    m_sm1Lock.UnLock();
}

void FwdSyncServ::SM2Lock()
{
    m_sm2Lock.Lock();
}

void FwdSyncServ::SM2UnLock()
{
    m_sm2Lock.UnLock();
}

void FwdSyncServ::AuthLock()
{
    m_tnlAuthLock.Lock();
}

void FwdSyncServ::AuthUnLock()
{
    m_tnlAuthLock.UnLock();
}

void FwdSyncServ::DeInitServ()
{
    m_sm1Lock.DelLock();
    m_sm2Lock.DelLock();
    m_tnlAuthLock.DelLock();
}

FwdSyncServ::FwdSyncServ()
{
}

FwdSyncServ::~FwdSyncServ()
{
    DeInitServ();
}

