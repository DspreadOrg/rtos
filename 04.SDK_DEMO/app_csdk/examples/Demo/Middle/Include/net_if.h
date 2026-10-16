/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : net_if.h
 *  Version      :
 *  Author       :
 * Description   : Encapsulating GPRS and WIFi network communication APIs
 **********************************************************************************************************/

#ifndef _NET_IF_H
#define _NET_IF_H

#include "LibTypedef.h"
#include "LibNetwork.h"


//-----------------------------------------------------------------------------
// Macro definition
//-----------------------------------------------------------------------------

// Socket parameters
#define  SOL_SOCKET  	0xfff    /* options for socket level */

#define SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_SNDBUF       0x1001 /* Unimplemented: send buffer size */
#define SO_RCVBUF       0x1002 /* receive buffer size */
#define SO_SNDLOWAT     0x1003 /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT     0x1004 /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO     0x1005 /* send timeout */
#define SO_RCVTIMEO     0x1006 /* receive timeout */
#define SO_ERROR        0x1007 /* get error status and clear */
#define SO_TYPE         0x1008 /* get socket type */
#define SO_CONTIMEO     0x1009 /* Unimplemented: connect timeout */
#define SO_NO_CHECK     0x100a /* don't create UDP checksum */
#define SO_BINDTODEVICE 0x100b /* bind to device */


// Only used for old APIs, single channel network communication
// Establishing TCP connection
#define NET_MODE_TCP    0
// Establishing SSL connection
#define NET_MODE_SSL    1

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

void Net_Init(void);
int Net_OpenCb(int timeout, int ct);
int Net_ConnectCb(int timeout, int ct);
int Net_ReadCb(int timeout, int ct);
int Net_WriteCb(void);
int Net_Status(int fd, uint8_t flag);

short Net_GetSocketMode(void);
short Net_SetSocketMode(unsigned char mode);

//===================================================================================================================================
// The following are the APIS of netif for multi-channel communication functions
// It is recommended to use this new APIs, and the old Apis is not recommended to continue to be used.
// Update date: 2024.04.29
//===================================================================================================================================


//-----------------------------------------------------------------------------
// Name    : Net_OpenEx
// Func    : Create a socket number
// Paras   : handle    : Used to record the socket number of new connections
//           protocol  : The network protocol type of the new link
//           timeout   : Timeout value,in milliseconds
//           Call      : a callback function for Creating socket connections
// Ret     : RET_OK : Created successfully, Other, Created failed
// Desc    :
//-----------------------------------------------------------------------------
int Net_OpenEx(int* handle, NETWORK_TYPE protocol, int timeout, int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : Net_ConnectEx
// Func    : Establish a network connection with specified socket number
// Paras   : handle    : Network socket handle,
//           server    : server address
//           port      : server port
//           timeout   : network connection timeout
//           Call      : Callback function for network connection
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ConnectEx(int      handle, char *server,  uint16_t port, int timeout, int (*Call)(int timeout, int ct));



//-----------------------------------------------------------------------------
// Name    : Net_Status
// Func    : Query socket status
// Paras   : fd        : Network socket handle
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_StatusEx(int handle);

//-----------------------------------------------------------------------------
// Name    : Net_ReadEx
// Func    : Network receive data with specified socket number
// Paras   : handle    : Network socket handle,
//           buff      : Data receiving cache buffer
//           buff_size : Data receiving cache buffer size
//           timeout   : Data receiving timeout
//           Call      : Callback function for network receiving data
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ReadEx(int handle, uint8_t *buff, int buff_size, int timeout, int (*Call)(int timeout, int ct));

//-----------------------------------------------------------------------------
// Name    : Net_WriteEx
// Func    : Network send data with specified socket number
// Paras   : handle    : Network socket handle,
//           buff      : sending data cache buffer
//           len       : sending data length
//           Call      : Callback function for network receiving data
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_WriteEx(int handle, uint8_t *buff, int len, int (*Call)(void));

//-----------------------------------------------------------------------------
// Name    : Net_CloseEx
// Func    : Close the specified socket connection
// Paras   : handle    : Network socket handle,
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_CloseEx(int handle);

void Net_StartChannelThread(void *param);

#endif

