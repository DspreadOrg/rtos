//-----------------------------------------------------------------------------
// @file    : LibWifi.h
// @brief   : Wifi related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_WIFI_H__
#define __LIB_WIFI_H__
#include "LibTypedef.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#define STA_AP_CONNECTING               (0)
#define STA_AP_CONNECTED                (1)
#define STA_AP_DISCONNECTED             (2)

#define WIFI_SET_NET_TIMEOUT        (1000*60)   // WIFI Connection Timeout

// Current status of WIFI
typedef enum
{
    // Initialization status
    WifiInitSta,
    // Idle state
    WifiIdleSta,
    // AP connected status
    WifiLinkApSta,
    // TCP Connected Status
    WifiLinkTcpSta,
    // TCP transparent status
    WifiLinkTcpUtSta,
    // SMARTCONFIG status
    WifiSmartConfig,
    // Module error status
    WifiFailure,
}WIFI_STATUS;

typedef struct WIFI_INF
{
    unsigned char  m_ausSSID[50];       // Access Point name
    unsigned char  m_aucMode[8];        // Mode: With "Managed" and "auto"
    unsigned char  m_aucEncryptType;    // Encryption type, "on" with encryption, "off" without encryption
    unsigned char  m_aucPass[50] ;      // The password used to connect to the AP
    signed long  m_aucSignalLevel;      // signal strength
}WIFI_INF;

typedef enum
{
    WIFI_PARA_SSID,
    WIFI_PARA_PWD,
    WIFI_PARA_REMOTE_IP,
    WIFI_PARA_REMOTE_PORT,
    WIFI_PARA_LOCAL_PORT,
    WIFI_PARA_TCP_MODE,
    WIFI_PARA_LOCAL_IP,
    WIFI_PARA_NET_GATE,
    WIFI_PARA_NET_MASK,
    WIFI_PARA_LOCAL_MAC,
    WIFI_PARA_SSID_MAC,
    WIFI_PARA_SSL_FLAG,
    WIFI_PARA_FWVER,
    WIFI_PARA_DEL_SSID,
    WIFI_PARA_NET_DNS1,
    WIFI_PARA_NET_DNS2,
} WIFI_PARA_TYPE;

typedef enum
{
    NET_WIFI_INFO_VER = 19,             // Firmware version
    NET_WIFI_INFO_MAC,                  // MAC Address
    NET_WIFI_INFO_CSQ,                  // Signal strength
    NET_WIFI_LOCAL_IP,                  // local ip address
    NET_WIFI_PARA_SSID,                 // WIFI Connected SSID Name
    NET_WIFI_PARA_PWD,                  // WIFI Connected SSID Password
    NET_WIFI_CONNECT_STATUS,
    NET_WIFI_SERVER_CH,
    NET_WIFI_CLIENT_CH,
    NET_WIFI_UDP_LPORT,
    NET_WIFI_QUERY_SSID,
    NET_WIFI_DNS_CLEAR,
    NET_WIFI_IP_CONFIG,

	NET_WIFI_CLEAR_AP_INFO = 100,       // Clear AP information saved in WIFI
    NET_WIFI_SETTING_DEL_SSID,          // Delete SSID record
    NET_WIFI_SETTING_STATIC_IP,         // WIFI static IP settings
    NET_WIFI_SETTING_STATIC_DNS,        // WIFI static DNS
    NET_WIFI_GET_AP_LIST,
} NET_INFO;

//-----------------------------------------------------------------------------
// Name    : Lib_WifiNetworkUp
// Func    : Open Wifi communication
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiNetworkUp(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiNetworkDown
// Func    : Close wifi communication
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiNetworkDown(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiNetworkScan
// Func    : Wifi network scanning for getting nearby access points
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiNetworkScan(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiGetAP
// Func    : Get access point information
// Paras   : pstWifiInf[in/out],  output WIFI_INF Access point information
//           pusNum[in/out],      output total number of nearby access points
//           usMax[in],           input maximum number of all access points 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiGetAP(WIFI_INF *pstWifiInf, unsigned short *pusNum, unsigned short usMax);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiConnectAp
// Func    : Connect to access point
// Paras   : pucAuthMode[in],     input authorization mode,include OPEN,SHARED,WEPAUTO,
//                                WPAPSK,WPA2PSK,WPA,NONE.
//           pucEncryptType[in],  input encryption parameters,include NONE,WEP,TKIP,AES
//           pscSSID[in],         input username
//           pucPwd[in],          input password
// Ret     : 0:wpa connection success,1: connection success without password,
//           2:wep conncetion success,others: fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiConnectAp(unsigned char *pucAuthMode, unsigned char *pucEncryptType, char *pscSSID, unsigned char *pucPwd);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiDisAp
// Func    : DisConnect to access point
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiDisAp(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiGetApStatus
// Func    : Get status of current access point
// Paras   : 
// Ret     : return status,0:STA_AP_CONNECTING,1:STA_AP_CONNECTED,2:STA_AP_DISCONNECTED
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiGetApStatus(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiCheckSignal
// Func    : Get Wifi Signal level
// Paras   : pSignalLevel[in/out],  output wifi signal level
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiCheckSignal(int *pSignalLevel);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiSetAutoConnectFlag
// Func    : Set up automatic Wifi connection
// Paras   : flag[in],  input 1:automatic connection,0:not automatic connection
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiSetAutoConnectFlag(unsigned char flag);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiGetLinkAp
// Func    : Get currently connected access point information
// Paras   : paraData[in/out],  output data
//           paraLen[in/out],   output data length
//           paraType[in],      input WIFI_PARA_TYPE
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiGetLinkAp(unsigned char* paraData, unsigned short *paraLen, WIFI_PARA_TYPE paraType);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiGetInfo
// Func    : Get Wifi Parameters
// Paras   : eType[in],       input NET_INFO
//           pucStr[in/out],  output wifi parameters
//           pusLen[in],      output wifi parameters length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiGetInfo(NET_INFO eType, unsigned char *pucStr, unsigned short *pusLen);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiSetInfo
// Func    : Set Wifi Parameters
// Paras   : eType[in],   input NET_INFO
//           pucStr[in],  input data 
//           usLen[in],   input data length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiSetInfo(NET_INFO eType, unsigned char *pucStr, unsigned short usLen);

//-----------------------------------------------------------------------------
// Name    : Lib_WifiGetVersion
// Func    : Get wifi module version
// Paras   : ver[in/out],  output version
//           len[in/out],  output version length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_WifiGetVersion(char *ver, unsigned short *len);

#endif