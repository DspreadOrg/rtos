//-----------------------------------------------------------------------------
// @file    : LibSysapi.h
// @brief   : System-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef _LIB_SYS_API_H
#define _LIB_SYS_API_H
#include "LibTypedef.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#define TERMINAL_AID    0
#define TERMINAL_CAPK   1

#define TOPBAR_DISP_TIME_ENABLE     1
#define TOPBAR_DISP_TIME_DISENABLE  0

// Power on method
typedef enum
{
    POWERON_CAUSE_UNKNOWN   = 0,            // unknown
    POWERON_CAUSE_PWRKEY    = (1 << 0),     // startup through the power key
    POWERON_CAUSE_RESET     = (1 << 15),    // APP reset startup
} E_POWERON_CAUSE;

typedef enum
{
    ID_DEV_IS_ENTER_APP,                // Check if entered the transaction application
    ID_DEV_IS_SE_PROTECT,               // Check if SE is sealed
    ID_DEV_IS_NEW,                      // Check new SE device
    ID_DEV_IS_KEY_TRIG,                 // Check volume - triggered or not
    ID_DEV_IS_WIFI_DOWNLOAD,            // Check if the device's WIFI firmware has been downloaded
    ID_DEV_IS_AGING_TEST,               // Check device aging test identification
} E_DEV_STATE_TYPE;

typedef enum
{
    E_FONT_NAME = 0,            // Font library file name
    E_LANG_TYPE = 1,            // Language Type
    E_MENU_XML_NAME,            // XML menu file name
    E_CHARACTER_ENCODING_TYPE,  // character encoding type, please refer to type: E-CHARACTER-ENCODING-COLLECTION
    E_TIME_ZONE,                // time zone
    E_NTP_SERVER,               // NTP server
    E_NET_MODE,                 // Network mode
    E_APP_VERSION,              // APP version
    E_EMV_LIB_VERSION,          // EMV Library Version
    E_SELFCHECK_LIB_VERSION,    // Self-Check library version
    E_TIME_DISPLAY,             // Status bar time display format
    E_TMS_LIB_VERSION,          // TMS library version
    E_BATTERY_LIB_VERSION,      // Battery management library version
    E_POWER_OFF_TIME,           // Poweroff time
    E_SLEEP_TIME,               // sleep time
    E_BLACKLIGHT_TIME,          // backlight time
    E_BATTERY_CURV,             // Battery curve
    E_USERAUDIO_LIB_VERSION,    // Voice library version
    E_SIM_IDX,                  // SIM Slot Number
    E_WAKEUP_HOST,              // Background task wake-up source
    E_WAKEUP_APP_FIXED,         // Application scheduled task wake-up source
    E_DEV_SLEEP_TYPE,           // Device sleep mode
    E_UNLOCK_ENABLE_TYPE = 23,  // Settings support online and offline unlock types
    E_FILE_TOTAL_SIZE,          // File system total size
}E_INFO;

typedef struct
{
    int (*pLcdDrowArea)(unsigned short, unsigned short, unsigned short, unsigned short, unsigned short);
    void (*pLcdClrScreen)(void);
    void (*pUIClrScreen)(void);
    void (*pLcdDisSingleRecent)(unsigned short, unsigned short, unsigned short, unsigned short , unsigned char);
    void (*pUIDrawLine)(unsigned short, unsigned short, unsigned short, unsigned short);
    void (*pUIIcon)(unsigned short, unsigned short, unsigned short, unsigned short, unsigned char, unsigned char *);
    void (*pUIClearAllScreen)(void);
    unsigned char (*pUIGetKeyWithTaskHandler)(void);
    void (*pUIDisPlayTimerReady)(void);
    int (*pLcdGetTextWidth)(unsigned char *, unsigned short, unsigned short);
    void (*pUIShowXCodeProcess1)(unsigned short, unsigned short, unsigned short, unsigned short, unsigned char *);
    void (*pUIShowXCodeProcess2)(unsigned short, unsigned short, unsigned short, unsigned short, unsigned char *);
    int (*pPrnStrEx)(unsigned char mode, char *prnBuf, unsigned short prnLen);
    int (*pPrnGetStrWidth)(char *prnBuf, unsigned short prnLen, unsigned char ucFont , unsigned char ucZoomH);
} ST_SDK_CALLBACK, *PST_SDK_CALLBACK;

//-----------------------------------------------------------------------------
// Name    : Lib_OsApiInit
// Func    : System startup parameters initialization
// Paras   : os_api[in],  input System parameters pointer
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_OsApiInit(void *os_api);

//-----------------------------------------------------------------------------
// Name    : Lib_SysInit
// Func    : Initialize System
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    : This function must be called before entering Application.
//-----------------------------------------------------------------------------
int Lib_SysInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_AppInit
// Func    : Initialize Application
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_AppInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_DelayMs
// Func    : delay
// Paras   : ulMs[in],  input time,unit is millisecond
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_DelayMs(unsigned long ulMs);

//-----------------------------------------------------------------------------
// Name    : Lib_RtcToUtcSec
// Func    : convert RTC to UTC
// Paras   : pucTimeBuf[in],     input RTC time,The format is YYMMDDhhmmss and BCD.
//           ucTimeBufSize[in],  input time length.It should be 6 bytes.
//           pulUtc[in/out],     output UTC time
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_RtcToUtcSec(unsigned char *pucTimeBuf, unsigned char ucTimeBufSize, unsigned long *pulUtc);

//-----------------------------------------------------------------------------
// Name    : Lib_UtcSecToRtc
// Func    : convert UTC to RTC
// Paras   : ulUtc[in],          input UTC time
//           pucTimeBuf[in],     output RTC time,The format is YYMMDDhhmmss and BCD.
//           ucTimeBufSize[in],  input time length.It should be 6 bytes.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_UtcSecToRtc(unsigned long ulUtc, unsigned char *pucTimeBuf, unsigned char ucTimeBufSize);

//-----------------------------------------------------------------------------
// Name    : Lib_SetDateTime
// Func    : Set System Date and Time
// Paras   : pucBuf[in],  input date and time,The format is YYMMDDhhmmss and BCD.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_SetDateTime(unsigned char *pucBuf);

//-----------------------------------------------------------------------------
// Name    : Lib_GetDateTime
// Func    : Get System Date and Time
// Paras   : pucBuf[in/out],  output date and time,The format is YYMMDDhhmmss and BCD.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_GetDateTime(unsigned char *pucBuf);

//-----------------------------------------------------------------------------
// Name    : Lib_SyncDateTime
// Func    : Synchronize time from NTP server
// Paras   : sntp[in],  intput NTP server
//           timeZone[in],  input timezone
//           timeoutMs[in],  input timeout,unit is millisecond. 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_SyncDateTime(char *sntp, int timeZone, int timeoutMs);

//-----------------------------------------------------------------------------
// Name    : Lib_TimeGetUnixTime
// Func    : Get Unix Time
// Paras   : 
// Ret     : return Unix Time
// Desc    : 
//-----------------------------------------------------------------------------
unsigned long Lib_TimeGetUnixTime(void);

//-----------------------------------------------------------------------------
// Name    : Lib_SysGetTimer
// Func    : Get system time
// Paras   : 
// Ret     : return system time
// Desc    :
//-----------------------------------------------------------------------------
unsigned long Lib_SysGetTimer(void);

//-----------------------------------------------------------------------------
// Name    : Lib_ReadSn
// Func    : Get device serial number
// Paras   : snBuf[in/out],  output device serial number
//           bufSize[in],    input length of data buffer
// Ret     : return the length of serial number, 
//           The length more than 0 indicates successful
//           The length less than or equal to 0 indicates failure.
// Desc    :
//-----------------------------------------------------------------------------
int Lib_ReadSn(unsigned char *snBuf, unsigned long bufSize);

//-----------------------------------------------------------------------------
// Name    : Lib_GetPN
// Func    : Get device PN
// Paras   : pucBuf[in/out],  output device PN
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_GetPN(unsigned char *pucBuf);

//-----------------------------------------------------------------------------
// Name    : Lib_GetDevState
// Func    : Get device state
// Paras   : devStateType[in],  input E_DEV_STATE_TYPE
// Ret     : return device state
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_GetDevState(E_DEV_STATE_TYPE devStateType);

//-----------------------------------------------------------------------------
// Name    : Lib_Reboot
// Func    : Reboot device
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_Reboot(void);

//-----------------------------------------------------------------------------
// Name    : Lib_ShutDown
// Func    : Shutdown device
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_ShutDown(void);

//-----------------------------------------------------------------------------
// Name    : Lib_FactoryRecovery
// Func    : Revoert factory settings
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_FactoryRecovery(void);

//-----------------------------------------------------------------------------
// Name    : Lib_SysSleep
// Func    : Sleep device
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_SysSleep(void);

//-----------------------------------------------------------------------------
// Name    : Lib_ExtPower
// Func    : Get the status of external power adapter
// Paras   : 
// Ret     : 0: unplug ,1: plug in
// Desc    :
//-----------------------------------------------------------------------------
int Lib_ExtPower(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PowerChargingCtrl
// Func    : Charging while the device is off
// Paras   : 
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_PowerChargingCtrl(void);

//-----------------------------------------------------------------------------
// Name    : Lib_GetPwrKeyStatus
// Func    : Get Power Key status
// Paras   : 
// Ret     : 0: Not pressed 1: Pressed
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_GetPwrKeyStatus(void);

//-----------------------------------------------------------------------------
// Name    : Lib_GetPowerOnCause
// Func    : Get the reason for device power on
// Paras   : 
// Ret     : refer to E_POWERON_CAUSE
// Desc    : 
//-----------------------------------------------------------------------------
unsigned long Lib_GetPowerOnCause(void);

//-----------------------------------------------------------------------------
// Name    : Lib_MaintenanceMode
// Func    : Device enters into Maintenance mode
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_MaintenanceMode(void);

//-----------------------------------------------------------------------------
// Name    : Lib_WDGReloadCounter
// Func    : Reload Watch Dog Counter
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_WDGReloadCounter(void);

//-----------------------------------------------------------------------------
// Name    : Lib_TamperIsOccured
// Func    : Check if the device has been in Tamper status.
// Paras   : pucType[in/out],  output Tamper Code when the device has been in Tamper status.
// Ret     : 0 - Yes, others - No
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_TamperIsOccured(unsigned char *pucType);

//-----------------------------------------------------------------------------
// Name    : Lib_KbGetKey
// Func    : check if key is pressed
// Paras   : 
// Ret     : return key
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_KbGetKey(void);

//-----------------------------------------------------------------------------
// Name    : Lib_Beep
// Func    : The beep sounds for a period of time
// Paras   : keep_ms[in],  input the duration time,unit is millisecond
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_Beep(unsigned long keep_ms);

//-----------------------------------------------------------------------------
// Name    : Lib_SdkCallbackInit
// Func    : Initialize callback 
// Paras   : pCallback[in],  input PST_SDK_CALLBACK
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_SdkCallbackInit(PST_SDK_CALLBACK pCallback);

//-----------------------------------------------------------------------------
// Name    : Lib_PrintfHeapInfo
// Func    : Print Heap information
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_PrintfHeapInfo(void);

//-----------------------------------------------------------------------------
// Name    : Lib_malloc
// Func    : apply for memory space
// Paras   : ulSize[in],  input length
// Ret     : return the pointer of memory space
// Desc    :
//-----------------------------------------------------------------------------
void *Lib_malloc(unsigned long ulSize);

//-----------------------------------------------------------------------------
// Name    : Lib_free
// Func    : free memory space
// Paras   : ptr[in],  input pointer of memory space
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_free(void* ptr);

//-----------------------------------------------------------------------------
// Name    : Lib_GetHardState
// Func    : Get hardware status
// Paras   : 
// Ret     : 0: normal,1: abnormal
// Desc    : 
//-----------------------------------------------------------------------------
unsigned long Lib_GetHardState(void);

//-----------------------------------------------------------------------------
// Name    : Lib_CheckUpgradeFlag
// Func    : Get device upgrade flag
// Paras   : 
// Ret     : 0: no upgrade flag,non-zero: upgrade flag
// Desc    : 
//-----------------------------------------------------------------------------
unsigned char Lib_CheckUpgradeFlag(void);

//-----------------------------------------------------------------------------
// Name    : Lib_SetInfo
// Func    : Set device information
// Paras   : ucId[in],       input device information type
//           pvInfoBuf[in],  input data buffer
//           ulInfoLen[in],  input data length
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_SetInfo(E_INFO ucId, void *pvInfoBuf, unsigned long ulInfoLen);

//-----------------------------------------------------------------------------
// Name    : Lib_GetInfo
// Func    : Get device information
// Paras   : ucId[in],            input device information type
//           pvInfoBuf[in/out],   output device information
//           pulInfoLen[in/out],  output data length
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_GetInfo(E_INFO ucId, void *pvInfoBuf, unsigned long *pulInfoLen);

//-----------------------------------------------------------------------------
// Name    : Lib_XmlCvtIdToStr
// Func    : Get the corresponding string from xml file based on ID
// Paras   : usId[in],  input id
// Ret     : return the corresponding string
// Desc    : 
//-----------------------------------------------------------------------------
const char* Lib_XmlCvtIdToStr(unsigned short usId);
                  
//-----------------------------------------------------------------------------
// Name    : Lib_Trace
// Func    : Output log
// Paras   : fmt[in],  input format
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_Trace(const char *fmt, ...);

//-----------------------------------------------------------------------------
// Name    : Lib_TraceBuf
// Func    : Output log
// Paras   : pBuf[in],  input data buffer
//           len[in],   input data length
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_TraceBuf(unsigned char *pBuf, unsigned short len);

//-----------------------------------------------------------------------------
// Name    : Lib_TraceLogConfig
// Func    : Set output log mode
// Paras   : ucFlag[in],  input 0x00: turn off log output;
//                          0x01: turn on log output;
//                          0x02: turn on log output and save log with file;
//                          0x03: upload log file to server;
//                          0x80: turn on CP log output.
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
int Lib_TraceLogConfig(unsigned char ucFlag);

//-----------------------------------------------------------------------------
// Name    : Lib_GetVersion
// Func    : Get module version
// Paras   : pucBuf[in/out],  output version
//           buf_size[in],    input maximum length of data buffer
//           flag[in],        input 0: os version,1: SDK version,
//                             2: Security chip version,3: hardware version.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_GetVersion(unsigned char *pucBuf, int buf_size, unsigned char flag);

//-----------------------------------------------------------------------------
// Name    : Lib_realloc
// Func    : Reallocate a memory block
// Paras   : ptr  - pointer to a previously allocated memory block,
//                  or NULL to allocate a new block
//           size - new size in bytes
// Ret     : Pointer to the reallocated memory block, or NULL on failure
// Desc    :
//-----------------------------------------------------------------------------
void *Lib_realloc(void *ptr, unsigned long size);

#endif