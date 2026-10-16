/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : DevTime.c
 *  Version      :
 *  Author       :
 * Description   : Device time management functiuon
 **********************************************************************************************************/

#include "Common.h"
#include "Power.h"
#include "Input.h"
#include "Ui.h"
#include "Language.h"
#include "DevTime.h"
#include "ParamManage.h"
#include "Key.h"
#include "Menu.h"
#include "MessageBox.h"

#include "api_lvgl.h"
#include "custom_Lvgl.h"

static unsigned long s_lastSyncUtcTime = 0;

// The time zone value is four times of the actual time zone, dividing one hour into four equal parts
static const stWorldTimeZone s_timeZoneTable[] = {
    {"UTC-12:00", -48},
    {"UTC-11:00", -44},
    {"UTC-10:00", -40},
    {"UTC-09:30", -38},
    {"UTC-09:00", -36},
    {"UTC-08:00", -32},
    {"UTC-07:00", -28},
    {"UTC-06:00", -24},
    {"UTC-05:00", -20},
    {"UTC-04:00", -16},
    {"UTC-03:30", -14},
    {"UTC-03:00", -12},
    {"UTC-02:00", -8 },
    {"UTC-01:00", -4 },
    {"UTC+00:00",  0 },
    {"UTC+01:00",  4 },
    {"UTC+02:00",  8 },
    {"UTC+03:00",  12},
    {"UTC+03:30",  14},
    {"UTC+04:00",  16},
    {"UTC+04:30",  18},
    {"UTC+05:00",  20},
    {"UTC+05:30",  22},
    {"UTC+05:45",  23},
    {"UTC+06:00",  24},
    {"UTC+06:30",  26},
    {"UTC+07:00",  28},
    {"UTC+08:00",  32},
    {"UTC+08:45",  35},
    {"UTC+09:00",  36},
    {"UTC+09:30",  38},
    {"UTC+10:00",  40},
    {"UTC+10:30",  42},
    {"UTC+11:00",  44},
    {"UTC+12:00",  48},
};

// Display input time
void ShowInputTime(unsigned char Y, char *pucIn)
{
    unsigned char ucLen = 0, ucPos = 0, i = 0;
    unsigned char aucDisBuf[] = "____-__-__ __:__:__";

    ucLen = strlen(pucIn);
    for (i = 0; i < ucLen; i++)
    {
        if (i < 4)
        {
            ucPos = i;
        }
        else if (i < 6)
        {
            ucPos = i + 1;
        }
        else if (i < 8)
        {
            ucPos = i + 2;
        }
        else if (i < 10)
        {
            ucPos = i + 3;
        }
        else if (i < 12)
        {
            ucPos = i + 4;
        }
        else
        {
            ucPos = i + 5;
        }

        aucDisBuf[ucPos] = pucIn[i];
    }

//  UIClearRect(0, Y, LCD_MAX_SEG, 1);
    //UITextAlign(ALIGN_L, Y, DISP_MODE_NORMAL, (const char*)aucDisBuf);
    UIText(11, Y, DISP_MODE_NORMAL, (const char*)aucDisBuf);
    UIUpdataRect(0, Y, LCD_MAX_SEG, 1);
}

// Display current time
void ShowCurTime(unsigned char Y)
{
    unsigned char aucTimeBcd[6];
    unsigned char aucTimeStr[12];
    unsigned char aucDisBuf[] = "2019-01-01 00:00:00";

    Lib_GetDateTime(aucTimeBcd);
    Lib_Bcd2Asc(aucTimeBcd, (char *)aucTimeStr, 6);

    aucDisBuf[2] = aucTimeStr[0];
    aucDisBuf[3] = aucTimeStr[1];

    aucDisBuf[5] = aucTimeStr[2];
    aucDisBuf[6] = aucTimeStr[3];

    aucDisBuf[8] = aucTimeStr[4];
    aucDisBuf[9] = aucTimeStr[5];

    aucDisBuf[11] = aucTimeStr[6];
    aucDisBuf[12] = aucTimeStr[7];

    aucDisBuf[14] = aucTimeStr[8];
    aucDisBuf[15] = aucTimeStr[9];

    aucDisBuf[17] = aucTimeStr[10];
    aucDisBuf[18] = aucTimeStr[11];

//  UIClearRect(0, Y, LCD_MAX_SEG, 1);
    //UITextAlign(ALIGN_L, Y, DISP_MODE_NORMAL, (const char*)aucDisBuf);
    UIText(11, Y, DISP_MODE_NORMAL, (const char*)aucDisBuf);
    UIUpdataRect(0, Y, LCD_MAX_SEG, 1);
}

static short UISyncTime(int timeout)
{
    unsigned char len = 0;
    int ret = 0;
    int timeZone = 0;
    char server[128] = {0};
    char info[150] = {0};

    // Get current server info
    ServerParamGet(NTP_SERVER_BIT, (unsigned char *)server, &len);
    sprintf(info, "%s\n%s", server, TIP_PLEASE_WAIT);

    LvUiMessageBox(info, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, MENU_NTP_SYNC, 1000);

    // Get current time zone
    if (TerminalParamGet(TIME_ZONE_BIT, &timeZone, &len) != RET_OK)
    {
        return LvUiMessageBox(TIP_WAIT_TIMEOUT, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
    }

    // The characters displayed here happen to be the website address of the NTP server
    ret = Lib_SyncDateTime(server, timeZone, 2000);
    if (ret != RET_OK)
    {
        return LvUiMessageBox(TIP_SET_FAIL, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);
    }

    LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, NULL, 3000);

    return RET_OK;
}


static short UITimeZoneSet(int timeout)
{
    unsigned char len = 0;
    short sRet = 0;
    int i = 0;
    int timeZone = 0;
    unsigned short w, h, font, forecolor, backcolor;
    char dispBuf[128] = { 0 };
    int parentWidth, parentHeight;
    lv_group_t *group = NULL;
    lv_obj_t *page = NULL;
    lv_obj_t *titleLabel = NULL;
    lv_obj_t *resultLabel = NULL;
    lv_obj_t *list = NULL;
    lv_obj_t *button = NULL;
    lv_obj_t *buttonLabel = NULL;
    lv_obj_t *curButton = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    FUN_IN;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    LvUilCls();

    sRet = TerminalParamGet(TIME_ZONE_BIT, &timeZone, &len);
    if (sRet != RET_OK)
    {
        snprintf(dispBuf, sizeof(dispBuf), "%s %s", MENU_TIME_ZONE, TIP_ERROR);
        LvUiMessageBox(dispBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
        return sRet;
    }

    group = lv_group_create();

    page = LvUiGetPageBody();
    parentWidth = lv_obj_get_width(page);
    parentHeight = lv_obj_get_height(page);
    //set page
    lv_obj_set_flex_flow(page, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(page, LCD_H / 20, LV_PART_MAIN|LV_STATE_DEFAULT);

    titleLabel = lv_label_create(page);
    //set title Label
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    stCtrlObjAttr.m_usHeight = LINE_HEIGTH;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_20;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLUE;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = MENU_TIME_ZONE;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_CONTROLER_TITLE);
    stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace = 2;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    resultLabel = lv_label_create(page);
    //set result label
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TIMEZONE);
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_GRAY;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    LvSetCtrlObj(resultLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    list = lv_list_create(page);
    //set list
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_usWidth = parentWidth / 2;
    stCtrlObjAttr.m_usHeight = parentHeight - LINE_HEIGTH * 2;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_GRAY;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    LvSetCtrlObj(list , &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(list, LV_OBJ_FLAG_EVENT_BUBBLE);

     // Check if current time zone is in the table
    len = sizeof(s_timeZoneTable) / sizeof(stWorldTimeZone);
    for (int i = 0; i < len; i++)
    {
        button = lv_list_add_btn(list, NULL, s_timeZoneTable[i].name);
        //set button
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
        stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
        LvSetCtrlObj(button , &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_FOCUSED);

        lv_obj_add_flag(button, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_event_cb(button, LvUilistButEventhandler, LV_EVENT_CLICKED, NULL);

        buttonLabel = lv_obj_get_child(button, 0);
        //set buttonLabel
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TIMEZONE);
        LvSetCtrlObj(buttonLabel , &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_group_add_obj(group, button);

        //record the current timeZone
        if (s_timeZoneTable[i].timeZone == timeZone)
        {
            curButton = button;
            snprintf(dispBuf, sizeof(dispBuf), "%s:\n%s", TIP_CURRENT_RESULT, s_timeZoneTable[i].name);
        }
    }

    // Time zone not in supported table
    if (i == len)
    {
        snprintf(dispBuf, sizeof(dispBuf), "%s %s", MENU_TIME_ZONE, TIP_ERROR);
        LvUiMessageBox(dispBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 3000);
        return RET_ERR;
    }

    //make the object center
    lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_CENTER);
    lv_obj_scroll_to_view(curButton, LV_ANIM_OFF);
    lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_NONE);
    lv_obj_scroll_by_bounded(list, 0, 0, LV_ANIM_OFF);
    lv_group_focus_obj(curButton);

    while (1)
    {
        lv_label_set_text(resultLabel, dispBuf);

        sRet = LvUiWaitKey(timeout);
        if (sRet == LVGL_OPER_TIMEOUT || sRet == KB_CANCEL)
        {
            return sRet;
        }
        else if (sRet == KB_UP)
        {
            lv_group_focus_prev(group);
            continue;
        }
        else if (sRet == KB_DOWN)
        {
            lv_group_focus_next(group);
            continue;
        }
        else if (sRet == KB_ENTER)
        {
            curButton = lv_group_get_focused(group);
            i = lv_obj_get_index(curButton);
            if (TerminalParamSet(TIME_ZONE_BIT, (void*)&s_timeZoneTable[i].timeZone, sizeof(s_timeZoneTable[i].timeZone)) == RET_OK)
            {
                LvUiMessageBox(TIP_SET_SUCC, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, NULL, 1000);
                snprintf(dispBuf, sizeof(dispBuf), "%s:\n%s", TIP_CURRENT_RESULT, s_timeZoneTable[i].name);
                sRet = UISyncTime(timeout);
                if (sRet != RET_OK)
                {
                    return sRet;
                }
            }
            else
            {
                LvUiMessageBox(TIP_SET_FAIL, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, NULL, 1000);
            }
        }
        else
        {
            continue;
        }
    }

}

static short UIDateTimeSet(int iTimeout)
{
    unsigned char aucTimeBcd[8] = { 0 };
    char acTimeStr[16] = { 0 };

    Lib_GetDateTime(aucTimeBcd);
    acTimeStr[0] = '2', acTimeStr[1] = '0';
    Lib_Bcd2Asc(aucTimeBcd, (char *)(acTimeStr + 2), 6);

    LvUilCls();

    if (RET_OK == LvUiDate(acTimeStr, "Date & Time", iTimeout))
    {
        Lib_Asc2Bcd(acTimeStr, aucTimeBcd, 14);
        Lib_SetDateTime(aucTimeBcd + 1);
    }

    return RET_OK;
}

static short UISetNtpServerName(int iTimeout)
{
    return UISetParaByTextAreaInfoInput(MENU_NTP_SERVER, TIP_DOMAIN_NAME_MAIN, NTP_SERVER_BIT, LVGL_INPUT_MODE_TEXT, NETWORK_ADDR_SIZE, iTimeout);
}

static short UISyncSwitchSet(int timeout)
{
    int ret = -1;

    ret = LvUiSwitch(MENU_NTP_SYNC_SWITCH, timeout, NTP_SYNC_SWITCH_BIT, SYNCON_ICON, SYNCOFF_ICON);

    if (ret != LVGL_OPER_SUCC)
    {
        return RET_ERR;
    }

    return RET_OK;
}

short UISyncInterHour(int timeout)
{
    return UISetParaByTextAreaInfoInput(MENU_NTP_SYNC_SETTING, TIP_SYNC_PERIOD, NTP_SYNC_INTER_BIT, LVGL_INPUT_MODE_NUM, 3, timeout);
}

static short UIAutoSync(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_NTP_SYNC_SWITCH,  UISyncSwitchSet,    timeout, NULL},
        {MENU_NTP_SYNC_SETTING, UISyncInterHour,    timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_NTP_AUTO_SYNC;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

short UINtpSet(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_NTP_SERVER,       UISetNtpServerName, timeout, NULL},
        {MENU_NTP_SYNC,         UISyncTime,         timeout, NULL},
        {MENU_NTP_AUTO_SYNC,    UIAutoSync,         timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_NTP;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

short UISysTimeSet(int timeout)
{
    STMenu menuList[] =
    {
        {MENU_NTP,              UINtpSet,           timeout, NULL},
        {MENU_TIME_ZONE,        UITimeZoneSet,      timeout, NULL},
        {MENU_DATE_AND_TIME,    UIDateTimeSet,      timeout, NULL},
    };

    LVGL_MENU menu = { 0 };

    menu.title = (const char *)MENU_SYSTIME_SETTING;
    menu.titltBgImg = TITLE_BG_IMG;

    menu.menuList = (STMenu *)&menuList;
    menu.menuSize = sizeof(menuList) / sizeof(STMenu);

    menu.style = LVGL_MENU_STYLE_2;

    return UICreatLvglMenu(timeout, &menu);
}

void AutoSyncDateTimeFun(void)
{
    static unsigned char isForce = 1;
    unsigned char paraSize = 0;
    unsigned char syncEn = 0;
    unsigned char syncInter = 0;
    unsigned char curBcdTime[6] = {0};
    unsigned long utc = 0;
    int timeZone = 0;
    short ret = 0;
    char server[64] = {0};

    // First, check if the automatic synchronization function is enable
    ret = TerminalParamGet(NTP_SYNC_SWITCH_BIT, &syncEn, &paraSize);
    if (ret != RET_OK)
    {
        return;
    }

    // The automatic synchronization function not enable, the function of synchronizing at startup cancel
    if (syncEn == 0)
    {
        isForce = 0;
        return;
    }

    // Check network status
    if ((Lib_WlsGetInitStatus() < 0 || Lib_WlsCheckSim()) && STA_AP_CONNECTED != Lib_WifiGetApStatus())
    {
        return;
    }

    // Non mandatory synchronization
    if (isForce == 0)
    {
        // get parameters of automatic timing interval
        ret = TerminalParamGet(NTP_SYNC_INTER_BIT, &syncInter, &paraSize);
        if (ret != RET_OK)
        {
            return;
        }

        // Get current time
        if (RET_OK != Lib_GetDateTime(curBcdTime))
        {
            return;
        }

        // Convert local current time to UTC timestamp
        if (RET_OK != Lib_RtcToUtcSec(curBcdTime, sizeof(curBcdTime), &utc))
        {
            return;
        }

        // The absolute difference between the local current time and the UTC timestamp at the last calibration
        utc = (utc > s_lastSyncUtcTime) ? (utc - s_lastSyncUtcTime) : (s_lastSyncUtcTime - utc);

        // Check if the interval of automatic synchronization has been reached
        if (utc <= ((unsigned long)syncInter * 24 * 60 * 60))
        {
            return;
        }
    }
    else
    {
        isForce = 0;
    }

    // Get server parameters
    ret = TerminalParamGet(NTP_SERVER_BIT, server, &paraSize);
    if (ret != RET_OK)
    {
        return;
    }

    // Get Time Zone
    ret = TerminalParamGet(TIME_ZONE_BIT, &timeZone, &paraSize);
    if (ret != RET_OK)
    {
        return;
    }

    // UITextAlign(ALIGN_M, MAX_COM_LINES - 1, DISP_MODE_NORMAL, MENU_NTP_AUTO_SYNC);
    // The synchronization time failed, and the current time record needs to be used as the new starting point for timing
    Lib_SyncDateTime(server, timeZone, 2000);
    // Lib_LcdDrawArea(RGB565_WHITE, 0, 210, 320, 30);
    // Get current time
    if (RET_OK != Lib_GetDateTime(curBcdTime))
    {
        return;
    }

    // Use the latest time as the starting point of the new timing
    Lib_RtcToUtcSec(curBcdTime, sizeof(curBcdTime), &s_lastSyncUtcTime);
    Lib_UtcSecToRtc(s_lastSyncUtcTime, curBcdTime, sizeof(curBcdTime));
    memset(server, 0, sizeof(server));
    Lib_Bcd2Asc(curBcdTime, server, sizeof(curBcdTime));
    TRACE_LOG("s_lastSyncUtcTime = %d,%s", s_lastSyncUtcTime, server);
}

