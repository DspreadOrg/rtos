/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Key.h
 *  Version      :
 *  Author       :
 * Description   : Used for processing keyboard related functions
 **********************************************************************************************************/

#ifndef __KEY_H__
#define __KEY_H__

#include "LibTypedef.h"


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// Key input status return value
#define KB_RETURN_OK                    0
#define KB_RETURN_TIMEOUT               (-1)
#define KB_RETURN_CANCEL                (-2)
#define KB_RETURN_ERR                   (-3)

// numeral keys 0-9
#define KB_DIGIT_ZERO                   '0' // numeral key0
#define KB_DIGIT_ONE                    '1' // numeral key1
#define KB_DIGIT_TWO                    '2' // numeral key2
#define KB_DIGIT_THREE                  '3' // numeral key3
#define KB_DIGIT_FOUR                   '4' // numeral key4
#define KB_DIGIT_FIVE                   '5' // numeral key5
#define KB_DIGIT_SIX                    '6' // numeral key6
#define KB_DIGIT_SEVEN                  '7' // numeral key7
#define KB_DIGIT_EIGHT                  '8' // numeral key8
#define KB_DIGIT_NINE                   '9' // numeral key9

// Function keys
#define KB_UP                           'U' // Up Key/F1
#define KB_DOWN                         'D' // down Key/F2
#define KB_MENU                         'M' // menu key/F3
#define KB_FUN                          'F' // Function keys
#define KB_CLEAR                        'C' // Clear key
#define KB_ENTER                        'E' // Confirm key
#define KB_CANCEL                       'Q' // escape key
#define KB_ZM                           'Z' // Alphabetic key
#define KB_POWER_OFF                    'P' // Power key
#define KB_POWER_AND_ENTER              'S' // power+enter Enter self check function

#define KB_TIMEOUT_SIGNAL               0xFE    // keys interrupted
#define KB_NULL                         0xFF    // No key presses

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

unsigned char KbReadKey(void);
void KbSetKey(unsigned char ucKey);
signed short KbWaitingKey(unsigned char *pucKey, int iTimeout);

#endif

