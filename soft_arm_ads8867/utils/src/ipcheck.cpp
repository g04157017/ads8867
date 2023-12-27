#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define MIN_IP_LEN  7
#define MAX_IP_LEN  15
#define MAX_PORT_LEN  5

bool IsValidIPString(const char * pc)
{
    int iLen = strlen(pc);
    if(iLen<MIN_IP_LEN || iLen>MAX_IP_LEN)
    {
        return false;
    }

    bool bCurrentIsDigit = false;
    bool bCurrentIsDot = false;
    int iNum = 0;
    for (int i=0; i<iLen; i++)
    {
        if( isdigit(pc[i]) )
        {
            iNum = iNum*10 + (pc[i]-'0');
            if(iNum > 255)
            {
                return false;
            }
            bCurrentIsDigit = true;
            bCurrentIsDot = false;
        }
        else if( pc[i] == '.' )
        {
            if(!bCurrentIsDigit || bCurrentIsDot)
            {
                return false;
            }
            bCurrentIsDigit = false;
            bCurrentIsDot = true;
            iNum = 0;
        }
        else
        {
            return false;
        }
    }
    if(bCurrentIsDot)
    {
        return false;
    }

    return true;
}

//example:
//Input: 00.007.000.01  Output: 0.7.0.1
void IPDeleteZero(char *pc)
{
    char ac[100] = {0};
    strcpy(ac, pc);
    int iLen = strlen(ac);
    char *pcTemp = &ac[0];
    char *pcFound = NULL;
    int i;
    int j;

    for (i=0; pcTemp[i] != '\0'; i++)
    {
        if(pcTemp[i] == '0')
        {
            if((i+1<3) && isdigit(pcTemp[i+1]))
            {
                pcTemp[i] = 'f';
            }
        }
        else
        {
            pcFound = strchr(pcTemp, '.');
            if(pcFound != NULL)
            {
                pcTemp = pcFound + 1;
                i = -1;
            }
            else
            {
                break;
            }
        }
    }

    for (i=0, j=0; i<iLen; )
    {
        if(ac[i] != 'f')
        {
            pc[j++] = ac[i++];
        }
        else
        {
            i++;
        }
    }
    pc[j] = '\0';
}

void IPchar2Int(const char *pc, long long & ll)
{
    ll = 0;
    char ac[1024] = {0};
    strcpy(ac, pc);

    int iTemp;
    char acTemp[100] = {0};
    char * pcHead = &ac[0];
    char * pcFind = strchr(ac, '.');

    while(pcFind != NULL)
    {
        *pcFind = '\0';
        strcpy(acTemp, pcHead);
        pcFind++;
        pcHead = pcFind;

        iTemp = atoi(acTemp);
        ll = (ll + (long long)iTemp) * 256;

        pcFind = strchr(pcHead, '.');
    }
    strcpy(acTemp, pcHead);
    iTemp = atoi(acTemp);
    ll = ll + (long long)iTemp;
}

bool IPAddressIsValid(const char *pcIPAddress, long long& llIp)
{
    if( !IsValidIPString(pcIPAddress) )
    {
        return false;
    }

    IPchar2Int(pcIPAddress, llIp);

    if((llIp==0) || (llIp==INVALID_VALUE))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool IsIPmaskValid(const char *pcIPMask, long long& llMask)
{
    if( !IsValidIPString(pcIPMask) )
    {
        return false;
    }

    IPchar2Int(pcIPMask, llMask);

    if((llMask|(llMask-1)) == INVALID_VALUE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int IpAndMaskIsValid(const char * pcIPAddress, const char * pcIPMask)
{
    long long llIp = 0;
    long long llMask = 0;

    if(!IPAddressIsValid(pcIPAddress, llIp))
    {
        return ERR;
    }
    if(!IsIPmaskValid(pcIPMask, llMask))
    {
        return ERR;
    }

    int iHead = (llIp & 0xff000000) >> 24;
    if(iHead == 127)
    {
        return ERR;
    }
    if( !(iHead>0 && iHead<=223) )
    {
        return ERR;
    }

    long long llNet = llIp & llMask;
    if(llNet==0 || llNet==llMask)
    {
        return ERR;
    }

    long long llHost = llIp & (~llMask);
    if(llHost==0 || llHost==((~llMask)&0xffffff))
    {
        return ERR;
    }
    
    return ERR;
}

bool InTheSameLAN(const char * pcIPAddr1, const char * pcIPMask1,
                   const char * pcIPAddr2, const char * pcIPMask2)
{
    long long llIPAddr1 = 0;
    long long llIPMask1 = 0;
    long long llIPAddr2 = 0;
    long long llIPMask2 = 0;

    if( (!IPAddressIsValid(pcIPAddr1, llIPAddr1)) ||
        (!IsIPmaskValid(pcIPMask1, llIPMask1)) ||
        (!IPAddressIsValid(pcIPAddr2, llIPAddr2)) ||
        (!IsIPmaskValid(pcIPMask2, llIPMask2)) )
    {
        return false;
    }

    long long llNet1 = llIPAddr1 & llIPMask1;
    long long llNet2 = llIPAddr2 & llIPMask2;

    return llNet1 == llNet2;
}

bool IsInvalidPort(const char* pcPort)
{
    int iLen = strlen(pcPort);
    if(iLen>MAX_PORT_LEN)
    {
        return false;
    }
    for (int i=0; i<iLen; i++)
    {
        if(isdigit(pcPort[i]))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    
    return true;
}


