#include "lvgl.h"
#include "api_lvgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//SDK head file
#include "Common.h"
#include "LibApi.h"
#include "M_NetInterface.h"
#include "Communication.h"
#include "Key.h"
#include "Language.h"
#include "MessageBox.h"
#include "custom_Lvgl.h"
#include "Power.h"
#include "Menu.h"
#include "Idle.h"
#include "lv_port_indev.h"

#define STATUS_BAR_HEIGHT               gStatusBarHeight

#define ARRAY_SIZE( ARRAY ) (sizeof (ARRAY) / sizeof (ARRAY[0]))

typedef struct
{
    int x;
    int y;
    int align;
    int font;
    char *text;
    lv_color_t bgColor;
    lv_color_t textColor;
} t_lvgl_text;

typedef struct
{
    int id;
    int width;
    int height;
    int align;
    int font;
    char *bgImg;
    char *noImg;
    char *icon;
    char *text;
    int textXOff;
    lv_color_t bgColor;
    lv_color_t textColor;
} t_lvgl_button;

typedef struct
{
    int range;
    int timeout;
    int curTime;
    int tickTime;
    int isCountDown;
    lv_obj_t *dialog;
    lv_obj_t *image;
    lv_obj_t *msgLabel;
    lv_obj_t *timeLabel;
    lv_obj_t *titleLabel;
} t_lvgl_dialog;

static lv_obj_t *gScrObj = NULL;
static lv_obj_t *gStatusBarObj = NULL;
static lv_obj_t *gPageBodyObj = NULL;

static t_lvgl_button *gButtonItems[MAX_BUTTON_PAGE] = { NULL };
static lv_obj_t *gTimeLabel = NULL;
static lv_timer_t *gTimeTask = NULL;

static lv_timer_t *gDialogTask = NULL;

static int gKeyPressed = -1;
static int gBtnSelected = -1;
static int gHasStatusBar = 0;
static int gHasStatusBarIcon = 0;
static lv_obj_t *gUserStatusBarIcon[MAX_USER_STATUSBAR_ICON] = { 0 };
static status_bar_cb gStatusBarCb = NULL;
static unsigned int gStatusBarHeight = 30;

lv_obj_t* gTextList[8] = { 0 };

static char *gStatusBarIconPower[] =
{
    "P:lvgl/power0.png",
    "P:lvgl/power1.png",
    "P:lvgl/power2.png",
    "P:lvgl/power3.png",
    "P:lvgl/power4.png",
    "P:lvgl/power5.png",
    "P:lvgl/power6.png",
};

static char *gStatusBarIconSigna[] =
{
    "P:lvgl/signal0.png",
    "P:lvgl/signal1.png",
    "P:lvgl/signal2.png",
    "P:lvgl/signal3.png",
    "P:lvgl/signal4.png",
    "P:lvgl/no_sim.png",
};

static char *gStatusBarIconWifi[] =
{
    "P:lvgl/wifi0.png",
    "P:lvgl/wifi1.png",
    "P:lvgl/wifi2.png",
    "P:lvgl/wifi3.png",
    "P:lvgl/wifi4.png",
};

static char *gStatusBarIconG[] =
{
    "P:lvgl/img_2g.png",
    "P:lvgl/img_3g.png",
    "P:lvgl/img_4g.png",
};


static void LvUiKeyCb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_KEY)
    {
        const void* d = lv_event_get_param(event);
        if (d)
        {
            memcpy(&gKeyPressed, d, sizeof (int));
            // BeepOn();
        }
    }
}

void LvUiBtnClickedCb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);

    //Lib_Trace ( "[button_callback] code: %d, gBtnSelected: %d", code, gBtnSelected);

    if (code == LV_EVENT_CLICKED)
    {
        if (gBtnSelected < 0)
        {
            gBtnSelected = ( int ) event->user_data;
            //Lib_Trace ( "[button_callback] gBtnSelected: %d", gBtnSelected);
            BeepOn();
        }
    }
}


void objCallbackForRefreshSleepTime(lv_event_t *event)
{
    if (gBtnSelected < 0)
    {
        gBtnSelected = LVGL_BUTTON_SELECTED_FOR_REFRESH;
    }
}

void LvUiSetCb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
{
    LvGuiGroupSetObj(obj);
    lv_obj_add_event_cb(obj, event_cb, filter, user_data);
}

void LvUiCreateStatusBarTask(status_bar_cb task_cb)
{
    gStatusBarCb = task_cb;
}

int LvUiSetStatusBarIcon(int x, char *iconname, int group)
{
    if (!(iconname && strlen (iconname)))
    {
        return LVGL_OPER_FAIL;
    }
    if ((group > MAX_USER_STATUSBAR_ICON) || (group < 0))
    {
        return LVGL_OPER_FAIL;
    }
    if (NULL == gUserStatusBarIcon[group])
    {
        gUserStatusBarIcon[group] = lv_img_create(gStatusBarObj);
        if (!gUserStatusBarIcon[group])
        {
            return LVGL_OPER_FAIL;
        }
    }

    lv_img_set_src(gUserStatusBarIcon[group], iconname);
    lv_obj_align_to(gUserStatusBarIcon[group], gStatusBarObj, LV_ALIGN_LEFT_MID, x, 0);

    return LVGL_OPER_SUCC;
}

static void LvUiBatIcon(lv_obj_t* parent)
{
    int index = 0;
    static lv_obj_t* status_bar_obj = NULL;
    static lv_obj_t* status_bar_parent = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if (NULL != parent)
    {
        status_bar_parent = parent;
    }

    //Do not display the default status bar icon
    if (gHasStatusBarIcon == 0)
    {
        if (NULL != status_bar_obj)
        {
            lv_obj_add_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }
    else
    {
        if (NULL != status_bar_obj)
        {
            lv_obj_clear_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN);
        }
    }

    if (NULL == status_bar_obj)
    {
        status_bar_obj = lv_img_create(status_bar_parent);
        stCtrlObjAttr.m_align = LV_ALIGN_RIGHT_MID;
        stCtrlObjAttr.m_x = -4;
//        stCtrlObjAttr.m_y = -9;
    }

    if (Lib_ExtPower())
    {
        // charging
        if (Lib_BatGetChargeState() != 0)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[5];
        }
        else
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[6];
        }
    }
    else
    {
        index = Lib_BatGetCapacity();

        if (index >= 80)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[4];
        }
        else if (index >= 60)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[3];
        }
        else if (index >= 40)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[2];
        }
        else if (index >= 20)
        {
            // 3.6-3.5V voltage drops slowly, optimizing battery cell count display to enhance low battery
            // interruption user experience
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[1];
        }
        else
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconPower[0];
        }
    }

    LvSetCtrlObj(status_bar_obj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void LvUiMobileCommIcon(lv_obj_t* parent)
{
    int index = 0;
    int mode = 0;
    unsigned char net_act;
    static lv_obj_t* status_bar_obj = NULL;
    static lv_obj_t* status_bar_obj_g = NULL;
    static lv_obj_t* status_bar_parent = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if (NULL != parent)
    {
        status_bar_parent = parent;
    }

    // not display the default status bar icon
    if (gHasStatusBarIcon == 0)
    {
        if ((NULL != status_bar_obj ) && ( NULL != status_bar_obj_g))
        {
            lv_obj_add_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(status_bar_obj_g, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }
    else
    {
        if ((NULL != status_bar_obj) && (NULL != status_bar_obj_g))
        {
            lv_obj_clear_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN );
            lv_obj_clear_flag(status_bar_obj_g, LV_OBJ_FLAG_HIDDEN);
        }
    }

    mode = GetNetWorkMode();

    if (NET_MODE_GPRS == mode)
    {
        // gprs mode
        if (NULL == status_bar_obj)
        {
            status_bar_obj = lv_img_create(status_bar_parent);
        }
        if (status_bar_obj_g == NULL)
        {
            status_bar_obj_g = lv_img_create(status_bar_obj);
        }

        index = Lib_WlsCheckSim();//Determine if there is a SIM
        if ( index == WLS_NOSIM )
        {
            index = 5;
        }
        else if (Lib_WlsGetInitStatus() < 0)
        {
            index = 0;
        }
        else
        {
            int sCsqD;
            Lib_WlsCheckSignal(&sCsqD);

            if ((sCsqD > 31) || (sCsqD <= 0))
            {
                //Signal unknown
            }
            sCsqD = -113 + 2 * sCsqD;
            TRACE("Lib_WlsCheckSignal UI sCsqD = %d dB",sCsqD);

            if (sCsqD < -113)
            {
                //Signal unavailable
                index = 0;
            }
            else if (sCsqD < -97)
            {
                index = 1;
            }
            else if (sCsqD < -81)
            {
                index = 2;
            }
            else if (sCsqD < -65)
            {
                index = 3;
            }
            else
            {
                index = 4;
            }
        }

        if ((index >= 0) && (index <= 4))
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconSigna[index];
        }
        else
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconSigna[5];
        }
        stCtrlObjAttr.m_align = LV_ALIGN_LEFT_MID;
        stCtrlObjAttr.m_x = 4;
        LvSetCtrlObj(status_bar_obj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
#if !defined( WIN32 )
        Lib_WlsNetActRefresh(&net_act);
#endif
        if (net_act == NET_SYS_2G)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconG[0];
        }
        else if (net_act == NET_SYS_3G)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconG[1];
        }
        else if (net_act == NET_SYS_4G)
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconG[2];
        }
        else
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = NULL;
        }
        stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
        stCtrlObjAttr.m_x = 0;
        LvSetCtrlObj(status_bar_obj_g, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        // WiFi mode, remove SIM card identification
        if (status_bar_obj_g)
        {
            lv_obj_del(status_bar_obj_g);
            status_bar_obj_g = NULL;
        }
        if (status_bar_obj)
        {
            lv_obj_del(status_bar_obj);
            status_bar_obj = NULL;
        }
    }
}

static void LvUiWiFiIcon ( lv_obj_t* parent )
{
    int index = 0;
    int mode = 0;
    static lv_obj_t* status_bar_obj = NULL;
    static lv_obj_t* status_bar_parent = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if (NULL != parent)
    {
        status_bar_parent = parent;
    }

    //Do not display the default status bar icon
    if (gHasStatusBarIcon == 0)
    {
        if (NULL != status_bar_obj)
        {
            lv_obj_add_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN);
        }

        return;
    }
    else
    {
        if (NULL != status_bar_obj)
        {
            lv_obj_clear_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN);
        }

    }

    mode = GetNetWorkMode();

    if (mode == 1)//wifi mode
    {
        int iRssiVal;

        if (NULL == status_bar_obj)
        {
            status_bar_obj = lv_img_create(status_bar_parent);
            stCtrlObjAttr.m_align = LV_ALIGN_LEFT_MID;
            stCtrlObjAttr.m_x = 4;
//            stCtrlObjAttr.m_y = -11;
        }

        short status = Lib_WifiGetApStatus();

        if ((status == STA_AP_CONNECTED) || (status == WifiLinkTcpSta))
        {
            Lib_WifiCheckSignal(&iRssiVal);

            if (iRssiVal < -100)
            {
                //Signal unavailable
                index = 0;
            }
            else if (iRssiVal < -85)
            {
                index = 1;
            }
            else if (iRssiVal < -70)
            {
                index = 2;
            }
            else if (iRssiVal < -50)
            {
                index = 3;
            }
            else
            {
                index = 4;
            }

        }
        else
        {
            index = 0;
        }
        if ((index > 0) && (index <= 4))
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconWifi[index];
        }
        else
        {
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = gStatusBarIconWifi[0];
        }

        LvSetCtrlObj(status_bar_obj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else//Delete WiFi identifier in GPRS mode
    {
        if (status_bar_obj)
        {
            lv_obj_del(status_bar_obj);
            status_bar_obj = 0;
        }
    }
}

static void LvUiTimeIcon(lv_obj_t *parent)
{
    char str[32] = { 0 };
    unsigned char aucTimeBcd[6];
    unsigned char aucTimeStr[12];

    if (gTimeLabel == NULL)
    {
        CTRL_OBJ_ATTR stCtrlObjAttr = {0};

        gTimeLabel = lv_label_create(gStatusBarObj);
        if (!gTimeLabel)
        {
            Lib_LvglLock(0);
            return;
        }

        stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
        stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_BLACK;
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_STATUSBAR_TIME);
        stCtrlObjAttr.m_stLabelAttr.m_bLabelIsRecolor = true;
        LvSetCtrlObj(gTimeLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    //1.Set status bar time
    Lib_GetDateTime(aucTimeBcd);
    Lib_Bcd2Asc(aucTimeBcd, (char *)aucTimeStr, 6);

    sprintf(str, "#000000 %c%c:%c%c# ", aucTimeStr[6], aucTimeStr[7], aucTimeStr[8], aucTimeStr[9]);

    if (gTimeLabel)
    {
        //lv_label_set_recolor(lab_time, true);
        //Do not display the default status bar icon
        if (gHasStatusBarIcon == 0)
        {
            lv_obj_add_flag(gTimeLabel, LV_OBJ_FLAG_HIDDEN);
            return;
        }
        else
        {
            lv_obj_clear_flag(gTimeLabel, LV_OBJ_FLAG_HIDDEN);
        }

        lv_label_set_text(gTimeLabel, str);
    }
}

static void LvUiStatusBarTask(struct _lv_timer_t *argv)
{
    // User defined status bar icon
    if (NULL != gStatusBarCb)
    {
        gStatusBarCb();
    }

    //wifi sim power
    LvUiBatIcon(gStatusBarObj);
    LvUiMobileCommIcon(gStatusBarObj);
    LvUiWiFiIcon(gStatusBarObj);
    LvUiTimeIcon(gStatusBarObj);
}

void LvUiSetStatusBar(int height, lv_color_t bgColor)
{
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    STATUS_BAR_HEIGHT = height;

    stCtrlObjAttr.m_usWidth = lv_obj_get_width (gScrObj);
    stCtrlObjAttr.m_usHeight = STATUS_BAR_HEIGHT;
    stCtrlObjAttr.m_stBgAttr.m_color = bgColor;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;

    LvSetCtrlObj(gStatusBarObj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
}

unsigned int LvUiGetStatusBarHeight(void)
{
    return STATUS_BAR_HEIGHT;
}

void LvUiCreateStatusBar()
{
    static int userData = 10;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    FUN_IN;
    //״̬�
    Lib_LvglLock(1);
    gStatusBarObj = lv_obj_create (gScrObj);
    if (!gStatusBarObj)
    {
        Lib_LvglLock(0);
        return;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    lv_obj_remove_style_all(gStatusBarObj);
    stCtrlObjAttr.m_usWidth = lv_obj_get_width (gScrObj);
    stCtrlObjAttr.m_usHeight = STATUS_BAR_HEIGHT;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
    LvSetCtrlObj(gStatusBarObj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Start the task
    if (gTimeTask)
    {
        lv_timer_del (gTimeTask);
        gTimeTask = NULL;
    }
    gTimeTask = lv_timer_create(LvUiStatusBarTask, 1000, &userData);
    lv_timer_ready(gTimeTask);

    Lib_LvglLock(0);
}

static void LvUiCreateMenuBody(void)
{
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    Lib_LvglLock(1);
    gPageBodyObj = lv_obj_create(gScrObj);
    if (!gPageBodyObj)
    {
        Lib_LvglLock(0);
        return;
    }

    lv_obj_remove_style_all(gPageBodyObj);

    stCtrlObjAttr.m_usWidth = lv_obj_get_width (gScrObj);
    stCtrlObjAttr.m_usHeight = lv_obj_get_height(gScrObj) - (0 == gHasStatusBar ? 0 : STATUS_BAR_HEIGHT);
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_x = 0;
    stCtrlObjAttr.m_y = (0 == gHasStatusBar ? 0 : STATUS_BAR_HEIGHT);
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = PAGE_BG_COLOR;
    stCtrlObjAttr.m_stBaseDirAttr.m_bIsSet = TRUE;
    stCtrlObjAttr.m_stBaseDirAttr.m_baseDir = LV_BASE_DIR_AUTO;
    LvSetCtrlObj(gPageBodyObj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(gPageBodyObj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(gPageBodyObj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(gPageBodyObj, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    lv_obj_add_event_cb(gPageBodyObj, objCallbackForRefreshSleepTime, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(gPageBodyObj, objCallbackForRefreshSleepTime, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(gPageBodyObj, objCallbackForRefreshSleepTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(gPageBodyObj, objCallbackForRefreshSleepTime, LV_EVENT_SCROLL, NULL);

    LvUiSetCb(gPageBodyObj, LvUiKeyCb, LV_EVENT_KEY, NULL);

    lv_obj_update_layout(gPageBodyObj);

    Lib_LvglLock(0);
}

static void LvUiReleaseButtonList()
{
    int i = 0;

    for ( i = 0; i < MAX_BUTTON_PAGE; i++)
    {
        if (gButtonItems[i] != NULL )
        {
            Lib_free ( gButtonItems[i]);
            gButtonItems[i] = NULL;
        }
    }
}

static void LvUiReleaseTextList()
{
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        if (gTextList[i])
        {
            lv_obj_del(gTextList[i]);
            gTextList[i] = NULL;
        }
    }
}

int LvUiInit()
{
    if (!gScrObj)
    {
        gScrObj = lv_scr_act();
    }
    if (!gScrObj)
    {
        return -1;
    }

    Lib_LvglLock(1);
    lv_obj_remove_style_all(gScrObj);
    Lib_LvglLock(0);

    // status bar
    gHasStatusBar = 1;
    gHasStatusBarIcon = 1;
    LvUiCreateStatusBar();
    LvUiCtrlStatusBar(0);
    // main menu
    LvUiCreateMenuBody();

    //
    Lib_LvglLock(1);

    //LvUiLoadStatusBarIcon();
    Lib_LvglLock(0);
    LvUiFunctionRegisterToSdk();
    Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

    return LVGL_OPER_SUCC;
}

void LvUiCtrlStatusBar(int status)
{
    if (status)
    {
        unsigned short w, h, font, forecolor, backcolor;
        Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);
        lv_obj_set_style_bg_color(gStatusBarObj, (lv_color_t)forecolor, LV_PART_MAIN|LV_STATE_DEFAULT);
        gHasStatusBar = 1;
    }
    else
    {
        gHasStatusBar = 0;
    }
}

void LvUiResetStatusBarIcons(int status)
{
    if (status)
    {
        gHasStatusBarIcon = 1;
    }
    else
    {
        gHasStatusBarIcon = 0;
    }
}

void LvUilCls()
{
    Lib_LvglLock(1);
    LvUiReleaseButtonList();
    LvUiReleaseTextList();
    Lib_LvglLock(0);

    if (gPageBodyObj)
    {
        Lib_LvglLock(1);
        lv_obj_del(gPageBodyObj);
        gPageBodyObj = NULL;
        Lib_LvglLock(0);
        LvUiCreateMenuBody();
    }
}

static int LvUiGetBtnItemIndex()
{
    int index = 0;

    for (index = 0; index < MAX_BUTTON_PAGE; index++)
    {
        if (gButtonItems[index] == NULL)
        {
            return index;
        }
    }
    return -1;
}

extern unsigned long s_ulDelayStart;

unsigned char LvGetKey(void)
{
    unsigned char ret = KB_NULL;
    unsigned int tick = 0;
    int timeout = 20;

    while (1)
    {
        Lib_LvglTask();

        if (gKeyPressed >= 0)
        {
            TRACE("apilvgl button event gKeyPressed = %d", gKeyPressed);
            ret = gKeyPressed;
            gKeyPressed = -1;
            break;
        }

        if ((Lib_SysGetTimer() - tick) >= (unsigned int)timeout)
        {
            break;
        }
    }

    if (ret != KB_NULL)
    {
        //Refresh the sleep time
        FUN_IN;
        DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);

        //The backlight is on and the extinguishing delay is set
        Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        s_ulDelayStart = Lib_SysGetTimer();
    }

    return ret;
}

static int LvUiBtnWait(int timeout)
{
    unsigned int tick = 0;
    int ret = 0;
    unsigned char ucCode;
    short sRC;
    static bool tamperFlag = false;

    gKeyPressed = -1;
    gBtnSelected = -1;

    tick = Lib_SysGetTimer();

    if (timeout < 20)
    {
        timeout = 20;
    }

    while (1)
    {
        Lib_LvglTask();

        if (gBtnSelected >= 0)
        {
            TRACE("apilvgl button event gBtnSelected = %d", gBtnSelected);
            ret = gBtnSelected;
            gBtnSelected = -1;
            break;
        }
        if (gKeyPressed >= 0)
        {
            TRACE("apilvgl button event gKeyPressed = %d", gKeyPressed);
            ret = gKeyPressed;
            gKeyPressed = -1;
            break;
        }

        if ((Lib_SysGetTimer() - tick) >= (unsigned int)timeout)
        {
            ret = LVGL_OPER_TIMEOUT;
            break;
        }

        sRC = PowerOffOp(60000);
        if (sRC < 0)
        {
            return KB_CANCEL;
        }

        // Hibernate the device
        DeviceSleepProcess();

        //other key query service
        KbOtherQueryOp();

        if (0 == Lib_TamperIsOccured(&ucCode))
        {
            UIWireNetInit();

            if (!tamperFlag)
            {
                LvUiCtrlStatusBar(1);
                Lib_MaintenanceMode();
                tamperFlag = true;
            }
        }
        else
        {
            tamperFlag = false;
        }
    }
    Lib_Trace("[LvUiBtnWait] ret: %d", ret);

    if (ret >= 0)
    {
        //Refresh the sleep time
        FUN_IN;
        DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);

        //The backlight is on and the extinguishing delay is set
        Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        s_ulDelayStart = Lib_SysGetTimer();
    }

    //LvUiSetCb(gPageBodyObj, LvUiKeyCb);
    return ret;
}

static const char* keybuff[10] =
{
    "0 .,'?!\"-*@/\\:_;+&%#$=()<>^","1QZqz","2ABCabc","3DEFdef","4GHIghi",
    "5JKLjkl","6MNOmno","7PRSprs","8TUVtuv","9WXYwxy",
};

static char _input_newchar ( char oldchar )
{
    int i, j;
    int size;
    char ch = oldchar;
    for (i = 0; i < (sizeof(keybuff) / sizeof(const char *) + 1); i++)
    {
        size = strlen ( keybuff[i]);
        for (j = 0; j < size; j++)
        {
            if (oldchar == keybuff[i][j])
            {
                j = ( j + 1 ) % size;
                ch = keybuff[i][j];
                return ch;
            }
        }
    }
    return oldchar;
}

static lv_obj_t* LvUiCreateLabelNobgcolor(t_lvgl_text *item)
{
    lv_obj_t* label = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    parentWidth = lv_obj_get_width(gPageBodyObj);

    Lib_LvglLock(1);
    if (item->y == 0)
    {
        lv_obj_t *obj_bg = lv_obj_create(gPageBodyObj);
        if (!obj_bg)
        {
            Lib_LvglLock(0);
            return NULL;
        }
        stCtrlObjAttr.m_usWidth = parentWidth;
        stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(item->font)) + lv_font_get_line_height(Lib_LvglGetFont(item->font)) / 10;
        stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
        stCtrlObjAttr.m_y = item->y;
        stCtrlObjAttr.m_stBgAttr.m_color = item->bgColor;
        LvSetCtrlObj(obj_bg, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    label = lv_label_create(gPageBodyObj);
    if (!label)
    {
        Lib_LvglLock(0);
        return NULL;
    }

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_usWidth = parentWidth;
    if (item->align == LVGL_ALIGN_CENTER)
    {
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    }
    else if (item->align == LVGL_ALIGN_RIGHT)
    {
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_RIGHT;
    }
    else
    {
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
    }
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(item->font);
    stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(item->font)) + lv_font_get_line_height(Lib_LvglGetFont(item->font)) / 10;
    stCtrlObjAttr.m_stBgAttr.m_color = item->bgColor;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_0;
    stCtrlObjAttr.m_stTextAttr.m_color = item->textColor;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_SCROLL_CIRCULAR;
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = item->text;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_x = item->x;
    stCtrlObjAttr.m_y = item->y;
    LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    Lib_LvglLock(0);
    return label;
}

static int LvUiBtn(int x, int y, t_lvgl_button items[], int size)
{
    int i = 0;
    int buttonWidth = 0;
    int noImgWidth = 0;
    int iconWidth = 0;
    char numArray[10];
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    unsigned short w, h, font, forecolor, backcolor;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    Lib_LvglLock(1);
    for (i = 0; i < size; i++)
    {
        lv_obj_t *button = NULL;
        lv_obj_t *icon = NULL;
        lv_obj_t *noImg = NULL;
        lv_obj_t *noLabel = NULL;

        int index = LvUiGetBtnItemIndex();
        if (index < 0)
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        gButtonItems[index] = Lib_malloc(sizeof(t_lvgl_button));
        if (!gButtonItems[index])
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        memcpy(gButtonItems[index], &items[i], sizeof(t_lvgl_button));

        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        if (items[i].bgImg && strlen(items[i].bgImg) > 0)
        {
            button = lv_imgbtn_create(gPageBodyObj);
            if (button)
            {
                stCtrlObjAttr.m_stImgAttr.m_imageBtnstate = LV_IMGBTN_STATE_RELEASED;
                stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc = items[i].bgImg;
            }
        }
        else
        {
            button = lv_btn_create(gPageBodyObj);
        }
        if (!button)
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        stCtrlObjAttr.m_ucBidiFlag = TRUE;
        stCtrlObjAttr.m_stBgAttr.m_color = items[i].bgColor;
        stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
        stCtrlObjAttr.m_x = x;
        stCtrlObjAttr.m_y = y;

        if (items[i].align == 1)
        {
            stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
            stCtrlObjAttr.m_x = 0;
            stCtrlObjAttr.m_y = y;
        }
        else if (items[i].align == 2)
        {
            stCtrlObjAttr.m_align = LV_ALIGN_TOP_RIGHT;
            stCtrlObjAttr.m_x = 0;
            stCtrlObjAttr.m_y = y;
        }
        LvSetCtrlObj(button, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_update_layout(button);
        buttonWidth = lv_obj_get_width(button);

        lv_obj_add_event_cb(button, LvUiBtnClickedCb, LV_EVENT_CLICKED, (void *)items[i].id);

        if (items[i].noImg && strlen(items[i].noImg) > 0)
        {
            noImg = lv_imgbtn_create(button);
            if (noImg)
            {
                memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
                stCtrlObjAttr.m_ucBidiFlag = TRUE;
                stCtrlObjAttr.m_stImgAttr.m_imageBtnstate = LV_IMGBTN_STATE_RELEASED;
                stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc = items[i].noImg;
                stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
                stCtrlObjAttr.m_stImgAttr.m_imageRecolor = (lv_color_t)forecolor;
                stCtrlObjAttr.m_stImgAttr.m_ucimageRecolorOpa = LV_OPA_100;
                LvSetCtrlObj(noImg, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            else
            {
                Lib_LvglLock(0);
                return LVGL_OPER_FAIL;
            }
            lv_obj_update_layout(noImg);
            noImgWidth = lv_obj_get_width(noImg);

            noLabel = lv_label_create(noImg);
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_MENU_ID);
            sprintf(numArray, "%c", items[i].id);
            stCtrlObjAttr.m_stTextAttr.m_pcTextContent = numArray;
            stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_WHITE;
            stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
            LvSetCtrlObj(noLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        if (items[i].icon && strlen(items[i].icon) > 0)
        {
            icon = lv_imgbtn_create(button);
            if (icon)
            {
                memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
                stCtrlObjAttr.m_ucBidiFlag = TRUE;
                stCtrlObjAttr.m_stImgAttr.m_imageBtnstate = LV_IMGBTN_STATE_RELEASED;
                stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc = items[i].icon;
                stCtrlObjAttr.m_align = LV_ALIGN_LEFT_MID;
                stCtrlObjAttr.m_x = noImgWidth;
                LvSetCtrlObj(icon, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_add_event_cb(icon, LvUiBtnClickedCb, LV_EVENT_CLICKED, (void *)items[i].id);
            }
            else
            {
                Lib_LvglLock(0);
                return LVGL_OPER_FAIL;
            }
            lv_obj_update_layout(icon);
            iconWidth = lv_obj_get_width(icon);
        }

        if (items[i].text && strlen(items[i].text) > 0)
        {
            lv_obj_t* label = NULL;

            label = lv_label_create(button);
            if (!label)
            {
                Lib_LvglLock(0);
                return LVGL_OPER_FAIL;
            }

            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_ucBidiFlag = TRUE;
            stCtrlObjAttr.m_usWidth = buttonWidth - buttonWidth / 20 - noImgWidth - iconWidth;
            stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
            stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_MENU_ITEM_TEXT);
            stCtrlObjAttr.m_stTextAttr.m_color = items[i].textColor;
            stCtrlObjAttr.m_stTextAttr.m_pcTextContent = items[i].text;
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            stCtrlObjAttr.m_align = LV_ALIGN_LEFT_MID;
            stCtrlObjAttr.m_x = buttonWidth / 20 + noImgWidth + iconWidth;
            LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

    }
    Lib_LvglLock(0);

    return LVGL_OPER_SUCC;
}

int LvUiWaitKey(int timeout)
{
    return LvUiBtnWait(timeout);
}

int LvUiText(int align, int x, int y, int font, char* text, lv_color_t textColor)
{
    t_lvgl_text item = { 0 };
    lv_obj_t* label = NULL;

    item.x = x;
    item.y = y;
    item.align = align;
    item.font = font;
    item.text = text;
    item.bgColor = PAGE_BG_COLOR;
    item.textColor = textColor;
    label = LvUiCreateLabelNobgcolor(&item);
    if (!label)
    {
        return LVGL_OPER_FAIL;
    }

    return LVGL_OPER_SUCC;
}


int LvUiShowImage(int x, int y, t_lvgl_image *img, int id)
{
    int index = 0;
    t_lvgl_button items[1] = {0};
    lv_obj_t *image = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if (!img->bgImg)
    {
        return LVGL_OPER_FAIL;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    items[0].id = id;
    items[0].text = img->text;
    items[0].bgImg = img->bgImg;
    items[0].noImg = img->noImg;
    items[0].icon = img->icon;
    items[0].textXOff = img->textXOff;
    items[0].width = 0;
    items[0].height = 0;
    items[0].bgColor = BUTTON_BG_COLOR;
    items[0].textColor = (lv_color_t)forecolor;
    if (id >= 0)
    {
        return LvUiBtn(x, y, items, 1);
    }
    else if (id < 0)
    {
        Lib_LvglLock(1);

        image = lv_img_create(gPageBodyObj);
        if (!image)
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        index = LvUiGetBtnItemIndex();
        if (index < 0)
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        gButtonItems[index] = Lib_malloc(sizeof(t_lvgl_button));
        if (!gButtonItems[index])
        {
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }
        memcpy(gButtonItems[index], &items[0], sizeof(t_lvgl_button));

        stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = img->bgImg;
        stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
        stCtrlObjAttr.m_x = x;
        stCtrlObjAttr.m_y = y;
        stCtrlObjAttr.m_stImgAttr.m_imageRecolor = (lv_color_t)forecolor;
        if (!img->noRecolor)
        {
            stCtrlObjAttr.m_stImgAttr.m_ucimageRecolorOpa = LV_OPA_100;
        }

        LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        Lib_LvglLock(0);

        return LVGL_OPER_SUCC;
    }

    return LVGL_OPER_FAIL;
}


int LvUiInputText(lv_obj_t* ta, int mode, int min, int max, int timeout)
{
    unsigned int tick = 0;
    int ret = 0, size = 0;
    char buf[32 + 1] = { 0 };
    long long amount = 0;
    int keyCharSwitch = 0;
    int inputTimeout = 0;
    //nAmount = nAmount * 10 + (presskey - GUI_KEY_0);

    gKeyPressed = -1;
    gBtnSelected = -1;
    tick = Lib_SysGetTimer();
    while ( 1 )
    {
        if (keyCharSwitch)
        {
            if ((Lib_SysGetTimer() - inputTimeout ) / 1000 >= 3)
            {
                keyCharSwitch = 0;
            }
        }

        Lib_LvglTask();

        // Hibernate the device
        DeviceSleepProcess();

        //other key query service
        KbOtherQueryOp();

        if ((Lib_SysGetTimer() - tick ) / 1000 >= ( unsigned int ) timeout)
        {
            ret = LVGL_OPER_TIMEOUT;
            break;
        }
        if (gKeyPressed <= 0 && gBtnSelected <= 0)
        {
            continue;
        }
        Lib_LvglLock(1);
        const char* text = lv_textarea_get_text (ta);
        Lib_LvglLock(0);
        size = strlen ( text );
        if (gKeyPressed >= KB_DIGIT_ZERO && gKeyPressed <= KB_DIGIT_NINE)
        {
            if (mode == LVGL_INPUT_MODE_AMOUNT)
            {

                if (size < max)
                {
                    amount = amount * 10 + ( gKeyPressed - KB_DIGIT_ZERO );
                    sprintf(buf, "%lld", amount);
                    size = strlen ( buf );

                    sprintf(buf, "%lld.%02lld", amount / 100, amount % 100);
                    Lib_LvglLock(1);
                    lv_textarea_set_text(ta, buf);
                    Lib_LvglLock(0);
                }
            }
            else
            {
                if (size < max)
                {
                    Lib_LvglLock(1);
                    lv_textarea_set_placeholder_text (ta, "");
                    sprintf(buf, "%d", (gKeyPressed - KB_DIGIT_ZERO));
                    lv_textarea_add_text (ta, buf);
                    Lib_LvglLock(0);
                }
            }
        }
        else if (gKeyPressed == KB_DOWN && mode == LVGL_INPUT_MODE_TEXT && keyCharSwitch == 1)
        {
            if (size > 0)
            {
                //buf[0] = _input_newchar ( text[size - 1]);
                buf[0] = _input_newchar(buf[0]);
                Lib_LvglLock(1);
                lv_textarea_del_char ( ta );
                lv_textarea_add_text ( ta, buf );
                Lib_LvglLock(0);
            }
        }
        else if (gKeyPressed == KB_UP)
        {
            lv_textarea_cursor_left(ta);
        }
        else if (gKeyPressed == KB_DOWN)
        {
            lv_textarea_cursor_right(ta);
        }
        else if (gKeyPressed == KB_CLEAR)
        {
            if (size > 0)
            {
                if (mode == LVGL_INPUT_MODE_AMOUNT && text[size - 1] >= KB_DIGIT_ZERO && text[size - 1] <= KB_DIGIT_NINE)
                {
                    amount = amount / 10;
                    sprintf ( buf, "%lld.%02lld", amount / 100, amount % 100 );
                    Lib_LvglLock(1);
                    lv_textarea_set_text ( ta, buf );
                    Lib_LvglLock(0);
                }
                else
                {
                    Lib_LvglLock(1);
                    lv_textarea_del_char ( ta );
                    Lib_LvglLock(0);
                }
            }
        }
        else if (gKeyPressed == KB_CANCEL || gBtnSelected == KB_CANCEL)
        {
            ret = LVGL_OPER_CANCEL;
            break;
        }
        else if (gKeyPressed == KB_ENTER || gBtnSelected == KB_ENTER)
        {
            if (mode == LVGL_INPUT_MODE_AMOUNT)
            {
                size = strlen(buf);
                if (size)
                {
                    sprintf(buf, "%lld", amount);
                }
                if (size >= min && size <= max)
                {
                    Lib_LvglLock(1);
                    lv_textarea_set_text(ta, buf);
                    Lib_LvglLock(0);
                    break;
                }
            }
            else if (size >= min && size <= max)
            {
                ret = LVGL_OPER_SUCC;
                break;
            }
        }

        // 按数字键后再按字母键才需要切换字符
        if ((gKeyPressed >= KB_DIGIT_ZERO && gKeyPressed <= KB_DIGIT_NINE)
            || ((gKeyPressed == KB_DOWN) && (keyCharSwitch == 1)))
        {
            keyCharSwitch = 1;
            inputTimeout = Lib_SysGetTimer();
        }
        else
        {
            keyCharSwitch = 0;
        }

        {
            //Refresh the sleep time
            DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);

            //The backlight is on and the extinguishing delay is set
            Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
            s_ulDelayStart = Lib_SysGetTimer();
        }

        gKeyPressed = -1;
        gBtnSelected = -1;
    }

    gKeyPressed = -1;
    gBtnSelected = -1;
    //LvUiSetCb ( gPageBodyObj, LvUiKeyCb, LV_EVENT_KEY, NULL );

    return ret;
}

lv_obj_t* LvUiCreateInputTextBody(int font, char* title, char* msg)
{
    lv_obj_t *body = NULL;
    lv_obj_t *titleLabel = NULL;
    lv_obj_t *msgLabel = NULL;
    unsigned short w, h, f, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;

    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    body = lv_obj_create(gPageBodyObj);
    titleLabel = lv_label_create(body);

    Lib_LcdParaGet(&w, &h, &f, &forecolor, &backcolor);

    //set body
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    LvSetCtrlObj(body, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(body, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(body, LV_FLEX_ALIGN_SPACE_EVENLY, 0, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(body, LV_OBJ_FLAG_EVENT_BUBBLE);

    //set titleLabel
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = title;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(font);
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    LvSetCtrlObj(titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (msg != NULL)
    {
        msgLabel = lv_label_create(body);
        //set msgLabel
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_ucBidiFlag = TRUE;
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = msg;
        stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
        stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_SCROLL_CIRCULAR;
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TEXT_INPUT_IMFORMATION);
        stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
        LvSetCtrlObj(msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        //to adapt the different font size of the library.
        lv_obj_update_layout(msgLabel);
        stCtrlObjAttr.m_usHeight = lv_obj_get_height(msgLabel) + lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_TEXT_INPUT_IMFORMATION)) / 10;
        LvSetCtrlObj(msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    return body;
}

lv_obj_t* LvUiCreateInputTextArea(lv_obj_t *body, char* text, int font, int mode, int min, int max)
{
    lv_obj_t* textArea = NULL;
    unsigned short w, h, f, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    Lib_LcdParaGet(&w, &h, &f, &forecolor, &backcolor);

    lv_obj_update_layout(body);
    parentWidth = lv_obj_get_width(body);

    textArea = lv_textarea_create(body);

    //set textArea
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = text;
    if (mode == LVGL_INPUT_MODE_PWD)
    {
        stCtrlObjAttr.m_stTextAreaAttr.m_bPasswordMode = true;
    }
    if (mode == LVGL_INPUT_MODE_NUM)
    {
        stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars = "0123456789";
    }
    if (mode == LVGL_INPUT_MODE_IP || mode == LVGL_INPUT_MODE_AMOUNT)
    {
        stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars = "0123456789.";
    }
    stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength = max;
    stCtrlObjAttr.m_stTextAreaAttr.m_bOneLineMode = true;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(font)) + 20;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(font);
    stCtrlObjAttr.m_usRadius = stCtrlObjAttr.m_usHeight / 6;
    stCtrlObjAttr.m_stBorderAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 2;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_stPadAttr.m_usTop = 10;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = 5;
    LvSetCtrlObj(textArea, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_LEFT;
    stCtrlObjAttr.m_ulAnimTime = 500;
    LvSetCtrlObj(textArea, &stCtrlObjAttr, LV_PART_CURSOR | LV_STATE_FOCUSED);

    lv_obj_add_state(textArea, LV_STATE_FOCUSED);
    lv_obj_add_flag(textArea, LV_OBJ_FLAG_EVENT_BUBBLE);

    return textArea;
}

lv_obj_t* LvUiCreateInputIPTextArea(lv_obj_t *body, int font)
{
    lv_group_t *group = NULL;
    lv_obj_t *textAreaContainer = NULL;
    lv_obj_t *textArea[4];
    lv_obj_t *label = NULL;
    unsigned short w, h, f, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    Lib_LcdParaGet(&w, &h, &f, &forecolor, &backcolor);

    lv_obj_update_layout(body);
    parentWidth = lv_obj_get_width(body);

    group = lv_group_create();

    textAreaContainer = lv_obj_create(body);
    //set textAreaContainer
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
    stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(font)) + 20;
    stCtrlObjAttr.m_stBorderAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 2;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_stPadAttr.m_usBottom = 10;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = 10;
    stCtrlObjAttr.m_stPadAttr.m_usRight = 10;
    stCtrlObjAttr.m_stPadAttr.m_usTop = 10;
    stCtrlObjAttr.m_usRadius = stCtrlObjAttr.m_usHeight / 6;
    LvSetCtrlObj(textAreaContainer, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(textAreaContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(textAreaContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(textAreaContainer, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < 4; i++)
    {
        textArea[i] = lv_textarea_create(textAreaContainer);
        //set textArea
        lv_obj_set_flex_grow(textArea[i], 1);
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(font));
        stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength = 3;
        stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars = "0123456789";
        stCtrlObjAttr.m_stTextAreaAttr.m_bOneLineMode = true;
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
        stCtrlObjAttr.m_stTextAttr.m_pcTextContent = "0";
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(font);
        LvSetCtrlObj(textArea[i], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
        stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
        stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_LEFT;
        stCtrlObjAttr.m_ulAnimTime = 500;
        LvSetCtrlObj(textArea[i], &stCtrlObjAttr, LV_PART_CURSOR | LV_STATE_FOCUSED);

        lv_group_add_obj(group, textArea[i]);

        if (i != 3)
        {
            label = lv_label_create(textAreaContainer);
            //set label
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_stTextAttr.m_pcTextContent = ".";
            stCtrlObjAttr.m_usWidth = parentWidth / 16;
            stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(24);
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
            LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    lv_obj_add_flag(textAreaContainer, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(textArea[0], LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(textArea[1], LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(textArea[2], LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(textArea[3], LV_OBJ_FLAG_EVENT_BUBBLE);


    return textAreaContainer;
}


lv_obj_t* LvUiGetPageBody()
{
    return gPageBodyObj;
}

static void LvUiDialogTask(struct _lv_timer_t* task)
{
    char acTimeStr[8] = {0};
    t_lvgl_dialog* dialog = ( t_lvgl_dialog*) (task->user_data);

    if (IS_TIME_OUT ( dialog->tickTime, 1000))
    {
        dialog->tickTime = Lib_SysGetTimer();//Sys_TimerOpen ( ( unsigned int ) 1000 );
        if ( dialog->isCountDown )
        {
            if ( dialog->curTime == 0 )
            {
                return;
            }
            dialog->curTime--;
        }
        else
        {
            if ( dialog->curTime == dialog->timeout )
            {
                return;
            }
            dialog->curTime++;
        }
        sprintf ( acTimeStr, "%d", dialog->curTime );
        strcat ( acTimeStr, " s");
        lv_label_set_text ( dialog->timeLabel, acTimeStr );
    }
}

int LvUiDialog(LVGL_DIALOG *pDialog, void **handle)
{
    char acTimeStr[8] = {0};
    t_lvgl_dialog* dialog = NULL;
    lv_obj_t* img = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;
    unsigned short w, h, font, forecolor, backcolor;

    parentWidth = lv_obj_get_width(lv_scr_act());
    parentHeight = lv_obj_get_height(lv_scr_act());

    dialog = (t_lvgl_dialog *)Lib_malloc(sizeof(t_lvgl_dialog));
    if (!dialog)
    {
        return LVGL_OPER_FAIL;
    }
    dialog->range = 0;
    dialog->curTime = 1;
    dialog->timeout = pDialog->timeout;
    dialog->isCountDown = pDialog->isCountDown;
    dialog->tickTime = Lib_SysGetTimer(); // Sys_TimerOpen ( ( unsigned int ) 1000 );

    //LvUilCls();
    Lib_LvglLock(1);

    dialog->dialog = lv_obj_create(lv_scr_act());
    if (!dialog->dialog)
    {
        Lib_free((t_lvgl_dialog *) dialog);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    lv_obj_remove_style_all(dialog->dialog);

    stCtrlObjAttr.m_usWidth = parentWidth;
    if (pDialog->gif != NULL)
    {
        stCtrlObjAttr.m_usHeight = parentHeight - BAR_HEIGTH - LINE_HEIGTH;
        stCtrlObjAttr.m_y = BAR_HEIGTH + LINE_HEIGTH;
    }
    else
    {
        stCtrlObjAttr.m_usHeight = parentHeight - BAR_HEIGTH - LINE_HEIGTH * 2;
        stCtrlObjAttr.m_y = BAR_HEIGTH + LINE_HEIGTH;
    }
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_stBgAttr.m_color = PAGE_BG_COLOR;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    LvSetCtrlObj(dialog->dialog, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(dialog->dialog, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_add_event_cb(dialog->dialog, objCallbackForRefreshSleepTime, LV_EVENT_CLICKED, NULL);

    if (pDialog->gif != NULL)
    {
        // picture
        dialog->image = lv_gif_create(dialog->dialog);
        if (!dialog->image)
        {
            lv_obj_del(dialog->dialog);
            Lib_free((t_lvgl_dialog *)dialog);
            Lib_LvglLock(0);
            return LVGL_OPER_FAIL;
        }

        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_stImgAttr.m_pcGifSrc = pDialog->gif;
        stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
        stCtrlObjAttr.m_y = 0;
        LvSetCtrlObj(dialog->image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    // title
    dialog->titleLabel = lv_label_create(dialog->dialog);
    if (!dialog->titleLabel)
    {
        lv_obj_del (dialog->dialog);
        Lib_free((t_lvgl_dialog *) dialog);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDialog->title;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(pDialog->font);
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 25;
    LvSetCtrlObj(dialog->titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    // msg
    dialog->msgLabel = lv_label_create(dialog->dialog);
    if (!dialog->msgLabel)
    {
        lv_obj_del(dialog->dialog);
        Lib_free ((t_lvgl_dialog *)dialog);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDialog->msg;
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(pDialog->font);
    if (pDialog->gif != NULL)
    {
        stCtrlObjAttr.m_pAlignObj = dialog->image;
        stCtrlObjAttr.m_align = LV_ALIGN_OUT_BOTTOM_MID;
    }
    else
    {
        stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    }
    stCtrlObjAttr.m_y = 0;
    LvSetCtrlObj(dialog->msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    img = lv_imgbtn_create(dialog->dialog);
    if (!img)
    {
        lv_obj_del(dialog->dialog);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    lv_img_header_t header;
    lv_img_decoder_get_info(COUNT_TIME_IMG, &header);
    memset((char *)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_usWidth = header.w;
    stCtrlObjAttr.m_usHeight = header.h;
    stCtrlObjAttr.m_stImgAttr.m_imageBtnstate = LV_IMGBTN_STATE_RELEASED;
    stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc = COUNT_TIME_IMG;
    stCtrlObjAttr.m_pAlignObj = dialog->msgLabel;
    stCtrlObjAttr.m_align = LV_ALIGN_OUT_BOTTOM_MID;
    stCtrlObjAttr.m_y = 0;
    LvSetCtrlObj(img, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    // time
    dialog->timeLabel = lv_label_create(dialog->dialog);
    if (!dialog->timeLabel)
    {
        lv_obj_del(dialog->dialog);
        Lib_free((t_lvgl_dialog *)dialog);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    if (pDialog->isCountDown)
    {
        dialog->curTime = pDialog->timeout;
    }
    sprintf(acTimeStr, "%d", dialog->curTime);
    strcat(acTimeStr, " s");
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_usWidth = header.w;
    stCtrlObjAttr.m_usHeight = header.h;
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = acTimeStr;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TIMER_TIME);
    stCtrlObjAttr.m_pAlignObj = img;
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    LvSetCtrlObj(dialog->timeLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (gDialogTask)
    {
        lv_timer_del(gDialogTask);
        gDialogTask = NULL;
    }
    gDialogTask = lv_timer_create(LvUiDialogTask, 50, dialog);

    Lib_LvglLock(0);

    *handle = dialog;
    return LVGL_OPER_SUCC;
}

int LvUiRefreshDialog(void *handle, LVGL_DIALOG *pDialog)
{
    char acTimeStr[8] = {0};
    t_lvgl_dialog *dialog = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentHeight;

    Lib_LvglLock(1);

    if (gDialogTask)
    {
        lv_timer_del(gDialogTask);
        gDialogTask = NULL;
    }

    dialog = (t_lvgl_dialog *)handle;
    if (!dialog)
    {
        return LVGL_OPER_FAIL;
    }
    dialog->range = 0;
    dialog->curTime = 1;
    dialog->timeout = pDialog->timeout;
    dialog->isCountDown = pDialog->isCountDown;

    parentHeight = lv_obj_get_height(dialog->dialog);

    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDialog->title;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 25;
    LvSetCtrlObj(dialog->titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pDialog->msg;

    if (pDialog->gif != NULL)
    {
        stCtrlObjAttr.m_pAlignObj = dialog->image;
        stCtrlObjAttr.m_align = LV_ALIGN_OUT_BOTTOM_MID;
    }
    else
    {
        stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    }

    stCtrlObjAttr.m_y = 0;
    LvSetCtrlObj(dialog->msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (pDialog->isCountDown)
    {
        dialog->curTime = pDialog->timeout;
    }
    dialog->tickTime = Lib_SysGetTimer();
    sprintf(acTimeStr, "%d", dialog->curTime);
    strcat(acTimeStr, " s");

    lv_label_set_text(dialog->timeLabel, acTimeStr);

    gDialogTask = lv_timer_create (LvUiDialogTask, 50, dialog);

    Lib_LvglLock(0);

    return LVGL_OPER_SUCC;
}

int LvUiCloseDialog(void *handle)
{
    if (NULL == handle)
    {
        return LVGL_OPER_FAIL;
    }
    t_lvgl_dialog *dialog = handle;

    Lib_LvglLock(1);

    if (gDialogTask)
    {
        lv_timer_del(gDialogTask);
        gDialogTask = NULL;
    }

    if (dialog->dialog)
    {
        lv_obj_del(dialog->dialog);
    }
    Lib_free(dialog);
    Lib_LvglLock(0);

    return LVGL_OPER_SUCC;
}


static void LvUiRollerEventHandler (lv_event_t *event )
{
    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        BeepOn();
    }
}


int LvUiDate(char *Date, char *title, int iTimeout)
{
    lv_obj_t* year_roller = NULL;
    lv_obj_t* month_roller = NULL;
    lv_obj_t* day_roller = NULL;
    lv_obj_t* hour_roller = NULL;
    lv_obj_t* min_roller = NULL;
    lv_obj_t* sec_roller = NULL;
    char aucYear[5] = { 0 };
    char aucMonth[3] = { 0 };
    char aucDay[3] = { 0 };
    char aucHour[3] = { 0 };
    char aucMin[3] = { 0 };
    char aucSec[3] = { 0 };

    int ret;
    lv_obj_t* p_page_menu_1_obj = NULL;
    const char sys_year[] = "2019\n2020\n2021\n2022\n2023\n2024\n2025\n2026\n2027\n2028\n2029\n2030";
    const char sys_month[] = "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12";
    const char sys_day[] = "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31";
    const char sys_hour[] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
    const char sys_min[] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    const char sys_sec[] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    lv_obj_t* titleLabel = NULL;
    lv_obj_t* tipLabel = NULL;
    unsigned short sel_opt;
    char *offset;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttrMain = {0};
    CTRL_OBJ_ATTR stCtrlObjAttrSelected = {0};
    int parentWidth, parentHeight;

    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    sscanf(Date, "%04s%02s%02s%02s%02s%02s", aucYear, aucMonth, aucDay, aucHour, aucMin, aucSec);
    TRACE("Year:%s Month:%s Day:%s %s:%s:%s\r\n", aucYear, aucMonth, aucDay, aucHour, aucMin, aucSec);
    TRACE("parent Width : %d, Height : %d", parentWidth, parentHeight);

    Lib_LvglLock(1);
    p_page_menu_1_obj = lv_obj_create(gPageBodyObj);
    lv_obj_remove_style_all(p_page_menu_1_obj);

    stCtrlObjAttrMain.m_usWidth = lv_obj_get_width(gPageBodyObj);
    stCtrlObjAttrMain.m_usHeight = lv_obj_get_height(gPageBodyObj);
    stCtrlObjAttrMain.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttrMain.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttrMain.m_stBgAttr.m_ucOpa = LV_OPA_100;
    LvSetCtrlObj(p_page_menu_1_obj, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(p_page_menu_1_obj, LV_OBJ_FLAG_EVENT_BUBBLE);

    titleLabel = lv_label_create (p_page_menu_1_obj);
    if (!titleLabel)
    {
        lv_obj_del(p_page_menu_1_obj);
        Lib_LvglLock(0);
        return RET_OK;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    //set titleLabel
    memset((char*)&stCtrlObjAttrMain, 0x00, sizeof(stCtrlObjAttrMain));
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = title;
    stCtrlObjAttrMain.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_DATETIME_TITLE);
    stCtrlObjAttrMain.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttrMain.m_align = LV_ALIGN_TOP_MID;
    LvSetCtrlObj(titleLabel, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set yearRoller
    year_roller = lv_roller_create(p_page_menu_1_obj);
    memset((char*)&stCtrlObjAttrMain, 0x00, sizeof(stCtrlObjAttrMain));
    stCtrlObjAttrMain.m_stRollerAttr.m_rollerMode = LV_ROLLER_MODE_NORMAL;
    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_year;
    stCtrlObjAttrMain.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttrMain.m_x = parentWidth / 32;
    stCtrlObjAttrMain.m_y = 73;
    stCtrlObjAttrMain.m_usWidth = 50;
    stCtrlObjAttrMain.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttrMain.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttrMain.m_stTextAttr.m_usTextLineSpace = 15;
    stCtrlObjAttrMain.m_stRollerAttr.m_cVisibleRowCount = 3;
    stCtrlObjAttrMain.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_DATETIME);
    offset = strstr(sys_year, aucYear);
    sel_opt = (offset - sys_year) / 5;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_stRollerAttr.m_isAnim = LV_ANIM_ON;
    LvSetCtrlObj(year_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    stCtrlObjAttrSelected.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttrSelected.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttrSelected.m_stBorderAttr.m_usWidth = 2;
    stCtrlObjAttrSelected.m_stBorderAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttrSelected.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT;
    LvSetCtrlObj(year_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(year_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set month_roller
    month_roller = lv_roller_create(p_page_menu_1_obj);
    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_month;
    offset = strstr(sys_month, aucMonth);
    sel_opt = (offset - sys_month) / 3;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_x = 60;
    LvSetCtrlObj(month_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);
    LvSetCtrlObj(month_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_event_cb (month_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set day_roller
    day_roller = lv_roller_create(p_page_menu_1_obj);
    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_day;
    offset = strstr(sys_day, aucDay);
    sel_opt = (offset - sys_day) / 3;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_x = 110;
    LvSetCtrlObj(day_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);
    LvSetCtrlObj(day_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(day_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set hour_roller
    hour_roller = lv_roller_create(p_page_menu_1_obj);
    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_hour;
    offset = strstr(sys_hour, aucHour);
    sel_opt = (offset - sys_hour) / 3;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_x = 160;
    LvSetCtrlObj(hour_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);
    LvSetCtrlObj(hour_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(hour_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set min_roller
    min_roller = lv_roller_create(p_page_menu_1_obj);
    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_min;
    offset = strstr(sys_min, aucMin);
    sel_opt = (offset - sys_min) / 3;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_x = 210;
    LvSetCtrlObj(min_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);
    LvSetCtrlObj(min_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(min_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    //set sec_roller
    sec_roller = lv_roller_create ( p_page_menu_1_obj );

    stCtrlObjAttrMain.m_stRollerAttr.m_pcRollerOptions = (char *)sys_sec;
    offset = strstr(sys_sec, aucSec);
    sel_opt = (offset - sys_sec) / 3;
    stCtrlObjAttrMain.m_stRollerAttr.m_usSelOpt = sel_opt;
    stCtrlObjAttrMain.m_x = 260;
    LvSetCtrlObj(sec_roller, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);
    stCtrlObjAttrSelected.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    LvSetCtrlObj(sec_roller, &stCtrlObjAttrSelected, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(sec_roller, LvUiRollerEventHandler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t* title_label = lv_label_create ( p_page_menu_1_obj );
    memset((char*)&stCtrlObjAttrMain, 0x00, sizeof(stCtrlObjAttrMain));
    stCtrlObjAttrMain.m_pAlignObj = year_roller;
    stCtrlObjAttrMain.m_align = LV_ALIGN_OUT_TOP_MID;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "YYYY";
    stCtrlObjAttrMain.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_DATETIME);
    stCtrlObjAttrMain.m_stTextAttr.m_color = (lv_color_t)forecolor;
    LvSetCtrlObj(title_label, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* title_label1 = lv_label_create ( p_page_menu_1_obj );
    stCtrlObjAttrMain.m_pAlignObj = month_roller;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "MM";
    LvSetCtrlObj(title_label1, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* title_label2 = lv_label_create ( p_page_menu_1_obj );
    stCtrlObjAttrMain.m_pAlignObj = day_roller;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "DD";
    LvSetCtrlObj(title_label2, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* title_hour = lv_label_create ( p_page_menu_1_obj );
    stCtrlObjAttrMain.m_pAlignObj = hour_roller;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "HH";
    LvSetCtrlObj(title_hour, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* title_minute = lv_label_create ( p_page_menu_1_obj );
    stCtrlObjAttrMain.m_pAlignObj = min_roller;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "min";
    LvSetCtrlObj(title_minute, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* title_second = lv_label_create ( p_page_menu_1_obj );
    stCtrlObjAttrMain.m_pAlignObj = sec_roller;
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = "sec";
    LvSetCtrlObj(title_second, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(year_roller, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(month_roller, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(day_roller, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(hour_roller, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(min_roller, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(sec_roller, LV_OBJ_FLAG_EVENT_BUBBLE);

    tipLabel = lv_label_create(p_page_menu_1_obj);
    if (!tipLabel)
    {
        lv_obj_del(p_page_menu_1_obj);
        Lib_LvglLock(0);
        return RET_ERR;
    }
    memset((char*)&stCtrlObjAttrMain, 0x00, sizeof(stCtrlObjAttrMain));
    stCtrlObjAttrMain.m_stTextAttr.m_pcTextContent = TIP_TOUCH_TO_SLIDE_ENTER_TO_CONTINUE;
    stCtrlObjAttrMain.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TIP);
    stCtrlObjAttrMain.m_stTextAttr.m_color = LV_COLOR_GRAY;
    stCtrlObjAttrMain.m_align = LV_ALIGN_BOTTOM_MID;
    LvSetCtrlObj(tipLabel, &stCtrlObjAttrMain, LV_PART_MAIN | LV_STATE_DEFAULT);

    Lib_LvglLock(0);

    int key = 0;

    while ( 1 )
    {
        key = LvUiWaitKey(iTimeout);
        if (key == KB_CANCEL)
        {
            ret = RET_ERR;
            break;
        }
        else if (key == KB_ENTER)
        {
            unsigned short year;
            unsigned char month;
            unsigned char day;

            lv_roller_get_selected_str(year_roller, aucYear, sizeof(aucYear));
            lv_roller_get_selected_str(month_roller, aucMonth, sizeof(aucMonth));
            lv_roller_get_selected_str(day_roller, aucDay, sizeof(aucDay));
            lv_roller_get_selected_str(hour_roller, aucHour, sizeof(aucHour));
            lv_roller_get_selected_str(min_roller, aucMin, sizeof(aucMin));
            lv_roller_get_selected_str(sec_roller, aucSec, sizeof(aucSec));

            TRACE ( "Year:%s Month:%s Day:%s %s:%s:%s\r\n",
                aucYear, aucMonth, aucDay, aucHour, aucMin, aucSec);

            // validity check
            year = (aucYear[0]-'0')*1000+(aucYear[1]-'0')*100+(aucYear[2]-'0')*10 + (aucYear[3]-'0');
            month = (aucMonth[0]-'0')*10 + (aucMonth[1]-'0');
            day = (aucDay[0]-'0')*10 + (aucDay[1]-'0');

            if (TRUE != DateIsLegal(year, month, day))
            {
                MessageBox(TIP_DATA_ERR, MB_CENTER|MB_COLOR_RED, 1000);
                continue;
            }

            sprintf(Date, "%4s%2s%2s%2s%2s%2s",
                aucYear, aucMonth, aucDay, aucHour, aucMin, aucSec);

            MessageBox(TIP_SET_SUCC, MB_CENTER|MB_COLOR_GREEN, 1000);
            ret = RET_OK;
            break;
        }
        else if (key == LVGL_OPER_TIMEOUT)
        {
            ret = RET_ERR_TIMEOUT;
            break;
        }
    }

    Lib_LvglLock(1);
    lv_obj_del ( p_page_menu_1_obj );
    Lib_LvglLock(0);
    FUN_IN;
    return ret;
}

int LvUiInputAmt(char* str_amt, int max, int timeout)
{
    int ret = 0;
    int parentWidth, parentHeight;
    lv_obj_t* body = NULL;
    lv_obj_t* msgLabel = NULL;
    lv_obj_t* textArea = NULL;
    lv_obj_t* titleLabel = NULL;
    lv_obj_t* image = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    FUN_IN;

    Lib_LvglLock(1);

    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    TRACE("%d, %d", parentWidth, parentHeight);

    body = lv_obj_create(gPageBodyObj);
    if (!body)
    {
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_COVER;
    LvSetCtrlObj(body, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    image = lv_img_create(body);
    if (!image)
    {
        FUN_OUT;
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = ENTERAMT_ICON;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_update_layout(image);
    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    titleLabel = lv_label_create(image);
    if (!titleLabel)
    {
        lv_obj_del(body);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_AMOUNT;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_AMOUNT_TIP);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 20;
    LvSetCtrlObj(titleLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    textArea = lv_textarea_create(image);
    if (!textArea)
    {
        lv_obj_del(body);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars = "0123456789.,";
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_DEFAULT_TRASACTION_AMOUNT;
    //Decimal point
    max = max + 3;
    stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength = max;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 5;
    stCtrlObjAttr.m_usHeight = lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_TRANSACTION_AMOUNT)) + lv_font_get_line_height(Lib_LvglGetFont(28)) / 5;
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_AMOUNT);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_usRadius = stCtrlObjAttr.m_usHeight / 3;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_80;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 3;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_BOTTOM;
    LvSetCtrlObj(textArea, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_clear_flag(textArea, LV_OBJ_FLAG_SCROLLABLE);

    msgLabel = lv_label_create(textArea);
    if (!msgLabel)
    {
        lv_obj_del(body);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_USD;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_AMOUNT_TIP);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_align = LV_ALIGN_BOTTOM_LEFT;
    LvSetCtrlObj(msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    Lib_LvglLock(0);

    lv_obj_add_flag(body, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(msgLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(textArea, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(titleLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(image, LV_OBJ_FLAG_EVENT_BUBBLE);

    int key = 0, size = 0;
    char buf[32 + 1] = { 0 };
    long long amount = 0;
    unsigned int tick = 0;
    tick = Lib_SysGetTimer();
    while(1)
    {
        if((Lib_SysGetTimer() - tick) / 1000 >= (unsigned int)timeout)
        {
            ret = LVGL_OPER_TIMEOUT;
            break;
        }
        key = LvUiWaitKey(100);
        if ( key == KB_CANCEL )
        {
            ret = LVGL_OPER_CANCEL;
            break;
        }
        Lib_LvglLock(1);
        const char* text = lv_textarea_get_text(textArea);
        Lib_LvglLock(0);
        size = strlen(text);
        if (key >= KB_DIGIT_ZERO && key <= KB_DIGIT_NINE)
        {
            if (size < max)
            {
                amount = amount * 10 + (key - KB_DIGIT_ZERO);
                sprintf (buf, "%lld", amount);
                size = strlen(buf);

                sprintf(buf, "%lld.%02lld", amount / 100, amount % 100);

                AmtAddThdsPt(buf);

                TRACE("amount=%s", buf);
                Lib_LvglLock(1);
                lv_textarea_set_text(textArea, buf);
                Lib_LvglLock (0);
                tick = Lib_SysGetTimer();
            }
        }
        else if (key == KB_CLEAR)
        {
            if (size > 0)
            {
                if (text[size - 1] >= '0' && text[size - 1] <= '9')
                {
                    amount = amount / 10;
                    sprintf(buf, "%lld.%02lld", amount / 100, amount % 100);
                    AmtAddThdsPt(buf);
                    TRACE ("amount=%s",buf);
                    Lib_LvglLock(1);
                    lv_textarea_set_text(textArea, buf);
                    Lib_LvglLock(0);
                    tick = Lib_SysGetTimer();
                }
            }
        }
        else if (key == KB_ENTER)
        {
            if (amount > 0)
            {
                sprintf(buf, "%lld", amount);
                strcpy (str_amt, buf);
                ret = LVGL_OPER_SUCC;
                break;
            }
        }
    }

    Lib_LvglLock(1);
    lv_obj_del (body);
    Lib_LvglLock(0);

    return ret;
}

int LvUiShowResult(LVGL_RESULT *pResult)
{
    int ret = 0;
    lv_obj_t* img = NULL;
    lv_obj_t* page = NULL;
    lv_obj_t* flgLabel = NULL;
    lv_obj_t* msgLabel = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;

    Lib_LvglLock(1);

    page = lv_obj_create (gPageBodyObj);
    if (!page)
    {
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    LvSetCtrlObj(page, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(page, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    img = lv_imgbtn_create(page);
    if (!img)
    {
        lv_obj_del(page);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    flgLabel = lv_label_create(page);
    if (!flgLabel)
    {
        lv_obj_del(page);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    msgLabel = lv_label_create(page);
    if (!msgLabel)
    {
        lv_obj_del(page);
        Lib_LvglLock(0);
        return LVGL_OPER_FAIL;
    }

    stCtrlObjAttr.m_stImgAttr.m_imageBtnstate = LV_IMGBTN_STATE_RELEASED;
    stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc = pResult->img;
    lv_img_header_t header;
    lv_img_decoder_get_info(pResult->img, &header);
    stCtrlObjAttr.m_usWidth = header.w;
    stCtrlObjAttr.m_usHeight = header.h;
    LvSetCtrlObj(img, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pResult->title;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_RESULT);
    stCtrlObjAttr.m_stTextAttr.m_color = pResult->fontColor;
    LvSetCtrlObj(flgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_TRANSACTION_RESULT_IMFORMATION);
    stCtrlObjAttr.m_stTextAttr.m_color = LV_COLOR_GRAY;
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = pResult->msg;
    LvSetCtrlObj(msgLabel, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    Lib_LvglLock(0);

    lv_obj_add_flag(page, LV_OBJ_FLAG_EVENT_BUBBLE);


    while (1)
    {
        ret = LvUiWaitKey(pResult->timeout);
        if ( ret == KB_ENTER || ret == KB_CANCEL || ret == LVGL_OPER_TIMEOUT || ret == LVGL_OPER_CANCEL)
        {
            break;
        }
    }

    Lib_LvglLock(1);
    if (page)
    {
        lv_obj_del(page);
    }
    Lib_LvglLock(0);

    return ret;
}

void LvUiInitStyle(lv_style_t * style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void LvUiSwitchEventHandler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    int *userData = lv_event_get_user_data(e);

    switch (code)
    {
    case LV_EVENT_CLICKED:
        gKeyPressed = KB_ENTER;
        *userData = TRUE;
        BeepOn();
        break;
    default:
        break;
    }
}

int LvUiSwitch(char *title, int iTimeout, unsigned char ucParamId, char *imgOnSrc, char *imgOffSrc)
{
    int ret;
    int status;
    int touchFlag = FALSE;
    unsigned char flag;
    unsigned char flag_len = 1;
    unsigned short w, h, font, forecolor, backcolor;
    lv_obj_t * sw = NULL;
    lv_obj_t * imgOn = NULL;
    lv_obj_t * imgOff = NULL;
    lv_obj_t * label = NULL;
    lv_obj_t * container = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;
    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    TerminalParamGet(ucParamId, &flag, &flag_len);

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    LvUilCls();

    label = lv_label_create(gPageBodyObj);
    container = lv_obj_create(gPageBodyObj);
    sw = lv_switch_create(container);
    imgOn = lv_img_create(container);
    imgOff = lv_img_create(container);

    //set label style
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
    LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //set container style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    stCtrlObjAttr.m_y = parentHeight / 12;
    stCtrlObjAttr.m_usWidth = parentHeight / 12 * 7;
    stCtrlObjAttr.m_usHeight = parentHeight / 12 * 7;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 1;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_40;
    stCtrlObjAttr.m_stBorderAttr.m_color = LV_COLOR_BLACK;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_usRadius = stCtrlObjAttr.m_usWidth / 10;
    LvSetCtrlObj(container, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(container, LV_OBJ_FLAG_EVENT_BUBBLE);

    parentWidth = stCtrlObjAttr.m_usWidth;
    parentHeight = stCtrlObjAttr.m_usHeight;

    //set imgOn style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentWidth / 7;
    stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = imgOnSrc;
    LvSetCtrlObj(imgOn, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    //set imgOff style
    stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = imgOffSrc;
    LvSetCtrlObj(imgOff, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_update_layout(imgOn);

    //set sw style
    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_align = LV_ALIGN_BOTTOM_MID;
    stCtrlObjAttr.m_y = -parentWidth / 7;
    stCtrlObjAttr.m_usWidth = parentWidth / 7 * 2;
    stCtrlObjAttr.m_usHeight = parentWidth / 7;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_SILVER;
    stCtrlObjAttr.m_usRadius = parentWidth / 7;
    LvSetCtrlObj(sw, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_usRadius = parentWidth / 7;
    LvSetCtrlObj(sw, &stCtrlObjAttr, LV_PART_INDICATOR | LV_STATE_CHECKED);

    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    LvSetCtrlObj(sw, &stCtrlObjAttr, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_add_flag(sw, LV_OBJ_FLAG_EVENT_BUBBLE);

    if (flag == 0)
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        lv_obj_clear_flag(imgOff, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(imgOn, LV_OBJ_FLAG_HIDDEN);
    }
    else if (flag == 1)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
        lv_obj_clear_flag(imgOn, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(imgOff, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        // Illegal parameter, default enabled
        lv_obj_add_state(sw, LV_STATE_CHECKED);
        lv_obj_clear_flag(imgOn, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(imgOff, LV_OBJ_FLAG_HIDDEN);
        TerminalParamSet(ucParamId, &flag, flag_len);
    }

    lv_obj_add_event_cb(sw, LvUiSwitchEventHandler, LV_EVENT_ALL, &touchFlag);

    while (1)
    {
        ret = LvUiWaitKey(iTimeout);

        if (ret == KB_CANCEL || ret == LVGL_OPER_TIMEOUT)
        {
            return ret;
        }
        else if (ret == KB_ENTER)
        {
            status = lv_obj_has_state(sw, LV_STATE_CHECKED) ? 1 : 0;
            if ((status == 1 && touchFlag) || (status == 0 && !touchFlag))
            {
                //show switchOn
                lv_obj_add_state(sw, LV_STATE_CHECKED);
                lv_obj_clear_flag(imgOn, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(imgOff, LV_OBJ_FLAG_HIDDEN);
                flag = 1;
            }
            else
            {
                //show switchOff
                lv_obj_clear_state(sw, LV_STATE_CHECKED);
                lv_obj_clear_flag(imgOff, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(imgOn, LV_OBJ_FLAG_HIDDEN);
                flag = 0;
            }
            touchFlag = FALSE;

            TerminalParamSet(ucParamId, &flag, flag_len);
        }
        else
        {
            continue;
        }
    }

    return LVGL_OPER_SUCC;
}

void LvUilistButEventhandler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    int keyPressed = (int)lv_event_get_user_data(e);

    switch (code)
    {
    case LV_EVENT_CLICKED:
        if (keyPressed)
        {
            gKeyPressed = keyPressed;
        }
        BeepOn();
        break;
    default:
        break;
    }
}

void LvUiSliderEventHandler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * currentObj = lv_event_get_current_target(e);
    int value;

    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
        if (lv_slider_is_dragged(currentObj))
        {
            value = lv_slider_get_value(currentObj);
            gKeyPressed = value + '0';
            BeepOn();
        }
        break;
    default:
        break;
    }
}

bool LvUiStatusBarIsExist(void)
{
    return (0 == gHasStatusBar ? false : true);
}

void msgbox_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * mbox = lv_event_get_current_target(event);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        const char * active_btn_text = lv_msgbox_get_active_btn_text(mbox);

        if (strcmp(active_btn_text, "YES") == 0)
        {
            LV_LOG_USER("Ok button clicked.\n");
        }
        else if (strcmp(active_btn_text, "NO") == 0)
        {
            LV_LOG_USER("Cancel button clicked.\n");
            //lv_msgbox_close(mbox);
        }
    }
}

int LvUiMessageBox(char* pcDisp, unsigned int uiType, char *title, int iTimeDly)
{
    lv_obj_t * mbox = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;
    lv_obj_t *parent = gPageBodyObj;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    if (iTimeDly)
    {
        parent = NULL;
    }

    if (title != NULL)
    {
        mbox = lv_msgbox_create(parent, title, pcDisp, NULL, false);
    }
    else
    {
        mbox = lv_msgbox_create(parent, TIP_MESSAGEBOX, pcDisp, NULL, false);
    }

    parentWidth = lv_obj_get_width(gPageBodyObj);
    parentHeight = lv_obj_get_height(gPageBodyObj);

    lv_obj_add_event_cb(mbox, msgbox_event_handler, LV_EVENT_ALL, NULL);

    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 3;
    stCtrlObjAttr.m_usHeight = parentHeight - parentHeight / 3;
    stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_WHITE;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_usWidth = 6;
    stCtrlObjAttr.m_stBorderAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stBorderAttr.m_ucOpa = LV_OPA_100;
    stCtrlObjAttr.m_stBorderAttr.m_ucSide = LV_BORDER_SIDE_FULL;
    stCtrlObjAttr.m_usRadius = 10;
    LvSetCtrlObj(mbox, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    if ((uiType & MB_COLOR_RED) == MB_COLOR_RED)
    {
        stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_RED);
    }
    else if ((uiType & MB_COLOR_GREEN) == MB_COLOR_GREEN)
    {
        stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_GREEN);
    }
    else if ((uiType & MB_COLOR_BLUE) == MB_COLOR_BLUE)
    {
        stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_BLUE);
    }
    else if ((uiType & MB_COLOR_YELLOW) == MB_COLOR_YELLOW)
    {
        stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_YELLOW);
    }

    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_NORMAL);
    stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    LvSetCtrlObj(lv_msgbox_get_title(mbox), &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    stCtrlObjAttr.m_stTextAttr.m_usTextLineSpace = 2;
    LvSetCtrlObj(lv_msgbox_get_text(mbox), &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    //to adapt the different font size of the library.
    lv_obj_update_layout(lv_msgbox_get_text(mbox));
    stCtrlObjAttr.m_usHeight = lv_obj_get_height(lv_msgbox_get_text(mbox)) + lv_font_get_line_height(Lib_LvglGetFont(FONT_SIZE_NORMAL)) / 10;
    LvSetCtrlObj(lv_msgbox_get_text(mbox), &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (lv_msgbox_get_btns(mbox) != NULL)
    {
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
        stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)forecolor;
        stCtrlObjAttr.m_stBorderAttr.m_usWidth = 4;
        stCtrlObjAttr.m_usRadius = 4;
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_NORMAL);
        if ((uiType & MB_COLOR_RED) == MB_COLOR_RED)
        {
            stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_RED);
        }
        else if ((uiType & MB_COLOR_GREEN) == MB_COLOR_GREEN)
        {
            stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_GREEN);
        }
        else if ((uiType & MB_COLOR_BLUE) == MB_COLOR_BLUE)
        {
            stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_BLUE);
        }
        else if ((uiType & MB_COLOR_YELLOW) == MB_COLOR_YELLOW)
        {
            stCtrlObjAttr.m_stTextAttr.m_color = lv_palette_main(LV_PALETTE_YELLOW);
        }
        LvSetCtrlObj(lv_msgbox_get_btns(mbox), &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    int ret = LvUiWaitKey(iTimeDly);
    // 无倒计时需求的弹窗, 清屏清除gPageBodyObj时会删除所有对象
    if (iTimeDly)
    {
        lv_msgbox_close(mbox);
    }
    return ret;
}

void LvUiShowUiMenuPackY(char *pText, char **ppcMenuInfo, unsigned char ucTotalLines, unsigned char ucHighlightIndex, unsigned char isPicture)
{
    int i;
    char buf[100] = {0};
    lv_obj_t* page = NULL;
    lv_obj_t *item = NULL;
    lv_obj_t *image = NULL;
    lv_group_t* g = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    t_lvgl_image img = {0};
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth, parentHeight;

    if (ucTotalLines > 4)
    {
        return;
    }

    LvUilCls();

    // Show title bar
    if (pText != NULL)
    {
        img.bgImg = TITLE_BG_IMG;
        LvUiShowImage(0, 0, &img, -1);
        LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char *)pText, LV_COLOR_WHITE);
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();
    g = lv_group_create();

    parentWidth = lv_obj_get_width(page);
    parentHeight = lv_obj_get_height(page);

    //Write codes main_list_1
    lv_obj_t *menu_list = lv_list_create(page);

    stCtrlObjAttr.m_y = parentHeight / 8 * 2;
    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight / 8 * 6;
    stCtrlObjAttr.m_scrollbarMode = LV_SCROLLBAR_MODE_AUTO;
    LvSetCtrlObj(menu_list, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(menu_list, LV_OBJ_FLAG_EVENT_BUBBLE);

    for (i=0; i<ucTotalLines; i++)
    {
        memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
        if (!isPicture)
        {
            sprintf(buf, "%d.%s", i+1, ppcMenuInfo[i]);
            item = lv_list_add_btn(menu_list, NULL, buf);
        }
        else
        {
            item = lv_btn_create(menu_list);
            stCtrlObjAttr.m_usHeight = LV_SIZE_CONTENT;
            stCtrlObjAttr.m_usWidth = parentWidth;
        }

        stCtrlObjAttr.m_ucBidiFlag = TRUE;
        stCtrlObjAttr.m_stBaseDirAttr.m_bIsSet = TRUE;
        stCtrlObjAttr.m_stBaseDirAttr.m_baseDir = LV_BASE_DIR_RTL;
        stCtrlObjAttr.m_stPadAttr.m_usLeft = parentWidth / 20;
        stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_MENU_PACK_Y);
        stCtrlObjAttr.m_stBgAttr.m_color = LV_COLOR_SILVER;
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_0;
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
        LvSetCtrlObj(item, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
        LvSetCtrlObj(item, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_FOCUSED);

        lv_obj_add_flag(item, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_event_cb(item, LvUiBtnClickedCb, LV_EVENT_CLICKED, (void *)(i + 1));

        if (isPicture)
        {
            image = lv_img_create(item);
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_stImgAttr.m_pcImageSrc = ppcMenuInfo[i];
            stCtrlObjAttr.m_stImgAttr.m_imageRecolor = (lv_color_t)forecolor;
            stCtrlObjAttr.m_stImgAttr.m_ucimageRecolorOpa = LV_OPA_100;
            stCtrlObjAttr.m_align = LV_ALIGN_LEFT_MID;
            LvSetCtrlObj(image, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        lv_group_add_obj(g, item);
        if (i == ucHighlightIndex)
        {
            lv_obj_scroll_to_view(item, LV_ANIM_OFF);
            lv_group_focus_obj(item);
        }
    }
}

ST_SDK_CALLBACK stSdkCallback =
{
    UILcdDrawAreaLvgl,
    UILcdClrScreenLvgl,
    UIClrScreenLvgl,
    LcdDisSingleRecentLvgl,
    UIDrawLineLvgl,
    UIIconLvgl,
    UIClearAllScreenLvgl,
    UIGetKeyWithTaskHandlerLvgl,
    UIDisPlayTimerReadyLvgl,
    UIGetTextWidthLvgl,
    UIShowXCodeLvglProcess1,
    UIShowXCodeLvglProcess2,
    NULL,
    NULL,
};

void LvUiFunctionRegisterToSdk(void)
{
    Lib_SdkCallbackInit(&stSdkCallback);
}

// Secondary encapsulated lvgl UI application APIs
void LvSetCtrlObj(lv_obj_t *pstObj, CTRL_OBJ_ATTR *pstCtrlObjAttr, lv_style_selector_t selector)
{
    CTRL_OBJ_ATTR stCtrlObjAttr;
    lv_img_header_t header;
    //int ret = LV_RES_INV;

    memcpy((char*)&stCtrlObjAttr, pstCtrlObjAttr, sizeof(stCtrlObjAttr));

    //bidi process
    if (stCtrlObjAttr.m_ucBidiFlag == TRUE)
    {
        if (FARSI_ID == GetLanguageType())
        {
            if (stCtrlObjAttr.m_align == LV_ALIGN_TOP_LEFT)
            {
                stCtrlObjAttr.m_align = LV_ALIGN_TOP_RIGHT;
            }

            if (stCtrlObjAttr.m_align == LV_ALIGN_LEFT_MID)
            {
                stCtrlObjAttr.m_align = LV_ALIGN_RIGHT_MID;
            }

            stCtrlObjAttr.m_x = -stCtrlObjAttr.m_x;

            if (stCtrlObjAttr.m_stTextAttr.m_ucTextAlign == LV_TEXT_ALIGN_LEFT)
            {
                stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_RIGHT;
            }
            else if (stCtrlObjAttr.m_stTextAttr.m_ucTextAlign == LV_TEXT_ALIGN_RIGHT)
            {
                stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            }

            if (stCtrlObjAttr.m_stBaseDirAttr.m_bIsSet == TRUE)
            {
                lv_obj_set_style_base_dir(pstObj, stCtrlObjAttr.m_stBaseDirAttr.m_baseDir, selector);
            }
        }
    }

    //common
    if (stCtrlObjAttr.m_usWidth != 0)
    {
        lv_obj_set_width(pstObj, stCtrlObjAttr.m_usWidth);
    }
    if (stCtrlObjAttr.m_usHeight != 0)
    {
        lv_obj_set_height(pstObj, stCtrlObjAttr.m_usHeight);
    }
    if (stCtrlObjAttr.m_usRadius != 0)
    {
        lv_obj_set_style_radius(pstObj, stCtrlObjAttr.m_usRadius, selector);
    }
    if (stCtrlObjAttr.m_scrollbarMode != LV_SCROLLBAR_MODE_OFF)
    {
        lv_obj_set_scrollbar_mode(pstObj, stCtrlObjAttr.m_scrollbarMode);
    }
    if (stCtrlObjAttr.m_ulAnimTime != 0)
    {
        lv_obj_set_style_anim_time(pstObj, stCtrlObjAttr.m_ulAnimTime, selector);
    }

    //pad
    if (stCtrlObjAttr.m_stPadAttr.m_usTop != 0)
    {
        lv_obj_set_style_pad_top(pstObj, stCtrlObjAttr.m_stPadAttr.m_usTop, selector);
    }
    if (stCtrlObjAttr.m_stPadAttr.m_usLeft != 0)
    {
        lv_obj_set_style_pad_left(pstObj, stCtrlObjAttr.m_stPadAttr.m_usLeft, selector);
    }
    if (stCtrlObjAttr.m_stPadAttr.m_usRight != 0)
    {
        lv_obj_set_style_pad_right(pstObj, stCtrlObjAttr.m_stPadAttr.m_usRight, selector);
    }
    if (stCtrlObjAttr.m_stPadAttr.m_usBottom != 0)
    {
        lv_obj_set_style_pad_bottom(pstObj, stCtrlObjAttr.m_stPadAttr.m_usBottom, selector);
    }
    //border
    if (stCtrlObjAttr.m_stBorderAttr.m_usWidth != 0)
    {
        lv_obj_set_style_border_width(pstObj, stCtrlObjAttr.m_stBorderAttr.m_usWidth, selector);
        lv_obj_set_style_border_color(pstObj, stCtrlObjAttr.m_stBorderAttr.m_color, selector);
    }
    if (stCtrlObjAttr.m_stBorderAttr.m_ucOpa != LV_OPA_0)
    {
        lv_obj_set_style_border_opa(pstObj, stCtrlObjAttr.m_stBorderAttr.m_ucOpa, selector);
    }
    if (stCtrlObjAttr.m_stBorderAttr.m_ucSide != LV_BORDER_SIDE_NONE)
    {
        lv_obj_set_style_border_side(pstObj, stCtrlObjAttr.m_stBorderAttr.m_ucSide, selector);
    }
    //text
    if (stCtrlObjAttr.m_stTextAttr.m_pcTextContent != NULL)
    {
        if (lv_obj_check_type(pstObj, &lv_label_class) == true)
        {
            lv_label_set_text(pstObj, stCtrlObjAttr.m_stTextAttr.m_pcTextContent);
        }
    }
    if (stCtrlObjAttr.m_stTextAttr.m_pvTextFont != NULL)
    {
        lv_obj_set_style_text_font(pstObj, stCtrlObjAttr.m_stTextAttr.m_pvTextFont, selector);
        lv_obj_set_style_text_align(pstObj, stCtrlObjAttr.m_stTextAttr.m_ucTextAlign, selector);
    }
    if (stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace != 0)
    {
        lv_obj_set_style_text_letter_space(pstObj, stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace, selector);
    }
    if (stCtrlObjAttr.m_stTextAttr.m_color.full != 0)
    {
        lv_obj_set_style_text_color(pstObj, stCtrlObjAttr.m_stTextAttr.m_color, selector);
    }
    if (lv_obj_check_type(pstObj, &lv_label_class) == true)
    {
        lv_label_set_long_mode(pstObj, stCtrlObjAttr.m_stTextAttr.m_textLongMode);
    }
    if (stCtrlObjAttr.m_stTextAttr.m_usTextLineSpace != 0)
    {
        lv_obj_set_style_text_line_space(pstObj, stCtrlObjAttr.m_stTextAttr.m_usTextLineSpace, selector);
    }
    //bg
    if (stCtrlObjAttr.m_stBgAttr.m_ucOpa != LV_OPA_0)
    {
        lv_obj_set_style_bg_opa(pstObj, stCtrlObjAttr.m_stBgAttr.m_ucOpa, selector);
        lv_obj_set_style_bg_color(pstObj, stCtrlObjAttr.m_stBgAttr.m_color, selector);
    }
    //img
    if (lv_obj_check_type(pstObj, &lv_img_class) == true)
    {
        if (stCtrlObjAttr.m_stImgAttr.m_pcImageSrc != NULL)
        {
            lv_img_set_src(pstObj, stCtrlObjAttr.m_stImgAttr.m_pcImageSrc);
            if (stCtrlObjAttr.m_usWidth == 0 || stCtrlObjAttr.m_usHeight == 0)
            {
                if (lv_img_decoder_get_info(stCtrlObjAttr.m_stImgAttr.m_pcImageSrc, &header) != LV_RES_OK)
                {
                    lv_obj_set_width(pstObj, SHOW_ERROR_NO_DATA_DEFAULT_W);
                    lv_obj_set_height(pstObj, SHOW_ERROR_NO_DATA_DEFAULT_H);
                }
                else
                {
                    lv_obj_set_width(pstObj, header.w);
                    lv_obj_set_height(pstObj, header.h);
                }
            }
        }
        else
        {
            lv_obj_set_width(pstObj, 0);
            lv_obj_set_height(pstObj, 0);
        }
    }


    if (lv_obj_check_type(pstObj, &lv_imgbtn_class) == true)
    {
        if (stCtrlObjAttr.m_stImgAttr.m_pcImageBtnLeftSrc != NULL ||
            stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc != NULL ||
            stCtrlObjAttr.m_stImgAttr.m_pcImageBtnRightSrc != NULL)
        {
            lv_imgbtn_set_src(pstObj, stCtrlObjAttr.m_stImgAttr.m_imageBtnstate, stCtrlObjAttr.m_stImgAttr.m_pcImageBtnLeftSrc,
                                                                                    stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc,
                                                                                    stCtrlObjAttr.m_stImgAttr.m_pcImageBtnRightSrc);
            if (stCtrlObjAttr.m_usWidth == 0 || stCtrlObjAttr.m_usHeight == 0)
            {
                if (lv_img_decoder_get_info(stCtrlObjAttr.m_stImgAttr.m_pcImageBtnMiddleSrc, &header) != LV_RES_OK)
                {
                    lv_obj_set_width(pstObj, SHOW_ERROR_NO_DATA_DEFAULT_W);
                    lv_obj_set_height(pstObj, SHOW_ERROR_NO_DATA_DEFAULT_H);
                }
                else
                {
                    lv_obj_set_width(pstObj, header.w);
                    lv_obj_set_height(pstObj, header.h);
                }
            }
        }
        else
        {
            lv_obj_set_width(pstObj, 0);
            lv_obj_set_height(pstObj, 0);
        }
    }

    if (stCtrlObjAttr.m_stImgAttr.m_pcGifSrc != NULL)
    {
        lv_gif_set_src(pstObj, stCtrlObjAttr.m_stImgAttr.m_pcGifSrc);
    }

    if (stCtrlObjAttr.m_stImgAttr.m_imageRecolor.full != 0)
    {
        lv_obj_set_style_img_recolor(pstObj, stCtrlObjAttr.m_stImgAttr.m_imageRecolor, selector);
        lv_obj_set_style_img_recolor_opa(pstObj, stCtrlObjAttr.m_stImgAttr.m_ucimageRecolorOpa, selector);
    }
    //textArea
    if (stCtrlObjAttr.m_stTextAttr.m_pcTextContent != NULL)
    {
        if (lv_obj_check_type(pstObj, &lv_textarea_class) == true)
        {
            lv_textarea_set_text(pstObj, stCtrlObjAttr.m_stTextAttr.m_pcTextContent);
            lv_textarea_set_one_line(pstObj, stCtrlObjAttr.m_stTextAreaAttr.m_bOneLineMode);
            lv_textarea_set_password_mode(pstObj, stCtrlObjAttr.m_stTextAreaAttr.m_bPasswordMode);
        }
    }
    if (stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength != 0)
    {
        lv_textarea_set_max_length(pstObj, stCtrlObjAttr.m_stTextAreaAttr.m_usMaxLength);
    }
    if (stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars != NULL)
    {
        lv_textarea_set_accepted_chars(pstObj, stCtrlObjAttr.m_stTextAreaAttr.m_pcAcceptChars);
    }
    //arc
    if (stCtrlObjAttr.m_stArcAttr.m_arcMode != LV_ARC_MODE_NORMAL)
    {
        lv_arc_set_mode(pstObj, stCtrlObjAttr.m_stArcAttr.m_arcMode);
    }
    if (stCtrlObjAttr.m_stArcAttr.m_usMaxValue != stCtrlObjAttr.m_stArcAttr.m_usMinValue)
    {
        lv_arc_set_range(pstObj, stCtrlObjAttr.m_stArcAttr.m_usMinValue, stCtrlObjAttr.m_stArcAttr.m_usMaxValue);
        lv_arc_set_value(pstObj, stCtrlObjAttr.m_stArcAttr.m_us_CurValue);
    }
    if (stCtrlObjAttr.m_stArcAttr.m_usAngleStartValue != stCtrlObjAttr.m_stArcAttr.m_usAngleEndValue)
    {
        lv_arc_set_bg_angles(pstObj, stCtrlObjAttr.m_stArcAttr.m_usAngleStartValue, stCtrlObjAttr.m_stArcAttr.m_usAngleEndValue);
    }
    if (stCtrlObjAttr.m_stArcAttr.m_bRoundedMode != false)
    {
        lv_obj_set_style_arc_rounded(pstObj, stCtrlObjAttr.m_stArcAttr.m_bRoundedMode, selector);
    }
    if (stCtrlObjAttr.m_stArcAttr.m_usWidth != 0)
    {
        lv_obj_set_style_arc_width(pstObj, stCtrlObjAttr.m_stArcAttr.m_usWidth, selector);
    }
    if (stCtrlObjAttr.m_stArcAttr.m_ucOpa != 0)
    {
        lv_obj_set_style_arc_opa(pstObj, stCtrlObjAttr.m_stArcAttr.m_ucOpa, selector);
        lv_obj_set_style_arc_color(pstObj, stCtrlObjAttr.m_stArcAttr.m_color, selector);
    }
    //slider
    if (lv_obj_check_type(pstObj, &lv_slider_class) == true)
    {
        lv_slider_set_mode(pstObj, stCtrlObjAttr.m_stSliderAttr.m_sliderMode);
    }
    if (stCtrlObjAttr.m_stSliderAttr.m_usMaxValue != stCtrlObjAttr.m_stSliderAttr.m_usMinValue)
    {
        lv_slider_set_range(pstObj, stCtrlObjAttr.m_stSliderAttr.m_usMinValue, stCtrlObjAttr.m_stSliderAttr.m_usMaxValue);
    }
    //roller
    if (stCtrlObjAttr.m_stRollerAttr.m_pcRollerOptions != NULL)
    {
        lv_roller_set_options(pstObj, stCtrlObjAttr.m_stRollerAttr.m_pcRollerOptions, stCtrlObjAttr.m_stRollerAttr.m_rollerMode);
        lv_roller_set_visible_row_count(pstObj, stCtrlObjAttr.m_stRollerAttr.m_cVisibleRowCount);
        lv_roller_set_selected(pstObj, stCtrlObjAttr.m_stRollerAttr.m_usSelOpt, stCtrlObjAttr.m_stRollerAttr.m_isAnim);
    }
    //line
    if (stCtrlObjAttr.m_stLineAttr.m_pLinePointArray != NULL && stCtrlObjAttr.m_stLineAttr.m_usPointNum != 0)
    {
        lv_line_set_points(pstObj, stCtrlObjAttr.m_stLineAttr.m_pLinePointArray, stCtrlObjAttr.m_stLineAttr.m_usPointNum);
    }
    if (stCtrlObjAttr.m_stLineAttr.m_usLineWidth != 0)
    {
        lv_obj_set_style_line_width(pstObj, stCtrlObjAttr.m_stLineAttr.m_usLineWidth, selector);
    }
    if (stCtrlObjAttr.m_stLineAttr.m_ucLineOpa != LV_OPA_0)
    {
        lv_obj_set_style_line_color(pstObj, stCtrlObjAttr.m_stLineAttr.m_lineColor, selector);
        lv_obj_set_style_line_opa(pstObj, stCtrlObjAttr.m_stLineAttr.m_ucLineOpa, selector);
    }
    //canvas
    if (stCtrlObjAttr.m_stCanvasAttr.m_pvCanvasBuf != NULL)
    {
        lv_canvas_set_buffer(pstObj, stCtrlObjAttr.m_stCanvasAttr.m_pvCanvasBuf,
                                    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufWidth,
                                    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufHeight,
                                    stCtrlObjAttr.m_stCanvasAttr.m_imgCf);
    }
    //label
    if (stCtrlObjAttr.m_stLabelAttr.m_bLabelIsRecolor != 0)
    {
        lv_label_set_recolor(pstObj, stCtrlObjAttr.m_stLabelAttr.m_bLabelIsRecolor);
    }
    //align
    if (stCtrlObjAttr.m_pAlignObj != NULL)
    {
        lv_obj_align_to(pstObj, stCtrlObjAttr.m_pAlignObj, stCtrlObjAttr.m_align, stCtrlObjAttr.m_x, stCtrlObjAttr.m_y);
    }
    else
    {
        if (stCtrlObjAttr.m_align != LV_ALIGN_DEFAULT)
        {
            lv_obj_set_align(pstObj, stCtrlObjAttr.m_align);
        }
        if (stCtrlObjAttr.m_x != 0)
        {
            lv_obj_set_x(pstObj, stCtrlObjAttr.m_x);
        }
        if (stCtrlObjAttr.m_y != 0)
        {
            lv_obj_set_y(pstObj, stCtrlObjAttr.m_y);
        }
    }
}

void LvSetStyle(lv_style_t* pstStyleObj, LV_STYLE_ATTR* pstStyleAttr)
{
    if (pstStyleAttr->m_usWidth != 0)
    {
        lv_style_set_width(pstStyleObj, pstStyleAttr->m_usWidth);
    }

    if (pstStyleAttr->m_usHeight != 0)
    {
        lv_style_set_height(pstStyleObj, pstStyleAttr->m_usHeight);
    }
    lv_style_set_align(pstStyleObj, pstStyleAttr->m_align);
    lv_style_set_x(pstStyleObj, pstStyleAttr->m_x);
    lv_style_set_y(pstStyleObj, pstStyleAttr->m_y);
    lv_style_set_radius(pstStyleObj, pstStyleAttr->m_usRadius);

    lv_style_set_pad_top(pstStyleObj, pstStyleAttr->m_stPadAttr.m_usTop);
    lv_style_set_pad_left(pstStyleObj, pstStyleAttr->m_stPadAttr.m_usLeft);
    lv_style_set_pad_right(pstStyleObj, pstStyleAttr->m_stPadAttr.m_usRight);
    lv_style_set_pad_bottom(pstStyleObj, pstStyleAttr->m_stPadAttr.m_usBottom);

    lv_style_set_border_width(pstStyleObj, pstStyleAttr->m_stBorderAttr.m_usWidth);
    lv_style_set_border_opa(pstStyleObj, pstStyleAttr->m_stBorderAttr.m_ucOpa);
    lv_style_set_border_color(pstStyleObj, pstStyleAttr->m_stBorderAttr.m_color);
    lv_style_set_border_side(pstStyleObj, pstStyleAttr->m_stBorderAttr.m_ucSide);

    if (pstStyleAttr->m_stTextAttr.m_pvTextFont != NULL)
    {
        lv_style_set_text_font(pstStyleObj, pstStyleAttr->m_stTextAttr.m_pvTextFont);
    }

    if (pstStyleAttr->m_stTextAttr.m_usTextLetterSpace != 0)
    {
        lv_style_set_text_letter_space(pstStyleObj, pstStyleAttr->m_stTextAttr.m_usTextLetterSpace);
    }

    if (pstStyleAttr->m_stTextAttr.m_ucTextAlign != 0)
    {
        lv_style_set_text_align(pstStyleObj, pstStyleAttr->m_stTextAttr.m_ucTextAlign);
    }

    lv_style_set_bg_opa(pstStyleObj, pstStyleAttr->m_stBgAttr.m_ucOpa);
    lv_style_set_bg_color(pstStyleObj, pstStyleAttr->m_stBgAttr.m_color);
}

