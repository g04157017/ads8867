#include <stdio.h>
#include "def.h"
#include "iniparser.h"
#include "dictionary.h"
#include "iniext.h"

IniExt* IniExt::m_pIniExt = NULL_PTR;

IniExt::IniExt()
{
    m_pconfig = NULL;
}

IniExt::~IniExt()
{
    if(NULL == m_pconfig)
    {
        iniparser_freedict(m_pconfig);
    }
    m_pconfig = NULL;
}

int IniExt::SaveIni(pcstr config_file)
{
    FILE *fp  = NULL;
    if (NULL == config_file || NULL == m_pconfig)
    {
        return ERR;
    }
    
    fp = fopen(config_file, "w");
    if( fp == NULL ) 
    {
        return ERR;
    }

    iniparser_dump_ini(m_pconfig, fp);

    fclose(fp);
    fp = NULL;

    return OK;
}

IniExt* IniExt::GetInstance()   
{
    if (NULL_PTR == m_pIniExt)
    {
        m_pIniExt = new IniExt();
    }
    
    return (m_pIniExt == NULL_PTR) ? NULL_PTR : m_pIniExt;
}

int IniExt::LoadIni(pcstr config_file)
{
    CHK_NULL(config_file, ERR);
    if (NULL == m_pconfig) 
    {
        m_pconfig =  iniparser_load(config_file);
        CHK_NULL(m_pconfig, ERR);
    }
    return OK;
}

int IniExt::UnLoadIni()
{
    if (NULL == m_pconfig) 
    {
        return OK;
    }
    
    iniparser_freedict(m_pconfig);
    
    m_pconfig = NULL;
    
    return OK;
}

int IniExt::Getint(pcstr key)
{
    int def = ERR;
    CHK_NULL(key, ERR);
    int nval = iniparser_getint(m_pconfig, key, def);
    if(def == nval)
    {
        return ERR;
    }
    return nval;
}

pcstr IniExt::GetString(pcstr key)
{
    char def = ERR;
    CHK_NULL(key, NULL);
    const char* strval = iniparser_getstring(m_pconfig, key, &def);
    if (NULL == strval || def == strval[0])
    {
        return NULL;
    }
    return strval;
}

int IniExt::GetString(pcstr key, std::string& strval)
{
    char def = ERR;
    CHK_NULL(key, ERR);
    const char* cstrval = iniparser_getstring(m_pconfig, key, &def);
    if (NULL == cstrval || def == cstrval[0])
    {
        return ERR;
    }
    
    strval = std::string(cstrval);
    return OK;
}

int IniExt::SetVal(pcstr key, pcstr val)
{
    if((NULL == key) || (NULL == val))
    {
        return ERR;
    }
    return iniparser_set(m_pconfig, key, val);
}

int IniExt::AddKey(pcstr key)
{
    if(NULL == key)
    {
        return ERR;
    }
    return iniparser_set(m_pconfig, key, NULL);
}

int IniExt::DelKey(pcstr key)
{
    if(NULL == key)
    {
        return ERR;
    }

    iniparser_unset(m_pconfig, key);
    return OK;
}

int IniExt::GetSecnkeys(pcstr sec)
{
    if(NULL == sec)
    {
        return 0;
    }
    return iniparser_getsecnkeys(m_pconfig, sec);
}

pcstr* IniExt::GetSeckeys(pcstr sec, pcstr* keys)
{
    if(NULL == sec)
    {
        return NULL;
    }

    return iniparser_getseckeys(m_pconfig, sec, keys);
}

