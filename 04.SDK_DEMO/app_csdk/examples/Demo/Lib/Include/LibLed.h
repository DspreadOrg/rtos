//-----------------------------------------------------------------------------
// @file    : LibLed.h
// @brief   : LED functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_LED_H__
#define __LIB_LED_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

//LED ID
typedef enum     
{
    LED_QR,         // camer exposure light
    LED_TIP1 = 1,   // The first light in the front row of the POS
    LED_TIP2,       // The second light in the front row of the POS
    LED_TIP3,       // The third light in the front row of the POS
    LED_TIP4,       // The fourth light in the front row of the POS
}LED_ID;

//-----------------------------------------------------------------------------
// Name    : Lib_LedControl
// Func    : Control Led lights
// Paras   : ucLedNo[in],  input Number of LED lights
//           ucMode[in],   input mode,0: OFF, 1: ON
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_LedControl(LED_ID ucLedNo, unsigned char ucMode); 

#endif