/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : Idle.h
 *  Version      :
 *  Author       :
 * Description   : device system status management
 **********************************************************************************************************/

#ifndef _IDLE_H_
#define _IDLE_H_

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

unsigned char DispMainTimeGet(void);
void DispMainTimeSet(unsigned char mode);
void KbLedSetDelayOff(unsigned short usDelaySec);
void KbOtherQueryOp(void);
void DevFactoryCheck(void);
void BeepOn(void);

#endif

