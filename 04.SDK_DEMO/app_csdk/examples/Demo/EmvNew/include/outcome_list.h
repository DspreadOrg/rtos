/*
********************************************************************************
*
*   File Name:
*       outcome_list.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _OUTCOME_LIST_H
#define _OUTCOME_LIST_H

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
/*Kernel ID*/
#define KERNEL_ID_EMV                                                     0xE0

#define KERNEL_ID_MASTERCARD                                              0x02
#define KERNEL_ID_VISA                                                    0x03
#define KERNEL_ID_AMEX                                                    0x04
#define KERNEL_ID_JCB                                                     0x05
#define KERNEL_ID_D_PAS                                                   0x06
#define KERNEL_ID_CUP                                                     0x07
#define KERNEL_ID_RUPAY                                                   0x0D
#define KERNEL_ID_INTERAC                                                 0x21
#define KERNEL_ID_PURE                                                    0x22
#define KERNEL_ID_MIRPS                                                   0x23
#define KERNEL_ID_BANCOMAT                                                0x24
#define KERNEL_ID_VCCS                                                    0x25
#define KERNEL_ID_CPACE													  0x26

/*Transaction Type Code*/
#define TRANSACTION_TYPE_CODE_EMV_GOODS                                   0x00
#define TRANSACTION_TYPE_CODE_EMV_SERVICES                                0x00
#define TRANSACTION_TYPE_CODE_EMV_PURCHASE                                0x00
#define TRANSACTION_TYPE_CODE_EMV_CASH                                    0x01
#define TRANSACTION_TYPE_CODE_EMV_CASHBACK                                0x09
#define TRANSACTION_TYPE_CODE_EMV_MANUAL_CASH                             0x17
#define TRANSACTION_TYPE_CODE_EMV_CASH_DISBURSEMENT                       0x17
#define TRANSACTION_TYPE_CODE_EMV_REFUND                                  0x20
#define TRANSACTION_TYPE_CODE_EMV_DEPOSIT                                 0x21
#define TRANSACTION_TYPE_CODE_EMV_NON_LEGACY_MONEY_ADD					  0x28
#define TRANSACTION_TYPE_CODE_EMV_BALANCE_UPDATE                          0x29
#define TRANSACTION_TYPE_CODE_EMV_QUERY                                   0x31
#define TRANSACTION_TYPE_CODE_EMV_LEGACY_MONEY_ADD						  0x33
#define TRANSACTION_TYPE_CODE_EMV_VOID									  0x34
#define TRANSACTION_TYPE_CODE_EMV_TRANSFER                                0x40
#define TRANSACTION_TYPE_CODE_EMV_PAYMENT                                 0x50
#define TRANSACTION_TYPE_CODE_EMV_ADMIN                                   0x66
#define TRANSACTION_TYPE_CODE_EMV_SERVICE_CREATION						  0x83

/*Account Type Code*/
#define TRANSACTION_ACCOUNT_TYPE_DEFAULT                                  0x00
#define TRANSACTION_ACCOUNT_TYPE_SAVINGS                                  0x10
#define TRANSACTION_ACCOUNT_TYPE_CHEQUE_DEBIT                             0x20
#define TRANSACTION_ACCOUNT_TYPE_CREDIT                                   0x30

/*Cryptogram Info Bit Map*/
#define CRYPTOGRAM_INFO_TC                                                0x40
#define CRYPTOGRAM_INFO_ARQC                                              0x80
#define CRYPTOGRAM_INFO_AAC                                               0x00
#define CRYPTOGRAM_INFO_AAR                                               0xC0
#define CRYPTOGRAM_INFO_CDA_REQUIRED                                      0x10
#define CRYPTOGRAM_INFO_ADVICE_REQUIRED                                   0x08
#define CRYPTOGRAM_INFO_SERVICE_NOT_ALLOWED                               0x01
#define CRYPTOGRAM_INFO_PIN_TRY_LIMIT_EXCEED                              0x02
#define CRYPTOGRAM_INFO_ISSUER_AUTH_FAILED                                0x03

/*ASI defination*/
#define ASI_PARTIAL_MATCH                                                 0x00
#define ASI_EXACT_MATCH                                                   0x01

/*CVM Code*/
#define CVM_TYPE_FAIL                                                     0x00
#define CVM_TYPE_PLAINTEXT_PIN_ICC                                        0x01
#define CVM_TYPE_ENCIPH_PIN_ONLINE                                        0x02
#define CVM_TYPE_PLAINTEXT_PIN_ICC_SIGN                                   0x03
#define CVM_TYPE_ENCIPH_PIN_ICC                                           0x04
#define CVM_TYPE_ENCIPH_PIN_ICC_SIGN                                      0x05
#define CVM_TYPE_SIGN                                                     0x1E
#define CVM_TYPE_NO_CVM                                                   0x1F
#define CVM_TYPE_ID_CARD                                                  0x20
#define CVM_TYPE_UNRECOGNISED                                             0x3F

/*CVM Condition*/
#define COND_ALWAYS                                                       0x00
#define COND_CASH                                                         0x01
#define COND_NOTCASH                                                      0x02
#define COND_IF_SUPPORTED                                                 0x03
#define COND_IF_MANUAL_CASH                                               0x04
#define COND_IF_PURCHASE_WITH_CASHBACK                                    0x05
#define COND_UNDER_X_AMOUNT                                               0x06
#define COND_OVER_X_AMOUNT                                                0x07
#define COND_UNDER_Y_AMOUNT                                               0x08
#define COND_OVER_Y_AMOUNT                                                0x09

/*CVM Result*/
#define CVM_RESULT_UNKNOWN                                                0x00
#define CVM_RESULT_FAILED                                                 0x01
#define CVM_RESULT_SUCCESSFUL                                             0x02

/*ARC*/
#define ARC_OFFLINEAPPROVED                                               "Y1"
#define ARC_OFFLINEDECLINED                                               "Z1"
#define ARC_REFERRALAPPROVED                                              "Y2"
#define ARC_REFERRALDECLINED                                              "Z2"
#define ARC_ONLINEFAILOFFLINEAPPROVED                                     "Y3"
#define ARC_ONLINEFAILOFFLINEDECLINED                                     "Z3"

// Control flow flag
#define CVM_APPLY_NEXT                                                    0x40

#define DATA_CAPTURE_TYPE_BDC                                             0x00
#define DATA_CAPTURE_TYPE_ODC                                             0x01

/*Transaction Message Type*/
#define TRANS_MSG_TYPE_FINANCIAL_AUTH                                     0x41
#define TRANS_MSG_TYPE_AUTH                                               0x42
#define TRANS_MSG_TYPE_FINANCIAL_CONFIRM                                  0x43
#define TRANS_MSG_TYPE_BATCHUP                                            0x44
#define TRANS_MSG_TYPE_ADVICE                                             0x45
#define TRANS_MSG_TYPE_REVERSAL                                           0x46

/*EMV Message Type, Book4*/
#define EMV_MSG_TYPE_CLEARING_MESSAGE                                     0x01
#define EMV_MSG_TYPE_AUTHORISATION_REQUEST								  0x02
#define EMV_MSG_TYPE_RESPONSE_MESSAGE                                     0x03

/*EMV Transaction Advice Type*/
#define EMV_TRANS_ADVICE_SERVICE_NOT_ALLOWED                              0x01
#define EMV_TRANS_ADVICE_PIN_TRY_LIMIT_EXCEEDED                           0x02
#define EMV_TRANS_ADVICE_ISSUER_AUTH_FAILED                               0x03
#define EMV_TRANS_ADVICE_ONLINE_FAILED_MERCHANT_FORCE_APPROVED            0x04
#define EMV_TRANS_ADVICE_OFFLINE_FAILED_MERCHANT_FORCE_APPROVED           0x05
#define EMV_TRANS_ADVICE_TYPE_OTHER                                       0xFF

/*OPS*/
#define CONTACTLESS_OPS_STATUS_APPROVED                                   0x10
#define CONTACTLESS_OPS_STATUS_DECLINED                                   0x20
#define CONTACTLESS_OPS_STATUS_ONLINE_REQUEST                             0x30
#define CONTACTLESS_OPS_STATUS_END_APPLICATION                            0x40
#define CONTACTLESS_OPS_STATUS_SELECT_NEXT                                0x50
#define CONTACTLESS_OPS_STATUS_TRY_ANOTHER_INTERFACE                      0x60
#define CONTACTLESS_OPS_STATUS_TRY_AGAIN                                  0x70
#define CONTACTLESS_OPS_STATUS_NA                                         0xF0

#define CONTACTLESS_OPS_STATUS_ONLINE_REQUEST_TWO_PRESENTMENTS            0xC0
#define CONTACTLESS_OPS_STATUS_ONLINE_REQUEST_PRESENT_AND_HOLD            0xD0
#define CONTACTLESS_OPS_STATUS_END_APPLICATION_WITH_RESTART_COMMUNICATION_ERROR 0xA0
#define CONTACTLESS_OPS_STATUS_END_APPLICATION_WITH_RESTART_ON_DEVICE_CVM 0xB0

#define CONTACTLESS_OPS_START_A                                           0x00
#define CONTACTLESS_OPS_START_B                                           0x10
#define CONTACTLESS_OPS_START_C                                           0x20
#define CONTACTLESS_OPS_START_D                                           0x30
#define CONTACTLESS_OPS_START_NA                                          0xF0

#define CONTACTLESS_OPS_ONLINE_RESPONSE_DATA_NA                           0xF0
#define CONTACTLESS_OPS_ONLINE_RESPONSE_DATA_EMV_DATA                     0x03
#define CONTACTLESS_OPS_ONLINE_RESPONSE_DATA_ANY                          0x04
//BCTC defined OPS-Online Response Data
#define BCTC_OPS_ONLINE_RESPONSE_DATA_EMV_DATA	                          0x10
#define BCTC_OPS_ONLINE_RESPONSE_DATA_ANY                                 0x20

#define CONTACTLESS_OPS_CVM_NO_CVM                                        0x00
#define CONTACTLESS_OPS_CVM_SIGNATURE                                     0x10
#define CONTACTLESS_OPS_CVM_ONLINE_PIN                                    0x20
#define CONTACTLESS_OPS_CVM_CONFIRMATION_CODE_VERIFIED                    0x30
#define CONTACTLESS_OPS_CVM_NA                                            0xF0

#define CONTACTLESS_OPS_DATA_PRESENCE_MASK_UIR_ON_OUTCOME                 0x80
#define CONTACTLESS_OPS_DATA_PRESENCE_MASK_UIR_ON_RESTART                 0x40
#define CONTACTLESS_OPS_DATA_PRESENCE_MASK_DATA_RECORD                    0x20
#define CONTACTLESS_OPS_DATA_PRESENCE_MASK_DISCRET_DATA                   0x10
#define CONTACTLESS_OPS_DATA_PRESENCE_MASK_RECEIPT                        0x08

#define CONTACTLESS_OPS_ALTERNATIVE_INTERFACE_PREF_NA                     0xF0
#define CONTACTLESS_OPS_ALTERNATIVE_INTERFACE_PREF_ContactChip            0x01
#define CONTACTLESS_OPS_ALTERNATIVE_INTERFACE_PREF_MagStripe              0x02
//BCTC defined OPS-Alternative Interface Preference
#define BCTC_OPS_ALTERNATIVE_INTERFACE_PREF_ContactChip                   0x10

#define CONTACTLESS_OPS_FIELD_OFF_REQUEST_NA                              0xFF

/*UIRD*/
#define CONTACTLESS_UIRD_MessageID_APPROVED                               0x03
#define CONTACTLESS_UIRD_MessageID_CARD_ERROR                             0x06
#define CONTACTLESS_UIRD_MessageID_DECLINED                               0x07
#define CONTACTLESS_UIRD_MessageID_ENTER_PIN                              0x09
#define CONTACTLESS_UIRD_MessageID_PROCESSING_ERROR                       0x0F
#define CONTACTLESS_UIRD_MessageID_REMOVE_CARD                            0x10
#define CONTACTLESS_UIRD_MessageID_WELCOME                                0x14
#define CONTACTLESS_UIRD_MessageID_PRESENT_CARD                           0x15
#define CONTACTLESS_UIRD_MessageID_PROCESSING                             0x16
#define CONTACTLESS_UIRD_MessageID_CARD_READ_OK                           0x17
#define CONTACTLESS_UIRD_MessageID_PLEASE_INSERT_OR_SWIPE_CARD            0x18
#define CONTACTLESS_UIRD_MessageID_PLEASE_PRESENT_ONE_CARD_ONLY           0x19
#define CONTACTLESS_UIRD_MessageID_APPROVED_SIGN                          0x1A
#define CONTACTLESS_UIRD_MessageID_AUTHORISING_PLEASEWATI                 0x1B
#define CONTACTLESS_UIRD_MessageID_ERROR_OTHER_CARD                       0x1C
#define CONTACTLESS_UIRD_MessageID_INSERT_SWIPE_OR_TRY_ANOTHER_CARD       0x1C
#define CONTACTLESS_UIRD_MessageID_INSERT_CARD                            0x1D
#define CONTACTLESS_UIRD_MessageID_CLEAR_DISPLAY                          0x1E
#define CONTACTLESS_UIRD_MessageID_SEE_PHONE                              0x20
#define CONTACTLESS_UIRD_MessageID_TRY_AGAIN                              0x21
#define CONTACTLESS_UIRD_MessageID_DPAS_GPO_6987                          0x22
#define CONTACTLESS_UIRD_MessageID_EXPDATE_DECLINE                        0x71
#define CONTACTLESS_UIRD_MessageID_EXPDATE_ONLINE                         0x72
#define CONTACTLESS_UIRD_MessageID_BALCK_LIST_CARD_DECLINE                0x73
#define CONTACTLESS_UIRD_MessageID_DELAY_AUTH                             0xA1
#define CONTACTLESS_UIRD_MessageID_AGAIN_FOR_A_MATCH_CARD                 0xA2
#define CONTACTLESS_UIRD_MessageID_ONLINE_APPROVED                        0xB0
#define CONTACTLESS_UIRD_MessageID_ONLINE_DECLINED                        0xB1
#define CONTACTLESS_UIRD_MessageID_TIMEOUT                                0xB2
#define CONTACTLESS_UIRD_MessageID_USER_CANCEL                            0xB3
#define CONTACTLESS_UIRD_MessageID_NA                                     0xFF

#define CONTACTLESS_UIRD_STATUS_NOT_READY                                 0x00
#define CONTACTLESS_UIRD_STATUS_IDLE                                      0x01
#define CONTACTLESS_UIRD_STATUS_READY_TO_READ                             0x02
#define CONTACTLESS_UIRD_STATUS_PROCESSING                                0x03
#define CONTACTLESS_UIRD_STATUS_CARD_READ_SUCCESSFULLY                    0x04
#define CONTACTLESS_UIRD_STATUS_PROCESSING_ERROR                          0x05
#define CONTACTLESS_UIRD_STATUS_NA                                        0xFF

#define CONTACTLESS_UIRD_VALUE_QUALIFIER_NONE                             0x00
#define CONTACTLESS_UIRD_VALUE_QUALIFIER_AMOUNT                           0x10
#define CONTACTLESS_UIRD_VALUE_QUALIFIER_BALANCE                          0x20

/*Contact Display ID*/
#define CONTACT_DISP_ID_OFFLINE_APPROVED                                  0x30
#define CONTACT_DISP_ID_OFFLINE_DECLINED                                  0x31
#define CONTACT_DISP_ID_ONLINE_APPROVED                                   0x32
#define CONTACT_DISP_ID_ONLINE_DECLINED                                   0x33
#define CONTACT_DISP_ID_UNABLEONLINE_OFFLINE_APPROVED                     0x34
#define CONTACT_DISP_ID_UNABLEONLINE_OFFLINE_DECLINED                     0x35
#define CONTACT_DISP_ID_TRY_ANOTHER_INTERFACE                             0x36
#define CONTACT_DISP_ID_TERMINATE                                         0x37
#define CONTACT_DISP_ID_NOT_ALLOWED                                       0x38
#define CONTACT_DISP_ID_NOT_ACCEPTED                                      0x39
#define CONTACT_DISP_ID_PIN_TRY_LIMIT_EXCEEDED                            0x3A

/*SIGNAL*/
#define	CORRESPONDING_SIGNAL_ACT                                          0xA0
#define CORRESPONDING_SIGNAL_STOP                                         0xA1
#define CORRESPONDING_SIGNAL_CLEAN                                        0xA2
#define CORRESPONDING_SIGNAL_DET                                          0xA3
#define CORRESPONDING_SIGNAL_RA                                           0xA4
#define	CORRESPONDING_SIGNAL_L1RSP                                        0xA5
#define	CORRESPONDING_SIGNAL_TIMEOUT                                      0xA6
#define	CORRESPONDING_SIGNAL_CARD_ACTIVE                                  0xA7
#define CORRESPONDING_SIGNAL_MAG_CARD_ACTIVE                              0xA8
#define CORRESPONDING_SIGNAL_IC_CARD_ACTIVE                               0xA9
#define CORRESPONDING_SIGNAL_CANCEL                                       0xAA

#define CORRESPONDING_SIGNAL_L1RSP_TIME_OUT_ERROR                         0x01
#define CORRESPONDING_SIGNAL_L1RSP_PROTOCOL_ERROR                         0x02
#define CORRESPONDING_SIGNAL_L1RSP_TRANSMISSION_ERROR                     0x03
#define CORRESPONDING_SIGNAL_L1RSP_COLLISION_ERROR                        0x04

/*CMD*/
#define CMD_REQUEST_SAME_CARD_TOTAL_TRANS_AMOUNT                          0x91
#define CMD_REQUEST_RUPAY_SERVICE_SHORTLISTING                            0x92
#define CMD_REQUEST_RUPAY_SERVICE_GET_PRMACQ_KEY                          0x93

#define CMD_REQUEST_DATAEXCHANGEBUFFER                                    0xA0
#define CMD_REQUEST_TORN_BUFFER1                                          0xA1
#define CMD_REQUEST_STORES_ONLINE_TRANSACTION_CONTEXT                     0xA2
#define CMD_REQUEST_GET_ONLINE_TRANSACTION_CONTEXT                        0xA3
#define CMD_REQUEST_STORES_RECOVERY_CONTEXT                               0xA4
#define CMD_REQUEST_GET_RECOVERY_CONTEXT                                  0xA5
#define CMD_REQUEST_STORES_DELAY_AUTH_OUTCOME                             0xA6
#define CMD_AMEX_GET_Dynamic_Limit_Param                                  0xA7
#define CMD_CTL_NOTIFY_AFTER_READ_RECORD								  0xA8
#define CMD_CTL_NOTIFY_AFTER_SELECT_APP								      0xA9

#define CMD_EMV_CALLBACK_EXCHANGE_APDU                                    0xB0
#define CMD_EMV_CALLBACK_DISPLAY_UIRD_MSG                                 0xB1
#define CMD_EMV_CALLBACK_GET_AID_PARAM                                    0xB2
#define CMD_EMV_CALLBACK_GET_PUBLIC_KEY                                   0xB3
#define CMD_EMV_CALLBACK_GET_RANDOM_NUMBER                                0xB4
#define CMD_EMV_CALLBACK_GET_ANY_LENGTH_RANDOM_NUMBER                     0xB5
#define CMD_EMV_CALLBACK_GET_TOTAL_AMOUNT                                 0xB6
#define CMD_EMV_CALLBACK_CHECK_EXCEPTION_FILE                             0xB7
#define CMD_EMV_CALLBACK_CHECK_IPK_REVOKE                                 0xB8
#define CMD_EMV_CALLBACK_CARDHOLDER_CONFIRMATION                          0xB9
#define CMD_EMV_CALLBACK_CVM_OFFLINE_PIN                                  0xBA
#define CMD_EMV_CALLBACK_CVM_ONLINE_PIN                                   0xBB
#define CMD_EMV_CALLBACK_ONLINE_REQUEST                                   0xBC
#define CMD_EMV_CALLBACK_ISSUER_REFERAL                                   0xBD
#define CMD_EMV_CALLBACK_PBOC_AUTHENTICATION                              0xBE
#define CMD_EMV_CALLBACK_TRANS_FORCE_ACCEPT                               0xBF

#define CMD_EMV_CALLBACK_GET_PAN_BEFORE_ODA_CUSTOMER					  0xFB
#define CMD_EMV_CALLBACK_NOTIFY_AFTER_READ_RECORD   					  0xFC
#define CMD_EMV_CALLBACK_NOTIFY_AFTER_SELECT_APP                          0xFD

#define CMD_ICC_REQUEST_AID_LIST										  0xC1
#define CMD_READER_MAG_DATA												  0xC2
#define CMD_DISPLAY_ICC_MSG												  0xC3
#define CMD_NFC_ONLINE_REQUEST											  0xC4

#define CMD_NOTHING_TO_DO                                                 0xD0
#define CMD_REQUEST_ICS                                                   0xD1
#define CMD_REQUEST_AID_LIST                                              0xD2
#define CMD_REQUEST_AID_PARAM                                             0xD3
#define CMD_REQUEST_PKI                                                   0xD4
#define CMD_REQUEST_UNPREDICATABLE_NUMBER                                 0xD5
#define CMD_REQUEST_POWER_ON                                              0xD6
#define CMD_DISPLAY_UIRD_MSG                                              0xD7
#define CMD_REQUEST_PDOL_DATA                                             0xD8
#define CMD_TRANSACTION_APPROVED                                          0xD9
#define CMD_TRANSACTION_ONLINE_REQUEST                                    0xDA
#define CMD_TRANSACTION_ADVICE                                            0xDB
#define CMD_TRANSACTION_DECLINED                                          0xDC
#define CMD_TRANSACTION_TERMINATE                                         0xDD
#define CMD_TRANSACTION_ITEM_REQUEST                                      0xDE
#define CMD_TRANSACTION_FIELD_OFF                                         0xDF

#define CMD_CARDHOLDER_CONFIRMATION                                       0xE1
#define CMD_CVM_OFFLINE_PIN                                               0xE2
#define CMD_CVM_ONLINE_PIN                                                0xE3
#define CMD_GET_TXN_LOG                                                   0xE4
#define CMD_GET_TOP_UP_LOG                                                0xE5
#define CMD_REQUEST_EXCEPTION_FILE                                        0xE6
#define CMD_GET_BALANCE                                                   0xE7
#define CMD_LED_BLINK                                                     0xE8
#define CMD_REQUEST_PKI_IPK_REVO                                          0xE9
#define CMD_TRANSMIT_DEK                                                  0xEA

#define CMD_TRANSMIT_APDU_KERNEL                                          0xF1
#define CMD_MIR_DATA_EXCHANGE                                             0xF2
#define CMD_TIMER_START                                                   0xF3
#define CMD_TIMER_STOP                                                    0xF4
#define CMD_REQUEST_TORN_BUFFER                                           0xF5
#define CMD_WAIT_BY_APP_MS                                                0xF7
#define CMD_CCC_NO_RES                                                    0xF8
#define CMD_SAVE_RRP_TIME                                                 0xF9
#define CMD_TRANSMIT_TORN                                                 0xFA

#define CMD_OUT_SIGNAL_ENTRYPOINT                                         0xFE
#define CMD_PRINT_MSG_DEBUG                                               0xFF

/*PayPass Error Indication*/
#define PAYPASS_ERROR_INDICATION_L1_OK                                    0x00
#define PAYPASS_ERROR_INDICATION_L1_TIME_OUT_ERROR                        0x01
#define PAYPASS_ERROR_INDICATION_L1_TRANSMISSION_ERROR                    0x02
#define PAYPASS_ERROR_INDICATION_L1_PROTOCOL_ERROR                        0x03

#define PAYPASS_ERROR_INDICATION_L2_OK                                    0x00
#define PAYPASS_ERROR_INDICATION_L2_CARD_DATA_MISSING                     0x01
#define PAYPASS_ERROR_INDICATION_L2_CAM_FAILED                            0x02
#define PAYPASS_ERROR_INDICATION_L2_STATUS_BYTES                          0x03
#define PAYPASS_ERROR_INDICATION_L2_PARSING_ERROR                         0x04
#define PAYPASS_ERROR_INDICATION_L2_MAX_LIMIT_EXCEEDED                    0x05
#define PAYPASS_ERROR_INDICATION_L2_CARD_DATA_ERROR                       0x06
#define PAYPASS_ERROR_INDICATION_L2_MAGSTRIPE_NOT_SUPPORTED               0x07
#define PAYPASS_ERROR_INDICATION_L2_NO_PPSE                               0x08
#define PAYPASS_ERROR_INDICATION_L2_PPSE_FAULT                            0x09
#define PAYPASS_ERROR_INDICATION_L2_EMPTY_CANDIDATE_LIST                  0x0A
#define PAYPASS_ERROR_INDICATION_L2_IDS_READ_ERROR                        0x0B
#define PAYPASS_ERROR_INDICATION_L2_IDS_WRITE_ERROR                       0x0C
#define PAYPASS_ERROR_INDICATION_L2_IDS_DATA_ERROR                        0x0D
#define PAYPASS_ERROR_INDICATION_L2_IDS_NO_MATCHING_AC                    0x0E
#define PAYPASS_ERROR_INDICATION_L2_TERMINAL_DATA_ERROR                   0x0F

#define PAYPASS_ERROR_INDICATION_L3_OK                                    0x00
#define PAYPASS_ERROR_INDICATION_L3_TIME_OUT                              0x01
#define PAYPASS_ERROR_INDICATION_L3_STOP                                  0x02
#define PAYPASS_ERROR_INDICATION_L3_AMOUNT_NOT_PRESENT                    0x03

/*EMV Transaction Results*/
#define EMV_TRANS_RESULT_OFFLINE_APPROVED                                 0x00
#define EMV_TRANS_RESULT_OFFLINE_DECLINED                                 0x01
#define EMV_TRANS_RESULT_ONLINE_APPROVED                                  0x02
#define EMV_TRANS_RESULT_ONLINE_DECLINED                                  0x03
#define EMV_TRANS_RESULT_UNABLEONLINE_OFFLINE_APPROVED                    0x04
#define EMV_TRANS_RESULT_UNABLEONLINE_OFFLINE_DECLINED                    0x05
#define EMV_TRANS_RESULT_TRY_ANOTHER_INTERFACE                            0x06
#define EMV_TRANS_RESULT_TERMINATE                                        0x07
#define EMV_TRANS_RESULT_NOT_ALLOWED                                      0x08
#define EMV_TRANS_RESULT_NOT_ACCEPTED                                     0x09

/*BCTC Transaction Results (tag 03)*/
//0x01~0x05: emvL2, PBOC
#define BCTC_TRANS_APPROVED                                               0x01
#define BCTC_TRANS_DECLINED                                               0x02
#define BCTC_TRANS_TERMINATE                                              0x03
#define BCTC_TRANS_TERMINATE_SERVICE_NOT_SUPPORT                          0x04
#define BCTC_TRANS_FORCE_APPROVED                                         0x05
//0x11~0x16: qPBOC, qUICS
#define BCTC_TRANS_Q_OFFLINE_APPROVED                                     0x11
#define BCTC_TRANS_Q_OFFLINE_DECLINED                                     0x12
#define BCTC_TRANS_Q_TERMINATE											  0x13
#define BCTC_TRANS_Q_ONLINE_APPROVED  									  0x14
#define BCTC_TRANS_Q_ONLINE_DECLINED  									  0x15
#define BCTC_TRANS_Q_ONLINE_FAILED_OFFLINE_DECLINED  					  0x16
#define BCTC_TRANS_Q_DECLINED_BLACK_LIST_CARD 							  0x17
#define BCTC_TRANS_Q_ONLINE_EXPIRED_CARD 								  0x18
#define BCTC_TRANS_Q_DECLINED_EXPIRED_CARD 								  0x19
//0x4x: ExpressPay
#define BCTC_TRANS_AMEX_APPROVED										  0x41
#define BCTC_TRANS_AMEX_APPROVED_WITH_SIGNATURE						      0x42
#define BCTC_TRANS_AMEX_DECLINED										  0x43
#define BCTC_TRANS_AMEX_TRY_ANOTHER_INTERFACE    						  0x44
#define BCTC_TRANS_AMEX_PRE_TERMINATE_OR_ANOTHER_PAYMENT 				  0x45
//0x6x: Discover Dpas-Connect
#define BCTC_TRANS_DPAS_OFFLINE_APPROVED                                  0x61
#define BCTC_TRANS_OFFLINE_DECLINED                                       0x62
#define BCTC_TRANS_DPAS_ONLINE_APPROVED_DPAS                              0x63
#define BCTC_TRANS_DPAS_ONLINE_DECLINED                                   0x64
#define BCTC_TRANS_DPAS_APPROVED_PLS_SIGN                                 0x65
#define BCTC_TRANS_DPAS_PLS_USE_ANOTHER_CARD                              0x66
#define BCTC_TRANS_DPAS_PLS_ENTER_PIN                                     0x67
#define BCTC_TRANS_DPAS_PLS_PRESENT_CARD                                  0x68
#define BCTC_TRANS_DPAS_IINSERT_SWIPE_CARD 								  0x69
#define BCTC_TRANS_DPAS_TERMINATE_DPAS 									  0x6A


/*OTHER*/
#define JCB_SPEEDY_TRANSACTION_UndefinedMode                              0x00
#define JCB_SPEEDY_TRANSACTION_EMV_Mode                                   0x01
#define JCB_SPEEDY_TRANSACTION_MagStripe                                  0x02
#define JCB_SPEEDY_TRANSACTION_Legacy_Mode                                0x04

#define LOG_TYPE_T_APDU                                                   0x00
#define LOG_TYPE_C_APDU                                                   0x01
#define LOG_TYPE_DEK                                                      0x02
#define LOG_TYPE_DET                                                      0x03

/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/
typedef struct
{
	unsigned char	signal_type;
	unsigned char*	syn_data;
	unsigned int	syn_data_len;
}Signal_Message,*pSingal_Message;

typedef struct _cvm_results_t
{
	unsigned char	ucPerformed;
	unsigned char	ucCondition;
	unsigned char	ucResult;
}cvm_results_t, * p_cvm_results_t;

typedef struct _outcome_parameter_set_t
{
	unsigned char	ucStatus;
	unsigned char	ucStart;
	unsigned char	ucOnlineResponseData;
	unsigned char	ucCVM;
	unsigned char	ucPresence;
	unsigned char	ucAlternateInterfacePreference;
	unsigned char	ucFieldOffRequest;
	unsigned char	ucRemovalTimeout;
}outcome_parameter_set_t, * p_outcome_parameter_set_t;

typedef struct _user_interface_request_data_t
{
	unsigned char	ucMessageIdentifier;
	unsigned char	ucStatus;
	unsigned char	ucHoldTime[3];
	unsigned char	ucLanguagePreference[8];
	unsigned char	ucValueQualifier;
	unsigned char	ucValue[6];
	unsigned char	ucCurrencyCode[2];
}user_interface_request_data_t, * p_user_interface_request_data_t;

typedef struct _error_indication_t
{
	unsigned char	ucL1;
	unsigned char	ucL2;
	unsigned char	ucL3;
	unsigned char	ucSW12[2];
	unsigned char	ucMsgOnError;
}error_indication_t, * p_error_indication_t;



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/

/*
*******************************************************************************
*   End of File
*******************************************************************************
*/

#undef GLOBAL

#endif  /* #ifndef _OUTCOME_LIST_H */

