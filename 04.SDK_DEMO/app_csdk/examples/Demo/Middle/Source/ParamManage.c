/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : ParamManage.c
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : Parameter management function
 **********************************************************************************************************/

#include "ParamManage.h"
#include "LibApi.h"
#include "Common.h"
#include "Power.h"
#include "Drv.h"
#include "Idle.h"
#include "LanguageProcess.h"
#include "Tms_Module.h"

extern unsigned char g_ucNetMode;


ST_TEMINAL_PARAM g_teminal_param = {0};

ST_SERVER_PARAM g_stServerParam = {0};

ST_COLOR_PARAM g_stColorParam = {0};

ST_TMS_PARAM g_stTmsParam = {0};

//-----------------------------------------------------------------------------
// Name  : WriteParamToFile
// Func  : Write parameter data to file
// Paras : fileName    -- parameter file name
//         para       --  parameter buffer
//         paraSize   --  parameter buffersize
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short WriteParamToFile(char *fileName, void *para, int paraSize)
{
    int fd = -1;
    int ret = 0;
    int len = 0;
    int iFileSize = -1;
    char *ptrFileDataBuf = NULL;

    FUN_IN;
    // open file
    fd = Lib_FileOpen(fileName, LFS_O_RDWR | LFS_O_CREAT);
    if (fd < 0)
    {
        TRACE("Lib_FileOpen %s fail,%d", fileName, fd);
        return RET_ERR;
    }

    iFileSize = Lib_FileSize(fileName);
    if (iFileSize == paraSize)
    {
        ptrFileDataBuf = Lib_malloc(paraSize + 16);
        if (NULL != ptrFileDataBuf)
        {
            memset(ptrFileDataBuf, 0x00, paraSize + 16);
            ret = Lib_FileSeek(fd, 0, LFS_SEEK_SET);
            if (ret >= 0)
            {
                iFileSize = Lib_FileRead(fd, (unsigned char *)ptrFileDataBuf, paraSize);
                //TRACE("iFileSize:%d, paraSize:%d\n", iFileSize, paraSize);
            }

            // Compare whether the parameter file data is the same
            if (0 == memcmp(ptrFileDataBuf, para, paraSize))
            {
                // The data is the same, No need to save, return directly
                Lib_free(ptrFileDataBuf);
                Lib_FileClose(fd);
                return RET_OK;
            }
            // need to save
            Lib_free(ptrFileDataBuf);
        }
    }

    ret = Lib_FileSeek(fd, 0, LFS_SEEK_SET);
    if (ret < 0)
    {
        TRACE("Lib_FileSeek %s fail,%d", fileName, ret);
        Lib_FileClose(fd);
        return RET_ERR;
    }

    len = Lib_FileWrite(fd, (unsigned char *)para, paraSize);
    if (len < 0)
    {
        TRACE("Lib_FileWrite %s fail,%d", fileName, len);
        Lib_FileClose(fd);
        return RET_ERR;
    }

    ret = Lib_FileClose(fd);
    if (ret < 0)
    {
        TRACE("Lib_FileClose %s fail,%d", fileName, ret);
        return RET_ERR;
    }

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : ReadFileToParam
// Func  : Reading parameter information from a parameter file
// Paras : fileName  : Parameter file name
//         para      : parameter data
//         paraSize  : parameter data length
//         initParaCb: Default callback function for processing
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short ReadFileToParam(char *fileName, void *para, int paraSize, short (*initParaCb)(void))
{
    int fd = -1;
    int ret = 0;
    int len = 0;

    // Open File
    fd = Lib_FileOpen(fileName, LFS_O_RDONLY);
    if(fd < 0)
    {
        ret = RET_ERR;
        TRACE("Lib_FileOpen %s fail", fileName);
        if (initParaCb != NULL)
        {
            ret = initParaCb();
        }
        return ret;
    }

    ret = Lib_FileSeek(fd, 0, LFS_SEEK_SET);
    if(ret < 0)
    {
        TRACE("Lib_FileSeek %s fail,%d", fileName, ret);
        Lib_FileClose(fd);
        return RET_ERR;
    }

    len = Lib_FileRead(fd, (unsigned char *)para, paraSize);
    if(len < 0)
    {
        TRACE("Lib_FileRead %s fail,%d", fileName, len);
        Lib_FileClose(fd);
        return RET_ERR;
    }

    ret = Lib_FileClose(fd);
    if(ret < 0)
    {
        TRACE("Lib_FileClose %s fail,%d", fileName, ret);
        return RET_ERR;
    }

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : TerminalDefaultParam
// Func  : Write default values into parameter file
// Paras :
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
static void TerminalDefaultParam(void)
{
    FUN_IN;
    memcpy(g_teminal_param.aucTerminalNo, "\x31\x32\x33\x34\x35\x36\x37\x38", TERMINAL_NO_SIZE);
    g_teminal_param.ucNetWorkMode = 1;
    g_teminal_param.ucBrightLevel = 5;
    g_ucNetMode = g_teminal_param.ucNetWorkMode;
    memcpy(g_teminal_param.ssid, "ssid_name", strlen("ssid_name"));
    memcpy(g_teminal_param.pass, "wifi_password", strlen("wifi_password"));
    g_teminal_param.beep_en = 1;
    // Default East 1 Region
    g_teminal_param.timeZone = 32;
    memcpy(g_teminal_param.ntpDomain, NTP_SERVER_DOMAIN, strlen(NTP_SERVER_DOMAIN));
    // Default disable automatic synchronization function
    g_teminal_param.timeSyncSwitch = 1;
    // Default synchronization once a week
    g_teminal_param.SyncInterHour = 7;
}


//-----------------------------------------------------------------------------
// Name  : TerminalParamSetDefaultParam
// Func  : Initialize terminal parameters to default values
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short TerminalParamSetDefaultParam(void)
{
    TerminalDefaultParam();

    return WriteParamToFile(TEMIMAL_PARAM_FILE, &g_teminal_param, sizeof(g_teminal_param));
}


//-----------------------------------------------------------------------------
// Name  : TerminalParamInit
// Func  : Terminal parameter information initialization
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short TerminalParamInit(void)
{
    int ret = 0;
    int curFileSize = -1;

    FUN_IN;
    if(Lib_FileMkdir(TEMIMAL_PARAM_FILE, 0) < 0)
    {
        TRACE("TerminalParamInit Lib_FileMkdir error");
        return RET_ERR;
    }

    curFileSize = Lib_FileSize(TEMIMAL_PARAM_FILE);
    // The size of the structure has changed
    if (curFileSize != sizeof(g_teminal_param))
    {
        // file does not exist
        if (curFileSize <= 0)
        {
            return TerminalParamSetDefaultParam();
        }

        // any change in data, reinitialize it. Reading the file below will overwrite the existing parameters
        TerminalDefaultParam();

        // If the file exists, read the current content and rewrite it
        if (RET_OK != ReadFileToParam(TEMIMAL_PARAM_FILE, &g_teminal_param, curFileSize, NULL))
        {
            return TerminalParamSetDefaultParam();
        }

        Lib_FileRemove(TEMIMAL_PARAM_FILE);
        g_ucNetMode = g_teminal_param.ucNetWorkMode;
        return WriteParamToFile(TEMIMAL_PARAM_FILE, &g_teminal_param, sizeof(g_teminal_param));
    }

    ret = ReadFileToParam(TEMIMAL_PARAM_FILE, &g_teminal_param, sizeof(g_teminal_param), TerminalParamSetDefaultParam);
    g_ucNetMode = g_teminal_param.ucNetWorkMode;

    return ret;
}


//-----------------------------------------------------------------------------
// Name  : TerminalParamSet
// Func  : set terminal parameter information
// Paras : ucParamId  -- Terminal parameter number
//         para       -- Terminal parameter buffer
//         ucParamLen -- Terminal parameter length
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short TerminalParamSet(unsigned char ucParamId, void *para, unsigned char ucParamLen)
{
    unsigned char *pucParam = (unsigned char *)para;
    int temp = 0;

    if (pucParam == NULL || ucParamLen <= 0)
        return RET_ERR;

    FUN_IN;

    switch (ucParamId)
    {
    case TERMINAL_NO_BIT:
        memset(g_teminal_param.aucTerminalNo, 0, TERMINAL_NO_SIZE);
        if (ucParamLen > TERMINAL_NO_SIZE)
            ucParamLen = TERMINAL_NO_SIZE;
        memcpy(g_teminal_param.aucTerminalNo, pucParam, ucParamLen);
        break;
    case NET_WORK_MODE_BIT:
        g_teminal_param.ucNetWorkMode = pucParam[0];
        TRACE("TerminalParamSet net:%d", g_teminal_param.ucNetWorkMode);
        break;
    case BRIGHT_LEVEL_BIT:
        g_teminal_param.ucBrightLevel = pucParam[0];
        TRACE("TerminalParamSet Bright level:%d", g_teminal_param.ucBrightLevel);
        break;
    case BEEP_BIT:
        g_teminal_param.beep_en = *pucParam;
        break;
    case WIFI_SSID_BIT:
        memcpy(g_teminal_param.ssid, pucParam, ucParamLen);
        g_teminal_param.ssid[ucParamLen] = 0;
        break;
    case WIFI_PASS_BIT:
        memcpy(g_teminal_param.pass, pucParam, ucParamLen);
        g_teminal_param.pass[ucParamLen] = 0;
        break;
    case TIME_ZONE_BIT:
        temp = *(int *)para;
        if (temp < TIME_ZONE_MIN || temp > TIME_ZONE_MAX || ucParamLen != sizeof(int))
        {
            return RET_ERR_PARAMETER;
        }
        g_teminal_param.timeZone = temp;
        Lib_SetInfo(E_TIME_ZONE, &g_teminal_param.timeZone, sizeof(g_teminal_param.timeZone));
        break;
    case NTP_SERVER_BIT:
        if (ucParamLen > NETWORK_ADDR_SIZE)
        {
            return RET_ERR_OUT_OF_MEMORY;
        }
        memcpy(g_teminal_param.ntpDomain, pucParam, ucParamLen);
        g_teminal_param.ntpDomain[ucParamLen] = 0;
        break;
    case NTP_SYNC_SWITCH_BIT:
        g_teminal_param.timeSyncSwitch = *pucParam;
        break;
    case NTP_SYNC_INTER_BIT:
        if (*pucParam < NTP_SYNC_INTER_HOUR_MIN || *pucParam > NTP_SYNC_INTER_HOUR_MAX || ucParamLen != sizeof(unsigned char))
        {
            return RET_ERR_PARAMETER;
        }
        g_teminal_param.SyncInterHour = *pucParam;
        break;

    default:
        return RET_ERR;
    }

    return WriteParamToFile(TEMIMAL_PARAM_FILE, &g_teminal_param, sizeof(g_teminal_param));
}


//-----------------------------------------------------------------------------
// Name  : TerminalParamGet
// Func  : get terminal parameter information
// Paras : ucParamId   -- Terminal parameter number
//         para        -- Terminal parameter buffer
//         pucParamLen -- Terminal parameter length
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short TerminalParamGet(unsigned char ucParamId, void *para, unsigned char *pucParamLen)
{
    unsigned char *pucParam = (unsigned char *)para;

    if (para == NULL || pucParamLen == NULL)
        return RET_ERR;

    switch (ucParamId)
    {
    case TERMINAL_NO_BIT:
        memcpy(pucParam, g_teminal_param.aucTerminalNo, TERMINAL_NO_SIZE);
        *pucParamLen = TERMINAL_NO_SIZE;
        break;
    case NET_WORK_MODE_BIT:
        pucParam[0] = g_teminal_param.ucNetWorkMode;
        *pucParamLen = 1;
        break;
    case BRIGHT_LEVEL_BIT:
        pucParam[0] = g_teminal_param.ucBrightLevel;
        *pucParamLen = 1;
        break;
    case BEEP_BIT:
        *pucParam =  g_teminal_param.beep_en;
        *pucParamLen = 1;
        break;
    case WIFI_SSID_BIT:
        *pucParamLen = strlen((char *)g_teminal_param.ssid);
        memcpy(pucParam, g_teminal_param.ssid, *pucParamLen);
        break;
    case WIFI_PASS_BIT:
        *pucParamLen = strlen((char *)g_teminal_param.pass);
        memcpy(pucParam, g_teminal_param.pass, *pucParamLen);
        break;
    case TIME_ZONE_BIT:
        *pucParamLen = sizeof(g_teminal_param.timeZone);
        *(int *)para = g_teminal_param.timeZone;
        break;
    case NTP_SERVER_BIT:
        *pucParamLen = strlen((char *)g_teminal_param.ntpDomain) + 1;
        memcpy(pucParam, g_teminal_param.ntpDomain, *pucParamLen);
        break;
    case NTP_SYNC_SWITCH_BIT:
        *pucParam = g_teminal_param.timeSyncSwitch;
        *pucParamLen = 1;
        break;
    case NTP_SYNC_INTER_BIT:
        *pucParam = g_teminal_param.SyncInterHour;
        *pucParamLen = 1;
        break;

    default:
        return RET_ERR;
    }

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : ServerParamSetDefaultParam
// Func  : Server default parameter initialization
// Paras :
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
static short ServerParamSetDefaultParam(void)
{
    // Default use domain name connection
    g_stServerParam.ucUseDomainNameEn = '1';

    memcpy(g_stServerParam.aucIP1, HOST1_IP, strlen(HOST1_IP));
    memcpy(g_stServerParam.aucPort1, HOST1_PORT, strlen(HOST1_PORT));

    memcpy(g_stServerParam.aucIP2, HOST2_IP, strlen(HOST2_IP));
    memcpy(g_stServerParam.aucPort2, HOST2_PORT, strlen(HOST2_PORT));

    memcpy(g_stServerParam.aucDomain1, NET_NAME_POSP, strlen(NET_NAME_POSP));
    memcpy(g_stServerParam.aucDomainPort1, NET_NAME_POSP_PORT, strlen(NET_NAME_POSP_PORT));


    memcpy(g_stServerParam.aucDomain2, NET_NAME_POSP_2, strlen(NET_NAME_POSP_2));
    memcpy(g_stServerParam.aucDomainPort2, NET_NAME_POSP_PORT_2, strlen(NET_NAME_POSP_PORT_2));

    return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
}


//-----------------------------------------------------------------------------
// Name  : ServerParamInit
// Func  : Server parameter initialization
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short ServerParamInit(void)
{
    int curFileSize = -1;

    curFileSize = Lib_FileSize(SERVER_PARAM_FILE);
    // The size of the structure has changed
    if (curFileSize != sizeof(g_stServerParam))
    {
        // file does not exist
        if (curFileSize <= 0)
        {
            return ServerParamSetDefaultParam();
        }

        ServerParamSetDefaultParam();
        // If file exists, read the current content and rewrite it
        if (RET_OK != ReadFileToParam(SERVER_PARAM_FILE, &g_stServerParam, curFileSize, NULL))
        {
            return ServerParamSetDefaultParam();
        }

        Lib_FileRemove(SERVER_PARAM_FILE);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    }

    return ReadFileToParam(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam), ServerParamSetDefaultParam);
}


//-----------------------------------------------------------------------------
// Name  : TmsServerDefaultParam
// Func  : TMS Server & TMS  Server default parameter initialization
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short TmsServerDefaultParam(void)
{
    // Default use url to connect
    g_stTmsParam.stTmsServerParam.ucUseDomainNameEn = '1';

    memcpy(g_stTmsParam.stTmsServerParam.aucIP1, TMS_HOST1_IP, strlen(TMS_HOST1_IP));
    memcpy(g_stTmsParam.stTmsServerParam.aucPort1, TMS_HOST1_PORT, strlen(TMS_HOST1_PORT));

    memcpy(g_stTmsParam.stTmsServerParam.aucIP2, TMS_HOST2_IP, strlen(TMS_HOST2_IP));
    memcpy(g_stTmsParam.stTmsServerParam.aucPort2, TMS_HOST2_PORT, strlen(TMS_HOST2_PORT));

    memcpy(g_stTmsParam.stTmsServerParam.aucDomain1, TMS_NET_NAME_POSP, strlen(TMS_NET_NAME_POSP));
    memcpy(g_stTmsParam.stTmsServerParam.aucDomainPort1, TMS_NET_NAME_POSP_PORT, strlen(TMS_NET_NAME_POSP_PORT));

    memcpy(g_stTmsParam.stTmsServerParam.aucDomain2, TMS_NET_NAME_POSP_2, strlen(TMS_NET_NAME_POSP_2));
    memcpy(g_stTmsParam.stTmsServerParam.aucDomainPort2, TMS_NET_NAME_POSP_PORT_2, strlen(TMS_NET_NAME_POSP_PORT_2));

    memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomain1, TMS_MODULE_DOMAIN_1, strlen(TMS_MODULE_DOMAIN_1));
    memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1, TMS_MODULE_DOMAIN_PORT_1, strlen(TMS_MODULE_DOMAIN_PORT_1));
    memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomain2, TMS_MODULE_DOMAIN_1, strlen(TMS_MODULE_DOMAIN_1));
    memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort2, TMS_MODULE_DOMAIN_PORT_1, strlen(TMS_MODULE_DOMAIN_PORT_1));

    //return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsServerParam, sizeof(g_stTmsServerParam));
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : TmsParamSetDefaultParam
// Func  : TMS Server & TMS module Server parameter initialization
// Paras :
// Ret   : Normal return RET_OK, otherwise return < 0
// Desc  :
//-----------------------------------------------------------------------------
static short TmsParamSetDefaultParam(void)
{
    TmsServerDefaultParam();

    return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));
}


//-----------------------------------------------------------------------------
// Name  : TmsServerParamInit
// Func  : TMS Server parameter initialization
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short TmsServerParamInit(void)
{
    int curFileSize = -1;

    curFileSize = Lib_FileSize(TMS_SERVER_PARAM_FILE);

    TRACE("TMS Server Param sizeo:%d\n", curFileSize);
    // The size of the structure has changed
    if (curFileSize != sizeof(g_stTmsParam))
    {
        if (curFileSize <= 0)
        {
            return TmsParamSetDefaultParam();
        }
        TmsServerDefaultParam();
        if (RET_OK != ReadFileToParam(TMS_SERVER_PARAM_FILE, &g_stTmsParam, curFileSize, NULL))
        {
            return TmsParamSetDefaultParam();
        }

        Lib_FileRemove(TMS_SERVER_PARAM_FILE);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));
    }

    return ReadFileToParam(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam), TmsParamSetDefaultParam);
}


//-----------------------------------------------------------------------------
// Name  : ServerParamSet
// Func  : Set terminal server parameters
// Paras : ucParamId   -- Server parameter number
//         pucParam    -- Server parameter buffer
//         ucParamLen  -- Server parameter cache length
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short ServerParamSet(unsigned char ucParamId, unsigned char *pucParam, unsigned char ucParamLen)
{
    if (pucParam == NULL || ucParamLen <= 0)
        return RET_ERR;

    FUN_IN;

    switch (ucParamId)
    {
    case POSP_SERVER_MODE_BIT:
        g_stServerParam.ucUseDomainNameEn = pucParam[0];
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case NET_NAME_POSP_BIT:// domain name
        memset(g_stServerParam.aucDomain1, 0, sizeof(g_stServerParam.aucDomain1));
        if(ucParamLen > sizeof(g_stServerParam.aucDomain1))
        {
            ucParamLen = sizeof(g_stServerParam.aucDomain1);
        }
        memcpy(g_stServerParam.aucDomain1, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case NET_NAME_POSP_PORT_BIT:// Port
        memset(g_stServerParam.aucDomainPort1, 0, sizeof(g_stServerParam.aucDomainPort1));
        if(ucParamLen > sizeof(g_stServerParam.aucDomainPort1))
        {
            ucParamLen = sizeof(g_stServerParam.aucDomainPort1);
        }
        memcpy(g_stServerParam.aucDomainPort1, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case NET_NAME_POSP_2_BIT:
        memset(g_stServerParam.aucDomain2, 0, sizeof(g_stServerParam.aucDomain2));
        if(ucParamLen > sizeof(g_stServerParam.aucDomain2))
        {
            ucParamLen = sizeof(g_stServerParam.aucDomain2);
        }
        memcpy(g_stServerParam.aucDomain2, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case NET_NAME_POSP_PORT_2_BIT:
        memset(g_stServerParam.aucDomainPort2, 0, sizeof(g_stServerParam.aucDomainPort2));
        if(ucParamLen > sizeof(g_stServerParam.aucDomainPort2))
        {
            ucParamLen = sizeof(g_stServerParam.aucDomainPort2);
        }
        memcpy(g_stServerParam.aucDomainPort2, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));

    case HOST1IP_BIT:
        memset(g_stServerParam.aucIP1, 0, sizeof(g_stServerParam.aucIP1));
        if(ucParamLen > sizeof(g_stServerParam.aucIP1))
        {
            ucParamLen = sizeof(g_stServerParam.aucIP1);
        }
        memcpy(g_stServerParam.aucIP1, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case HOST1PORT_BIT:
        memset(g_stServerParam.aucPort1, 0, sizeof(g_stServerParam.aucPort1));
        if(ucParamLen > sizeof(g_stServerParam.aucPort1))
        {
            ucParamLen = sizeof(g_stServerParam.aucPort1);
        }
        memcpy(g_stServerParam.aucPort1, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case HOST2IP_BIT:
        memset(g_stServerParam.aucIP2, 0, sizeof(g_stServerParam.aucIP2));
        if(ucParamLen > sizeof(g_stServerParam.aucIP2))
        {
            ucParamLen = sizeof(g_stServerParam.aucIP2);
        }
        memcpy(g_stServerParam.aucIP2, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));
    case HOST2PORT_BIT:
        memset(g_stServerParam.aucPort2, 0, sizeof(g_stServerParam.aucPort2));
        if(ucParamLen > sizeof(g_stServerParam.aucPort2))
        {
            ucParamLen = sizeof(g_stServerParam.aucPort2);
        }
        memcpy(g_stServerParam.aucPort2, pucParam, ucParamLen);
        return WriteParamToFile(SERVER_PARAM_FILE, &g_stServerParam, sizeof(g_stServerParam));

    // TMS server parameters
    case TMS_SERVER_MODE_BIT:
        g_stTmsParam.stTmsServerParam.ucUseDomainNameEn = pucParam[0];
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case NET_NAME_TMS_BIT:// domain name
        memset(g_stTmsParam.stTmsServerParam.aucDomain1, 0, sizeof(g_stTmsParam.stTmsServerParam.aucDomain1));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucDomain1))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomain1);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucDomain1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case NET_NAME_TMS_PORT_BIT:// port
        memset(g_stTmsParam.stTmsServerParam.aucDomainPort1, 0, sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort1));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort1))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort1);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucDomainPort1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case NET_NAME_TMS_2_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucDomain2, 0, sizeof(g_stTmsParam.stTmsServerParam.aucDomain2));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucDomain2))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomain2);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucDomain2, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case NET_NAME_TMS_PORT_2_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucDomainPort2, 0, sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort2));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort2))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort2);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucDomainPort2, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case TMS1IP_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucIP1, 0, sizeof(g_stTmsParam.stTmsServerParam.aucIP1));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucIP1))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucIP1);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucIP1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case TMS1PORT_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucPort1, 0, sizeof(g_stTmsParam.stTmsServerParam.aucPort1));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucPort1))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucPort1);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucPort1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));
    case TMS2IP_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucIP2, 0, sizeof(g_stTmsParam.stTmsServerParam.aucIP2));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucIP2))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucIP2);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucIP2, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case TMS2PORT_BIT:
        memset(g_stTmsParam.stTmsServerParam.aucPort2, 0, sizeof(g_stTmsParam.stTmsServerParam.aucPort2));
        if(ucParamLen > sizeof(g_stTmsParam.stTmsServerParam.aucPort2))
        {
            ucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucPort2);
        }
        memcpy(g_stTmsParam.stTmsServerParam.aucPort2, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    //NTP server parameters
    case NTP_SERVER_BIT:
        return TerminalParamSet(ucParamId, pucParam, ucParamLen);

    case NET_NAME_MODULE_BIT:
        if (ucParamLen > NETWORK_ADDR_SIZE)
        {
            ucParamLen = NETWORK_ADDR_SIZE;
        }
        memset(g_stTmsParam.stTmsModuleParam.aucModuleDomain1, 0, sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomain1));
        memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomain1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    case NET_NAME_MODULE_PORT_BIT:
        if (ucParamLen > NETWORK_PORT_SIZE + 1)
        {
            ucParamLen = NETWORK_PORT_SIZE + 1;
        }
        memset(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1, 0, sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1));
        memcpy(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1, pucParam, ucParamLen);
        return WriteParamToFile(TMS_SERVER_PARAM_FILE, &g_stTmsParam, sizeof(g_stTmsParam));

    default:
        return RET_ERR;
    }
}


//-----------------------------------------------------------------------------
// Name  : ServerParamGet
// Func  : Obtain server parameter information
// Paras : ucParamId   -- Server parameter number
//         pucParam    -- Server parameter buffer
//         pucParamLen -- Server parameter cache length
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short ServerParamGet(unsigned char ucParamId, unsigned char *pucParam, unsigned char *pucParamLen)
{
    if (pucParam == NULL || pucParamLen == NULL)
        return RET_ERR;

    switch (ucParamId)
    {
    case POSP_SERVER_MODE_BIT:
        *pucParam =  g_stServerParam.ucUseDomainNameEn;
        *pucParamLen = 1;
        break;
    case NET_NAME_POSP_BIT:
        memcpy(pucParam, g_stServerParam.aucDomain1, sizeof(g_stServerParam.aucDomain1));
        *pucParamLen = sizeof(g_stServerParam.aucDomain1);
        break;
    case NET_NAME_POSP_PORT_BIT:
        memcpy(pucParam, g_stServerParam.aucDomainPort1, sizeof(g_stServerParam.aucDomainPort1));
        *pucParamLen = sizeof(g_stServerParam.aucDomainPort1);
        break;
    case NET_NAME_POSP_2_BIT:
        memcpy(pucParam, g_stServerParam.aucDomain2, sizeof(g_stServerParam.aucDomain2));
        *pucParamLen = sizeof(g_stServerParam.aucDomain2);
        break;
    case NET_NAME_POSP_PORT_2_BIT:
        memcpy(pucParam, g_stServerParam.aucDomainPort2, sizeof(g_stServerParam.aucDomainPort2));
        *pucParamLen = sizeof(g_stServerParam.aucDomainPort2);
        break;
    case HOST1IP_BIT:
        memcpy(pucParam, g_stServerParam.aucIP1, sizeof(g_stServerParam.aucIP1));
        *pucParamLen = sizeof(g_stServerParam.aucIP1);
        break;
    case HOST1PORT_BIT:
        memcpy(pucParam, g_stServerParam.aucPort1, sizeof(g_stServerParam.aucPort1));
        *pucParamLen = sizeof(g_stServerParam.aucPort1);
        break;
    case HOST2IP_BIT:
        memcpy(pucParam, g_stServerParam.aucIP2, sizeof(g_stServerParam.aucIP2));
        *pucParamLen = sizeof(g_stServerParam.aucIP2);
        break;
    case HOST2PORT_BIT:
        memcpy(pucParam, g_stServerParam.aucPort2, sizeof(g_stServerParam.aucPort2));
        *pucParamLen = sizeof(g_stServerParam.aucPort2);
        break;

    // TMS server parameters
    case TMS_SERVER_MODE_BIT:
        *pucParam =  g_stTmsParam.stTmsServerParam.ucUseDomainNameEn;
        *pucParamLen = 1;
        break;
    case NET_NAME_TMS_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucDomain1, sizeof(g_stTmsParam.stTmsServerParam.aucDomain1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomain1);
        break;
    case NET_NAME_TMS_PORT_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucDomainPort1, sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort1);
        break;
    case NET_NAME_TMS_2_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucDomain2, sizeof(g_stTmsParam.stTmsServerParam.aucDomain2));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomain2);
        break;
    case NET_NAME_TMS_PORT_2_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucDomainPort2, sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort2));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucDomainPort2);
        break;
    case TMS1IP_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucIP1, sizeof(g_stTmsParam.stTmsServerParam.aucIP1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucIP1);
        break;
    case TMS1PORT_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucPort1, sizeof(g_stTmsParam.stTmsServerParam.aucPort1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucPort1);
        break;
    case TMS2IP_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucIP2, sizeof(g_stTmsParam.stTmsServerParam.aucIP2));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucIP2);
        break;
    case TMS2PORT_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsServerParam.aucPort2, sizeof(g_stTmsParam.stTmsServerParam.aucPort2));
        *pucParamLen = sizeof(g_stTmsParam.stTmsServerParam.aucPort2);
        break;

    //NTP server parameters
    case NTP_SERVER_BIT:
        return TerminalParamGet(ucParamId, pucParam, pucParamLen);

    case NET_NAME_MODULE_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsModuleParam.aucModuleDomain1, sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomain1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomain1);
        break;

    case NET_NAME_MODULE_PORT_BIT:
        memcpy(pucParam, g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1, sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1));
        *pucParamLen = sizeof(g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1);
        break;

    default:
        return RET_ERR;
    }

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : ColorParamSetDefaultParam
// Func  : Color/Theme parameter file set default values
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short ColorParamSetDefaultParam(void)
{
    FUN_IN;

    g_stColorParam.mainForeColor = BACK565_BLUE;
    g_stColorParam.mainBackColor = RGB565_WHITE;

    g_stColorParam.menu1ForeColor = RGB565_WHITE;
    g_stColorParam.menu1BackColor = BACK565_BLUE;

    g_stColorParam.menu2ForeColor = RGB_USER_BLACK;
    g_stColorParam.menu2BackColor = RGB_USER_LIGHT_GREY;

    g_stColorParam.tipsForeColor = BACK565_BLUE;
    g_stColorParam.tipsBackColor = RGB565_WHITE;

    return WriteParamToFile(COLOR_PARAM_FILE, &g_stColorParam, sizeof(g_stColorParam));
}


//-----------------------------------------------------------------------------
// Name  : ColorParamInit
// Func  : Color/Theme parameter initialization
// Paras :
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
static short ColorParamInit(void)
{
    short ret = 0;

    ret = ReadFileToParam(COLOR_PARAM_FILE, &g_stColorParam, sizeof(g_stColorParam), ColorParamSetDefaultParam);
    Lib_LcdParaSet(LCD_W, LINE_HEIGTH, DEFAULT_LIST_EN, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

    return ret;
}


//-----------------------------------------------------------------------------
// Name  : SetColorParam
// Func  : set Color/Theme parameter
// Paras : param      -- colour parameter buffer
//         paramSize  -- colour parameter size
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short SetColorParam(ST_COLOR_PARAM *param, int paramSize)
{
    if (paramSize != sizeof(g_stColorParam))
    {
        return RET_ERR;
    }

    memcpy(&g_stColorParam, param, paramSize);

    return WriteParamToFile(COLOR_PARAM_FILE, &g_stColorParam, paramSize);
}


//-----------------------------------------------------------------------------
// Name  : GetColorParam
// Func  : get Color/Theme parameter
// Paras : param      -- colour parameter buffer
//         paramSize  -- colour parameter size
// ret   : Normal return RET_OK, otherwise return < 0
// desc  :
//-----------------------------------------------------------------------------
short GetColorParam(ST_COLOR_PARAM *param, int paramSize)
{
    if (paramSize != sizeof(g_stColorParam))
    {
        return RET_ERR;
    }

    memcpy(param, &g_stColorParam, paramSize);

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : GetColorParamEx
// Func  : get Color/Theme parameter extended function
// Paras : paramId -- Parameter number ID
// ret   : Normal return 0, otherwise return<0
// desc  :
//-----------------------------------------------------------------------------
unsigned short GetColorParamEx(E_ID_COLOR paramId)
{
    unsigned short ret = 0;

    switch (paramId)
    {
    case MAIN_FORE_COLOR_ID:
        ret = g_stColorParam.mainForeColor;
        break;
    case MAIN_BACK_COLOR_ID:
        ret = g_stColorParam.mainBackColor;
        break;
    case MENU1_FORE_COLOR_ID:
        ret = g_stColorParam.menu1ForeColor;
        break;
    case MENU1_BACK_COLOR_ID:
        ret = g_stColorParam.menu1BackColor;
        break;
    case MENU2_FORE_COLOR_ID:
        ret = g_stColorParam.menu2ForeColor;
        break;
    case MENU2_BACK_COLOR_ID:
        ret = g_stColorParam.menu2BackColor;
        break;
    case TIPS_FORE_COLOR_ID:
        ret = g_stColorParam.tipsForeColor;
        break;
    case TIPS_BACK_COLOR_ID:
        ret = g_stColorParam.tipsBackColor;
        break;
    default:
        break;
    }

    return ret;
}


//-----------------------------------------------------------------------------
// Name  : LanguageParamSetDefaultParam
// Func  : Writing default values to language parameter files
// Paras : void
// ret   : Normal return 0, otherwise return<0
// desc  :
//-----------------------------------------------------------------------------
static short LanguageParamSetDefaultParam(void)
{
    FUN_IN;
    // Default English
    g_ucLanguage = ENGLISH_ID;
    return WriteParamToFile(LANGUAGE_PARAM_FILE, &g_ucLanguage, sizeof(g_ucLanguage));
}


//-----------------------------------------------------------------------------
// Name  : LanguageParamInit
// Func  : language parameter initialization
// Paras : void
// ret   : Normal return 0, otherwise return<0
// desc  :
//-----------------------------------------------------------------------------
static short LanguageParamInit(void)
{
    short ret = 0;

    ret = ReadFileToParam(LANGUAGE_PARAM_FILE, &g_ucLanguage, sizeof(g_ucLanguage), LanguageParamSetDefaultParam);
    // If the language parameter is not initialized, defaults to English
    if ((g_ucLanguage == 0xFF) || (g_ucLanguage >= MAX_LANG_ID))
    {
        g_ucLanguage = ENGLISH_ID;
    }
    return ret;
}


//-----------------------------------------------------------------------------
// Name  : SetLanguageParam
// Func  : set language parameter
// Paras : param      -- Language parameter buffer
//         paramSize  -- Language parameter size
// ret   : Normal return 0, otherwise return<0
// desc  :
//-----------------------------------------------------------------------------
short SetLanguageParam(unsigned char *param, int paramSize)
{
    if (paramSize != sizeof(g_ucLanguage))
    {
        return RET_ERR;
    }

    memcpy(&g_ucLanguage, param, paramSize);

    return WriteParamToFile(LANGUAGE_PARAM_FILE, &g_ucLanguage, paramSize);
}


//-----------------------------------------------------------------------------
// Name  : GetLanguageParam
// Func  : Get language parameter
// Paras : param      -- Language parameter buffer
//         paramSize  -- Language parameter size
// ret   : Normal return 0, otherwise return<0
// desc  :
//-----------------------------------------------------------------------------
short GetLanguageParam(unsigned char *param, int paramSize)
{
    if (paramSize != sizeof(g_ucLanguage))
    {
        return RET_ERR;
    }

#if 1
    memcpy(param, &g_ucLanguage, paramSize);

    return RET_OK;
#else
    return ReadFileToParam(LANGUAGE_PARAM_FILE, param, paramSize, NULL);
#endif
}


//-----------------------------------------------------------------------------
// Name  : TmsParaInit
// Func  : Initialize TMS upgrade parameters
// Paras : void
// Ret   : Successfully returns 0, otherwise returns<0
// Desc  : Store TMS version records + TMS version index + TMS remote download
//         source file information,in sequential order
//-----------------------------------------------------------------------------
int TmsParaInit(void)
{
    return Lib_TmsParamInit();
}

//-----------------------------------------------------------------------------
// Name  : DeviceParaInit
// Func  : Device system operation parameter initialization
// Paras :
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
void DeviceParaInit(void)
{
    TerminalParamInit();
    ServerParamInit();
    ColorParamInit();
    LanguageParamInit();
    TmsServerParamInit();

#if (FUNCTION_TMS_ENABLE == 1)
    TmsParaInit();
#endif

    TRACE("TerminalParamInit ssid:%s", g_teminal_param.ssid);
    TRACE("TerminalParamInit pass:%s", g_teminal_param.pass);
    if(g_teminal_param.ucBrightLevel <= 0)
    {
        g_teminal_param.ucBrightLevel = BRIGHT_LEVEL_DEF;
    }
    TRACE("TerminalParamInit ucBrightLevel:%d", g_teminal_param.ucBrightLevel);
    Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
    // Initialize printing concentration level 5
    Lib_PrnSetGrade(PRINT_GRADE_MAX);
}



//-----------------------------------------------------------------------------
// Name  : AppendFile
// Func  : Write data to file(open + write)
// Paras : fileName : file name
//         para     : Parameter cache address
//         paraSize : parameter size
// Ret   : Success returns 0, failure returns<0
// Desc  :
//-----------------------------------------------------------------------------
int AppendFile(char *fileName, void *para, int paraSize)
{
    int fd = -1;
    int ret = 0;

    // Open File
    fd = Lib_FileOpen(fileName, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);
    if (fd < 0)
    {
        ret = -1;
        goto Result;
    }

    // Zeroing variables that store write length
    ret = 0;
    if (para != NULL)
    {
        ret = Lib_FileWrite(fd, (unsigned char *)para, paraSize);
        if (ret < 0)
        {
            ret = -2;
            goto Result;
        }
    }

Result:
    if (fd > 0)
    {
        Lib_FileClose(fd);
    }

    TRACE_LOG("ret = %d", ret);

    return ret;
}


//-----------------------------------------------------------------------------
// Name  : WriteFile
// Func  : Write data from the specified offset address in the file(open + write)
// Paras : fileName : file name
//         offset   : Write offset address
//         para     : Parameter cache address
//         paraSize : parameter size
// ret   : success return the length of written, otherwise return a negative number
// desc  :
//-----------------------------------------------------------------------------
int WriteFile(char *fileName, int offset, void *para, int paraSize)
{
    int fd = -1;
    int ret = 0;

    // Open File
    fd = Lib_FileOpen(fileName, LFS_O_RDWR | LFS_O_CREAT);
    if (fd < 0)
    {
        ret = -1;
        goto Result;
    }

    ret = Lib_FileSeek(fd, offset, LFS_SEEK_SET);
    if (ret < 0)
    {
        ret = -2;
        goto Result;
    }

    // Zeroing variables that store write length
    ret = 0;
    if (para != NULL)
    {
        ret = Lib_FileWrite(fd, (unsigned char *)para, paraSize);
        if (ret < 0)
        {
            ret = -3;
            goto Result;
        }
    }

Result:
    if (fd > 0)
    {
        Lib_FileClose(fd);
    }

    TRACE_LOG("ret = %d", ret);
    // Successfully returns the length of the write, otherwise returns a negative number
    return ret;
}


//-----------------------------------------------------------------------------
// Name  : ReadFile
// Func  : Read data from the specified offset address in a file(open + read)
// Paras : fileName : file name
//         offset   : read offset address
//         para     : data cache address
//         paraSize : data cache size
// ret   : success return read length, otherwise returns negative number
// Desc  :
//-----------------------------------------------------------------------------
int ReadFile(char *fileName, int offset, void *para, int paraSize)
{
    int fd = -1;
    int ret = 0;

    // Open files in read-only mode
    fd = Lib_FileOpen(fileName, LFS_O_RDONLY);
    if(fd < 0)
    {
        ret = -1;
        goto Result;
    }

    // Check file size
    ret = Lib_FileSeek(fd, 0, LFS_SEEK_END);
    if(ret < 0)
    {
        ret = -2;
        goto Result;
    }

    // Check the starting offset of the read
    if (offset >= ret)
    {
        ret = -3;
        goto Result;
    }

    // check read content size, exceeds maximum file size
    if (offset + paraSize > ret)
    {
        // Read remaining size
        paraSize = ret - offset;
    }

    ret = Lib_FileSeek(fd, offset, LFS_SEEK_SET);
    if(ret < 0)
    {
        ret = -4;
        goto Result;
    }

    if (para != NULL)
    {
        ret = Lib_FileRead(fd, (unsigned char *)para, paraSize);
        if(ret < 0)
        {
            ret = -5;
            goto Result;
        }
    }

Result:

    if (fd > 0)
    {
        Lib_FileClose(fd);
    }

    TRACE_LOG("ret = %d", ret);

    // success return read length, otherwise returns a negative number for errors
    return ret;
}


//-----------------------------------------------------------------------------
// Name  : InitFilePara
// Func  : Initialize parameter file
// Paras : fileName : file name
//         fileSize : file seize
// Ret   : Success returns 0, failure returns<0
// Desc  : 1. Check if the file size meets the expectedv size when poweron each time, and do not modify it if it meets the expected size
//         2. If detects the file does not exist when poweron, create it and write all FF
//         3. If the file exist when powered on detection, the file content will be read out, deleted it, and rewritten.
//            The newly added content will be based on the default Init
//         4. Note: It is not allowed to delete parameter content, only can be added parameter content
//-----------------------------------------------------------------------------
int InitFilePara(char *fileName, unsigned long fileSize, void (*DefaultInit)(void *para, unsigned long paraSize))
{
    unsigned char *srcBuf = NULL;
    int curFileSize = 0;

    // Get the size of current file
    curFileSize = Lib_FileSize(fileName);
    // The current size is consistent with the expected size
    if (curFileSize == fileSize)
    {
        TRACE_LOG("init file:%s[%d/%d] already exist!", fileName, curFileSize, fileSize);
        return RET_OK;
    }

    // If curFileSize is negative, it will affect the comparison criteria below
    if (curFileSize < 0)
    {
        curFileSize = 0;
    }

    // Expected write size not greater than 0 or smaller than the current file content
    if ((fileSize <= 0) || (fileSize < curFileSize))
    {
        TRACE_LOG("init file:%s[%d/%d] init para err!", fileName, curFileSize, fileSize);
        return RET_ERR_PARAMETER;
    }

    // Apply storage space
    srcBuf = (unsigned char *)Lib_malloc(fileSize);
    if (srcBuf == NULL)
    {
        TRACE_LOG("init file:%s[%d/%d] malloc failed!", fileName, curFileSize, fileSize);
        return RET_ERR_OUT_OF_MEMORY;
    }

    // Initialize storage space
    if (DefaultInit != NULL)
    {
        DefaultInit(srcBuf, fileSize);
    }
    else
    {
        memset(srcBuf, 0xFF, fileSize);
    }

    // file already exist
    if (curFileSize > 0)
    {
        // Get current file data
        if (ReadFile(fileName, 0, srcBuf, curFileSize) <= 0)
        {
            TRACE_LOG("read file:%s[%d/%d] failed!", fileName, curFileSize, fileSize);
            Lib_free(srcBuf);
            return RET_ERR;
        }

        // Delete old file
        Lib_FileRemove(fileName);
    }

    // Write data to the new file
    if (fileSize != WriteFile(fileName, 0, srcBuf, fileSize))
    {
        TRACE_LOG("write file:%s[%d/%d] failed!", fileName, curFileSize, fileSize);
        Lib_free(srcBuf);
        return RET_ERR;
    }

    TRACE_LOG("write file:%s[%d/%d] success!", fileName, curFileSize, fileSize);
    Lib_free(srcBuf);

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name      : GetPwrParam
// Function  : Get power management related parameters
// Parameter : ucId:Parameter ID
// Return    : Parameter value
// Desc      :
//-----------------------------------------------------------------------------
unsigned char GetPwrParam(E_INFO ucId)
{
    unsigned char aucValue = 0;
    unsigned long ulInfoLen = 1;

    if (ucId < E_POWER_OFF_TIME || (ucId > E_BLACKLIGHT_TIME))
    {
        return 0;
    }

    if (RET_OK != Lib_GetInfo(ucId, &aucValue, &ulInfoLen))
    {
        switch(ucId)
        {
        case E_POWER_OFF_TIME:
            return SDK_TIME_MIN_POWER_OFF_DEF;

        case E_SLEEP_TIME:
            return SDK_TIME_SEC_SLEEP_DEF;

        case E_BLACKLIGHT_TIME:
            return SDK_TIME_BACKLIGHT_LCD_DEF;
        default:
            return 0;
        }
    }

    return aucValue;
}

