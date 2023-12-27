#ifndef INIEXT_H
#define INIEXT_H

#include "def.h"
#include "dictionary.h"

class IniExt
{
public:
    int LoadIni(pcstr config_file);
    int UnLoadIni();
    
    int Getint(pcstr key);
    pcstr GetString(pcstr key);
    int GetString(pcstr key, std::string& strval);

    int AddKey(pcstr key);
    int SetVal(pcstr key, pcstr val);
    int DelKey(pcstr key);
    int SaveIni(pcstr config_file);
    int GetSecnkeys(pcstr sec);
    pcstr* GetSeckeys(pcstr sec, pcstr* keys);

    static IniExt* GetInstance();
    
private:
    IniExt();
    virtual ~IniExt();    

private:
    dictionary*        m_pconfig;
    static IniExt*     m_pIniExt;
};


#endif


