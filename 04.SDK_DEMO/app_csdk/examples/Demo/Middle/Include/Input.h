/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : Input.h
 *  Version      :
 *  Author       :
 * Description   : Implement of input apis related letters and special characters on menu
 **********************************************************************************************************/

#ifndef __INPUT_H__
#define __INPUT_H__

#include "LibTypedef.h"


//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

// Input Mode
// When in password input mode, a "*" is displayed on the menu; in normal mode display the input information
#define INPUT_PWD_MODE          BIT0
// Waiting for input of required information length
#define INPUT_MATCH_LEN         BIT1
// Allow input of zero length in Input-MATCH-LEN mode
#define INPUT_EN_ZERO_LEN       BIT2
// Allow input of zero length in Input-MATCH-LEN mode
#define INPUT_DIGIT_MODE        BIT3
// Input number 0 or 1 mode, only allowing input of number 0 or 1
#define INPUT_ZERO_ONE          BIT4
// Automatically exit when the input data reaches the maximum length
#define INPUT_AUTO_RETURN       BIT5


#define FONT_HZ_WIDTH           24
#define FONT_ASCII_WIDTH        12

// Display height of each row
#define ROW_HEIGTH              32
// The maximum number of characters displayed in one line is 21
#define LCD_LINE_CHAR_ML        20


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

signed short KbWaitZM(unsigned char *pucKey, int iTimeout, unsigned char ucClewY);
signed short KbWaitAsc(unsigned char *pucKey, int iTimeout, unsigned char ucClewY);


#endif //__INPUT_H__

