/**********************************************************************************************************
 * Copyright(C)2018, 
 * File Name     : EmvError.h
 * Version       : v1.0
 * Author        :
 * Description   : EMV kernel error code
 **********************************************************************************************************/

#ifndef __EMV_ERROR_H__
#define __EMV_ERROR_H__

//#include "EmvTrace.h"

#if !defined(__NOTRACE)
#define RELEASE_TEST    0
#else
#define RELEASE_TEST    1
#endif

#if !(RELEASE_TEST)
#define EMV_TRACE_ERR(...)         //do{EMV_TRACE("ERROR   %s %d : ", __FUNCTION__, __LINE__);EMV_TRACE(__VA_ARGS__);}while(0)
#else
#define EMV_TRACE_ERR(...)
#endif


#define ERR_LIST(_) \
    _(ERR_REDUNDANCY,                1) /**< data redundancy */                    \
    _(ERR_OK,                        0) /**< success */                        \
    _(ERR_NORMAL,                   -1) /**< General error */                    \
    _(ERR_PARAMETER,                -2) /**< parameter error */                    \
    _(ERR_OUT_OF_MEMORY,            -3) /**< out of memory */                    \
    _(ERR_TIMEOUT,                  -4) /**< timeout */                        \
    _(ERR_NONE_DATA,                -5) /**< No Data */                      \
    _(ERR_TLV_ITERATION_COMPLETE,   -6) /**< TLV List parses to the last TAG  */  \
    _(ERR_KERNEL_NEXT,              -7) /**< Select secondary applications  */               \
    _(ERR_KERNEL_RESTART,           -8) /**< Restart kernel  */                   \
    _(ERR_KERNEL_EXIT,              -9) /**< exit kernel  */                   \
    _(ERR_L1_TIME_OUT,              -10)    /* timeout error                 */  \
    _(ERR_L1_TRANSMISSION,          -11)    /* transmission error                 */  \
    _(ERR_L1_PROTOCOL,              -12)    /* Protocol error                 */  \
    _(ERR_L1_COLLISION,             -13)    /* Cards Conflict                 */  \
    _(ERR_SELECT_AIDS,              -14)    /* Enter AID list & select          */  \
    _(ERR_OTHER_INTERFACE,          -1001)  /* Please try other type Channel       */  \
    _(ERR_QPBOC_OFFLINE,            -1002)  /* Offline acceptance of contactless QPBOC transactions  */  \
    _(ERR_QPBOC_ONLINE,             -1003)  /* contactless QPBOC transaction online*/  \
    _(ERR_PBOC_ONLINE,              -1004)  /* contactless PBOC transaction online       */  \
    _(ERR_MSD_ONLINE,               -1005)  /* contactless MSD transaction online        */  \
    _(ERR_EC_ACCEPT,                -1006)  /* Offline acceptance of e-cash         */  \
    _(ERR_OFFLINE_ACCEPT,           -1007)  /* Standard process offline acceptance         */  \
    _(ERR_CARD_REMOVED,             -1008)  /* Card removed during transaction         */  \
    _(ERR_COMMAND_FAIL,             -1009)  /* Card reading failure                 */  \
    _(ERR_CARD_BLOCK,               -1010)  /* Card locked               */  \
    _(ERR_CABDIDATELIST_EMPTY,      -1011)  /* No available AID               */  \
    _(ERR_APP_BLOCK,                -1012)  /* App locked               */  \
    _(ERR_FALLBACK,                 -1013)  /* transaction fallback             */  \
    _(ERR_AUTH_FAIL,                -1014)  /* Data auth failed             */  \
    _(ERR_APP_INEFFECT,             -1015)  /* application not yet effect             */  \
    _(ERR_APP_EXPIRED,              -1016)  /* application expired               */  \
    _(ERR_CVM_FAIL,                 -1017)  /* Cardholder verify failed           */  \
    _(ERR_ARPC_FAIL,                -1018)  /* issuing bank certification failed           */  \
    _(ERR_SCRIPT_FAIL,              -1019)  /* Issuing bank script execution failed       */  \
    _(ERR_APP_NOACCEPT,             -1020)  /* application not accepted & selected again   */  \
    _(ERR_EC_DECLINED,              -1021)  /* E-cash offline rejection         */  \
    _(ERR_ICC_DATA_ERROR,         	-1022) 	/* IC card data template error     */	\
	_(ERR_ICC_DATA_REDUNDANCY,    	-1023) 	/* IC card data redundancy  */	\
	_(ERR_ICC_DATA_LACK,          	-1024) 	/* IC card data missed             */  \
	_(ERR_ICC_NO_RECCORD,          	-1025) 	/* IC card no record, read SFI TAG_88 */  \
    _(ERR_CHECK_DATE,            	-1026) 	/* Error processing restriction          */  \
    _(ERR_KERNEL_NOT_SUPPORT,      	-1027) 	/* No supported kernel types       */  \
    _(ERR_CDCVM_TRY_AGAIN,      	-1028) 	/* CDCVM secondary swing card            */  \
    _(ERR_USER_CANCEL,      	    -1029) 	/* User cancel                 */  \
    _(ERR_CVM_PIN_VERIFY_FAIL,      -1030) 	/* CVM verify PIN failed           */  \
    _(ERR_CVM_PIN_BYPASS,           -1031) 	/* CVM skips PIN input           */  \
    _(ERR_CVM_PIN_EXCEED,           -1032) 	/* CVM PIN retry count exceeded limit       */  \
    _(ERR_SCRIPT_FORMAT,            -1033) 	/* Script format error             */  \
    _(ERR_ICC_CID_ABNORMAL,         -1034) 	/* Card response CID abnormality          */  \
	_(ERR_ICC_MSD_ABNORMAL,         -1035) 	/* Magnetic stripe card abnormal response           */  \
    _(ERR_TORN_TRANSACTION,         -1036) 	/* flash card                     */  \
    _(ERR_MULTIPLE_NFC_CARD,        -1037) 	/* Mult-card conflicts               */  \
    _(ERR_MAGNETIC_FAIL,            -1038)  /* Magnetic stripe card swipe failure           */  \
	_(ERR_ONLINE_FAIL,              -1039)  /* Online failure                 */  \
    _(ERR_HASH_CHECK,               -1040)  /* HASH Verify error             */  \
    _(ERR_DATA_INVALID,             -1041)  /* Invalid data                 */  \
    _(ERR_UNREALIZED,               -1042)  /* Not yet implemented                 */  \
    _(ERR_OTHER_ERROR,              -1043)  /* Other error exceptions             */  \
	_(ERR_AUTH_FAIL_SWNO9000,       -1044)  /* Internal Auth command, return SW not 9000             */  \
    _(ERR_IC_F062,                  -3998)  /* Data Auth fDDA failed, 9F6Cbit6-5=01 and terminal supports PBOC transaction failure F062 Data Auth failed    */  \
    _(ERR_IC_F061,                  -3999)  /* Data Auth fDDA failed and transaction rejection failed F061 Data Auth failed                               */  \
    _(ERR_IC_F060,                  -4000)  /* Read record, length of the record template of response to the reading record card is incorrect,not 70 template, transaction failed. F060 card template format is abnormal */  \
    _(ERR_IC_F059,                  -4007)  /* Read record, 57 (equivalent data of track 2) duplicate transaction failure F059 Card track 2 equivalent data duplicate               */  \
    _(ERR_IC_F058,                  -4008)  /* Read record, 5F24 (application expiration date) duplicate transaction failure F058 Card application expiration date duplicate               */  \
    _(ERR_IC_F057,                  -4009)  /* Read record ,PAN (primary account) duplicate transaction failed F057 Card primary account duplicate                             */  \
    _(ERR_IC_F056,                  -4010)  /* Read Record, Read Record Card Response 9001 Transaction Failure F056 Card Response Exception                               */  \
    _(ERR_IC_F055,                  -4011)  /* Read Record, Read Record Card Response 6500 Transaction Failed F055 Card Response Exception                               */  \
    _(ERR_IC_F054,                  -4012)  /* Read Record, Read Record Card Response 6400 Transaction Failed F054 Card Response Exception                               */  \
    _(ERR_IC_F053,                  -4013)  /* Read Record, Read Record Card Response 6A88 Transaction Failed F053 Card Not Found Reference Data                         */  \
    _(ERR_IC_F052,                  -4014)  /* Read Record, Read Record Card Response 6A83 Transaction Failed F052 Card No Record Found                             */  \
    _(ERR_IC_F051,                  -4015)  /* Read Record, Read Record Card Response 6A82 Transaction Failed F051 Card Not Found File                           */  \
    _(ERR_IC_F050,                  -4016)  /* Read Record, Read Record Card Response 6A81 Transaction Failed F050 Card Locked                                   */  \
    _(ERR_IC_F049,                  -4023)  /* Read Record, Read Record Card Response 6985 Transaction Failed F049 Card Usage Conditions Not Met                         */  \
    _(ERR_IC_F048,                  -4024)  /* Read Record, Read Record Card Response 6984 Transaction Failure F048 Card Verification Failure                               */  \
    _(ERR_IC_F047,                  -4025)  /* Read Record, Read Record Card Response 6983 Transaction Failure F047 Card Verification Failure                               */  \
    _(ERR_IC_F046,                  -4026)  /* Read Record, Read Record Card Response 63C1 Transaction Failure F046 Card Verification Failure                               */  \
    _(ERR_IC_F045,                  -4027)  /* Read Record, Read Record Card Response 6300 Transaction Failure F045 Card Verification Failure                               */  \
    _(ERR_IC_F044,                  -4028)  /* Read Record, Read Record Card Response 6283 Transaction Failed F044 Card Application Locked                               */  \
    _(ERR_IC_F043,                  -4029)  /* Read record, abnormal file transaction failed F043. The card has been blacklisted                                   */  \
    _(ERR_IC_F042,                  -4030)  /* Read records, terminal date greater than application expiration date, transaction failed F042 card expired                           */  \
    _(ERR_IC_F041,                  -4031)  /* Initialization, GPO command returns 9408 transaction failure F041 sleep card, please proceed with online transaction*/  \
    _(ERR_IC_F040,                  -4032)  /* Initialization, AFL contains incorrect SFI (including values of 0 or 31 SFI start record number is abnormal SFI end record number exception, record number exception participating in data authentication) transaction failure F040 card short file identifier exception  */  \
    _(ERR_IC_F039,                  -4039)  /* Initialization, GPO response error template, incorrect template length AFL's TAG error, AIP length error transaction failure F039 card template format abnormality*/  \
    _(ERR_IC_F038,                  -4040)  /* Initialization, card 9F10 rejected transaction failed F038 card rejected                                     */  \
    _(ERR_IC_F037,                  -4041)  /* Initialization, GPO response does not include issuer data (9F26), transaction failed, F037 card lacks necessary data (9F26)        */  \
    _(ERR_IC_F036,                  -4042)  /* Initialization, GPO response does not include issuer data (9F10), transaction failed, F036 card lacks necessary data (9F10)        */  \
    _(ERR_IC_F035,                  -4043)  /* Initialization, GPO response does not contain mandatory data (57), transaction failed, F035 card lacks necessary data (57)              */  \
    _(ERR_IC_F034,                  -4044)  /* Initialization,  GPO response does not contain mandatory data (9F36), transaction failed, F034 card lacks necessary data (ATC)           */  \
    _(ERR_IC_F033,                  -4045)  /* Initialization, GPO response does not contain mandatory data (82), transaction failed F033, missing essential data (AIP)                 */  \
    _(ERR_IC_F032,                  -4046)  /* Initialization, GPO command returns 6985 transaction failure F032 card does not meet usage conditions                          */  \
    _(ERR_IC_F031,                  -4047)  /* Initialization, GPO command returns 6984 transaction failure F031 card verification failure                                */  \
    _(ERR_IC_F030,                  -4048)  /* Initialization, GPO command returns 6400 transaction failure F030 card response exception                                */  \
    _(ERR_IC_F029,                  -4055)  /* Initialization, GPO command returns 6500 prompts for other interfaces                                            */  \
    _(ERR_IC_F028,                  -4056)  /* Initialization, GPO command returns 6A88 transaction failure F028 card not found reference data                          */  \
    _(ERR_IC_F027,                  -4057)  /* Initialization, GPO command returns 6A83 transaction failure F027 card record does not exist                           */  \
    _(ERR_IC_F026,                  -4058)  /* Initialization, GPO command returns 6A82 transaction failure F026 card not found this file                            */  \
    _(ERR_IC_F025,                  -4059)  /* Initialization, GPO command returns 6A81 transaction failure F025 card locked                                    */  \
    _(ERR_IC_F024,                  -4060)  /* Initialization, GPO command returns 9001 transaction failure F024 card response exception                                */  \
    _(ERR_IC_F023,                  -4061)  /* Initialization, GPO command returns 6986 transaction failure F023 card does not support this command                            */  \
    _(ERR_IC_F022,                  -4062)  /* Initialization, GPO command returns 6983 transaction failure F022 card verification failure                                */  \
    _(ERR_IC_F021,                  -4063)  /* Initialization, GPO command returns 63C1 transaction failure F021 card verification failure                                */  \
    _(ERR_IC_F020,                  -4064)  /* Initialization, GPO command returns 6300 transaction failure F020 card verification failure                                */  \
    _(ERR_IC_F019,                  -4071)  /* Initialization, GPO command returns 6283 transaction failure F019 card application lock                                */  \
    _(ERR_IC_F018,                  -4072)  /* Initialization, GPO Response Exception Transaction Failure F018 Card GPO Exception                                     */  \
    _(ERR_IC_F017,                  -4073)  /* App select, Final application selection card response 6A82 transaction failure F017 card does not have the application text               */  \
    _(ERR_IC_F016,                  -4074)  /* App select, Final Application Selection Card Response 9001 Transaction Failure F016 Card Response Exception                       */  \
    _(ERR_IC_F015,                  -4075)  /* App select, Final Application Selection Card Response 6500 Transaction Failed F015 Card Response Exception                       */  \
    _(ERR_IC_F014,                  -4076)  /* App select, Final application selection card response 6400 transaction failure F014 card response exception                       */  \
    _(ERR_IC_F013,                  -4077)  /* App select, Final application selection card response 6283 transaction failure F013 card application lock                       */  \
    _(ERR_IC_F012,                  -4078)  /* App select, Final application selection card response 6A88 transaction failure F012 card not found reference data                 */  \
    _(ERR_IC_F011,                  -4079)  /* App select, Final application selection card response 6A83 transaction failed F011 card does not have this record                   */  \
    _(ERR_IC_F010,                  -4080)  /* App select, Final application selection card response 6985 transaction failed F010 card does not meet usage conditions                 */  \
    _(ERR_IC_F009,                  -4087)  /* App select, Final application selection card response 6984 transaction failure F009 card verification failure                       */  \
    _(ERR_IC_F008,                  -4088)  /* App select, Final application selection card response 6983 transaction failure F008 card verification failure                       */  \
    _(ERR_IC_F007,                  -4089)  /* App select, Final application selection card response 63C1 transaction failed F007 card verification failed                       */  \
    _(ERR_IC_F006,                  -4090)  /* App select, Final application selection card response 6300 transaction failure F006 card verification failure                       */  \
    _(ERR_IC_F005,                  -4091)  /* App select, PDOL without 9F66 for AID selection transaction failed     F005 card not exist 9F66                        */  \
    _(ERR_IC_F004,                  -4092)  /* App select, AID selection did not return PDOL. Transaction failed. F004 card does not have PDOL                          */  \
    _(ERR_IC_F003,                  -4093)  /* Preprocess, Transaction Failure F003 Exceeding Non Connected Offline Limit                           */  \
    _(ERR_IC_F002,                  -4094)  /* Preprocess, Transaction amount is 0, transaction failed F002, transaction amount incorrect                                      */  \
    _(ERR_IC_F001,                  -4095)  /* Preprocess, The transaction amount is greater than the non accepted transaction limit. Transaction failed. F001 The amount exceeds the non accepted limit*/

#define ERR_DEFINE_ENUM(err,num)    err = num,

typedef enum _ERR
{
    ERR_LIST(ERR_DEFINE_ENUM)
}ERR;



// Pass in a function call statement, and if the function does not return ERR_OK, return it from the current function
// eg : ERR_RETURN(Foo());
// The current function definition needs to have an INT type variable err
#define ERR_RETURN(func)                                        \
    do{ ERR err_code = func;                                    \
        if (err_code != ERR_OK) {                               \
        EMV_TRACE_ERR(" Return %s.\r\n", ErrString(err_code));  \
        return err_code;}                                       \
    }while(0)

#if !(RELEASE_TEST)

#define ERR_SHOW(func)  func
#else
#define ERR_SHOW(func)  func
#endif



// Pass in an error code, if it is not ERR_OK, return it from the current function
// Define local variable err_code to prevent confusion with ERR-RETURN from affecting efficiency and unexpected errors
#define ERR_RETURN_CODE(errCode)                                \
    do{ ERR err_code = errCode;                                 \
        if (err_code != ERR_OK) {                               \
        EMV_TRACE_ERR(" Return %s.\r\n", ErrString(err_code));  \
        return err_code;}                                       \
    }while(0)


void SortErrString(void);
const char *ErrString(ERR err);

#endif
