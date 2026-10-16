/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Trade.h
 *  Version      :
 *  Author       :
 * Description   : Transaction related functions
 **********************************************************************************************************/

#ifndef __TRADE_H__
#define __TRADE_H__

//#include "LibApi.h"

//---------------------------------------------------------------------------
// macro definition
//---------------------------------------------------------------------------
// consumption
#define TT_CONSUMPTION                           0x01102101
// logoin
#define TT_LOGIN                                 0x04005101
// logo out
#define TT_LOGOUT                                0x04005102
// Card insertion consumption
#define TT_ZH_CONTACT_CONSUMPTION                0x01109001
// Card insertion and Contactless
#define TT_ZH_NFC_CONTACT_CONSUMPTION            0x01109002
// Scan QR code for consumption
#define TT_QR_CONSUMPTION                        0x05000000

// Null transaction type code
#define TT_NULL                                  0xFFFFFFFF

#undef u8
#define  u8   unsigned char
#undef U8

#undef s8
#undef S8

#undef u16
#define  u16  unsigned short
#undef U16

#undef s16
#define  s16   short
#undef S16

#undef u32
#define  u32  unsigned int
#undef U32

#undef s32
#define  s32    int
#undef S32

#undef bool
typedef   unsigned char bool;

//Terminal ID size
#define TERM_ID_SIZE                10
//Merchant ID size
#define MER_ID_SIZE                 15
#define MAX_EMAIL_LEN               40
#define MAX_LOGINPWD_LEN            20
#define MIN_LOGINPWD_LEN            8
#define MAX_TOKEN_LEN               250
#define MAX_EMAILCODE_LEN           20
#define MIN_EMAILCODE_LEN           4

#define MAX_NAME_LEN                20
#define MIN_NAME_LEN                1

#define MAX_PHONE_LEN               11
#define MIN_PHONE_LEN               11

#define MAX_PIN_LEN                 10
#define MIN_PIN_LEN                 3

#define MAX_CLIENTID_LEN            20
#define MIN_CLIENTID_LEN            5

#define MAX_BANK_NUM                100
#define MAX_TRANS_NUM               200
#define MAX_PRODUCT_NUM             100
#define MAX_BANKRATE_NUM            100

#define MSG_MAXLEN                  (1024*30)

typedef struct {
    char *version;
    char *code;   // ×´Ì¬·µ»ØÂë
    char *desc;   // ·µ»ØÃèÊö
    char *body;
    int bodySize;
} HttpResponse;


//Entry Mode
typedef enum
{
    MODE_UNKNOWN = 0x00,
    MODE_MANUAL,
    MODE_MSC,
    MODE_SCI,
    MODE_FALLBACK,
    MODE_CONTACTLESS,
} E_ENTRY_MODE;

//Card Type
typedef enum
{
    CARD_UNKNOWN = 0x00,
    CARD_VISA,
    CARD_MASTERCARD,
    CARD_DISCOVER,
    CARD_AMEX,
    CARD_JCB,
    CARD_UNIONPAY,
    CARD_DINERS,
    CARD_VERVE,
} E_CARD_TYPE;



typedef enum
{
    ACCOUNT_DEFAULT = 0X00,
    ACCOUNT_SAVING,
    ACCOUNT_CURRENT,
    ACCOUNT_CREDIT,
} E_ACCOUNT_TYPE;



// card information
typedef struct
{
    E_ENTRY_MODE  eEntryMode;           //card entry mode
    E_CARD_TYPE eCardType;              //card type
    u8  ucCardLen;                      // length of card number
    u8  aucCardNo[20];                  // card number
    u8  aucExpiredDate[6+1];            // expired date(YYMM)
    u8  ucTrack2Len;                    // length of track 2 data
    u8  aucTrack2Data[128];             // track 2 data
    bool ucPinExistFlag;                //1: Pin entered 0 : no pin
    u8 aucPinBlockH[16+1];
    u8 aucServiceCode[4+1];             //track2 service code

	u8 auc9F27[2+1];                    // 9F27
	u8 auc82[4+1];                      // 82 AIP
	u8 auc9F36[4+1];                    // 9F36 ATC
	u8 auc9F33[6+1];                    // 9F33 capabilities
	u8 auc9F34[6+1];                    // cvm
	u8 auc9F26[16+1];                   // 9F26 AC
	u8 auc9F06[32+1];                   // AID
	u8 auc9F20[64+1];                   // name
	u8 auc95[10+1];                     // tvr
	u8 aucClearPinBlock[16+1];
} ST_CARDINFO;

typedef struct
{
    u8 aucICCData[512+2];               //field55 ICClen 2+ ICC data
    u8 aucTVR[10+1];                    //Tag 95  (M): Terminal Verification Results
    u8 ucAIDLen;                        //AID length
    u8 aucAID[32+1];                    //Tag 84  (O): Dedicated File Name/Application ID
    u8 aucTSI[4+1];                     //Tag 9B  (O): Transaction Status Information
    u8 aucPanSeqNoH;                    //Tag 5F34(O): Application PAN Sequence Number
    u8 ucCVMMode;
    u8 ucCVMr[16+1];                    //9f34
    u8 ucAppLable[16+1];
    u8 ucArqc[16+1];
    u8 aucIssuerData[256];              //response field55 ICC data
    short aucIssuerDataLen;
} ST_EMVINFO;


// transaction message info.
typedef struct
{
	u8 aucName[100+1];           // Name
	u8 aucAllowance[40+1];           // Allowance
	long lCode;             //code
	u8 aucCode[40+1];           //Code
	u8 aucValidity[40+1];             //Validity
	u8 aucPrice[12+1];      //price format for DATA
	long lPeriod;             //period used for TV pay
} ST_PRODUCT;


// transaction infos.
typedef struct
{
    u8 aucAmount[6];                // #04 Amount
    u8 aucDateTime[10+1];           // #07 Transmission Date & Time MMDDhhmmss
    u8 aucTransDate[6+1];           // for report purpose YYMMDD
    long lTraceNo;                  // #11 System Trace Audit Number
    u8 aucEntryMode[3+1];           // #22 Point of Service Entry Mode
    u8 aucAcqCode[11+1];            // Field #32 - Acquiring Institution Identification Code
    u8 aucTermID[TERM_ID_SIZE+1];   // #41 Card Acceptor Terminal ID(TID): stTermInfo.asTermID
    u8 aucMerchID[MER_ID_SIZE+1];   // #42 Card Acceptor Identification Code(MID): stTermInfo.asMID
    u8 aucMerchInfo[40+1];          // Field #43 - Card Acceptor Name / Location
    u8 aucCurrencyCode[3+1];        // #49 Transaction Currency Code
    u8 aucReasonCode[4+1];          // Field #56 ¡§C Message Reason Code
    u8 aucRefNo[12+1];              // #37 Retrieval Ref. Number
    u8 aucAuthCode[6+1];            // #38 Authorisation Identification Res
    u8 aucRespCode[2+1];            // #39 response code
    u8 aucMsg[99+1];                // message

    //account type
    E_ACCOUNT_TYPE eAccType;
    u8 eTransID;                    // transaction ID
    u8 aucMerchTypeCode[4+1];       // merchant type
    u8 aucCBAmt[12+1];              // cashback amount

    //original data
    u8 aucOrigDate[4+1];            // original date for VOID purpose
    u8 aucOrigTime[6+1];            // original time for VOID purpose
    long lOrigTraceNo;
    u8 aucRespMsg[64+1];
    long lSysBatchNo;               // #11 System Trace Audit Number
    u8 aucBalance[8];
    u8 eTransQrCodeID;
    u8 OrigTransID;                 // transaction ID
    u8 aucOrigRefNo[12+1];
    u8 aucOrigAmount[12+1];
} ST_TRANSINFO;


//transaction recode will be stored in DB
typedef struct
{
    ST_CARDINFO stCardInfo;                 // card info
    ST_EMVINFO stEmvInfo;                   // EMV info
    ST_TRANSINFO stTransInfo;               // MSG info
} ST_TRANSDATA;

//-----------------------------------------------------------------------------
// variable declaration
//-----------------------------------------------------------------------------
extern ST_TRANSDATA gstTransData;
//-----------------------------------------------------------------------------

// Function declaration
//-----------------------------------------------------------------------------

short CupTradeInit(int iTimeout, unsigned long ulOpCode);
void SetCash(unsigned char *pcIN, unsigned char pucLen, unsigned char *pcOut);
short CupOpSum(int iTimeout, unsigned char ucOpSumFlag, unsigned char *pucMaxSum, unsigned char bNeedEnterSum);
short CupOpInputPinProcess(int iTimeout, unsigned long ulOpCode);
short CupOpTxJsonPack(short (*CallBack)(unsigned char *pucOut, unsigned short *pucOutLen), unsigned long ulOpCode, int iTimeout);
short CupOpTx8583Pack(short (*CallBack)(unsigned char *pucOut, unsigned short *pucOutLen), unsigned long ulOpCode, int iTimeout);
short CupOpRx8583Pack(int iTimeout, unsigned long ulOpCode, short (*CallBack)(unsigned char *pucIn, unsigned short usInLen));
void CupOpResultDeal(unsigned long ulOpCode, int iTimeout);
void IsFastRevert(int iTimeout, unsigned long ulOpCode);
void DisplayDisEnbleIC(void);
short PosReversal(void);
short SearchCard(char *title);

#endif /*__TRADE_H__*/

