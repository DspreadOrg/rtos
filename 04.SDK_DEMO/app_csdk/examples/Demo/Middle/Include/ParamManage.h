/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : ParamManage.h
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : Parameter management function
 **********************************************************************************************************/
#ifndef __PARAM_MANAGE_H__
#define __PARAM_MANAGE_H__

#include "LibApi.h"
#include "LibSysapi.h"


//---------------------------------------------------------------------------
//macro definition
//---------------------------------------------------------------------------
// Terminal configuration information file
#define TEMIMAL_PARAM_FILE          "Terminalparam/terminal_param.bin"
// Server Information File
#define SERVER_PARAM_FILE           "server_param.bin"
// Color information file
#define COLOR_PARAM_FILE            "color_param.bin"
// Language information file
#define LANGUAGE_PARAM_FILE         "language_param.bin"
// TMS server files
#define TMS_SERVER_PARAM_FILE       "tms_server_param.bin"

//---------------------------------------------------------------------------
// Terminal parameter bitmap definition:
//---------------------------------------------------------------------------

// Terminal number (8 digits)
#define TERMINAL_NO_BIT                 1
#define TERMINAL_NO_SIZE                8


// Network working mode
#define NET_WORK_MODE_BIT               2
// Network working mode parameter length
#define NET_WORK_MODE_SIZE              1

 // Voice Volume
#define BRIGHT_LEVEL_BIT                3
#define BRIGHT_LEVEL_SIZE               1
#define BRIGHT_LEVEL_MIN                0
#define BRIGHT_LEVEL_MAX                8
#define BRIGHT_LEVEL_DEF                5

//beep
#define BEEP_BIT                        4

//Print
#define PRINT_GRADE_MAX                 5

//ssid
#define WIFI_SSID_BIT                   5

#define WIFI_PASS_BIT                   6

#define TIME_ZONE_MIN                   (-48)
#define TIME_ZONE_MAX                   (48)
#define TIME_ZONE_BIT                   7

#define NTP_SERVER_BIT                  8

#define NTP_SYNC_SWITCH_BIT             9

#define NTP_SYNC_INTER_HOUR_MIN         (1)
#define NTP_SYNC_INTER_HOUR_MAX         (24 * 10)
#define NTP_SYNC_INTER_BIT              10

// tms 1IP
#define TMS1IP_BIT                      18
// tms 1 port
#define TMS1PORT_BIT                    19
// tms 2IP
#define TMS2IP_BIT                      20
// tms 2 port
#define TMS2PORT_BIT                    21
//
// Host1 IP
#define HOST1IP_BIT                     40
// Host1 port
#define HOST1PORT_BIT                   41
// Host2 IP
#define HOST2IP_BIT                     42
// Host2 port
#define HOST2PORT_BIT                   43

// The software version number issued by TMS
#define TMS_APP_VER_BIT                 76
// The parameter version number issued by TMS
#define TMS_PARA_VER_BIT                77

// POSP primary domain name & port
#define NET_NAME_POSP_BIT               86
#define NET_NAME_POSP_PORT_BIT          87
// POSP backup domain name & port
#define NET_NAME_POSP_2_BIT             88
#define NET_NAME_POSP_PORT_2_BIT        89

// TMS primary domain name & port
#define NET_NAME_TMS_BIT                91
#define NET_NAME_TMS_PORT_BIT           92
// TMS backup domain name & port
#define NET_NAME_TMS_2_BIT              93
#define NET_NAME_TMS_PORT_2_BIT         94

// TMS module primary domain name & port
#define NET_NAME_MODULE_BIT             96
#define NET_NAME_MODULE_PORT_BIT        97


// POSP server access method, '1' is domain name, '0' is IP
#define POSP_SERVER_MODE_BIT            90
// TMS server access method, '1' is domain name, '0' is IP
#define TMS_SERVER_MODE_BIT             95

// Network port,CHAR type
#define SERVER_MODE_SIZE                1
#define NETWORK_PORT_SIZE               5
#define NETWORK_ADDR_SIZE               32

// The software version number szie issued by TMS
#define TMS_APP_VER_SIZE                20
// The parameter version number szie issued by TMS
#define TMS_PARA_VER_SIZE               20

//-----------------------------------------------------------------------------
// variable declaration
//-----------------------------------------------------------------------------
// Transaction records
typedef struct
{
    unsigned char aucTerminalNo[TERMINAL_NO_SIZE];      // Terminal number
    unsigned char ucNetWorkMode;                        // Network mode
    unsigned char ucBrightLevel;                        // Brightness level
    unsigned char ssid[14];                             // WiFi SSID
    unsigned char pass[32];                             // WiFi PWD
    unsigned char beep_en;                              // Whether enable beep
    int timeZone;                                       // time zone
    unsigned char ntpDomain[NETWORK_ADDR_SIZE];         // NTP server domain name
    unsigned char timeSyncSwitch;                       // Automatic time synchronization switch, 0 : off, 1 : on
    unsigned char SyncInterHour;                        // The interval of automatic time synchronization, in days, can be set from 1-240 days
    unsigned char aucManagePwd[8+1];                    // Manage Pwd

    unsigned char ucNetSimNo;                           // SIM card Index No
}ST_TEMINAL_PARAM;

typedef struct
{
    unsigned char ucUseDomainNameEn;                    // whether use domain names or not,' 0 'and' 1 '
    unsigned char aucIP1[15];                           // Host1 IP
    unsigned char aucIP2[15];                           // Host2 IP
    unsigned char aucPort1[NETWORK_PORT_SIZE+1];        // Host1 port, save '\0'
    unsigned char aucPort2[NETWORK_PORT_SIZE+1];        // Host2 Port
    unsigned char aucDomain1[NETWORK_ADDR_SIZE];        // Domain Name 1
    unsigned char aucDomain2[NETWORK_ADDR_SIZE];        // Domain Name 2
    unsigned char aucDomainPort1[NETWORK_PORT_SIZE+1];  // Domain Name 1 port
    unsigned char aucDomainPort2[NETWORK_PORT_SIZE+1];  // Domain Name 2 port
    //unsigned char reserved;                           // Reserve Bytes
}ST_SERVER_PARAM;

typedef struct
{
    unsigned char aucDomain1[NETWORK_ADDR_SIZE];        // Domain Name 1
    unsigned char aucDomainPort1[NETWORK_PORT_SIZE+1];  // Domain Name 1 port
}ST_MODULE_SERVER_PARAM;

typedef struct
{
    unsigned char aucModuleDomain1[NETWORK_ADDR_SIZE];        //Domain name 1
    unsigned char aucModuleDomainPort1[NETWORK_PORT_SIZE+1];  //Domain name 1port
    unsigned char aucModuleDomain2[NETWORK_ADDR_SIZE];        //Domain name 2
    unsigned char aucModuleDomainPort2[NETWORK_PORT_SIZE+1];  //Domain name 2 port
    //unsigned char reserved;                                 //Reserved
}ST_TMSMODULE_PARAM;

typedef struct
{
	ST_SERVER_PARAM stTmsServerParam;                 // Firmware download server
	ST_TMSMODULE_PARAM stTmsModuleParam;              // Module download server
} ST_TMS_PARAM;



typedef struct
{
    unsigned char newAppVer[TMS_APP_VER_SIZE];
    unsigned char newParaVer[TMS_PARA_VER_SIZE];
}ST_TMS_VER_RECORD;

typedef enum
{
    MAIN_FORE_COLOR_ID,
    MAIN_BACK_COLOR_ID,
    MENU1_FORE_COLOR_ID,
    MENU1_BACK_COLOR_ID,
    MENU2_FORE_COLOR_ID,
    MENU2_BACK_COLOR_ID,
    TIPS_FORE_COLOR_ID,
    TIPS_BACK_COLOR_ID,
}E_ID_COLOR;

typedef enum
{
    ID_TMS_VER_RECORD,
    ID_TMS_VER_INFO,
    ID_TMS_DOWNLOAD_INFO,
    ID_TMS_MODULE_VER_INFO,
    ID_TMS_MODULE_DOWNLOAD_INFO,
    ID_TMS_MODULE_SREVER_INFO,
}E_ID_TMS_FILE;

typedef struct
{
    unsigned short mainForeColor;                // Main Window Foreground Color
    unsigned short mainBackColor;                // Main window background color
    unsigned short menu1ForeColor;               // Menu 1 foreground color
    unsigned short menu1BackColor;               // Menu 1 Background Color
    unsigned short menu2ForeColor;               // Menu 2 foreground color
    unsigned short menu2BackColor;               // Menu 2 Background Color
    unsigned short tipsForeColor;                // Text foreground color
    unsigned short tipsBackColor;                // Text background color
}ST_COLOR_PARAM;

//-----------------------------------------------------------------------------
// variable declaration
//-----------------------------------------------------------------------------

extern ST_TEMINAL_PARAM g_teminal_param;
extern ST_SERVER_PARAM g_stServerParam ;
extern ST_TMS_PARAM  g_stTmsParam;

// get the offset position of someone element in the structure
#if defined ( __CC_ARM )
#define GET_STRUCT_OFFSET(stName, stMem)     ((unsigned int)&(((stName *)0)->##stMem))
#define GET_STRUCT_SIZE(stName, stMem)       (sizeof(((stName *)0)->##stMem))
#else
#define GET_STRUCT_OFFSET(stName, stMem)     ((unsigned int)&(((stName *)0)->stMem))
#define GET_STRUCT_SIZE(stName, stMem)       (sizeof(((stName *)0)->stMem))
#endif

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

short TerminalParamGet(unsigned char ucParamId, void *para, unsigned char *paraSize);
short TerminalParamSet(unsigned char ucParamId, void *para, unsigned char paraSize);
short ServerParamGet(unsigned char ucParamId, unsigned char *pucParam, unsigned char *pucParamLen);
short ServerParamSet(unsigned char ucParamId, unsigned char *pucParam, unsigned char ucParamLen);
short SetColorParam(ST_COLOR_PARAM *param, int paramSize);
short GetColorParam(ST_COLOR_PARAM *param, int paramSize);
unsigned short GetColorParamEx(E_ID_COLOR paramId);

void DeviceParaInit(void);
short SetLanguageParam(unsigned char *param, int paramSize);
short GetLanguageParam(unsigned char *param, int paramSize);


//int UpdateTmsSoft(E_ID_TMS_SOFT_OPT tmsSoftOptId, int offset, void *para, int paraSize);
// Used to operate files in other source files
int AppendFile(char *fileName, void *para, int paraSize);
int WriteFile(char *fileName, int offset, void *para, int paraSize);
int ReadFile(char *fileName, int offset, void *para, int paraSize);
int InitFilePara(char *fileName, unsigned long fileSize, void (*DefaultInit)(void *para, unsigned long paraSize));
extern unsigned char GetPwrParam(E_INFO ucId);


#endif //__PARAM_MANAGE_H__

