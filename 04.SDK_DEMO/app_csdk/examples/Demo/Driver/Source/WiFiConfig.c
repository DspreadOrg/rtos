/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : WifiConfig.c
 *  Version      :
 *  Author       :
 * Description   : Implement WiFi management related functions
 **********************************************************************************************************/

#include "WifiConfig.h"
#include "LibApi.h"
#include "Common.h"
#include "MessageBox.h"
#include "Drv.h"
#include "Ui.h"
#include "ParamManage.h"
#include "Input.h"
#include "Menu.h"
#include "Key.h"
#include "Language.h"
#include "M_NetInterface.h"
#include "Communication.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"

#if HW_WIFI

#define DECLARATION 1

#if DECLARATION
static short UINetModeCfg(int iTimeout, uint8_t flag);
short UIWifiNetList(int iTimeout);
static short UIWifiSelectNet(int iTimeout, WIFI_INF wifiInfo);
static short UIWifiLinkAct(unsigned int iTimeout, WIFI_INF wifiInfo);
static short UIWifiLinkActNoCofig(unsigned int iTimeout, unsigned char *pucSsid, unsigned char ucSsidLen, unsigned char *pucPwd, unsigned char ucPwdLen, unsigned char ucEncryptType);
#endif

// Network mode configuration maximum item
#define NET_MODE_MAX_ITEM               2

#define MAX_WIFI_LIST_NUM               40

#define WIFI_GET_ICON(rssi) \
    (((rssi) == WIFI_RSSI_MIN_VAL || (rssi) < -100) ? WIFI0_ICON : \
     ((rssi) < -85) ? WIFI1_ICON : \
     ((rssi) < -70) ? WIFI2_ICON : WIFI3_ICON)

//-----------------------------------------------------------------------------
// Name    : ShowUIWifiCfgMenu
// Func    : Menu display interface
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static void ShowUIWifiCfgMenu(char *pText, char **ppcMenuInfo, unsigned char ucTotalLines)
{
    unsigned char i = 0;
    unsigned char curPage = 1;
    unsigned char maxPage = 0;
    unsigned char curShowSize = 0;
    unsigned char curMenuIndex = 0;
    unsigned char positionX = 0;
    unsigned char positionY = 0;
    char dispBuff[48] = {0};
    t_lvgl_image img = { 0 };

    //Calculate the maximum number of pages
    maxPage = (ucTotalLines - 1) / PAGE_MAX_SHOW_MENU_COUNT_LVGL + 1;

    //clear the screen
    LvUilCls();

    BUFCLR ( dispBuff );
    sprintf(dispBuff, "%s[%d/%d]", pText, curPage, maxPage);

    img.bgImg = TITLE_BG_IMG;
    LvUiShowImage(0, 0, &img, -1);
    LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char*)dispBuff, LV_COLOR_WHITE);


    //Counts the number of icons on the currently displayed page
    curShowSize = PAGE_MAX_SHOW_MENU_COUNT_LVGL;
    if (curPage >= maxPage && (ucTotalLines % PAGE_MAX_SHOW_MENU_COUNT_LVGL))
    {
        curShowSize = ucTotalLines % PAGE_MAX_SHOW_MENU_COUNT_LVGL;
    }

    for (i = 0; i < curShowSize; i++)
    {
        //Subscript for the current menu
        curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_COUNT_LVGL + i;

        //Calculate the position of x and y
        positionX = MENU_LEVEL_STYLE2_COLUMN_ONE_OFF_X;
        positionY = MENU_LEVEL_STYLE2_LINE_ONE_OFF_Y + MENU_LEVEL_STYLE2_LINE_WIDTH * i;

        img.bgImg = (char *)MENU_STYLE2_BG0_ICON;
        img.noImg = (char *)MENU_STYLE2_ICON;
        img.text = ppcMenuInfo[curMenuIndex];
        LvUiShowImage(positionX, positionY, &img, i + '1');
    }
}


//-----------------------------------------------------------------------------
// Name    : UINetCfgMenu
// Func    : WIFI configuration main menu
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UINetCfgMenu(int iTimeout)
{
    short sRC=0;
    unsigned char ucLine;
    unsigned char ucLen, ucParam = NET_MODE_GPRS;
    unsigned char menu_flag;
    unsigned char save_flag;
    unsigned char ucMode;
    char **apcMenu = NULL;
    char *apcMenuWifi[] =
    {
        MENU_SELECT_MODE,           // Communication mode selection
        MENU_CONNECT_WIFI,          // Connect WIFI AP
    };
    char *apcMenuGprs[] =
    {
        MENU_SELECT_MODE,           // Communication mode selection
        MENU_APN_DNS_SETTING,       // APN DNS setup
        MENU_NET_MODE_SETTING,      // Net mode

    };

    bool statusBar = LvUiStatusBarIsExist();
    if (statusBar)
    {
        LvUiCtrlStatusBar(0);
    }

    sRC = sRC;

NEW_UPDATE:
    ucLen = NET_WORK_MODE_SIZE;
    sRC = TerminalParamGet(NET_WORK_MODE_BIT, &ucParam, &ucLen);

    if (NET_MODE_GPRS == ucParam)
    {
        menu_flag = 0;
        apcMenu = apcMenuGprs;
        ucLine = sizeof(apcMenuGprs)/sizeof(char *);
    }
    else
    {
        // When entering WIFI settings, set the current working module to WIFI module
        //NETSetInfo(NET_SETTING_CURRENT_MODULE, &ucMode, 1);
        Lib_WifiSetAutoConnectFlag(TRUE);
        menu_flag = 1;
        apcMenu = apcMenuWifi;
        ucLine = sizeof(apcMenuWifi)/sizeof(char *);
    }

    while (1)
    {
        ShowUIWifiCfgMenu(MENU_NETWORK_SETTING, apcMenu, ucLine);
        sRC = LvUiWaitKey(iTimeout);
        switch (sRC)
        {
            case KB_CANCEL:
            case LVGL_OPER_TIMEOUT:

                if (statusBar)
                {
                    LvUiCtrlStatusBar(statusBar);
                }

                return RET_ERR;

            case KB_DIGIT_ONE:   // Communication mode selection
                ucLen = NET_WORK_MODE_SIZE;
                TerminalParamGet(NET_WORK_MODE_BIT, &save_flag, &ucLen);
                sRC = UINetModeCfg(iTimeout, save_flag);
                goto NEW_UPDATE;
            case KB_DIGIT_TWO:
                if (menu_flag == 1)
                    sRC = UIWifiNetList(iTimeout);
                else if(menu_flag == 0)
                    sRC = UICommParamSet(iTimeout);
                break;
            case KB_DIGIT_THREE:
                if(menu_flag == 0)
                {
                    sRC = UIEnterGprsModeCfg(iTimeout);
                }
                break;

            default:
                break;
        }
    }

    if (statusBar)
    {
        LvUiCtrlStatusBar(statusBar);
    }

    ucMode = GetNetWorkMode();
    // If connected to AP in WiFi mode
    if (((ucMode == NET_MODE_WIFI) || (ucMode == NET_MODE_WIFI_PRIOR)) && (STA_AP_CONNECTED == Lib_WifiGetApStatus()))
    {
        return RET_OK;
    }

    // Or select GPRS mode; Return OK
    if (ucMode == NET_MODE_GPRS)
    {
        return RET_OK;
    }
    return RET_ERR;
}


//-----------------------------------------------------------------------------
// Name    : UINetModeCfg
// Func    : Network mode settings
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UINetModeCfg(int iTimeout, uint8_t flag)
{
    char buff[128];
    short sRC = 0;
    char *menuName[] =
    {
        MENU_GPRS,
        MENU_WIFI,
    };

    FUN_IN;

    while (1)
    {
        if (sRC != LVGL_BUTTON_SELECTED_FOR_REFRESH)
        {
            ShowUiMenuPackY(MENU_SELECT_MODE, menuName, ITEM_OF(menuName), flag);
        }

        sRC = LvUiWaitKey(iTimeout);
        switch (sRC)
        {
            case LVGL_OPER_TIMEOUT:
            case KB_CANCEL:
                return sRC;

            case KB_DOWN:
                flag ++;
                flag = flag % NET_MODE_MAX_ITEM;
                break;

            case KB_UP:
                if (flag == 0)
                {
                    flag = NET_MODE_MAX_ITEM - 1;
                }
                else
                {
                    flag--;
                }
                break;

            case KB_DIGIT_ONE:
            case KB_DIGIT_TWO:
                flag = sRC - '1';
                break;

            case 1:
            case 2:
                flag = sRC - 1;
                break;

            case KB_ENTER:

                memset(buff, 0, sizeof(buff));
                strcpy(buff, "\n");
                strcat(buff, menuName[flag]);
                if (KB_ENTER == LvUiMessageBox(buff, MB_COLOR_YELLOW |MB_CENTER | MB_ICONQUESTION | MB_YESNO, "TIP", 10000))
                {
                    TRACE("WIFI MENU: FLAG = %d", flag);

                    if (RET_OK != SetNetWorkMode(flag))
                    {
                        FUN_IN;
                        return RET_ERR;
                    }
#if 1
                    if (flag == NET_MODE_GPRS)
                    {
                        Lib_WifiNetworkDown();
                    }
                    else if (flag == NET_MODE_WIFI)
                    {
                        Lib_WifiNetworkUp();
                        Lib_WifiSetAutoConnectFlag(TRUE);
                    }
#endif
                    memset(buff, 0, sizeof(buff));
                    sprintf(buff, "\n%s", TIP_OK);
                    LvUiMessageBox(buff, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, "TIP", 3000);
                    return RET_OK;
                }

            default:
                break;
        }

    }

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : CheckWiFiFree
// Func    : Check if the network module is idle
// Paras   : pusS, Pointer to waiting time, returns the existing result after processing
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CheckWiFiFree(unsigned short *pusS)
{
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : GetLinkWifi
// Func    : Get connected WiFi AP infos
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short GetLinkWifi(WIFI_INF* wifiInfo)
{
    if (NULL == wifiInfo)
        return RET_ERR;

    TRACE("GetLinkWifi");

    unsigned short paraLen = 0;
    unsigned char status = Lib_WifiGetApStatus();
    TRACE("GetLinkWifi status : %d ", status);
    if (STA_AP_CONNECTED == status)
    {
        Lib_WifiGetLinkAp(wifiInfo->m_ausSSID, &paraLen, WIFI_PARA_SSID);
        Lib_WifiGetLinkAp(wifiInfo->m_aucPass, &paraLen, WIFI_PARA_PWD);
        TRACE("GetLinkWifi OK");
        return RET_OK;
    }
    else if (STA_AP_DISCONNECTED == status)
    {
        TRACE("GetLinkWifi Fail");
        return RET_ERR_TIMEOUT;
    }
    return RET_ERR;
}


//-----------------------------------------------------------------------------
// Name    : GetLinkWifiConnectInfo
// Func    :
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short GetLinkWifiConnectInfo(WIFI_NET_INFO* wifiNetInfo)
{
    if (NULL == wifiNetInfo)
        return RET_ERR;

    FUN_IN;

    unsigned short paraLen = 0;
    unsigned char status = Lib_WifiGetApStatus();
    if (STA_AP_CONNECTED == status)
    {
        Lib_WifiGetLinkAp(wifiNetInfo->SsidName, &paraLen, WIFI_PARA_SSID);
        wifiNetInfo->SsidName[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->PassWordBuf, &paraLen, WIFI_PARA_PWD);
        wifiNetInfo->PassWordBuf[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->LocalIpAdd, &paraLen, WIFI_PARA_LOCAL_IP);
        wifiNetInfo->LocalIpAdd[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->LocalGateway, &paraLen, WIFI_PARA_NET_GATE);
        wifiNetInfo->LocalGateway[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->MacAdd, &paraLen, WIFI_PARA_LOCAL_MAC);
        wifiNetInfo->MacAdd[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->SsidMacAdd, &paraLen, WIFI_PARA_SSID_MAC);
        wifiNetInfo->SsidMacAdd[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->LocalSubnetMask, &paraLen, WIFI_PARA_NET_MASK);
        wifiNetInfo->LocalSubnetMask[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->DNS1, &paraLen, WIFI_PARA_NET_DNS1);
        wifiNetInfo->DNS1[paraLen] = 0;

        Lib_WifiGetLinkAp(wifiNetInfo->DNS2, &paraLen, WIFI_PARA_NET_DNS2);
        wifiNetInfo->DNS2[paraLen] = 0;
        return RET_OK;
    }
    return RET_ERR;
}


//-----------------------------------------------------------------------------
// Name    : WifiConfigInfo
// Func    : Display detailed configuration information of connected WiFi
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short WifiConfigInfo(int iTimeout)
{
    short sRC;
    unsigned short w, h, font, forecolor, backcolor;
    WIFI_NET_INFO st_WifiNetInfo;
    char pDispBuf[1024] = {0};
    lv_obj_t *page = NULL;
    lv_obj_t *container = NULL;
    lv_obj_t *titleLable = NULL;
    lv_obj_t *contentLable = NULL;
    char *ipType = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;

    FUN_IN;

    // Get WiFi configuration information
    // Check WiFi connection status
    WIFI_INF wifiInfo;
    sRC = GetLinkWifi(&wifiInfo);
    if (RET_OK != sRC)
    {
        FUN_IN;
        return RET_ERR;
    }

    sRC = GetLinkWifiConnectInfo(&st_WifiNetInfo);
    if (RET_OK != sRC)
    {
        FUN_IN;
        BUFCLR(pDispBuf);
        sprintf(pDispBuf, "\n%s", TIP_WIFI_INFO_GET_FAIL);
        UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES - 1);
        LvUiMessageBox(pDispBuf, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, "TIP", 3000);
        return RET_ERR;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();

    parentWidth = lv_obj_get_width(page);
    parentHeight = lv_obj_get_height(page);

    container = lv_obj_create(page);
    titleLable = lv_label_create(container);
    contentLable = lv_label_create(container);

    //set container style
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    LvSetCtrlObj(container, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(container, LV_OBJ_FLAG_EVENT_BUBBLE);

    //set titleLable style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_WIFI_INFO;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_y = parentHeight / 64;
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_WIFI_INFO_TITLE);
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    LvSetCtrlObj(titleLable, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set contentLable style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_ucBidiFlag = TRUE;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_x = parentWidth / 32;
    lv_obj_update_layout(titleLable);
    stCtrlObjAttr.m_y = parentHeight / 64 + lv_obj_get_height(titleLable) + parentHeight / 64;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 32;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_WIFI_INFO);
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;

    //get wifi info
    ipType = st_WifiNetInfo.StaticIpFlag?TIP_WIFI_STATIC_IP:TIP_WIFI_DYNAMIC_IP;
    sprintf(pDispBuf, "%s\n%s:%s\n%s:%s\n%s:%s\n%s:%s\n%s:%s\n%s:%s\n%s:%s", ipType,
            TIP_WIFI_GATEWAY, st_WifiNetInfo.LocalGateway,
            TIP_WIFI_DNS1, st_WifiNetInfo.DNS1,
            TIP_WIFI_IP, st_WifiNetInfo.LocalIpAdd,
            TIP_WIFI_SUBWAY, st_WifiNetInfo.LocalSubnetMask,
            TIP_WIFI_DNS2, st_WifiNetInfo.DNS2,
            TIP_WIFI_MAC, st_WifiNetInfo.MacAdd,
            TIP_WIFI_BID, st_WifiNetInfo.SsidMacAdd);
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDispBuf;
    LvSetCtrlObj(contentLable, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    sRC = LvUiWaitKey(iTimeout);

    TRACE("WifiConfigInfo UIWaitKey:%d", sRC);

    lv_obj_del(container);

    if (UI_INPUT_ENTER == sRC)
    {
        return RET_OK;
    }
    else
    {
        return sRC;
    }

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UIWifiNetList
// Func    : Query AP list in WIFI mode
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWifiNetList(int iTimeout)
{
    short sRC;
    unsigned int ulStartTime, uiRefreshTime;
    unsigned short usS;
    unsigned char ucConnectF = FALSE;//ucMode = 1,
    unsigned char aucCurSSID[WIFI_SSID_MAX_LEN + 1];
    unsigned short usCurSSIDLen = 0;
    WIFI_INF wifiInfo[MAX_WIFI_LIST_NUM];
    unsigned short apCount = 0;
    unsigned char pDispBuf[100];

    memset(wifiInfo, 0, sizeof(wifiInfo));

    FUN_IN;
    memset(pDispBuf, 0, sizeof(pDispBuf));
    sprintf((char *)pDispBuf, "%s", TIP_WIFI_SEARCH);
    LvUiMessageBox((char *)pDispBuf, MB_CENTER | MB_ICONINFORMATION|MB_COLOR_YELLOW | MB_NOKEY, "TIP", 0);
    // WIFI starts searching for AP
    Lib_WifiNetworkScan();

    usS = 60;
    sRC = CheckWiFiFree(&usS);
    if (sRC != RET_OK)
    {
        return sRC;
    }

    ulStartTime = Lib_SysGetTimer();
    while (1)
    {
        Lib_WifiGetAP(wifiInfo, &apCount, MAX_WIFI_LIST_NUM);
        TRACE("AP List Len = %d\n", apCount);
        if (apCount > 0)
            break;

        if (KB_CANCEL == LvUiWaitKey(50))
        {
            FUN_IN;
            return RET_ERR;
        }

        if (usS == 0)
        {
            return UI_INPUT_TIMEOUT;
        }
        else
        {
            // show countdown time
            if (IS_TIME_OUT(ulStartTime, 1000))
            {
                usS--;
                ulStartTime = Lib_SysGetTimer();
                memset(pDispBuf, 0, 100);
                sprintf((char*)pDispBuf, "\n%s\n%s(%03d)...", TIP_WIFI_GET_LIST, TIP_WAITTING_CHA, usS);
                LvUiMessageBox((char*)pDispBuf, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_NOKEY, "TIP", 0);
            }
        }
    }

    if (apCount == 0)
    {
        memset(pDispBuf, 0, sizeof(pDispBuf));
        sprintf((char*)pDispBuf, "\n%s", TIP_WIFI_NO_AVAILABLE);
        LvUiMessageBox((char*)pDispBuf, MB_CENTER | MB_ICONINFORMATION | MB_COLOR_YELLOW | MB_ANYKEY, "TIP", 3000);
        FUN_IN;
        return RET_ERR;
    }

    // If there is a AP connection, migrate the AP to the top of the list
    ulStartTime = Lib_SysGetTimer();

    while (1)
    {
        unsigned char ucFlag = 0;

        // View the currently connected WiFi
        WIFI_INF curWifi;
        memset(&curWifi, 0, sizeof(WIFI_INF));
        sRC = GetLinkWifi(&curWifi);
        if (RET_OK == sRC)
        {
            FUN_IN;
            usCurSSIDLen = strlen((const char *)curWifi.m_ausSSID);

            // If there is a AP connection, migrate the AP to the top of the list
            for (int i=0; i<apCount; i++)
            {
                if ((0 == memcmp(curWifi.m_ausSSID, wifiInfo[i].m_ausSSID, usCurSSIDLen))
                    && (strlen((const char *)wifiInfo[i].m_ausSSID) == usCurSSIDLen))
                {
                    FUN_IN;
                    ucFlag = 1;
                    if (0 != i)
                    {
                        WIFI_INF tempWifi ;
                        memcpy(&tempWifi, &wifiInfo[0], sizeof(WIFI_INF));
                        memcpy(&wifiInfo[0], &wifiInfo[i], sizeof(WIFI_INF));
                        memcpy(&wifiInfo[i], &tempWifi, sizeof(WIFI_INF));
                    }
                    break;
                }
            }

            if (!ucFlag)
            {
                // If there is no connected WiFi in the list, insert it directly
                for (int i=apCount-1; i>=0; i--)
                {
                    memcpy(&wifiInfo[i+1], &wifiInfo[i], sizeof(WIFI_INF));
                }
                memcpy(&wifiInfo[0], &curWifi, sizeof(WIFI_INF));
                apCount++;
                ucFlag = 1;
                break;
            }

            // Connected WiFi is set to the top, exit directly
            if (ucFlag)
            {
                FUN_IN;
                break;
            }
        }
        else
        {
            //Exit directly without non-connection
            break;
        }
    }

    TRACE("=usCnt: %d==\r\n", apCount);

    //lvgl code block
    {
        int ret;
        int focusedIndex;
        int cRssi;
        int connectIndex = -1;
        char *imgSrc;
        unsigned short w, h, font, forecolor, backcolor;
        lv_group_t *g = NULL;
        lv_obj_t *label = NULL;
        lv_obj_t *list = NULL;
        lv_obj_t *btn[MAX_WIFI_LIST_NUM];
        lv_obj_t *btnLabel = NULL;
        lv_obj_t *image = NULL;
        lv_obj_t *focusedObj = NULL;
        lv_obj_t *page;
        CTRL_OBJ_ATTR stCtrlObjAttr = {0};
        int parentWidth, parentHeight;

        //clear the screen
        LvUilCls();

        Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

        page = LvUiGetPageBody();

        parentWidth = lv_obj_get_width(page);
        parentHeight = lv_obj_get_height(page);

        g = lv_group_create();

        label = lv_label_create(page);
        list = lv_list_create(page);

        //label style
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = MENU_CONNECT_WIFI;
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

        for (int i = 0; i < apCount; i++)
        {
            btn[i] = lv_list_add_btn(list, NULL, (const char *)wifiInfo[i].m_ausSSID);
            btnLabel = lv_obj_get_child(btn[i], 0);
            image = lv_img_create(btn[i]);

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
            LvSetCtrlObj(btn[i], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

            stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
            stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
            LvSetCtrlObj(btn[i], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_FOCUSED);

            lv_obj_set_flex_align(btn[i], LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

            lv_obj_add_flag(btn[i], LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_obj_add_event_cb(btn[i], LvUilistButEventhandler, LV_EVENT_CLICKED, (void *)KB_ENTER);

            lv_group_add_obj(g, btn[i]);

            //set btnLabel
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_ucBidiFlag = TRUE;
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_SMALL);
            LvSetCtrlObj(btnLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

            //set image src
            cRssi = wifiInfo[i].m_aucSignalLevel;
            imgSrc = WIFI_GET_ICON(cRssi);

            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = imgSrc;
            LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        }

        lv_obj_update_layout(page);

        lv_group_focus_obj(btn[0]);

        ulStartTime = Lib_SysGetTimer();
        uiRefreshTime = Lib_SysGetTimer() - 1000;
        while (1)
        {
            // Refresh every 1000ms
            if (IS_TIME_OUT(uiRefreshTime, 1000))
            {
                uiRefreshTime = Lib_SysGetTimer();

                // Check current WiFi connection status
                WIFI_INF curWifi = { 0 };
                sRC = GetLinkWifi(&curWifi);

                if (RET_OK == sRC)
                {
                    ucConnectF = TRUE;
                }
                else
                {
                    ucConnectF = FALSE;
                }

                for (int i = 0; i < apCount; i++)
                {
                    btnLabel = lv_obj_get_child(btn[i], 0);

                    if (strcmp((char *)curWifi.m_ausSSID, lv_label_get_text(btnLabel)) == 0)
                    {
                        connectIndex = i;
                    }
                }
            }

            if (connectIndex != -1)
            {
                memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
                if (ucConnectF)
                {
                    lv_obj_t *image = lv_obj_get_child(btn[connectIndex], 1);
                    stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = WIFI_CHECK_ICON;
                    LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                else
                {
                    lv_obj_t *image = lv_obj_get_child(btn[connectIndex], 1);

                    cRssi = wifiInfo[connectIndex].m_aucSignalLevel;
                    imgSrc = WIFI_GET_ICON(cRssi);;

                    stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = imgSrc;
                    LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

                    connectIndex = -1;
                }
            }

            //Get the key
            ret = LvUiWaitKey(40);

            if (ret != LVGL_OPER_TIMEOUT)
            {
                ulStartTime = Lib_SysGetTimer();
            }

            if (IS_TIME_OUT(ulStartTime, 60 * 1000))
            {
                return UI_INPUT_TIMEOUT;
            }

            if (ret == KB_CANCEL)
            {
                return 0;
            }
            else if (ret == KB_UP)
            {
                lv_group_focus_prev(g);
                focusedObj = lv_group_get_focused(g);
                lv_obj_scroll_to_view(focusedObj, LV_ANIM_ON);
            }
            else if (ret == KB_DOWN)
            {
                lv_group_focus_next(g);
                focusedObj = lv_group_get_focused(g);
                lv_obj_scroll_to_view(focusedObj, LV_ANIM_ON);
            }
            else if (ret == KB_ENTER)
            {
                focusedObj = lv_group_get_focused(g);
                focusedIndex = lv_obj_get_index(focusedObj);
                if (ucConnectF && (connectIndex == focusedIndex))
                {
                    // The selected item is first item, and the first item is already connected. The confirm button displays the configuration list
                    sRC = WifiConfigInfo(iTimeout);

                    // Connection failed, return directly, need to refresh the list again
                    if ((sRC == NET_EXEC_FAIL)||(sRC == RET_OK))
                    {
                        return sRC;
                    }
                }
                else
                {
                    // Confirm key,connect
                    if (focusedIndex < apCount)
                    {
                        // Confirm key, execute wifi connection
                        sRC = UIWifiSelectNet(iTimeout, wifiInfo[focusedIndex]);
                        // Connect failed, return directly, need to refresh the list again
                        if ((sRC == NET_EXEC_FAIL)||(sRC == RET_OK))
                        {
                            return sRC;
                        }
                    }
                }
            }
            else if (ret == KB_CLEAR)
            {
                focusedObj = lv_group_get_focused(g);
                focusedIndex = lv_obj_get_index(focusedObj);

                memset(aucCurSSID, 0, sizeof(aucCurSSID));
                usCurSSIDLen = strlen((const char*)wifiInfo[focusedIndex].m_ausSSID);
                memcpy(aucCurSSID, wifiInfo[focusedIndex].m_ausSSID, usCurSSIDLen);

                if (LvUiMessageBox(TIP_WIFI_FORGET_NETWORK, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, "TIP", 3000) == KB_ENTER)
                {
                    Lib_WifiSetInfo(NET_WIFI_SETTING_DEL_SSID, aucCurSSID, usCurSSIDLen);
                    ucConnectF = FALSE;
                }

                return sRC;
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name    : UIWifiLinkActNoCofig
// Func    : Execute WiFi connection (without configuration, directly connect via WiFi)
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIWifiLinkActNoCofig(unsigned int iTimeout, unsigned char *pucSsid, unsigned char ucSsidLen, unsigned char *pucPwd, unsigned char ucPwdLen, unsigned char ucEncryptType)
{
    //unsigned short usS;
    unsigned int uiStartTime;
    char acDisBuff[96] = {0};
    short sRC;

    FUN_IN;

    /*memset(acDisBuff, 0, sizeof(acDisBuff));
    sprintf(acDisBuff, "\n%s", TIP_PLEASE_WAIT);
    MessageBox(acDisBuff, MB_CENTER |MB_COLOR_YELLOW| MB_ICONINFORMATION | MB_NOKEY, 0);

    usS = 60;
    sRC = CheckWiFiFree(&usS);
    if (sRC != RET_OK)
    {
        return sRC;
    }*/

    // Connect to AP
    if (ucEncryptType == 6)
    {
        char acBuff[48] = {0};
        sprintf(acBuff, "%d", ucEncryptType);
        Lib_WifiConnectAp((unsigned char *)acBuff, (unsigned char *)"NONE", (char *)pucSsid, pucPwd);
    }
    else
    {
        Lib_WifiConnectAp((unsigned char *)"WPA", (unsigned char *)"NONE", (char *)pucSsid, pucPwd);
    }
    uiStartTime = Lib_SysGetTimer();
    lv_obj_t * img = NULL;
    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, WIFICONNECTTING_GIF);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    WIFI_INF wifiInfo = { 0 };
    while (1)
    {
        sRC = GetLinkWifi(&wifiInfo);
        if ((RET_OK == sRC) || (RET_ERR_TIMEOUT == sRC))
        {
            break;
        }
        if (KB_CANCEL == LvUiWaitKey(50))
        {
            // escape/cancel key
            lv_obj_del(img);
            return UI_INPUT_CANCEL;
        }

        if ((NET_EXEC_TIMEOUT == sRC) || (IS_TIME_OUT(uiStartTime, WIFI_SET_NET_TIMEOUT)))
        {
            lv_obj_del(img);
            // timeout
            return RET_ERR_TIMEOUT;
        }
    }
    lv_obj_del(img);

    TRACE("UIWifiLinkActNoCofig src:%d", sRC);
    if (RET_OK == sRC)
    {
        memset(acDisBuff, 0, sizeof(acDisBuff));
        sprintf(acDisBuff, "\n%s", TIP_WIFI_CONNECT_SUCC);
        MessageBox(acDisBuff, MB_CENTER | MB_COLOR_GREEN | MB_ICONINFORMATION | MB_ANYKEY, 3000);

        if (strcmp((void *)wifiInfo.m_ausSSID, (void *)pucSsid) == 0)
        {
            TerminalParamSet(WIFI_SSID_BIT, pucSsid, strlen((char*)pucSsid));
            TerminalParamSet(WIFI_SSID_BIT, pucPwd, strlen((char*)pucPwd));
        }
    }
    else
    {
        memset(acDisBuff, 0, sizeof(acDisBuff));
        sprintf(acDisBuff, "\n%s", TIP_WIFI_CONNECT_FAIL);
        MessageBox(acDisBuff, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, 3000);
    }

    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : UIWifiLinkAct
// Func    : Execute WiFi connection (WiFi configuration+connection)
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIWifiLinkAct(unsigned int iTimeout, WIFI_INF wifiInfo)
{
    short sRC;
    char acDisBuff[128] = {0};

    sprintf(acDisBuff, "%s:%s\n%s:%s", TIP_WIFI_SSID, wifiInfo.m_ausSSID, TIP_WIFI_PASS, wifiInfo.m_aucPass);

    while (1)
    {
        sRC = LvUiMessageBox(acDisBuff, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, TIP_WIFI_PLEASE_CHECK, iTimeout);
        if (KB_ENTER == sRC)
        {
            break;
        }
        else if (KB_CANCEL == sRC || LVGL_OPER_TIMEOUT == sRC)
        {
            return sRC;
        }
    }

    return UIWifiLinkActNoCofig(iTimeout, wifiInfo.m_ausSSID, strlen((const char *)wifiInfo.m_ausSSID),
        wifiInfo.m_aucPass, strlen((const char *)wifiInfo.m_aucPass), wifiInfo.m_aucEncryptType);
}


//-----------------------------------------------------------------------------
// Name    : UIWifiSelectNet
// Func    : input WiFi password for WiFi connection
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
static short UIWifiSelectNet(int iTimeout, WIFI_INF wifiInfo)
{
    unsigned char aucPwd[WIFI_PWD_MAX_LEN * 2 + 1], ucSsidLen;
    unsigned short usPwdLen;
    short sRC;
    lv_obj_t *body = NULL;
    lv_obj_t *textArea = NULL;

    TRACE("UIWifiSelectNet");

    // Query password input/output buffer maximum length
    usPwdLen = sizeof(aucPwd);
    memset(aucPwd, 0x00, usPwdLen);

    ucSsidLen = strlen((const char *)wifiInfo.m_ausSSID);
    memcpy(aucPwd, wifiInfo.m_ausSSID, ucSsidLen);

    usPwdLen = strlen((const char *)wifiInfo.m_aucPass);

    // All APs automatically display a password input interface when connected,
    // and users can determine whether password input is necessary or not
    //if (!cEnc || ucPwdLen != 0)
    if (usPwdLen != 0)
    {
        sRC = UIWifiLinkAct(WIFI_INPUT_TIMEOUT, wifiInfo);
    }
    else
    {
        //memset(aucPwd, 0, sizeof(aucPwd));
        usPwdLen = sizeof(aucPwd);
        // Determine whether a password needs to be entered based on SSID
        sRC = Lib_WifiGetInfo(NET_WIFI_QUERY_SSID, aucPwd, &usPwdLen);

        if (sRC == RET_OK)
        {
            memset(wifiInfo.m_aucPass, 0, sizeof(wifiInfo.m_aucPass));
            memcpy(wifiInfo.m_aucPass, aucPwd, usPwdLen);
            sRC = UIWifiLinkAct(WIFI_INPUT_TIMEOUT, wifiInfo);
        }
        else
        {
            body = LvUiCreateInputTextBody(FONT_SIZE_TEXT_INPUT_TITLE, TIP_WIFI_INPUT_PASS, (char *)wifiInfo.m_ausSSID);
            textArea = LvUiCreateInputTextArea(body, (char *)aucPwd, FONT_SIZE_TEXT_INPUT, LVGL_INPUT_MODE_TEXT, 0, WIFI_PWD_MAX_LEN);
            while (1)
            {
                sRC = LvUiInputText(textArea, LVGL_INPUT_MODE_TEXT, 0, WIFI_PWD_MAX_LEN, WIFI_INPUT_TIMEOUT);
                if (sRC != LVGL_OPER_SUCC)
                {
                    FUN_IN;
                    lv_obj_del(body);
                    break;
                }
                else
                {
                    memset(aucPwd, 0, sizeof(aucPwd));
                    strcpy((char *)aucPwd, lv_textarea_get_text(textArea));
                    usPwdLen = strlen((const char *)aucPwd);
                    memcpy(wifiInfo.m_aucPass, aucPwd, usPwdLen + 1);
                }

                sRC = UIWifiLinkAct(WIFI_INPUT_TIMEOUT, wifiInfo);
                if (KB_CANCEL == sRC || LVGL_OPER_TIMEOUT == sRC)
                {
                    continue;
                }
                else
                {
                    lv_obj_del(body);
                    break;
                }
            }
        }
    }

    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : WifiQrInfoDeal
// Func    : Retrieve wireless network information from QR code
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
__attribute__((unused)) static short WifiQrInfoDeal(const unsigned char *pucSource, unsigned char *pucSsid, unsigned char *pucSsidLen, unsigned char *pucPwd, unsigned char *pucPwdLen)
{
    //    short sRC;
    unsigned char *pStr;
    unsigned int uiDelta;
//  unsigned char i;
    unsigned char ucBufLen, aucBuf[40];

    if (NULL == strstr((char*)pucSource, QRCODE_WIFI_INFO_STR_A) || NULL == strstr((char*)pucSource, QRCODE_WIFI_INFO_STR_B) ||
        NULL == strstr((char*)pucSource, QRCODE_WIFI_INFO_STR_C)
        )
    {
        return RET_ERR;
    }

    // Get SSID
    pStr = (unsigned char*)(strstr((char*)pucSource, QRCODE_WIFI_INFO_STR_C) + strlen(QRCODE_WIFI_INFO_STR_C));
    uiDelta = Strcspn((char*)pStr, QRCODE_WIFI_INFO_STR_D);
    if (uiDelta > WIFI_SSID_MAX_LEN)
    {
        return RET_ERR;
    }
    *pucSsidLen = uiDelta;
    memcpy(pucSsid, pStr, uiDelta);

    // Get PWD, some shared passwords use "", which needs to be filtered out first
    pStr = (unsigned char*)(strstr((char*)pucSource, QRCODE_WIFI_INFO_STR_B) + strlen(QRCODE_WIFI_INFO_STR_B));
    uiDelta = Strcspn((char*)pStr, QRCODE_WIFI_INFO_STR_D);
    //
    memcpy(aucBuf, pStr, uiDelta);
    ucBufLen = 0;

#if 1
    // Some shared passwords use "", which needs to be filtered out first: remove the "" before and after ""
    if ((0x22 == aucBuf[0]) && (0x22 == aucBuf[uiDelta-1]))
    {
        ucBufLen = uiDelta - 2;
        memcpy(pucPwd, aucBuf+1, ucBufLen);
    }
    else
    {
        ucBufLen = uiDelta;
        memcpy(pucPwd, aucBuf, ucBufLen);
    }
#else
    for (i = 0; i < uiDelta; i++)
    {
        if (0x22 != aucBuf[i])
        {
            *pucPwd++ = aucBuf[i];
            ucBufLen++;
        }
    }
#endif

    if (ucBufLen > WIFI_PWD_MAX_LEN)
    {
        return RET_ERR;
    }
    *pucPwdLen = ucBufLen;

    return RET_OK;
}


#endif

