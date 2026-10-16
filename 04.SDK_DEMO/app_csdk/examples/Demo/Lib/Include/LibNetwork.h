//-----------------------------------------------------------------------------
// @file    : LibNetwork.h
// @brief   : Network communication related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_NETWORK_H__
#define __LIB_NETWORK_H__

#include "LibGprs.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// Invalid handle value
#define NET_INVALID_HANDLE                  (-1)

typedef enum
{
    NET_MODE_GPRS = 0,       // GPRS 
    NET_MODE_WIFI ,          // WIFI 
    NET_MODE_WIFI_PRIOR,     // Priority Wifi
}NET_MODE;

typedef enum
{
    NETWORK_TCP = 0,
    NETWORK_UDP = 1,
    NETWORK_TLS = 2,
    NETWORK_DTLS = 3
} NETWORK_TYPE;

typedef enum
{
    TLSSOCK_CFG_TYPE_VERSION,
    TLSSOCK_CFG_TYPE_AUTHMODE,
    TLSSOCK_CFG_TYPE_CIPHER_SUITE,
    TLSSOCK_CFG_TYPE_CA,
    TLSSOCK_CFG_TYPE_CLI_CERT,
    TLSSOCK_CFG_TYPE_CLI_KEY,
    TLSSOCK_CFG_TYPE_IGNORE_RTC_TIME,
    TLSSOCK_CFG_TYPE_HTTPS,
    TLSSOCK_CFG_TYPE_HTTPS_CTXI,
    TLSSOCK_CFG_TYPE_SMTPS_TYPE,
    TLSSOCK_CFG_TYPE_SMTPS_CTXI,
    TLSSOCK_CFG_TYPE_SNI,
} SOCK_CFG_TYPE_E;

typedef enum
{
    PACKET_TYPE_ISO8583,                        // ISO8583 message format
    PACKET_TYPE_RAW_DATA,                       // Network raw data format

    PACKET_TYPE_RESERVE = 255,                  // reserved
}NT_PACKET_TYPE;

typedef enum
{
    SERVER_OPT_POSP_MAIN,                        //POSP primary server
    SERVER_OPT_POSP_BACKUP,                      //POSP standby server
    SERVER_OPT_TMS_MAIN,                         //TMS primary server
    SERVER_OPT_TMS_BACKUP,                       //TMS standby server
    SERVER_OPT_TMS_MODULE,                       //TMS Module server
    // The newly added business server must be placed behind the TMS server

    // reserved
    SERVER_OPT_RESERVE = 255,                    // reserved
}EN_SERVER_OPT;

//-----------------------------------------------------------------------------
// Name    : Lib_NetSocket
// Func    : Request resources for socket network communication
// Paras   : domain[in],    input domain
//           type[in],      input type
//           protocol[in]:  input NETWORK_TYPE
// Ret     : return handle
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetSocket(int domain, int type, NETWORK_TYPE protocol);

//-----------------------------------------------------------------------------
// Name    : Lib_NetConnect
// Func    : Socket Connection
// Paras   : handle[in],    input Socket Number
//           host[in],  input host
//           port[in]:  input port
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetConnect(int handle, const char *host, unsigned short port);

//-----------------------------------------------------------------------------
// Name    : Lib_NetSend
// Func    : Send data over the network
// Paras   : handle[in],      input Socket Number
//           data[in],    input data buffer
//           dataLen[in]: input length of data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetSend(int handle, const void *data, int dataLen);

//-----------------------------------------------------------------------------
// Name    : Lib_NetRecv
// Func    : Receive data from network
// Paras   : handle[in],      input Socket Number
//           buff[in],    input data buffer and output data received
//           buffSize[in]: input maximum length of data buffer
// Ret     : return length of datas received. 
//             Length less than 1 is fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetRecv(int handle, void *buff, int buffSize);

//-----------------------------------------------------------------------------
// Name    : Lib_NetClose
// Func    : close network
// Paras   : handle[in],      input Socket Number
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_NetClose(int handle);

//-----------------------------------------------------------------------------
// Name    : Lib_NetGetStatus
// Func    : Query network status
// Paras   : handle[in],      input Socket Number
// Ret     : return socket status,for example,CFW_SOCKET_CONNECTING ,
//                  CFW_SOCKET_CONNECTED and so on
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetGetStatus(int handle);

//-----------------------------------------------------------------------------
// Name    : Lib_NetSetCfg
// Func    : Configure network communication related parameters
// Paras   : handle[in],      input Socket Number
//           type[in],    input SOCK_CFG_TYPE_E
//           param[in],   input param
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetSetCfg(int handle, SOCK_CFG_TYPE_E type, unsigned long param);

//-----------------------------------------------------------------------------
// Name    : Lib_NetDns
// Func    : Parse DNS
// Paras   : name[in],          input domain name
//           result[in/out],    output result
//           pusIpLen[in/out],  output result length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetDns(char *name,char *result, unsigned short *pusIpLen);

//-----------------------------------------------------------------------------
// Name    : Lib_NetOpenEx
// Func    : Create a socket number
// Paras   : handle[in/out],  output socket number of new connections
//           protocol[in],    input network protocol
//           timeout[in],     input timeout,unit is millisecond
//           Call[in],        input a callback function for Creating socket 
//                            connections
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetOpenEx(int *handle, NETWORK_TYPE protocol, int timeout, int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : Lib_NetConnectEx
// Func    : Socket Connection
// Paras   : handle[in],    input Network socket handle
//           server[in],    input server address
//           port[in],      input server port
//           timeout[in],   input timeout,unit is millisecond
//           Call[in],      input a callback function 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetConnectEx(int handle, char *server, unsigned short port,int timeout,int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : Lib_NetStatusEx
// Func    : Query network status
// Paras   : handle[in],      input socket handle
// Ret     : return socket status,for example,CFW_SOCKET_CONNECTING ,
//                  CFW_SOCKET_CONNECTED and so on
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetStatusEx(int handle);

//-----------------------------------------------------------------------------
// Name    : Lib_NetReadEx
// Func    : Network receive data with specified socket number
// Paras   : handle[in],    input Network socket handle
//           buff[in/out],  input data buffer and output data
//           buff_size[in], input maximum length of data buffer
//           timeout[in],   input timeout,unit is millisecond
//           Call[in],      input a callback function for network receiving data
// Ret     : return length of datas received. 
//             Length less than 1 is fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetReadEx(int handle, unsigned char *buff, int buff_size, int timeout, int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : Lib_NetWriteEx
// Func    : Network send data with specified socket number
// Paras   : handle[in],    input Network socket handle
//           buff[in],      input data buffer
//           len[in],       input data length
//           Call[in],      input a callback function for network sending data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_NetWriteEx(int handle, unsigned char *buff, int len, int (*Call)(void));

//-----------------------------------------------------------------------------
// Name    : Lib_NetCloseEx
// Func    : Close Socket connection
// Paras   : handle[in],    input Network socket handle
// Ret     : 0 - success, others - fail
// Desc    : after calling this function, handle needs to be explicitly configured 
//           as invalid.
//-----------------------------------------------------------------------------
int Lib_NetCloseEx(int handle);

//-----------------------------------------------------------------------------
// Name    : Lib_NetResetEx
// Func    : Network reset
// Paras   : timeout[in],  input Timeout,unit is millisecond
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_NetResetEx(int timeout);

#endif