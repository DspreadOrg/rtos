/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Battery.h
 *  Version      :
 *  Author       :
 * Description   : Battery level management function
 **********************************************************************************************************/
#ifndef _BATTERY_H_
#define _BATTERY_H_

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

void BatteryTimerWork(bool lowVolDetect);
unsigned char BatteryGetBatCapacity(void);

#endif

