/**********************************************************************************************************
 * Copyright(C)2018,
 * File Name     : EmvDemo.c
 * Version       : v1.0
 * Author        :
 * Description   : EMV application sample apis
 **********************************************************************************************************/
/**************************** INCLUDE FILES BEGIN **************************/
#include "LibEmvKernel.h"
#include "LibPinpad.h"
#include "EmvInterface.h"
#include "EmvDemo.h"
#include "key.h"
#include "Ui.h"
#include "LibEmvKernel.h"
#include "EmvType.h"
#include "Common.h"
#include <math.h>
#include "LibDukpt.h"
#include "card.h"
#include "VAN_security.h"
#include "MessageBox.h"
#include "Idle.h"
#include "Trade.h"
#include "LanguageEN.h"
#include "Communication.h"
#include "CardTrade.h"

extern ST_MSC_TRACK astCiTrack[TRACK_NUM];

// Save tracks data
extern unsigned char aucCiTrackBuf[SDK_TRACK_NUM][MSC_TRACK_BUFSIZE];

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
static unsigned char aucMacPanAscii[30];

#define MSC_TMP_BUF_LEN     1000  //

ST_PARAMETER_CONFIG_LIST stParConlist;
ST_PARAMETER_TRANSACTION_PRESET stTrasactionSet;

unsigned char g_aucTradeMsgBuf[1024];
unsigned short g_usTradeMsgLen = 0;

// random number
u8 g_aucRandom[16];
#if 0
static unsigned char PanProcess(unsigned char *pucSource, unsigned char ucSourceLen, unsigned char *pucDest, unsigned char *pucDestLen, unsigned char ucCardType);
static unsigned char TrackDataProcess(unsigned char *pucSource, unsigned char ucSourceLen, unsigned char *pucDest, unsigned char *pucDestLen, unsigned char ucTrack, unsigned char ucTrackProcess);
#endif
//the default pos parameters
unsigned char aucPosParaDefault[157] =
{
    0xAA,

    0x9B,

    0x4E,
    0x54,
    0x59,
    0x4C,

    0xF0,
    0xF1,
    0xA6,
    0xEB,

    0xFF,0xAA,0xBB,// Fixed characters
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,// Random number format

    0x00,// Downgraded Transactions. 00- No Downgraded Transactions
    0x00,// Downgrading transaction. 00- not allowed
    0x00,// Password input timeout value. 00- No timeout time
    0x00,// Batch serial number. 00- do not return

    0x02,// Magnetic track information processing method
    0x02,// Second track information processing method
    0x01,// IC Card 55 Domain Processing Method
    0x01,// MAC processing method
    0x00,// Master key processing method
    0x01,// Work key processing method

    0x02,// Swipe card encryption key
    0x03,// PIN encryption key
    0x04,// MAC encryption key

    //55 Domain
    0x9F,0x26,0x9F,0x27,0x9F,0x10,0x9F,0x37,0x9F,0x36,0x00,0x95,0x00,0x9A,0x00,0x9C,0x9F,0x02,0x5F,0x2A,0x00,0x82,0x9F,0x1A,0x9F,0x03,0x9F,0x33,0x9F,0x34,0x9F,0x35,0x9F,0x1E,0x00,0x84,0x9F,0x09,0x9F,0x41,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

    0x07,
    0x03,// Main account processing method 03- Magnetic stripe card returns plaintext, IC card encrypts after fills in length and 0
    0x00,// contactless card process

    0x00,
    0x00,
    0x00,
};

short RsaDecrypt(unsigned char *pucDes, unsigned char *pucSrc,
                    unsigned char *pucE, unsigned char ucELen,
                    unsigned char *pucN, unsigned short usNLen)
{
    return 0;
}

/**
 * @method	pOnEmvGetTerminalCapk
 * @brief	Get terminal CA public key parameter callback method
 * @param	pucRid		    :RID, form TAG 4F
 * @param	ucCapkIndex		:Certification authority public key index, TAG 8F
 * @param	pucCapk		    :output parameter, CA public key buffer
 * @param	pusCapkLen  	:output parameter, the length of returned  CA public key
 * @param	pucExp		    :output parameter, CA public key exponent buffer
 * @param	pucExpLen     	:output parameter, the length of returned  CA public key exponent
 * @retval					:0 -success, -1 -there is no matching CA public key
 */
int EmvGetTerminalCapk(const unsigned char pucRid[5], unsigned char ucCapkIndex, unsigned char *pucCapk, unsigned short *pusCapkLen, unsigned char *pucExp, unsigned char *pucExpLen)
{
    return Lib_GetTerminalCapk(pucRid, ucCapkIndex, pucCapk, pusCapkLen, pucExp, pucExpLen);
}


/**
 * @method	pOnEmvGetTerminalAID
 * @brief	Get terminal AID parameter callback method
 * @param	uiAidIndex		:AID parameter index, start at 0
 * @param	pucTerAid		:output parameter, AID parameter buffer
 * @param	uiSize		    :AID parameter buffer size
 * @param	puiTerAidLen	:output parameter, the length of returned AID parameter
 * @retval					:0 -success, -1 -there is no AID parameter
 */
int EmvGetTerminalAID(unsigned int uiAidIndex, unsigned char *pucTerAid, unsigned int uiSize, unsigned int *puiTerAidLen)
{
    return Lib_GetTerminalAID(uiAidIndex, pucTerAid, uiSize, puiTerAidLen);
}

/**
 * @method	pOnEmvCertRevolutionCheck
 * @brief	CA certificate revolution check callback method
 * @param	aucRID  		:RID, form TAG 4F
 * @param	ucCaIndex		:Certification authority public key index, TAG 8F
 * @param	aucCertSerial	:Certification authority public key serial number
 * @retval					:0 -not in the revolution list, -1 -in the revolution list
 */
int EmvCertRevolutionCheck(const unsigned char aucRID[5], unsigned char ucCaIndex, const unsigned char aucCertSerial[3])
{
    return 0;
}

/**
 * @method	pOnEmvExceptionFileCheck
 * @brief	Application PAN blacklist check callback method
 * @param	pucPanBcd		:Application PAN, from TAG 5A, BCD format, Fill FF to 10 bytes at the end
 * @param	ucPanSerial		:Application PAN sequence number, TAG 5F34, n2
 * @retval					:0 -not in the blacklist, -1 -in the blacklist
 */
int EmvExceptionFileCheck(const unsigned char pucPanBcd[10], unsigned char ucPanSerial)
{
    return 0;
}

/**
 * @method	pOnEmvDispMsg
 * @brief	the user interface callback method
 * @param	ucIdentify		:user interface message identifier, refer to chapter 9 of EMV book A
 * @param	ucStatus		:user interface status, refer to chapter 9 of EMV book A
 * @retval					:0 -success, <0 -error code
 */
int EmvDispMsg(ST_UIRD *pstUird, unsigned char ucStatus)
{
    return CbAppDisplayMsg(pstUird, ucStatus);
}

/**
  * @method	pOnEmvAppConfirm
  * @brief	card application confirm callback method
  * @param	ucAppNum		:application total number
  * @param	pucAppData		:application name TLV list, the index start at 1, for example: 0107454d5641505031 0207454d5641505032
                            means there are 2 applications, the first name is "EMVAPP1", the second name is "EMVAPP2".
  * @param	uiDataLen		:pucAppData length
  * @param	pucIndex		:output parameter, the index of selected application
  * @retval					:0 -confirm success, -1029 -user cancel
  */
int EmvAppConfirm(unsigned char ucAppNum, unsigned char *pucAppData, unsigned int uiDataLen, unsigned char *pucIndex)
{
    *pucIndex = 1;

    return 0;
}


/**
 * @method	pOnEmvCardNoConfirm
 * @brief	confirm card number callback method
 * @param	pszPAN		    :string card number
 * @retval					:0 -success, -1029 -user cancel
 */
int EmvCardNoConfirm(char *pszPAN)
{
    return 0;
}


short SetPinblock(u8* ascPin, u8 ascPinLen, ST_TRANSDATA* pstTransData)
{
    unsigned char ucNoPanPinblock[8], ucNoPanPinblockLen = 0;
    unsigned char aucPAN[20], ucPANLen = 0, aucPANBCD[8];
    unsigned char ucPanPinblock[8] = {0},i = 0;
    ST_CARDINFO* pstCardInfo = NULL;
    unsigned char buf[32]={0};
    unsigned char iv[16]={0};
    unsigned char aucBuf[64];
    u32 ulLen;
    FUN_IN;
    pstCardInfo = &pstTransData->stCardInfo;

    ucNoPanPinblock[0] = ascPinLen;
    Lib_AscNum2bcd(ascPin, ucNoPanPinblock + 1, ascPinLen, &ucNoPanPinblockLen, 0x0f);
    memset(ucNoPanPinblock + 1 + ucNoPanPinblockLen, 0xFF, 8 - 1 - ucNoPanPinblockLen);
    TRACE("plain PIN:");
    TRACEBUF(ucNoPanPinblock, 8);

    memset(aucPAN, 0, sizeof(aucPAN));
    strcpy((char*)aucPAN, (char*)pstCardInfo->aucCardNo);

    if (0 == Lib_EmvGetTlv(0x004F, aucBuf, sizeof(aucBuf), (unsigned int*)&ulLen))
    {
        TRACEBUF ( aucBuf,ulLen);
        if(memcmp(aucBuf,"\xA0\x00\x00\x06\x15",5) != 0)
        {
            memcpy(ucPanPinblock,ascPin,ascPinLen);
            Lib_PciGetPin(KEYID_TPK, aucPAN,0, ucPanPinblock);
            TRACE ( "ucPanPinblock" );
            TRACEBUF ( ucPanPinblock, 8 );
            pstCardInfo->ucPinExistFlag = true;
            memcpy(pstCardInfo->aucPinBlockH, ucPanPinblock, 8);
            return RET_OK;
        }
    }

    ucPANLen = strlen((const char*)aucPAN);
    memset(aucPANBCD, 0, sizeof(aucPANBCD));
    Lib_Asc2Bcd((char*)(aucPAN + ucPANLen - 13), aucPANBCD + 2, 12);

    TRACE("PAN: ");
    TRACEBUF(aucPANBCD, 8);
    for (i = 0; i < 8; i++)
    {
        ucPanPinblock[i] = ucNoPanPinblock[i] ^ aucPANBCD[i];
    }

    TRACE ( "PIN XOR PAN: " );
    TRACEBUF ( ucPanPinblock, 8 );
    TRACE ( "TPK: " );
    TRACEBUF ( buf, 16 );

    Lib_AlgDes(iv, buf, ucPanPinblock, 8, MODE_CBC|ENCRYPTION|DES_DOUBLE_KEY);


    TRACE("PinBlock : ");
    TRACEBUF(ucPanPinblock, 8);

    pstCardInfo->ucPinExistFlag = true;
    memcpy(pstCardInfo->aucPinBlockH, ucPanPinblock, 8);
    return RET_OK;
}

void _lv_online_pinpad_callback(unsigned char input_count, const unsigned char ucLastTimes)
{
    unsigned char aucInput[13] = {0};
    memset(aucInput, '*', input_count);

    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, "Please enter PIN:");

    UIText(14, 3, DISP_MODE_NORMAL, (void *)aucInput);
}

void _lv_offline_pinpad_callback(unsigned char input_count, const unsigned char ucLastTimes)
{
    unsigned char aucInput[13] = {0};
    memset(aucInput, '*', input_count);

    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, "Off-Line Pin:");

    UIText(14, 3, DISP_MODE_NORMAL, (void *)aucInput);
}


/**
 * @method	pOnEmvInputPIN
 * @brief	PIN input callback method
 * @param	ucPinType		:PIN type,  0 -offline pin, 1 -online pin
 * @param	pucPinBlock		:output 8 bytes PIN block
 * @param	ucLastTimes		:pin retry count
 * @retval					:0 -pin input success, -1013 -PIN bypass, -1029 -user cancel
 */
int EmvInputPIN(const unsigned char ucPinType, unsigned char *pucPinBlock, const unsigned char ucLastTimes)
{
    pin_param param = { 0 };
    int ret = ERR_NORMAL;

    if (ucPinType != 0)
    {
        unsigned char aucBuf[50];
        unsigned int ulLen = 0;

        param.min_input_count = 4;
        param.max_input_count = 6;
        param.timeout = 20;
        param.is_bypass = 0;

        Lib_PciSetPinpadUI(_lv_online_pinpad_callback);
        ret = Lib_PciOnlinePinpad(param, ucLastTimes);
        if (ret != ERR_OK)
        {
            return ret;
        }

        if (gstTransData.stCardInfo.ucCardLen == 0)
        {
            if (0 != Lib_EmvGetTlv(0x005A, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen))
            {
                Lib_EmvGetTlv(0x0057, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen);
            }

            TRACE("PAN:");
            TRACEBUF(aucBuf, ulLen);
            Lib_Bcd2Asc(aucBuf, (char*)gstTransData.stCardInfo.aucCardNo, ulLen);
            gstTransData.stCardInfo.aucCardNo[ulLen * 2] = '\0';

            while (gstTransData.stCardInfo.aucCardNo[strlen((const char *)gstTransData.stCardInfo.aucCardNo) - 1] > '9')
            {
                gstTransData.stCardInfo.aucCardNo[strlen((const char *)gstTransData.stCardInfo.aucCardNo) - 1] = '\0';
            }
        }
        gstTransData.stCardInfo.ucCardLen = strlen((char *)gstTransData.stCardInfo.aucCardNo);
        pucPinBlock[0] = 0;
        Lib_PciGetPin(KEYID_TPK, gstTransData.stCardInfo.aucCardNo, 0, pucPinBlock);
        return RET_OK;
    }
    else
    {
        param.min_input_count = 4;
        param.max_input_count = 6;
        param.timeout = 20;
        param.is_bypass = 1;

        Lib_PciSetPinpadUI(_lv_offline_pinpad_callback);
        return Lib_PciOfflinePinpad(param, ucLastTimes, pucPinBlock);
    }
}


ST_KERNEL_CALLBACK stEMVCallback =
{
    EmvGetTerminalCapk,
    EmvGetTerminalAID,
    EmvCertRevolutionCheck,
    EmvExceptionFileCheck,
    EmvDispMsg,
    EmvAppConfirm,
    EmvCardNoConfirm,
    EmvInputPIN,
};


typedef enum
{
    TRADE_INIT,
    TRADE_IC_CARD,
    TRADE_MSC_CARD,
    TRADE_IC_PIN,
    TRADE_MSC_PIN,
    TRADE_IC_RESP,
    TRADE_NONE,
} TRADESTEP;

// Transaction hold flag 0: Not traded 1: Transaction in progress, waiting for next instruction
unsigned char g_ucTradeHoldStatus = 0;
// Control transaction steps
TRADESTEP g_ucTradeStep = TRADE_INIT;
// Transaction retention time
unsigned int ulTradeHoldTime = 0;

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
//static unsigned char aucMacPanAscii[30];


void SetTradeHoldStatus(unsigned char status)
{
    if (status >= 1)
    {
        ulTradeHoldTime = Lib_SysGetTimer();
        g_ucTradeHoldStatus = status;
    }
    else if ((status == 0) && (g_ucTradeHoldStatus == 1))
    {
        g_ucTradeHoldStatus = 0;
        // LcdDisStr(0, 2, 1, "Transaction Complete!");
    }
    else if ((status == 0) && (g_ucTradeHoldStatus == 2))
    {
        g_ucTradeHoldStatus = 0;
    }
}

TRADESTEP GetTradeStep(void)
{
    TRACE("TradeStep = %d. ", g_ucTradeStep);
    return g_ucTradeStep;
}

void SetTradeStep(TRADESTEP status)
{
    g_ucTradeStep = status;
    TRACE("TradeStep = %d. ", g_ucTradeStep);
}

// EMV kernel error converted to status code
unsigned short KernelErrorConvertSWCode(unsigned char ucErrL1, unsigned char ucErrL2, unsigned char ucErrL3)
{
    unsigned short usSWCode;

    if (ucErrL1 != 0x00)
    {
        usSWCode = 0x8531 + ucErrL1 - 1;
    }
    else if (ucErrL2 != 0x00)
    {
        usSWCode = 0x8535 + ucErrL2 - 1;
    }
    else if (ucErrL3 != 0x00)
    {
        usSWCode = 0x8550 + ucErrL3 - 1;
    }
    else
    {
        usSWCode = 0x8501;
    }

    return usSWCode;
}

// Package 55 filed
static char PackIcCardData(unsigned short *pusTagList, unsigned char *pucBuf, unsigned short *pusBufLen)
{
    unsigned char i;
    unsigned char *pbuf;
    unsigned int TempLen;
    short sRc;

    pbuf = pucBuf;
    for (i = 0; pusTagList[i] != 0xFFFF; i++)
    {
        TRACE("TAG_%04x", pusTagList[i]);
        if ((pusTagList[i] & 0xFF00) != 0)
        {
            *pbuf++ = (unsigned char)((pusTagList[i] >> 8) & 0xFF);
        }
        *pbuf++ = (unsigned char)(pusTagList[i] & 0xFF);

        sRc = Lib_EmvGetTlv(pusTagList[i], pbuf + 1, *pusBufLen, &TempLen);
        if (0 != sRc)
        {
            // TAG_9F27 ,TAG_9F74, AG_008A,Not necessary data, may not be obtained, if not, assign the value yourself
            if (pusTagList[i] == 0x9F27)
            {
                *pbuf++ = 0x01;
                *pbuf++ = 0x80;
                continue;
            }

            if (pusTagList[i] == 0x9F74)
            {
                *pbuf++ = 0x06;
                memcpy(pbuf, "\x45\x43\x43\x30\x30\x31", 6);
                pbuf += 6;
                continue;
            }

            if (pusTagList[i] == 0x008A)
            {
                *pbuf++ = 0x02;
                memcpy(pbuf, "\x59\x31", 2);
                pbuf += 2;
                continue;
            }

            if (pusTagList[i] == 0x9F34)
            {
                *pbuf++ = 0x03;
                memcpy(pbuf, "\x00\x00\x00", 6);
                pbuf += 3;
                continue;
            }

            if (pusTagList[i] == 0x9F63)
            {
                pbuf -= 2;
                TRACE("Get TAG_%04X error\r\n", pusTagList[i]);
                continue;
            }
            else
            {
                pbuf--;
                if ((pusTagList[i] & 0xFF00) != 0)
                {
                    pbuf--;
                }

                TRACE("Get TAG_%04X error\r\n", pusTagList[i]);
                continue;
            }
            if (pusTagList[i] == 0x9F6E)
            {
                pbuf -= 2;
                TRACE("Get TAG_%04X error\r\n", pusTagList[i]);
                continue;
            }
        }
        *pbuf++ = TempLen;
        pbuf += TempLen;
    }

    *pusBufLen = pbuf - pucBuf;
    return 0;
}

#if 0
//-----------------------------------------------------------------------------
// Name    : PanProcess
// Func    : Main account processing
// Paras   : pucSource[in],     Source data
//         : ucSourceLen[in],   Source data length
//         : pucDest[out],      Destination Data
//         : pucDestLen[out],   Destination Data length
//         : ucCardType[in],    Card Type
// Ret     : - 0  : success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
static unsigned char PanProcess(unsigned char *pucSource, unsigned char ucSourceLen, unsigned char *pucDest, unsigned char *pucDestLen, unsigned char ucCardType)
{
    unsigned char aucBuf[24], ucLen = 0;
    unsigned char ucKeyIndex = stParConlist.ucTrackEncrypKey;
    if(g_ucSubAlgFlag == ALGORITHM_DUKPT)//DUKPT
    {
        ucKeyIndex = DUKPT_WK_TDK_REQUIRE;
    }
    switch (stParConlist.ucPanPro)
    {
    case 1:
        // plaint text, return directly
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return 1;
        // break;

    case 2:
        // Fill length and 0 data
        // After converting to BCD, the last digit of the data is filled with f
        if ((*(pucSource + ucSourceLen - 1) | 0xF0) == 0xFF)
        {
            ucLen = 2 * ucSourceLen - 1;
        }
        else
        {
            ucLen = 2 * ucSourceLen;
        }
        aucBuf[0] = ucLen;
        memcpy(aucBuf + 1, pucSource, ucSourceLen);
        ucLen = ucSourceLen + 1;
        while (ucLen % 8)
        {
            aucBuf[ucLen++] = 0x00;
        }
        break;

    case 3:
        // Magnetic stripe card returns plaintext, IC card encrypts after fills length and 0 data
        if (ucCardType == 0)
        {
            memcpy(pucDest, pucSource, ucSourceLen);
            *pucDestLen = ucSourceLen;
            return 1;
        }
        else
        {
            // After converting to BCD, the last digit of the data is filled with f
            if ((*(pucSource + ucSourceLen - 1) | 0xF0) == 0xFF)
            {
                ucLen = 2 * ucSourceLen - 1;
            }
            else
            {
                ucLen = 2 * ucSourceLen;
            }
            aucBuf[0] = ucLen;
            memcpy(aucBuf + 1, pucSource, ucSourceLen);
            ucLen = ucSourceLen + 1;
            while (ucLen % 8)
            {
                aucBuf[ucLen++] = 0x00;
            }
        }
        break;

    default: // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return 1;
        // break;
    }
    TRACE("The length of filled PAN: %d\r\n", ucLen);
    TRACE("Filled PAN: ");
    TRACEBUF(aucBuf, ucLen);

    if (1 != DataEncryptProcess(aucBuf, pucDest, ucLen, ucKeyIndex))
    {
        return 0;
    }
    *pucDestLen = ucLen;
    TRACE("Encrypted PAN: ");
    TRACEBUF(pucDest, *pucDestLen);

    return 1;
}

//-----------------------------------------------------------------------------
// Name    : ICCardDataProcess
// Func    : IC card 55 domain data processing
// Paras   : pucSource[in],     Source data
//         : ucSourceLen[in],   Source data length
//         : pucDest[out],      Destination Data
//         : pucDestLen[out],   Destination Data length
// Ret     : - 0 , success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
static char ICCardDataProcess(unsigned char *pucSource, unsigned short usSourceLen, unsigned char *pucDest, unsigned short *pusDestLen)
{
    unsigned short usTemBuflen = 0, i = 0;
    unsigned char ucDiff;
    unsigned char ucKeyIndex = stParConlist.ucTrackEncrypKey;
    unsigned int ulLen;
	if(g_ucSubAlgFlag == ALGORITHM_DUKPT)//DUKPT
	{
		ucKeyIndex = DUKPT_WK_TDK_REQUIRE;
	}

    TRACE("55 filed length: %d\r\n", usSourceLen);
    TRACE("55 filed data: ");
    TRACEBUF(pucSource, usSourceLen);
    switch (stParConlist.ucICCardPro)
    {
    case 1:
        // plaint text, return directly
        memcpy(pucDest, pucSource, usSourceLen);
        *pusDestLen = usSourceLen;
        return 1;
        // break;

    case 2:
        // Add one 0x80 and then add 0x00 to multiples of 8
        *(pucSource + usSourceLen) = 0x80;
        usTemBuflen = usSourceLen + 1;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucSource + usTemBuflen, 0x00, padLen);
            usTemBuflen += padLen;
        }

        break;

    case 3:
        // padding with 0xff to multiples of 8
        usTemBuflen = usSourceLen;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucSource + usTemBuflen, 0xff, padLen);
            usTemBuflen += padLen;
        }

        break;

    case 4:
        // padding with 0x80 to multiples of 8
        usTemBuflen = usSourceLen;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucSource + usTemBuflen, 0x80, padLen);
            usTemBuflen += padLen;
        }

        break;

    case 5:
        // 2-bytes length + 55 fields data + padding byte 00
        *pucDest = (usSourceLen >> 8) & 0xFF;
        *(pucDest + 1) = usSourceLen & 0xFF;
        memcpy(pucDest + 2, pucSource, usSourceLen);
        usTemBuflen = usSourceLen + 2;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucDest + usTemBuflen, 0x00, padLen);
            usTemBuflen += padLen;
        }

        memcpy(pucSource, pucDest, usTemBuflen);
        break;

    case 6:
        // PKCS5 padding
        usTemBuflen = usSourceLen;
        while (usTemBuflen % 8)
        {
            usTemBuflen++;
        }
        ucDiff = usTemBuflen - usSourceLen;
        usTemBuflen = usSourceLen;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucSource + usTemBuflen, ucDiff, padLen);
            usTemBuflen += padLen;
        }

        break;

    case 7:
        // Encrypt after adding track data and length (1 byte 2th track length + 2th track plaintext + 2 bytes 55 field length+55 field plaintext)
        if (0 != Lib_EmvGetTlv(0x0057, pucDest + 1, *pusDestLen, &ulLen))
        {
            return 0;
        }
        TRACE("2th track equ data: ");
        TRACEBUF(pucDest + 1, ulLen);

        // The last digit of the magnetic track data is f
        if ((*(pucDest + ulLen) | 0xF0) == 0xFF)
        {
            TRACE("The last digit of the magnetic track data is F\r\n");
            *pucDest = ulLen * 2 - 1;
            usTemBuflen = ulLen;
            *(pucDest + usTemBuflen) &= 0xF0;
            *(pucDest + usTemBuflen) |= (usSourceLen * 2) / 256 / 16;
            usTemBuflen++;

            ulLen = ((((usSourceLen * 2) / 256) % 16) << 4) | (((usSourceLen * 2) % 256) / 16);
            *(pucDest + usTemBuflen) = ulLen;
            usTemBuflen++;

            ulLen = ((((usSourceLen * 2) % 256) % 16) << 4) | (*pucSource / 16);
            *(pucDest + usTemBuflen) = ulLen;
            usTemBuflen++;

            for (i = 0; i < (usSourceLen - 1); i++)
            {
                ulLen = ((*(pucSource + i) % 16) << 4) | (*(pucSource + i + 1) / 16);
                *(pucDest + usTemBuflen) = ulLen;
                usTemBuflen++;
            }

            ulLen = ((*(pucSource + i) % 16) << 4) | 0x0F;
            *(pucDest + usTemBuflen) = ulLen;
            usTemBuflen++;

            if (usTemBuflen % 8)
            {
                unsigned short padLen = 8 - (usTemBuflen % 8);
                memset(pucDest + usTemBuflen, 0x00, padLen);
                usTemBuflen += padLen;
            }

            memcpy(pucSource, pucDest, usTemBuflen);
        }
        else
        {
            TRACE("The last digit of magnetic track data is not F\r\n");
            *pucDest = ulLen * 2;
            usTemBuflen = ulLen + 1;
            *(pucDest + usTemBuflen) = (usSourceLen * 2) / 256;
            usTemBuflen++;
            *(pucDest + usTemBuflen) = (usSourceLen * 2) % 256;
            usTemBuflen++;
            memcpy(pucDest + usTemBuflen, pucSource, usSourceLen);
            usTemBuflen += usSourceLen;

            if (usTemBuflen % 8)
            {
                unsigned short padLen = 8 - (usTemBuflen % 8);
                memset(pucDest + usTemBuflen, 0x00, padLen);
                usTemBuflen += padLen;
            }

            memcpy(pucSource, pucDest, usTemBuflen);
        }
        break;

    case 8: // padding 00
    case 9: // padding 00
        usTemBuflen = usSourceLen;

        if (usTemBuflen % 8)
        {
            unsigned short padLen = 8 - (usTemBuflen % 8);
            memset(pucSource + usTemBuflen, 0x00, padLen);
            usTemBuflen += padLen;
        }

        break;

    default:
        memcpy(pucDest, pucSource, usSourceLen);
        *pusDestLen = usSourceLen;
        return 1;
        // break;
    }
    TRACE("filled 55 field data length: %d\r\n", usTemBuflen);
    TRACE("Filled 55 field data: ");
    TRACEBUF(pucSource, usTemBuflen);

    if (1 != DataEncryptProcess(pucSource, pucDest, usTemBuflen, ucKeyIndex))
    {
        return 0;
    }

    if (stParConlist.ucICCardPro == 8)
    {
        memcpy(pucSource, pucDest, usTemBuflen);
        Lib_Int2Bcd(usSourceLen, pucDest, 2);
        memcpy(pucDest + 2, pucSource, usTemBuflen);
        *pusDestLen = usTemBuflen + 2;
    }
    else
    {
        *pusDestLen = usTemBuflen;
    }
    TRACE("Encrypted 55 field data length: %d\r\n", *pusDestLen);
    TRACE("Encrypted 55 field data: ");
    TRACEBUF(pucDest, *pusDestLen);
    return 1;
}

 //-----------------------------------------------------------------------------
// Name    : TrackDataProcess
// Func    : 2th track, 3th track and 2th track equivalent data processing
// Paras   : pucSource[in],         Source data
//         : ucSourceLen[in],       Source data length
//         : pucDest[out],          Destination Data
//         : pucDestLen[out],       Destination Data length
//         : ucTrack[in],           Magnetic track
//         : ucTrackProcess[in],    Magnetic track data processing method
// Ret     : - 0, success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
static unsigned char TrackDataProcess(unsigned char *pucSource, unsigned char ucSourceLen, unsigned char *pucDest, unsigned char *pucDestLen, unsigned char ucTrack, unsigned char ucTrackProcess)
{
    unsigned char aucBuf[255];
	unsigned short ucLen = 0;
    unsigned char i = 0;
    unsigned char ucKeyIndex = stParConlist.ucTrackEncrypKey;
	if(g_ucSubAlgFlag == ALGORITHM_DUKPT)//DUKPT
	{
		ucKeyIndex = DUKPT_WK_TDK_REQUIRE;
	}
    TRACE("Track data length: %d\r\n", ucSourceLen);
    TRACE("Track data: ");
    TRACEBUF(pucSource, ucSourceLen);

    // Track data processing
    switch (ucTrackProcess)
    {
    case 1:
        // plaint text, return directly
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return 1;
        //break;

    case 2:
        // padding with 0xff to multiples of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;

        if (ucLen % 8)
        {
            unsigned short padLen = 8 - (ucLen % 8);
            memset(aucBuf + ucLen, 0xFF, padLen);
            ucLen += padLen;
        }

        break;

    case 3:
        // padding with 0x80 to multiples of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;

        if (ucLen % 8)
        {
            unsigned short padLen = 8 - (ucLen % 8);
            memset(aucBuf + ucLen, 0x80, padLen);
            ucLen += padLen;
        }

        break;

    case 4:
        // partial encryption
        memcpy(aucBuf, pucSource + ucSourceLen - 1 - 8, 8);
        ucLen = 8;
        break;

    case 5: // format
    case 9: // format
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;

        // 2th tracks, padding 0xff to 24 bytes
        if (SDK_TRACK2 == ucTrack)
        {
            if (ucLen < 24)
            {
                memset(aucBuf + ucLen, 0xFF, 24 - ucLen);
                ucLen = 24;
            }
        }
        else
        {
            // 3th tracks, padding 0xff to 56 bytes
            if (ucLen < 56)
            {
                memset(aucBuf + ucLen, 0xFF, 56 - ucLen);
                ucLen = 56;
            }
        }
        break;

    case 6:
        // padding 0x80 to 80 bytes
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;

        if (ucLen < 80)
        {
            memset(aucBuf + ucLen, 0xFF, 80 - ucLen);
            ucLen = 80;
        }

        break;

    case 7:
        // padding with length and 0
        // After converting to BCD, the last digit of the track data is filled with f
        if ((*(pucSource + ucSourceLen - 1) | 0xF0) == 0xFF)
        {
            ucLen = 2 * ucSourceLen - 1;
            *(pucSource + ucSourceLen - 1) &= 0xF0;
        }
        else
        {
            ucLen = 2 * ucSourceLen;
        }

        if (SDK_TRACK2 == ucTrack)
        {
            // The length of 2th magnetic tracks occupies one byte
            Lib_Int2Bcd(ucLen, aucBuf, 1);
            memcpy(aucBuf + 1, pucSource, ucSourceLen);
            ucLen = ucSourceLen + 1;
        }
        else
        {
            // The length of 3th magnetic tracks occupies 2 bytes
            Lib_Int2Bcd(ucLen, aucBuf, 2);
            memcpy(aucBuf + 2, pucSource, ucSourceLen);
            ucLen = ucSourceLen + 2;
        }

        if (ucLen % 8)
        {
            unsigned short padLen = 8 - (ucLen % 8);
            memset(aucBuf + ucLen, 0x00, padLen);
            ucLen += padLen;
        }

        break;

    case 8:
        // ASCII code padding 0x00 to multiples of 8
        Lib_Bcd2Asc(pucSource, (char *)aucBuf, ucSourceLen);
        ucLen = ucSourceLen * 2;
        // The last byte is the padding byte f
        if (aucBuf[ucLen - 1] > 0x40)
        {
            ucLen -= 1;
        }

        for (i = 0; i < ucLen; i++)
        {
            // D convert to '='
            if (aucBuf[i] > 0x40)
            {
                aucBuf[i] = 0x3D;
            }
        }

        if (ucLen % 8)
        {
            unsigned short padLen = 8 - (ucLen % 8);
            memset(aucBuf + ucLen, 0x00, padLen);
            ucLen += padLen;
        }
        break;

    case 10:
        // 2th tracks data, encrypt after padding 0xff to multiples of 8;
        // 3th track data, return plaintext
        if (SDK_TRACK2 == ucTrack)
        {
            // 2th tracks data, padding 0xff to multiples of 8
            memcpy(aucBuf, pucSource, ucSourceLen);
            ucLen = ucSourceLen;

            if (ucLen % 8)
            {
                unsigned short padLen = 8 - (ucLen % 8);
                memset(aucBuf + ucLen, 0xFF, padLen);
                ucLen += padLen;
            }
        }
        else
        {
            // 3th track data, return plaintext
            memcpy(pucDest, pucSource, ucSourceLen);
            *pucDestLen = ucSourceLen;
            return 1;
        }
        break;

    case 11:
        // 2th track data, encrypt after padding 0xFF to 24 bytes, and
        // 3th track data, encrypt after padding 0xFF to multiple of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;
        if (SDK_TRACK2 == ucTrack)
        {
            // 2th tracks data, padding 0xff to 24 bytes
            if (ucLen < 24)
            {
                memset(aucBuf + ucLen, 0xFF, 24 - ucLen);
                ucLen = 24;
            }
        }
        else
        {
            // 3th track data, padding 0xFF to multiples of 8
            if (ucLen % 8)
            {
                unsigned short padLen = 8 - (ucLen % 8);
                memset(aucBuf + ucLen, 0xFF, padLen);
                ucLen += padLen;
            }
        }
        break;

    case 12:
        // 2th track data, encrypt after padding 0xFF to 24 bytes, and
        // 3th track data, encrypt after padding 0xFF to multiple of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;
        if (SDK_TRACK2 == ucTrack)
        {
            // 2th tracks data, padding 0xff to 24 bytes
            if (ucLen < 24)
            {
                memset(aucBuf + ucLen, 0xFF, 24 - ucLen);
                ucLen = 24;
            }
        }
        else
        {
            // 3th track's ASC data length is multiple of 16 bytes, padding with 8 bytes of 0xFF; Otherwise, encrypt after padding with 0xFF to multiple of 8
            if ((*(pucSource + ucSourceLen - 1) | 0xF0) == 0xFF)
            {
                // After converting to BCD, the last digit of the track data is filled with f
                if (ucLen % 8)
                {
                    unsigned short padLen = 8 - (ucLen % 8);
                    memset(aucBuf + ucLen, 0xFF, padLen);
                    ucLen += padLen;
                }
            }
            else
            {
                if (ucLen % 8)
                {
                    unsigned short padLen = 8 - (ucLen % 8);
                    memset(aucBuf + ucLen, 0xFF, padLen);
                    ucLen += padLen;
                }
                else
                {
                    memset(aucBuf + ucLen, 0xFF, 8);
                    ucLen += 8;
                }
            }
        }
        break;

    case 13:
        // padding with length and 0
        if ((*(pucSource + ucSourceLen - 1) | 0xF0) == 0xFF)
        {
            // After converting to BCD, the last digit of the track data is filled with f
            i = 2 * ucSourceLen - 1;
            *(pucSource + ucSourceLen - 1) &= 0xF0;
        }
        else
        {
            i = 2 * ucSourceLen;
        }
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;

        if (ucLen % 8)
        {
            unsigned short padLen = 8 - (ucLen % 8);
            memset(aucBuf + ucLen, 0x00, padLen);
            ucLen += padLen;
        }

        break;

    default:
        // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return 1;
        // break;
    }
    TRACE("Length of filled track data: %d\r\n", ucLen);
    TRACE("Filled track data: ");
    TRACEBUF(aucBuf, ucLen);

    if (1 != DataEncryptProcess(aucBuf, pucDest, ucLen, ucKeyIndex))
    {
        return 0;
    }

    if (4 == ucTrackProcess)
    {
        // partial encryption
        memcpy(pucSource + ucSourceLen - 1 - 8, pucDest, 8);
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
    }
    else if (13 == ucTrackProcess)
    {
        // add length bytes
        TRACE("i = %d\r\n", i);
        memcpy(pucSource, pucDest, ucLen);
        Lib_Int2Bcd(i, pucDest, 2);
        memcpy(pucDest + 2, pucSource, ucLen);
        *pucDestLen = ucLen + 2;
    }
    else
    {
        *pucDestLen = ucLen;
    }
    TRACE("Length of encrypted track data: %d\r\n", *pucDestLen);
    TRACE("Encrypted track data: ");
    TRACEBUF(pucDest, *pucDestLen);

    return 1;
}
#endif
u8 GetRandom(u8 *pucRecv, u8 *pucRandom, u8 *pucRandomNum)
{
    u8 i = 0, j = 0, k = 0;
    unsigned long ulSysTime = 0;

    // Internally generated 8-byte random number
    ulSysTime = Lib_SysGetTimer();
    memmove(pucRandom, (u8 *)&ulSysTime, 4);
    memcpy(pucRandom + 4, pucRandom, 4);

    if (!Lib_AlgDes(NULL, (u8 *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", pucRandom, 8, MODE_ECB|ENCRYPTION|DES_DOUBLE_KEY))
    {
        return 0;
    }
    TRACE("Generate Random : ");
    TRACEBUF(pucRandom, 8);

    // Generate random numbers based on their format
    for (i = 0; i < 16; i++)
    {
        if (stParConlist.ucRandomTag1 == stParConlist.aucRandomMode[i])
        {
            // Fixed characters
            g_aucRandom[i] = stParConlist.ucRandomTag1;
        }
        else if (stParConlist.ucRandomTag2 == stParConlist.aucRandomMode[i])
        {
            // command incoming characters
            g_aucRandom[i] = *(pucRecv + j);
            j++;
        }
        else if (stParConlist.ucRandomTag3 == stParConlist.aucRandomMode[i])
        {
            // Internally generated characters
            if (k < 8)
            {
                g_aucRandom[i] = *(pucRandom + k);
            }
            else
            {
                // When more than 8 internally generated random numbers are needed,
                // loop through the 8-byte random numbers generated internally
                g_aucRandom[i] = *(pucRandom + (k - 8));
            }
            k++;
        }
        else
        {
            // The random number format contains other Tags, indicating that the last 8 bytes of the random number are XOR results
            if (i >= 8)
            {
                // Other tags can only exist in the last 8 bytes of the random number format
                g_aucRandom[i] = g_aucRandom[i - 8] ^ 0xFF;
            }
            else
            {
                return 0;
            }
        }
    }
    *pucRandomNum = k;
    TRACE("RandomNum = %d\r\n", *pucRandomNum);
    TRACE("Random :");
    TRACEBUF(g_aucRandom, 16);
    return 1;
}


static u8 GetCardType(unsigned char* pucPan)
{
    int iBin = 0;
    unsigned char ucBuf[8]= {0};
    unsigned char ucCardType = 0;

    FUN_IN;
    memcpy(ucBuf, pucPan, 8);
    iBin = atoi((char*)ucBuf);
    TRACE("Bin = %d", iBin);

    if ((40000000 <= iBin) && (iBin <= 49999999))
    {
        ucCardType = CARD_VISA;
    }
    else if (((51000000 <= iBin) && (iBin <= 55999999)) ||
              ((22210000 <= iBin) && (iBin <= 27209999)) ||
              ((58276600 <= iBin) && (iBin <= 58276699)))
    {
        ucCardType = CARD_MASTERCARD;
    }
    else if (((37000000 <= iBin) && (iBin <= 37999999)) ||
              ((34000000 <= iBin) && (iBin <= 34999999)))
    {
        ucCardType = CARD_AMEX;
    }
    else if (((30000000 <= iBin) && (iBin <= 30599999)) ||
              ((30950000 <= iBin) && (iBin <= 30959999)) ||
              ((38000000 <= iBin) && (iBin <= 39999999)))
    {
        ucCardType = CARD_DINERS;
    }
    else if ((35280000 <= iBin) && (iBin <= 35899999))
    {
        ucCardType = CARD_JCB;
    }
    else if (((60110000 <= iBin) && (iBin <= 60110999)) ||
              ((60112000 <= iBin) && (iBin <= 60114999)) ||
              ((60117400 <= iBin) && (iBin <= 60117499)) ||
              ((60117700 <= iBin) && (iBin <= 60117999)) ||
              ((60118600 <= iBin) && (iBin <= 60119999)) ||
              ((62212600 <= iBin) && (iBin <= 62292599)) ||
              ((62400000 <= iBin) && (iBin <= 62699999)) ||
              ((62820000 <= iBin) && (iBin <= 62889999)) ||
              ((64400000 <= iBin) && (iBin <= 65999999)) ||
              ((6210940 <= iBin) && (iBin <= 6210949)))
    {
        ucCardType = CARD_DISCOVER;
    }
    return ucCardType;
}


unsigned char PosParameterInit(unsigned char *pBuf)
{
    memcpy((u8 *)&stParConlist, pBuf, sizeof(ST_PARAMETER_CONFIG_LIST));
    return TRUE;
}

unsigned char DukptKeyInit(void)
{
    //DUKPT key inject
    unsigned short data_len = 0;
    unsigned char aucBuf[8]={0};
    unsigned char ksn[10]={0xFF,0xFF,0x98,0x76,0x54,0x32,0x10,0x00,0x00,0x00};

    //KSN-FFFF9876543210E10005
    //BDK-C1D0F8FB4958670DBA40AB1F3752EF0D IPEK-74E912996F1245CC1CF6F5C1E02FD05A
    // Encrypted data with transmission key (0xff ...)
    //unsigned char ucIpek[16]={0x31,0x88,0xFB,0xDF,0x7A,0x1B,0x99,0x15,0xC7,0x90,0xA9,0x9B,0x2E,0xFF,0xEA,0x97};
    // Encrypted data with transmission key (0xff ...)
    unsigned char ucBdk[16]={0x95,0xBB,0x5C,0x8A,0x49,0xAE,0xC9,0x71,0x4C,0x3E,0x09,0x65,0xC8,0xF6,0x9C,0x93};
    unsigned char ucTestPinBlock[8]={0x04,0x12,0x43,0x89,0x99,0x9A,0xAA,0xAB};//04124389999AAAAB
    TRACE("Start test dukpt!!!\r\n");
    // Inject IPEK
    //Lib_DukptKeyInject(0,0,ksn,0,ucIpek,16);
    // Inject BDK
    Lib_DukptKeyInject(0,0,ksn,0,ucBdk,16);
    //DUKPT encrypt test
    memcpy(aucBuf,ucTestPinBlock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    data_len = 8;
    Lib_DukptEncryptOrDecrypt(0,0,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);

    return TRUE;
}


//trade pre-process
u8 ReadCardPreProcess(PST_PARAMETER_TRANSACTION_PRESET      pBuf)
{
    // Parameter Check - Waiting for Add more
    memcpy((u8 *)&stTrasactionSet, (u8 *)pBuf, sizeof(ST_PARAMETER_TRANSACTION_PRESET));
    return 1;
}


unsigned char CardDemoInit(void)
{
#if defined(DUKPT_EN)
    //Switch to dukpt algorithm
    SetAlgorithmMode(2);
    //0- ecb, 1- cbc
    SetEcbOrCbcMode(0);
    //inject dukpt key
    DukptKeyInit();
#endif
    PosParameterInit(aucPosParaDefault);
    return TRUE;
}


//card transaction demo
short CardDemo(short cardMode, ST_TRANSDATA* pstTransData)
{
    short errCode;

    ST_PARAMETER_TRANSACTION_PRESET pstTradePreset;
    unsigned char ucRandomData[16]={1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};   //
    memcpy(pstTradePreset.RandomInput, ucRandomData, 16);
    ReadCardPreProcess(&pstTradePreset);

    //Switch to des algorithm
    //SetAlgorithmMode(0);
    //set initial vector
    //SetCipherInitializationVector(iv);
    //Switch to dukpt algorithm
    SetAlgorithmMode(2);
    //0- ecb, 1- cbc
    SetEcbOrCbcMode(0);

    switch(cardMode)
    {
    case SCI_DETECETED://Contact card
    case NFC_DETECETED://Contacless card
        TRACE("Start EMV Demo!!!\r\n");
        errCode =  EmvDemo(cardMode, pstTransData);
        break;

    case MSC_DETECETED:
        //Magnetic stripe cards
        TRACE("Start Mcc Demo!!!\r\n");
        errCode = MccDemo(pstTransData);
        break;

    default:
        //Invalid card
        errCode = 0x8001;
        break;
    }

    TRACE("errCode %x\r\n",errCode);
    return errCode;
}

static signed short searchChar(unsigned char *pBuf, unsigned char ch, signed short startpos)
{
    signed short pos = startpos;

    if (startpos < 0)
    {
        return -1;
    }

    while (*(pBuf + pos) != 0)
    {
        if (*(pBuf + pos) == ch)
        {
            return pos;
        }
        pos++;
    }
    return -1;
}

// service code
static u8 GetCardSevCode(unsigned char *pucTrackBuf, unsigned char *pucCardSevCode, unsigned char *pucCardSevCodeLen)
{
    signed short sOffSet1;
    unsigned char *pucTrack2Buf;
    unsigned char ucLen;

    pucTrack2Buf = pucTrackBuf;

    // Find the position of the equal sign
    sOffSet1 = searchChar(pucTrack2Buf, '=', 0);
    if (sOffSet1 < 0)
    {
        return FALSE;
    }

    //+1, starting position of validity period
    sOffSet1++;

    //+4, starting position of the service code
    sOffSet1 += 4;

    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardSevCode, 3, &ucLen, 0x0f);
    *pucCardSevCodeLen = ucLen;
    //    Printf_STR_BUF("Card Service Code: ", pucCardSevCode, *pucCardSevCodeLen);
    return TRUE;
}

short GetEMVPrintData(ST_TRANSDATA* pstTransData)
{
    unsigned char aucBuf[128] = {0};
    unsigned int ulLen = 0;
    ST_EMVINFO* pstEMVInfo = NULL;

    FUN_IN;
    pstEMVInfo = &pstTransData->stEmvInfo;

    if (0 == Lib_EmvGetTlv(0x004F, aucBuf, sizeof(aucBuf), (unsigned int*)&ulLen))
    {
        Lib_Bcd2Asc(aucBuf, (char*) pstEMVInfo->aucAID, ulLen);
        pstEMVInfo->aucAID[ulLen * 2] = '\0';
        pstEMVInfo->ucAIDLen = strlen((char*)pstEMVInfo->aucAID);
        TRACE("aucAID: %s",pstEMVInfo->aucAID);
    }
    memset(aucBuf, 0, sizeof(aucBuf));

    if (0 == Lib_EmvGetTlv(0x009B, aucBuf, sizeof(aucBuf), (unsigned int*)&ulLen))
    {
        Lib_Bcd2Asc(aucBuf, (char*)pstEMVInfo->aucTSI, ulLen);
        pstEMVInfo->aucTSI[ulLen * 2] = '\0';
        TRACE("aucTSI: %s", pstEMVInfo->aucTSI);
    }

    if ( 0 ==Lib_EmvGetTlv(0x0095, aucBuf, sizeof(aucBuf), (unsigned int*) &ulLen))
    {
        Lib_Bcd2Asc(aucBuf, (char*)pstEMVInfo->aucTVR, ulLen);
        pstEMVInfo->aucTVR[ulLen * 2] = '\0';
        TRACE("aucTVR: %s", pstEMVInfo->aucTVR);
    }
    return RET_OK;
}

short EmvDemo(short cardMode, ST_TRANSDATA* pstTransData)
{
    ST_EMV_TRANS stTrans;
    ST_KERNEL_RESULT stResult;
    short errCode;
    unsigned char aucBuf[512];
    unsigned short uslen = 0;
    unsigned int ulLen;
    //unsigned char ucLen;
    u16 usTagList[50];
    unsigned int i;

    ST_TRANSINFO* pstTransInfo = NULL;
    ST_CARDINFO* pstCardInfo = NULL;
    ST_EMVINFO*  pstEMVInfo = NULL;

    pstTransInfo = &pstTransData->stTransInfo;
    pstCardInfo = &pstTransData->stCardInfo;
    pstEMVInfo = &pstTransData->stEmvInfo;

    memset((unsigned char *)&stTrans, 0, sizeof(stTrans));
    if (SCI_DETECETED == cardMode)
    {
        pstCardInfo->eEntryMode = MODE_SCI;
        //   0 -contact, 1 -Contacless
        stTrans.channelType = TRANS_CHN_TYPE_SCI;
    }
    else if (NFC_DETECETED == cardMode)
    {
        pstCardInfo->eEntryMode = MODE_CONTACTLESS;
        //  0 -contact, 1 -Contacless
        stTrans.channelType = TRANS_CHN_TYPE_NFC;
    }

    // mandatory online
    stTrans.forceOnline = 1;
    // do not perform card number confirmation
    stTrans.cardNoConfirm = 0;
    // Transaction Type  n2  9C   0-Pay for consumption
    stTrans.transType = 0;
    stTrans.currencyExp = 2;

    // Amount of the transaction n12  9F02
    memcpy(stTrans.transAmt, pstTransInfo->aucAmount, 6);
    TRACE("transAmt:");
    TRACEBUF(stTrans.transAmt, 6);

    if (strlen((const char *)pstTransInfo->aucCBAmt))
    {
        Lib_Asc2Bcd((char *)pstTransInfo->aucCBAmt, stTrans.transAmt, 12);
        TRACE("otherAmt:");
        TRACEBUF(stTrans.otherAmt, 6);
        stTrans.transType = 0x09;
    }
    else
    {
        memcpy(stTrans.otherAmt, "\x00\x00\x00\x00\x00\x00", 6);
    }

    // Set basic parameters for EMV transactions
    memcpy(stTrans.countryCode, "\x01\x56", 2);
    memcpy(stTrans.currencyCode, "\x01\x56", 2);
    stTrans.odaFlag = 1;

    // Date of the transaction n8  YYYYMMDD  9A
    memcpy(stTrans.transDate, "\x22\x08\x10", 3);
    // time of the transaction n6  hhmmss    9F21
    memcpy(stTrans.transTime, "\x13\x00\x01", 3);
    // Transaction serial number n6  9F41
    memcpy(stTrans.posSer, "\x00\x00\x01", 3);
    if (stTrans.channelType == TRANS_CHN_TYPE_NFC)
    {
        memcpy(stTrans.addition, "\xFF\x60\x02\x00\x84", 5);
        stTrans.additionLen =5;
    }

    TRACE("EMV kernel init\r\n");
    Lib_EmvKernelInit(&stEMVCallback);

    TRACE("start EMV process\r\n");
    // EMV process
    Lib_EmvProcess(&stTrans);

    TRACE("get EMV process result\r\n");
    // Get EMV process result
    memset((char*)&stResult, 0x00, sizeof(stResult));
    Lib_EmvGetKernelProcessResult(&stResult);

    TRACE("stResult:");
    TRACEBUF((unsigned char *)&stResult, sizeof(stResult));
    TRACE("ETAG trade status %x \r\n", stResult.ucStatus);

    if ((SDK_EMV_STATUS_SELECT_NEXT == stResult.ucStatus))
    {
        errCode = KernelErrorConvertSWCode(stResult.ucErrL1, stResult.ucErrL2, stResult.ucErrL3);
        TRACE("ETAG trade end %x \r\n", errCode);
        FUN_OUT;
        MessageBox((char*)TIP_EMV_TERMINATED, MB_CENTER|MB_COLOR_RED, 3000);
        return errCode;
    }
    else if (SDK_EMV_STATUS_TRY_AGAIN == stResult.ucStatus)
    {
        errCode = 2;
        return errCode;
    }
    else if (SDK_EMV_STATUS_END_APPLICATION == stResult.ucStatus)
    {
        if (stResult.ucStart == SDK_EMV_START_B)
        {
            errCode = 2;
            return errCode;
        }

        if (memcmp(stResult.aucSw1Sw2 ,"\x69\x85", 2) == 0)
        {
            UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);
            UITextAlign(ALIGN_M, LCD_OP_Y_S, DISP_MODE_NORMAL, TIP_6985);
            UIWaitKey(2000, 1);
        }

        errCode = KernelErrorConvertSWCode(stResult.ucErrL1, stResult.ucErrL2, stResult.ucErrL3);
        FUN_OUT;
        MessageBox((char*)TIP_EMV_TERMINATED, MB_CENTER|MB_COLOR_RED, 3000);
        return errCode;
    }
    else if (SDK_EMV_STATUS_TRY_ANOTHER_INTERFACE == stResult.ucStatus)
    {
        errCode = KernelErrorConvertSWCode(stResult.ucErrL1, stResult.ucErrL2, stResult.ucErrL3);
        TRACE("1ETAG trade end %x \r\n", errCode);
        FUN_OUT;
        return errCode;
    }

    if (0 == Lib_EmvGetTlv(TAG_9F12, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen))
    {
        TRACE ( "TAG_9F12:" );
        memcpy(pstEMVInfo->ucAppLable, aucBuf, ulLen);
    }
    else
    {
        if (0 == Lib_EmvGetTlv(TAG_0050, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen))
        {
            TRACE("TAG_0050:");
            memcpy(pstEMVInfo->ucAppLable, aucBuf, ulLen);
        }
    }
    TRACEBUF(pstEMVInfo->ucAppLable, ulLen);

    if (0 != Lib_EmvGetTlv(TAG_005A, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen))
    {
        if (TRANS_CHN_TYPE_NFC == stTrans.channelType)
        {
            Lib_EmvGetTlv(TAG_0057, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen);
        }
        else
        {
            TRACE("Get 5A fail.\n");
            errCode = 0x8502;
            FUN_OUT;
            MessageBox((char*)TIP_EMV_DATAINVALID, MB_CENTER|MB_COLOR_RED, 3000);
            return errCode;
        }
    }

    TRACE("PAN:");
    TRACEBUF(aucBuf, ulLen);
    // Converting PAN to asc save into a global variable, which is used when calculating pinblock
    Lib_Bcd2Asc(aucBuf, (char*)pstCardInfo->aucCardNo, ulLen);
    pstCardInfo->aucCardNo[ulLen * 2] = '\0';

    for (int i = 0; i < ulLen*2; i++)
    {
        if (pstCardInfo->aucCardNo[i] > '9')
        {
            pstCardInfo->aucCardNo[i] = '\0';
            break;
        }
    }
    pstCardInfo->ucCardLen = strlen((char*)pstCardInfo->aucCardNo);
    pstCardInfo->eCardType = GetCardType(pstCardInfo->aucCardNo);

    // Card Sequence Number
    memset(aucBuf, 0, sizeof(aucBuf));
    if (0 != Lib_EmvGetTlv(TAG_5F34, aucBuf, sizeof(aucBuf), (unsigned int *)&ulLen))
    {
        // TRACE("Failed to get card serial number.\n");
        // pstResp->usSwCode = SW8502;
        // return RET_ERR;
        aucBuf[0] = 0x00;
        ulLen = 1;
    }

    memcpy(&pstEMVInfo->aucPanSeqNoH, aucBuf, 1);
    TRACE ("Card Sequence Number: %x", pstEMVInfo->aucPanSeqNoH);

    // 55 field
    for (i = 0; i < 50; i++)
    {
        usTagList[i] = (stParConlist.aucTagList[i * 2] << 8) | stParConlist.aucTagList[i * 2 + 1];
    }

    memset (aucBuf, 0, sizeof(aucBuf));
    uslen = sizeof(aucBuf);
    if (0 != PackIcCardData((unsigned short *)usTagList, aucBuf, &uslen))
    {
        TRACE("pack emv data failed.\n");
        errCode = 0x8502;
        //MessageBox((char*)TIP_EMV_DATAINVALID, MB_CENTER|MB_COLOR_RED, 3000);
        return errCode;
    }
    PubLongToChar(uslen, 2, (char *)pstEMVInfo->aucICCData);

    memcpy(&pstEMVInfo->aucICCData[2], aucBuf, uslen);
    pstEMVInfo->aucICCData[uslen+2] = '\0';
    TRACE("emv data uslen: %d", uslen);
    TRACEBUF(pstEMVInfo->aucICCData, uslen);

    // Equivalent track2 data
    /*memset(aucBuf, 0, sizeof(aucBuf));
    if (0 != Lib_EmvGetTlv(TAG_0057, aucBuf, sizeof(aucBuf), &ulLen))
    {
        TRACE("Failed to get track2 data of magnetic tracks.\n");
        errCode = 0x8502;
        return errCode;
    }
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucTrack2Data, ulLen);
    pstCardInfo->ucTrack2Len = ulLen*2;
    pstCardInfo->aucTrack2Data[ulLen * 2] = '\0';

    pstCardInfo->ucTrack2Len = strlen((char*)pstCardInfo->aucTrack2Data);
    TRACE("track 2: %s", pstCardInfo->aucTrack2Data);

    memset(aucBuf, 0, sizeof(aucBuf));
    if (GetCardSevCode(pstCardInfo->aucTrack2Data, aucBuf, &ucLen))
    {
        Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucServiceCode, ucLen);
        pstCardInfo->aucServiceCode[ucLen * 2] = '\0';
        TRACE("aucServiceCode: %s", pstCardInfo->aucServiceCode);
    }*/

    if (TRANS_CHN_TYPE_NFC != stTrans.channelType)
    {
        // CARD EXPIRE DATE
        memset(aucBuf, 0, sizeof(aucBuf));
        if (0 != Lib_EmvGetTlv(TAG_5F24, aucBuf, sizeof(aucBuf), &ulLen))
        {
            TRACE("Failed to get card expire date.\n");
            errCode = 0x8502;
            return errCode;
        }
        Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucExpiredDate, ulLen);
        pstCardInfo->aucExpiredDate[ulLen * 2] = '\0';
        TRACE("Card expire Date: %s", pstCardInfo->aucExpiredDate);
    }
    else
    {
        // NFC card
        TRACE("NFC set: \r\n");
        if (SDK_EMV_CVM_ONLINE_PIN == (stResult.ucCvm & SDK_EMV_CVM_NA))
        {
            // Online PIN
            pstEMVInfo->ucCVMMode = SDK_EMV_CVM_ONLINE_PIN;
            memset(aucBuf, 0, sizeof (aucBuf));
            errCode =  EmvInputPIN(1, aucBuf, 0);
            if(RET_OK == errCode)
            {

            }
            else if (ERR_USER_CANCEL == errCode)
            {
                //MessageBox(TIP_USER_CANCEL, MB_CENTER|MB_COLOR_YELLOW, 3000);
                return errCode;
            }
            else
            {
                //MessageBox((char*)TIP_EMV_OFFLINEDEC, MB_CENTER|MB_COLOR_RED, 3000);
                return SDK_EMV_STATUS_DECLINED;
            }
        }
        else if (SDK_EMV_CVM_SIGNATURE == (stResult.ucCvm & SDK_EMV_CVM_NA))
        {
            // signature
            pstEMVInfo->ucCVMMode = SDK_EMV_CVM_SIGNATURE;
        }
        else if (SDK_EMV_CVM_CONFIRM_CODE_VERIFIED == (stResult.ucCvm & SDK_EMV_CVM_NA))
        {
            // Verification code
            pstEMVInfo->ucCVMMode = SDK_EMV_CVM_CONFIRM_CODE_VERIFIED;
        }
        else
        {
            pstEMVInfo->ucCVMMode = SDK_EMV_CVM_NO_CVM;
        }
    }

    // Non primary account without offline transactions, getting online password through PIN command
    // Contact card, the primary account for online transactions (successful offline transaction, no PIN return),
    // online transactions (with PIN return), offline to online transactions (successful offline PIN verification, but no PIN return, password obtained through PIN command)
    if ((SDK_EMV_STATUS_ONLINE_REQUEST == stResult.ucStatus) ||
        (SDK_EMV_STATUS_TC_TO_ARQC == stResult.ucStatus) || (SDK_EMV_STATUS_ARQC_ODA == stResult.ucStatus))
    {
        TRACE ( "Online request.\r\n" );
    }
    else if (SDK_EMV_STATUS_APPROVED == stResult.ucStatus)
    {
        // OFFLINE APPROVE
        TRACE ( "Transaction offline Approval.\r\n" );
        //MessageBox ( ( char* ) TIP_EMV_OFFLINEAPPROVE, MB_CENTER|MB_COLOR_GREEN, 3000 );
        return SDK_EMV_STATUS_DECLINED;
    }
    else
    {
        //offline decline
        TRACE("Transaction Decline.\r\n");
        //MessageBox((char*)TIP_EMV_OFFLINEDEC, MB_CENTER|MB_COLOR_RED, 3000);
        return SDK_EMV_STATUS_DECLINED;
    }

    TRACE("Transaction end\r\n");
    //GetEMVPrintData (pstTransData);

    return RET_OK;
}

// valide data
u8 GetCardValidity(unsigned char *pucTrackBuf, unsigned char *pucCardValidity, unsigned char *pucCardValidityLen)
{
    signed short sOffSet1;
    unsigned char *pucTrack2Buf;
    unsigned char ucLen;

    pucTrack2Buf = pucTrackBuf;

    // Find the position of the equal sign
    sOffSet1 = searchChar(pucTrack2Buf, '=', 0);
    if (sOffSet1 < 0)
    {
        return FALSE;
    }

    //+1, Starting bit of validity period
    sOffSet1++;

    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardValidity, 4, &ucLen, 0x0f);
    *pucCardValidityLen = ucLen;
    //Printf_STR_BUF("Card validity period : ", pucCardValidity, *pucCardValidityLen);
    return TRUE;
}


// PAN
static void GetPANfromTrackData(unsigned char *pucTrackBuf, unsigned char *pucCardId, unsigned char *pucCardIdLen)
{
    signed short sOffSet1, sOffSet2;
    unsigned char *pucTrack2Buf;

    pucTrack2Buf = pucTrackBuf;

    // Find the location of the ';' semicolon
    sOffSet1 = searchChar(pucTrack2Buf, ';', 0);
    if (sOffSet1 < 0)
    {
        return;
    }

    // Find the position of the equal sign
    sOffSet2 = searchChar(pucTrack2Buf, '=', sOffSet1);
    if (sOffSet2 < 0)
    {
        return;
    }

    // +1, starting position of the main account
    sOffSet1++;
    // The length of the main account
    sOffSet2 -= sOffSet1;

    if (sOffSet2 < 13 || sOffSet2 > 19)
    {
        // The length of the main account cannot be less than 13 and cannot be greater than 19
        return;
    }

    // As calculating pinblock, pan is used, so the PAN character is stored in the global variable
    memcpy(g_aucPAN, pucTrack2Buf + sOffSet1, sOffSet2);
    g_ucPANLen = sOffSet2;

    memcpy(aucMacPanAscii + 1, g_aucPAN, g_ucPANLen);
    aucMacPanAscii[0] = g_ucPANLen;
    //    Printf_STR_BUF("aucMacPanAscii : ", aucMacPanAscii, aucMacPanAscii[0] + 1);

    // CUT PAN ,change to bcd, if PAN is not even, the last digit will be filled with an F
    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardId, sOffSet2, pucCardIdLen, 0x0f);
    //Printf_STR_BUF("Main account: ", pucCardId, *pucCardIdLen);
}

//-----------------------------------------------------------------------------
// Name    : Track1Process
// Func    : 1th track data processing
// Paras   : pucSource[in],         Source data
//         : ucSourceLen[in],       Source data length
//         : pucDest[out],          Destination Data
//         : pucDestLen[out],       Destination Data length
//         : ucProcess[in],         Magnetic track data processing method
// Ret     : - 0 , success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
void Track1Process(u8 *pucSource, u8 ucSourceLen, u8 *pucDest, u8 *pucDestLen, u8 ucProcess)
{
    u8 aucBuf[128], ucLen = 0;
	u8 ucKeyIndex;
	if(g_ucSubAlgFlag == ALGORITHM_DUKPT)//DUKPT
	{
		ucKeyIndex = DUKPT_WK_TDK_REQUIRE;
	}
	else
	{
		ucKeyIndex = stParConlist.aucResver[2];
	}

    TRACE("Data length of track 1 : %d\r\n", ucSourceLen);
    TRACEBUF(pucSource, ucSourceLen);

    // 1th track data processing
    switch (ucProcess)
    {
    case 1:
        // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return;
        // break;

    case 2:
        // padded with 00 to a multiple of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;
        while (ucLen % 8)
        {
            aucBuf[ucLen++] = 0x00;
        }
        break;

    default:
        // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return;
        // break;
    }
    TRACE("Data length of track 1 after padding : %d", ucLen);
    TRACEBUF(aucBuf, ucLen);

    if (0 != DataEncryptProcess(aucBuf, pucDest, ucLen, ucKeyIndex))
    {
        memcpy(pucDest, aucBuf, ucLen);
    }
    *pucDestLen = ucLen;
    TRACE("Data length of track 1 after encryption : %d\r\n", *pucDestLen);
    TRACEBUF(pucDest, *pucDestLen);

    return;
}

short InputPIN ( ST_TRANSDATA* pstTransData )
{
    unsigned char ucKeyDown = KB_NULL;
    unsigned char ascPin[12] = {0}, ascPinLen = 0;
    unsigned int uiTimeout = 0;//, ulWaitTime;
    unsigned char aucInput[13]= {0};

    FUN_IN;
	UIClearRect ( 0, 2, LCD_MAX_SEG, MAX_COM_LINES-2 );

	UITextAlign ( ALIGN_L, 2, DISP_MODE_NORMAL, TIP_ENTER_PIN );

	uiTimeout = Lib_SysGetTimer() + 60000;  // timeout 60S
	while ( Lib_SysGetTimer() <= uiTimeout )
	{
		//IWDG_ReloadCounter();
		ucKeyDown = KbReadKey();
		if ( ucKeyDown == KB_NULL )
		{
			continue;
		}
		BeepOn();
		TRACE ( "%s:KeyScan = %x\r\n", __func__, ucKeyDown );
		if ( ( KB_DIGIT_ZERO <= ucKeyDown ) && ( ucKeyDown <= KB_DIGIT_NINE ) )
		{
			if ( ascPinLen < 12 )
			{
				ascPin[ascPinLen++] = ucKeyDown;
				aucInput[ascPinLen-1] = '*';
				aucInput[ascPinLen] = '\0';
				//SdkLcdDisStr(6 + ascPinLen, 2, 0, "*");
				UIText ( 14, 3, DISP_MODE_REVERSE, ( void* ) aucInput );
			}
			else
			{
				BeepOn();
				Lib_DelayMs (20);
				BeepOn();
			}
		}
		else if ( ucKeyDown == KB_CLEAR )
		{
			if ( ascPinLen > 0 )
			{
				memset ( ascPin, 0x00, sizeof ( ascPin ) );
				ascPinLen = 0;
				aucInput[ascPinLen] = '\0';
				//SdkLcdDisStr(0, 2, 1, " ");
				UIText ( 14, 3, DISP_MODE_REVERSE, ( void* ) aucInput );
			}
			else
			{
				BeepOn();
				Lib_DelayMs ( 20 );
				BeepOn();
			}
		}
		else if ( ucKeyDown == KB_ENTER )
		{
            // cannot less than 4 digits
			if ( ascPinLen < 4 )
			{
				BeepOn();
				Lib_DelayMs ( 20 );
				BeepOn();
				continue;
			}
			else
			{
				SetPinblock ( ascPin, ascPinLen,pstTransData );
				return RET_OK;
			}
		}
		else if ( ucKeyDown == KB_CANCEL )
		{
            // cancel
			return UI_INPUT_CANCEL;
		}
		else
		{
			BeepOn();
		}
		TRACE ( "ASC PassWord : " );
		TRACE ( ( void* ) ascPin, ascPinLen );
	}

	return UI_INPUT_TIMEOUT;

}


short MccDemo(ST_TRANSDATA* pstTransData)
{
    unsigned char aucBuf[512];
    unsigned char ucLen = 0, ucTmpLen = 0;
    //unsigned char aucPanBuf[24];
    short errCode = 0;
    u8 aucTmpBuf[SDK_TRACK_NUM][MSC_TMP_BUF_LEN];
    ST_CARDINFO* pstCardInfo = NULL;
    short sRet = 0;
    FUN_IN;

    pstCardInfo = &pstTransData->stCardInfo;

	signed short sOffSet1 = 0, sOffSet2 = 0;

	if (!astCiTrack[SDK_TRACK2].ucTrackValid)
	{
		errCode = 0x8004;
		return errCode;
	}

//DO_NEXT:
    TRACE("TRACK2 length = %d, TRACK3 length = %d\r\n", astCiTrack[SDK_TRACK2].ucTrackLen, astCiTrack[SDK_TRACK3].ucTrackLen);
    TRACE ( "TRACK2 = %s, TRACK3 = %s\r\n", astCiTrack[SDK_TRACK2].pucTrackBuf, astCiTrack[SDK_TRACK3].pucTrackBuf );

    pstCardInfo->eEntryMode = MODE_MSC;
    memset(aucBuf,0,sizeof(aucBuf));
    //get PAN
    GetPANfromTrackData(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    //PanProcess(aucBuf, ucLen, aucPanBuf, &ucTmpLen, 0);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucCardNo, ucLen);
    pstCardInfo->aucCardNo[ucLen * 2] = '\0';
    TRACE("CardNo: %s", pstCardInfo->aucCardNo);

    // remove padding F
    while (pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] > '9')
    {
        pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] = '\0';
    }
    pstCardInfo->ucCardLen = strlen((char*)pstCardInfo->aucCardNo);
    pstCardInfo->eCardType = GetCardType(pstCardInfo->aucCardNo);

    memset(aucBuf, 0, sizeof(aucBuf));

    // Patch: Malaysia GHL track data format is : track1 | track2 | track3
    if (0 == memcmp(stParConlist.aucName, "\x47\x48\x4C\x00", 4))
    {
        ucTmpLen = 0;

    // 1th track
    if (astCiTrack[SDK_TRACK1].ucTrackValid)
    {
        // Find the position of the %
        sOffSet1 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '%', 0);
        // Find the position of the ?
        sOffSet2 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '?', 0);
        if ((sOffSet1 >= 0) && (sOffSet2 >= 0) && (sOffSet2 > sOffSet1))
        {
            sOffSet1++;
            ucLen = sOffSet2 - sOffSet1;
            memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK1].pucTrackBuf + sOffSet1, ucLen);
            ucTmpLen += ucLen;
        }
    }

        // Separator
        aucTmpBuf[SDK_TRACK2][ucTmpLen++] = '|';

        // 2th track
		memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK2].pucTrackBuf + 1, astCiTrack[SDK_TRACK2].ucTrackLen - 2);
		ucTmpLen += astCiTrack[SDK_TRACK2].ucTrackLen - 2;

        // Separator
		aucTmpBuf[SDK_TRACK2][ucTmpLen++] = '|';

        // 3th track
		if (astCiTrack[SDK_TRACK3].ucTrackValid)
		{
			memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK3].pucTrackBuf + 1, astCiTrack[SDK_TRACK3].ucTrackLen - 2);
			ucTmpLen += astCiTrack[SDK_TRACK3].ucTrackLen - 2;
		}
	}
#if 0
    else
    {
        // 2th track data, ucTrackLen includes; And ? length, ucTrackLen-2 has been removed; And ? , just leave only data
		Lib_AscNum2bcd(astCiTrack[SDK_TRACK2].pucTrackBuf + 1, aucTmpBuf[SDK_TRACK2], astCiTrack[SDK_TRACK2].ucTrackLen - 2, &ucTmpLen, 0x0f);
	}
	if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], ucTmpLen, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK2, stParConlist.ucTrackPro))
	{
		errCode = 0x8025;
		g_usTradeMsgLen = 0;
		return errCode;
	}
	g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x02;
	g_usTradeMsgLen++;
	g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
	g_usTradeMsgLen++;
	memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
	g_usTradeMsgLen += ucLen;

    // 3th track
	if (astCiTrack[SDK_TRACK3].ucTrackValid)
	{
		Lib_AscNum2bcd(astCiTrack[SDK_TRACK3].pucTrackBuf + 1, aucTmpBuf[SDK_TRACK2], astCiTrack[SDK_TRACK3].ucTrackLen - 2, &ucTmpLen, 0x0f);
		if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], ucTmpLen, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK3, stParConlist.ucTrackPro))
		{
			errCode = 0x8025;
			g_usTradeMsgLen = 0;
			return errCode;
		}
		g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x03;
		g_usTradeMsgLen++;
		g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
		g_usTradeMsgLen++;
		memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
		g_usTradeMsgLen += ucLen;
	}
	else
	{
		if (5 == stParConlist.ucTrackPro)
		{
            // After formatting, it needs to be returned
			// 56 bytes 0XFF
			memset(aucTmpBuf[SDK_TRACK2], 0xFF, sizeof(aucTmpBuf[SDK_TRACK2]));
			if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], 56, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK3, stParConlist.ucTrackPro))
			{
				errCode = 0x8025;
				g_usTradeMsgLen = 0;
				return errCode;
			}
			g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x03;
			g_usTradeMsgLen++;
			g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
			g_usTradeMsgLen++;
			memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
			g_usTradeMsgLen += ucLen;
		}
	}
#endif
    else
    {
        memset(aucBuf,0,sizeof(aucBuf));
        memcpy ( aucBuf,astCiTrack[SDK_TRACK2].pucTrackBuf+1,astCiTrack[SDK_TRACK2].ucTrackLen-2 );

        for ( int i = 0; i < astCiTrack[SDK_TRACK2].ucTrackLen-2; i++ )
        {
            // '=' convert to D
            if ( aucBuf[i] == 0x3D )
            {
                aucBuf[i] = 0x44;
            }
        }
    }
    memcpy(pstCardInfo->aucTrack2Data, aucBuf, astCiTrack[SDK_TRACK2].ucTrackLen-2);
    pstCardInfo->ucTrack2Len = astCiTrack[SDK_TRACK2].ucTrackLen-2;


    memset(aucBuf,0,sizeof(aucBuf));
    // card valid date
    GetCardValidity(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucExpiredDate, ucLen);
    TRACE("Card ExpiredDate: %s", pstCardInfo->aucExpiredDate);

    memset(aucBuf,0,sizeof(aucBuf));
    // Get service code
    GetCardSevCode(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucServiceCode, ucLen);
    TRACE("Card ServiceCode: %s", pstCardInfo->aucServiceCode);

	if (astCiTrack[SDK_TRACK1].ucTrackValid)
	{
        //1th track
        // Find the position of the %
		sOffSet1 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '%', 0);
		if (sOffSet1 < 0)
		{
			errCode = 0x8025;
			g_usTradeMsgLen = 0;
			return errCode;
		}

        // Find the position of the ?
		sOffSet2 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '?', sOffSet1);
		if (sOffSet2 < 0)
		{
			errCode = 0x8025;
			g_usTradeMsgLen = 0;
			return errCode;
		}
		TRACE("TRACK1 length = %d, sOffSet1 = %d, sOffSet2 = %d\r\n", astCiTrack[SDK_TRACK1].ucTrackLen, sOffSet1, sOffSet2);

		sOffSet1++;
        // 1th track data length
		sOffSet2 -= sOffSet1;
		Track1Process(astCiTrack[SDK_TRACK1].pucTrackBuf + sOffSet1, sOffSet2, aucTmpBuf[SDK_TRACK3], &ucLen, stParConlist.aucResver[1]);
	}

	sRet = InputPIN ( pstTransData );

	return sRet;
}

#if defined(DUKPT_EN)
void DukptTest(void)
{
    unsigned char aucBuf[512];
    unsigned short data_len =0;
    unsigned char ksn[10]={0xFF,0xFF,0x98,0x76,0x54,0x32,0x10,0x00,0x00,0x00};
    //KSN-FFFF9876543210E10005
    //BDK-C1D0F8FB4958670DBA40AB1F3752EF0D IPEK-74E912996F1245CC1CF6F5C1E02FD05A
    // Encrypted data with transmission key (0xff ...)
    //unsigned char ipek[16]={0x31,0x88,0xFB,0xDF,0x7A,0x1B,0x99,0x15,0xC7,0x90,0xA9,0x9B,0x2E,0xFF,0xEA,0x97};
    // Encrypted data with transmission key (0xff ...)
    unsigned char bdk[16]={0x95,0xBB,0x5C,0x8A,0x49,0xAE,0xC9,0x71,0x4C,0x3E,0x09,0x65,0xC8,0xF6,0x9C,0x93};
    unsigned char test_pinblock[8]={0x04,0x12,0x43,0x89,0x99,0x9A,0xAA,0xAB};//04124389999AAAAB
    TRACE("Start test dukpt!!!\r\n");
    // Switch to DUKPT algorithm
    SetAlgorithmMode(2);
    // Set Algorithm Mode
    SetEcbOrCbcMode(1);
    // Set initial vector
    //SetCipherInitializationVector(iv);
    //inject IPEK
    //Lib_DukptKeyInject(0,0,ksn,0,ipek,16);
    //inject BDK
    Lib_DukptKeyInject(0,0,ksn,0,bdk,16);

    //DUKPT test
    memcpy(aucBuf,test_pinblock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    data_len = 8;
    Lib_DukptEncryptOrDecrypt(0,0,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);

    memcpy(aucBuf,test_pinblock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    Lib_DukptEncryptOrDecrypt(0,1,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);

    memcpy(aucBuf,test_pinblock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    Lib_DukptEncryptOrDecrypt(0,2,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);

    memcpy(aucBuf,test_pinblock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    Lib_DukptEncryptOrDecrypt(0,3,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);

    memcpy(aucBuf,test_pinblock,8);
    TRACE("Start dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
    Lib_DukptEncryptOrDecrypt(0,4,0,aucBuf,&data_len);
    TRACE("End dukpt encrypt\r\n");
    TRACEBUF(aucBuf,8);
}
#endif

// Used by the LedsOn function
#define LED_BIT_BLUE          0x01   // Blue LED
#define LED_BIT_YELLOW        0x02   // Yellow LED
#define LED_BIT_GREEN         0x04   // Green LED
#define LED_BIT_RED           0x08   // Red LED
// LED control
short LedsControlOn(unsigned char ucLeds)
{
    if(ucLeds & LED_BIT_BLUE)
        Lib_LedControl(LED_TIP1, 1);
    else
        Lib_LedControl(LED_TIP1, 0);

    if(ucLeds & LED_BIT_YELLOW)
        Lib_LedControl(LED_TIP2, 1);
    else
        Lib_LedControl(LED_TIP2, 0);

    if(ucLeds & LED_BIT_GREEN)
        Lib_LedControl(LED_TIP3, 1);
    else
        Lib_LedControl(LED_TIP3, 0);

    if(ucLeds & LED_BIT_RED)
        Lib_LedControl(LED_TIP4, 1);
    else
        Lib_LedControl(LED_TIP4, 0);

    return 0;
}


//-----------------------------------------------------------------------------
// Name    : CbAppDisplayMsg
// Func    : Display corresponding content based on pstUird->ucIdentify
// Paras   : pstUird[in],         UIRD
//         : ucOpsStatus[in],     OPS status
// Ret     : - 0 , success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
short CbAppDisplayMsg(ST_UIRD *pstUird, uint8 ucOpsStatus)
{
    uint32 ulHoldTime = 0, ulStartTime = 0, ulProcessTime = 0;

    if (NULL != pstUird)
    {
        if (UIRD_MSGID_NA != pstUird->ucIdentify)
        {
            // Need to display, clear the screen first
            //Lib_LcdClrScreen();

            UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);
        }

        // LCD
        switch (pstUird->ucIdentify)
        {
        case UIRD_MSGID_CARD_READ_OK:
            // Card reading OK
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Card read OK");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "Please remove card");
            break;

        case UIRD_MSGID_TRY_AGAIN:
            // retry
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Present card again");
            break;

        case UIRD_MSGID_PLEASE_LOOK:
            // retry
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Please look at your");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "handset to obtain further");
            UITextAlign(ALIGN_M, 3, DISP_MODE_NORMAL, "instructions");
            break;

        case UIRD_MSGID_PLEASE_AUTH:
            // retry
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Please authenticate");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "yourself to your device");
            UITextAlign(ALIGN_M, 3, DISP_MODE_NORMAL, "and try again");
            break;

        case UIRD_MSGID_APPROVED:
            // Transaction Approval
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Approved");
            break;

        case UIRD_MSGID_REMOVE_CARD:
            // Remove Card
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Please remove card");
            break;

        case UIRD_MSGID_WELCOME:
            // Idle welcome tips
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Welcome");
            break;

        case UIRD_MSGID_PRESENT_CARD:
            // Please wave your card
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Present card");
            break;

        case UIRD_MSGID_PROCESSING:
            // Processing
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Processing");
            break;

        case UIRD_MSGID_PRESENT_ONE_CARD:
            // Card Conflict
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Please present");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "one card only");
            break;

        case UIRD_MSGID_APPROVED_SIGN:
            // Transaction Approval - Signature
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Approved");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "Please sign");
            break;

        case UIRD_MSGID_DECLINED:
            // Transaction declined
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Not Authorised");
            break;

        case UIRD_MSGID_ERROR_OTHER_CARD:
            // Error - Other Cards
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Insert, swipe or try");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "another card");
            break;

        case UIRD_MSGID_INSERT_CARD:
            // Insert Card
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Please insert card");
            break;

        case UIRD_MSGID_CARD_ERROR:
            // Card error JCB Bulletin No.012
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Card Error");
            break;

        case UIRD_MSGID_SEE_PHONE:
            // Check your phone
            //2021-11-05
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "See your Phone for");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "Instructions");
            break;

        case UIRD_MSGID_AUTH_PLEASE_WAIT:
            // Authorising - Please Wait
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Authorising");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "Please Wait");
            break;

        case UIRD_MSGID_PIN_OVER_LIMIT:
            // PIN attempt exceeded limit
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Card read OK");
            break;

        case UIRD_MSGID_PIN_LAST_TRY:
            // PIN Last Attempt
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "PIN Last Try!");
            break;

        case UIRD_MSGID_PIN_VERIFY_OK:
            // PIN auth successful
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "PIN Ok.");
            break;

        case UIRD_MSGID_PIN_VERIFY_ERR:
            // PIN auth failed
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "PIN Error!");
            break;

        case UIRD_MSGID_PLEASE_ENTER_PIN:
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Enter PIN");
            break;

        case UIRD_MSGID_PLEASE_INSERT_SWIPE_CARD:
            UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Insert or swipe card");
            UITextAlign(ALIGN_M, 2, DISP_MODE_NORMAL, "Try another interface");
            break;

        case UIRD_MSGID_CLEAR_DISPLAY:
            // Clear screen
            break;

        default:                    // N/A
            break;
        }
    }

    // LED
    if ((OPS_STATUS_END_APPLICATION == (ucOpsStatus & OPS_STATUS_NA))           // End application
        || (OPS_STATUS_DECLINED == (ucOpsStatus & OPS_STATUS_NA))               // Transaction declined
        || (OPS_STATUS_TRY_ANOTHER_INTERFACE == (ucOpsStatus & OPS_STATUS_NA))  // Other interfaces
        )
    {
        LedsControlOn(LED_BIT_RED);
    }
    else if (NULL != pstUird)
    {
        // LED
        switch (pstUird->ucStatus)
        {
        case UIRD_STATUS_READY_TO_READ:
            // Preparing to read card
            LedsControlOn(LED_BIT_BLUE);
            break;

        case UIRD_STATUS_PROCESSING:
            // in processing
            LedsControlOn(LED_BIT_BLUE|LED_BIT_YELLOW);
            break;

        case UIRD_STATUS_CARD_READ_SUCCESS:
            // Card reading successful
            LedsControlOn(~LED_BIT_RED);
            break;

        case UIRD_STATUS_PROCESSING_ERROR:
            // Processing error
            LedsControlOn(LED_BIT_RED);
            break;

        default:
            // N/A
            break;
        }
    }

    // Hold Time
    if (NULL != pstUird)
    {
        // The unit is 100ms
        ulHoldTime = Lib_Bcd2Int(pstUird->aucHoldTime, sizeof(pstUird->aucHoldTime)) * 100;
    }

    ulStartTime = Lib_SysGetTimer();
    TRACE("ulStartTime = 0x%08X\n", ulStartTime);

    if ((NULL != pstUird) && (UIRD_STATUS_CARD_READ_SUCCESS == pstUird->ucStatus))
    {
        Lib_Beep(500);
        BeepOn();
    }
    else
    {
        if (((NULL != pstUird) && (UIRD_STATUS_PROCESSING_ERROR == pstUird->ucStatus))
            || (OPS_STATUS_END_APPLICATION == (ucOpsStatus & OPS_STATUS_NA))        // End application
            || (OPS_STATUS_DECLINED == (ucOpsStatus & OPS_STATUS_NA))               // Transaction declined
            || (OPS_STATUS_TRY_ANOTHER_INTERFACE == (ucOpsStatus & OPS_STATUS_NA))  // Other interfaces
            )
        {
            BeepOn();
        }
    }

    ulProcessTime = Lib_SysGetTimer() - ulStartTime;
    TRACE("ulProcessTime = %d; ulHoldTime = %d\n", ulProcessTime, ulHoldTime);

    if (ulHoldTime > ulProcessTime)
    {
        Lib_DelayMs(ulHoldTime - ulProcessTime);
    }

    LedsControlOn(0);

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : CbAppConfirmHolderInfo
// Func    : get the comparison results of the cardholder's identification information.
// Paras   : pucInfo[in],       Cardholder's ID information (ID type+ID number)
//         : uiInfoLen[in],     Comparison result: 0 identification does not match, 1 identification matches
//         : pucRes[in],     OPS status
// Ret     : - 0 , success
//           - <0, failure
// Desc    : hook
//-----------------------------------------------------------------------------
short CbAppConfirmHolderInfo(uint8 * pucInfo, uint32 uiInfoLen, uint8 *pucRes)
{
    return RET_OK;
}



