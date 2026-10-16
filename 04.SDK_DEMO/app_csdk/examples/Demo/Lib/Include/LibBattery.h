//-----------------------------------------------------------------------------
// @file    : LibBattery.h
// @brief   : Battery-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef _LIB__BATTERY_H_
#define _LIB__BATTERY_H_

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name    : Lib_BatRefreshDisCapacity
// Func    : When switching from charge to discharge, refresh the initial value of the battery level
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void Lib_BatRefreshDisCapacity(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatGetCapacity
// Func    : Get current actual battery capacity
// Paras   :
// Ret     : 0 -- 100
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_BatGetCapacity(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatDischargeLowCap
// Func    : Determine whether a forced shutdown is required in battery discharge state
// Paras   :
// Ret     : return 1, force shutdown,0: do not shut down
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_BatDischargeLowCap(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatGetPowerOffVol
// Func    : get the configured shutdown voltage,Unit mV
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
unsigned long Lib_BatGetPowerOffVol(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatGetVoltage
// Func    : Get battery voltage
// Paras   : 
// Ret     : return battery voltage
// Desc    :
//-----------------------------------------------------------------------------
unsigned long Lib_BatGetVoltage(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatGetChargeState
// Func    : Get the battery's status of charge
// Paras   : 
// Ret     : 0: Not charged,1: charging
// Desc    :
//-----------------------------------------------------------------------------
int Lib_BatGetChargeState(void);

//-----------------------------------------------------------------------------
// Name    : Lib_BatGetLibVersion
// Func    : Get battery library version
// Paras   : paucLibVer[in/out],  output version
//           pusLen[in/out],      output version length
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void Lib_BatGetLibVersion(char *paucLibVer, unsigned short *pusLen);

#endif

