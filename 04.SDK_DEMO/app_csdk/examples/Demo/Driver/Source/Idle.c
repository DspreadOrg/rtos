/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Idle.c
 *  Version      :
 *  Author       :
 * Description   : device system status management
 **********************************************************************************************************/

#include "Idle.h"
#include "LibApi.h"
#include "Drv.h"
#include "Power.h"
#include "ParamManage.h"
#include "Common.h"
#include "Battery.h"
#include "LibSelfCheck.h"
#include "DevTime.h"

#define BEEP_TIME_DEF   25

static unsigned char s_TimeOn = 0;

// Initial timing of backlight
extern unsigned long s_ulDelayStart;
// Backlight timing time
extern unsigned long s_ulDelaySetCnt;

//LCD on/off switch, default off
extern unsigned char s_ucLcdBacklightAllOn;

void DispMainTimeSet(unsigned char mode)
{
    if(!mode)
    {
        s_TimeOn = 0;
    }
    else
    {
        s_TimeOn = 1;
    }
}

unsigned char DispMainTimeGet(void)
{
    return s_TimeOn;
}


//-----------------------------------------------------------------------------
// Name    : SetLcdBacklightAllOn
// Func    : Set LCD backlight status
// Paras   : UcF: FALSE light switch turned off (default); TRUE light switch is turned on
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short SetLcdBacklightAllOn(unsigned char ucF)
{
	// Switch off, refresh backlight off time
	if (FALSE == ucF)
	{
		//Backlight on, set off delay time
		s_ulDelayStart = Lib_SysGetTimer();
	}

	if ((TRUE == ucF) || (FALSE == ucF))
	{
		s_ucLcdBacklightAllOn = ucF;
	}

	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : KbLedSetDelayOff
// Func    : Set the time of the LCD backlight turn off
// Paras   : usDelaySec, unit seconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void KbLedSetDelayOff(unsigned short usDelaySec)
{
	if (!usDelaySec)
	{
		s_ulDelaySetCnt = 0;
		return;
	}

	s_ulDelayStart = Lib_SysGetTimer();

	if (usDelaySec > SDK_TIME_BACKLIGHT_LCD_MAX)
	{
		s_ulDelaySetCnt = SDK_TIME_BACKLIGHT_LCD_MAX*1000;
	}
	else
	{
		s_ulDelaySetCnt = usDelaySec*1000;
	}
}




//
void BeepOn(void)
{
	unsigned char en = 0;
	unsigned char len;

	len = sizeof(en);
	TerminalParamGet(BEEP_BIT, &en, &len);

	if(en == 0){
		return;
	}

	// beep
	TRACE("beep on");
	Lib_Beep(BEEP_TIME_DEF);

	return;
}


//-----------------------------------------------------------------------------
// Name    : KbOtherQueryOp
// Func    : Other query service functions in keyboard
// Paras   : usDelaySec, unit seconds
// Ret     :
// Desc    : The general sub application is blocking, and some rotation services must be integrated into the buttons,
//           Including: status bar display, LED light status control, low battery alarm, countdown, etc
//-----------------------------------------------------------------------------
void KbOtherQueryOp(void)
{
    static unsigned long ulUiRefreshTime = 0;
    //extern unsigned char GprsIconFreshNow;
    unsigned long ulRefreTime = 2000;
    ST_TOPBAR_TIME_PARAM st_TopBar_Time = {0};
    unsigned long ucTimeSize = sizeof(ST_TOPBAR_TIME_PARAM);

    static unsigned char ucCmute = FALSE;

    if (ucCmute)
    {
        return;
    }

    ucCmute = TRUE;

    if (s_TimeOn)
    {
        //TimeHook(0);
        AutoSyncDateTimeFun();
    }

    Lib_GetInfo(E_TIME_DISPLAY, &st_TopBar_Time, &ucTimeSize);
    if (st_TopBar_Time.uc_Switch == TOPBAR_DISP_TIME_ENABLE)
    {
        //When the time is displayed in the status bar, the refresh time is changed to 1s
        ulRefreTime = 1000;
    }

#if 0
	if(GprsIconFreshNow)
	{
		ulUiRefreshTime = Lib_SysGetTimer() + ulRefreTime;
		UiRefreshNow();

		GprsIconFreshNow = 0;
	}
	else
#endif
	{
		// Status bar refresh
		if (Lib_SysGetTimer() > ulUiRefreshTime)
		{
			ulUiRefreshTime = Lib_SysGetTimer() + ulRefreTime;
			UiRefreshNow();
		}
	}

    // Backlight: if there is an external power supply or the constant light indicator is set, the backlight will not be turned off
    if ((!ChargerPresence()) && (!s_ucLcdBacklightAllOn))
    {
        //s_ulDelaySetCnt=0, Indicates that the backlight is not turned off
        if (s_ulDelaySetCnt)
        {
            if (IS_TIME_OUT(s_ulDelayStart, s_ulDelaySetCnt))
            {
                // Backlight set to minimum brightness
                Lib_LcdSetBrightLevel(1);
            }
        }
        else
        {
            Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        }
    }
    else
    {
        // USB connected or LCD on/off switch turned on
        // Refresh Sleep Time
        DeviceSleepTimeReload(TIME_SEC_SLEEP_DEF*1000);

        Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        // Refresh backlight time
        KbLedSetDelayOff(TIME_BACKLIGHT_LCD_DEF);
    }

    // battery management loop
    BatteryTimerWork(TRUE);

    ucCmute = FALSE;
}


//-----------------------------------------------------------------------------
// Name    : DevFactoryCheck
// Func    : Intercept devides that has not undergone complete safety production
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void DevFactoryCheck(void)
{
#if 1
    unsigned int time = Lib_SysGetTimer();
    // First, check hardware status
    while (1)
    {
        if ((Lib_GetHardState() & 0x01) == 0)
        {
            // SE communication successful
            break;
        }
        if (Lib_SysGetTimer() - time > 3000)
        {
            // SE communication failed, directly enter maintenance mode
            Lib_MaintenanceMode();
        }
    }

    // After upgrade, the new device or combination key triggers a forced internal self-test
    if (GetMenuOnFlag() || (Lib_GetDevState(ID_DEV_IS_NEW) && Lib_CheckUpgradeFlag()))
    {
        Lib_SelfCheckTest(SELF_CHECK_MODE_MANUAL);
    }
    else
    {
        Lib_SelfCheckTest(SELF_CHECK_MODE_AUTO);
    }

#if !defined(SKD_PROD)
    if (TRUE == Lib_GetDevState(ID_DEV_IS_AGING_TEST))
    {
        Lib_SelfCheckTest(SELF_CHECK_MODE_AGING);
    }
#endif
    Lib_LcdClrScreen();

    while (1)
    {
        // Enter application
        if (Lib_GetDevState(ID_DEV_IS_ENTER_APP))
        {
            break;
        }

        Lib_MaintenanceMode();
    }
#endif
}

