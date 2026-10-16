/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Power.c
 *  Version      :
 *  Author       :
 * Description   : Power management function
 **********************************************************************************************************/
#include "Language.h"
#include "Power.h"
#include "Common.h"
#include "LibApi.h"
#include "Ui.h"
#include "Drv.h"
#include "ParamManage.h"
#include "Input.h"
#include "Idle.h"
#include "Battery.h"
#include "Key.h"
#include "LanguageProcess.h"
#include "LanguageResource.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"
#include "MessageBox.h"

#define SLEEP_TIME 10000

#if defined(MQTT_SUPPORT)
bool MqttGetTaskStatus(void);
#endif

extern const unsigned char IconBigBattery0[];
extern const unsigned char IconBigBattery1[];
extern const unsigned char IconBigBattery2[];
extern const unsigned char IconBigBattery3[];
extern const char testLogoPic[];

static unsigned int s_ulPowerOnDeviceTime = 0, s_ulPowerOffDeviceTime = 0;
static unsigned int s_ulPowerOnDeviceTimeOut = 0, s_ulPowerOffDeviceTimeOut = 0;
static unsigned int ulSleepTime = SLEEP_TIME;
static unsigned char g_SysPowerOnFlag = FALSE;
static unsigned char g_CheckMenuOnFlag = FALSE;

// Shutdown charging flag
unsigned char g_ucPowerOffChargingFlag = 0;

// Device sleep enable switch
unsigned char s_ucSleepEnableF = TRUE;
//-----------------------------------------------------------------------------
// Name    : DeviceSleepEnDisableSet
// Func    : Set device sleep switch
// Paras   : UcF: TRUE enables sleep (default), FALSE prohibits sleep
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void DeviceSleepEnDisableSet(unsigned char ucF)
{
    s_ucSleepEnableF = ucF;

    if (TRUE == ucF)
    {
        DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);
    }
}


//-----------------------------------------------------------------------------
// Name    : DeviceSleepEnDisableGet
// Func    : Get device's sleep enable/disable mode params
// Paras   :
// Ret     : TRUE:enabled, FALSE:disabled
// Desc    :
//-----------------------------------------------------------------------------
unsigned char DeviceSleepEnDisableGet(void)
{
    return s_ucSleepEnableF;
}


//-----------------------------------------------------------------------------
// Name    : PowerOffOp
// Func    : Shutdown processing
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short PowerOffOp(int iTimeout)
{
    unsigned char ucKey;
    short sRC;
    static unsigned char ucCnt = 0;
    static unsigned long s_ulStartTime = 0;
    unsigned char s_timestatus = DispMainTimeGet();

    // In any case, press and hold the power key over 2 seconds will enter shutdown menu
    if (PowerKeyDown())
    {
        if (!s_ulStartTime)
        {
            s_ulStartTime = Lib_SysGetTimer();
        }

        if(!IS_TIME_OUT(s_ulStartTime, 2000))
        {
            return RET_OK;
        }

        s_ulStartTime = 0;
    }
    else
    {
        s_ulStartTime = 0;
        return RET_OK;
    }

    ucCnt++;
    if (ucCnt >= 2)
    {
        // shutdown
        Lib_LcdClrScreen();
        PowerOffDevice();
        return RET_OK;
    }

    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    lv_obj_t *BodyObj = NULL;
    lv_obj_t *Label1 = NULL;
    lv_obj_t *Label2 = NULL;
    int parentWidth, parentHeight;
    unsigned short w, h, font, forecolor, backcolor;

    Lib_LvglLock(1);
    BodyObj = lv_obj_create(lv_scr_act());
    if (!BodyObj)
    {
        Lib_LvglLock(0);
        return RET_OK;
    }

    lv_obj_remove_style_all(BodyObj);

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    parentWidth = lv_obj_get_width(lv_scr_act());
    parentHeight = lv_obj_get_height(lv_scr_act());

    stCtrlObjAttr.m_usWidth = parentWidth;
    stCtrlObjAttr.m_usHeight = parentHeight;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_LEFT;
    stCtrlObjAttr.m_x = 0;
    stCtrlObjAttr.m_y = 0;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_COVER;
    stCtrlObjAttr.m_stBgAttr.m_color = PAGE_BG_COLOR;
    stCtrlObjAttr.m_stBaseDirAttr.m_bIsSet = TRUE;
    stCtrlObjAttr.m_stBaseDirAttr.m_baseDir = LV_BASE_DIR_AUTO;
    LvSetCtrlObj(BodyObj, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_IS_SHUT_DOWN;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_SMALL);
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 8 * 3;
    Label1 = lv_label_create(BodyObj);
    LvSetCtrlObj(Label1, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_O_CONFIRM_X_CANCEL;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(FONT_SIZE_SMALL);
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_align = LV_ALIGN_TOP_MID;
    stCtrlObjAttr.m_y = parentHeight / 8 * 7;
    Label2 = lv_label_create(BodyObj);
    LvSetCtrlObj(Label2, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    while (1)
    {
        DispMainTimeSet(0);
        BeepOn();
        DeviceSleepEnDisableSet(FALSE);

        while (1)
        {
            sRC = KbWaitingKey(&ucKey, iTimeout);

            if (KB_RETURN_OK == sRC)
            {
                if (KB_ENTER == ucKey)
                {
                    // shutdown
                    Lib_LcdClrScreen();
                    PowerOffDevice();
                    return RET_OK;
                }
                else if (KB_CANCEL == ucKey)
                {
                    DeviceSleepEnDisableSet(TRUE);
                    ucCnt = 0;
                    // Cancel, return
                    DispMainTimeSet(s_timestatus);
                    lv_obj_del(BodyObj);
                    return UI_INPUT_CANCEL;
                }
            }
            else
            {
                DeviceSleepEnDisableSet(TRUE);
                ucCnt = 0;
                DispMainTimeSet(s_timestatus);
                lv_obj_del(BodyObj);
                return UI_INPUT_CANCEL;
            }
        }
    }
}

void PowerRefreshTimeOut(void)
{
    s_ulPowerOnDeviceTime  = Lib_SysGetTimer() + s_ulPowerOnDeviceTimeOut;
    s_ulPowerOffDeviceTime = Lib_SysGetTimer() + s_ulPowerOffDeviceTimeOut;
}

unsigned int DevicePowerOnTime(void)
{
    return s_ulPowerOnDeviceTime;
}


//-----------------------------------------------------------------------------
// Name    : DeviceSleepTimeReload
// Func    : Refresh Sleep Time
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void DeviceSleepTimeReload(unsigned long ulSleep)
{
    ulSleepTime = Lib_SysGetTimer() + ulSleep;
}

unsigned char PowerKeyDown(void)
{
    unsigned char key;
    key = Lib_GetPwrKeyStatus();
	return key;
}

// Check if the powerkey to turn on the device
unsigned char IsPowerKeyPowerOn(void)
{
    if(Lib_GetPowerOnCause() & POWERON_CAUSE_PWRKEY)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// Check if reset caused Reset
unsigned char IsResetPowerOn(void)
{
    if(Lib_GetPowerOnCause() & POWERON_CAUSE_RESET)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

unsigned char IsSysPowerOn(void)
{
    return g_SysPowerOnFlag;
}

void PowerOffDevice(void)
{
    Lib_ShutDown();
}


void PowerOnDevice(void)
{
    g_SysPowerOnFlag = TRUE;
    // Shutdown charging time not counted
    //ulRecordStartTimer = SysGetTimer();
}

unsigned char GetMenuOnFlag(void)
{
    return g_CheckMenuOnFlag;
}

unsigned char PowerOnDeviceDetect(void)
{
    TRACE("enter %s.\r\n", __func__);
    while (1)
    {

        if (PowerKeyDown())
        {
            if (Lib_SysGetTimer() > DevicePowerOnTime())
            {
                TRACE_LOG("device has been power on.\r\n");

                PowerOnDevice();

                Lib_Beep(50);
                while (PowerKeyDown())
                {
                    // power+enter Enter the self-test menu
                    if (Lib_KbGetKey() == KB_POWER_AND_ENTER)
                    {
                        g_CheckMenuOnFlag = TRUE;
                    }
                }
                return 1;
            }
            TRACE_LOG("Hold Power On...");
        }
        else
        {
            PowerRefreshTimeOut();
            return 0;
        }
    }

//    return 0;
}

//-----------------------------------------------------------------------------
// Name    : ChargerPresence
// Func    : Get the charger-in position status (usually not used for sub
//           applications), 1: in position, 0: not in position
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
unsigned char ChargerPresence(void)
{
    if(IsSysPowerOn() == FALSE)
    {
        return TRUE;
    }
    if(Lib_ExtPower())
    {
        return TRUE;
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
// Name    : PowerOnProtect
// Func    : PowerOn Protection
// Paras   :
// Ret     :
// Desc    : If battery level is below 20%, it is not allowed to start up,
//           and the gprs module is not working properly
//-----------------------------------------------------------------------------
short PowerOnProtect(void)
{
    // Display initialization image and prompt initialization
    lv_obj_t * img = NULL;
    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, BOOT_GIF);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    unsigned int ulStartTime = Lib_SysGetTimer();
    while (1)
    {
        Lib_WDGReloadCounter();
        BatteryTimerWork(FALSE);
        //LvUiWaitKey(1000);
        Lib_LvglTask();

        //UIText(2*LCD_FONT_SIZE, 4, DISP_MODE_NORMAL, "CURRENT POWER:%03d", gBatLastCapacity);

        if (IS_TIME_OUT(ulStartTime, 4000))
        {
            TRACE("gBatLastCapacity=%d %d %d\n", BatteryGetBatCapacity(), Lib_BatGetVoltage(), Lib_BatGetPowerOffVol());

            // Charging state, low battery threshold set to 5%, discharge state set to 20%
            if ((Lib_ExtPower()) && (Lib_BatGetVoltage() > Lib_BatGetPowerOffVol()))
            {
                lv_obj_del(img);
                return RET_OK;
            }
            else if ((!(Lib_ExtPower())) && (BatteryGetBatCapacity() > 10))
            {
                lv_obj_del(img);
                return RET_OK;
            }
            else
            {
                lv_obj_del(img);

                UITextAlign(ALIGN_M, 2, DISP_MODE_REVERSE, TIP_BAT_LOW_TO_CHG);
                UITextAlign(ALIGN_M, 3, DISP_MODE_REVERSE, TIP_BAT_LOW_SHUT_DOWN);

                Lib_DelayMs(2000);

                // Shutdown
                Lib_ShutDown();

                return RET_ERR;
            }
        }
    }
}


// Dynamic display of charging icon
void PowerOffChargingDisplay(void)
{
#if 0
//    unsigned char i = 1, ucY = 3, ucStartY = 10;
    unsigned int ulTimerOut = 0;
//	char capacity[100] = {0};

//	ucY = 3;
//	ucStartY = 26;

    // Reduce screen brightness
    Lib_LcdSetBrightLevel(1);
    // Clear screen
    Lib_LcdClrScreen();

    g_ucPowerOffChargingFlag = 1;

    while(1)
    {
        Lib_WDGReloadCounter();

		//BatteryTimerWork();

        // Power on detection
        if (PowerOnDeviceDetect())
        {
            Lib_LcdClrScreen();

			// Power on protection: if the battery is below 10%, it is not allowed to startup,
			// and gprs module is not working properly
            if (RET_OK == PowerOnProtect())
			{
				break;
			}
        }

        if(FALSE == Lib_ExtPower())
        {
            Lib_ShutDown();
        }

        if (Lib_SysGetTimer() >= ulTimerOut)
        {
            if (!Lib_BatGetChargeState())
            {
                //PowerCtrlLedChargeSplash(LED_SPLASH_SLOW);
                // if battery is fully charged, the full charge image will be displayed
//                i = 4;
//                UITextAlign(ALIGN_M, ucY, DISP_MODE_NORMAL, "CHARGED");
                UIShowBMP(0, 0, "charged.zip");

            }
            else
            {
                UIShowBMP(0, 0, "charge.zip");
//				UITextAlign(ALIGN_M, ucY, DISP_MODE_NORMAL, "CHARGING...");
            }

//			sprintf(capacity, "CURRENT POWER:%03d", gBatLastCapacity);
//			UITextAlign(ALIGN_M, ucY+1, DISP_MODE_NORMAL,capacity);
#if 0
            switch (i)
            {
                case 1:
                    //LcdClearRect(X, Y*16, 40, 16);
                    // blank space
                    UIIconExtend(40, ucStartY, 40, 16, (unsigned char *)IconBigBattery0);
                    break;
                case 2:
                    // level 1
                    UIIconExtend(40, ucStartY, 40, 16, (unsigned char *)IconBigBattery1);
                    break;
                case 3:
                    // level 2
                    UIIconExtend(40, ucStartY, 40, 16, (unsigned char *)IconBigBattery2);
                    break;
                case 4:
                    // level 3
                    UIIconExtend(40, ucStartY, 40, 16, (unsigned char *)IconBigBattery3);
                    break;
                default:
                    break;
            }

            i++;
            if (i == 5)
            {
                i = 1;
            }

            ulTimerOut = SysGetTimer() + 1000;
            SysTimerRefresh();
#endif
        }
    }
#else

    Lib_PowerChargingCtrl();

    DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);
    KbLedSetDelayOff(TIME_BACKLIGHT_LCD_DEF);
    Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);

    // The program has run to this point, indicating that the power button has been pressed to turn on the device.
    PowerOnDevice();

    // Power on protection: if the battery is below 10%, it is not allowed to startup, and gprs module is not working properly
    if (RET_OK == PowerOnProtect())
    {
        return;
    }
#endif
}

//-----------------------------------------------------------------------------
// Name    : ChargOnly
// Func    : Shutdown charging function
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void ChargOnly(void)
{
    // when the device turned on,if it is a new device, in order to accelerate the speed
	// of downloading font library in the outsourced factory, it can be directly started. Or booting from bootloader after firmware upgrade
    if (IsSysPowerOn())
    {
        PowerOnProtect();
        return;
    }

    if (Lib_GetDevState(ID_DEV_IS_NEW))
    {
        return;
    }

    // Display shutdown charging icon
    PowerOffChargingDisplay();
}

//-----------------------------------------------------------------------------
// Name    : DeviceSleepProcess
// Func    : Device sleep process
// Paras   :
// Ret     :
// Desc    :If the sleep conditions are met, go to sleep
//-----------------------------------------------------------------------------
void DeviceSleepProcess(void)
{
	extern unsigned char s_ucSleepEnableF;
#if defined(MQTT_SUPPORT)
	unsigned char mode = 0;
#endif

	// Whether to connect to external power supply
	if (ChargerPresence())
	{
		return;
	}

	// If sleep is prohibited, return directly
	if (FALSE == DeviceSleepEnDisableGet())
	{
		return;
	}

	// Sleep time is 0, indicating no sleep
	if (TIME_SEC_SLEEP_DEF == 0)
	{
		return;
	}

    if (Lib_SysGetTimer() < ulSleepTime)
    {
        return;
    }

    TRACE("MCU stop. %d\r\n", ulSleepTime);
#if defined(MQTT_SUPPORT)
    if (MqttGetTaskStatus())
    {
        mode = 1;
    }
    Lib_SetInfo(E_DEV_SLEEP_TYPE, &mode, sizeof(unsigned char));
#endif
    Lib_LvglSleep();
    Lib_SysSleep();
    Lib_LvglWakeUp();
    TRACE("MCU start. %d\r\n", ulSleepTime);

    Lib_WDGReloadCounter();

    // Avoid direct shutdown after powerkey wake-up
	PowerRefreshTimeOut();

    // Refresh Sleep Time & Backlight Time
    DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);
    KbLedSetDelayOff(TIME_BACKLIGHT_LCD_DEF);

    // Restore screen brightness
    Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);

	// After waking up from sleep, set the software key to the power key
	KbSetKey(KB_POWER_OFF);
}

//-----------------------------------------------------------------------------
// Name    : PowerInit
// Func    : Power management initialization
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void PowerInit(void)
{
    // Refresh Sleep & Time Backlight Time
    DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);
    KbLedSetDelayOff(TIME_BACKLIGHT_LCD_DEF);

    // The power processing for successful firmware upgrade is the same as the power button startup process
    if (Lib_CheckUpgradeFlag())
    {
        // After successful upgrade
        PowerOnDevice();
        UITextAlign(ALIGN_M, MAX_COM_LINES-1, DISP_MODE_NORMAL, UPGRADE_SUCCESS);
        UIUpdataRect(0, 0, LCD_MAX_SEG, MAX_COM_LINES);
        Lib_DelayMs(1000);
    }

    // check if the startup is caused by the powerkey or reset
    if (IsPowerKeyPowerOn() || IsResetPowerOn())
    {
        FUN_IN;
        PowerOnDevice();
        BeepOn();
    }

    // Shutdown charging management
    ChargOnly();
}

