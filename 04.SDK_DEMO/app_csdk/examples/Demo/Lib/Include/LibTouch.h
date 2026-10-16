//-----------------------------------------------------------------------------
// @file    : LibTouch.h
// @brief   : Touch Screen related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_TOUCH_H__
#define __LIB_TOUCH_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef enum
{
    TP_KEY_NONE = 0,

    TP_KEY_0 = '0',     // 0
    TP_KEY_1,           // 1
    TP_KEY_2,           // 2
    TP_KEY_3,           // 3
    TP_KEY_4,           // 4
    TP_KEY_5,           // 5
    TP_KEY_6,           // 6
    TP_KEY_7,           // 7
    TP_KEY_8,           // 8
    TP_KEY_9,           // 9

    TP_KEY_OK           = 0x01,
    TP_KEY_CANCEL       = 0x02,
    TP_KEY_CLEAR        = 0x03,
    TP_KEY_BACKSPACE    = 0x04,
    TP_KEY_F1           = 0x05,
    TP_KEY_F2           = 0x06
} TP_KEY;

typedef struct _TOUCH_AREA
{
    unsigned short x;
    unsigned short y;
    unsigned short w;
    unsigned short h;
} TP_TOUCH_AREA;

typedef struct _TOUCH_KEY
{
    TP_KEY keyValue;
    TP_TOUCH_AREA touchArea;
} TP_TOUCH_KEY;

//-----------------------------------------------------------------------------
// Name    : Lib_TpGetCoordinates
// Func    : Get the coordinates of the touch point
// Paras   : pusX[in/out],  output x-axis coordinate
//           pusY[in/out],  output y-axis coordinate
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_TpGetCoordinates(unsigned short *pusX, unsigned short *pusY);

//-----------------------------------------------------------------------------
// Name    : Lib_TpGetESignData
// Func    : Get electronic signature data
// Paras   : pucOut[in/out],          output electronic signature data
//           pscFeatureCode[in/out],  output feature code
//           iTimeout[in],            input timeout,unit is millisecond
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_TpGetESignData(unsigned char *pucOut, char *pscFeatureCode, int iTimeout);

//-----------------------------------------------------------------------------
// Name    : Lib_TpCalibrate
// Func    : Calibrate Touch Screen
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_TpCalibrate(void);

//-----------------------------------------------------------------------------
// Name    : Lib_TpGetKey
// Func    : Get the key value touched
// Paras   : keyList[in],  input TP_TOUCH_KEY
//           keyNum[in],   input key number
// Ret     : return key ID
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_TpGetKey(TP_TOUCH_KEY *keyList, unsigned char keyNum);

#endif