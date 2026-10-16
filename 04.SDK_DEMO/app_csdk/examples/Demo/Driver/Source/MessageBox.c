/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : MessageBox.c
 *  Version      :
 *  Author       :
 * Description   : App main function
 **********************************************************************************************************/

#include "MessageBox.h"
#include "Common.h"
#include "Key.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"

//-----------------------------------------------------------------------------
// Name    : MessageBox
// Func    : Message prompt box
// Paras   : pcDisp[in],   Message box display content, can display multiple lines, use "\ n" to wrap
//           uiType[in],   uiType message box type, such as alignment | character color | message box type | response button
//                         Alignment: MB-LEFT (left), MB-RIGHT (right), MB-CENTER (center)
//                         Character colors: MB-COLOR-RED (red), MB-COLOR_GREEN (green), MB-COLOR_BLUE (blue)
//                         Message box types: MB-ICONERROR (error box), MB-ICONQUESTION (question box), MB-ICONINFORMATION (information box)
//                         Response buttons: MB-ANYKEY (any key), MB_YESNO (confirm key and cancel key)
//           title[in],   the messageBox title
//           iTimeDly[in], message box displays time in milliseconds
// Ret     : Return key values or timeout
// Desc    :
//-----------------------------------------------------------------------------
int MessageBox(char* pcDisp, unsigned int uiType, int iTimeDly)
{
    return LvUiMessageBox(pcDisp, uiType, NULL, iTimeDly);
}

