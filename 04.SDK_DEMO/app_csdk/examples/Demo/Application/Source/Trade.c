/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Trade.c
 *  Version      :
 *  Author       :
 * Description   : Transaction related functions
 **********************************************************************************************************/

#include "MessageBox.h"
#include "Language.h"
#include "Trade.h"
#include "Common.h"
#include "Trade.h"
#include "Input.h"
#include "Drv.h"
#include "net_if.h"
#include "Communication.h"
#include "TradeRecord.h"
#include "Card.h"
#include "Idle.h"
#include "Menu.h"
#include "Key.h"
#include "EmvDemo.h"
#include "cJSON.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"

void* hDialog = 0;

ST_TRADE_REC g_trade_info;
ST_TRANSDATA gstTransData;

// Handle of transaction network communication
static int g_iTradeHandle = -1;


// Is it necessary to reversal immediately
void IsFastRevert(int iTimeout, unsigned long ulOpCode)
{

}


//-----------------------------------------------------------------------------
// Name    : CupRecordPrepare
// Func    : Preparation of transaction records
// Paras   : ulOpCode[in], Transaction code
// Ret     :
// Desc    : __Attributed_ ((unused)) indicates that if this function has not been called, remove the warning
//-----------------------------------------------------------------------------
__attribute__((unused)) static short CupRecordPrepare(unsigned long ulOpCode)
{
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : IsCupOpNeedPin
// Func    : Determine whether a password needs to be entered based on the business code and terminal parameter settings
// Paras   : ulOpCode[in], Transaction code
// Ret     : True - requires password, FALSE - not required
// Desc    : __Attributed_ ((unused)) indicates that if this function has not been called, remove the warning
//-----------------------------------------------------------------------------
__attribute__((unused)) static unsigned char IsCupOpNeedPin(unsigned long ulOpCode)
{
    return FALSE;
}


//-----------------------------------------------------------------------------
// Name    : IsCupOpNeedMasterPin
// Func    : Determine whether to enter the supervisor password based on the business code and terminal parameter settings
// Paras   : ulOpCode[in], Transaction code
// Ret     : True - requires password, FALSE - not required
// Desc    : __Attributed_ ((unused)) indicates that if this function has not been called, remove the warning
//-----------------------------------------------------------------------------
__attribute__((unused)) static unsigned char IsCupOpNeedMasterPin(unsigned long ulOpCode)
{
    return FALSE;
}


//-----------------------------------------------------------------------------
// Name    : CupTradeInit
// Func    : Transaction initialization
// Paras   : ulOpCode[in], Transaction code
// Ret     :
// Desc    : This step is the first step for all Transaction, mainly handling: displaying the title,
//           initializing the transaction TLV cache, preparing for transactions (querying whether
//           transaction records need to be saved), setting transaction types, and so on;
//-----------------------------------------------------------------------------
short CupTradeInit(int iTimeout, unsigned long ulOpCode)
{
    g_trade_info.ulOpCode = ulOpCode;
    // Set up servers separately according to business type
    //SetServerOpt(SERVER_OPT_POSP_MAIN);
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : SetCash
// Func    : Realize the conversion of left aligned amounts to right aligned amounts, with zero padding on the left side
// Paras   : ulOpCode[in], Transaction code
// Ret     :
// Desc    : Output 6 bytes of amount (bcd), with 0 added to the left side
//-----------------------------------------------------------------------------
void SetCash(unsigned char *pcIN, unsigned char pucLen, unsigned char *pcOut)
{
    unsigned char aucTemp[12];

    if (pucLen > 12)
    {
        return;
    }

    memcpy(aucTemp, pcIN, pucLen);

    //ON_TRACE("pucLen1 = %x\r\n",pucLen );
    TRACE("aucTemp[pucLen-1] = %x\r\n",aucTemp[pucLen-1]);

    // Convert the amount into BCD code
    if (pucLen < 12)
    {
        memmove(aucTemp+12 - pucLen, aucTemp, pucLen);
        memset(aucTemp, '0', 12 - pucLen);
    }
    pucLen=12;
    Lib_Asc2Bcd((char*)aucTemp, pcOut, 12);
}

char* substring ( char* ch, int pos, int length )
{
	char* p = ch;//定义??个字符指针，指向传???进来的ch地址??
	char* subch = ( char* ) calloc ( sizeof ( char ), length + 1 ); //通过calloc来分配一个length长度的字符数组，返回的是字符指针??
	int i;
	p = p + pos;//是p指针指向pos位置??
	for ( i = 0; i < length; i++ )
	{
		subch[i] = * ( p++ ); //循环遍历赋???数组???
	}
	subch[length] = '\0';//加上字符串结束符??
	return subch;       //返回分配的字符数组地????
}

// HttpResponse解析函数
HttpResponse* parseResponse ( char* response )
{
	FUN_IN;
	HttpResponse* _httpResponseTemp = ( HttpResponse* ) Lib_malloc ( sizeof ( HttpResponse ) );
	memset ( _httpResponseTemp, 0, sizeof ( HttpResponse ) );
	HttpResponse* _httpResponse = ( HttpResponse* ) Lib_malloc ( sizeof ( HttpResponse ) );
	memset ( _httpResponse, 0, sizeof ( HttpResponse ) );

	// 解析第一??
	char* _start = response;
	for ( ; *_start && *_start != '\r'; _start++ )
	{
		if ( _httpResponseTemp->version == NULL )
		{
			_httpResponseTemp->version = _start;
		}

		if ( *_start == ' ' )
		{
			if ( _httpResponseTemp->code == NULL )
			{
				_httpResponseTemp->code = _start + 1;
			}
			else
			{
				_httpResponseTemp->desc = _start + 1;
			}
			*_start = '\0';
		}
	}
	*_start = '\0'; // \r -> \0
	_start++;   // skip \n
	//LOGD("parseResponse(): VERSION= %s , CODE= %s , DESC= %s", _httpResponseTemp->version, _httpResponseTemp->code, _httpResponseTemp->desc);

	// 解析header
	_start++;
	char* _line = _start;
	while ( *_line != '\r' && *_line != '\0' )
	{
		char* _key;
		char* _value;
		while ( * ( _start++ ) != ':' );
		* ( _start - 1 ) = '\0';
		_key = _line;
		_value = _start + 1;
		while ( _start++, *_start != '\0' && *_start != '\r' );
		*_start = '\0'; // \r -> \0
		_start++;   // skip \n

		_start++;
		_line = _start;

		//LOGD("parseResponse(): KEY= %s , VALUE= %s", _key, _value);
		if ( !strcmp ( _key, "Content-Length" ) )
		{
			_httpResponseTemp->bodySize = atoi ( _value );
		}
	}

	// 解析body 如果最后一行不是空行，说明有body数据
	if ( *_line == '\r' )
	{
		_line += 2;
		_httpResponseTemp->body = _line;
		//LOGD("parseResponse(): BODY= %s", _httpResponseTemp->body);
	}

	if ( _httpResponseTemp->version != NULL )
	{
		_httpResponse->version = strdup ( _httpResponseTemp->version );
	}
	if ( _httpResponseTemp->code != NULL )
	{
		_httpResponse->code = strdup ( _httpResponseTemp->code );
	}
	if ( _httpResponseTemp->desc != NULL )
	{
		_httpResponse->desc = strdup ( _httpResponseTemp->desc );
	}
	if ( _httpResponseTemp->body != NULL )
	{

		if ( strlen ( _httpResponseTemp->body ) )
		{
			char* ptr;
			char* ptr1;
			ptr = strchr ( _httpResponseTemp->body, '{' );

			ptr1 = strrchr ( _httpResponseTemp->body, '}' );
			if ( ptr1!= NULL && ptr!= NULL )
			{
				_httpResponse->body = substring ( _httpResponseTemp->body,ptr-_httpResponseTemp->body,ptr1-ptr+1 );
				TRACE ( "body len: %d",strlen ( _httpResponse->body ) );
			}
		}
	}

	_httpResponse->bodySize = _httpResponseTemp->bodySize;

	Lib_free ( _httpResponseTemp );
	_httpResponseTemp = NULL;

	return _httpResponse;
}

short UnpackErrJson(u8* aucJsonstr, ST_TRANSDATA* pstTransData)
{
	const cJSON* response = NULL;
	const cJSON* code = NULL;
	const cJSON* msg = NULL;
	FUN_IN;


	cJSON* monitor_json = cJSON_Parse ( (const char *)aucJsonstr );

	if ( monitor_json == NULL )
	{
		const char* error_ptr = cJSON_GetErrorPtr();
		if ( error_ptr != NULL )
		{
			TRACE ( "Error before: %s\n", error_ptr );
			cJSON_Delete ( monitor_json );
			return RET_ERR;
		}
	}

	response = cJSON_GetObjectItemCaseSensitive ( monitor_json, "response" );
	if ( response != NULL )
	{
		code = cJSON_GetObjectItemCaseSensitive ( response, "code" );
		if ( cJSON_IsString ( code ) && ( code->valuestring != NULL ) )
		{
			strcpy ( (char *)pstTransData->stTransInfo.aucRespCode,code->valuestring );
			TRACE ( "aucRespCode = %s", pstTransData->stTransInfo.aucRespCode );
			cJSON_Delete ( monitor_json );
            return RET_ERR;
		}
	}
    msg = cJSON_GetObjectItemCaseSensitive ( monitor_json, "message" );
    if ( cJSON_IsString ( msg ) && ( msg->valuestring != NULL ) )
    {
        strcpy ( (char *)pstTransData->stTransInfo.aucMsg,msg->valuestring );
        TRACE ( "msg = %s", pstTransData->stTransInfo.aucMsg );

    }

	cJSON_Delete ( monitor_json );
    return RET_ERR;
}

void releaseHttpResponse ( HttpResponse* httpResponse )
{
	if ( httpResponse == NULL )
	{
		return;
	}

	if ( httpResponse->version != NULL )
	{
		Lib_free ( httpResponse->version );
		httpResponse->version = NULL;
	}

	if ( httpResponse->code != NULL )
	{
		Lib_free ( httpResponse->code );
		httpResponse->code = NULL;
	}

	if ( httpResponse->desc != NULL )
	{
		Lib_free ( httpResponse->desc );
		httpResponse->desc = NULL;
	}

	if ( httpResponse->body != NULL )
	{
		Lib_free ( httpResponse->body );
		httpResponse->body = NULL;
	}

	Lib_free ( httpResponse );
	httpResponse = NULL;
}

short RemoveLengthByteInJsonBody ( u8* aucJsonBody )
{
	u8 buf[64] = {0};
	char *pbuf= NULL;
	char *ptemp= NULL;
	int iOffset =0;
	unsigned long jsonBodyLen;

	FUN_IN;
	if (4096> strlen((char *)aucJsonBody))
	{
		FUN_OUT;
		return RET_OK;
	}

    jsonBodyLen = strlen((char *)aucJsonBody) + 32;
	pbuf = (char *)Lib_malloc(jsonBodyLen);
	ptemp = (char *)Lib_malloc(jsonBodyLen);
	strcpy(ptemp, (char *)aucJsonBody);

	while ( 1 )
	{
		char *ptr1 = strstr(ptemp, "\r\n");
		if ( ptr1 == NULL )
		{
			FUN_OUT;
			break;
		}
		else
		{
			TRACE ( "1st \r\n exist" );
			BUFCLR ( buf );
			memcpy ( buf,ptr1,10 );
			TRACE ( "before remove: %s", buf );
			ptr1+=2;
		}
		char *ptr2 = strstr(ptr1, "\r\n");
		if ( ptr2 == NULL )
		{
			FUN_OUT;
			break;
		}
		else
		{
			TRACE ( "2nd \r\n exist" );
			ptr2+=2;
		}
		memset(pbuf, 0x00, jsonBodyLen);
		iOffset = ptr1- ( char* ) ptemp-2;
		TRACE ( "1iOffset: %d", iOffset );
		memcpy ( pbuf,ptemp,iOffset );
		TRACE ( "substring" );
		pbuf[iOffset] = '\0';

		TRACE ( "strlen: %d", strlen ( ( char* ) ptemp ) );

		TRACE ( "2iOffset: %d", ( ptr2- ( char* ) ptemp ) );

		TRACE ( "3iOffset: %d", strlen ( ( char* ) ptemp )- ( ptr2- ( char* ) ptemp ) );

		memcpy ( pbuf+iOffset,ptr2,strlen ( ( char* ) ptemp )- ( ptr2- ( char* ) ptemp ) );
		BUFCLR ( buf );
		memcpy ( buf,pbuf+iOffset,10 );
		TRACE ( "after remove: %s", buf );
		iOffset+=strlen ( ( char* ) ptemp )- ( ptr2- ( char* ) ptemp );
		pbuf[iOffset] = '\0';

		memset(ptemp, 0x00, jsonBodyLen);
		TRACE ( "BUFCLR" );
		strcpy ( ptemp, pbuf );
		TRACE ( "Jsonbody len =%d",strlen ( ( char* ) ptemp ) );

//
//        TRACE ( "json body:" );
//        for ( int i = 0; i < strlen ( ( char* ) ptemp ) /TRACE_MAXLEN+1; i++ )
//        {
//            BUFCLR ( aucPrnbuf );
//            memcpy ( aucPrnbuf,ptemp + ( TRACE_MAXLEN*i ),TRACE_MAXLEN );
//            aucPrnbuf[TRACE_MAXLEN] = '\0';
//            TRACE ( "%s",aucPrnbuf );
//        }
	}

	//BUFCLR ( aucJsonBody );
	strcpy ((char *)aucJsonBody, ptemp);

	Lib_free ( pbuf );
	Lib_free ( ptemp );
	FUN_OUT;
	return RET_OK;

}

//-----------------------------------------------------------------------------
// Name    : CupOpTxJsonPack
// Func    : Send Json packets
// Paras   : CallBack[in], Callback function for assembling and sending 8583 packages
//           iTimeout[in], Timeout value,in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CupOpTxJsonPack(short (*CallBack)(unsigned char *pucOut, unsigned short *pucOutLen), unsigned long ulOpCode, int iTimeout)
{
    // 8583 packet data
    unsigned char* aucPackBuffer = NULL;
    // 8583 packet data length
    unsigned short usPackLen;
    short sRC;
    EN_SERVER_OPT stServerOpt = SERVER_OPT_RESERVE;

    FUN_IN;

	aucPackBuffer = (unsigned char *)Lib_malloc(MSG_MAXLEN);
    if (aucPackBuffer == NULL)
    {
        TRACE("Buf malloc fail.\n");
        return RET_ERR;
    }
    memset(aucPackBuffer, 0, MSG_MAXLEN);

    // Judging network connection status: Some packets require obtaining base station information, etc. Here, it is
    // necessary to ensure that the network status is OK
    //check network status
    if (Net_GetSocketMode() == NET_MODE_SSL)
    {
        sRC = UIWireNetTcpLinkExt(&g_iTradeHandle, NETWORK_TLS, NET_MAX_TCPLINK_TIME, stServerOpt, Net_OpenCb);
    }
    else
    {
        sRC = UIWireNetTcpLinkExt(&g_iTradeHandle, NETWORK_TCP, NET_MAX_TCPLINK_TIME, stServerOpt, Net_OpenCb);
    }
    if (UI_INPUT_CANCEL == sRC)
    {
        MessageBox(TIP_USER_CANCEL, MB_CENTER|MB_COLOR_YELLOW, 2000);
		free(aucPackBuffer);
		UIWireNetCloseExt(&g_iTradeHandle);
        return sRC;
    }
    else if (RET_OK != sRC)
    {
        MessageBox(TIP_LINK_FAIL, MB_CENTER|MB_COLOR_YELLOW, 2000);
		free(aucPackBuffer);
		UIWireNetCloseExt(&g_iTradeHandle);
        return RET_ERR;
    }

    // TCP connection successful, sending packets
    MessageBox(TIP_NET_SEND_DATA, MB_CENTER|MB_COLOR_YELLOW, 0);

    Lib_DelayMs(10);

    // Consumption 8583 message
    memset(aucPackBuffer, 0, MSG_MAXLEN);
    usPackLen = MSG_MAXLEN;

    sRC = CallBack(aucPackBuffer, &usPackLen);
    TRACE( "CupOpTxJsonPack() = %d\r\n", sRC);

    TRACE( "!!! tcp prepare11 buff size = %d\r\n", usPackLen);

	Trace_LongData((char *)aucPackBuffer);

    Lib_DelayMs(100);

    // send data
    sRC = UIWireNetTcpTxExt(g_iTradeHandle, aucPackBuffer, usPackLen, NET_MAX_TX_TIME, Net_WriteCb);
    TRACE("UIWireNetTcpTx ret:%d", sRC);
	free(aucPackBuffer);
    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : CupOpRxJsonPack
// Func    : Receive json packets and process it
// Paras   : iTimeout[in], Timeout value,in milliseconds
//           ulOpCode[in], transaction code
//           CallBack[in], Callback function for 8583 unpack processing
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CupOpRxJsonPack(int iTimeout, unsigned long ulOpCode, short (*CallBack)(unsigned char *pucIn, unsigned short usInLen))
{
    unsigned char* aucPackBuffer = NULL;
    unsigned short usPackLen;
    unsigned long ulPackLen;
    short sRC;

    FUN_IN;

	aucPackBuffer = (unsigned char *)Lib_malloc(MSG_MAXLEN);
    if (aucPackBuffer == NULL)
    {
        TRACE("Buf malloc fail.\n");
        return RET_ERR;
    }
    memset(aucPackBuffer, 0, MSG_MAXLEN);

    // Packet reception
    MessageBox(TIP_NET_RECV_DATA, MB_CENTER|MB_COLOR_YELLOW, 0);

    ulPackLen = MSG_MAXLEN;
    sRC = UIWireNetTcpRxExt(g_iTradeHandle,aucPackBuffer, &ulPackLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Net_ReadCb);
    //UIWireNetCloseExt(&g_iTradeHandle);
    if (RET_OK != sRC)
    {
        TRACE("CupOpRxJsonPack err");
		free(aucPackBuffer);
        return sRC;
    }

    usPackLen = (unsigned short)ulPackLen;

    // Response data processing
    sRC = CallBack(aucPackBuffer, usPackLen);
    TRACE("CupOpRxJsonPack() = %d\r\n", sRC);

    if (RET_OK != sRC)
    {
        // Waiting for user confirmation of error message
        UIWaitKey(6000, 1);

        // Check if there are any contact cards that have not been removed
        DisplayDisEnbleIC();
    }
	free(aucPackBuffer);
    return sRC;
}

//-----------------------------------------------------------------------------
// Name    : CupOpResultDeal
// Func    : Process successful business results based on business code
// Paras   : iTimeout[in], Timeout value,in milliseconds
//           ulOpCode[in], transaction code
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void CupOpJsonResultDeal(unsigned long ulOpCode, int iTimeout)
{
    return;
}

void CupNetCloseEx(void)
{
	UIWireNetCloseExt(&g_iTradeHandle);
}

//-----------------------------------------------------------------------------
// Name    : CupOpTx8583Pack
// Func    : Send 8583 packets
// Paras   : CallBack[in], Callback function for assembling and sending 8583 packages
//           iTimeout[in], Timeout value,in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CupOpTx8583Pack(short (*CallBack)(unsigned char *pucOut, unsigned short *pucOutLen), unsigned long ulOpCode, int iTimeout)
{
    // 8583 packet data
    unsigned char aucPackBuffer[1024];
    // 8583 packet data length
    unsigned short usPackLen;
    short sRC;
    LVGL_RESULT result;

    result.img = FAILURE_IMG;
    result.title = TIP_FAILURE;
    result.fontColor = LV_COLOR_RED;

    // Judging network connection status: Some packets require obtaining base station information, etc. Here, it is
    // necessary to ensure that the network status is OK
    // The server number can be converted from ulOpCode, eg:SERVER_OPT_POSP_MAIN
    sRC = UIWireNetTcpLinkExt(&g_iTradeHandle, NETWORK_TCP, NET_MAX_TCPLINK_TIME, SERVER_OPT_POSP_MAIN, Net_ConnectCb);
    if (UI_INPUT_CANCEL == sRC)
    {
        result.msg = TIP_USER_CANCEL;
        result.timeout = 10000;
        LvUiShowResult(&result);

        return sRC;
    }
    else if (RET_OK != sRC)
    {
        result.msg = TIP_LINK_FAIL;
        result.timeout = 10000;
        LvUiShowResult(&result);

        return RET_ERR;
    }

    // TCP connection successful, sending packets
    // Consumption 8583 message
    memset(aucPackBuffer, 0, sizeof(aucPackBuffer));
    usPackLen = sizeof(aucPackBuffer);

    sRC = CallBack(aucPackBuffer, &usPackLen);
    TRACE( "CupOpTx8583Pack() = %d\r\n", sRC);

    TRACE( "!!! tcp prepare11 buff size = %d\r\n", usPackLen);

    Lib_DelayMs(100);

    // send data
    sRC = UIWireNetTcpTxExt(g_iTradeHandle, aucPackBuffer, usPackLen, NET_MAX_TX_TIME, Net_WriteCb);
    TRACE("UIWireNetTcpTx ret:%d", sRC);
    return sRC;
}


//-----------------------------------------------------------------------------
// Name    : CupOpRx8583Pack
// Func    : Receive 8583 packets and process it
// Paras   : iTimeout[in], Timeout value,in milliseconds
//           ulOpCode[in], transaction code
//           CallBack[in], Callback function for 8583 unpack processing
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CupOpRx8583Pack(int iTimeout, unsigned long ulOpCode, short (*CallBack)(unsigned char *pucIn, unsigned short usInLen))
{
    unsigned char aucPackBuffer[1200];
    unsigned short usPackLen;
    unsigned long ulPackLen;
    short sRC;

    FUN_IN;

    // Packet reception
    ulPackLen = sizeof(aucPackBuffer);
    sRC = UIWireNetTcpRxExt(g_iTradeHandle,aucPackBuffer, &ulPackLen, NET_MAX_RX_TIME, PACKET_TYPE_ISO8583, Net_ReadCb);

    UIWireNetCloseExt(&g_iTradeHandle);

    if (RET_OK != sRC)
    {
        TRACE("CupOpRx8583Pack err");
        LvUiCloseDialog(hDialog);
        hDialog = NULL;

        return sRC;
    }

    usPackLen = (unsigned short)ulPackLen;
    // Response data processing
    sRC = CallBack(aucPackBuffer, usPackLen);
    TRACE("CupOpRx8583Pack() = %d\r\n", sRC);


    if (RET_OK != sRC)
    {
        // Waiting for user confirmation of error message
        LvUiWaitKey(6000);
        // Check if there are any contact cards that have not been removed
        DisplayDisEnbleIC();
    }
    LvUiCloseDialog(hDialog);
    hDialog = NULL;

    return sRC;
}



void DisplayDisEnbleIC(void)
{
    // Prompt user to unplug card
    if ((1 == Lib_IccIsPresent(ICC_SLOT_EMV_IC)))
    {
        //IC: Unplug card
        LvUiMessageBox(TIP_PULL_OUT_CARD, MB_COLOR_YELLOW, "TIP", 0);
        while (Lib_IccIsPresent(ICC_SLOT_EMV_IC))
        {
            LvUiWaitKey(1000);
        }

        // IC card contact card
        Lib_IccClose(ICC_SLOT_EMV_IC);
    }
    TRACE("DisplayDisEnbleIC\r\n");
}


//-----------------------------------------------------------------------------
// Name    : CupOpResultDeal
// Func    : Process successful business results based on business code
// Paras   : iTimeout[in], Timeout value,in milliseconds
//           ulOpCode[in], transaction code
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void CupOpResultDeal(unsigned long ulOpCode, int iTimeout)
{
    return;
}

//-----------------------------------------------------------------------------
// Name    : PosReversal
// Func    : Transaction Reversal
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short PosReversal(void)
{
    return RET_OK;
}

short SearchCard(char *title)
{
    short sRC;
    int key = 999;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    LVGL_DIALOG dialog = { 0 };
    short cardMode = -1;

    FUN_IN;

    //close ICC and PICC module first
    Lib_IccClose(ICC_SLOT_EMV_IC);
    Lib_PiccClose();

    CardInitForTrade();
    LvUiCloseDialog(hDialog);
    hDialog = NULL;
    LvUilCls();

    lv_obj_t *img = lv_gif_create(lv_scr_act());

    stCtrlObjAttr.m_stImgAttr.m_pcGifSrc = ICC_GIF;
    stCtrlObjAttr.m_align = LV_ALIGN_CENTER;
    LvSetCtrlObj(img, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

	while (1)
	{
		//display RTC
		key = LvUiWaitKey(40);

		if (LVGL_OPER_TIMEOUT != key)
		{
			if (KB_CANCEL == key)
			{
				//取消退出
				sRC = RET_ERR;
                lv_obj_del(img);
				goto QUIT;
			}
			else
			{
				continue;
			}
		}

		//checking if card has been detected
		sRC = CheckCardForTradeInDefaultUI();
		TRACE ( "sRC=%d\n", sRC );
		if (sRC < 0)
		{
			continue;
		}
		else if((MSC_DETECETED != sRC) && (SCI_DETECETED != sRC) && (NFC_DETECETED != sRC))
		{
			continue;
		}
		cardMode = sRC;
        // Close the unused card channels.
        if (MSC_DETECETED != cardMode)
        {
            Lib_MscClose();
        }
        if (SCI_DETECETED != cardMode)
        {
            Lib_IccClose(ICC_SLOT_EMV_IC);
        }
        if (NFC_DETECETED != cardMode)
        {
            Lib_PiccClose();
        }

        BeepOn();
		//LvUiCloseCardDebugIconTask ( anim );
		lv_obj_del(img);
        LvUilCls();

        dialog.font = FONT_SIZE_TIMER_MSG;
        dialog.title = title;
        dialog.msg = TIP_PROCESSING;
        dialog.gif = LOAD_DATA_GIF;
        dialog.timeout = 90;
        dialog.isCountDown = FALSE;
        LvUiDialog(&dialog, &hDialog);

        //LvUiWaitKey(4000);
        if(CardDemo(cardMode, &gstTransData) == 0)
        {
            TRACE("Emv Test OK.\n");
            //MessageBox("test ok", MB_CENTER|MB_COLOR_GREEN, 0);
        }
        else
        {
            TRACE("Emv Test Failed.\n");
            //MessageBox("test failed", MB_CENTER|MB_COLOR_RED, 0);
        }

        sRC = RET_OK;
		break;
	}
QUIT:
    //close modules
    LvUiCloseDialog(hDialog);
    hDialog = NULL;
    UIClearRect(0, 0, LCD_MAX_SEG,  MAX_COM_LINES - 1);
    // Close the card channel.
    if (MSC_DETECETED == cardMode)
    {
        Lib_MscClose();
    }
    if (SCI_DETECETED == cardMode)
    {
        // Do not close the IC card channel, as it is required for the subsequent EMV data exchange.
    }
    if (NFC_DETECETED == cardMode)
    {
        Lib_PiccClose();
    }
    FUN_OUT;

    return sRC ;
}

