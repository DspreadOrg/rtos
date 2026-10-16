//-----------------------------------------------------------------------------
// @file    : LibCards.h
// @brief   : Card-related functions, such as MSC,ICC,NFC
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_CARDS_H__
#define __LIB_CARDS_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef enum {
    ICC_SLOT_EMV_IC = 0x00,
    ICC_SLOT_SAM1,
    ICC_SLOT_SAM2,
    ICC_SLOT_NON_EMV_IC = 0x80,
} ICC_SLOT;

typedef enum {
    SDK_TRACK1,                      // Magnetic card, the first track
    SDK_TRACK2,                      // Magnetic card, the second track
    SDK_TRACK3,                      // Magnetic card, the third track
    SDK_TRACK_NUM,                   
} SDK_ST_TRACK_NUM;

typedef struct MSC_TRACK
{
    unsigned char *pucTrackBuf;             // Track information buffer
    unsigned char *pucTrackTmpBuf;          // Temporary buffer for magnetic track soft decoding
    unsigned char ucTrackLen;               // Track information length
    unsigned char ucTrackValid;             // Effective identification of magnetic track information
}ST_MSC_TRACK, *PST_MSC_TRACK;

//-----------------------------------------------------------------------------
// Name    : Lib_MscOpen
// Func    : Open Magnetic Card Reader
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_MscOpen(void);

//-----------------------------------------------------------------------------
// Name    : Lib_MscClose
// Func    : Close Magnetic Card Reader
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_MscClose(void);

//-----------------------------------------------------------------------------
// Name    : Lib_MscRead
// Func    : Read data from Magnetic Card
// Paras   : pstMscTrack1[in/out],    input data buffer and output the first track data
//           pstMscTrack2[in/out],    input data buffer and output the second track data
//           pstMscTrack3[in/out]:    input data buffer and output the third track data
// Ret     : more than 0 - Card Reading successful
//           equal 0     - Unread card
//           less than 0 - Card Reading failed
// Desc    :
//-----------------------------------------------------------------------------
int Lib_MscRead(PST_MSC_TRACK pstMscTrack1, PST_MSC_TRACK pstMscTrack2, PST_MSC_TRACK pstMscTrack3);

//-----------------------------------------------------------------------------
// Name    : Lib_IccOpen
// Func    : Open Contact Card Reader
// Paras   : iccSlot[in],  the card slot number
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_IccOpen(ICC_SLOT iccSlot);

//-----------------------------------------------------------------------------
// Name    : Lib_IccClose
// Func    : Open Contact Card Reader
// Paras   : iccSlot[in],  the card slot number
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_IccClose(ICC_SLOT iccSlot);

//-----------------------------------------------------------------------------
// Name    : Lib_IccInquiry
// Func    : Query contact card information
// Paras   : iccSlot[in],    the contact card slot number
//           pucAtr[in/out],  input buffer and output the ATR of contact card 
//           pucAtrLen[in/out], input ATR length pointer and output the length of ATR
//           pucUid[in/out],    input UID pointer and output the UID of contact card
//           pucUidLen[in/out], input UID length pointer and output the length of UID
// Ret     : more than 0 - Card Reading successful
//           equal 0     - Unread card
//           less than 0 - Card Reading failed
// Desc    :
//-----------------------------------------------------------------------------
int Lib_IccInquiry(ICC_SLOT iccSlot, unsigned char *pucAtr, unsigned char *pucAtrLen, unsigned char *pucUid, unsigned char *pucUidLen);

//-----------------------------------------------------------------------------
// Name    : Lib_IccCommand
// Func    : communicate with contact card
// Paras   : iccSlot[in],        input the contact card slot number
//           pucSend[in],        input the data to be sent
//           uSendLen[in]:       input the length of data sent
//           pucRecv[in/out]:    input data buffer and output the received data
//           puRecvLen[in/out]:  output the length of data received
// Ret     : more than 0 - success
//           others     -  fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_IccCommand(ICC_SLOT iccSlot, unsigned char *pucSend, unsigned short uSendLen, unsigned char *pucRecv, unsigned short *puRecvLen);

//-----------------------------------------------------------------------------
// Name    : Lib_IccIsPresent
// Func    : Check if the card is in the contact card slot
// Paras   : iccSlot[in],  the card slot number
// Ret     : more than 0 - success
//           others     -  fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_IccIsPresent(ICC_SLOT ucSlot);

//-----------------------------------------------------------------------------
// Name    : Lib_PiccOpen
// Func    : Open Contactless Card Reader
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PiccOpen(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PiccClose
// Func    : Close Contactless Card Reader
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PiccClose(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PiccInquiry
// Func    : Query contactless card information
// Paras   : pucAtr[in/out],  input buffer and output the ATR of contactless card 
//           pucAtrLen[in/out], input ATR length pointer and output the length of ATR
//           pucUid[in/out],    input UID pointer and output the UID of contactless card
//           pucUidLen[in/out], input UID length pointer and output the length of UID
// Ret     : more than 0 - Card Reading successful
//           equal 0     - Unread card
//           less than 0 - Card Reading failed
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PiccInquiry(unsigned char *pucAtr, unsigned char *pucAtrLen, unsigned char *pucUid, unsigned char *pucUidLen);

//-----------------------------------------------------------------------------
// Name    : Lib_PiccCommand
// Func    : communicate with the contactless card
// Paras   : pucSend[in],   input the data to be sent
//           uSendLen[in]:  the length of data sent
//           pucRecv[in/out]:   input data buffer and output the received data
//           puRecvLen[in/out]: the length of data received
// Ret     : more than 0 - success
//           others     -  fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PiccCommand(unsigned char *pucSend, unsigned short uSendLen, unsigned char *pucRecv, unsigned short *puRecvLen);

//-----------------------------------------------------------------------------
// Name    : Lib_PiccRemove
// Func    : Check if the contactless card has been removed.
// Paras   : 
// Ret     : 0 - Yes, others - No
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PiccRemove(void);

#endif