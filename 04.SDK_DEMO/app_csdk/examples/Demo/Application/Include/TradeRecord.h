/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : TradeRecord.h
 *  Version      :
 *  Author       :
 * Description   : Transaction record related functions
 **********************************************************************************************************/

#ifndef __RECORD_EXTEND_H__
#define __RECORD_EXTEND_H__

#include "LibApi.h"

//---------------------------------------------------------------------------
// macro definition
//---------------------------------------------------------------------------
// Maximum number of transaction records
#define RECORD_NUM_MAX    (200)

// Maximum transaction record length
#define RECORD_LEN_MAX	  (1024-2)

// Transaction records
typedef struct
{
    // Transaction type code
    unsigned long ulOpCode;
    // Transaction status identifier
    unsigned long ulTag;
    // Card types: magnetic stripe 1, contact card 2, contactless card 3, manual input card number 5
    unsigned char ucCardType;

    // Mainly used for quick retrieval of transaction records
    // Transaction serial number,3bcd
    unsigned char aucTraceNoH[3];
    // Transaction time
    unsigned char aucDateH[20];
    // AUTHORIZE CODE lv 6asc
    unsigned char aucAuthCodeH[7];
    // Amount 6bcd
    unsigned char aucSum[6];
    // Card No lv 1 byte length+10 bytes bcd
    unsigned char aucCardNo[11];
    // 4B reserved bytes
    unsigned char NotUsed[4];
}ST_TRADE_REC;

//-----------------------------------------------------------------------------
// variable declaration
//-----------------------------------------------------------------------------
extern ST_TRADE_REC g_trade_info;

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------
short ApiInitRecord(void);
short ApiGetRecNum(void);
int ApiGetRecByIndex(ST_TRADE_REC *pstTradeRec, int index);
short ApiAddRec(ST_TRADE_REC *pstTradeRec);

#endif

