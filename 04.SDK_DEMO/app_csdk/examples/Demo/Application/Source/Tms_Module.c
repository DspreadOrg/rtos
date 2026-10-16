/**********************************************************************************************************
 * Copyright(C),
 * File Name     : Tms_Module.c
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : tms upgrade module process
 **********************************************************************************************************/
#include "Common.h"
#include "Ui.h"
#include "Language.h"
#include "Communication.h"
#include "ParamManage.h"
#include "Tms_Module.h"
#include "Power.h"

//The module's single packet information unit 512 bytes and the regular packet 512 bytes header have been downloaded then can proceed to the next step
extern short Lib_TmsDownloadRemote(NET_MODE netMode,DOWNLOAD_MODULE module,int timeout,unsigned char *updateFlag);


//-----------------------------------------------------------------------------
// Name    : UITmsGprsModuleDownOnline
// Func    : GPRS module upgrade function
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UITmsGprsModuleDownOnline(int timeout)
{
    unsigned char ucUpdateFlag = FALSE;

    return Lib_TmsDownloadRemote(GetNetWorkMode(), DL_GPRS, timeout, &ucUpdateFlag);
}


//-----------------------------------------------------------------------------
// Name    : UITmsWifiModuleDownOnline
// Func    : WiFi module upgrade function
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UITmsWifiModuleDownOnline(int timeout)
{
    unsigned char ucUpdateFlag = FALSE;

    return Lib_TmsDownloadRemote(GetNetWorkMode(), DL_WIFI, timeout, &ucUpdateFlag);
}


//-----------------------------------------------------------------------------
// Name    : UITmsSeModuleDownOnline
// Func    : SE module upgrade function
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UITmsSeModuleDownOnline(int timeout)
{
    unsigned char ucUpdateFlag = FALSE;

    return Lib_TmsDownloadRemote(GetNetWorkMode(), DL_SCHIP, timeout, &ucUpdateFlag);
}


//-----------------------------------------------------------------------------
// Name    : UITmsFsModuleDownOnline
// Func    : FS module upgrade function
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UITmsFsModuleDownOnline(int timeout)
{
    unsigned char ucUpdateFlag = FALSE;

    return Lib_TmsDownloadRemote(GetNetWorkMode(), DL_FILE, timeout, &ucUpdateFlag);
}


// New software + new parameter version management

//-----------------------------------------------------------------------------
// Name    : UITmsAppModuleDownOnline
// Func    : Remote Download Source File Process
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UITmsAppModuleDownOnline(int timeout)
{
    unsigned char ucUpateFlag = FALSE;

    return Lib_TmsDownloadRemote(GetNetWorkMode(), DL_FIRMWARE, timeout, &ucUpateFlag);
}

//-----------------------------------------------------------------------------
// Name    : TmsAutoUpgrade
// Func    : Automatic detection of TMS upgrades
// Paras   :
// Ret     :
// Desc    : The module upgrade order within this function cannot be modified
//-----------------------------------------------------------------------------
short TmsAutoUpgrade(int timeout)
{
    unsigned char ucSleepEnableF;
    short ret = RET_ERR;

    ucSleepEnableF = DeviceSleepEnDisableGet();
    if (ucSleepEnableF)
    {
        DeviceSleepEnDisableSet(FALSE);
    }

    //Detect OS upgrade tasks and download
    ret = UITmsGprsModuleDownOnline(timeout);
    if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
    {
        DeviceSleepEnDisableSet(ucSleepEnableF);
        return ret;
    }

#if (FUNCTION_TMS_ENABLE == 1)
    //Detect SE upgrade tasks and download
    ret = UITmsSeModuleDownOnline(timeout);
    if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
    {
        DeviceSleepEnDisableSet(ucSleepEnableF);
        return ret;
    }

    //Detect Fs upgrade tasks and download
    ret = UITmsFsModuleDownOnline(timeout);
    if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
    {
        DeviceSleepEnDisableSet(ucSleepEnableF);
        return ret;
    }

    //Detect App upgrade tasks and download
    ret = UITmsAppModuleDownOnline(timeout);
    if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
    {
        DeviceSleepEnDisableSet(ucSleepEnableF);
        return ret;
    }

    //Detect Wifi upgrade tasks and download
    ret = UITmsWifiModuleDownOnline(timeout);
    if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
    {
        DeviceSleepEnDisableSet(ucSleepEnableF);
        return ret;
    }

#endif

    DeviceSleepEnDisableSet(ucSleepEnableF);

    return RET_OK;
}



