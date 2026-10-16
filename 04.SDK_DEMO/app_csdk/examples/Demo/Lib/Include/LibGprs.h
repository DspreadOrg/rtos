//-----------------------------------------------------------------------------
// @file    : LibGprs.h
// @brief   : GPRS-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_GPRS_H__
#define __LIB_GPRS_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#include "LibTypedef.h"

#define AF_INET         2

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

// Operation successful
#define WLS_OK                  0
//An error occurred
#define WLS_ERR                 -5501
// Module not responding to command
#define WLS_NORSP               -5502
// Module returns "Error"
#define WLS_RSPERR              -5503
// No SIM/UIM card
#define WLS_NOSIM               -5504
//SIM/UIM cards require a PIN code
#define WLS_NEEDPIN             -5505
// SIM/UIM card requires PUK code
#define WLS_NEEDPUK             -5506
// SIM/UIM card locked
#define WLS_SIMBLOCKED          -5507
// SIM/UIM card does not exist or requires a PIN code
#define WLS_SIMERR              -5508
// PIN code error
#define WLS_PINERR              -5509
// This operation is not allowed
#define WLS_NOTALLOWED          -5510
// Parameter error
#define WLS_PARAMERR            -5511
// Incorrect username or password
#define WLS_USERORPWDERR        -5512
// The signal is too weak, please check the antenna, etc
#define WLS_SNLTOOWEAK          -5513
//Module not connected to network
#define WLS_LINKCLOSED          -5514
// The module is already connected to the network
#define WLS_LINKOPENED          -5515
// The module is currently making a network connection
#define WLS_LINKOPENING         -5516
// TCP Socket is closed
#define WLS_TCPCLOSED           -5517
// TCP Socket is open
#define WLS_TCPOPENED           -5518
// The module is attempting to open a TCP Socket
#define WLS_TCPOPENING          -5519
// The module is attached to the network
#define WLS_ATTACHED            -5520
// Module not attached to network, detached
#define WLS_DETTACHED           -5521
// The module is searching for a base station
#define WLS_ATTACHING           -5522
// No communication module board
#define WLS_NOBOARD             -5523
// Invalid module type
#define WLS_UNKNOWNTYPE         -5524
// User name and password registration rejected by the network side, or APN error, card arrears, etc
#define WLS_DIALERR             -5525
// TCP socket fail to send data
#define WLS_TCPSENDERR          -5526
// TCP socket connection timeout
#define WLS_TCPCONNECTOVERTIME  -5527
// Dialing
#define WLS_DIALING             -5528
// Serial port error
#define WLS_PORTERR             -5529
// Serial port occupied
#define WLS_PORTINUSE           -5530
// Error querying wireless module
#define WLS_DETECTERR           -5531
// The input socket number is incorrect
#define WLS_SOCKETOVERRUN       -5532
// Other errors
#define WLS_OTHERR              -5533

#define CACERT                  "A30R_CaCert.cer"
#define CLIENTCERT              "A30R_ClientCert.cer"
#define CLIENTKEY               "A30R_ClientKey.key"
#define CERT_TYPE_CA            0
// client certificate includes the client certificate and key, both of which need to be updated and deleted synchronously
#define CERT_TYPE_CLIENT        1
#define CERT_TYPE_CLIENTKEY     2

// Define the maximum length of each field
#define ELE_MAX_LEN             1024 //
// MCC 3-digit(MCC="204")
#define MCC_MAX_LEN             3
// MNC 2 or 3 digits (mnc="04")
#define MNC_MAX_LEN             3
// APN (apn="live.vodafone.com")
#define APN_MAX_LEN             127
// User (user="vodafone")
#define USER_MAX_LEN            63
// password (password="vodafone")
#define PWD_MAX_LEN             63
// protocol (protocol="IPV4V6")
#define PROT_MAX_LEN            6
// auth type (authtype="1")
#define AUTH_MAX_LEN            1

//socket
#ifndef __SOCKET__
#define __SOCKET__

typedef int SOCKET;
typedef unsigned long in_addr_t;
typedef int socklen_t;

struct in_addr {
  in_addr_t s_addr;
} ;

typedef struct in_addr in_addr;

typedef struct _CFW_TCPIP_SOCKET_ADDR
{
    unsigned char sin_len;
    unsigned char sin_family;
    unsigned short sin_port;
    in_addr sin_addr;
    signed char sin_zero[8];
} CFW_TCPIP_SOCKET_ADDR;

typedef enum
{
    CFW_SIM_0 = 0x00,
    CFW_SIM_1 = 0x01,
    CFW_SIM_END = 0xFF,
    CFW_SIM_ENUM_FILL = 0x7FFFFFFF
} CFW_SIM_ID;

/** This is the aligned version of ip4_addr_t,
   used as local variable, on the stack, etc. */
struct ip4_addr {
  unsigned long addr;
};

typedef struct ip4_addr ip_addr_t;

//add for DNS
typedef enum e_resolv_result
{
    RESOLV_QUERY_INVALID,
    RESOLV_QUERY_QUEUED,
    RESOLV_COMPLETE,
    RESOLV_ENUM_FILL = 0x7fffffff
} RESOLV_RESULT;

#endif

struct net_sockaddr {
#if 1
	unsigned char sin_len;
    unsigned char sin_family;
    unsigned short sin_port;
    in_addr sin_addr;
    signed char sin_zero[8];
#else
	signed short sin_family;                     /* Address family */
	unsigned short sin_port;       					/* Port number */
	BYTE sin_addr[16];              		/* Internet address */
#endif
};

typedef enum
{
    CFW_SOCKET_NONE,
    CFW_SOCKET_CONNECTING,
    CFW_SOCKET_CONNECTED,
    CFW_SOCKET_BEARER_LOSING,
    CFW_SOCKET_CLOSING,
    CFW_SOCKET_COLSED,
    CFW_SOCKET_ERROR,
} socket_status_t;

typedef enum
{
    // IMEI
    ID_GPRS_IMEI,
    ID_LOCAL_IP,
    // IMSI
    ID_GPRS_IMSI,
} E_WLS_INFO_ID;

// MCC: 3 digits, fixed length
// MNC: 2 or 3 digits, variable length
// Define TyPosXML node (node==element in XML file)
typedef struct tagXmlNode
{
    // Mobile Country Code(Mobile Country Code)
    signed char m_ascMCC[MCC_MAX_LEN+1];
    // Mobile Network Code(Mobile NetworkCode)
    signed char m_ascMNC[MNC_MAX_LEN+1];
    // APN access point
    signed char m_ascApn[APN_MAX_LEN+1];
    // APN access point username
    signed char m_ascUser[USER_MAX_LEN+1];
    // APN Access Point Password
    signed char m_ascPwd[PWD_MAX_LEN+1];
    // Authentication method
    unsigned char m_ucAuth;
    // protocol
    unsigned char m_ucProtocol;
    // The corresponding MNC length types for this country are bit0-2 bits and bit1-3 bits
    unsigned char m_ucMncLenType;
    // Valid Flag
    unsigned char m_ucApnValidFlag;
    // Reserved fields
    signed char m_ascReservedFields[128];
}XML_APN_NODE_ST;

typedef enum
{
    NET_SYS_2G  = 2,
    NET_SYS_3G  = 3,
    NET_SYS_4G  = 4,
    NET_SYS_5G  = 5,
    NET_SYS_NA  = 0xFF,
} GPRS_NET_SYS_TYPE;

//-----------------------------------------------------------------------------
// Name    : Lib_WlsInit
// Func    : Initialize GPRS Module
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetInitStatus
// Func    : Get the initialization status
// Paras   : 
// Ret     : return the initialization status, for example,WLS_NOSIM,WLS_OK,
//                       WLS_ATTACHING and so on.
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetInitStatus(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsCheckSignal
// Func    : Get Signal Level
// Paras   : pSignalLevel[in/out],   output the signal level
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsCheckSignal(int *pSignalLevel);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsCheckSim
// Func    : Check the status of SIM card
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsCheckSim(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetInfo
// Func    : Get IMEI,IMSI,IP,ICCID
// Paras   : id[in],         input E_WLS_INFO_ID
//           buff[in/out],   input data buffer and output data information
//           buffSize[in],   input the maximum length of data buffer
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetInfo(E_WLS_INFO_ID id, void *buff, int buffSize);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetBSinfo
// Func    : Get Base Station information
// Paras   : bsInfo[in/out],  input data buffer and output data information
//           len[in],         input the maximum length of data buffer 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetBSinfo(unsigned char *bsInfo, unsigned long len);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetDns
// Func    : Set DNS
// Paras   : pucMainDns[in],  input Main DNS
//           pucBackDns[in],  input Backup DNS
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetDns(unsigned char *pucMainDns, unsigned char *pucBackDns);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetDns
// Func    : Get DNS
// Paras   : pscMainDnsBuf[in/out],  input Main DNS data buffer and output Main DNS
//           ulMainDnsBufLen[in],    input the maximum length of Main DNS data buffer
//           pcBackDnsBuf[in/out],   input Backup DNS data buffer and output Backup DNS
//           ulBackDnsBufLen[in],    input the maximum length of Backup DNS data buffer
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetDns(char *pscMainDnsBuf, unsigned long ulMainDnsBufLen, char *pcBackDnsBuf, unsigned long ulBackDnsBufLen);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsDnsResolve
// Func    : Resolve DNS
// Paras   : name[in],       input domain name
// Paras   : result[in/out], input data buffer and output the result
// Paras   : len[in],        input the maximum length of data buffer
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsDnsResolve(char *name,char *result,int len);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetApn
// Func    : Get APN
// Paras   : pscApn[in],   input data buffer and output APN
//           ulApnLen[in], input the maximum length of data buffer
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetApn(char *pscApn, unsigned long ulApnLen);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetApn
// Func    : Set APN 
// Paras   : pscUserName[in],   input UserName
//           pscPassword[in],   input Password
//           ucType,            input protocol type, 1:ip 2:ipv6 3:ipv4v6
//           ucAuthProt,        input authentication prototype,
//                                  0:(NONE) 1:plaintext 2:Cipher
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetApn(char *pscApn, char *pscUserName, char *pscPassword, unsigned char ucType, unsigned char ucAuthProt);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetAdaptiveAPN
// Func    : Open or Close APN Adaptive
// Paras   : value[in],   input 1: open,0: close
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetAdaptiveAPN(unsigned char value);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetAPNList
// Func    : Query APN node information
// Paras   : ptrApnNum[in/out],   input data buffer and output APN number
// Ret     : return APN List
// Desc    :
//-----------------------------------------------------------------------------
XML_APN_NODE_ST* Lib_WlsGetAPNList(unsigned long *ptrApnNum);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetApnAuth
// Func    : Get APN 
// Paras   : pucType[in],     input protocol type, 1:ip 2:ipv6 3:ipv4v6
//           pucAuthProt[in], input authentication prototype,
//                                  0:(NONE) 1:plaintext 2:Cipher
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetApnAuth(unsigned char *pucType, unsigned char *pucAuthProt);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsPing
// Func    : Ping external network
// Paras   : dstIpStr[in],  target IP address string, length must not exceed 15 bytes
//                          e.g. "192.168.1.1"
//           timeout[in],   timeout for each ping, in seconds, range: 1-255
//           size[in],      test data length, range: 36-1500
//           pingTimes[in], number of ping attempts, range: 1-65535
// Ret     : Number of successful pings, range: 0 ~ pingTimes
//           0 means no reply, pingTimes means all succeeded
// Desc    : Sends ICMP echo requests to the target IP address and returns
//           the number of successful replies.
//-----------------------------------------------------------------------------
int Lib_WlsPing(char *dstIpStr, long timeout, int size, int pingTimes);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetNetMode
// Func    : Get network mode
// Paras   : 
// Ret     : return network mode,GPRS_NET_SYS_TYPE
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetNetMode(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetNetMode
// Func    : Close file
// Paras   : netMode[in],   input network mode,refer to GPRS_NET_SYS_TYPE
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetNetMode(GPRS_NET_SYS_TYPE netMode);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetAirplaneMode
// Func    : Open or Close Airplane Mode
// Paras   : value[in],   input 1: Open Airplane Mode,0:Close Airplane Mode
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetAirplaneMode(unsigned char value);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetOperatorList
// Func    : Get Operator List
// Paras   : operatorList[in/out],  input data buffer and output Operator List
//           length[in],            input maximum length of data buffer
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetOperatorList(char *operatorList, unsigned int length);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsSetOperator
// Func    : Set Operator
// Paras   : mode[in],     input mode
//           operator[in], input operator
//           act[in],      input act
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsSetOperator(unsigned char mode, char *operator, unsigned int act);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsGetOperator
// Func    : Get Operator
// Paras   : operator[in],   input operator
//           length[in],     input length of operator
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WlsGetOperator(char *operator, unsigned int length);

//-----------------------------------------------------------------------------
// Name    : Lib_WlsNetActRefresh
// Func    : Get 4G Network Mode
// Paras   : pucNetAct[in/out],   input data buffer and output 4G Network Mode
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_WlsNetActRefresh(unsigned char *pucNetAct);

#endif