//-----------------------------------------------------------------------------
// @file    : LibCamera.h
// @brief   : Camera-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_CAMERA_H__
#define __LIB_CAMERA_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#define CAM_LED_ENABLE                  (1)
#define CAM_LED_DISABLE                 (0)

//-----------------------------------------------------------------------------
// Name    : Lib_CamInit
// Func    : initialize the camera
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_CamInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_CamOpen
// Func    : open the camera
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_CamOpen(void);

//-----------------------------------------------------------------------------
// Name    : Lib_CamClose
// Func    : close the camera
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_CamClose(void);

//-----------------------------------------------------------------------------
// Name    : Lib_CamGetResult
// Func    : Get the scan result
// Paras   : pucBuf[in/out],    input data buffer and output the scan result 
//           usMaxLen[in],  input the length of buffer
// Ret     : return the length of result, 
//           The length more than 0 indicates successful
//           The length less than or equal to 0 indicates failure.
// Desc    :
//-----------------------------------------------------------------------------
int Lib_CamGetResult(unsigned char *pucBuf, unsigned short usMaxLen);

//-----------------------------------------------------------------------------
// Name    : Lib_CamDisp
// Func    : enable or disable the camera screen display function
// Paras   : enable[in],    1: enable,0:disable
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_CamDisp(unsigned char enable);

//-----------------------------------------------------------------------------
// Name    : Lib_CamLed
// Func    : Open or close the led of camera
// Paras   : ucMode[in],    1:open,0:close
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_CamLed(unsigned char ucMode);

#endif
