//-----------------------------------------------------------------------------
// @file    : LibTms.h
// @brief   : TMS upgrade related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_TMS_H__
#define __LIB_TMS_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

//Modules that need to be upgraded by TMS
typedef enum
{
    DL_GPRS = 0,                //GPRS module
    DL_WIFI,                    //WIFI module
    DL_SCHIP,                   //SE
    DL_FILE,                    //File system
    DL_FIRMWARE,                //firmware
} DOWNLOAD_MODULE;

int Lib_TmsParamInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_TmsDeleteSoft
// Func    : Delete TMS upgrade file
// Paras   : module[in],  input DOWNLOAD_MODULE
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_TmsDeleteSoft(DOWNLOAD_MODULE module);

//-----------------------------------------------------------------------------
// Name    : Lib_TmsDownloadRemote
// Func    : Download all Modules
// Paras   : netMode[in],         input network mode
//           module[in],          input DOWNLOAD_MODULE
//           timeout[in],         input timeout,unit is millisecond
//           updateFlag[in/out],  output update flag
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
short Lib_TmsDownloadRemote(NET_MODE netMode,DOWNLOAD_MODULE module,int timeout,unsigned char *updateFlag);

//-----------------------------------------------------------------------------
// Name    : Lib_TmsGetLibVersion
// Func    : Get TMS library version
// Paras   : paucLibVer[in/out],  output version
//           pusLen[in/out],      output version length
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_TmsGetLibVersion(char *paucLibVer, unsigned short *pusLen);

#endif