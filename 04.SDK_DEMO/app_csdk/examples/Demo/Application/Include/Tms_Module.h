/**********************************************************************************************************
 * Copyright(C), 
 * File Name     : Tms_Module.h
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : Tms upgrade module process base on TMS system
 **********************************************************************************************************/
#ifndef __TMS_MODULE_H__
#define __TMS_MODULE_H__
#include "LibTms.h"

// TMS function enable switch
#define FUNCTION_TMS_ENABLE                (1)

short UITmsGprsModuleDownOnline(int timeout);
short UITmsWifiModuleDownOnline(int timeout);
short UITmsSeModuleDownOnline(int timeout);
short UITmsFsModuleDownOnline(int timeout);
short UITmsAppModuleDownOnline(int timeout);
short TmsAutoUpgrade(int timeout);

#endif
