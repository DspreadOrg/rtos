/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : Barcode.h
 *  Version      :
 *  Author       :
 * Description   : Scan code transactions
 **********************************************************************************************************/
#ifndef __BARCODE_TRADE_H__
#define __BARCODE_TRADE_H__

#include "LibApi.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

short QrCamClose(void);
short UIOnlineBSQrcodeTrade(int iTimeout, unsigned long ulOpCode);
void CamEnableControl(unsigned char Flg);

#endif /*__BARCODE_TRADE_H__*/

