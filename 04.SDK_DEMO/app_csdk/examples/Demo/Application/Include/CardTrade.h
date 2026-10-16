/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : CradTrade.h
 *  Version      :
 *  Author       :
 * Description   : Card transactions
 **********************************************************************************************************/

#ifndef __CARD_TRADE_H__
#define __CARD_TRADE_H__

#include "LibApi.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

short UIOnlineTrade(int iTimeout, unsigned long ulOpCode);
short BatTestOnlineTrade(int iTimeout);

short UIOnlineTransaction(void);

#endif /*__CARD_TRADE_H__*/

