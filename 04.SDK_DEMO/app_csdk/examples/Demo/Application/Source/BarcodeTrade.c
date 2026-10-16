/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Barcode.c
 *  Version      :
 *  Author       :
 * Description   : Scan code transactions
 **********************************************************************************************************/
#include "BarcodeTrade.h"
#include "Language.h"
#include "Common.h"
#include "Drv.h"
#include "WifiConfig.h"
#include "TradeRecord.h"
#include "MessageBox.h"
#include "ParamManage.h"
#include "Key.h"
#include "Idle.h"
#include "Power.h"
#include "Trade.h"
#include "Menu.h"
#include "Communication.h"
#include "api_lvgl.h"
#if HW_TPH
#include "Print.h"
#endif

// After scan successful LED start tick
unsigned long g_ulQrCamSLedStartTime = 0;

//-----------------------------------------------------------------------------
// Name    : QrCamOpen
// Func    : open the camera
// Paras   : iTimeout[in],   timeout value, in milliseconds
// Ret     : Successfully returned RET_OK
// Desc    :
//-----------------------------------------------------------------------------
short QrCamOpen(void)
{
    Lib_CamLed(CAM_LED_ENABLE);
    Lib_CamInit();
    Lib_CamOpen();
    Lib_CamDisp(TRUE);
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : QrCamClose
// Func    : close camera
// Paras   :
// Ret     :  Successfully returned RET_OK
// Desc    :
//-----------------------------------------------------------------------------
short QrCamClose(void)
{
    Lib_CamLed(CAM_LED_DISABLE);
    Lib_CamClose();
    Lib_CamDisp(FALSE);
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : QrCamScanSuccessLedCtrl
// Func    : LED control after successful camera scanning
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void QrCamScanSuccessLedCtrl(unsigned char ucStatus)
{
    if (ucStatus == 0)
    {
        g_ulQrCamSLedStartTime = 0;
        Lib_CamLed(CAM_LED_DISABLE);
    }
    else
    {
        Lib_CamLed(CAM_LED_ENABLE);
        g_ulQrCamSLedStartTime = Lib_SysGetTimer();
    }
}

static unsigned char prvCheckQrRlt(const char *data, char *qrCode, unsigned long uiDatalen, unsigned long *OutLen)
{
    //Increase the handling of buff length to avoid overflow changes by gx 20201203
    unsigned long uiOffset;
    if (data == NULL || qrCode == NULL)
    {
        return FALSE;
    }

    // Scan result format"+QCAMPRES: 0,18,8,XXXXXXXX"
    // +QCAMPRES: 0 :fixed format, 18 is the scanning format, and 8 is the length of the scanning result,
    // XXXXXXXX is the actual scan result
    uiOffset = 0;
    char *qrhead = "+QCAMPRES: 0,";
    char *qrPre = strstr(data, qrhead);
    if (qrPre == NULL)
    {
        return FALSE;
    }

    char *qrStart = qrPre + strlen(qrhead);
    uiOffset += strlen(qrhead);
    // Find the remaining first comma
    char *firstSeq = strstr(qrStart, ",");
    if (firstSeq == NULL)
    {
        TRACE("QR data %s", data);
        return FALSE;
    }
    uiOffset += firstSeq - qrStart+1;
    // Find the remaining second comma
    char *secSeq = strstr(firstSeq + 1, ",");
    if (secSeq == NULL)
    {
        return FALSE;
    }
    uint32_t len = secSeq - firstSeq;
    char lenText[8] = {0};
    if (len > 8)
    {
        return FALSE;
    }

    memcpy(lenText, firstSeq + 1, len);
    uint32_t copyLen = atoi(lenText);

    uiOffset += secSeq - firstSeq;
    // TRACE("uiOffset = %d",uiOffset);
    // TRACE("copyLen = %d",copyLen);
    // TRACE("qrCodeLen = %d",qrCodeLen);
    if(copyLen > uiDatalen-uiOffset)
    {
        FUN_IN_ERR;
        return FALSE;
    }

    *OutLen = copyLen;
    memmove(qrCode, secSeq + 1, copyLen);
    qrCode[copyLen] = 0;
    return TRUE;
}

short QrCamScan(unsigned char *buf, unsigned short *plen, int iTimeout)
{
    short ret = RET_ERR;
    short sRc;
    unsigned long ulTick = Lib_SysGetTimer() + iTimeout;
    unsigned long uiCodeLen = 0;

    if ((NULL == buf) || (NULL == plen) || (0 == (*plen)) || (19 > *plen))
    {
        // The length of the incoming cache is 0
        return RET_ERR;
    }

    while (1)
    {
        sRc = Lib_CamGetResult(buf, *plen);
        TRACE("QrCamScan = %d", sRc);
        if (sRc > 0)
        {
            if (FALSE == prvCheckQrRlt((char*)buf, (char*)buf, sRc, &uiCodeLen))
            {
                // Scan code parsing exception, restart scan code
                Lib_CamOpen();
                continue;
            }

            // *plen = strlen(buf);
            *plen = uiCodeLen;

            TRACE("*plen = %d", *plen);
            ret = RET_OK;
            QrCamScanSuccessLedCtrl(1);

            BeepOn();
            break;
        }
        // Cancel key to exit
        //if (UI_INPUT_CANCEL == UIWaitKey(100, 0))
        if (KB_CANCEL == Lib_KbGetKey())
        {
            BeepOn();
            return UI_INPUT_CANCEL;
        }

        if (ulTick < Lib_SysGetTimer())
        {
            return RET_ERR_TIMEOUT;
        }

        Lib_DelayMs(1);
    }
    return ret;
}


//-----------------------------------------------------------------------------
// Name    : BarCodeScan
// Func    : Barcode scanning
// Paras   : iTimeout[in],   timeout value, in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short BarCodeScan(int iTimeout)
{
    unsigned char aucTemp[255];
    unsigned short wLen;

    LvUilCls();

    if(RET_OK != QrCamOpen())
    {
        QrCamClose();
        return RET_ERR;
    }

    Lib_LcdSetBrightLevel(8);

    wLen = sizeof(aucTemp);
    if(RET_OK == QrCamScan(aucTemp, &wLen, iTimeout))
    {
        BeepOn();
        aucTemp[wLen] = 0;
        UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);
        QrCamClose();
        extern ST_TEMINAL_PARAM g_teminal_param;
        Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);

        return RET_OK;
    }
    else
    {
        QrCamClose();
        Lib_LcdSetBrightLevel(g_teminal_param.ucBrightLevel);
        return RET_ERR;
    }
}

short OnlineBSQrTradeSetSendData(unsigned char *pucOut, unsigned short *pucOutLen)
{
    extern unsigned char payBuffer[271];
    *pucOutLen = sizeof(payBuffer);
    TRACE( "!!! tcp prepare buff size = %d\r\n", *pucOutLen);
    memcpy(pucOut, payBuffer, *pucOutLen);
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : OnlineBSQrTradeDealRecvData
// Func    : QR code consumption message processing
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short OnlineBSQrTradeDealRecvData(unsigned char *pucIn, unsigned short usInLen)
{
    TRACE("OnlineBSQrTradeDealRecvData");

    if (usInLen <= 0)
        return RET_ERR;

    for (int i=0; i<usInLen; i++)
    {
        TRACE("0x%02x", pucIn[i]);
    }
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : OnlineBSQrTradeDealRecvData
// Func    : QR code consumption message processing
// Paras   : iTimeout  : timeout value, in milliseconds
//           ulOpCode  :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIOnlineBSQrcodeTrade(int iTimeout, unsigned long ulOpCode)
{
    short sRC;
    unsigned char aucAmount[12+1] = { 0 };
    LVGL_RESULT result = { 0 };

    // Transaction initialization
    sRC = CupTradeInit(iTimeout, ulOpCode);
    if (RET_OK != sRC)
    {
        return sRC;
    }

    LvUilCls();

    // 1.Get input amount
    sRC = LvUiInputAmt((char *)aucAmount, 9, 60);
    if ( sRC < 0 )
    {
        return sRC;
    }

#if 1
    //2.Scan QR code, set QR code type WeChat, Alipay
    sRC =BarCodeScan(iTimeout);
    if (RET_OK != sRC)
    {
        return sRC;
    }
#endif

    LvUilCls();

    //2.Send 8583 packet
    sRC = CupOpTx8583Pack(OnlineBSQrTradeSetSendData, ulOpCode, iTimeout);
    if (RET_OK != sRC)
    {
        return sRC;
    }

    //3.Receive 8583 packet and process it
    sRC = CupOpRx8583Pack(iTimeout, ulOpCode, OnlineBSQrTradeDealRecvData);
    if (RET_OK != sRC)
    {
        return sRC;
    }

    CupOpResultDeal(ulOpCode, iTimeout);

    char aucTemp[40],aucBuf[128];

    // Right aligned amount becomes a 6-byte BCD
    memset(aucTemp, 0, sizeof(aucTemp));
    SetCash(aucAmount, strlen((const char *)aucAmount), (unsigned char*)aucTemp);
    memcpy(g_trade_info.aucSum, aucTemp, 6 );

    // Display transaction amount
    memset(aucTemp, 0, sizeof(aucTemp));
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, g_trade_info.aucSum,  6);
    BcdSum2AscSum((unsigned char *)aucBuf, (char*)aucTemp);
    memset(aucBuf, 0, sizeof(aucBuf));
#if HW_TPH
    sprintf(aucBuf, "%s\n%s%s\n%s", TIP_QR_TRADE_SUCC, TIP_AMOUNT, aucTemp, TIP_PRINTING);
#else
    sprintf(aucBuf, "%s\n%s%s", TIP_QR_TRADE_SUCC, TIP_AMOUNT, aucTemp);
#endif
    result.img = SUCCESS_IMG;
    result.title = TIP_SUCCESS;
    result.msg = aucBuf;
    result.fontColor = LV_COLOR_GREEN;
    result.timeout = 10000;

    LvUiShowResult(&result);

    // Add transaction records
    ApiAddRec(&g_trade_info);

    //AudioPlay(AUDIO_STR_AND_AMOUNT, (unsigned char *)aucTemp);
    // Press the key to exit
#if HW_TPH
    PrintReceipt();
#endif

    return sRC;
}


unsigned char Flg_NeedCamer = 0;
void CamEnableControl(unsigned char Flg)
{
    if( Flg == 1) //
    {
        Flg_NeedCamer = 1;
    }
    else if( Flg == 0)
    {
        // close camera
        QrCamClose();
        // Clear the sign of re-poweron
        Flg_NeedCamer = 0;
    }
}

unsigned char GetCamEnableStatus()
{
    return Flg_NeedCamer;
}

