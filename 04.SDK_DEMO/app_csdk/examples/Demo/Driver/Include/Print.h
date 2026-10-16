/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : print.h
 *  Version      :
 *  Author       :
 * Description   : Printing function management
 **********************************************************************************************************/

#ifndef __PRINT_H__
#define __PRINT_H__

#include "LibApi.h"

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

// Printer font size support
#define FONT8                           8
#define FONT16                          16
#define FONT18                          18
#define FONT24                          24
#define FONT32                          32
#define FONT48                          48
#define FONT64                          64
#define FONT_MAX                        384

// printer is out of paper
#define RET_ERR_TPH_NO_PAPER              (-10)

// Reverse printing
#define PRINT_REVERSE                   0x10
// Character border
#define PRINT_BORDER                    0x20

#define PT_BORDER_WIDTH                 3       // Print Border Width

#define LTP_MAX_DOT                     384
#define LTP_MAX_WORD                    (LTP_MAX_DOT/32)
#define LTP_MAX_BYTE                    (LTP_MAX_DOT/8)
#define TPH_BUF_MAX                     32

#define CANVAS_WIDTH                    LTP_MAX_DOT

#define PRINTER_HW_MODE                 0
#define PRINTER_OTHER_MODE              1

#define TYPE_NULL_ROW                   0
#define TYPE_PIC                        1
#define TYPE_SYMBOL                     2
#define TYPE_OTHER                      3


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

int Lib_PrintStr(unsigned char ucMode, unsigned char *pucBuf, unsigned long uiLen);
int PrintBmp(unsigned short usStartX, char *pscFileName);
int PrintReceipt(void);

#endif

