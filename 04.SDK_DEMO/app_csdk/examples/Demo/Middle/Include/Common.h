/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Common.h
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : Device General Algorithm apis
 **********************************************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

#include "LibTypedef.h"

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------
//0-maintenance mode, 1-TMS mode, 2-application mode
#define USB_WORK_MODE       0
#define TMS_WORK_MODE       1
#define SUBAPP_WORK_MODE    2

#define RET_OK                          (0)  /**< correct */
#define RET_ERR                         (-1) /**< General error */
#define RET_ERR_PARAMETER               (-2) /**< parameter error */
#define RET_ERR_OUT_OF_MEMORY           (-3) /**< out of memory */
#define RET_ERR_TIMEOUT                 (-4) /**< timeout */

// UI menu error value definition
#define UI_INPUT_ENTER                  (0)
#define UI_PY_INPUT_DONE                (1)
#define UI_RF_ACTIVE_DONE               (2)
#define UI_INPUT_CANCEL                 (-1003)
#define UI_INPUT_CLEAR                  (-1004)
#define UI_INPUT_TIMEOUT                (-1005)
#define UI_INPUT_UP                     (-1006)
#define UI_INPUT_DOWN                   (-1007)
#define UI_INPUT_CANT_DISPLAY           (-1008)
#define UI_INPUT_DIFFERENT_PWD          (-1009)
#define UI_WRONG_PWD                    (-1010)
#define UI_CHANGE_PWD_FAIL              (-1011)
#define UI_PY_NO_MATCH                  (-1012)
#define UI_PY_INPUT_NONE                (-1013) // No Pinyin input
#define UI_RF_ACTIVE_FAIL               (-1014)
#define UI_RF_ACTIVE_TIMEOUT            (-1015)
#define UI_RF_ACTIVE_CANCEL             (-1016)
#define UI_INPUT_DEFAULT_STR_FMT_ERR    (-1017) // Default string format error
#define UI_INPUT_FMT_NOT_SUPPORT        (-1018)
#define UI_CA_PARAM_NULL                (-1019) // CA parameter is empty
#define UI_AID_PARAM_NULL               (-1020) // AID parameter is empty
#define UI_POWER_OFF                    (-1021)
#define UI_INPUT_MENU                   (-1022)
#define UI_INPUT_ZM					    (-1023)
#define UI_NO_RECORD				    (-1024)
#define UI_INPUT_FUN				    (-1025)
#define UI_INPUT_ENTER_NULL             (-1035)

#define UI_SWITCH_GPRS                  (-2000)

// Net Error codes
// The requested network handle is invalid
#define NET_ERR_INVALID_HANDLE                  (-16)


typedef enum
{
    E_ID_EMPTY = 0,         // 20x20 blank image, all except for the battery icon are 20x20

    E_ID_DEBUG,
    E_ID_USB,

	E_ID_BAT_DIS_C0,        // discharge capacity, level0
    E_ID_BAT_DIS_C1,        // discharge capacity, level1
    E_ID_BAT_DIS_C2,        // discharge capacity, level2
    E_ID_BAT_DIS_C3,        // discharge capacity, level3
    E_ID_BAT_DIS_C4,        // discharge capacity, level4

    E_ID_BAT_CHARGING,      // charging
	E_ID_BAT_CHARGE_FULL,   // Full

	E_ID_GPRS_NOSIM,
	E_ID_GPRS_FAIL,
	E_ID_GPRS_CSQ_1,
    E_ID_GPRS_CSQ_2,
    E_ID_GPRS_CSQ_3,
    E_ID_GPRS_CSQ_4,

	E_ID_WIFI_01,			// no signal
    E_ID_WIFI_11,			// signal level1
    E_ID_WIFI_12,			// signal level2
    E_ID_WIFI_13,			// signal level3
	E_ID_WIFI_14,			// No external network access permission

	E_ID_DATA_UP,			// Data upload
	E_ID_DATA_DOWN,			// Data download
	E_ID_DATA_UP_DOWN,		// Data upload and download
	E_ID_DATA_NOT_UP_DOWN,	// data not have upload and download icon

	E_ID_WIFI_CONNECTED,	// WIFI connected icon

    E_ID_CONTACT_BAD,		// Bad, contact card
    E_ID_CONTACTLESS,       // non-contact
	E_ID_CONTACT,			// Contact card

    E_ID_WIFI_CONNECTING_1,           // WIFI connecting icon 1
    E_ID_WIFI_CONNECTING_2,           // WIFI connecting icon 2
    E_ID_WIFI_CONNECTING_3,           // WIFI connecting icon 3
    E_ID_WIFI_CONNECTING_4,           // WIFI connecting icon 4
    E_ID_WIFI_CONNECTING_5,           // WIFI connecting icon 5
    E_ID_WIFI_CONNECTING_6,           // WIFI connecting icon 6
	E_ID_WIFI_CONNECTING_7,           // WIFI connecting icon 7
	E_ID_WIFI_CONNECTING_8,           // WIFI connecting icon 8

	E_ID_TIME = 150,
	E_ID_GPRS_TYPE,                   // 4G ICON
    E_ID_BEEP_ON,                     // BEEP ON ICON
    E_ID_BEEP_OFF,                    // BEEP OFF ICOn
    E_ID_GPRS_OPERATOR_1,             // SIM Operator
    E_ID_GPRS_OPERATOR_2,
    E_ID_GPRS_OPERATOR_3,
    E_ID_GPRS_OPERATOR_4,

	E_ID_TOTAL,
}WIFI_ICON_TYPE;

typedef struct
{
    void (* CbTopBarTimeFormat)(unsigned char *paucRtcTime, unsigned char *paucRtcTimeStr, unsigned char *ucRtcTimeStrLen);
    unsigned char uc_Switch;// switch
}ST_TOPBAR_TIME_PARAM;


#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
// Does the current system time exceed delta compared to start
#define IS_TIME_OUT(start, delta)       ((unsigned long)(Lib_SysGetTimer() - (unsigned long)(start)) >= (unsigned long)(delta))
// Number of array elements
#define ITEM_OF(array)                  (sizeof(array)/sizeof(array[0]))



#define TRACE                   ApiLibTrace
#define TRACEBUF                ApiLibTraceBuf
#define FUN_IN                  {TRACE("@FUN_IN--->%s:%d:%s\n", __FILE__, __LINE__, __FUNCTION__);}
#define FUN_OUT                 {TRACE("@FUN_OUT--->%s:%d:%s\n", __FILE__, __LINE__, __FUNCTION__);}

#define FUN_IN_ERR              {TRACE("@FUN_IN_ERR--->%s:%d:%s ------------ERR------------\r\n", __FILE__, __LINE__, __FUNCTION__);}
#define TRACE_LOG(fmt, ...)     {TRACE("APP@ Line: %d,Func: %s," fmt"\n", __LINE__, __FUNCTION__, ##__VA_ARGS__);}


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------
signed long long BcdToLongLong(unsigned char *pucBcd, unsigned char ucBcdLen);
unsigned char DateIsLegal(unsigned long year, unsigned char month, unsigned char day);
void BcdSum2AscSum(unsigned char *pucIn, char *pucOut);
short IsLeapYear(unsigned long ulYear);
short DateCheck(unsigned char ucMonth1, unsigned char ucDay1);
int Strcspn(const char * string,const char * control);
int iSplitStr(const char *pcStr, int iStr1Len, char *pcStr1, char *pcStr2);
void itoaw (int n,char s[]);
void ApiLibTrace(const char *fmt, ...);
void ApiLibTraceBuf(uint8_t *pBuf, uint16_t len);
void SetTraceLogConfig(unsigned char ucFlag);
unsigned char GetTraceFlag(void);


short Trace_LongData(char *data);
short Trace_LongBuf(char *data, unsigned int len);

void AmtAddThdsPt(char* amtstr);
unsigned long dfToInt(double dValue);

#endif //__COMMON_H__

