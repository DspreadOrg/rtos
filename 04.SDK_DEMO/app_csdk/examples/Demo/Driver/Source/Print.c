/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : print.c
 *  Version      :
 *  Author       :
 * Description   : Printing function management
 **********************************************************************************************************/
#include "Print.h"
#include "Language.h"
#include "LibSysapi.h"
#include "Drv.h"
#include "Trade.h"
#include "Common.h"
#include "TradeRecord.h"
#include "Menu.h"
#include "MessageBox.h"
#include "LanguageProcess.h"
#include "Key.h"
#include "api_lvgl.h"

#define LTP_MAX_DOT    384

// the maximum value of a single line of print data
#define PRINT_ONELINE_BUF_LEN   1024

/*****************************************************************************
** Description :  printer prints a string
** Parameters  :  unsigned char ucMode: print mode ALIGN_L: left align, ALIGN_M: center align, ALIGN_R: right align
                  unsigned char* pucBuf: print string
                  unsigned long uiLen : string length

** Return      :
** Author      :
** Remarks     :
*****************************************************************************/
int Lib_PrintStr ( unsigned char ucMode, unsigned char* pucBuf, unsigned long uiLen )
{
    if (uiLen == 0)
    {
        Lib_PrnPaperForward(3000);
        return RET_OK;
    }
    return Lib_PrnStrEx(ucMode, (char *)pucBuf, uiLen);
}

//print bitmap file
int PrintBmp(unsigned short usStartX, char *pscFileName)
{
    int fd;
    int ulBmpSize;
    unsigned char *pucBmpBuff;

    fd = Lib_FileOpen(pscFileName, LFS_O_RDONLY);
    if (fd < 0)
    {
        return RET_ERR;
    }
    ulBmpSize = Lib_FileSize(pscFileName);
    pucBmpBuff = (unsigned char *)Lib_malloc(ulBmpSize);
    if (pucBmpBuff == NULL)
    {
        Lib_FileClose(fd);
        return RET_ERR;
    }

    memset(pucBmpBuff, 0, ulBmpSize);
    Lib_FileSeek(fd, 0, LFS_SEEK_SET);
    Lib_FileRead(fd, pucBmpBuff, ulBmpSize);
    Lib_FileClose(fd);

    Lib_PrnBmp(usStartX, pucBmpBuff);
    Lib_free(pucBmpBuff);
    return RET_OK;
}

int PrintReceipt(void)
{
    int iRc = 0;
    // Print data cache
    unsigned char aucPrintBuf[PRINT_ONELINE_BUF_LEN + 1] = {0};
    unsigned short usLen = 0;
    unsigned char aucTimeBcd[7];
    unsigned char aucTimeStr[12];

    Lib_PrnOpen();

Again:
    Lib_PrnSetSpace(2);

    PrintBmp(0, "printlogo.bmp");

    Lib_PrnSetFont(FONT24, 2, 2);
    // Purchase order title
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)TIP_RECEIPT, strlen(TIP_RECEIPT));
    if (RET_OK != iRc)
    {
        return iRc;
    }

    Lib_PrnSetFont(FONT8, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);

    Lib_PrnSetFont(FONT24, 1, 1);

    // Merchant name
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));

    sprintf((char *)aucPrintBuf, "%s %s", TIP_MERCHANT_ID, "Wal_Mart");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char *)aucPrintBuf));
    // Merchant ID
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s %s", TIP_MERCHANT_NO, "804020101");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char *)aucPrintBuf));
    // terminal number
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s %s", TIP_TERMINAL_ID, "12345678");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char *)aucPrintBuf));
    // operator
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s %s", TIP_CASHIER_NO, "01");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char *)aucPrintBuf));

	// consumption pattern
	memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
	switch(g_trade_info.ulOpCode)
	{
	case TT_CONSUMPTION:
		sprintf((char *)aucPrintBuf, "%s %s", TIP_PAY_TYPE, TIP_PAY_CARD);
		break;
	case TT_QR_CONSUMPTION :
		sprintf((char *)aucPrintBuf, "%s %s", TIP_PAY_TYPE, TIP_PAY_SCAN);
		break;
	default:
		sprintf((char *)aucPrintBuf, "%s %s", TIP_PAY_TYPE, TIP_PAY_UNKNOW);
		break;
	}
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char *)aucPrintBuf));


	Lib_PrnSetFont(FONT24, 1, 1);
    // Amount
    unsigned char aucTemp[20];
	memset(aucTemp, 0, sizeof(aucTemp));
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
	memcpy(aucPrintBuf, g_trade_info.aucSum,  6);
	BcdSum2AscSum(aucPrintBuf, (char*)aucTemp);
	memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)TIP_TOTAL, strlen(TIP_TOTAL));
    sprintf((char *)aucPrintBuf, " %s", aucTemp);
    iRc = Lib_PrintStr(ALIGN_M, aucPrintBuf, strlen((char *)aucPrintBuf));

	Lib_PrnSetFont(FONT24, 1, 1);
    // DATE/TIME:
    Lib_GetDateTime(aucTimeBcd);
    Lib_Bcd2Asc(aucTimeBcd, (char *)aucTimeStr, 6);
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    memcpy(aucPrintBuf, "2020-01-01 00:00:00", strlen("2020-01-01 00:00:00"));
    aucPrintBuf[2] = aucTimeStr[0];
    aucPrintBuf[3] = aucTimeStr[1];

    aucPrintBuf[5] = aucTimeStr[2];
    aucPrintBuf[6] = aucTimeStr[3];

    aucPrintBuf[8] = aucTimeStr[4];
    aucPrintBuf[9] = aucTimeStr[5];

    aucPrintBuf[11] = aucTimeStr[6];
    aucPrintBuf[12] = aucTimeStr[7];

    aucPrintBuf[14] = aucTimeStr[8];
    aucPrintBuf[15] = aucTimeStr[9];

    aucPrintBuf[17] = aucTimeStr[10];
    aucPrintBuf[18] = aucTimeStr[11];

    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)TIP_DATE_TIME, strlen(TIP_DATE_TIME));
    iRc = Lib_PrintStr(ALIGN_M, aucPrintBuf, strlen((char *)aucPrintBuf));

    // merchant stub
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    usLen = strlen(TIP_PURCHASE);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)TIP_PURCHASE, usLen);
    if (RET_OK != iRc)
    {
        return iRc;
    }

    // Turn off the printer when printing is complete
    iRc = Lib_PrnClose();
    if (iRc == RET_ERR_TPH_NO_PAPER)
    {
        char aucDispBuffer[128] = {0};

        memset(aucDispBuffer, 0x00, sizeof(aucDispBuffer));
        sprintf((char *)aucDispBuffer, "%s\n %s",TIP_PRN_PAPER_OUT, TIP_PRN_PAPER_ADD);
        iRc = MessageBox(aucDispBuffer, MB_COLOR_YELLOW |MB_CENTER | MB_ICONQUESTION, 60000);
        if (iRc == KB_ENTER)
        {
            LvUiMessageBox(TIP_PRINTING, MB_COLOR_YELLOW |MB_CENTER | MB_ICONQUESTION, NULL, 500);
            Lib_PrnOpen();
            goto Again;
        }
    }

    return RET_OK;
}

int PrintTest(void)
{
    int iRc = 0;
    // Print data cache
    unsigned char aucPrintBuf[PRINT_ONELINE_BUF_LEN + 1] = {0};
    unsigned char aucPrintSeparator[] = "- - - - - - - - - - - - - - - - - -";

    Lib_PrnOpen();

Again:
    Lib_PrnSetSpace(8);

    PrintBmp(0, "printlogo.bmp");

    Lib_PrnSetFont(FONT32, 1, 1);
    // Purchase order title
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)"Testing", strlen("Testing"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)"POS Signing of purchase orders", strlen("POS Signing of purchase orders"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)"MERCHANT COPY", strlen("MERCHANT COPY"));
    if (RET_OK != iRc)
    {
        return iRc;
    }

    Lib_PrnSetSpace(10);
    Lib_PrnSetFont(FONT32, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, aucPrintSeparator, strlen((char*)aucPrintSeparator));
    if (RET_OK != iRc)
    {
        return iRc;
    }

    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"ISSUER Agricultural Bank of China", strlen("ISSUER Agricultural Bank of China"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"ACQ 48873110", strlen("ACQ 48873110"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"CARD number.", strlen("CARD number."));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"6228 48x*****8 116 S", strlen("6228 48x*****8 116 S"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"TYPE of transaction(TXN TYPE)", strlen("TYPE of transaction(TXN TYPE)"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"SALE", strlen("SALE"));
    if (RET_OK != iRc)
    {
        return iRc;
    }
    iRc = Lib_PrintStr(ALIGN_M, aucPrintSeparator, strlen((char*)aucPrintSeparator));
    if (RET_OK != iRc)
    {
        return iRc;
    }

    Lib_PrnSetSpace(10);
    Lib_PrnSetFont(20, 1, 1);
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s   %s\r\n%s", "Item/NO", "Quantity", "Amount");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));

    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s\r\n%s", "BATCH NO", "000043");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));

    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s\r\n%s", "VOUCHER NO", "000509");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));

    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s\r\n%s", "AUTH NO", "000786");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));

    // DATE/TIME:
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s\r\n%s", "DATE/TIME", "2014/12/07");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));
    iRc = Lib_PrintStr(ALIGN_R, (unsigned char *)"16:15:17", strlen("16:15:17"));

    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "%s\r\n%s", "REF NO", "0000015952");
    iRc = Lib_PrintStr(ALIGN_J, aucPrintBuf, strlen((char *)aucPrintBuf));
    iRc = Lib_PrintStr(ALIGN_R, (unsigned char *)"76", strlen("76"));

    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"2014/12/07", strlen("2014/12/07"));
    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"16:15:17", strlen("16:15:17"));

    iRc = Lib_PrintStr(ALIGN_L, (unsigned char *)"AMOUNT:", strlen("AMOUNT:"));

    Lib_PrnSetFont(FONT8, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);
    Lib_PrnSetFont(FONT32, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, aucPrintSeparator, strlen((char*)aucPrintSeparator));
    Lib_PrnSetFont(FONT8, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);

    Lib_PrnSetFont(FONT48, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)"RMB:249.00", strlen("RMB:249.00"));

    Lib_PrnSetFont(FONT8, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);
    Lib_PrnSetFont(FONT32, 1, 1);
    iRc = Lib_PrintStr(ALIGN_M, aucPrintSeparator, strlen((char*)aucPrintSeparator));

    Lib_PrnSetSpace(8);
    Lib_PrnSetFont(FONT18, 1, 1);
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "Please scan the BarCode for getting more information:");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char*)aucPrintBuf));
    Lib_PrnXCode(60, "123456", 250, 88, NULL);
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);

    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
    sprintf((char *)aucPrintBuf, "Please scan the QRCode for getting more information:");
    iRc = Lib_PrintStr(ALIGN_L, aucPrintBuf, strlen((char*)aucPrintBuf));
    iRc = Lib_PrintStr(ALIGN_M, (unsigned char *)" ", 1);
    Lib_PrnXCode(100, "123456", 136, 0, NULL);

    // Turn off the printer when printing is complete
    iRc = Lib_PrnClose();
    if (iRc == RET_ERR_TPH_NO_PAPER)
    {
        char aucDispBuffer[128] = {0};

        memset(aucDispBuffer, 0x00, sizeof(aucDispBuffer));
        sprintf((char *)aucDispBuffer, "%s\n %s",TIP_PRN_PAPER_OUT, TIP_PRN_PAPER_ADD);
        iRc = MessageBox(aucDispBuffer, MB_COLOR_YELLOW |MB_CENTER | MB_ICONQUESTION, 60000);
        if (iRc == KB_ENTER)
        {
            LvUiMessageBox(TIP_PRINTING, MB_COLOR_YELLOW |MB_CENTER | MB_ICONQUESTION, NULL, 500);
            Lib_PrnOpen();
            goto Again;
        }
    }

    return RET_OK;
}

