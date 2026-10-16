#ifndef __EMVDEMO_H__
#define __EMVDEMO_H__

#include "Trade.h"

#define TRANS_CHN_TYPE_SCI       0x00
#define TRANS_CHN_TYPE_NFC       0x01
#define TRANS_CHN_TYPE_MSC       0x02

#ifndef u8
#define u8   unsigned char
#endif

#ifndef u16
#define u16   unsigned short
#endif

typedef struct _PARAMETER_CONFIG_LIST
{
  unsigned char ucTag;
  unsigned char ucLen;
  unsigned char aucName[4];     // Version information

  unsigned char ucTrackIns;     // Swipe card INS
  unsigned char ucPinIns;       // Input password INS
  unsigned char ucMacIns;       // MAC calculation INS
  unsigned char ucMasterKeyIns; // Master key update INS

  unsigned char ucRandomTag1;      // Fixed character Tag1
  unsigned char ucRandomTag2;      // Incoming characters Tag2
  unsigned char ucRandomTag3;      // Internally generated Tag3
  unsigned char aucRandomMode[16]; // Random format (consisting of fixed character Tag1, incoming character Tag2, internally generated Tag3, and other TAGs)

  unsigned char ucInputDegrdTrade; // Downgraded trading (non 0- incoming with downgraded trading parameters, 0- incoming without downgraded trading parameters)
  unsigned char ucDegrdTrade;      // Downgrade trading (non 0-allowed downgrade trading, 0-not allowed downgrade trading)
  unsigned char ucPinTimeOut;      // Password input timeout time (non 0- input has timeout time, 0- input has no timeout time)
  unsigned char ucBatchAndTrace;   // Batch number serial number (non 0- returned, 0- not returned)

  unsigned char ucTrackPro;     // Magnetic track information processing method
  unsigned char ucETrackPro;    // 2th track equivalent data processing method
  unsigned char ucICCardPro;    // IC Card 55 field Processing Method
  unsigned char ucMacPro;       // MAC processing method
  unsigned char ucMasterKeyPro; // Master key processing method
  unsigned char ucWorkKeyPro;   // Work keys processing method

  unsigned char ucTrackEncrypKey; // Swipe card encryption key
  unsigned char ucPinEncrypKey;   // PIN encryption key
  unsigned char ucMacEncrypKey;   // MAC encryption key

  unsigned char aucTagList[100];    // 55 field TAG
  unsigned char aucResver[10];      // reserved
  unsigned char ucBatchAndTraceTag; // Batch number and serial number Tag
  unsigned char ucPanPro;           // Main account processing method
  unsigned char ucNfcPro;           // Contact Card Process (0-Main Account, Non 0-Electronic Cash)
  unsigned char aucCrc16[2];        // CRC verification value
} ST_PARAMETER_CONFIG_LIST, *PST_PARAMETER_CONFIG_LIST;


typedef struct _PAPAMETER_TRANSACTION_PRESET
{
//	unsigned char DowngradeDeal; //0-Close downgrade deal 1-Open downgrade deal
	unsigned char RandomInput[16]; //cmd input data to generate random data
}ST_PARAMETER_TRANSACTION_PRESET, *PST_PARAMETER_TRANSACTION_PRESET;


extern ST_PARAMETER_CONFIG_LIST stParConlist;
extern ST_PARAMETER_TRANSACTION_PRESET stTrasactionSet;
// random number
extern u8 g_aucRandom[16];
extern unsigned char g_aucTradeMsgBuf[1024];
extern unsigned short g_usTradeMsgLen;

//-----------------------------------------------------------------------------
// name      : EmvDemo
// founction : emv transaction demo
// parameter : cardMode     : 0-Contact card, 1-Contacless card, 2-Magnetic stripe cards
//             pstTransData : transaction data
// return    : 0-success 1-fail
// description:
//-----------------------------------------------------------------------------
short EmvDemo(short cardMode, ST_TRANSDATA* pstTransData);

//-----------------------------------------------------------------------------
// name      : CardDemo
// founction : card transaction demo
// parameter : cardMode 0-Contact card, 1-Contacless card, 2-Magnetic stripe cards
// return    : 0-success 1-fail
// description:
//-----------------------------------------------------------------------------
short CardDemo(short cardMode, ST_TRANSDATA* pstTransData);

//-----------------------------------------------------------------------------
// name      : CardDemoInit
// founction : init transaction key and parameters
// parameter : null
// return    : 0-success 1-fail
// description:
//-----------------------------------------------------------------------------
extern unsigned char CardDemoInit(void);

//-----------------------------------------------------------------------------
// name      : ReadCardPreProcess
// founction : Set parameters for each transaction(demo)
// parameter : pBuf	Pointer to the structure of the parameters of the transaction
// return    : 0-success 1-fail
// description:	This function can be executed before each transaction to set transaction parameters, you can implement
//				this function yourself or modify the parameters of _PAPAMETER_TRANSACTION_PRESET structure
//-----------------------------------------------------------------------------
extern unsigned char ReadCardPreProcess(PST_PARAMETER_TRANSACTION_PRESET      pBuf);

//-----------------------------------------------------------------------------
// name      : PosParameterInit
// founction : Set device parameters(demo)
// parameter : pBuf	Pointer to the structure of the parameters of pos device
// return    : 0-success 1-fail
// description:	This function must be executed every time the device is started, this stParConlist parameter can be
// 				stored in the device to indicate the capabilities and processing mechanism of the device
//-----------------------------------------------------------------------------
extern unsigned char PosParameterInit(unsigned char *pBuf);

//-----------------------------------------------------------------------------
// name      : GetCardData
// founction : Get the transaction data(PAN,PIN,random and so on)
// parameter : pBuf	Pointer to the buf to receive transaction data
// return    : 0-success 1-fail
// description:	This function must be executed every time the device is started, this stParConlist parameter can be
// 				stored in the device to indicate the capabilities and processing mechanism of the device
//-----------------------------------------------------------------------------
extern unsigned char GetCardData(unsigned char *pBuf, unsigned char *pLen);


short MccDemo(ST_TRANSDATA* pstTransData);



// User Interface Request Data DF8116 Value
#define UIRD_MSGID_CARD_READ_OK         0x17 // Card reading OK
#define UIRD_MSGID_TRY_AGAIN            0x21 // Retry
#define UIRD_MSGID_APPROVED             0x03 // Transaction Approval
#define UIRD_MSGID_REMOVE_CARD          0x10 // Remove Card
#define UIRD_MSGID_WELCOME              0x14 // Idle welcome Tips
#define UIRD_MSGID_PRESENT_CARD         0x15 // Please wave your card
#define UIRD_MSGID_PROCESSING           0x16 // Processing
#define UIRD_MSGID_PRESENT_ONE_CARD     0x19 // Card Conflict
#define UIRD_MSGID_APPROVED_SIGN        0x1A // Transaction Approval - Signature
#define UIRD_MSGID_DECLINED             0x07 // Transaction Declined
#define UIRD_MSGID_ERROR_OTHER_CARD     0x1C // Error - Other Cards
#define UIRD_MSGID_INSERT_CARD          0x1D // Insert Card
#define UIRD_MSGID_SEE_PHONE            0x20 // Check your phone
#define UIRD_MSGID_AUTH_PLEASE_WAIT     0x1B // Authorising - Please Wait
#define UIRD_MSGID_CLEAR_DISPLAY        0x1E // Clear screen
#define UIRD_MSGID_NA                   0xFF // N/A

#define UIRD_MSGID_PIN_OVER_LIMIT       0x80 // PIN attempt exceeded limit
#define UIRD_MSGID_PIN_LAST_TRY         0x81 // PIN Last Attempt
#define UIRD_MSGID_PIN_VERIFY_OK        0x82 // PIN auth successful
#define UIRD_MSGID_PIN_VERIFY_ERR       0x83 // PIN auth failed


// Outcome Parameter Set  DF8129 Value
#define OPS_STATUS_APPROVED             0x10 // Approved
#define OPS_STATUS_DECLINED             0x20 // Declined
#define OPS_STATUS_ONLINE_REQUEST       0x30 // online request
#define OPS_STATUS_END_APPLICATION      0x40 // End application
#define OPS_STATUS_SELECT_NEXT          0x50 // Select Next
#define OPS_STATUS_TRY_ANOTHER_INTERFACE   0x60 // Try other interfaces
#define OPS_STATUS_TRY_AGAIN            0x70 // Retry
#define OPS_STATUS_NA                   0xF0 // N/A

#define UIRD_STATUS_NOT_READY           0x00 // not ready
#define UIRD_STATUS_IDLE                0x01 // Idle
#define UIRD_STATUS_READY_TO_READ       0x02 // Preparing to read cards
#define UIRD_STATUS_PROCESSING          0x03 // Processing
#define UIRD_STATUS_CARD_READ_SUCCESS   0x04 // Card reading successful
#define UIRD_STATUS_PROCESSING_ERROR    0x05 // Processing error
#define UIRD_STATUS_NA                  0xFF // N/A

// Kernel type
#define KERNEL_TYPE_NULL                0 // No matching kernel
#define KERNEL_TYPE_EMV                 1 // EMV kernel
#define KERNEL_TYPE_QUICS               2 // QUICS kernel
#define KERNEL_TYPE_VISA                3 // VISA kernel
#define KERNEL_TYPE_MASTER              4 // MASTER kernel
#define KERNEL_TYPE_PURE                5 // PURE  kernel
#define KERNEL_TYPE_AXP                 6 // Expresspay kernel
#define KERNEL_TYPE_DISCOVER            7 // DISCOVER kernel
#define KERNEL_TYPE_JCB                 8 // JCB kernel




#define UIRD_MSGID_PLEASE_LOOK                  0x22 // Retry
#define UIRD_MSGID_PLEASE_AUTH                  0x23 // Retry

#define UIRD_MSGID_CARD_ERROR                   0x06 // Card data error(JCB Bulletin No.012)
#define UIRD_MSGID_PLEASE_ENTER_PIN             0x09 // Please Enter your PIN; PURE exclusive
#define UIRD_MSGID_PLEASE_INSERT_SWIPE_CARD     0x18 // Please insert or swipe card; PURE exclusive



short CbAppDisplayMsg(ST_UIRD *pstUird, unsigned char ucOpsStatus);


#endif
