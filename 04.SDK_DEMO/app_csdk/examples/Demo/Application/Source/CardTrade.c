/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : CradTrade.c
 *  Version      :
 *  Author       :
 * Description   : Card transactions
 **********************************************************************************************************/

#include "Trade.h"
#include "Language.h"
#include "LibApi.h"
#include "Trade.h"
#include "Common.h"
#include "Card.h"
#include "Drv.h"
#include "TradeRecord.h"
#include "MessageBox.h"
#include "Menu.h"
#include "Power.h"
#include "LibEmvKernel.h"
#include "Communication.h"
#include "api_lvgl.h"
#if HW_TPH
#include "print.h"
#endif

// 8583 message data for example transactions
unsigned char payBuffer[271] =
{
    // length
    0x01, 0x0D,
    0x60, 0x00, 0x06, 0x00, 0x00, 0x60, 0x31, 0x00, 0x31, 0x01, 0x00,
    // Field [00], length [004]: [00200]
    0x02, 0x00,
    0x70, 0x24, 0x06, 0xC0, 0x20, 0xC0, 0x8A, 0x11, 0x19,
    // Field [02], length [019]: [6217002870039460231] Card number
    0x62, 0x17, 0x00, 0x28, 0x70, 0x03, 0x94, 0x60, 0x23, 0x10, 0x00,
    // Field [03], length [006]: [00000]
    0x00, 0x00, 0x00,
    // Field [04], length [012]: [000000 00011] Amount
    0x00, 0x00, 0x00, 0x00, 0x11,
    // Field [11], length [006]: [000201]
    0x00, 0x02, 0x01,
    0x24, 0x04, 0x07, 0x20,
    0x00, 0x01, 0x00, 0x12, 0x37, 0x62, 0x17, 0x00, 0x28, 0x70,
    0x03, 0x94, 0x60, 0x23, 0x1D, 0x85, 0x46, 0xA1, 0x70, 0x1C,
    0xF9, 0xCB, 0xAB, 0x00, 0x33, 0x30, 0x30, 0x31, 0x32, 0x33,
    0x31, 0x30, 0x31, 0x31, 0x37, 0x35, 0x33, 0x39, 0x39, 0x31,
    0x36, 0x39, 0x35, 0x33, 0x30, 0x31, 0x30, 0x31, 0x35, 0x36,
    0x06, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x45,
    0x9F, 0x26, 0x08, 0xC5, 0x29, 0xE3, 0xC5, 0x8E, 0xA0, 0x60,
    0x8C, 0x9F, 0x27, 0x01, 0x80, 0x9F, 0x10, 0x13, 0x07, 0x01,
    0x01, 0x03, 0xA0, 0x00, 0x00, 0x01, 0x0A, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0xA1, 0xB8, 0x3F, 0x9F, 0x37, 0x04,
    0x7E, 0x9D, 0xEE, 0x3A, 0x9F, 0x36, 0x02, 0x00, 0x5F, 0x95,
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x03, 0x21, 0x11,
    0x25, 0x9C, 0x01, 0x00, 0x9F, 0x02, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x11, 0x5F, 0x2A, 0x02, 0x01, 0x56, 0x82, 0x02,
    0x7C, 0x00, 0x9F, 0x1A, 0x02, 0x01, 0x56, 0x9F, 0x03, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x33, 0x03, 0x60,
    0xE9, 0xC8, 0x9F, 0x34, 0x03, 0x00, 0x00, 0x00, 0x9F, 0x35,
    0x01, 0x22, 0x9F, 0x1E, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x84, 0x08, 0xA0, 0x00, 0x00, 0x03, 0x33,
    0x01, 0x01, 0x01, 0x9F, 0x09, 0x02, 0x00, 0x30, 0x9F, 0x41,
    0x04, 0x00, 0x00, 0x00, 0x06, 0x00, 0x12, 0x22, 0x00, 0x00,
    0x36, 0x00, 0x06, 0x45, 0x44, 0x36, 0x32, 0x33, 0x38, 0x30,
    0x46,
};


//-----------------------------------------------------------------------------
// Name    : OnlineTradeSetSendData
// Func    : Consumer message packaging
// Paras   : pucOut[in],    Output Data Cache
// Ret     : pucOutLen[in], Input cache size and output data length
// Desc    : Output data: 2-byte length+8583 data, sample code, cannot be directly
//           used on commercial PSP systems
//-----------------------------------------------------------------------------
short OnlineTradeSetSendData(unsigned char *pucOut, unsigned short *pucOutLen)
{
	unsigned char aucBuf[20];
	unsigned char aucTimeBcd[7];
	unsigned char aucTimeStr[12];

    *pucOutLen = sizeof(payBuffer);
	TRACE( "!!! tcp prepare buff size = %d\r\n", *pucOutLen);
	memcpy(pucOut, payBuffer, *pucOutLen);

	// MERCHANT NAME
	memcpy(g_trade_info.aucCardNo, "01234567890", 11);

	// Transaction Date
	Lib_GetDateTime(aucTimeBcd);
	Lib_Bcd2Asc(aucTimeBcd, (char *)aucTimeStr, 6);
	memset(aucBuf, 0, sizeof(aucBuf));
	memcpy(aucBuf, "2020-01-01 00:00:00", strlen("2020-01-01 00:00:00"));
	aucBuf[2] = aucTimeStr[0];
	aucBuf[3] = aucTimeStr[1];
	aucBuf[5] = aucTimeStr[2];
	aucBuf[6] = aucTimeStr[3];
	aucBuf[8] = aucTimeStr[4];
	aucBuf[9] = aucTimeStr[5];
	aucBuf[11] = aucTimeStr[6];
	aucBuf[12] = aucTimeStr[7];
	aucBuf[14] = aucTimeStr[8];
	aucBuf[15] = aucTimeStr[9];
	aucBuf[17] = aucTimeStr[10];
	aucBuf[18] = aucTimeStr[11];
	memcpy(g_trade_info.aucDateH, aucBuf, 20);
	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : OnlineTradeDealRecvData
// Func    : Consumer message processing
// Paras   : pucIn[in],    input message Data
// Ret     : usInLen[in],  input message Data length
// Desc    : The input data is a 2-byte length+8583 data packet
//-----------------------------------------------------------------------------
short OnlineTradeDealRecvData(unsigned char *pucIn, unsigned short usInLen)
{
    unsigned char aucOnlineAfterBuf[100];
    int iBufLen = 0;
    unsigned char aucField39[2];

	TRACE("OnlineTradeDealRecvData");
    TRACEBUF(pucIn, usInLen);

    memcpy(aucField39, "\x30\x30", 2);

	aucOnlineAfterBuf[iBufLen++] = 0x8A;
    aucOnlineAfterBuf[iBufLen++] = 0x02;
    aucOnlineAfterBuf[iBufLen++] = aucField39[0];
    aucOnlineAfterBuf[iBufLen++] = aucField39[1];

	// Online successful,issuing bank script processing
	Lib_EmvAfterOnlineProcess(1, aucOnlineAfterBuf, iBufLen);

	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : UIOnlineTrade
// Func    : Online consumption
// Paras   : iTimeout[in], Timeout value, in milliseconds
// Ret     : ulOpCode[in], Transaction type code
// Desc    :
//-----------------------------------------------------------------------------
short UIOnlineTrade(int iTimeout, unsigned long ulOpCode)
{
    short sRC;
    unsigned char aucAmount[12+1] = { 0 };
    LVGL_RESULT result = { 0 };
    unsigned char aucTemp[40],aucBuf[128];

    FUN_IN;

    if ((TT_CONSUMPTION != ulOpCode) && (TT_ZH_CONTACT_CONSUMPTION != ulOpCode) && (TT_ZH_NFC_CONTACT_CONSUMPTION != ulOpCode))
    {
        return RET_ERR;
    }

    // Transaction initialization
    sRC = CupTradeInit(iTimeout, ulOpCode);
    if (RET_OK != sRC)
    {
        return sRC;
    }

    LvUilCls();

    sRC = LvUiInputAmt((char *)aucAmount, 9, 60);
    if ( sRC < 0 )
    {
        return sRC;
    }

    memset(&gstTransData, 0, sizeof(gstTransData));
    SetCash(aucAmount, strlen((const char *)aucAmount), gstTransData.stTransInfo.aucAmount);

    sRC = SearchCard("");
    if (RET_ERR == sRC )
    {
        FUN_IN_ERR;
        Lib_IccClose(ICC_SLOT_EMV_IC);
        return sRC;
    }

    // Set server
    //SetServerOpt(SERVER_OPT_POSP_MAIN);
    // Transaction message sending
	sRC = CupOpTx8583Pack(OnlineTradeSetSendData, ulOpCode, iTimeout);
	if (RET_OK != sRC)
	{
        Lib_IccClose(ICC_SLOT_EMV_IC);
		return sRC;
	}

    // Receive and process transaction messages
    sRC = CupOpRx8583Pack(iTimeout, ulOpCode, OnlineTradeDealRecvData);
    Lib_IccClose(ICC_SLOT_EMV_IC);
    if (RET_OK != sRC)
    {
        // Is it necessary to reversal immediately
        IsFastRevert(iTimeout, ulOpCode);
        return sRC;
    }

    // Result processing after successful transaction
    CupOpResultDeal(ulOpCode, iTimeout);

    // Right aligned amount becomes a 6-byte BCD
    memset(aucTemp, 0, sizeof(aucTemp));
    SetCash(aucAmount, strlen((const char *)aucAmount), aucTemp);
    memcpy(g_trade_info.aucSum, aucTemp, 6);

    // Display transaction amount
    memset(aucTemp, 0, sizeof(aucTemp));
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, g_trade_info.aucSum, 6);
    BcdSum2AscSum(aucBuf, (char*)aucTemp);
    memset(aucBuf, 0, sizeof(aucBuf));
#if HW_TPH
    sprintf((char *)aucBuf, "%s\n%s%s $\n%s",TIP_TRADE_OK, TIP_AMOUNT, aucTemp, TIP_PRINTING);
#else
    sprintf((char *)aucBuf, "%s\n%s%s $",TIP_TRADE_OK, TIP_AMOUNT, aucTemp);
#endif
    result.img = SUCCESS_IMG;
    result.title = TIP_SUCCESS;
    result.msg = (char *)aucBuf;
    result.fontColor = LV_COLOR_GREEN;
    result.timeout = 3000;

    // Add transaction records
	ApiAddRec(&g_trade_info);

    LvUiShowResult(&result);

	//AudioPlay(AUDIO_STR_AND_AMOUNT, (unsigned char *)aucTemp);
    // Press the key to exit
#if HW_TPH
	PrintReceipt();
#endif
	return sRC;
}

short BatTestOnlineTrade(int iTimeout)
{
	short sRC;
	unsigned long ulOpCode = TT_CONSUMPTION;

    // Transaction message sending
	sRC = CupOpTx8583Pack(OnlineTradeSetSendData, ulOpCode, iTimeout);
	if (RET_OK != sRC)
	{
		return sRC;
	}

    // Receive and process transaction messages
	sRC = CupOpRx8583Pack(iTimeout, ulOpCode, OnlineTradeDealRecvData);
	if (RET_OK != sRC)
	{
        // Is it necessary to reversal immediately
        IsFastRevert(iTimeout, ulOpCode);
		return sRC;
	}

	return sRC;
}


//-----------------------------------------------------------------------------
// Name    : UIOnlineTransaction
// Func    : Hook function of online transaction process
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIOnlineTransaction(void)
{
    FUN_IN;
    return RET_OK;
}

