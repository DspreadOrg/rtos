/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : TradeRecord.c
 *  Version      :
 *  Author       :
 * Description   : Transaction record related functions
 **********************************************************************************************************/

#include "TradeRecord.h"
#include "Common.h"
#include "Trade.h"
#include "MessageBox.h"
#include "Input.h"
#include "Language.h"


#define RECORD_FILE_NAME "Record/record.bin"


static short g_trade_count = 0;
static int fd_record = -1;


/*****************************************************************************
** Descriptions: Init trans recordfile
** Parameters:
** Returned value:
** Created By:
** Remarks:
*****************************************************************************/
short ApiInitRecord(void)
{
	int len = 0;

	FUN_IN;

	if(Lib_FileMkdir(RECORD_FILE_NAME, 0) < 0)
    {
        TRACE("ApiInitRecord Lib_FileMkdir error");
        return RET_ERR;
    }
	// open file
	fd_record = Lib_FileOpen(RECORD_FILE_NAME, LFS_O_RDWR);
	TRACE("open fd_record : %d", fd_record);
	if(fd_record <= 0)
	{
		TRACE("open fail, create file");
		fd_record = Lib_FileOpen(RECORD_FILE_NAME, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);
		if(fd_record <= 0)
		{
			TRACE("Lib_FileOpen create  fail,%d", fd_record);
			return RET_ERR;
		}
		g_trade_count = 0;
		return RET_OK;
	}

    len = Lib_FileSeek(fd_record, 0, LFS_SEEK_END);
	if(len <= 0)
	{
		TRACE("ApiInitRecord get size fail,%d", len);
		return RET_ERR;
	}

    if(len % sizeof(ST_TRADE_REC) == 0)
    {
	    g_trade_count = len / sizeof(ST_TRADE_REC);
    }
	TRACE("g_trade_count: %d", g_trade_count);

	return RET_OK;
}

/*****************************************************************************
** Descriptions: close trans recordfile
** Parameters:
** Returned value:
** Created By:
** Remarks:
*****************************************************************************/
short ApiCloseRecord(void)
{
	int ret = 0;

    if (fd_record < 0)
    {
        return RET_ERR;
    }

    FUN_IN;

    if(fd_record < 0)
    {
        TRACE("Lib_FileOpen  fail,%d", fd_record);
        return RET_ERR;
    }

	ret = Lib_FileClose(fd_record);
	if(ret < 0)
	{
		TRACE("Lib_FileClose  fail,%d", ret);
		return RET_ERR;
	}
    fd_record = -1;
	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : ApiAddRec
// Func    : Add one transaction record
// Paras   : pstTradeRec[in], Transaction record structure pointer
// Ret     : RET_OK, Write successful; RET_ERR, Write failed
// Desc    :
//-----------------------------------------------------------------------------
short ApiAddRec(ST_TRADE_REC *pstTradeRec)
{
    unsigned char aucDispBuffer[64] = {0};
	int ret;

    if (fd_record < 0)
    {
        return RET_ERR;
    }

    if (NULL == pstTradeRec)
    {
        return RET_ERR;
   }
    //Exceeding storage limit, lear records
    if(g_trade_count > RECORD_NUM_MAX-1)
    {
        sprintf((char *)aucDispBuffer, "%s\n %s",TIP_RECORDS_FULL, TIP_ANY_TO_CONTINUE);
        MessageBox((char *)aucDispBuffer, MB_ANYKEY|MB_CENTER|MB_COLOR_YELLOW, 1000 * 5);
        g_trade_count = 0;
        ret = Lib_FileClose(fd_record);
        if(ret < 0)
        {
            TRACE("ApiAddRec close error %d", ret);
            return RET_ERR;
        }
        ret = Lib_FileRemove(RECORD_FILE_NAME);
        if(ret < 0)
        {
            TRACE("ApiAddRec delete error %d", ret);
            return RET_ERR;
        }
        fd_record = Lib_FileOpen(RECORD_FILE_NAME, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);
		if(fd_record <= 0)
		{
			TRACE("Lib_FileOpen create fail,%d", fd_record);
			return RET_ERR;
		}
    }

    ret = Lib_FileSeek(fd_record, g_trade_count * sizeof(ST_TRADE_REC), LFS_SEEK_SET);
    if(ret < 0)
    {
        TRACE("ApiAddRec seek1 error %d", ret);
        return RET_ERR;
    }
    ret = Lib_FileWrite(fd_record, (unsigned char *)pstTradeRec, sizeof(ST_TRADE_REC));
	if(ret < 0)
	{
		TRACE("ApiAddRec write fail,%d", ret);
		return RET_ERR;
	}
    ret = Lib_FileClose(fd_record);
    if(ret < 0)
    {
        TRACE("ApiAddRec save fail,%d", ret);
		return RET_ERR;
    }
    g_trade_count++;

    fd_record = Lib_FileOpen(RECORD_FILE_NAME, LFS_O_RDWR);
    if(fd_record < 0)
    {
        TRACE("ApiAddRec open fail,%d", fd_record);
		return RET_ERR;
    }
	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : ApiGetRecNum
// Func    : Count the number of transaction records based on transaction status
// Paras   :
// Ret     : Number of transaction records
// Desc    :
//-----------------------------------------------------------------------------
short ApiGetRecNum(void)
{
	return g_trade_count;
}


//-----------------------------------------------------------------------------
// Name    : ApiGetRecByIndex
// Func    : Query transaction records based on index
// Paras   : PstTradeRecord[in], The pointer to the transaction output structure
//           index[in], Index number
// Ret     : RET_OK, successful
// Desc    :
//-----------------------------------------------------------------------------
int ApiGetRecByIndex(ST_TRADE_REC *pstTradeRec, int index)
{
    int ret;
	if (index >= g_trade_count)
		return RET_ERR;

	FUN_IN;
    ret = Lib_FileSeek(fd_record, index * sizeof(ST_TRADE_REC), LFS_SEEK_SET);
    if(ret < 0)
    {
        TRACE("ApiGetRecByIndex seek2 error %d", ret);
        return RET_ERR;
    }
	ret = Lib_FileRead(fd_record, (unsigned char *)pstTradeRec, sizeof(ST_TRADE_REC));
    if(ret != sizeof(ST_TRADE_REC))
    {
        TRACE("ApiGetRecByIndex read error %d, %d", ret, sizeof(ST_TRADE_REC));
        return RET_ERR;
    }

	TRACE("Op Code:0x%x", pstTradeRec->ulOpCode);
	TRACE("aucDateH:0x%s:", pstTradeRec->aucDateH);
	TRACE("aucSum:");
	TRACEBUF(pstTradeRec->aucSum,6);
	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : ApiRecordTest
// Func    : Transaction record testing
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void ApiRecordTest()
{
    // Initialize transaction records
    ApiInitRecord();

    // Add one transaction record
    ST_TRADE_REC trade1;
    trade1.ulOpCode = TT_CONSUMPTION;
    memcpy(trade1.aucDateH, "\x12\x03", 2);
    memcpy(trade1.aucSum, "\x00\x11\x22\x33\x44\x55", 6);
    ApiAddRec(&trade1);

    ST_TRADE_REC trade2;
    trade1.ulOpCode = TT_CONSUMPTION;
    memcpy(trade2.aucDateH, "\x12\x01", 2);
    memcpy(trade2.aucSum, "\x01\x23\x45\x67\x89\x00", 6);
    ApiAddRec(&trade2);

    // Save transaction records
    ApiCloseRecord();

    // Reinitialize transaction records
    ApiInitRecord();

    // Refresh transaction records
    int tradeCount = 0;
    tradeCount = ApiGetRecNum();
    TRACE("tradeCount %d", tradeCount);
    for (int i=0; i<tradeCount; i++)
    {
    ST_TRADE_REC tempTrade;
    ApiGetRecByIndex(&tempTrade, i);
    TRACE("Op Code:0x%x", tempTrade.ulOpCode);
    TRACE("aucDateH:0x%s:", tempTrade.aucDateH);
    TRACE("aucSum:");
    TRACEBUF(tempTrade.aucSum,6);
    }

    ApiCloseRecord();
    return;
}


