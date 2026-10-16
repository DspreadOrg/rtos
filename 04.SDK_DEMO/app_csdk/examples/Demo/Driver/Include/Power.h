/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : Power.h
 *  Version      :
 *  Author       :
 * Description   : Power management function
 **********************************************************************************************************/

#ifndef _TY_POWER_CONTROL_H_
#define _TY_POWER_CONTROL_H_
#include "ParamManage.h"

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

// setting Sleep time (in seconds)
// 0 indicates no sleep
#define SDK_TIME_SEC_SLEEP_MIN          0
#define SDK_TIME_SEC_SLEEP_MAX          250
#define SDK_TIME_SEC_SLEEP_DEF          60
#define TIME_SEC_SLEEP_DEF              (GetPwrParam(E_SLEEP_TIME))

// LCD backlight (in seconds)
// 0 means not turning off backlight
#define SDK_TIME_BACKLIGHT_LCD_MIN      0
#define SDK_TIME_BACKLIGHT_LCD_MAX      250
#define SDK_TIME_BACKLIGHT_LCD_DEF      30
#define TIME_BACKLIGHT_LCD_DEF          (GetPwrParam(E_BLACKLIGHT_TIME))

// Shutdown time (in minutes)
// 0 means no shutdown
#define SDK_TIME_MIN_POWER_OFF_MIN      0
// The platform supports maximum of 90 minutes
#define SDK_TIME_MIN_POWER_OFF_MAX      250
#define SDK_TIME_MIN_POWER_OFF_DEF      60

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

void ChargOnly(void);
void DeviceSleepProcess(void);
unsigned char PowerKeyDown(void);
void DeviceSleepEnDisableSet(unsigned char ucF);
unsigned char DeviceSleepEnDisableGet(void);
short PowerOffOp(int iTimeout);
void DeviceSleepTimeReload(unsigned long ulSleep);
void PowerOffDevice(void);
void PowerOnDevice(void);
unsigned char ChargerPresence(void);
unsigned char GetMenuOnFlag(void);
extern unsigned char IsPowerKeyPowerOn(void);
extern void PowerInit(void);

#endif

