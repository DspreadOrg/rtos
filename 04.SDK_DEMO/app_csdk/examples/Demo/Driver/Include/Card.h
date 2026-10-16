/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : Card.h
 *  Version      :
 *  Author       :
 * Description   : Card Management
 **********************************************************************************************************/


#ifndef __CARD_H__
#define __CARD_H__

#include "LibApi.h"

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------
#define MSC_TRACK_BUFSIZE   256     // Single track information length

#define TRACK_NUM           3       // Number of tracks, 2, 3 tracks
#define TRACK1              0       // Track 1 array index
#define TRACK2              1       // Track 2 array index
#define TRACK3              2       // Track 3 array index

#define FAIL                0       // fail
#define MSC_DETECETED       1       // MAGNETIC STRIPE CARD
#define SCI_DETECETED	    2       // Contact IC card
#define NFC_DETECETED	    3       // contactless ic card
#define ABORT               4       // cancel
#define INPUT_CARD_ID	    5       // Manually input card number (only for POS applications)
#define NO_CARD_ID		    6       // No card required (only for POS applications)

#define  CARD_NONE          0       // No card

typedef enum {
    ID_CARD_CONTACTLESS = 0,
    ID_CARD_CONTACT,
    ID_CARD_SAM1,
    ID_CARD_SAM2,
    ID_CARD_NONE,
} ID_CARD_TYPE;

extern ST_MSC_TRACK astCiTrack[TRACK_NUM];

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name    : CardInitForTrade
// Func    : Inquiry card initialization
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void CardInitForTrade(void);


//-----------------------------------------------------------------------------
// Name    : CheckCardForTradeInDefaultUI
// Func    : Inquiry card in standby menu
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short CheckCardForTradeInDefaultUI(void);


short OpenCiCard(int iTimeout, unsigned char ucInputCardIdEnable, unsigned char ucCardBit);

#endif

