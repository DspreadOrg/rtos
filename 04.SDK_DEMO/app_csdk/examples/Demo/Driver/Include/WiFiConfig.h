/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : WifiConfig.h
 *  Version      :
 *  Author       :
 * Description   : Implement WiFi management related functions
 **********************************************************************************************************/


#ifndef __WIFI_CONFIG_H
#define __WIFI_CONFIG_H

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

#define uiWiFiConnet                8
#define uiWiFiDisConnet             9
#define uiWiFiConneting             10

#define WIFI_PWD_MAX_LEN            (32)        // PWD maximum length
#define WIFI_SSID_MAX_LEN           (32)        // SSID maximum length
#define WIFI_SET_NET_TIMEOUT        (1000*60)   // WIFI connection AP timeout
#define WIFI_INPUT_TIMEOUT          (1000*60)   // Set input timeout time for WIFI
#define WIFI_NET_LIST_MAX_SIZE      (40)        // Maximum number of WIFI AP lists
#define DISP_NET_NUMS               (5)         // The maximum number of rows per page when displaying the WIFI list
#define SSID_MAX_LEN                (14)        // The maximum number of characters for SSID displayed on one line
#define WIFI_APINFO_LENGTH_POS      (0)         // WIFI AP list information length offset
#define WIFI_APINFO_ENC_POS         (1)         // WIFI AP List Information Encryption Method Offset
#define WIFI_APINFO_RSSI_POS        (2)         // WIFI AP list information signal strength offset
#define WIFI_APINFO_SSID_POS        (3)         // WIFI AP List Information SSID Offset

#define QRCODE_WIFI_INFO_STR_A      "WIFI"
#define QRCODE_WIFI_INFO_STR_B      "P:"
#define QRCODE_WIFI_INFO_STR_C      "S:"
#define QRCODE_WIFI_INFO_STR_D      ";"



// Default DNS for dynamic IP
#define WIFI_DNS1_DEF		"114.114.114.114"
#define WIFI_DNS2_DEF		"223.5.5.5"


typedef enum
{
    AUTO_CONN_DIS = 0,
    AUTO_CONN_EN = 1,
}WIFI_AUTO_CONN_YTPE;


// WiFi configuration information
typedef struct
{
	unsigned char SsidName[34];         // SSID Name
	unsigned char PassWordBuf[64];      // SSID password
	unsigned char SsidMacAdd[18];       // SSID MAC address, ASCII code

	unsigned char StaticIpFlag;         // Is static IP tagging enabled
	unsigned char LocalIpAdd[16];       // local ip address
	unsigned char LocalGateway[16];     // local gateway
	unsigned char LocalSubnetMask[16];  // Local subnet mask
	unsigned char DNS1[16];
	unsigned char DNS2[16];
	unsigned char MacAdd[18];           // Local MAC address

	unsigned char Rev[64];              // Reserve 64 bytes to avoid affecting old sub applications after future modifications
} WIFI_NET_INFO;


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

short UINetCfgMenu(int iTimeout);
short UIWifiNetList(int iTimeout);


#endif

