/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : LanguageProcess.c
 *  Version      :
 *  Author       :
 * Description   : Implement multilingual display function
 **********************************************************************************************************/

#include "LanguageProcess.h"
#include "Common.h"
#include "Drv.h"
#include "ParamManage.h"
#include "LanguageResource.h"
#include "Language.h"
#include "LibEmvKernel.h"
#include "LibSelfCheck.h"
#include "Communication.h"
#include "LibTms.h"
#include "MessageBox.h"
#include "Key.h"
#include "api_lvgl.h"
#include "version.h"

typedef struct tagLanguageFileInfo
{
    unsigned char m_ucLanguage;
    char*         m_pscFileName;
    char*         m_pscFontFileName;
    unsigned long m_ulFontFileSize;
}LANGUAGE_FILE_INFO_ST;

LANGUAGE_FILE_INFO_ST g_astLangFileInfo[] =
{
    {ENGLISH_ID,     "language-en-US.xml", "AlibabaSans-Regular.ttf",           113*1024},
    {PORTUGUESE_ID,  "language-pt-BR.xml", "AlibabaSans-Regular.ttf",           113*1024},
    {FARSI_ID,       "language-fa-IR.xml", "IRANYekanMobileBold(FaNum).ttf",    100*1024},
};


unsigned char g_ucLanguage = 0xFF;

//-----------------------------------------------------------------------------
// Name      : TopBarTimeFormat
// Function  : Callback function that combines the time to be displayed on the status bar based on paucRtcTime
// Parameter :
//              paucRtcTime[in]: the content is the current time, a total of 12 data, in order of the year, month, day, hour, minutes, seconds, each data has two(YYMMDDHHMMSS)
//              paucRtcTimeStr[out]:The time string to be displayed on the status bar, ending with '\0'.
//              ucRtcTimeStrLen[in/out]:The input is the maximum length of data that can be received in the paucRtcTimeStr cache, and the output is the real length of data stored in paucRtcTimeStr
// return:
// Description:
//-----------------------------------------------------------------------------
void TopBarTimeFormat (unsigned char *paucRtcTime, unsigned char *paucRtcTimeStr, unsigned char *ucRtcTimeStrLen)
{
    unsigned char ucRtcTime[20] = {0};

    if(paucRtcTime == NULL)
    {
        FUN_IN_ERR;
        return;
    }

    //Here, you need to convert the paucRtcTime to the desired format and fill it into paucRtcTimeStr
    memcpy(ucRtcTime, "2020-01-01 00:00:00", strlen("2020-01-01 00:00:00"));

    ucRtcTime[2] = paucRtcTime[0];
    ucRtcTime[3] = paucRtcTime[1];

    ucRtcTime[5] = paucRtcTime[2];
    ucRtcTime[6] = paucRtcTime[3];

    ucRtcTime[8] = paucRtcTime[4];
    ucRtcTime[9] = paucRtcTime[5];

    ucRtcTime[11] = paucRtcTime[6];
    ucRtcTime[12] = paucRtcTime[7];

    ucRtcTime[14] = paucRtcTime[8];
    ucRtcTime[15] = paucRtcTime[9];

    ucRtcTime[17] = paucRtcTime[10];
    ucRtcTime[18] = paucRtcTime[11];

    if(strlen((char *)ucRtcTime) < *ucRtcTimeStrLen)
    {
        memcpy(paucRtcTimeStr, ucRtcTime, strlen((char *)ucRtcTime));
        *ucRtcTimeStrLen = strlen((char *)ucRtcTime);
    }

    return;
}


// Initialize relevant resources based on parameters
int ResourceInit(void)
{
    unsigned char len = 0;
    int timeZone = 0;
    unsigned char NTPServer[NETWORK_ADDR_SIZE] = {0};
    char acVersionBuffer[64] = {0x00};
    unsigned short usVerInfoLen = 0;
    unsigned char ucParam;

    //Language setting: The second parameter must be passed as an unsigned char * type
    Lib_SetInfo(E_LANG_TYPE, &g_ucLanguage, sizeof(unsigned char));

    // Set Font Library File Name
    Lib_SetInfo(E_FONT_NAME, g_astLangFileInfo[g_ucLanguage].m_pscFontFileName, strlen(g_astLangFileInfo[g_ucLanguage].m_pscFontFileName));

    // Set the font size based on the actual use of the font library, and the second parameter must be passed in as an unsigned long * type
    // Lib_SetInfo(E_FONT_SIZE, &g_astLangFileInfo[g_ucLanguage].m_ulFontFileSize, sizeof(unsigned long));

    // Set XML file name
    Lib_SetInfo(E_MENU_XML_NAME, g_astLangFileInfo[g_ucLanguage].m_pscFileName, strlen(g_astLangFileInfo[g_ucLanguage].m_pscFileName));

    // Set Time Zone
    TerminalParamGet(TIME_ZONE_BIT, &timeZone, &len);
    Lib_SetInfo(E_TIME_ZONE, &timeZone, sizeof(timeZone));

    TerminalParamGet(NTP_SERVER_BIT, NTPServer, &len);
    Lib_SetInfo(E_NTP_SERVER, NTPServer, sizeof(NTPServer));

    // Set APP version
    Lib_SetInfo(E_APP_VERSION, APP_VERSION, strlen(APP_VERSION));

    // Set EMV library version
    usVerInfoLen = sizeof(acVersionBuffer);
    Lib_EmvGetLibVersion(acVersionBuffer, &usVerInfoLen);
    if(usVerInfoLen)
    {
        Lib_SetInfo(E_EMV_LIB_VERSION, acVersionBuffer, usVerInfoLen);
    }

    // Set self-test library version
    usVerInfoLen = sizeof(acVersionBuffer);
    memset(acVersionBuffer, 0x00, usVerInfoLen);
    Lib_SelfCheckGetLibVersion(acVersionBuffer, &usVerInfoLen);
    if(usVerInfoLen)
    {
        Lib_SetInfo(E_SELFCHECK_LIB_VERSION, acVersionBuffer, usVerInfoLen);
    }

    // Set TMS library version
    usVerInfoLen = sizeof(acVersionBuffer);
    memset(acVersionBuffer, 0x00, usVerInfoLen);
    Lib_TmsGetLibVersion(acVersionBuffer, &usVerInfoLen);
    if(usVerInfoLen)
    {
        Lib_SetInfo(E_TMS_LIB_VERSION, acVersionBuffer, usVerInfoLen);
    }

    // Set battery library version
    usVerInfoLen = sizeof(acVersionBuffer);
    memset(acVersionBuffer, 0x00, usVerInfoLen);
    Lib_BatGetLibVersion(acVersionBuffer, &usVerInfoLen);
    if (usVerInfoLen)
    {
        Lib_SetInfo(E_BATTERY_LIB_VERSION, acVersionBuffer, usVerInfoLen);
    }

    ucParam = GetNetWorkMode();
    // Set up to SDK for subsequent network communication management
    Lib_SetInfo(E_NET_MODE, &ucParam, sizeof(ucParam));

#if defined(DUAL_SIM)
    // Configure SIM card slot number
    if (NET_MODE_GPRS == ucParam)
    {
        NetSimParamInit();
    }
#endif

    return RET_OK;
}

short LanguageMenuSet(int iTimeout)
{
    short sRC = -1;
    unsigned char initialPara;
    unsigned char languagePara;
    int paraLen = 1;
    char* menuName[] =
    {
        ENGLISH_ICON,
        PORTUGUESE_ICON,
        FARSI_ICON,
    };
    GetLanguageParam(&initialPara, paraLen);

    languagePara = initialPara;

    while (1)
    {
        if (sRC != LVGL_BUTTON_SELECTED_FOR_REFRESH)
        {
            LvUiShowUiMenuPackY(SELECT_LANG, menuName, ITEM_OF(menuName), languagePara, TRUE);
        }

        sRC = LvUiWaitKey(iTimeout);
        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT:
        case KB_CANCEL:
            return sRC;

        case KB_DOWN:
            languagePara++;
            languagePara = languagePara % ITEM_OF(menuName);
            break;

        case KB_UP:
            if (languagePara == 0)
            {
                languagePara = ITEM_OF(menuName) - 1;
            }
            else
            {
                languagePara--;
            }
            break;

        case KB_DIGIT_ONE:
        case KB_DIGIT_TWO:
        case KB_DIGIT_THREE:
            languagePara = sRC - '1';
            break;

        case 1:
        case 2:
        case 3:
            languagePara = sRC - 1;
            break;

        case KB_ENTER:
            if (languagePara != initialPara)
            {
                LvUiMessageBox(INITIALIZE_LANG, MB_COLOR_YELLOW | MB_CENTER | MB_ICONQUESTION | MB_YESNO, NULL, 1000);
                // Switch Language
                //sRC = LanguagePackSwitch(ucLanguage);
                sRC = RET_OK;
                if (RET_OK == sRC)
                {
                    LvUiMessageBox(SETTING_DONE, MB_COLOR_YELLOW | MB_CENTER | MB_ICONQUESTION | MB_YESNO, NULL, 1000);
                    // Save language parameters
                    SetLanguageParam(&languagePara, sizeof(languagePara));
                    LvUiMessageBox(TIP_PLEASE_REBOOT_LATER, MB_COLOR_YELLOW | MB_CENTER | MB_ICONQUESTION | MB_YESNO, NULL, 1000);
                    ResourceInit();
                    UILcdClrScreenLvgl();
                    LvUiWaitKey(60);
                    Lib_Reboot();
                }
                else
                {
                    LvUiMessageBox(INITIALIZE_FAIL, MB_COLOR_YELLOW | MB_CENTER | MB_ICONQUESTION | MB_YESNO, NULL, 3000);
                }
            }

            return RET_OK;
        default:
            break;
        }
    }

    return RET_OK;
}

