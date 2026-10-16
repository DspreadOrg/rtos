/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : Card.c
 *  Version      :
 *  Author       :
 * Description   : Card Management
 **********************************************************************************************************/

#include "Card.h"
#include "Drv.h"
#include "Trade.h"
#include "Common.h"
#include "Idle.h"
#include "key.h"

#define CONTACT					0
#define CONTACTLESS				1
#define QPBOC_CONTACTLESS		2
#define MAGNETIC				3

// Card types: Magnetic Stripe 1, Contact Card 2, Contactless Card 3, Manual Input Card Number 5, No need for Card 6
//static unsigned char s_ucCardMode = 0x00;

ST_MSC_TRACK astCiTrack[TRACK_NUM];
// Save track data
unsigned char aucCiTrackBuf[TRACK_NUM][MSC_TRACK_BUFSIZE];
unsigned char g_ucCardType = 0xFF;


//-----------------------------------------------------------------------------
// Name    : CardInitForTrade
// Func    : Inquiry card initialization
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void CardInitForTrade(void)
{
	// Initialize card related operations
	TRACE("Lib_IccOpen START  IC");

	Lib_IccOpen(ICC_SLOT_EMV_IC);
	Lib_PiccOpen();

	astCiTrack[TRACK1].pucTrackBuf    = aucCiTrackBuf[TRACK1];
	astCiTrack[TRACK1].ucTrackValid   = 0;
	astCiTrack[TRACK1].ucTrackLen     = 0;

	astCiTrack[TRACK2].pucTrackBuf    = aucCiTrackBuf[TRACK2];
	astCiTrack[TRACK2].ucTrackValid   = 0;
	astCiTrack[TRACK2].ucTrackLen     = 0;

	astCiTrack[TRACK3].pucTrackBuf    = aucCiTrackBuf[TRACK3];
	astCiTrack[TRACK3].ucTrackValid   = 0;
	astCiTrack[TRACK3].ucTrackLen     = 0;

	memset(aucCiTrackBuf[TRACK1], 0, MSC_TRACK_BUFSIZE);
	memset(aucCiTrackBuf[TRACK2], 0, MSC_TRACK_BUFSIZE);
	memset(aucCiTrackBuf[TRACK3], 0, MSC_TRACK_BUFSIZE);
    Lib_MscOpen();
}


//-----------------------------------------------------------------------------
// Name    : CheckCardForTradeInDefaultUI
// Func    : Inquiry card in standby menu
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CheckCardForTradeInDefaultUI(void)
{
    unsigned char ucTemp = 0;
    int ret = 0;

    if (1 == Lib_IccIsPresent(ICC_SLOT_EMV_IC))
    {
        if (CARD_NONE < Lib_IccInquiry(ICC_SLOT_EMV_IC,  NULL, NULL, NULL, NULL))
        {
            TRACE("Lib_IccInquiry OK");
            BeepOn();
            ucTemp = SCI_DETECETED;
        }
    }

    if (CARD_NONE < Lib_PiccInquiry( NULL, NULL, NULL, NULL))
    {
        TRACE("Lib_PiccInquiry2 OK");
        BeepOn();
        ucTemp = NFC_DETECETED;
    }

    ret = Lib_MscRead(&astCiTrack[SDK_TRACK1], &astCiTrack[SDK_TRACK2], &astCiTrack[SDK_TRACK3]);

    TRACE("Lib_MscRead ret %d", ret);
    if(1 == ret)
    {
        TRACE("Lib_MscRead OK");
        BeepOn();
        ucTemp = MSC_DETECETED;
    }
#if 0
    if (ucTemp > 0)
    {
        Lib_IccClose(ICC_SLOT_EMV_IC);
        Lib_PiccClose();
        Lib_MscClose();
        return ucTemp ;
    }
#endif
    return ucTemp;
}


//-----------------------------------------------------------------------------
// Name    : OpenCiCard
// Func    : Card recognition
// Paras   : ucInputCardIdEnable : support manual input card numbers or not?
//           ucCardBit           : Bit00 supports magnetic stripe cards,
//                                 Bit01 supports non card access,
//                                 Bit02 supports contact cards
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short OpenCiCard(int iTimeout, unsigned char ucInputCardIdEnable, unsigned char ucCardBit)
{
    unsigned char ucKey;
    short sRC;
    unsigned long ulStartTime;
    int ret = 0;

    if (!ucCardBit)
    {
        return RET_ERR;
    }


    // Turn off card detection power: To avoid being cancelled during transaction processes,
    // the card detection module power is not turned off
    Lib_IccClose(ICC_SLOT_EMV_IC);
    Lib_PiccClose();

    CardInitForTrade();


	ulStartTime = Lib_SysGetTimer();
	while (1)
	{
		// Support magnetic stripe card
		if (ucCardBit & BIT0)
		{
			// Magnetic stripe card detection
            ret = Lib_MscRead(&astCiTrack[SDK_TRACK1], &astCiTrack[SDK_TRACK2], &astCiTrack[SDK_TRACK3]);

            TRACE("Lib_MscRead ret %d", ret);
            if (1 == ret)
            {
                TRACE("Lib_MscRead OK");
                BeepOn();
                sRC = MSC_DETECETED;
                break;
            }
		}


		// Support contact cards
		if (ucCardBit & BIT2)
		{
			// First, check the contact card
			if (1 == Lib_IccIsPresent(ICC_SLOT_EMV_IC))
            {
                if (CARD_NONE < Lib_IccInquiry(ICC_SLOT_EMV_IC,  NULL, NULL, NULL, NULL))
                {
                    TRACE("Lib_IccInquiry OK");
                    BeepOn();
                    sRC = SCI_DETECETED;
                    break;
                }
            }
		}


		// Supports contact-less cards
		if (ucCardBit & BIT1)
		{
			if (CARD_NONE < Lib_PiccInquiry( NULL, NULL, NULL, NULL))
            {
                TRACE("Lib_PiccInquiry OK");
                BeepOn();
                sRC = NFC_DETECETED;
                break;
            }
		}


		if (IS_TIME_OUT(ulStartTime, iTimeout))
		{
			//return UI_INPUT_TIMEOUT;
			sRC = UI_INPUT_TIMEOUT;
			break;
		}
		else
		{
			// Waiting for key selection
			ucKey = KB_NULL;
			ucKey = KbReadKey();
			if (KB_CANCEL == ucKey)
			{
				sRC = ABORT;
				FUN_IN;
				break;
			}
			else if (ucInputCardIdEnable && (KB_ENTER == ucKey))
			{
				sRC = INPUT_CARD_ID;
				FUN_IN;
				break;
			}
		}
	}

    Lib_MscClose();
    Lib_IccClose(ICC_SLOT_EMV_IC);
    Lib_PiccClose();

    //UICloseTimeoutCbCountDown();
    return sRC;
}


