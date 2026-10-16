/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Battery.c
 *  Version      :
 *  Author       :
 * Description   : Battery level management function
 **********************************************************************************************************/

#include "Language.h"
#include "Battery.h"
#include "power.h"
#include "Ui.h"
#include "Drv.h"
#include "Common.h"
#include "LibBattery.h"
#include "MessageBox.h"
#include "api_lvgl.h"

// The displayed battery level, refresh every two seconds
unsigned char gBatLastCapacity = 100;
//unsigned char gChargingStatus = FALSE;
static unsigned char gBatCapacity = 50; // Current battery level

//-----------------------------------------------------------------------------
// Name    : BatteryTimerWork
// Func    : Power management logic entry
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void BatteryTimerWork(bool lowVolDetect)
{
    static unsigned long ulBatTimerOut = 0;
    static unsigned long ulChaTimerOut = 0;

    if (Lib_SysGetTimer() < 2000)
    {
        return;
    }
    if (Lib_SysGetTimer() > ulBatTimerOut)
    {
         ulBatTimerOut = Lib_SysGetTimer() + 1000;
        // Charging
        if (ChargerPresence())
        {
            ulChaTimerOut = Lib_SysGetTimer() + 2000;
            return;
        }
        else if (Lib_SysGetTimer() > ulChaTimerOut)
        {
            // After standing for 2 seconds, refresh the initial value of discharge power
            if (ulChaTimerOut)
            {
                ulChaTimerOut = 0;
                // Refresh discharge battery level
                Lib_BatRefreshDisCapacity();
            }

            gBatCapacity = Lib_BatGetCapacity();

            // update battery parameter after adc and capacity has been changed
            if (gBatCapacity != gBatLastCapacity)
            {
                gBatLastCapacity = gBatCapacity;
            }

            // Detect whether to force shutdown
            if (lowVolDetect)
            {
                if (TRUE == Lib_BatDischargeLowCap())
                {
                    if (!ChargerPresence())
                    {
                        // Low battery forced shutdown
                        //UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, TIP_BAT_LOW_TO_CHG);
                        //UITextAlign(ALIGN_M, 3, DISP_MODE_NORMAL, TIP_BAT_LOW_SHUT_DOWN);
                        unsigned char pDispBuf[100];
                        memset(pDispBuf, 0, sizeof(pDispBuf));
                        sprintf((char *)pDispBuf, "%s\n%s", TIP_BAT_LOW_TO_CHG, TIP_BAT_LOW_SHUT_DOWN);

                        LvUiMessageBox((char *)pDispBuf, MB_COLOR_BLUE, NULL, 1000);
                        Lib_DelayMs(1000);
                        Lib_LvglTask();

                        PowerOffDevice();
                    }
                }
            }

            //BattRefreshCount++;
            TRACE("battery capacity is %d \r\n", gBatLastCapacity);
        }
     }
}


//-----------------------------------------------------------------------------
// Name    : BatteryGetBatCapacity
// Func    : get power (percentage value)
// Paras   :
// Ret     : Return<1 indicates low battery level
// Desc    :
//-----------------------------------------------------------------------------
unsigned char BatteryGetBatCapacity(void)
{
    return gBatLastCapacity;
}


