#include <stdio.h>
#include "utils.h"
#include "utils_inc.h"

void utils_dbg(const char* strlog, ...)
{
	printf  ("[UTIL]: ");
	va_list vp;
	va_start(vp, strlog);
	vprintf (strlog, vp);
	va_end  (vp);
	printf  ("\n");
}

int utils_init()
{
	utils_log("utils_init called.");
	return 0;
}

int utils_destory()
{
	utils_log("utils_destory called.");
	return 0;
}

uint get_netmask(const char* mask)
{
    uint masklen=0, i=0;
    uint netmask=0;

    if(mask == NULL)
    {
        return 0;
    }

    struct in_addr ip_addr;
    if( inet_aton(mask, &ip_addr) )
    {
        netmask = ntohl(ip_addr.s_addr);
    }else{
        netmask = 0;
        return 0;
    }
    
    while(0 == (netmask & 0x01) && i<32)
    {
        i++;
        netmask = netmask>>1;
    }
    masklen = 32-i;
    return masklen;
}

char *get_netmask(uint masklen)
{
    uint mask=0;
    uint i=0;
    struct in_addr ip_addr;

    if(masklen < 0 || masklen > 32)
    {
        return NULL;
    }
    
    for(i=0;i<masklen;i++)
        mask |= 0x80000000>>i;
    
    ip_addr.s_addr = htonl(mask);
    
    return inet_ntoa(ip_addr);
}

int net_aton(const char *cp, double *nip)
{
    int dots = 0;
    register uint  acc  = 0;
    register uint  addr = 0;

    do 
    {
		register char cc = *cp;
		switch (cc) 
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				acc = acc * 10 + (cc - '0');
				break;
				
			case '.':
				if (++dots > 3)
				{
					return ERR;
				}
				// Fall through
				
			case '\0':
				if (acc > 255) 
				{
					return ERR;
				}
				addr = addr << 8 | acc; // 这句是精华,每次将当前值左移八位加上后面的值
				acc = 0;
				break;
				
			default:
				return 0;
		}
 
    } while (*cp++) ;
	
    // Normalize the address 
    if (dots < 3)
	{
		addr <<= 8 * (3 - dots) ;
    }

    if (nip) 
	{
		*nip = addr;
    }
	
    return OK;    
}

int Readfile(const char* src, char* des,ulong &enclen)
{
	int nRet = OK;
	FILE* pSrc = NULL;
	pSrc = fopen(src, "r");
 
	if (pSrc)
	{
		uint nLen = 0;
		char szBuf[1024] = {0};
		while((nLen = fread(szBuf, 1, sizeof szBuf, pSrc)) > 0)
		{
			strcpy(des,szBuf);
			enclen = nLen;
		}
	}
	else
	{
	    nRet = ERR;
    }
 
	if (pSrc)
	{
	    fclose(pSrc), pSrc = NULL;
    }
 
	return nRet;
}

int copyfile(const char* src, const char* des)
{
	int nRet = OK;
	FILE* pSrc = NULL, *pDes = NULL;
	pSrc = fopen(src, "r");
 	pDes = fopen(des, "w+");
	
	if (pSrc && pDes)
	{
		int nLen = 0;
		char szBuf[1024] = {0};
		while((nLen = fread(szBuf, 1, sizeof szBuf, pSrc)) > 0)
		{
			fwrite(szBuf, 1, nLen, pDes);
		}
	}
	else
	{
	    nRet = ERR;
    }
 
	if (pSrc)
	{
	    fclose(pSrc), pSrc = NULL;
    }
 
	if (pDes)
	{	
	    fclose(pDes), pDes = NULL;
    }
 
	return nRet;
}

int updatefile(const char* filename, const char* data, size_t nLen)
{
    int ret  = ERR;
	size_t nWrite = 0;
    FILE* pfile = NULL;

    if (nLen < 0)
    {
        ret = ERR;
    }
    remove(filename);
	pfile = fopen(filename, "aw+");
    
	if (pfile)
	{
		nWrite = fwrite(data, 1, nLen, pfile);
        if(nWrite != nLen)
        {
            ret = ERR;
        }
        else
        {
            ret = OK;
        }
	}
	else
	{
	    ret = ERR;
    }
 
	if (pfile)
	{
	    fclose(pfile);
        pfile = NULL;
    }

	return ret;
}


int writefile(const char* filename, const char* data, size_t nLen)
{
    int ret  = ERR;
	size_t nWrite = 0;
    FILE* pfile = NULL;

    if (nLen < 0)
    {
        ret = ERR;
    }
    remove(filename);
	pfile = fopen(filename, "aw+");
    
	if (pfile)
	{
		nWrite = fwrite(data, 1, nLen, pfile);
        if(nWrite != nLen)
        {
            ret = ERR;
        }
        else
        {
            ret = OK;
        }
	}
	else
	{
	    ret = ERR;
    }
 
	if (pfile)
	{
	    fclose(pfile);
        pfile = NULL;
    }

	return ret;
}

ulong getfilesize(pcstr filename)
{
    CHK_NULL_PTR(filename, 0);
    
    ulong nLen = 0;
    FILE* pFile = fopen(filename, "r");
    CHK_NULL_PTR(pFile, 0);
    fseek(pFile, 0, SEEK_END);
    
    nLen = ftell(pFile);
    rewind(pFile);

    fclose(pFile);
    return nLen;
}

/*int getmd5str(byte* data, uint datalen, std::string& strmd5)
{
    CHK_NULL(data, ERR);
    static byte  szMd5[16] = {0};
    static char  szMd5Res[33] = {0};
    
    MD5(data, datalen, szMd5);

    for (int i = 0; i < 16; i++) 
    {
        sprintf(szMd5Res+i*2, "%02x", szMd5[i]);
    }
    
    szMd5Res[32] = 0;
    strmd5 = szMd5Res;

    return OK;
}

int getsha256str(byte* data, uint datalen, std::string& strsha256)
{
    CHK_NULL(data, ERR);
    static byte  szSHA[32]    = {0};
    static char  szSHARes[65] = {0};
    
    SHA256(data, datalen, szSHA);

    for (int i = 0; i < 32; i++) 
    {
        sprintf(szSHARes+i*2, "%02x", szSHA[i]);
    }

    szSHARes[64] = 0;
    strsha256 = szSHARes;
    
    return OK;
}
*/
uint get_timespan()
{
    static ulong lastsec = 0;
    ulong spansec = 0;
    ulong currsec = 0;
    
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    currsec = tn.tv_sec;
    spansec = currsec - lastsec;
    lastsec = currsec;
    
    return (spansec == 0) ? 1 : spansec;
}

void debug_natime()
{
    struct timespec tn;
    clock_gettime(CLOCK_REALTIME, &tn);
    utils_log("sec:%u.nsec:%u.", tn.tv_sec, tn.tv_nsec);
}

void debug_data(byte* data, uint len)
{
    for (int i = 1; i <= len; i++)
    {
        printf("%02x ", data[i-1]);
        if(i % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n\n");
}


/* 日志时间戳,精确到毫秒 */
char* get_stime(void)
{ 
    static char timestr[200 + 1] = {0};
    struct tm * pTempTm;
    struct timeval time;
        
    gettimeofday(&time,NULL);
    pTempTm = localtime(&time.tv_sec);
    if( NULL != pTempTm )
    {
        snprintf(timestr,200,"%04d-%02d-%02d %02d:%02d:%02d.%03ld",
            pTempTm->tm_year+1900,
            pTempTm->tm_mon+1, 
            pTempTm->tm_mday,
            (pTempTm->tm_hour+8), 
            pTempTm->tm_min, 
            pTempTm->tm_sec,
            time.tv_usec/1000);
    }
    return timestr;
}

char* get_filename(pcstr prefix)
{ 
    static char timestr[200 + 1] = {0};
    struct tm * pTempTm;
    struct timeval time;
        
    gettimeofday(&time,NULL);
    pTempTm = localtime(&time.tv_sec);
    if( NULL != pTempTm )
    {
        snprintf(timestr,200,"%s_%04d_%02d_%02d_%02d_%02d_%02d_%03ld.ini",
            prefix,
            pTempTm->tm_year+1900,
            pTempTm->tm_mon+1, 
            pTempTm->tm_mday,
            pTempTm->tm_hour, 
            pTempTm->tm_min, 
            pTempTm->tm_sec,
            time.tv_usec/1000);
    }
    return timestr;
}


