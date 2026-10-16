/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Logon.c
 *  Version      :
 *  Author       :
 * Description   : POS Logo-in
 **********************************************************************************************************/

#include "Logon.h"
#include "Input.h"
#include "Ui.h"
#include "Trade.h"
#include "Common.h"
#include "MessageBox.h"
#include "Language.h"
#include "Communication.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"

char *g_logonbuf = "\x00\x3C\x60\x00\x06\x00\x00\x60\x31\x00\x31\x01\x00\x08\x00\x00\x20\x00\x00\x00\xC0\x00\x12\x00\x02\x00\x33\x30\x30\x31\x32\x33\x31\x30\x31\x31\x37\x35\x33\x39\x39\x31\x36\x39\x35\x33\x30\x31\x30\x00\x11\x00\x00\x00\x36\x00\x40\x00\x03\x30\x31\x20";
short OnlineLogonSetSendData(unsigned char *pucOut, unsigned short *pucOutLen)
{
    *pucOutLen = 62;
    memcpy(pucOut, g_logonbuf, *pucOutLen);
    return RET_OK;
}

short OnlineLogonDealRecvData(unsigned char *pucIn, unsigned short usInLen)
{
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : UIOnlineLogon
// Func    : POS logon function
// Paras   : timeout   : timeout value, in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIOnlineLogon(int iTimeout)
{
    short sRC;

//    unsigned long TradeTime = 0;
    unsigned long ulOpCode = TT_LOGIN;

    LvUilCls();

    // 1. Transaction initialization
    sRC = CupTradeInit(iTimeout, ulOpCode);
    if (RET_OK != sRC)
    {
      return sRC;
    }

    // Set server
    //SetServerOpt(SERVER_OPT_POSP_MAIN);
    //TradeTime = Lib_SysGetTimer();
    // 2. Send 8583 packets
    sRC = CupOpTx8583Pack(OnlineLogonSetSendData, ulOpCode, iTimeout);
    if (RET_OK != sRC)
    {
        return sRC;
    }

    // 3. Receive 8583 packets and process them
    sRC = CupOpRx8583Pack(iTimeout, ulOpCode, OnlineLogonDealRecvData);
    if (RET_OK != sRC)
    {
        // Is it necessary to rectify immediately
        IsFastRevert(iTimeout, ulOpCode);
        return sRC;
    }

    // 4. Result processing after successful transaction
    CupOpResultDeal(ulOpCode, iTimeout);

    LvUiMessageBox(TIP_LOG_IN_SUCC, MB_CENTER|MB_COLOR_GREEN, NULL, 3000);

    return sRC;

}

