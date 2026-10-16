/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : MessageBox.c
 *  Version      :
 *  Author       :
 * Description   : App main function
 **********************************************************************************************************/

#ifndef  __MESSAGEBOX_H__
#define  __MESSAGEBOX_H__

#include "LibApi.h"
#include "Drv.h"

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

// Response keys: MB-ANYKEY (any key), MB_YESNO (confirm key and cancel key)
#define MB_NOKEY                        0x00// Key press No need
#define MB_OK                           0x01// Not yet used
#define MB_YESNO                        0x02// Not yet used
#define MB_ANYKEY                       0x04// Any key returns the key value
#define MB_KEY0KEY1                     0x08// Not yet used

// Not used yet: Message box types: MB-ICONERROR (error box), MB-ICONQUESTION (question box), MB-ICONINFORMATION (information box)
#define MB_ICONERROR                    0x00000010
#define MB_ICONQUESTION                 0x00000020
#define MB_ICONINFORMATION              0x00000040

// Alignment: MB-LEFT (left), MB-RIGHT (right), MB-CENTER (center)
#define MB_LEFT                         0x00000100
#define MB_RIGHT                        0x00000200
#define MB_CENTER                       0x00000400

// Not used yet: Character colors: MB-COLOR-RED (red), MB-COLOR_GREEN (green), MB-COLOR-BLUE (blue)
#define MB_COLOR_RED                    0x00001000
#define MB_COLOR_GREEN                  0x00002000
#define MB_COLOR_BLUE                   0x00004000
#define MB_COLOR_YELLOW                 0x00008000

#define MB_TITLE                        0x00010000

#define IDYES                           6
#define IDNO                            7

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

int MessageBox(char* pcDisp, unsigned int uiType, int iTimeDly);


#endif

