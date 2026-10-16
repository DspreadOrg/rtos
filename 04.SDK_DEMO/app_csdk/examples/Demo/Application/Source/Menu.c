/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : menu.c
 *  Version      :
 *  Author       :
 * Description   : Menu function implementation
 **********************************************************************************************************/

#include "Menu.h"
#include "LibSysApi.h"
#include "Language.h"
#include "Drv.h"
#include "Trade.h"
#include "Common.h"
#include "TradeRecord.h"
#include "Ui.h"
#include "Input.h"
#include "ParamManage.h"
#include "Battery.h"
#include "Ui.h"
#include "MessageBox.h"
#include "Key.h"
#include "CardTrade.h"
#include "BarcodeTrade.h"
#include "Logon.h"
#include "WiFiConfig.h"
#include "LibSelfCheck.h"
#include "Communication.h"
#include "net_if.h"
#include "LanguageProcess.h"
#include "LanguageResource.h"
#include "Tms_Module.h"
#include "DevTime.h"
#include "Power.h"
#include "LibGprs.h"
#include "Idle.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"
#if HW_TPH
#include "print.h"
#endif

extern short UIGpsSignalInfo(int timeout);
extern short UIGpsSatInfo(int timeout);

static short UISetTime(char *title, unsigned int minTime, unsigned int maxTime, E_INFO timeInfo, int timeout);

void LvStatusBarCb(void)
{
    unsigned char beep_flag;
    unsigned char beep_len = 1;

    // Determine if the device is in DEBUG state
    if (!Lib_GetDevState(ID_DEV_IS_SE_PROTECT))
    {
        LvUiSetStatusBarIcon(ICON_DEBUG_X, DEBUG_ICON, 0);
    }

    // Determine if the buzzer is in a switch state
    TerminalParamGet(BEEP_BIT, &beep_flag, &beep_len);
    if (beep_flag == 1)
    {
        LvUiSetStatusBarIcon(ICON_SPK_X, SPKON_ICON, 1);
    }
    else
    {
        LvUiSetStatusBarIcon(ICON_SPK_X, SPKOFF_ICON, 1);
    }
}


void LvHomeMenu(void *p)
{
    char acTemp[64] = { 0 };
    int key = KB_NULL;
    unsigned short w, h, font, forecolor, backcolor;
    t_lvgl_image img = { 0 };

    img.bgImg = (char *)BG_IMG;
    img.noRecolor = true;

    Lib_GetVersion((unsigned char *)acTemp, sizeof(acTemp), 5);

    //Customize the status bar
    LvUiCreateStatusBarTask(LvStatusBarCb);

    //Check TMS for tasks that need to be upgraded
    TmsAutoUpgrade(60*1000);

    Lib_osiThreadCreate("nettask", (osiThreadEntry_t)Net_StartChannelThread, NULL, OSI_PRIORITY_NORMAL, (unsigned int)32*1024, 0);

#if defined(MQTT_SUPPORT)
    Lib_osiThreadCreate("mqttThread", (osiThreadEntry_t)MqttTaskThread, NULL, OSI_PRIORITY_NORMAL, (unsigned int)32*1024, 0);
#endif

    while (1)
    {
        LvUiCtrlStatusBar(1);

        LvUilCls();

        Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

        LvUiShowImage(0, -LvUiGetStatusBarHeight(), &img, -1);
        LvUiText(LVGL_ALIGN_CENTER, 0, 127, FONT_SIZE_SMALL, acTemp, (lv_color_t)forecolor);

        DispMainTimeSet(1);
        key = LvUiWaitKey(30 * 1000);
        DispMainTimeSet(0);

        if (key == KB_ENTER || (1 == key))
        {
            // close status bar
            LvUiCtrlStatusBar(0);
            DeviceSleepEnDisableSet(FALSE);
            mainMenu(60 * 1000);
            DeviceSleepEnDisableSet(TRUE);
            // enable status bar
            LvUiCtrlStatusBar(1);
            LvUilCls();
            key = KB_NULL;
        }
        else if ( key == KB_CANCEL )
        {
        }
    }
}



//-----------------------------------------------------------------------------
// Name    : UIDevInfo
// Func    : Display device information menu
// Paras   : iTimeout[in],   timeout value, in milliseconds
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
static short UIDevInfo(int iTimeout)
{
    t_lvgl_image img = { 0 };
    unsigned char ucLen, ucBatCap;
    char acTemp[128];
    unsigned char aucPN[32] = { 0 };
    char acBuf[1024] = { 0 };
    short sRC;
    unsigned long ulStrlen = 0;
    unsigned int uiTotalSize = 0;
    unsigned int uiFreeSize = 0;
    unsigned long ulLen;

    LvUilCls();

    img.bgImg = (char *)TITLE_BG_IMG;
    LvUiShowImage(MENU_COMMON_TITLEBACKGROUND_OFF_X, MENU_COMMON_TITLEBACKGROUND_OFF_Y, &img, -1);
    LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char*)TIP_DEV_INFO, lv_color_hex(0xFFFFFF));
    Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

DEVINFO:

    memset(acBuf, 0x00, sizeof(acBuf));
    // device model
    memset(acTemp, 0, sizeof(acTemp));
    Lib_GetPN(aucPN);
    sprintf((char *)acTemp, " %s: %s\n", TIP_MODEL, aucPN);
    strcat(acBuf, acTemp);

    //SN
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_SN);
    ucLen = strlen((char *)acTemp);
    if (Lib_ReadSn((unsigned char *)(acTemp + ucLen), sizeof(acTemp) - ucLen) <= 0)
    {
        memcpy(acTemp + ucLen, "null", 4);
    }
    UITextSimplify((char *)acTemp, strlen(TIP_SN) + 3, NULL);
    strcat(acBuf, acTemp);
    acBuf[strlen(acBuf)] = '\n';

    //IMEI
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_IMEI);
    ucLen = strlen((char *)acTemp);
    if (Lib_WlsGetInfo(ID_GPRS_IMEI, acTemp + ucLen, sizeof(acTemp) - ucLen) != RET_OK)
    {
        memcpy(acTemp + ucLen, "null", 4);
    }
    UITextSimplify((char *)acTemp, strlen(TIP_IMEI) + 3, NULL);
    strcat(acBuf, acTemp);
    acBuf[strlen(acBuf)] = '\n';

    ulStrlen = strlen(acBuf);

    while (1)
    {
        // Battery level information
        memset(acTemp, 0, sizeof(acTemp));
        memset(acBuf + ulStrlen, 0, sizeof(acBuf) - ulStrlen);
        if (Lib_ExtPower())
        {
            sprintf((char *)acTemp, " %s: %s", TIP_POWER, TIP_CHARGING);
        }
        else
        {
            ucBatCap = BatteryGetBatCapacity();
            sprintf((char *)acTemp, " %s: %02d", TIP_POWER, ucBatCap);
        }
        acTemp[strlen(acTemp)] = '\n';
        strcat(acBuf, acTemp);

        memset(acTemp, 0, sizeof(acTemp));
        sprintf((char *)acTemp, " Volt:  %04ld mV", Lib_BatGetVoltage());
        acTemp[strlen(acTemp)] = '\n';
        strcat(acBuf, acTemp);

        memset(acTemp, 0, sizeof(acTemp));
        if (!uiTotalSize)
        {
            sprintf((char *)acTemp, " %s: %s", "Free size", TIP_PLEASE_WAIT);
        }
        else
        {
            sprintf((char *)acTemp, " %s: %d KB(%d%%)", "Free size", uiFreeSize/1024, (uiFreeSize * 100)/uiTotalSize);
        }
        strcat(acBuf, acTemp);

        UITextAlign(ALIGN_L, 1, DISP_MODE_NORMAL, (char *)acBuf);

        sRC = LvUiWaitKey(1000);
        if ((KB_CANCEL == sRC))
        {
            return sRC;
        }
        else if (KB_ENTER == sRC)
        {
            // char *pstrSn2 = Lib_GetSn2Info();
            // memset(acTemp, 0, sizeof(acTemp));
            // if (memcmp(pstrSn2, DEV_PN, 4) != 0)
            // {
            //     sprintf((char *)acTemp, " Info: null");
            // }
            // else
            // {
            //     sprintf((char *)acTemp, " Info: %s", (char*)pstrSn2);
            // }
            // memset(acBuf, 0x00, sizeof(acBuf));
            // memcpy(acBuf, acTemp, strlen(acTemp));
            // UITextAlign(ALIGN_L, 1, DISP_MODE_NORMAL, (const char *)acBuf);
            // TRACE_LOG("info:%s\r\n", (char*)acTemp);
            // UIWaitKey(-1, 1);
            goto DEVINFO;
        }

        if (!uiTotalSize)
        {
            ulLen = sizeof(uiTotalSize);
            uiFreeSize = Lib_FileFreeSize();
            Lib_GetInfo(E_FILE_TOTAL_SIZE, &uiTotalSize, &ulLen);
        }
    }

    return RET_OK;
}

static short UIGprsNetModeCfg(int iTimeout, uint8_t flag);
static short UIPowerManage(int timeout);

//-----------------------------------------------------------------------------
// Name    : UIFwVer
// Func    : Display software version information menu
// Paras   : iTimeout[in],   timeout value, in milliseconds
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
short UIFwVer(int iTimeout)
{
    t_lvgl_image img = { 0 };
    unsigned short ucLen = 0;
    char acTemp[128];
    short sRC;
    char acBuf[1024] = { 0 };

    LvUilCls();

    img.bgImg = (char *)TITLE_BG_IMG;
    LvUiShowImage(MENU_COMMON_TITLEBACKGROUND_OFF_X, MENU_COMMON_TITLEBACKGROUND_OFF_Y, &img, -1);
    LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char*)MENU_VER_INFO, lv_color_hex(0xFFFFFF));
    Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

    // OS version
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_OS);
    ucLen = strlen((char *)acTemp);
    Lib_GetVersion((unsigned char *)(acTemp + ucLen), sizeof(acTemp) - ucLen, 0);
    acTemp[strlen(acTemp)] = '\n';
    strcat(acBuf, acTemp);

    // SDK version
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_SDK);
    ucLen = strlen((char *)acTemp);
    Lib_GetVersion((unsigned char *)(acTemp + ucLen), sizeof(acTemp) - ucLen, 1);
    acTemp[strlen(acTemp)] = '\n';
    strcat(acBuf, acTemp);

    // App version
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_APP);
    ucLen = strlen((char *)acTemp);
    Lib_GetVersion((unsigned char *)(acTemp + ucLen), sizeof(acTemp) - ucLen, 5);
    acTemp[strlen(acTemp)] = '\n';
    strcat(acBuf, acTemp);

    // se version
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_SE);
    ucLen = strlen((char *)acTemp);
    Lib_GetVersion((unsigned char *)(acTemp + ucLen), sizeof(acTemp) - ucLen, 2);
    acTemp[strlen(acTemp)] = '\n';
    strcat(acBuf, acTemp);

    // wifi Firmware version
    memset(acTemp, 0, sizeof(acTemp));
    sprintf((char *)acTemp, " %s: ", TIP_WIFI);
    ucLen = sizeof(acTemp) - strlen((char *)acTemp) -1;
    Lib_WifiGetVersion((char *)acTemp + strlen((char *)acTemp), &ucLen);
    TRACE("UISoftVer WIFI %s", acTemp);
    acTemp[strlen(acTemp)] = '\n';
    strcat(acBuf, acTemp);

    UITextAlign(ALIGN_L, 1, DISP_MODE_NORMAL, (char *)acBuf);

    while (1)
    {
        sRC = LvUiWaitKey(iTimeout);
        if ((LVGL_OPER_TIMEOUT == sRC) || (KB_CANCEL == sRC))
        {
            return sRC;
        }
    }

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : QueryListShow
// Func    : Display transaction details based on transaction records.
// Paras   : pstTradeRec[in],   Transaction record address
//         : usCurIndex[in],    Current transaction index
//         : usCount[in],       Total number of transaction records
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short QueryListShow(ST_TRADE_REC *pstTradeRec,  unsigned char usCurIndex, unsigned char usCount)
{
    char aucStr[32], aucTemp[50], aucName[64];
    unsigned short w, h, font, forecolor, backcolor;
    lv_obj_t *page = NULL;
    lv_obj_t *titleLabel = NULL;
    lv_obj_t *amountLabel = NULL;
    lv_obj_t *statusLabel = NULL;
    lv_obj_t *typeLabel = NULL;
    lv_obj_t *numberLabel = NULL;
    lv_obj_t *timeLabel = NULL;
    lv_obj_t *amountContentLabel = NULL;
    lv_obj_t *statusContentLabel = NULL;
    lv_obj_t *typeContentLabel = NULL;
    lv_obj_t *numberContentLabel = NULL;
    lv_obj_t *timeContentLabel = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    FUN_IN;

    LvUilCls();

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();
    parentWidth = lv_obj_get_width(page);

    lv_obj_set_flex_flow(page, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(page, 6, LV_PART_MAIN|LV_STATE_DEFAULT);

    titleLabel = lv_label_create(page);
    //set titleLabel
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    sprintf(aucTemp, "%s %d/%d", MENU_QUERY_DETAIL, usCurIndex + 1, usCount);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = aucTemp;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_DETAILS_IMFORMATION);
    LvSetCtrlObj(titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    amountLabel = lv_label_create(page);
    amountContentLabel = lv_label_create(page);

    statusLabel = lv_label_create(page);
    statusContentLabel = lv_label_create(page);

    typeLabel = lv_label_create(page);
    typeContentLabel = lv_label_create(page);

    numberLabel = lv_label_create(page);
    numberContentLabel = lv_label_create(page);

    timeLabel = lv_label_create(page);
    timeContentLabel = lv_label_create(page);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_DETAILS_IMFORMATION);
    stCtrlObjAttr.m_usWidth = parentWidth / 4;
    //set amount
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_MONEY;
    LvSetCtrlObj(amountLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set status
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_STATUS;
    LvSetCtrlObj(statusLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set type
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_OPCODE;
    LvSetCtrlObj(typeLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set number
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_TRANS_NO;
    LvSetCtrlObj(numberLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set time
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_TIME;
    LvSetCtrlObj(timeLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    //set amount content
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_RED;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_DETAILS_AMOUNT);
    stCtrlObjAttr.m_usWidth = parentWidth / 8 * 5;
    memset(aucStr, 0, sizeof(aucStr));
    BcdSum2AscSum(pstTradeRec->aucSum, (char*)aucStr);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = aucStr;
    LvSetCtrlObj(amountContentLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_DETAILS_IMFORMATION);
    stCtrlObjAttr.m_usWidth = parentWidth / 8 * 5;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
    //set status content
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_PAY_OK;
    LvSetCtrlObj(statusContentLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set type content
    memset(aucName, 0, 64);
    if (TT_QR_CONSUMPTION == (pstTradeRec->ulOpCode & TT_QR_CONSUMPTION))
    {
        memcpy(aucName, TIP_PAY_SCAN, strlen(TIP_PAY_SCAN));
    }
    else if (TT_CONSUMPTION == (pstTradeRec->ulOpCode & TT_CONSUMPTION))
    {
        memcpy(aucName, TIP_PAY_CARD, strlen(TIP_PAY_CARD));
    }
    else
    {
        memcpy(aucName, TIP_OTHERS, strlen(TIP_OTHERS));
    }
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = aucName;
    LvSetCtrlObj(typeContentLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set number content
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = "8554212122222";
    LvSetCtrlObj(numberContentLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set time content
    memset(aucStr, 0, sizeof(aucStr));
    memcpy(aucStr, pstTradeRec->aucDateH, 20);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = aucStr;
    LvSetCtrlObj(timeContentLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UIQueryList
// Func    : Display transaction records details
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
 //                       ==0, return immediately
 //                       <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIQueryList(int iTimeout)
{
    unsigned short usPos, usNum;
    short sRC;
    ST_TRADE_REC stTradeRec;

    FUN_IN;

    // get the total number of transaction records
    usNum = ApiGetRecNum();
    TRACE("usNum=%x\n", usNum);

    if (!usNum)
    {
        LvUiMessageBox(TIP_NO_RECORD, MB_CENTER |MB_COLOR_YELLOW|MB_OK, NULL, 3000);
        return RET_OK;
    }

    usPos = usNum - 1;

    // View transaction records in sequence
    while (1)
    {
        TRACE("usPos=%x\n", usPos);
        //UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);

        // Get transaction header
        sRC = ApiGetRecByIndex(&stTradeRec, usPos);
        if (RET_OK != sRC)
        {
            LvUiMessageBox(TIP_NO_RECORD, MB_CENTER |MB_COLOR_YELLOW|MB_OK, NULL, 3000);
        }
        else
        {
            // Display transaction details
            QueryListShow(&stTradeRec, usPos, usNum);
        }

        sRC = LvUiWaitKey(iTimeout);
        if (KB_UP == sRC)
        {
            if (usPos)
            {
                usPos--;
            }
            else
            {
                usPos = usNum-1;
            }
        }
        else if (KB_DOWN == sRC)
        {
            if (usPos < usNum-1)
            {
                usPos++;
            }
            else
            {
                usPos = 0;
            }
        }
        else if ((LVGL_OPER_TIMEOUT == sRC) || (KB_CANCEL == sRC))
        {
            //ApiCloseRecord();
            return sRC;
        }
    }
}


//-----------------------------------------------------------------------------
// Name    : UIQueryMenu
// Func    : Transaction Query Menu
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIQueryMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_QUERY_DETAIL, UIQueryList, timeout, NULL},
        {MENU_QUERY_TOTAL,  UIUnsupport, timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_QUERY_TITLE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

//-----------------------------------------------------------------------------
// Name    : UIEnterFwOta
// Func    : TMS Upgrade Main Menu
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
 //                       ==0, return immediately
 //                       <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIEnterFwOta(int timeout)
{
    short sRC;
    unsigned char ucSleepEnableF;

    STMenu menuList[] =
    {
        {MENU_OS_UPDATE,     UITmsGprsModuleDownOnline,    timeout, NULL},
#if (FUNCTION_TMS_ENABLE == 1)
        {MENU_APP_FW_UPDATE, UITmsAppModuleDownOnline,               timeout, NULL},
        {MENU_FILE_UPDATE,   UITmsFsModuleDownOnline,      timeout, NULL},
        {MENU_WIFI_UPDATE,   UITmsWifiModuleDownOnline,    timeout, NULL},
        {MENU_SE_UPDATE,     UITmsSeModuleDownOnline,      timeout, NULL},
#else
        {MENU_APP_FW_UPDATE, UIUnsupport,                  timeout, NULL},
#endif
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_FIRMWARE_OTA;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    ucSleepEnableF = DeviceSleepEnDisableGet();
    if (ucSleepEnableF)
    {
        DeviceSleepEnDisableSet(FALSE);
    }

    sRC = UICreatLvglMenu(timeout, &menu);

    DeviceSleepEnDisableSet(ucSleepEnableF);

    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : UIPosManageMenu
// Func    : General operator management sub menu.
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIPosManageMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_LOG_IN,       UIOnlineLogon,  timeout, (char *)LOGIN_IN_ICON},
        {MENU_LOG_OUT,      UIUnsupport,    timeout, (char *)LOGIN_OUT_ICON},
        {MENU_PAY_QUERY,    UIQueryMenu,    timeout, (char *)PAY_QUERY_ICON},
        {MENU_SETTLEMENT,   UIUnsupport,    timeout, (char *)SETTLEMENT_ICON},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_POS_MANAGE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}

//Display DispInfo on screen ucY line
static short UIDispInfo(unsigned char *DispInfo, unsigned char ucY, unsigned char ucDispMode)
{
    unsigned short usRgbForeTmp;
    unsigned short usRgbBackTmp;

    FUN_IN;

    if (ucY%2)
    {
        usRgbBackTmp = BACK565_MENU2;
    }
    else
    {
        usRgbBackTmp = DEFAULT_BACK_COLOR;
    }
    usRgbForeTmp = RGB565_BLACK;

    if (DISP_MODE_NORMAL != ucDispMode)
    {
        usRgbForeTmp = DEFAULT_FORG_COLOR;
        Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, usRgbForeTmp, usRgbBackTmp);
    }
    else
    {
        Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, usRgbForeTmp, usRgbBackTmp);
    }
    UITextAlign(ALIGN_L, ucY + 1, DISP_MODE_NORMAL, (char*)DispInfo);
    DRAW_DEFAULT();

    return RET_OK;
}

short UISelectProtocoOrAuthDisp(int iTimeout, unsigned char ucBit, char *title, char* pcStr, unsigned char *para)
{
    unsigned char ucSwpara = 0;
    unsigned char Dispcount = 0;
    unsigned char ucDispTempBuf[128];
    short sRC;
    lv_group_t *group = NULL;
    lv_obj_t *body = NULL;
    lv_obj_t *list = NULL;
    lv_obj_t *button[3];
    lv_obj_t *tipLabel = NULL;
    lv_obj_t *focusedObj = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;
    unsigned short w, h, f, forecolor, backcolor;
    unsigned char ucProtocolText[3][8] =
    {
        "IP",
        "IPV6",
        "IPV4V6"
    };
    unsigned char ucAuthType[3][8] =
    {
        "None",
        "PAP",
        "CHAP",
    };

    if (ucBit == 3)
    {
        Dispcount = ITEM_OF(ucProtocolText);
    }
    else if (ucBit == 4)
    {
        Dispcount = ITEM_OF(ucAuthType);
    }
    else
    {
        return RET_ERR;
    }

    Lib_LcdParaGet(&w, &h, &f, &forecolor, &backcolor);

    group = lv_group_create();
    body = LvUiCreateInputTextBody(FONT_SIZE_TEXT_INPUT_TITLE, title, NULL);
    lv_obj_update_layout(body);
    parentWidth = lv_obj_get_width(body);
    list = lv_list_create(body);
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    stCtrlObjAttr.m_usHeight = parentWidth / 2;
    LvSetCtrlObj(list, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_list_add_text(list, pcStr);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stPadAttr.m_usTop = 5;
    stCtrlObjAttr.m_stPadAttr.m_usRight = 5;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = 5;
    stCtrlObjAttr.m_stPadAttr.m_usBottom = 5;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 2;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
    stCtrlObjAttr.m_usRadius = 3;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_0;
    stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;

    for (int i = 0; i < Dispcount; i++)
    {
        //Show options
        memset(ucDispTempBuf, 0, sizeof(ucDispTempBuf));
        if (ucBit == 3)
        {
            sprintf((char*)ucDispTempBuf, "%d.%s", i + 1, ucProtocolText[i]);
        }
        else
        {
            sprintf((char*)ucDispTempBuf, "%d.%s", i + 1, ucAuthType[i]);
        }
        button[i] = lv_list_add_btn(list, NULL, (const char *)ucDispTempBuf);
        //set button style
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_0;
        LvSetCtrlObj(button[i], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
        LvSetCtrlObj(button[i], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_group_add_obj(group, button[i]);
    }
    tipLabel = lv_label_create(body);
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_APN_ENTE_NUM_SELECT;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    LvSetCtrlObj(tipLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    while (1)
    {
        lv_group_focus_obj(button[ucSwpara]);

        sRC = LvUiWaitKey (iTimeout);
        if (sRC == KB_CANCEL || sRC == LVGL_OPER_TIMEOUT)
        {
            return sRC;
        }
        else if (sRC >= KB_DIGIT_ONE && sRC <= KB_DIGIT_THREE)
        {
            ucSwpara = sRC - KB_DIGIT_ONE;
            continue;
        }
        else if (KB_ENTER == sRC)
        {
            focusedObj = lv_group_get_focused(group);
            *para = lv_obj_get_index(focusedObj);
            return sRC;
        }
    }
}

#define HOST_URL_SIZE (50)
#define URL_MAX_LEN (50)

//-----------------------------------------------------------------------------
// Name    : UISetAPNEx
// Func    : Set APN function
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
//         : ucBit[in],   Parameter mode bit
//         : pcStr[in],   Parameter string
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UISetAPNEx(int iTimeout, unsigned char ucBit, char *title, char *pcStr)
{
    static char ucApn[3][HOST_URL_SIZE + 1] = {0};
    short sRC;
    unsigned char ucParamLen;
    char acParam[HOST_URL_SIZE + 1];
    static unsigned char ucProtocol = 1;
    static unsigned char ucAuthType = 1;
    lv_obj_t* body = NULL;
    lv_obj_t* textArea = NULL;

    if (ucBit == 0)
    {
        // When first enter the settings page, restore the protocol type and authentication method to default values.
        // The default value of ucProtocol and ucAuthType is 1, which is determined according to 1, 2, and 3 displayed on the menu,
        // so ucAuthType needs to be reduced by one when setting.
        ucProtocol = 1;
        ucAuthType = 1;
    }

    if (ucBit == 3)
    {
        return UISelectProtocoOrAuthDisp(iTimeout, ucBit, title, pcStr, &ucProtocol);
    }
    else if (ucBit == 4)
    {
        sRC = UISelectProtocoOrAuthDisp(iTimeout, ucBit, title, pcStr, &ucAuthType);
        if (KB_ENTER == sRC)
        {
            // Set protocol and authentication method
            TRACE_LOG("ucProtocol = %d, ucAuthType = %d", ucProtocol, ucAuthType - 1);

            // clear screen
            UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES - 1);

            sRC = Lib_WlsSetApn((char*)&ucApn[0], (char*)&ucApn[1], (char*)&ucApn[2], ucProtocol, ucAuthType - 1);

            // Refresh the status bar to prevent from being white after the api exits.
//            UISetBarRefreshFlag(1);
            if (sRC < 0)
            {
                TRACE_LOG("set Apn fail, sRC = %d", sRC);
                LvUiMessageBox(TIP_SET_FAIL, MB_CENTER | MB_COLOR_RED | MB_ANYKEY, NULL, 3000);
                // Setting failed, return to next page
                return RET_ERR;
            }
            LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_COLOR_GREEN | MB_ANYKEY, NULL, 3000);
            return RET_OK;
        }
        else
        {
            return sRC;
        }
    }
    else
    {
        body = LvUiCreateInputTextBody(FONT_SIZE_TEXT_INPUT_TITLE, title, pcStr);

        if (ucBit == 2)
        {
            textArea = LvUiCreateInputTextArea(body, "", FONT_SIZE_TEXT_INPUT, LVGL_INPUT_MODE_PWD, 0, URL_MAX_LEN);
        }
        else
        {
            textArea = LvUiCreateInputTextArea(body, "", FONT_SIZE_TEXT_INPUT, LVGL_INPUT_MODE_TEXT, 0, URL_MAX_LEN);
        }

        while (1)
        {
            sRC = LvUiInputText(textArea, LVGL_INPUT_MODE_TEXT, 0, URL_MAX_LEN, iTimeout);

            if (sRC == LVGL_OPER_CANCEL || sRC == LVGL_OPER_TIMEOUT)
            {
                lv_obj_del(body);
                return sRC;
            }

            memset(acParam, 0, sizeof(acParam));
            strcpy(acParam, lv_textarea_get_text(textArea));
            ucParamLen = strlen(acParam);
            if (ucParamLen == 0)
            {
                LvUiMessageBox(TIP_PARA_CANNOT_EMPTY, MB_COLOR_YELLOW, NULL, 3000);
                continue;
            }

            memset(&ucApn[ucBit], 0, HOST_URL_SIZE + 1);
            memcpy(&ucApn[ucBit], acParam, ucParamLen);

            lv_obj_del(body);
            return KB_ENTER;
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : UISetAPN
// Func    : Set APN menu
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UISetAPN(char *title, int iTimeout)
{
    return UISetAPNEx(iTimeout, 0, title, TIP_APN_SET);
}

//-----------------------------------------------------------------------------
// Name    : UISetAPNName
// Func    : Set APN Name
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UISetAPNName(char *title, int iTimeout)
{
    return UISetAPNEx(iTimeout, 1, title, TIP_APN_USERNAME_SET);
}

//-----------------------------------------------------------------------------
// Name    : UISetAPNPW
// Func    : Set APN password
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UISetAPNPW(char *title, int iTimeout)
{
    return UISetAPNEx(iTimeout, 2, title, TIP_APN_PASSWORD_SET);
}

short UISetProtocol(char *title, int iTimeout)
{
    return UISetAPNEx(iTimeout, 3, title, TIP_APN_PROTOCOL_SET);
}

short UISetAuthType(char *title, int iTimeout)
{
    return UISetAPNEx(iTimeout, 4, title, TIP_APN_AUTH_TYPE_SET);
}

typedef short(*UI_FUN)(char *title, int iTimeout);
//-----------------------------------------------------------------------------
// Name    : UIGprsParamCfg
// Func    : Set APN password
// Paras   : iTimeout[in], timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
//                        ==0, return immediately
//                        <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIGprsParamCfg(int iTimeout)
{
    short sRC;
    UI_FUN aUiFun[] =
    {
        UISetAPN,               // APN
        UISetAPNName,           // apn name
        UISetAPNPW,             // apn password
        UISetProtocol,          // protocol
        UISetAuthType,          // Authentication Type
    };
    char acDisplayBuff[30];
    unsigned char i;
    unsigned char ucTotalItems;

    ucTotalItems = sizeof(aUiFun) / sizeof(UI_FUN);
    i = 0;
    while (1)
    {
        sprintf(acDisplayBuff, "%s [%d/%d]", MENU_APN_SET, (i + 1), ucTotalItems);
//        UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES - 1);
//        DRAW_TITLE(acDisplayBuff);
//        Lib_LcdRefresh();

        sRC = (aUiFun[i])(acDisplayBuff, iTimeout);
        TRACE_LOG("aUiFun[%d] sRC = %d", i, sRC);
        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT:
        case LVGL_OPER_CANCEL:
        case KB_CANCEL:
            return sRC;

        case KB_ENTER:
            i++;
            if (i >= ucTotalItems)
            {
                return RET_OK;
            }
            break;
        case RET_ERR:
            return RET_ERR;
        case RET_OK:
            return RET_OK;
        default:
            break;
        }
    }

    return RET_OK;
}

#define APN_DISP_MAX_NUMS (5)

static short UIGprsAPNSelect(int iTimeout)
{
    unsigned long ucApnListCount;           // The total number of APNs that need to be displayed
    static XML_APN_NODE_ST *pApnList = NULL;
    int i = 0;

    // 1.when entering the list selection, add a prompt to avoid long waiting caused by the menu stuck.
    if (pApnList == NULL)
    {
        LvUiMessageBox(TIP_GPRS_APN_LOADING, MB_COLOR_BLUE, NULL, 0);
    }

    pApnList = Lib_WlsGetAPNList(&ucApnListCount);
    // 2.Calculate how many pages need to be displayed
    if (ucApnListCount <= 0)
    {
        // No items were obtained
        return UIGprsParamCfg(iTimeout);
    }

    //lvgl code block
    {
        short sRC;
        int focusedIndex;
        unsigned char pDispBuf[APN_MAX_LEN + 16];
        lv_group_t *g = NULL;
        lv_obj_t *label = NULL;
        lv_obj_t *list = NULL;
        lv_obj_t *btn;
        lv_obj_t *btnLabel = NULL;
        lv_obj_t* page;
        lv_obj_t *focusedObj = NULL;
        unsigned short w, h, font, forecolor, backcolor;
        CTRL_OBJ_ATTR stCtrlObjAttr = {0};
        int parentWidth, parentHeight;

        Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

        //clear the screen
        LvUilCls();

        page = LvUiGetPageBody();
        parentWidth = lv_obj_get_width(page);
        parentHeight = lv_obj_get_height(page);

        g = lv_group_create();

        label = lv_label_create(page);
        list = lv_list_create(page);

        //label style
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_GPRS_APN_LIST;
        stCtrlObjAttr.m_y = parentHeight / 64;
        stCtrlObjAttr.m_usWidth = parentWidth;
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_LIST_TITLE);
        LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_update_layout(label);
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_y = parentHeight / 64 + lv_obj_get_height(label) + parentHeight / 64;
        stCtrlObjAttr.m_usWidth = parentWidth;
        stCtrlObjAttr.m_usHeight = parentHeight - stCtrlObjAttr.m_y - parentHeight / 64;
        stCtrlObjAttr.m_scrollbarMode = LV_SCROLLBAR_MODE_AUTO;
        stCtrlObjAttr.m_stPadAttr.m_usTop = stCtrlObjAttr.m_usHeight / 20;
        stCtrlObjAttr.m_stPadAttr.m_usBottom = stCtrlObjAttr.m_usHeight / 20;
        stCtrlObjAttr.m_stPadAttr.m_usLeft = stCtrlObjAttr.m_usHeight / 20;
        stCtrlObjAttr.m_stPadAttr.m_usRight = stCtrlObjAttr.m_usHeight / 20;
        LvSetCtrlObj(list, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(list, LV_OBJ_FLAG_EVENT_BUBBLE);

        for (i = 0; i < ucApnListCount; i++)
        {
            sprintf((char *)pDispBuf, "%d.%s", i + 1, pApnList[i].m_ascApn);
            btn = lv_list_add_btn(list, NULL, (const char *)pDispBuf);
            btnLabel = lv_obj_get_child(btn, 0);

            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_usHeight = parentHeight / 8;
            stCtrlObjAttr.m_stPadAttr.m_usBottom = stCtrlObjAttr.m_usHeight / 8;
            stCtrlObjAttr.m_stPadAttr.m_usLeft = stCtrlObjAttr.m_usHeight / 8;
            stCtrlObjAttr.m_stPadAttr.m_usRight = stCtrlObjAttr.m_usHeight / 8;
            stCtrlObjAttr.m_stPadAttr.m_usTop = stCtrlObjAttr.m_usHeight / 8;
            stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLACK;
            stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
            stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
            stCtrlObjAttr.m_stBorderAttr.m_usWidth = 2;
            LvSetCtrlObj(btn, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

            stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
            stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
            LvSetCtrlObj(btn, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_FOCUSED);

            lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_obj_add_event_cb(btn, LvUilistButEventhandler, LV_EVENT_CLICKED, (void *)KB_ENTER);

            lv_group_add_obj(g, btn);

            if (i == 0)
            {
                lv_group_focus_obj(btn);
            }

            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_ucBidiFlag = TRUE;
            stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_SMALL);
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            LvSetCtrlObj(btnLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        while (1)
        {
            sRC = LvUiWaitKey(iTimeout);

            if (sRC == KB_CANCEL)
            {
                return sRC;
            }
            else if (sRC == LVGL_OPER_TIMEOUT)
            {
                return sRC;
            }
            else if (KB_UP == sRC)
            {
                lv_group_focus_prev(g);
            }
            else if (KB_DOWN == sRC)
            {
                lv_group_focus_next(g);
            }
            else if (KB_FUN == sRC)
            {
                return UIGprsParamCfg(iTimeout);
            }
            else if (KB_ENTER == sRC)
            {
                focusedObj = lv_group_get_focused(g);
                focusedIndex = lv_obj_get_index(focusedObj);

                TRACE_LOG("sCurrItem = %d, pApnList[%d].m_ucAuth = %d, pApnList[].m_ucProtocol = %d", focusedIndex, focusedIndex, pApnList[focusedIndex].m_ucAuth, pApnList[focusedIndex].m_ucProtocol);

                sRC = Lib_WlsSetApn((char*)pApnList[focusedIndex].m_ascApn, (char*)pApnList[focusedIndex].m_ascUser, (char*)pApnList[focusedIndex].m_ascPwd, pApnList[focusedIndex].m_ucProtocol, pApnList[focusedIndex].m_ucAuth);

                TRACE_LOG("Set Apn Auth sRC = %d", sRC);
                if (sRC < 0)
                {
                    FUN_IN_ERR;
                    TRACE_LOG("Set Apn fail sRC = %d", sRC);
                    LvUiMessageBox(TIP_SET_FAIL, MB_CENTER | MB_COLOR_RED | MB_ANYKEY, NULL, 3000);
                    // Setting failed, return
                    return RET_ERR;
                }

                LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_COLOR_GREEN | MB_ANYKEY, NULL, 3000);
                return RET_OK;
            }
        }
    }
}

static short UISetDNSMain(char *title, int iTimeout)
{
    return UISetParaByIpInput(0, title, iTimeout);
}

static short UISetDNSBack(char *title, int iTimeout)
{
    return UISetParaByIpInput(1, title, iTimeout);
}

//-----------------------------------------------------------------------------
// Name    : UIDNSParamCfg
// Func    : Configure DNS parameters
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIDNSParamCfg(int iTimeout)
{
    short sRC;
    UI_FUN aUiFun[] =
    {
        UISetDNSMain,             // DNS host
        UISetDNSBack,             // DNS backup
    };
    char acDisplayBuff[48];
    unsigned char i;
    unsigned char ucTotalItems;

    ucTotalItems = sizeof(aUiFun) / sizeof(UI_FUN);
    i = 0;
    while (1)
    {
        sprintf(acDisplayBuff, "%s [%d/%d]", MENU_DNS_SET, (i + 1), ucTotalItems);

        sRC = (aUiFun[i])(acDisplayBuff, iTimeout);
        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT:
        case KB_CANCEL:
            return sRC;

        case KB_UP:
            if (i > 0)
            {
                i--;
            }
            else
            {
                i = (ucTotalItems - 1);
            }
            break;
        case KB_ENTER:
        case KB_DOWN:
            i++;
            if (i >= ucTotalItems)
            {
                i = 0;
            }
            break;
        default:
            break;
        }
    }
}

short UIGprsApnInfo(int iTimeout)
{
    // View APN information menu
    unsigned char ApnInfo[128] = {0};                   // cache buffer for Obtaining APN information
    unsigned char ucDispTempBuf[128] = {0};             // buf for displaying on the menu
    unsigned char ucProtocol = 0xFF, ucAuthType = 0xFF; // Protocol type and encryption type, the default is a large number
    unsigned char ucProtocolBuf[30] = {0}, ucAuthBuf[30] = {0};
    short sRC;
    t_lvgl_image img = { 0 };

    FUN_IN;
    // Get APN information
    Lib_WlsGetApn((char *)ApnInfo, sizeof(ApnInfo));

    // Get protocol and encryption type
    Lib_WlsGetApnAuth((unsigned char *)&ucProtocol, (unsigned char *)&ucAuthType);

    TRACE_LOG("APN :%s, ucProtocol = %d, ucAuthType = %d", ApnInfo, ucProtocol, ucAuthType);
    // title

    LvUilCls();

    memset(ucDispTempBuf, 0, 128);
    sprintf((char *)ucDispTempBuf, "%s", TIP_GPRS_APN_INFO);


    img.bgImg = TITLE_BG_IMG;
    LvUiShowImage(MENU_COMMON_TITLEBACKGROUND_OFF_X, MENU_COMMON_TITLEBACKGROUND_OFF_Y, &img, -1);
    LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char *)ucDispTempBuf, lv_color_hex(0xFFFFFF));

    //APN
    memset(ucDispTempBuf, 0, sizeof(ucDispTempBuf));
    sprintf((char *)ucDispTempBuf, "%s: %s",MENU_APN, ApnInfo);
    UIDispInfo(ucDispTempBuf, 2, DISP_MODE_NORMAL);

    switch(ucProtocol)
    {
        case 1:
            memcpy(ucProtocolBuf, TIP_GPRS_APN_IP, strlen(TIP_GPRS_APN_IP));
            break;
        case 2:
            memcpy(ucProtocolBuf, TIP_GPRS_APN_IPV6, strlen(TIP_GPRS_APN_IPV6));
            break;
        case 3:
            memcpy(ucProtocolBuf, TIP_GPRS_APN_IPV4V6, strlen(TIP_GPRS_APN_IPV4V6));
            break;
        default:
            memset(ucProtocolBuf, 0, sizeof(ucProtocolBuf));
            break;
    }

    switch(ucAuthType)
    {
        case 0:
            memcpy(ucAuthBuf, TIP_GPRS_APN_NONE, strlen(TIP_GPRS_APN_NONE));
            break;
        case 1:
            memcpy(ucAuthBuf, TIP_GPRS_APN_PAP, strlen(TIP_GPRS_APN_PAP));
            break;
        case 2:
            memcpy(ucAuthBuf, TIP_GPRS_APN_CHAP, strlen(TIP_GPRS_APN_CHAP));
            break;
        default:
            memset(ucAuthBuf, 0, sizeof(ucAuthBuf));
            FUN_IN;
            break;
    }

    // protocol
    memset(ucDispTempBuf, 0, sizeof(ucDispTempBuf));
    sprintf((char *)ucDispTempBuf, "%s %s", TIP_GPRS_APN_PROCOTOL, ucProtocolBuf);
    UIDispInfo(ucDispTempBuf, 0, DISP_MODE_NORMAL);

    // Encryption type
    memset(ucDispTempBuf, 0, sizeof(ucDispTempBuf));
    sprintf((char *)ucDispTempBuf, "%s %s", TIP_GPRS_APN_AUTH_TYPE, ucAuthBuf);
    UIDispInfo(ucDispTempBuf, 1, DISP_MODE_NORMAL);

    while (1)
    {
        sRC = LvUiWaitKey(iTimeout);
        if ((LVGL_OPER_TIMEOUT == sRC) || (KB_CANCEL == sRC))
        {
            return sRC;
        }
    }

    return RET_OK;
}

short UIGprsAPNAutoMatch(int timeout)
{
    int iRet;

    // clear screen
//    UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);

    // Query network nitialization status
    iRet = Lib_WlsGetInitStatus();
    if (iRet == WLS_OK)
    {
        // if network initialization is successful,exit
        TRACE_LOG("SIM card init success");
        LvUiMessageBox(TIP_SIM_INIT_SUCC, MB_CENTER|MB_COLOR_GREEN, NULL, 3000);
        return RET_OK;
    }
    else if (iRet == WLS_NOSIM)
    {
        TRACE_LOG("not sim!!");
        LvUiMessageBox(TIP_INSERT_SIM_CARD, MB_CENTER|MB_ICONERROR, NULL, 3000);
        return RET_ERR;
    }

    Lib_WlsSetAdaptiveAPN(1);

    // Refresh the status bar to prevent from being white after the menu exits.
//    UISetBarRefreshFlag(1);

    return RET_OK;
}

short UICommApnInfo(int timeout)
{
    STMenu menuList[] =
    {
        {TIP_GPRS_APN_INFO,        UIGprsApnInfo,      timeout, NULL},
        {MENU_APN_SET,             UIGprsAPNSelect,    timeout, NULL},
        {TIP_GPRS_APN_AUTH_MATCH,  UIGprsAPNAutoMatch, timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_APN;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

//-----------------------------------------------------------------------------
// Name    : UICommParamSet
// Func    : Configure common parameters
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UICommParamSet(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_APN, UICommApnInfo, timeout, NULL},
        {MENU_DNS, UIDNSParamCfg, timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_APN_DNS;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}


//-----------------------------------------------------------------------------
// Name    : UIConsume
// Func    : Consumption menu
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIConsume(int iTimeout)
{
    short sRC;

    sRC = UIOnlineTrade(iTimeout, TT_CONSUMPTION);
    return sRC;
}


//-----------------------------------------------------------------------------
// Name    : UIQRCodePayMenu
// Func    : QR code consumption transaction menu
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIQRCodePayMenu(int iTimeout)
{
    short sRC;

    CamEnableControl(1);
    sRC = UIOnlineBSQrcodeTrade(iTimeout, TT_QR_CONSUMPTION);
    CamEnableControl(0);

    return sRC;
    }

//----------------------------------add by leeon--------------------------------

t_OperatorTable s_OperatorList[16] = {};


//-----------------------------------------------------------------------------
// Name    : UIEnterGprsModeCfg
// Func    : GPRS mode settings
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIEnterGprsModeCfg(int timeout)
{
    return UIGprsNetModeCfg(timeout, Lib_WlsGetNetMode() >> 1);
}

short UISetPrintGrade(int iTimeout)
{
    return UIFuntionSlider(MENU_PRINTER_GRADE, PRINT_GRADE_MAX, E_PRINTER_GRADE, iTimeout);
}

static short UISetHost1IP(char *title, int iTimeout)
{
    return UISetParaByIpInput(HOST1IP_BIT, title, iTimeout);
}

static short UISetHost1Port(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_SERVER_PORT_MAIN, HOST1PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetHost2IP(char *title, int iTimeout)
{
    return UISetParaByIpInput(HOST2IP_BIT, title, iTimeout);
}

static short UISetHost2Port(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_SERVER_PORT_BACK, HOST2PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetPospServerName(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_DOMAIN_NAME_MAIN, NET_NAME_POSP_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
}

static short UISetPospServerNamePort(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_DOMAIN_PORT_MAIN, NET_NAME_POSP_PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetPospServerName2(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_DOMAIN_NAME_BACK, NET_NAME_POSP_2_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
}

static short UISetPospServerNamePort2(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_DOMAIN_PORT_BACK, NET_NAME_POSP_PORT_2_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetTmsHost1IP(char *title, int iTimeout)
{
    return UISetParaByIpInput(TMS1IP_BIT, title, iTimeout);
}

static short UISetTmsHost1Port(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_SERVER_PORT_MAIN, TMS1PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetTmsHost2IP(char *title, int iTimeout)
{
    return UISetParaByIpInput(TMS2IP_BIT, title, iTimeout);
}

static short UISetTmsHost2Port(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_SERVER_PORT_BACK, TMS2PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetTmsServerName(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_DOMAIN_NAME_MAIN, NET_NAME_TMS_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
//    return UISetHostAddr(TIP_TMS_DOMAIN_NAME_MAIN, NET_NAME_TMS_BIT, iTimeout);
}

static short UISetTmsServerNamePort(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_DOMAIN_PORT_MAIN, NET_NAME_TMS_PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetTmsServerName2(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_DOMAIN_NAME_BACK, NET_NAME_TMS_2_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
//    return UISetHostAddr(TIP_TMS_DOMAIN_NAME_BACK, NET_NAME_TMS_2_BIT, iTimeout);
}

static short UISetTmsServerNamePort2(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_DOMAIN_PORT_BACK, NET_NAME_TMS_PORT_2_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

static short UISetModuleServerName(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_OS_DOMAIN_NAME_MAIN, NET_NAME_MODULE_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
//    return UISetHostAddr(TIP_TMS_OS_DOMAIN_NAME_MAIN, NET_NAME_MODULE_BIT, iTimeout);
}

static short UISetModuleServerNamePort(char *title, int iTimeout)
{
    return UISetParaByTextAreaInfoInput(title, TIP_TMS_OS_DOMAIN_PORT_MAIN, NET_NAME_MODULE_PORT_BIT, LVGL_INPUT_MODE_NUM, NETWORK_PORT_SIZE, iTimeout);
}

//-----------------------------------------------------------------------------
// Name    : UINetParaCfg
// Func    : Configure network parameters
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until the key is pressed.
// Ret     : - UI_INPUT_CANCEL
//         : - UI_INPUT_TIMEOUT
//         : - UI_INPUT_UP
//         : - UI_INPUT_DOWN
// Desc    :
//-----------------------------------------------------------------------------
static short UINetParaCfg(int iTimeout)
{
    short sRC = RET_OK;
    unsigned char i = 0;
    unsigned char ucTotalItems = 0;
    char displayBuff[64] = {0};
    static UI_FUN aUiFun[] = {
        UISetHost1IP,             // host1 IP
        UISetHost1Port,           // host1 port
        UISetHost2IP,             // host2 IP
        UISetHost2Port,           // host2 port
        UISetPospServerName,      // POSP domain url
        UISetPospServerNamePort,  // POSP domain port
        UISetPospServerName2,     // POSP Backup domain url
        UISetPospServerNamePort2, // POSP Backup domain port
    };

    ucTotalItems = sizeof(aUiFun) / sizeof(UI_FUN);

    if (ucTotalItems == 0)
    {
        return RET_ERR;
    }

    while (1)
    {
        sprintf((char *)displayBuff, "%s [%d/%d]", TIP_POSP_CFG, i + 1, ucTotalItems);

        sRC = (aUiFun[i])(displayBuff, iTimeout);

        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT :
        case KB_CANCEL :
            return sRC;

        case KB_UP :
            if(i > 0)
            {
                i--;
            }
            else
            {
                i = (ucTotalItems - 1);
            }
            break;
        case KB_DOWN :
            i++;
            if(i >= ucTotalItems)
            {
                i = 0;
            }
            break;
        default :
            break;
        }
    }

    return sRC;
}

static short UITmsParaCfg(int iTimeout)
{
    short sRC = RET_OK;
    unsigned char i = 0;
    unsigned char ucTotalItems = 0;
    char displayBuff[64] = {0};
    static UI_FUN aUiFun[] = {
        UISetTmsHost1IP,            // TMS1 IP
        UISetTmsHost1Port,          // TMS1 port
        UISetTmsHost2IP,            // TMS2 IP
        UISetTmsHost2Port,          // TMS2 port
        UISetTmsServerName,         // TMS domain url
        UISetTmsServerNamePort,     // TMS domain port
        UISetTmsServerName2,        // TMS Backup domain url
        UISetTmsServerNamePort2,    // TMS Backup domain port
    };

    ucTotalItems = sizeof(aUiFun) / sizeof(UI_FUN);

    if(ucTotalItems == 0)
    {
        return RET_ERR;
    }

    while(1)
    {
        sprintf((char *)displayBuff, "%s [%d/%d]", TIP_TMS_CFG, i + 1, ucTotalItems);

        sRC = (aUiFun[i])(displayBuff, iTimeout);

        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT :
        case KB_CANCEL :
            return sRC;

        case KB_UP :
            if(i > 0)
            {
                i--;
            }
            else
            {
                i = (ucTotalItems - 1);
            }
            break;
        case KB_DOWN :
            i++;
            if(i >= ucTotalItems)
            {
                i = 0;
            }
            break;
        default :
            break;
        }
    }

    return sRC;
}

static short UITmsModuleParaCfg(int iTimeout)
{
    short sRC = RET_OK;
    unsigned char i = 0;
    unsigned char ucTotalItems = 0;
    char displayBuff[64] = {0};
    static UI_FUN aUiFun[] = {
        UISetModuleServerName,          //TMS Server Domain Name
        UISetModuleServerNamePort,      //TMS Server Domain Port
    };

    ucTotalItems = sizeof(aUiFun) / sizeof(UI_FUN);

    if(ucTotalItems == 0)
    {
        return RET_ERR;
    }

    while(1)
    {
        sprintf((char *)displayBuff, "%s [%d/%d]", MENU_TMS_OTHER_SERVER, i + 1, ucTotalItems);

        sRC = (aUiFun[i])(displayBuff, iTimeout);

        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT :
        case KB_CANCEL :
            return sRC;

        case KB_UP :
            if(i > 0)
            {
                i--;
            }
            else
            {
                i = (ucTotalItems - 1);
            }
            break;
        case KB_DOWN :
            i++;
            if(i >= ucTotalItems)
            {
                i = 0;
            }
            break;
        default :
            break;
        }
    }

    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : UISetPospServerMode
// Func    : Set the access method for the POSP server
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     : - UI_INPUT_CANCEL
//         : - UI_INPUT_TIMEOUT
//         : - UI_INPUT_UP
//         : - UI_INPUT_DOWN
// Desc    :
//-----------------------------------------------------------------------------
short UISetPospServerMode(int iTimeout)
{

    return UISetParaByTextAreaInfoInput(TIP_POSP_MODE_SETTING, TIP_IS_USE_DOMAIN, POSP_SERVER_MODE_BIT, LVGL_INPUT_MODE_NUM, SERVER_MODE_SIZE, iTimeout);
}

short UISetTmsServerMode(int iTimeout)
{
    return UISetParaByTextAreaInfoInput(TIP_TMS_MODE_SETTING, TIP_IS_USE_DOMAIN, TMS_SERVER_MODE_BIT, LVGL_INPUT_MODE_NUM, SERVER_MODE_SIZE, iTimeout);
}

static short UITmsServerCfg(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_TMS_APP_SERVER,      UITmsParaCfg,        timeout, NULL},
        {MENU_TMS_APP_SERVER_MODE, UISetTmsServerMode,  timeout, NULL},
        {MENU_TMS_OTHER_SERVER,    UITmsModuleParaCfg,  timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_TMS;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}


short UIBrightSet(int iTimeout)
{
    return UIFuntionSlider(MENU_BRIGHTNESS, BRIGHT_LEVEL_MAX, E_BRIGHTNESS, iTimeout);
}


short UIBeepSet(int iTimeout)
{
    int ret = -1;

    ret = LvUiSwitch(TIP_BEEP_SETTING, iTimeout, BEEP_BIT, BEEPON_ICON, BEEPOFF_ICON);

    if (ret != LVGL_OPER_SUCC)
    {
        return RET_ERR;
    }

    return RET_OK;
}

static short UIColorSet(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_THEME_BLUE,   UISelectThemeColor, THEME_COLOR_BLUE,   NULL},
        {MENU_THEME_ORANGE, UISelectThemeColor, THEME_COLOR_ORANGE, NULL},
        {MENU_THEME_RED,    UISelectThemeColor, THEME_COLOR_RED,    NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_THEME_TITLE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

static short UIGprsNetModeCfg(int iTimeout, uint8_t flag)
{
    short sRC;
    char buff[128];
    char *menu[] =
    {
        TIP_NET_MODE_AUTO,
        TIP_NET_MODE_2G,
        TIP_NET_MODE_4G
    };

    FUN_IN;

    while (1)
    {
        ShowUiMenuPackY(MENU_SELECT_MODE, menu, ITEM_OF(menu), flag);
        sRC = LvUiWaitKey(iTimeout);

        switch(sRC)
        {
            case LVGL_OPER_TIMEOUT:
            case KB_CANCEL:
                return sRC;

            case KB_DOWN:
                flag ++;
                flag = flag % 3;
                break;

            case KB_UP:
                if(flag == 0)
                {
                    flag = 2;
                }
                else
                {
                    flag --;
                }
                break;

            case KB_DIGIT_ONE:
            case KB_DIGIT_TWO:
            case KB_DIGIT_THREE:
                flag = sRC - '1';
                break;

            case 1:
            case 2:
            case 3:
                flag = sRC - 1;
                break;

            case KB_ENTER:
                memset(buff, 0, sizeof(buff));
                strcpy(buff, "\n");
                strcat(buff, menu[flag]);
                strcat(buff, " ");
                strcat(buff, TIP_OK);
                Lib_WlsSetNetMode(flag*2);
                LvUiMessageBox(buff, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, NULL, 3000);
                break;

            default:
                break;
        }

    }

    return RET_OK;
}

static short UIEnterMaintenanceMode(int timeout)
{
    LvUiCtrlStatusBar(1);
    LvUilCls();
    Lib_MaintenanceMode();
    LvUiCtrlStatusBar(0);
    LvUilCls();
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UIDebugSwitchSet
// Func    : Set device debugging switch
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIDebugSwitchSet(int timeout)
{
    short ret = 0;
    char *menuName[] =
    {
        TIP_OFF_EN,                 // Turn off debugging infos switch
        TIP_EN_TO_DEBUG_TOOLS,      // Open debugging infos switch and output it only to the coolwatcher
        TIP_EN_TO_FILE,             // Open debugging infos switch, output it to the coolwatcher, and save it into file
        TIP_EN_TO_UPLOAD_FILE,      // Open debugging infos switch, output it to the coolwatcher, and upload this file
    };
    char buff[128];
    unsigned char flag = GetTraceFlag();

    while (1)
    {
        if (ret != LVGL_BUTTON_SELECTED_FOR_REFRESH)
        {
            ShowUiMenuPackY(MENU_DEBUG_SWITCH, menuName, ITEM_OF(menuName), flag);
        }

        ret = LvUiWaitKey(timeout);
        switch (ret)
        {
            case KB_CANCEL:
            case LVGL_OPER_TIMEOUT:
                return RET_ERR;

            case KB_DOWN:
                flag ++;
                flag = flag % 4;
                break;

            case KB_UP:
                if (flag == 0)
                {
                    flag = 3;
                }
                else
                {
                    flag --;
                }
                break;

            case KB_DIGIT_ONE:
            case KB_DIGIT_TWO:
            case KB_DIGIT_THREE:
            case KB_DIGIT_FOUR:
                flag = ret - '1';
                break;

            case 1:
            case 2:
            case 3:
            case 4:
                flag = ret - 1;
                break;

            case KB_ENTER:
                memset(buff, 0, sizeof(buff));
                strcpy(buff, "\n");
                strcat(buff, menuName[flag]);
                strcat(buff, "\n");
                strcat(buff, TIP_OK);
                SetTraceLogConfig(flag);
                LvUiMessageBox(buff, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, NULL, 3000);
                break;

            default:
                break;
        }
    }

    return RET_OK;
}

short UIGpsMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_GPS_SATELLITE,    UIGpsSatInfo,       timeout, NULL},
        {MENU_GPS_SIGNAL,       UIGpsSignalInfo,    timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_GPS;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}


//-----------------------------------------------------------------------------
// Name    : UIMoreMenu
// Func    : UI more Menu
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIMoreMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_MAINTENANCE,  UIEnterMaintenanceMode, timeout, (char *)MAINTENNANCE_ICON},
        {MENU_DEBUG_SWITCH, UIDebugSwitchSet,       timeout, (char *)DEBUG_SW_ICON},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_MORE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}

void arcEventHandler(lv_event_t * e)
{
    lv_obj_t * currentObj = lv_event_get_current_target(e);
    lv_obj_t * textArea = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    char timeBuf[10] = {0};
    int value;

    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
        value = lv_arc_get_value(currentObj);
        sprintf(timeBuf, "%d", value);
        lv_textarea_set_text(textArea, timeBuf);
        break;
    case LV_EVENT_RELEASED:
        BeepOn();
        break;
    default:
        break;
    }
}

static short UISetBacklightTime(int timeout)
{
    return UISetTime(TIP_POWER_BACKLIGHT_TIME_SETTING, SDK_TIME_BACKLIGHT_LCD_MIN, SDK_TIME_BACKLIGHT_LCD_MAX, E_BLACKLIGHT_TIME, timeout);
}

static short UISetSleepTime(int timeout)
{
    return UISetTime(TIP_POWER_SLEEP_TIME_SETTING, SDK_TIME_SEC_SLEEP_MIN, SDK_TIME_SEC_SLEEP_MAX, E_SLEEP_TIME, timeout);
}

static short UISetPowerOffTime(int timeout)
{
    return UISetTime(TIP_POWER_POWEROFF_TIME_SETTING, SDK_TIME_MIN_POWER_OFF_MIN, SDK_TIME_MIN_POWER_OFF_MAX, E_POWER_OFF_TIME, timeout);
}

static short UIPowerManage(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_BACKLIGHT_TIME,   UISetBacklightTime,     timeout, NULL},
        {MENU_SLEEP_TIME,       UISetSleepTime,         timeout, NULL},
        {MENU_POWEROFF_TIME,    UISetPowerOffTime,      timeout, NULL},
        {MENU_BRIGHTNESS,       UIBrightSet,            timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_POWER_MANAGE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

//-----------------------------------------------------------------------------
// Name  : SystemInit
// Func  :
// Paras :
// ret   :
// desc  :
//-----------------------------------------------------------------------------
int SystemInit(void)
{
    g_teminal_param.ucBrightLevel = 5;
    g_teminal_param.beep_en = 1;

    Lib_SysInit();
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UIAboutMenu
// Func    : UI about Menu
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIAboutMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_DEV_INFO, UIDevInfo,  timeout, (char *)DEVICE_INFO_ICON},
        {MENU_VER_INFO, UIFwVer,    timeout, (char *)VERSION_INFO_ICON},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_ABOUT;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}


static short UIDispDemo(int timeout)
{

    LvUiWaitKey(60 * 1000);

    return 0;
}

short UIPOSPMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_SERVER,       UINetParaCfg,           timeout, NULL},
        {MENU_SERVER_MODE,  UISetPospServerMode,    timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_POSP;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

short UIServersMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_POSP, UIPOSPMenu,     timeout, NULL},
        {MENU_TMS,  UITmsServerCfg, timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_SERVERS;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

short UISystemManageMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_SERVERS,      UIServersMenu,  timeout, SERVERS_ICON},
        {MENU_OTA,          UIEnterFwOta,   timeout, OTA_ICON},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_SYSTEM_MANAGE;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}

short UISystemSettingMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_NETWORK,          UINetCfgMenu,   timeout, (char *)NETWORK_ICON},
        {MENU_POWER_MANAGE,     UIPowerManage,  timeout, (char *)POWERMANAGE_ICON},
        {MENU_SYSTIME_SETTING,  UISysTimeSet,   timeout, (char *)SYSTEM_TIME_ICON},
        {MENU_BEEP,             UIBeepSet,      timeout, (char *)BEEP_ICON},
        {MENU_THEME_COLOR,      UIColorSet,     timeout, (char *)THEMECOLOR_ICON},
#if (MULTI_LANGUAGE_EN == 1)
        {MENU_LANGUAGE_SETTING, LanguageMenuSet,timeout, (char *)LANGUAGE_ICON},
#endif
#if (HW_TPH)
        {MENU_PRINTER_GRADE,    UISetPrintGrade,timeout, (char *)PRINTERGRADE_ICON},
#endif
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_SYSTEM_SETTING;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}

short UIEsigTest(int timeout)
{
#if HW_TPH
    unsigned char key = KB_NULL;
#endif
    unsigned char esignData[1024] = {0};
    char *tipInfo = NULL;
    int ret = RET_OK;

    ret = Lib_TpGetESignData(esignData, "12345678", timeout);
    if (ret != RET_OK)
    {
        tipInfo = TIP_SIGN_ERR;
        goto Result;
    }

#if HW_TPH
    while (1)
    {
        // Clear screen
        LvUilCls();
        ret = Lib_PrnOpen();
        if (ret != RET_OK)
        {
            tipInfo = TIP_PRN_OPEN_FAIL;
            goto Result;
        }

        UITextAlign(ALIGN_M, 4, DISP_MODE_NORMAL, TIP_PRN_PRINTING);
        ret = Lib_PrnEsig(esignData);
        if (ret != RET_OK)
        {
            tipInfo = TIP_SIGN_FAIL;
            Lib_PrnClose();
            goto Result;
        }

        ret = Lib_PrnClose();
        if (ret == RET_OK)
        {
            tipInfo = TIP_SIGN_SUCC;
            goto Result;
        }
        else if (ret == RET_ERR_TPH_NO_PAPER)
        {
            LvUilCls();
            UITextAlign(ALIGN_M, 3, DISP_MODE_NORMAL, TIP_PRN_PAPER_OUT);
            UITextAlign(ALIGN_M, 4, DISP_MODE_NORMAL, TIP_PRN_PAPER_ADD);
            UITextAlign(ALIGN_M, 5, DISP_MODE_NORMAL, TIP_CANCEL_EXIT);
            while (1)
            {
                key = Lib_KbGetKey();
                if (key == KB_CANCEL)
                {
                    // Directly exit without displaying any information
                    tipInfo = NULL;
                    goto Result;
                }
                else if (key == KB_ENTER)
                {
                    LvUilCls();
                    UITextAlign(ALIGN_M, 4, DISP_MODE_NORMAL, TIP_PRN_PRINTING);
                    break;
                }
            }
        }
        else
        {
            // Other errors
            tipInfo = TIP_PRN_ERR;
            break;
        }
    }
#else
    if (RET_OK == ret)
    {
        // Directly exit without displaying any information
        tipInfo = TIP_SIGN_SUCC;
        goto Result;
    }
#endif

Result:
    if (tipInfo != NULL)
    {
        LvUilCls();
        UITextAlign(ALIGN_M, MAX_COM_LINES - 2, DISP_MODE_NORMAL, tipInfo);
        UIWaitKey(2000, 1);
    }
    return (short)ret;
}

#if HW_TPH
//-----------------------------------------------------------------------------
// Name    : UIPrintTest
// Func    : Print purchase receipt
// Paras   : iTimeout[in],  timeout value, unit: milliseconds. range: -1~655350
//                          >0,  Wait for the keys to be pressed, and return after timeout
//                          ==0, return immediately
//                          <0,  Always wait until CANCEL is pressed.
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIPrintTest(int timeout)
{
    static unsigned char printGrade = 0;
    unsigned char ucCurGrade = Lib_PrnGetGrade();

    printGrade++;
    if (printGrade > PT_PRINT_GRADE_MAX)
        printGrade = 1;
    Lib_PrnSetGrade(printGrade);
    PrintReceipt();

    Lib_PrnSetGrade(ucCurGrade);
    return RET_OK;
}
#endif

short UITestMenu(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_ESIGN,            UIEsigTest,     timeout,    NULL},
#if HW_TPH
        {MENU_PRINT,            UIPrintTest,    timeout,    NULL},
#endif
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_TEST;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_1;

    return UICreatLvglMenu(timeout, &menu);
}

void mainMenu(int iTimeout)
{
    int key = KB_NULL;
    t_lvgl_image img = { 0 };

    while (1)
    {
        TRACE("lvgl_main menu");
        Lib_PrintfHeapInfo();

        LvUilCls();

        img.bgImg = (char *)MAINBG_IMG;
        LvUiShowImage(MENU_COMMON_BACKGROUND_OFF_X, MENU_COMMON_BACKGROUND_OFF_Y, &img, -1);

        img.bgImg = (char *)CONSUME_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_ONE_OFF_X, MENU_LEVEL_ONE_LINE_ONE_OFF_Y, &img, KB_DIGIT_ONE);

        img.bgImg = (char *)REFUND_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_TWO_OFF_X, MENU_LEVEL_ONE_LINE_ONE_OFF_Y, &img, KB_DIGIT_TWO);

        img.bgImg = (char *)QROCDEPAY_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_THREE_OFF_X, MENU_LEVEL_ONE_LINE_ONE_OFF_Y, &img, KB_DIGIT_THREE);

        img.bgImg = (char *)POSMANAGE_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_ONE_OFF_X, MENU_LEVEL_ONE_LINE_TWO_OFF_Y, &img, KB_DIGIT_FOUR);

        img.bgImg = (char *)SYSSETTING_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_TWO_OFF_X, MENU_LEVEL_ONE_LINE_TWO_OFF_Y, &img, KB_DIGIT_FIVE);

        img.bgImg = (char *)SYSMANAGE_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_THREE_OFF_X, MENU_LEVEL_ONE_LINE_TWO_OFF_Y, &img, KB_DIGIT_SIX);

        img.bgImg = (char *)TEST_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_ONE_OFF_X, MENU_LEVEL_ONE_LINE_THREE_OFF_Y, &img, KB_DIGIT_SEVEN);

        img.bgImg = (char *)ABOUT_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_TWO_OFF_X, MENU_LEVEL_ONE_LINE_THREE_OFF_Y, &img, KB_DIGIT_EIGHT);

        img.bgImg = (char *)MORE_ICON;
        LvUiShowImage(MENU_LEVEL_ONE_COLUMN_THREE_OFF_X, MENU_LEVEL_ONE_LINE_THREE_OFF_Y, &img, KB_DIGIT_NINE);

        key = LvUiWaitKey(iTimeout);

        if (key == KB_CANCEL ||
            key == LVGL_OPER_TIMEOUT)
        {
            break;
        }

        switch (key)
        {
        case KB_DIGIT_ONE:
            UIConsume(iTimeout);
            DisplayDisEnbleIC();
            break;

        case KB_DIGIT_TWO:
            UIUnsupport(iTimeout);
            break;

        case KB_DIGIT_THREE:
#if HW_QR_CAM
            UIQRCodePayMenu(iTimeout);
#else
            UIUnsupport(iTimeout);
#endif
            break;

        case KB_DIGIT_FOUR:
            UIPosManageMenu(iTimeout);
            break;

        case KB_DIGIT_FIVE:
            UISystemManageMenu(iTimeout);
            break;

        case KB_DIGIT_SIX:
            UISystemSettingMenu(iTimeout);
            break;

        case KB_DIGIT_SEVEN:
            UITestMenu(iTimeout);
            break;

        case KB_DIGIT_EIGHT:
            UIAboutMenu(iTimeout);
            break;

        case KB_DIGIT_NINE:
            UIMoreMenu(iTimeout);
            break;

        case KB_DIGIT_ZERO:
            UIDispDemo(iTimeout);
            break;

        default:
            break;
        }
    }

}

//-----------------------------------------------------------------------------
// Name    : UIFuntionSlider
// Func    : Set function by slider
// Paras   : title[in], the funtion title which shows in the top of the screen
//         : ucMax[in], the max num of the slider range
//         : funtionType[in], the funtion type
//         : iTimeout[in], Timeout, in ms
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int UIFuntionSlider(char *title, unsigned char ucMax, E_SliderFuntionType funtionType, int iTimeout)
{
    short sRC;
    unsigned char ucLen;
    unsigned char ucGrade;
    unsigned char ucSetGrade;
    char pDispBuf[30] = {0};
    lv_obj_t *page = NULL;
    lv_obj_t *labelTitle = NULL;
    lv_obj_t *labelRange = NULL;
    lv_obj_t *valueLabel = NULL;
    lv_obj_t *slider = NULL;
    lv_obj_t *labelTip = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;
    int sliderHeight;

    // Clear screen
    LvUilCls();

    if (funtionType == E_PRINTER_GRADE)
    {
        ucGrade = Lib_PrnGetGrade();
        if (ucMax < ucGrade)
        {
            ucGrade = ucMax;
        }
    }
    else if (funtionType == E_BRIGHTNESS)
    {
        ucLen = 1;
        sRC = TerminalParamGet(BRIGHT_LEVEL_BIT, &ucGrade, &ucLen);
        if (RET_OK != sRC || ucMax < ucGrade || 1 != ucLen)
        {
            ucGrade = ucMax;
        }
    }
    ucSetGrade = ucGrade;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    //lvgl display
    page = LvUiGetPageBody();

    parentWidth = lv_obj_get_width(page);
    parentHeight = lv_obj_get_height(page);

    labelTitle = lv_label_create(page);
    labelRange = lv_label_create(page);
    valueLabel = lv_label_create(page);
    slider = lv_slider_create(page);
    labelTip = lv_label_create(page);

    //set labelTitle
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = title;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_x = parentWidth / 8;
    stCtrlObjAttr.m_y = parentHeight / 10;
    stCtrlObjAttr.m_usWidth = parentWidth / 8 * 6;
    stCtrlObjAttr.m_usHeight = parentHeight / 10;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_20;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLACK;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_TITLE);
    stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace = 2;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(labelTitle, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);


    //set labelRange style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    sprintf((char*)pDispBuf ,"%s (1-%d)", TIP_CURRENT, ucMax);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDispBuf;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_y = parentHeight / 8 * 3;
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_SLIDER_RANGE);
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(labelRange, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set valueLabel style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_y = parentHeight / 8 * 4;
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_SLIDER_VALUE);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_RED;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(valueLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set slider style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stSliderAttr.m_sliderMode = LV_SLIDER_MODE_NORMAL;
    stCtrlObjAttr.m_stSliderAttr.m_usMinValue = 1;
    stCtrlObjAttr.m_stSliderAttr.m_usMaxValue = ucMax;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 8 * 5;
    stCtrlObjAttr.m_usWidth = parentWidth / 16 * 11;
    sliderHeight = parentHeight / 12;
    stCtrlObjAttr.m_usHeight = sliderHeight;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_usRadius = sliderHeight / 2;
    LvSetCtrlObj(slider, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_usRadius = sliderHeight / 2;
    LvSetCtrlObj(slider, &stCtrlObjAttr, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stPadAttr.m_usBottom = sliderHeight / 4;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_stPadAttr.m_usRight = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_stPadAttr.m_usTop = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_usRadius = sliderHeight;
    LvSetCtrlObj(slider, &stCtrlObjAttr, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(slider, LvUiSliderEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set labelTip style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_ARR_NUM_TO_SET;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_align = LV_ALIGN_BOTTOM_MID;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TIP);
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(labelTip, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //when event bubble, there can not slider the knob.
//    lv_obj_add_flag(slider, LV_OBJ_FLAG_EVENT_BUBBLE);

    while (1)
    {
        sprintf((char*)pDispBuf, "%d", ucSetGrade);
        lv_label_set_text(valueLabel, pDispBuf);
        lv_slider_set_value(slider, ucSetGrade, LV_ANIM_OFF);

        if (funtionType == E_BRIGHTNESS)
        {
            g_teminal_param.ucBrightLevel = ucSetGrade;
            Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        }

        sRC = LvUiWaitKey(iTimeout);
        switch (sRC)
        {
        case LVGL_OPER_TIMEOUT:
            if (funtionType == E_BRIGHTNESS)
            {
                g_teminal_param.ucBrightLevel = ucGrade;
            }
            LvUiMessageBox(TIP_WAIT_TIMEOUT, MB_CENTER |MB_COLOR_YELLOW|MB_OK, NULL, 2000);
            return RET_OK;

        case KB_CANCEL:
            if (funtionType == E_BRIGHTNESS)
            {
                g_teminal_param.ucBrightLevel = ucGrade;
                Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
            }
            return RET_OK;

        case KB_ENTER:
            if (funtionType == E_PRINTER_GRADE)
            {
                Lib_PrnSetGrade(ucSetGrade);
            }
            else if (funtionType == E_BRIGHTNESS)
            {
                ucGrade = ucSetGrade;
                sRC = TerminalParamSet(BRIGHT_LEVEL_BIT, &ucGrade, BRIGHT_LEVEL_SIZE);
                Lib_LcdSetBrightLevel(ucGrade);
            }
            LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_COLOR_GREEN | MB_OK, NULL, 3000);

            return RET_OK;

        case KB_UP:
            if (ucSetGrade >= ucMax)
            {
                ucSetGrade = ucMax;
            }
            else
            {
                ucSetGrade++;
            }
            break;

        case KB_DOWN:
            if (ucSetGrade <= 1)
            {
                ucSetGrade = 1;
            }
            else
            {
                ucSetGrade--;
            }
            break;

        //case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        //case '9':
            if (sRC -'0' <= ucMax)
            {
                ucSetGrade = sRC -'0';
            }
            break;

        default:
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : UISetTime
// Func    : Set some time by arc
// Paras   : title[in], the funtion title which shows in the top of the screen
//         : minTime[in], the min num of the time range
//         : maxTime[in], the max num of the time range
//         : timeInfo[in], the info type to get the current setting, or to set the target setting
//         : iTimeout[in], Timeout, in ms
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UISetTime(char *title, unsigned int minTime, unsigned int maxTime, E_INFO timeInfo, int iTimeout)
{
    int ret;
    int time = -1;
    int ucDispTime = 0;
    unsigned short w, h, font, forecolor, backcolor;
    unsigned long ucDispTimeLen = 1;
    char timeBuf[10] = {0};
    lv_obj_t *page = NULL;
    lv_obj_t *titleLabel = NULL;
    lv_obj_t *arc = NULL;
    lv_obj_t *rangeStartLabel = NULL;
    lv_obj_t *rangeEndLabel = NULL;
    lv_obj_t *unitLabel = NULL;
    lv_obj_t *textArea = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;
    int arcWidth = 0;

    FUN_IN;

    //Get the current setup time
    ret = Lib_GetInfo(timeInfo, &ucDispTime, &ucDispTimeLen);
    if(ret == RET_OK)
    {
        //The currently set time will be displayed only if the fetch is successful
        time = ucDispTime;
    }
    else
    {
        ucDispTime = -1;
    }

    LvUilCls();

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();

    parentWidth = lv_obj_get_width(page);
    parentHeight = lv_obj_get_height(page);

    titleLabel = lv_label_create(page);
    arc = lv_arc_create(page);
    rangeStartLabel = lv_label_create(arc);
    rangeEndLabel = lv_label_create(arc);
    unitLabel = lv_label_create(arc);
    textArea = lv_textarea_create(arc);

    //set titleLabel
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = title;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_x = parentWidth / 8;
    stCtrlObjAttr.m_y = parentHeight / 10;
    stCtrlObjAttr.m_usWidth = parentWidth / 8 * 6;
    stCtrlObjAttr.m_usHeight = parentHeight / 10;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_20;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLACK;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_TITLE);
    stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace = 2;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set arc
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stArcAttr.m_arcMode = LV_ARC_MODE_NORMAL;
    stCtrlObjAttr.m_stArcAttr.m_usMaxValue = maxTime;
    stCtrlObjAttr.m_stArcAttr.m_usMinValue = minTime;
    stCtrlObjAttr.m_stArcAttr.m_usAngleStartValue = 135;
    stCtrlObjAttr.m_stArcAttr.m_usAngleEndValue = 45;
    if (ucDispTime != -1)
    {
        stCtrlObjAttr.m_stArcAttr.m_us_CurValue = ucDispTime;
    }
    else
    {
        stCtrlObjAttr.m_stArcAttr.m_us_CurValue = minTime;
    }
    stCtrlObjAttr.m_stArcAttr.m_bRoundedMode = true;
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    stCtrlObjAttr.m_y = parentHeight / 12;
    stCtrlObjAttr.m_usWidth = parentHeight / 12 * 7;
    stCtrlObjAttr.m_usHeight = parentHeight / 12 * 7;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLACK;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_usRadius = stCtrlObjAttr.m_usWidth / 10;
    arcWidth = stCtrlObjAttr.m_usWidth / 10;
    stCtrlObjAttr.m_stArcAttr.m_usWidth = arcWidth;
    stCtrlObjAttr.m_stArcAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stArcAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_stPadAttr.m_usBottom = stCtrlObjAttr.m_usHeight / 28;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = stCtrlObjAttr.m_usHeight / 14;
    stCtrlObjAttr.m_stPadAttr.m_usRight = stCtrlObjAttr.m_usHeight / 14;
    stCtrlObjAttr.m_stPadAttr.m_usTop = stCtrlObjAttr.m_usHeight / 14;
    LvSetCtrlObj(arc, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stArcAttr.m_usWidth = arcWidth;
    stCtrlObjAttr.m_stArcAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stArcAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stArcAttr.m_bRoundedMode = true;
    LvSetCtrlObj(arc, &stCtrlObjAttr, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;

    stCtrlObjAttr.m_stPadAttr.m_usBottom = arcWidth / 2;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_stPadAttr.m_usRight = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_stPadAttr.m_usTop = stCtrlObjAttr.m_stPadAttr.m_usBottom;
    stCtrlObjAttr.m_usRadius = arcWidth;
    LvSetCtrlObj(arc, &stCtrlObjAttr, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_add_flag(arc, LV_OBJ_FLAG_EVENT_BUBBLE);

    //set rangeStartLabel
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_align = LV_ALIGN_BOTTOM_LEFT;
    sprintf(timeBuf, "%d", minTime);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = timeBuf;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_RANGE);
    LvSetCtrlObj(rangeStartLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set rangeEndLabel
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_align = LV_ALIGN_BOTTOM_RIGHT;
    sprintf(timeBuf, "%d", maxTime);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = timeBuf;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_RANGE);
    LvSetCtrlObj(rangeEndLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set textArea
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));

    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = 100;
    stCtrlObjAttr.m_usWidth = 40;
    stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_VALUE)) + lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_VALUE)) / 5;
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    stCtrlObjAttr.m_y = -lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_VALUE)) / 2;
    if (ucDispTime != -1)
    {
        sprintf(timeBuf, "%d", ucDispTime);
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = timeBuf;
    }
    else
    {
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = "";
    }
    stCtrlObjAttr.m_stTextAreaAttr.m_bOneLineMode = true;
    stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars = "0123456789";
    stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength = 3;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_VALUE);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_BLACK;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(textArea, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_clear_flag(textArea, LV_OBJ_FLAG_CLICKABLE);

    //set unitLabel
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_pAlignObj = textArea;
    stCtrlObjAttr.m_align = LV_ALIGN_OUT_BOTTOM_MID;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_ARC_VALUE);
    if (timeInfo == E_POWER_OFF_TIME)
    {
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_UNIT_MIN;
    }
    else
    {
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_UNIT_S;
    }
    LvSetCtrlObj(unitLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_add_event_cb(arc, arcEventHandler, LV_EVENT_ALL, textArea);

    while (1)
    {
        ret = LvUiWaitKey(iTimeout);

        memset(timeBuf, 0, sizeof(timeBuf));
        strcpy(timeBuf, lv_textarea_get_text(textArea));
        ucDispTime = atoi(timeBuf);

        if (ret == KB_CANCEL || ret == LVGL_OPER_TIMEOUT)
        {
            return RET_OK;
        }
        else if (ret == KB_ENTER)
        {
            if ((time != -1) || (strlen(timeBuf) != 0))
            {
                if (time != ucDispTime)
                {
                    ret = Lib_SetInfo(timeInfo, &ucDispTime, ucDispTimeLen);
                    if (ret == RET_OK)
                    {
                        if (E_BLACKLIGHT_TIME == timeInfo)
                        {
                            KbLedSetDelayOff(TIME_BACKLIGHT_LCD_DEF);
                        }
                        LvUiMessageBox(TIP_SET_SUCC, MB_CENTER|MB_COLOR_GREEN, NULL, 3000);
                        return RET_OK;
                    }
                    else
                    {
                        LvUiMessageBox(TIP_SET_FAIL, MB_CENTER|MB_COLOR_RED, NULL, 3000);
                        return RET_ERR;
                    }
                }
            }
            else
            {
                LvUiMessageBox(TIP_SET_FAIL, MB_CENTER|MB_COLOR_RED, NULL, 3000);
                return RET_ERR;
            }
        }
        else if (KB_UP == ret)
        {
            if (ucDispTime < maxTime)
            {
                ucDispTime++;
                lv_arc_set_value(arc, ucDispTime);
                sprintf(timeBuf, "%d", ucDispTime);
                lv_textarea_set_text(textArea, timeBuf);
            }

            continue;
        }
        else if (KB_DOWN == ret)
        {
            if (ucDispTime > minTime)
            {
                ucDispTime--;
                lv_arc_set_value(arc, ucDispTime);
                sprintf(timeBuf, "%d", ucDispTime);
                lv_textarea_set_text(textArea, timeBuf);
            }

            continue;
        }
        else if (ret >= KB_DIGIT_ZERO && ret <= KB_DIGIT_NINE)
        {
            if (ucDispTime == 0)
            {
                lv_textarea_del_char(textArea);
            }
            lv_textarea_add_char(textArea, ret);
            memset(timeBuf, 0, sizeof(timeBuf));
            strcpy(timeBuf, lv_textarea_get_text(textArea));
            ucDispTime = atoi(timeBuf);

            if (ucDispTime < minTime)
            {
                ucDispTime = minTime;
                sprintf(timeBuf, "%d", ucDispTime);
                lv_textarea_set_text(textArea, timeBuf);
            }
            else if (ucDispTime > maxTime)
            {
                ucDispTime = maxTime;
                sprintf(timeBuf, "%d", ucDispTime);
                lv_textarea_set_text(textArea, timeBuf);
            }

            lv_arc_set_value(arc, ucDispTime);

            continue;
        }
        else if (ret == KB_CLEAR)
        {
            lv_textarea_del_char(textArea);
            memset(timeBuf, 0, sizeof(timeBuf));
            strcpy(timeBuf, lv_textarea_get_text(textArea));

            ucDispTime = atoi(timeBuf);
            if (ucDispTime <= minTime)
            {
                ucDispTime = minTime;
                sprintf(timeBuf, "%d", ucDispTime);
                lv_textarea_set_text(textArea, timeBuf);
            }

            lv_arc_set_value(arc, ucDispTime);

            continue;
        }
        else
        {
             continue;
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : UISetParaByTextAreaInfoInput
// Func    : Set some para by textArea
// Paras   : title[in], the funtion title which shows in the top of the screen
//         : pInT[in], the secondary title before the ture imformation
//         : ucBitMap[in], medium to get or set the system para
//         : infoMode[in], information mode like text password etc...
//         : infoMax[in], the max length of the information
//         : iTimeout[in], Timeout, in ms
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UISetParaByTextAreaInfoInput(char *title, char *pInT, unsigned char ucBitMap, unsigned char infoMode, unsigned char infoMax, int iTimeout)
{
    int sRC = -1;
    unsigned char hour = 0;
    unsigned char ucParamLen = 0;
    unsigned int temp = 0;
    char acDisplayBuff[100] = {0};
    char acParam[60];
    char tempBuf[50] = {0};
    lv_obj_t *body = NULL;
    lv_obj_t* label = NULL;
    lv_obj_t *textArea = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    while (1)
    {
        if (body == NULL)
        {
            memset(acParam, 0, sizeof(acParam));
            if (ucBitMap == NTP_SYNC_INTER_BIT)
            {
                sRC = TerminalParamGet(ucBitMap, &hour, &ucParamLen);
                if (RET_OK == sRC)
                {
                    sprintf(acParam, "%d", hour);
                }
                else
                {
                    ucParamLen = 0;
                }
            }
            else
            {
                // Server Domain Name
                if (RET_OK != ServerParamGet(ucBitMap, (unsigned char*)acParam, &ucParamLen))
                {
                    ucParamLen = 0;
                }
            }

            if (ucBitMap == NTP_SYNC_INTER_BIT)
            {
                if (!ucParamLen)
                {
                    sprintf(acDisplayBuff, "%s (D):\n%s", pInT, TIP_NONE);
                }
                else
                {
                    sprintf(acDisplayBuff, "%s (D):\n%s", pInT, acParam);
                }
            }
            else if (ucBitMap == POSP_SERVER_MODE_BIT || ucBitMap == TMS_SERVER_MODE_BIT)
            {
                if (!ucParamLen)
                {
                    sprintf(acDisplayBuff, "%s\n[%d]:%s [%d]:%s\n%s: %s", pInT, 1, TIP_YES, 0, TIP_NO, TIP_CURRENT_RESULT, TIP_NO);
                }
                else
                {
                    if (acParam[0] != '0')
                    {
                        sprintf(acDisplayBuff, "%s\n[%d]:%s [%d]:%s\n%s: %s", pInT, 1, TIP_YES, 0, TIP_NO, TIP_CURRENT_RESULT, TIP_YES);
                    }
                    else
                    {
                        sprintf(acDisplayBuff, "%s\n[%d]:%s [%d]:%s\n%s: %s", pInT, 1, TIP_YES, 0, TIP_NO, TIP_CURRENT_RESULT, TIP_NO);
                    }
                }
            }
            else
            {
                if (!ucParamLen)
                {
                    sprintf(acDisplayBuff, "%s\n%s", pInT, TIP_NONE);
                }
                else
                {
                    sprintf(acDisplayBuff, "%s\n%s", pInT, acParam);
                }
            }

            body = LvUiCreateInputTextBody(FONT_SIZE_TEXT_INPUT_TITLE, title, acDisplayBuff);

        }

        if (label == NULL)
        {
            label = lv_label_create(body);
            lv_obj_update_layout(body);
            parentWidth = lv_obj_get_width(body);
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_ucBidiFlag = TRUE;
            stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_ENTER_TO_EDIT;
            stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
            stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        sRC = LvUiWaitKey(iTimeout);
        if (sRC == KB_UP || sRC == KB_DOWN || sRC == KB_CANCEL || sRC == LVGL_OPER_TIMEOUT)
        {
            lv_obj_del(body);
            return sRC;
        }
        else if (sRC == KB_ENTER)
        {
            lv_obj_del(label);
            label = NULL;
            textArea = LvUiCreateInputTextArea(body, acParam, FONT_SIZE_TEXT_INPUT, infoMode, 0, infoMax);

            while (1)
            {
                sRC = LvUiInputText(textArea, infoMode, 0, infoMax, iTimeout);
                if (sRC != LVGL_OPER_SUCC)
                {
                    lv_obj_del(body);
                    return sRC;
                }
                else
                {
                    memset(acParam, 0, sizeof(acParam));
                    strcpy(acParam, lv_textarea_get_text(textArea));
                }

                if (ucBitMap == NTP_SYNC_INTER_BIT)
                {
                    if (strlen(acParam) == 0)
                    {
                        LvUiMessageBox(TIP_PARA_CANNOT_EMPTY, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                        continue;
                    }
                    else
                    {
                        temp = atoi(acParam);
                        if (temp > NTP_SYNC_INTER_HOUR_MAX)
                        {
                            sprintf(tempBuf, "%s %d!", TIP_PARA_CANNOT_MORE, NTP_SYNC_INTER_HOUR_MAX);
                            LvUiMessageBox(tempBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                            continue;
                        }
                        else if (temp < NTP_SYNC_INTER_HOUR_MIN)
                        {
                            sprintf(tempBuf, "%s %d!", TIP_PARA_LEN_LESS, NTP_SYNC_INTER_HOUR_MIN);
                            LvUiMessageBox(tempBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                            continue;
                        }
                        else
                        {
                            hour = (unsigned char)temp;
                            sRC = TerminalParamSet(NTP_SYNC_INTER_BIT, &hour, sizeof(hour));
                            if (sRC < 0)
                            {
                                lv_obj_del(body);
                                return sRC;
                            }
                            LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
                            lv_obj_del(body);
                            body = NULL;
                            break;
                        }
                    }
                }
                else if (ucBitMap == HOST1PORT_BIT || ucBitMap == HOST2PORT_BIT || ucBitMap == NET_NAME_POSP_PORT_BIT || ucBitMap == NET_NAME_POSP_PORT_2_BIT)
                {
                    if (strlen(acParam) == 0)
                    {
                        LvUiMessageBox(TIP_PARA_CANNOT_EMPTY, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                        continue;
                    }
                    else if (strlen(acParam) == 5)
                    {
                        temp = atoi(acParam);
                        if (temp > 65535)
                        {
                            sprintf(tempBuf, "%s %d!", TIP_PARA_CANNOT_MORE, 65535);
                            LvUiMessageBox(tempBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                        }
                    }

                    ucParamLen = strlen(acParam);
                    sRC = ServerParamSet(ucBitMap, (unsigned char*)acParam, ucParamLen);
                    if (sRC < 0)
                    {
                        lv_obj_del(body);
                        return sRC;
                    }
                    LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
                    lv_obj_del(body);
                    body = NULL;
                    break;
                }
                else if (ucBitMap == POSP_SERVER_MODE_BIT || ucBitMap == TMS_SERVER_MODE_BIT)
                {
                    if (strlen(acParam) != 1)
                    {
                        LvUiMessageBox(TIP_PARA_CANNOT_EMPTY, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                        continue;
                    }
                    else
                    {
                        if (acParam[0] > '1')
                        {
                            LvUiMessageBox(TIP_PARA_INPUT_ERR, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                            continue;
                        }

                        sRC = ServerParamSet(ucBitMap, (unsigned char*)acParam, ucParamLen);
                        if (sRC < 0)
                        {
                            lv_obj_del(body);
                            return sRC;
                        }

                        LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
                        lv_obj_del(body);
                        body = NULL;
                        break;
                    }
                }
                else
                {
                    //domain name, the length of the domain name cannot be less than 3
                    if (strlen(acParam) < 3)
                    {
                        LvUiMessageBox(TIP_PARA_LEN_ERR, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
                        continue;
                    }
                    else
                    {
                        ucParamLen = strlen(acParam);
                        sRC = ServerParamSet(ucBitMap, (unsigned char*)acParam, ucParamLen);
                        if (sRC < 0)
                        {
                            lv_obj_del(body);
                            return sRC;
                        }
                        LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
                        lv_obj_del(body);
                        body = NULL;
                        break;
                    }
                }
            }
        }
        else
        {
            continue;
        }
    }
}

