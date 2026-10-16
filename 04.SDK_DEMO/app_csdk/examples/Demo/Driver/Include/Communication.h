/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Communication.h
 *  Version      :
 *  Author       :
 * Description   : Implement network communication related functions
 **********************************************************************************************************/

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "LibApi.h"
#include "Drv.h"
#include "LibNetwork.h"

//---------------------------------------------------------------------------
// macro definition
//---------------------------------------------------------------------------

// Default timeout of communication
// Module maximum initialization time (milliseconds)
#define NET_MAX_INIT_TIME          (180*1000)
// TCP link timeout (milliseconds)
#define NET_MAX_TCPLINK_TIME       (15*1000)
// Data transmission timeout (milliseconds)
#define NET_MAX_TX_TIME            (20*1000)
// Data receiving timeout (milliseconds)
#define NET_MAX_RX_TIME            (15*1000)
// WiFi exception, switching GPRS time (milliseconds)
#define NET_WIFI2GPRS_TIME         (10*1000)
// SSL connection timeout (milliseconds)
#define NET_MAX_SSLLINK_TIME       (15*1000)


#define SINGLE_RECV_LEN             1024

// Maximum number of communication channels supported
// WiFi only supports 4 channels
#define NET_MULTI_CHN_MAX           5


//---------------------------------------------------------------------------
// Function declaration
//---------------------------------------------------------------------------

void PubLongToChar(unsigned long ulDecData, int iHexlen, char *psHexData);

short UIWireNetInit(void);
unsigned char GetNetWorkMode(void);
short SetNetWorkMode(unsigned char ucMode);

short GetServerIpPort(EN_SERVER_OPT stServerOpt,char *pucOutIp, unsigned short *pusOutPort);
void SetServerOpt(EN_SERVER_OPT serverOpt);
EN_SERVER_OPT GetServerOpt(void);

short UIWireHttpRecv(int handle, unsigned char *pucBuf, unsigned long *pulLen, int (*Call)(int timeout, int ct));
short UIWireHttpSend(int handle, char *pUrl, unsigned char *pucBuf, unsigned short usLen, char *pucExtHeader, int (*Call)(void));


//===================================================================================================================================
// The following are the APIS for multi-channel communication functions in UI level.
// It is recommended to use this new APIs, and the old Apis is not recommended to continue to be used.
// Update date: April 29, 2024
//===================================================================================================================================


//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpLinkExt
// Func    : Create TCP network connection function
// Paras   : handle    : Used to record the socket number of new connections
//           protocol  : The network protocol type of the new link
//           timeout   : Timeout value,in milliseconds
//           stSerOpt  : Connected business server number
//           Call      : The callback function for connection, refer to Net_ConnectCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpLinkExt(int* handle, NETWORK_TYPE protocol, int timeout, EN_SERVER_OPT stServerOpt, int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpTxExt
// Func    : Network TCP send data function
// Paras   : handle    : Network socket number
//           data      : Cache buffer for sending data;
//           data_len  : The length of the sending data
//           timeout   : send data timeout value,in milliseconds
//           Call      : The callback function for sending data, refer to Net_WriteCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpTxExt(int handle, uint8_t *data, uint16_t data_len, int timeout, int (*Call)(void));


//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpRxExt
// Func    : Network TCP receive data extend function with specified socket number
// Paras   : handle    : Network socket number
//           recv      : Cache buffer for storing data;
//           recv_size : The maximum length of the receiving cache buffer
//           timeout   : Receive timeout value,in milliseconds
//           PacketType: Format of data packets received from network
//           Call      : The callback function for receiving, refer to Net_ReadCb
// Ret     : 0: Received successfully, received all data, or received maximum buffer length
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpRxExt(int handle, uint8_t *recv, unsigned long *recv_size, int timeout, uint8_t PacketType, int (*Call)(int timeout, int ct));


//-----------------------------------------------------------------------------
// Name    : UIWireNetCloseExt
// Func    : Close the connection with the specified serial number
// Paras   : handle   : Network socket number
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetCloseExt(int *piHandle);


#endif //__COMMUNICATION_H__

