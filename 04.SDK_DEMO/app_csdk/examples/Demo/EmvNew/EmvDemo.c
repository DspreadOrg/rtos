/**********************************************************************************************************
 * Copyright(C)2018,
 * File Name     : EmvDemo.c
 * Version       : v1.0
 * Author        :
 * Description   : EMV application sample apis
 **********************************************************************************************************/
/**************************** INCLUDE FILES BEGIN **************************/

#include "EmvDemo.h"
#include "Key.h"
#include "Ui.h"
#include "Common.h"
#include <math.h>
#include <string.h>
#include "Card.h"
#include "VAN_security.h"
#include "MessageBox.h"
#include "Idle.h"
#include "Trade.h"
#include "LanguageEN.h"
#include "Communication.h"
#include "CardTrade.h"

#include "api_lvgl.h"
#include "EmvUtils.h"
#include "app_emv.h"

extern ST_MSC_TRACK astCiTrack[TRACK_NUM];

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
static unsigned char aucMacPanAscii[30];



#define MSC_TMP_BUF_LEN 1000 //

ST_PARAMETER_CONFIG_LIST stParConlist;
ST_PARAMETER_TRANSACTION_PRESET stTrasactionSet;

unsigned char g_aucTradeMsgBuf[1024];
unsigned short g_usTradeMsgLen = 0;

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
// static unsigned char aucMacPanAscii[30];

static u8 GetCardType(unsigned char *pucPan)
{
    int iBin = 0;
    unsigned char ucBuf[8] = {0};
    unsigned char ucCardType = 0;

    FUN_IN;
    memcpy(ucBuf, pucPan, 8);
    iBin = atoi((char *)ucBuf);
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

// card transaction demo
short CardDemo(short cardMode, ST_TRANSDATA *pstTransData)
{
    short errCode;

    switch (cardMode)
    {
    case SCI_DETECETED: // Contact card
    case NFC_DETECETED: // Contacless card
        TRACE("Start EMV Demo!!!\r\n");
        errCode = EmvDemo(cardMode, pstTransData);
        break;

    case MSC_DETECETED:
        // Magnetic stripe cards
        TRACE("Start Mcc Demo!!!\r\n");
        errCode = MccDemo(pstTransData);
        break;

    default:
        // Invalid card
        errCode = 0x8001;
        break;
    }

    TRACE("errCode %x\r\n", errCode);
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

void _lv_offline_pinpad_callback(unsigned char input_count, const unsigned char ucLastTimes)
{
    unsigned char aucInput[13] = {0};
    memset(aucInput, '*', input_count);

    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, "Off-Line Pin:");

    UIText(14, 3, DISP_MODE_NORMAL, (void *)aucInput);
}
static int input_offline_pin(EMV_INPUT_PIN_TYPE cType, char *offlinePin)
{ 
    int ret = -1;
    //unsigned char ucLastTimes;
    unsigned char aucPinBlock[12+1] = {0};
    pin_param param = { 0 };
    param.min_input_count = 4;
    param.max_input_count = 6;
    param.timeout = 20;
    param.is_bypass = 1;
    Lib_PciSetPinpadUI(_lv_offline_pinpad_callback);
    ret = Lib_PciOfflinePinpad(param, 1, (unsigned char *)offlinePin);
    TRACE("Emv Lib_PciOfflinePinpad ret = %d", ret);
    
    if (ret == -1029)
    {
        TRACE("Emv Lib_PciOfflinePinpad cancel");
        return -1;
    }
    else if (ret == -1031) // by pass
    {
        return 0;
    }

    TRACEBUF(aucPinBlock, sizeof(aucPinBlock));

    char acPlainPinAsc[12+1] = {0};
    memset(acPlainPinAsc, 0, sizeof(acPlainPinAsc));
    unsigned char ucPinLen = aucPinBlock[0] & 0x0F;
    Lib_Bcd2Asc(aucPinBlock+1, acPlainPinAsc, ucPinLen/2);
    if (ucPinLen % 2 != 0)
    {
        TRACE("Emv ucPinLen = %d", ucPinLen);
        Lib_Bcd2Asc(aucPinBlock+1+ucPinLen/2, acPlainPinAsc+ucPinLen-1, 1);
        acPlainPinAsc[ucPinLen] = 0;
    }

    TRACE("Emv acPlainPinAsc = %s", acPlainPinAsc);
    memcpy(offlinePin, acPlainPinAsc, strlen(acPlainPinAsc));
    // TRACEBUF()
    return strlen(acPlainPinAsc);
}

void _lv_online_pinpad_callback(unsigned char input_count, const unsigned char ucLastTimes)
{
    unsigned char aucInput[13] = {0};
    memset(aucInput, '*', input_count);

    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, "Please enter PIN:");

    UIText(14, 3, DISP_MODE_NORMAL, (void *)aucInput);
}
static int input_online_pin(char *pan,unsigned char *pinBlock)
{
    pin_param param = { 0 };
    param.min_input_count = 4;
    param.max_input_count = 6;
    param.timeout = 20;
    param.is_bypass = 1;

    Lib_PciSetPinpadUI(_lv_online_pinpad_callback);
    int ret = Lib_PciOnlinePinpad(param, 0);
    TRACE("Emv Lib_PciOnlinePinpad ret = %d", ret);
    if (ret == -1029)
    {
        TRACE("Emv Lib_PciOnlinePinpad cancel");
        return -1;
    }
    else if (ret == -1031)
    {
        return 0;
    }

    Lib_PciGetPin(KEYID_TPK, (unsigned char *)pan, 0, pinBlock);
    TRACEBUF(pinBlock, sizeof(pinBlock));
    return 8;
}
static int EmvInputPin(EMV_INPUT_PIN_TYPE cType, char *pan, char *pszPin)
{
    if(cType == EMV_ONLINEPIN_INPUT)
        return input_online_pin(pan, (unsigned char *)pszPin);
    else
        return input_offline_pin(cType, pszPin);    
}

static int EmvSelectApp(AidCandidate_t *pList, int listNum)
{
    //unsigned char* pnode = 0;
    //unsigned char* pchild = 0;
    //unsigned int total = 0;
    //const unsigned char* option = 0;
    //char disp[64] = {0};
    int index = -1;

    UIClearAllScreenLvgl();
    for(int i = 0; i < listNum; i++)
    {
        UITextAlign(ALIGN_L, i, DISP_MODE_NORMAL, (char *)pList[i]._lable);
    }
    //60s timeout
    unsigned int uiTimeout = Lib_SysGetTimer() + 60 * 1000;
    unsigned char ucKeyDown = KB_NULL;
    while(Lib_SysGetTimer() < uiTimeout)
    {
        ucKeyDown = KbReadKey();
        if (ucKeyDown == KB_NULL)
        {
            continue;
        }

        if ((KB_DIGIT_ZERO <= ucKeyDown) && (ucKeyDown <= KB_DIGIT_NINE))
        {
            index = ucKeyDown - KB_DIGIT_ZERO - 1;
            break;
        }
        else if (ucKeyDown == KB_CLEAR)
        {
            index = -1;
        }
    }

    TRACE("Emv app user select confirm index = %d", index);
    return index;
}

static int EmvProcessDisp(EmvKernelDisp type)
{
    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);
    switch (type)
	{
	case EMV_DISP_READING_CARD:
        UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, "Card reading...");
		break;
	case EMV_DISP_SEE_PHONE:
		UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL,(char*)"Please check your phone");
		Lib_DelayMs(5000);
		break;
	case EMV_DISP_REMOVE_CARD:
		UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL,(char*)"Please remove card");
		break;
	case EMV_DISP_NFC_RETAP:
		UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL,(char*)"Please retap card");
		break;
	case EMV_DISP_OFFLINE_PIN_BLOCKED:
		UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL,(char*)"Offline PIN Blocked");
		Lib_DelayMs(5000);
		break;	
	default:
		break;
	}
	return 0;
}

void disp_online_process()
{
    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);
    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL,(char*)"Online Processing...");
}
int onlineProcess(EmvOnlineData_t* pOnlineData){

    FUN_IN;
    TRACE("start onlineProcess");
    disp_online_process();
    memcpy(pOnlineData->iccResponse,"00",2);
	// memcpy(pOnlineData->ackdata,"\x91\x0A\xD1\x7F\x1E\x5F\x93\xD7\x12\xC0\x30\x30\x71\x12\x9F\x18\x04\xCA\xFE\x00\x01\x86\x09\x84\x24\x00\x00\x04\x15\x49\x9D\x67",32);
	// pOnlineData->ackdatalen = 32;

	// memcpy(pOnlineData->ackdata,"\x91\x10\x0D\xFA\x99\xD3\x80\x80\x40\x00\xFF\xFE\x00\x00\x00\x00\x10\x00",18);
	// pOnlineData->ackdatalen = 18;
		Lib_DelayMs(3000);
   
    return 0;
}

static void Initialize_EMV_CallBackFun(EmvCallBack_t *pcallbackfun)
{
	pcallbackfun->EMV_AidSelect = EmvSelectApp;
	pcallbackfun->EMV_SetKernelId = NULL;
	pcallbackfun->EMV_ConfirmCardInfo = NULL;
	pcallbackfun->EMV_InputPasswd = EmvInputPin;
	pcallbackfun->EMV_CertConfirm = NULL;
	pcallbackfun->EMV_ProcessDisp = EmvProcessDisp;
	pcallbackfun->EMV_OnlineProcess = onlineProcess;
	pcallbackfun->EMV_AfterSelectApp = NULL;
	pcallbackfun->EMV_AfterReadRecord = NULL;
}

void EmvL2_Init(){
    EmvCallBack_t t_callbackfun;
    Initialize_EMV_CallBackFun(&t_callbackfun);
    Emv_KernelInit(t_callbackfun);
}

void disp_result(int resultType,char *szResult)
{
    // UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);
    // UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, szResult);
    // LvUiWaitKey(3000);

    LVGL_RESULT result = { 0 };
    result.timeout = 3000;
    result.msg = (char *)szResult;

    if(resultType)
    {
        result.img = SUCCESS_IMG;
        result.title = TIP_SUCCESS;
        result.fontColor = LV_COLOR_GREEN;
    }
    else
    {
        result.img = FAILURE_IMG;
        result.title = TIP_FAILURE;
        result.fontColor = LV_COLOR_RED;
    }
    LvUiShowResult(&result);
}
short EmvDemo(short cardMode, ST_TRANSDATA *pstTransData)
{
    FUN_IN;
    int ret = -1;
    EMV_L2_Return nEmvRet = APP_RC_EMV_DENAIL;
    EmvL2_Init();
    memcpy(pstTransData->trans_amount,"000000010000",12);
    pstTransData->cardDetectType = cardMode;
    pstTransData->trans_type = EMV_L2_SALE;
    nEmvRet = Emv_Process(pstTransData);//emv_kernel_start(trans_data, trans_data_len);

    if(cardMode == NFC_DETECETED && nEmvRet == APP_RC_COMPLETED)
    {
		// do contactless trans online request
		EmvOnlineData_t emvOnlineData;
		memset(&emvOnlineData,0,sizeof(EmvOnlineData_t));
		ret = onlineProcess(&emvOnlineData);
		if(ret == 0 && memcmp(emvOnlineData.iccResponse,"00",2) == 0)
		{
			if(Emv_CheckContactlessDoubleTap(&emvOnlineData))
				ret = Emv_SetContactlessOnlineResult(&emvOnlineData);
            if(ret == 0)
                nEmvRet = APP_RC_COMPLETED;
            else
                nEmvRet = APP_RC_EMV_DENAIL;
		}
        else
        {
            nEmvRet = APP_RC_EMV_DENAIL;
        }
    }

    switch(nEmvRet)
    {
        case APP_RC_COMPLETED:
            disp_result(1,"Trans Success");
            break;
        case APP_RC_TERMINAL:
        case APP_RC_NFC_TERMINAL:
            disp_result(0,"Trans Terminal");
            break;
        case APP_RC_CANCEL:
            disp_result(0,"Trans Cancel");
            break;
        case APP_RC_EMV_APP_BLOCK:
            disp_result(0,"App Block");   
            break;
        case APP_RC_EMV_DENAIL:
            disp_result(0,"Trans Denail");
            break;
        case APP_RC_CARD_NOT_SUPPORT:
            disp_result(0,"Card Not Support");
            break;
        case APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE:
            disp_result(0,"Trans Try Another Interface");
            break;
        case APP_RC_NFC_TRY_AGAIN:
            disp_result(0,"NFC Try Again");
            break;
        case APP_RC_TRANS_REVERSEL:
            //start reversel process
            disp_result(0,"Pls start reversel process");
            break;
        default:
            disp_result(0,"Trans Fail");
            break;
    }

    // const char *str_version = emv_kernel_get_version();
    // TRACE("EMV get version: %s", str_version);
    if(nEmvRet == APP_RC_COMPLETED)
        ret = 0;
    else
        ret = -1;    
    return ret;
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
    // Printf_STR_BUF("Card validity period : ", pucCardValidity, *pucCardValidityLen);
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
    // Printf_STR_BUF("Main account: ", pucCardId, *pucCardIdLen);
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
    if (g_ucSubAlgFlag == ALGORITHM_DUKPT) // DUKPT
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

short InputPIN(ST_TRANSDATA *pstTransData)
{
    unsigned char ucKeyDown = KB_NULL;
    unsigned char ascPin[12] = {0}, ascPinLen = 0;
    unsigned int uiTimeout = 0; //, ulWaitTime;
    unsigned char aucInput[13] = {0};

    FUN_IN;
    UIClearRect(0, 2, LCD_MAX_SEG, MAX_COM_LINES - 2);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, TIP_ENTER_PIN);

    uiTimeout = Lib_SysGetTimer() + 60000; // timeout 60S
    while (Lib_SysGetTimer() <= uiTimeout)
    {
        // IWDG_ReloadCounter();
        ucKeyDown = KbReadKey();
        if (ucKeyDown == KB_NULL)
        {
            continue;
        }
        BeepOn();
        TRACE("%s:KeyScan = %x\r\n", __func__, ucKeyDown);
        if ((KB_DIGIT_ZERO <= ucKeyDown) && (ucKeyDown <= KB_DIGIT_NINE))
        {
            if (ascPinLen < 12)
            {
                ascPin[ascPinLen++] = ucKeyDown;
                aucInput[ascPinLen - 1] = '*';
                aucInput[ascPinLen] = '\0';
                // SdkLcdDisStr(6 + ascPinLen, 2, 0, "*");
                UIText(14, 3, DISP_MODE_REVERSE, (void *)aucInput);
            }
            else
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
            }
        }
        else if (ucKeyDown == KB_CLEAR)
        {
            if (ascPinLen > 0)
            {
                memset(ascPin, 0x00, sizeof(ascPin));
                ascPinLen = 0;
                aucInput[ascPinLen] = '\0';
                // SdkLcdDisStr(0, 2, 1, " ");
                UIText(14, 3, DISP_MODE_REVERSE, (void *)aucInput);
            }
            else
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
            }
        }
        else if (ucKeyDown == KB_ENTER)
        {
            // cannot less than 4 digits
            if (ascPinLen < 4)
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
                continue;
            }
            else
            {
                // SetPinblock(ascPin, ascPinLen, pstTransData);
                return RET_OK;
            }
        }
        else if (ucKeyDown == KB_CANCEL)
        {
            // cancel
            return UI_INPUT_CANCEL;
        }
        else
        {
            BeepOn();
        }
        TRACE("ASC PassWord : ");
        TRACE((void *)ascPin, ascPinLen);
    }

    return UI_INPUT_TIMEOUT;
}

short MccDemo(ST_TRANSDATA *pstTransData)
{
    unsigned char aucBuf[512];
    unsigned char ucLen = 0, ucTmpLen = 0;
    // unsigned char aucPanBuf[24];
    short errCode = 0;
    u8 aucTmpBuf[SDK_TRACK_NUM][MSC_TMP_BUF_LEN];
    ST_CARDINFO *pstCardInfo = NULL;
    short sRet = 0;
    FUN_IN;

    pstCardInfo = &pstTransData->stCardInfo;

    signed short sOffSet1 = 0, sOffSet2 = 0;

    if (!astCiTrack[SDK_TRACK2].ucTrackValid)
    {
        errCode = 0x8004;
        return errCode;
    }

    // DO_NEXT:
    TRACE("TRACK2 length = %d, TRACK3 length = %d\r\n", astCiTrack[SDK_TRACK2].ucTrackLen, astCiTrack[SDK_TRACK3].ucTrackLen);
    TRACE("TRACK2 = %s, TRACK3 = %s\r\n", astCiTrack[SDK_TRACK2].pucTrackBuf, astCiTrack[SDK_TRACK3].pucTrackBuf);

    pstCardInfo->eEntryMode = MODE_MSC;
    memset(aucBuf, 0, sizeof(aucBuf));
    // get PAN
    GetPANfromTrackData(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    // PanProcess(aucBuf, ucLen, aucPanBuf, &ucTmpLen, 0);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucCardNo, ucLen);
    pstCardInfo->aucCardNo[ucLen * 2] = '\0';
    TRACE("CardNo: %s", pstCardInfo->aucCardNo);

    // remove padding F
    while (pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] > '9')
    {
        pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] = '\0';
    }
    pstCardInfo->ucCardLen = strlen((char *)pstCardInfo->aucCardNo);
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
        memset(aucBuf, 0, sizeof(aucBuf));
        memcpy(aucBuf, astCiTrack[SDK_TRACK2].pucTrackBuf + 1, astCiTrack[SDK_TRACK2].ucTrackLen - 2);

        for (int i = 0; i < astCiTrack[SDK_TRACK2].ucTrackLen - 2; i++)
        {
            // '=' convert to D
            if (aucBuf[i] == 0x3D)
            {
                aucBuf[i] = 0x44;
            }
        }
    }
    memcpy(pstCardInfo->aucTrack2Data, aucBuf, astCiTrack[SDK_TRACK2].ucTrackLen - 2);
    pstCardInfo->ucTrack2Len = astCiTrack[SDK_TRACK2].ucTrackLen - 2;

    memset(aucBuf, 0, sizeof(aucBuf));
    // card valid date
    GetCardValidity(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucExpiredDate, ucLen);
    TRACE("Card ExpiredDate: %s", pstCardInfo->aucExpiredDate);

    memset(aucBuf, 0, sizeof(aucBuf));
    // Get service code
    GetCardSevCode(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucServiceCode, ucLen);
    TRACE("Card ServiceCode: %s", pstCardInfo->aucServiceCode);

    if (astCiTrack[SDK_TRACK1].ucTrackValid)
    {
        // 1th track
        //  Find the position of the %
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

    sRet = InputPIN(pstTransData);

    return sRet;
}
