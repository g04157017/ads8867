#ifndef JSCB_SDK_CERT_H
#define JSCB_SDK_CERT_H

#define PUBKEY_ALGO_LEN 128
#define SM2_PUB_KEY_LEN 64
#define SM2_PRI_KEY_LEN 32
#define CERT_SEC_MAX_LEN 1500
#define CERT_SM2_EXT_LEN 256

#define CERT_SEC_ISSUE_NAME     "IssuerName"
#define CERT_SEC_SERIAL_NO      "SerialNo"
#define CERT_SEC_VERSION        "Version"
#define CERT_SEC_KEY_TYPE       "KeyType"
#define CERT_SEC_ALGO_ID        "AlgoId"
#define CERT_SEC_SUBJECT_NAME   "SubjectName"
#define CERT_SEC_PUB_KEY        "PubKey"
#define CERT_SEC_SIGN           "Signature"
#define CERT_SEC_SIGN_LEN       "SignLength"

typedef enum e_keytype
{
    CERT_KEY_ALG_RSA = 0,
    CERT_KEY_ALG_SM2,
    CERT_KEY_ALG_ECC,
    CERT_KEY_ALG_DSA,
    CERT_KEY_ALG_DH,
} KEY_TYPE_E;

# define B_FORMAT_TEXT   0x8000
# define FORMAT_UNDEF    0
# define FORMAT_TEXT    (1 | B_FORMAT_TEXT)     /* Generic text */
# define FORMAT_BINARY   2                      /* Generic binary */
# define FORMAT_BASE64  (3 | B_FORMAT_TEXT)     /* Base64 */
# define FORMAT_ASN1     4                      /* ASN.1/DER */
# define FORMAT_PEM     (5 | B_FORMAT_TEXT)
# define FORMAT_PKCS12   6
# define FORMAT_SMIME   (7 | B_FORMAT_TEXT)
# define FORMAT_ENGINE   8                      /* Not really a file format */
# define FORMAT_PEMRSA  (9 | B_FORMAT_TEXT)     /* PEM RSAPubicKey format */
# define FORMAT_ASN1RSA  10                     /* DER RSAPubicKey format */
# define FORMAT_MSBLOB   11                     /* MS Key blob format */
# define FORMAT_PVK      12                     /* MS PVK file format */
# define FORMAT_HTTP     13                     /* Download using HTTP */
# define FORMAT_NSS      14                     /* NSS keylog format */

int SSLInit();

class SdkCert
{
public:
    SdkCert();
    virtual ~SdkCert();
	int LoadPem(pcstr filename);
    int UnLoadPem();
    int GetVersion();
    int GetSN(char* SN, uint *nLen);
    int GetKeyType(uint* nType);
    int GetKeyUsage(uint* pnUsage);
    int GetSignAlgOid();
    int GetSignAlgOName(char* pStrName);
    int GetIssuer(char* pIssuer, uint *pLen);
    int GetSubjectName(char* lpValue, uint *pulLen);
    int GetValidDate(/* SYSTEMTIME *ptmStart, SYSTEMTIME *ptmEnd */);
    int GetSM2PublicKey(pcstr filename, byte* pPublicKey, uint* pPubLen);
    int GetSM2PrivateKey(pcstr filename, byte* pPriateKey, uint* pPriLen, byte* pPublicKey, uint* pPubLen);
    int GetPrivateKey(pcstr filename, byte* pPriateKey, uint* pLen);
    int GetPublicKey(byte* pPublicKey, uint* pLen);

    int GetCertJson(pcstr certfile);
    int IsValidCert();
    int CheckPeerCert(std::string &peerCertString, byte* pPeerPubKey);
    int DoSignature(std::string &signString);
    int DoVerify(std::string &peerSignString);

    int DoDecry();
    int DoCrypt();
    
    byte  m_bSelfPubKey[SM2_PUB_KEY_LEN];
    byte  m_bSelfPriKey[SM2_PRI_KEY_LEN];
    CJsonObject m_certJson;
    
private:
    X509* m_pX509;
};


#endif
