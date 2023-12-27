#include "utils.h"
#include "sm2cert.h"

static int istext(int format)
{
    return (format & B_FORMAT_TEXT) == B_FORMAT_TEXT;
}

static const char *modestr(char mode, int format)
{
    OPENSSL_assert(mode == 'a' || mode == 'r' || mode == 'w');

    switch (mode) {
    case 'a':
        return istext(format) ? "a" : "ab";
    case 'r':
        return istext(format) ? "r" : "rb";
    case 'w':
        return istext(format) ? "w" : "wb";
    }
    /* The assert above should make sure we never reach this point */
    return NULL;
}

SdkCert::SdkCert()
{
    m_pX509 = NULL;
}

SdkCert::~SdkCert()
{
    if (NULL != m_pX509)
    {
        X509_free(m_pX509);
    }
    m_pX509 = NULL;
}

int SSLInit()
{
    /* SSL 库初始化 */
    SSL_library_init();

    SSLeay_add_ssl_algorithms();
    
    /* 载入所有 SSL 错误消息 */
    SSL_load_error_strings();
    
    ERR_load_crypto_strings();
    
    /* 载入所有 SSL 算法 */
    OpenSSL_add_all_algorithms();
    
    return OK;
}

int SdkCert::LoadPem(pcstr filename)
{
    X509* pX509 = NULL;
    FILE *fp = fopen(filename, "rb");
    if (!fp) 
    {
    	utils_log("SdkCert::LoadPem unable to open pem key file: %s", filename);
    	return ERR;
    }

    pX509 = PEM_read_X509(fp, NULL, NULL, NULL);
    if (!pX509) 
    {
    	utils_log("SdkCert::LoadPem unable to parse certificate: %s", filename);
    	fclose(fp);
    	return ERR;
    }
    
    UnLoadPem();
    
    m_pX509 = pX509;
    
    fclose(fp);     
    return OK;
}

int SdkCert::UnLoadPem()
{
    if (NULL != m_pX509)
    {
        X509_free(m_pX509);
    }

    m_pX509 = NULL;
    return OK;
}

int SdkCert::GetVersion()
{
    if (NULL == m_pX509)
    {
        return ERR;
    }
    return ((int)X509_get_version(m_pX509)) + 1;
}

int SdkCert::GetSN(char* SN, uint *nLen)
{
    int ret = OK;
    
    ASN1_INTEGER *asn1_i = NULL;
    BIGNUM *bignum = NULL;
    char *serial = NULL;
 
    if (!m_pX509)
    {
        return ERR;
    }
    if (!nLen)
    {
        return ERR;
    }
    asn1_i = X509_get_serialNumber(m_pX509);
    bignum = ASN1_INTEGER_to_BN(asn1_i, NULL);
    if (bignum == NULL) 
    {
        return ERR;
    }
    serial = BN_bn2hex(bignum);
    if (serial == NULL) 
    {
        return ERR;
    }
    BN_free(bignum);
    if (!SN)
    {
        *nLen = strlen(serial) + 1;
        OPENSSL_free(serial);
        return ERR;
    }
    if (*nLen < strlen(serial) + 1)
    {
        OPENSSL_free(serial);
        return ERR;
    }

    *nLen = strlen(serial);
    strncpy(SN, serial, *nLen);

    OPENSSL_free(serial);
    return OK;
}

int SdkCert::GetKeyType(uint* nType)
{
    EVP_PKEY *pk = NULL;
    stack_st_X509* chain = NULL;
    X509_EXTENSION *pex = NULL;
    
    if (!m_pX509)
    {
        return ERR;
    }
    if (!nType)
    {
        return ERR;
    }
 
    pk = X509_get_pubkey(m_pX509);
    if (!pk)
    {
        return ERR;
    }
 
    if (EVP_PKEY_RSA == EVP_PKEY_id(pk))
    {
        *nType = CERT_KEY_ALG_RSA;
    }
    /*else if (EVP_PKEY_SM2 == EVP_PKEY_id(pk))
    {
        *nType = CERT_KEY_ALG_SM2;
    }*/
    else if (EVP_PKEY_EC == EVP_PKEY_id(pk))
    {
        *nType = CERT_KEY_ALG_ECC;
    }
    else if (EVP_PKEY_DSA == EVP_PKEY_id(pk))
    {
        *nType = CERT_KEY_ALG_DSA;
    }
    else if (EVP_PKEY_DH == EVP_PKEY_id(pk))
    {
        *nType = CERT_KEY_ALG_DH;
    }   
    else 
    {
        return ERR;
    }
    
    return OK;
}


int SdkCert::GetKeyUsage(uint* pnUsage)
{
    uint nKeyUsage = 0;
     
    if (!m_pX509)
    {
        return ERR;
    }
    if (!pnUsage)
    {
        return ERR;
    }
 
    *pnUsage = 0;
    
    //X509_check_ca() MUST be called!
    //X509_check_ca(m_pX509);
    //nKeyUsage = X509_get_extended_key_usage(m_pX509);
    /*
    if ((nKeyUsage & KU_DATA_ENCIPHERMENT) == KU_DATA_ENCIPHERMENT)
    {
        *pnUsage = 0;
    }
    else if ((nKeyUsage & KU_DIGITAL_SIGNATURE) == KU_DIGITAL_SIGNATURE)
    {
        *pnUsage = 1;
    }
    */
    return OK;
}

int SdkCert::GetSignAlgOid()
{
    if (!m_pX509)
	{
		return ERR;
	}

    int pkey_nid = X509_get_signature_nid(m_pX509);
    if (pkey_nid == NID_undef) 
    {
    	utils_log("unable to find specified signature algorithm name");
    	return ERR;
    }
    
    return pkey_nid;
}

/*
    pStrName buff like this char sigalgo_name[SIG_ALGO_LEN+1];
*/
int SdkCert::GetSignAlgOName(char* pStrName)
{
    if (!m_pX509)
	{
		return ERR;
	}

    int pkey_nid = X509_get_signature_nid(m_pX509);
    if (pkey_nid == NID_undef) 
    {
    	utils_log("unable to find specified signature algorithm name");
    	return ERR;
    }
    
    const char* sslbuf = OBJ_nid2ln(pkey_nid);
    if (strlen(sslbuf) > PUBKEY_ALGO_LEN) 
    {
        utils_log("public key algorithm name longer than allocated buffer.");
        return ERR;
    }

    strncpy(pStrName, sslbuf, PUBKEY_ALGO_LEN); 
    
    return OK;
}


int SdkCert::GetIssuer(char* pIssuer, uint *pLen)
{
	int nNameLen = 512;
	char csCommonName[nNameLen] = {0};
	X509_NAME *pCommonName = NULL;
 
	if (!m_pX509)
	{
		return ERR;
	}
	if (!pLen)
	{
		return ERR;
	}
 
	pCommonName = X509_get_issuer_name(m_pX509);
	if (!pCommonName)
	{
		return ERR;
	}
	nNameLen = X509_NAME_get_text_by_NID(pCommonName, NID_commonName, csCommonName, nNameLen);
	if (-1 == nNameLen)
	{
		return ERR;
	}
    
	if (!pIssuer)
	{
		*pLen = nNameLen + 1;
		return ERR;
	}
    
	if (*pLen < nNameLen + 1)
	{
		return ERR;
	}
    
	strncpy(pIssuer, csCommonName, nNameLen);
    
    *pLen = nNameLen;
    
	return OK;
}

int SdkCert::GetSubjectName(char* lpValue, uint *pulLen)
{
    int iLen = 0;
    int iSubNameLen = 0;
    char csSubName[1024] = {0};
    char csBuf[256] = {0};
    X509_NAME *pSubName = NULL;
    
    if (!m_pX509)
    {
        return ERR;
    }
    if (!pulLen)
    {
        return ERR;
    }
 
    pSubName = X509_get_subject_name(m_pX509);
    if (!pSubName)
    {
        return ERR;
    }
    
    memset(csBuf, 0, 256);
    iLen = X509_NAME_get_text_by_NID(pSubName, NID_countryName, csBuf, 256);
    if (iLen > 0)
    {
        strcat(csSubName, "C=");
        strcat(csSubName, csBuf);
        strcat(csSubName, ", ");
    }
    
    memset(csBuf, 0, 256);
    iLen = X509_NAME_get_text_by_NID(pSubName, NID_organizationName, csBuf, 256);
    if (iLen > 0)
    {
        strcat(csSubName, "O=");
        strcat(csSubName, csBuf);
        strcat(csSubName, ", ");
    }
    
    memset(csBuf, 0, 256);
    iLen = X509_NAME_get_text_by_NID(pSubName, NID_organizationalUnitName, csBuf, 256);
    if (iLen > 0)
    {
        strcat(csSubName, "OU=");
        strcat(csSubName, csBuf);
        strcat(csSubName, ", ");
    }
    
    memset(csBuf, 0, 256);
    iLen = X509_NAME_get_text_by_NID(pSubName, NID_commonName, csBuf, 256);
    if (iLen > 0)
    {
        strcat(csSubName, "CN=");
        strcat(csSubName, csBuf);
    }
    
    if (!lpValue)
    {
        *pulLen = strlen(csSubName) + 1;
        return ERR;
    }
    if (*pulLen < strlen(csSubName) + 1)
    {
        return ERR;
    }
    
    *pulLen = strlen(csSubName);
    
    strncpy(lpValue, csSubName, *pulLen);
    
    return OK;
}

int SdkCert::GetValidDate()
{
    /* SYSTEMTIME *ptmStart, SYSTEMTIME *ptmEnd
    ASN1_TIME *start = NULL;
    ASN1_TIME *end = NULL;
    time_t ttStart = {0};
    time_t ttEnd = {0};
    unsigned long long nLLStart = 0;
    unsigned long long nLLEnd = 0;
    FILETIME ftStart = {0};
    FILETIME ftEnd = {0};
 
    if (!m_pX509)
    {
        return ERR;
    }
 
    start = X509_get_notBefore(m_pX509);
    end = X509_get_notAfter(m_pX509);
    
    ttStart = ASN1_TIME_normalize(start);
    ttEnd = ASN1_TIME_normalize(end);   
    nLLStart = Int32x32To64(ttStart, 10000000) + 116444736000000000;
    nLLEnd = Int32x32To64(ttEnd, 10000000) + 116444736000000000;
 
    ftStart.dwLowDateTime = (DWORD)nLLStart;
    ftStart.dwHighDateTime = (DWORD)(nLLStart >> 32);
    ftEnd.dwLowDateTime = (DWORD)nLLEnd;
    ftEnd.dwHighDateTime = (DWORD)(nLLEnd >> 32);
 
    FileTimeToSystemTime(&ftStart, ptmStart);
    FileTimeToSystemTime(&ftEnd, ptmEnd);
    */
    return OK;
}

int SdkCert::GetSM2PrivateKey(pcstr filename, byte* pPriateKey, uint* pPriLen, byte* pPublicKey, uint* pPubLen)
{
    int ret = ERR;
    int npublen = 0;
    int nprilen = 0;
    byte *pribuf  = NULL;
    byte *pubbuf  = NULL;
    const EC_GROUP *group;
    
    if (!pPriateKey)
    {   
        utils_log("SdkCert::GetSM2PrivateKey bad private buff");
        return ERR;
    }

    if (!pPublicKey)
    {  
        utils_log("SdkCert::GetSM2PrivateKey bad public buff");
        return ERR;
    }

    BIO *in = BIO_new_file(filename, modestr('r', FORMAT_PEM));
    if (!in) 
    {
    	utils_log("SdkCert::GetSM2PrivateKey unable to open pem key file: %s", filename);
    	return ERR;
    }

    EC_KEY *pprikey = PEM_read_bio_ECPrivateKey(in, NULL, NULL, NULL);
    if (!pprikey) 
    {
    	utils_log("SdkCert::GetSM2PrivateKey get private key fail.: %s", filename);
        BIO_free(in);
    	return ERR;
    }

    group = EC_KEY_get0_group(pprikey);
    if (!group) 
    {
    	utils_log("SdkCert::GetSM2PrivateKey check keyfile fail: %s", filename);
        BIO_free(in);
    	return ERR;
    }

    if (EC_KEY_get0_private_key(pprikey) != NULL)
    {
        nprilen = EC_KEY_priv2buf(pprikey, &pribuf);
        if(nprilen == 0)
        {
            OPENSSL_free(pribuf);
            BIO_free(in); 
            utils_log("SdkCert::GetSM2PrivateKey get private key buff fail.: %s", filename);
            return ERR;
        }

        if (*pPriLen < nprilen)
        {
            *pPriLen = nprilen;
            OPENSSL_free(pribuf);
            BIO_free(in); 
            utils_log("SdkCert::GetSM2PrivateKey get private key buff fail.: %s", filename);
            return ERR;
        }

        *pPriLen = nprilen;
        memcpy(pPriateKey, pribuf, nprilen);
        OPENSSL_free(pribuf);
    }
     
    if(EC_KEY_get0_public_key(pprikey) != NULL)
    {
        npublen = EC_KEY_key2buf(pprikey, EC_KEY_get_conv_form(pprikey), &pubbuf, NULL);
        if(npublen == 0)
        {
            OPENSSL_free(pubbuf);
            BIO_free(in);
            utils_log("SdkCert::GetSM2PrivateKey get public key buff fail.: %s", filename);
            return ERR;
        }
        if (*pPubLen < npublen)
        {
            *pPubLen = npublen;
            OPENSSL_free(pubbuf);
            BIO_free(in);
            utils_log("SdkCert::GetSM2PrivateKey get public key buff fail.: %s", filename);
            return ERR;
        }

        *pPubLen = npublen;
        memcpy(pPublicKey, pubbuf, npublen);
        OPENSSL_free(pubbuf);
    }

    BIO_free(in);
    return OK;
}

int SdkCert::GetSM2PublicKey(pcstr filename, byte* pPublicKey, uint* pPubLen)
{
    int ret = ERR;
    int npublen = 0;
    byte *pubbuf  = NULL;
    const EC_GROUP *group;
   
    if (!pPublicKey)
    {  
        utils_log("SdkCert::GetSM2PrivateKey bad public buff");
        return ERR;
    }

    BIO *in = BIO_new_file(filename, modestr('r', FORMAT_PEM));
    if (!in) 
    {
    	utils_log("SdkCert::GetSM2PublicKey unable to open pem key file: %s", filename);
    	return ERR;
    }

    EC_KEY *ppubkey = PEM_read_bio_EC_PUBKEY(in, NULL, NULL, NULL);
    if (!ppubkey) 
    {
    	utils_log("SdkCert::GetSM2PublicKey get public key fail.: %s", filename);
        BIO_free(in);
    	return ERR;
    }
    
    group = EC_KEY_get0_group(ppubkey);
    if (!group) 
    {
    	utils_log("SdkCert::GetSM2PublicKey check keyfile fail: %s", filename);
        BIO_free(in);
    	return ERR;
    }

    if(EC_KEY_get0_public_key(ppubkey) != NULL)
    {
        npublen = EC_KEY_key2buf(ppubkey, EC_KEY_get_conv_form(ppubkey), &pubbuf, NULL);
        if(npublen == 0)
        {
            OPENSSL_free(pubbuf);
            BIO_free(in);
            return ERR;
        }
        if (*pPubLen < npublen)
        {
            *pPubLen = npublen;
            OPENSSL_free(pubbuf);
            BIO_free(in);
            return ERR;
        }

        *pPubLen = npublen;
        memcpy(pPublicKey, pubbuf, npublen);
        OPENSSL_free(pubbuf);
    }

    BIO_free(in);
    return OK;
}


int SdkCert::GetPrivateKey(pcstr filename, byte* pPriateKey, uint* pLen)
{
    int pkeylen = 0;
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) 
    {
    	utils_log("SdkCert::GetPrivateKey unable to open pem key file: %s", filename);
    	return ERR;
    }    
 
    EVP_PKEY *pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);

    CHK_NULL(pkey,ERR);

    if (!pPriateKey)
    {
        *pLen = pkeylen;
        return ERR;
    }

    pkeylen = i2d_PrivateKey(pkey, &pPriateKey);
    if (0 == pkeylen)
    {
        *pLen = pkeylen;
        return ERR;
    }

    if (*pLen < pkeylen)
    {
        return ERR;
    }

    *pLen = pkeylen;
    
    EVP_PKEY_free(pkey);

    return OK;
}
int SdkCert::GetPublicKey(byte* pPublicKey, uint* pLen)
{
    int ret = ERR;
    uint nkeyType = -1;
    int pkeylen = 0;
    
    EVP_PKEY *pkey = NULL;

    ret = GetKeyType(&nkeyType);
    CHK_ERR(ret, ERR);

    switch (nkeyType)
    {
        case CERT_KEY_ALG_RSA:
        {
            break;
        }
        break;
        case CERT_KEY_ALG_SM2:
        case CERT_KEY_ALG_ECC:
        {
            pkey = X509_get_pubkey(m_pX509);
            if (NULL == pkey)
            {
                ret = ERR;
                break;
            }
            
            pkeylen = i2d_PublicKey(pkey, &pPublicKey);
            if (0 == pkeylen)
            {
                ret = ERR;
                break;
            }

            *pLen = pkeylen;
            
            EVP_PKEY_free(pkey);
            ret = OK;
        }
        break;
        case CERT_KEY_ALG_DSA:
        {
            break;
        }
        break;
        default:
        {
            break;
        }
    }
    
    return ret;
}

int SdkCert::GetCertJson(pcstr certfile)
{
    int ret = ERR;
    CJsonObject certJson;
    
    if (!certfile)
	{
		return ERR;
	}

    ret = LoadPem(certfile);
    CHK_ERR(ret, ERR);

    char CertSecBuff[CERT_SEC_MAX_LEN] = {0};
    
    uint nLen = CERT_SEC_MAX_LEN;
    ret = GetIssuer(CertSecBuff, &nLen);
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_ISSUE_NAME, std::string(CertSecBuff, nLen));

    nLen = CERT_SEC_MAX_LEN;
    ret = GetSN(CertSecBuff, &nLen);
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_SERIAL_NO, std::string(CertSecBuff, nLen));

    ret = GetVersion();
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_VERSION, ret);

    uint keytype = 0;
    ret = GetKeyType(&keytype);
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_KEY_TYPE, keytype);

    //ret = GetSignAlgOid();
    //CHK_ERR(ret, ERR);
    //certJson.Add(CERT_SEC_ALGO_ID, ret);
    
    nLen = CERT_SEC_MAX_LEN;
    ret = GetSubjectName(CertSecBuff, &nLen);
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_SUBJECT_NAME, std::string(CertSecBuff, nLen));

    nLen = CERT_SEC_MAX_LEN;
    ret =  GetPublicKey((byte*)CertSecBuff, &nLen);
    CHK_ERR(ret, ERR);
    certJson.Add(CERT_SEC_PUB_KEY, std::string(CertSecBuff+1, nLen-1));

    m_certJson.Clear();
    m_certJson = certJson;

    return OK;
}

int SdkCert::CheckPeerCert(std::string &peerCertString, byte* pPeerPubKey)
{
    bool res = false;
    std::string peerCertItem = "";
    std::string selfCertItem = "";
    int         peerIntItem  = 0;
    int         selfIntItem  = 0;
    CJsonObject peerCertJson(peerCertString);

    res = peerCertJson.Get(CERT_SEC_ISSUE_NAME, peerCertItem);
    CHK_TRUE(res, ERR);
    res = m_certJson.Get(CERT_SEC_ISSUE_NAME, selfCertItem);
    CHK_TRUE(res, ERR);
    if(peerCertItem != selfCertItem)
    {
        return ERR;
    }

    res = peerCertJson.Get(CERT_SEC_SERIAL_NO, peerCertItem);
    CHK_TRUE(res, ERR);
    res = m_certJson.Get(CERT_SEC_SERIAL_NO, selfCertItem);
    CHK_TRUE(res, ERR);
    if(peerCertItem != selfCertItem)
    {
        return ERR;
    }

    res = peerCertJson.Get(CERT_SEC_VERSION, peerIntItem);
    CHK_TRUE(res, ERR);
    res = m_certJson.Get(CERT_SEC_VERSION, selfIntItem);
    CHK_TRUE(res, ERR);
    if(peerIntItem != selfIntItem)
    {
        return ERR;
    }

    res = peerCertJson.Get(CERT_SEC_KEY_TYPE, peerIntItem);
    CHK_TRUE(res, ERR);
    res = m_certJson.Get(CERT_SEC_KEY_TYPE, selfIntItem);
    CHK_TRUE(res, ERR);
    if(peerIntItem != selfIntItem)
    {
        return ERR;
    }

    //res = peerCertJson.Get(CERT_SEC_ALGO_ID, peerIntItem);
    //CHK_TRUE(res, ERR);
    //res = m_certJson.Get(CERT_SEC_ALGO_ID, selfIntItem);
    //CHK_TRUE(res, ERR);
    //if(peerIntItem != selfIntItem)
    //{
    //    return ERR;
    //}

    res = peerCertJson.Get(CERT_SEC_SUBJECT_NAME, peerCertItem);
    CHK_TRUE(res, ERR);
    res = m_certJson.Get(CERT_SEC_SUBJECT_NAME, selfCertItem);
    CHK_TRUE(res, ERR);
    if(peerCertItem != selfCertItem)
    {
        return ERR;
    }

    res = peerCertJson.Get(CERT_SEC_PUB_KEY, peerCertItem);
    CHK_TRUE(res, ERR);

    if(peerCertItem.length() != SM2_PUB_KEY_LEN)
    {
        return ERR;
    }

    strncpy((char*)pPeerPubKey, peerCertItem.c_str(), peerCertItem.length());
    
    return OK;
}

int SdkCert::DoSignature(std::string &signString)
{
    int ret = ERR;
    
    char *uid = "ycdng@163.com";
	int lenUid = strlen(uid);	
	char *src = "messagedigest";
	int  lenSrc = strlen(src);
	unsigned char sig[200] = {0};
	unsigned long ulSigLen = 200;

    CJsonObject signJson;
    ret = GM_SM2Sign(sig, &ulSigLen, (unsigned char*)src, lenSrc, 
                                     (unsigned char*)uid, lenUid, m_bSelfPriKey, SM2_PRI_KEY_LEN);
	if (ret == 0)
	{
		utils_log("SdkCert::DoSignature OKEY.");
        ret = OK;
	}
	else
	{
		utils_log("SdkCert::DoSignature FAILED.");
        print_data(sig, ulSigLen);
        ret = ERR;
	}

    signJson.Add(CERT_SEC_SIGN, std::string((char*)sig, ulSigLen));
    signJson.Add(CERT_SEC_SIGN_LEN, ulSigLen);
    signString = signJson.ToString();

    return ret;
}

int SdkCert::DoVerify(std::string &peerSignString)
{
    int  ret = ERR;
    bool res = false;
    
    char *uid = "ycdng@163.com";
	int lenUid = strlen(uid);	
	char *src = "messagedigest";
	int  lenSrc = strlen(src);
    
	unsigned long ulSigLen = 0;
    std::string strSign = "";
    CJsonObject peerSignJson(peerSignString);
    res = peerSignJson.Get(CERT_SEC_SIGN, strSign);
    CHK_TRUE(res, ERR);
    res = peerSignJson.Get(CERT_SEC_SIGN_LEN, ulSigLen);
    CHK_TRUE(res, ERR);

    ret = GM_SM2VerifySig((unsigned char*)(strSign.c_str()), ulSigLen, (unsigned char *)src, lenSrc, 
                          (unsigned char*)uid, lenUid, m_bSelfPubKey, SM2_PUB_KEY_LEN);
	if (ret == 0)
	{
		utils_log("SdkCert::DoVerify OKEY.");
        ret = OK;
	}
	else
	{
		utils_log("SdkCert::DoVerify FAILED.");
        print_data((byte*)(strSign.c_str()), ulSigLen);
        ret = ERR;
	}

    return ret;
}


int SdkCert::DoDecry()
{
    return OK;
}

int SdkCert::DoCrypt()
{
    return OK;
}



