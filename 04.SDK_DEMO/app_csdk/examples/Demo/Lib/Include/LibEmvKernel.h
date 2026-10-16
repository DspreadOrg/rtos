//-----------------------------------------------------------------------------
// @file    : LibEmvKernel.h
// @brief   : EMV Kernel functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_EMVKERNEL_H__
#define __LIB_EMVKERNEL_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// User Interface Request Data DF8116
typedef struct
{
    unsigned char ucIdentify;       // Message Identify
    unsigned char ucStatus;         // Status
    unsigned char aucHoldTime[3];   // Hold time  n6
    unsigned char aucLanguage[8];   // The First Language an 5F2D
    unsigned char ucQualifier;      // Amount Qualify
    unsigned char aucValue[6];      // Amount n12
    unsigned char aucCurrency[2];   // Currency Code n3
} ST_UIRD;

//-----------------------------------------------------------------------------
//EMV API Define
//-----------------------------------------------------------------------------

// EMV kernel callback function
typedef struct
{
    /**
      * @method	pOnEmvGetTerminalCapk
      * @brief	Get terminal CA public key parameter callback method
      * @param	pucRid		    :RID, form TAG 4F
      * @param	ucCapkIndex		:Certification authority public key index, TAG 8F
      * @param	pucCapk		    :output parameter, CA public key buffer
      * @param	pusCapkLen  	:output parameter, the length of returned  CA public key
      * @param	pucExp		    :output parameter, CA public key exponent buffer
      * @param	pucExpLen     	:output parameter, the length of returned  CA public key exponent
      * @retval					:0 -success, -1 -there is no matching CA public key
      */
    int (*pOnEmvGetTerminalCapk)(const unsigned char pucRid[5], unsigned char ucCapkIndex, unsigned char *pucCapk, unsigned short *pusCapkLen, unsigned char *pucExp, unsigned char *pucExpLen);

    /**
      * @method	pOnEmvGetTerminalAID
      * @brief	Get terminal AID parameter callback method
      * @param	uiAidIndex		:AID parameter index, start at 0
      * @param	pucTerAid		:output parameter, AID parameter buffer
      * @param	uiSize		    :AID parameter buffer size
      * @param	puiTerAidLen	:output parameter, the length of returned AID parameter
      * @retval					:0 -success, -1 -there is no AID parameter
      */
    int (*pOnEmvGetTerminalAID)(unsigned int uiAidIndex, unsigned char *pucTerAid, unsigned int uiSize, unsigned int *puiTerAidLen);

    /**
      * @method	pOnEmvCertRevolutionCheck
      * @brief	CA certificate revolution check callback method
      * @param	aucRID  		:RID, form TAG 4F
      * @param	ucCaIndex		:Certification authority public key index, TAG 8F
      * @param	aucCertSerial	:Certification authority public key serial number
      * @retval					:0 -not in the revolution list, -1 -in the revolution list
      */
    int (*pOnEmvCertRevolutionCheck)(const unsigned char aucRID[5], unsigned char ucCaIndex, const unsigned char aucCertSerial[3]);

    /**
      * @method	pOnEmvExceptionFileCheck
      * @brief	Application PAN blacklist check callback method
      * @param	pucPanBcd		:Application PAN, from TAG 5A, BCD format, Fill FF to 10 bytes at the end
      * @param	ucPanSerial		:Application PAN sequence number, TAG 5F34, n2
      * @retval					:0 -not in the blacklist, -1 -in the blacklist
      */
    int (*pOnEmvExceptionFileCheck)(const unsigned char pucPanBcd[10], unsigned char ucPanSerial);

    /**
      * @method	pOnEmvDispMsg
      * @brief	the user interface callback method
      * @param	ucIdentify		:user interface message identifier, refer to chapter 9 of EMV book A
      * @param	ucStatus		:user interface status, refer to chapter 9 of EMV book A
      * @retval					:0 -success, <0 -error code
      */
    int (*pOnEmvDispMsg)(ST_UIRD *ucIdentify, unsigned char ucStatus);

    /**
      * @method	pOnEmvAppConfirm
      * @brief	card application confirm callback method
      * @param	ucAppNum		:application total number
      * @param	pucAppData		:application name TLV list, the index start at 1, for example: 0107454d5641505031 0207454d5641505032
                                means there are 2 applications, the first name is "EMVAPP1", the second name is "EMVAPP2".
      * @param	uiDataLen		:pucAppData length
      * @param	pucIndex		:output parameter, the index of selected application
      * @retval					:0 -confirm success, -1029 -user cancel
      */
    int (*pOnEmvAppConfirm)(unsigned char ucAppNum, unsigned char *pucAppData, unsigned int uiDataLen, unsigned char *pucIndex);

    /**
      * @method	pOnEmvCardNoConfirm
      * @brief	confirm card number callback method
      * @param	pszPAN		    :string card number
      * @retval					:0 -success, -1029 -user cancel
      */
    int (*pOnEmvCardNoConfirm)(char *pszPAN);

    /**
      * @method	pOnEmvInputPIN
      * @brief	PIN input callback method
      * @param	ucPinType		:PIN type,  0 -offline pin, 1 -online pin
      * @param	pucPinBlock		:output 8 bytes PIN block
      * @param	ucLastTimes		:pin retry count
      * @retval					:0 -pin input success, -1013 -PIN bypass, -1029 -user cancel
      */
    int (*pOnEmvInputPIN)(const unsigned char ucPinType, unsigned char *pucPinBlock, const unsigned char ucLastTimes);

    // Whether to save transaction log
    unsigned char (*pOnEmvSaveLog)(void);
} ST_KERNEL_CALLBACK, *PST_KERNEL_CALLBACK;

// Transaction parameters, EMV kernel processing mainstream input parameters
typedef struct
{
    unsigned char procType;             // Process types 0- EMV standard, 1- EMV simple, 2- Quick standard, 3- Quick simple (currently the terminal only supports contact EMV, non-contact EMV reserved, Quick only supports non connected devices
    unsigned char channelType;          // Channel type 0- Contact type, 1-contact-less card
    unsigned char odaFlag;              // Offline data authentication identifier, only applicable to EMV standard mode;0- Do not perform offline data authentication (virtual), 1- Perform offline data authentication
    unsigned char ecFlag;               // Supports electronic cash transaction identification, only applicable to EMV standard mode; 0- does not support electronic cash transactions, 1- supports electronic cash transactions
    unsigned char forceOnline;          // Force online: 0- Not mandatory, 1- Force online
    unsigned char forceOnlinePin;       // Force online PIN, only applicable to EMV standard mode. 0- Not mandatory, 1- Force online PIN
    unsigned char cardNoConfirm;        // Card number confirmation, only applicable to EMV standard mode. 0- Do not perform card number confirmation, 1- Perform card number confirmation
    unsigned char restartFlag;          // Kernel restart flag. 0- First kernel boot, 1- Kernel restart

    unsigned char transType;            // Transaction type  n2  9C
    unsigned char currencyExp;          // Transaction currency index n1  5F36
    unsigned char merNameLen;           // Merchant name length  b 1B
    unsigned char merName[40];          // MERCHANT NAME ans Var.Maximum length unknown 9F4E
    unsigned char posSer[3];            // Transaction serial No n6  9F41
    unsigned char transAmt[6];          // Transaction amount n12  9F02
    unsigned char otherAmt[6];          // Other amount n12  9F03
    unsigned char countryCode[2];       // Terminal Country Code n3  9F1A
    unsigned char currencyCode[2];      // Transaction Currency Code n3  5F2A
    unsigned char transDate[3];         // Transaction Date n6  YYMMDD  9A
    unsigned char transTime[3];         // Transaction Time n6  hhmmss    9F21
    unsigned char merId[15];            // Merchant ID ans15  9F16
    unsigned char termId[8];            // Terminal ID an8  9F1C

    unsigned char additionLen;          // Additional data length
    unsigned char addition[158];        // Additional data exists in TLV format
}ST_EMV_TRANS; // 256B

#define SDK_KERNEL_TYPE_NULL                    0 // no match kernel
#define SDK_KERNEL_TYPE_EMV                     1 // EMV
#define SDK_KERNEL_TYPE_UNIONPAY_QUICK          2 // UnionPay Quick pass
#define SDK_KERNEL_TYPE_VISA                    3 // VISA
#define SDK_KERNEL_TYPE_MASTER                  4 // MASTER Card
#define SDK_KERNEL_TYPE_AMERICA_EXPRESS         6 // America Express
#define SDK_KERNEL_TYPE_DISCOVER                7 // DISCOVER
#define SDK_KERNEL_TYPE_JCB                     8 // Japan Credit Bureau
#define SDK_KERNEL_TYPE_PURE                    0x2D // PUREkernel,PURE specification is not defined, while MADA (Saudi specification) is defined as 2D
#define SDK_KERNEL_TYPE_MIR                     0x810643 // MIR (Russian) MIR Specification Contactless Terminal Kernel Specification 2.3
#define SDK_KERNEL_TYPE_RUPAY                   0x0D // RUPAY (India) RUPAY Specification RuPay Contactless Terminal Specification 2.0.1


#define SDK_EMV_STATUS_APPROVED                 0x10 // Transaction Approved
#define SDK_EMV_STATUS_DECLINED                 0x20 // Transaction Declined
#define SDK_EMV_STATUS_ONLINE_REQUEST           0x30 // Transaction Online Request
#define SDK_EMV_STATUS_END_APPLICATION          0x40 // end application
#define SDK_EMV_STATUS_SELECT_NEXT              0x50 // choose next
#define SDK_EMV_STATUS_TRY_ANOTHER_INTERFACE    0x60 // Try other interface
#define SDK_EMV_STATUS_TRY_AGAIN                0x70 // Try again
#define SDK_EMV_STATUS_NA                       0xF0 // N/A


// Add 2019.1.16  Add custom status
#define SDK_EMV_STATUS_TC_TO_ARQC               0x80 // After the card returning TC, it will be connected online
#define SDK_EMV_STATUS_TC_TO_AAC                0x90 // Card returned to TC and then rejected
#define SDK_EMV_STATUS_ARQC_ODA                 0xA0 // Online ODA

#define SDK_EMV_STATUS_NON_FINA_TRANS_COMPL     0xB0 // NON_FINANCIAL TRANSACTION COMPLETED_RETRIEVAL
#define SDK_EMV_STATUS_NON_FINA_TRANS_DATA      0xC0 // NON_FINANCIAL TRANSACTION COMPLETED_DATA
#define SDK_EMV_STATUS_COMM_ERR_PAY_NOT_COMP    0xD0 // COMM ERROR PAY NOT COMP
#define SDK_EMV_STATUS_END_SERV_NOT_SUPPORT     0x41 // End of application (service not allowed) add at 2022 10 25


#define SDK_EMV_START_A                         0x00 // startup code : A
#define SDK_EMV_START_B                         0x10 // startup code : B
#define SDK_EMV_START_C                         0x20 // startup code : C
#define SDK_EMV_START_D                         0x30 // startup code : D
#define SDK_EMV_START_NA                        0xF0 // startup code : N/A

#define SDK_EMV_ONLINE_NA                       0xF0 // Online response data : N/A


#define SDK_EMV_CVM_NO_CVM                      0x00 // No need CVM
#define SDK_EMV_CVM_SIGNATURE                   0x10 // signature
#define SDK_EMV_CVM_ONLINE_PIN                  0x20 // Online PIN
#define SDK_EMV_CVM_CONFIRM_CODE_VERIFIED       0x30 // Verification confirmation code
#define SDK_EMV_CVM_NA                          0xF0 // CVM:N/A

// EMV kernel process result
typedef struct
{
    unsigned char ucKernel;     // Kernel type
    unsigned char ucStatus;     // The EMV kernel status
    unsigned char ucStart;      // startup code, add at 2020 09 02 by rjp
    unsigned char ucCvm;        // The CVM method
    unsigned char ucCmd;        // The last APDU command to card
    unsigned char ucErrL1;      // L1
    unsigned char ucErrL2;      // L2
    unsigned char ucErrL3;      // L3
    unsigned char aucSw1Sw2[2]; // SW1SW2
    unsigned char ucIdentify;   // The message Identifier of UIRD
} ST_KERNEL_RESULT, *PST_KERNEL_RESULT;

//-----------------------------------------------------------------------------
// Name    : Lib_EmvKernelInit
// Func    : EMV Kernel Init
// Paras   : pCallback[in], input EMV kernel callback function
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvKernelInit(PST_KERNEL_CALLBACK pCallback);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvProcess
// Func    : Start EMV process
// Paras   : pstKernelTrans[in], input transaction parameters
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvProcess(const ST_EMV_TRANS *pstKernelTrans);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvAfterOnlineProcess
// Func    : Secondary card reading process after online
// Paras   : ucOnlineRes[in], input the online result, 1: succeeed, 0: fail
//           pucData[in],     input online response data, TLV list format
//           ulLen[in],       the length of online response data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvAfterOnlineProcess(unsigned char ucOnlineRes, unsigned char *pucData, unsigned int ulLen);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvGetKernelProcessResult
// Func    : Get EMV kernel process result
// Paras   : pstResult[in/out], output the result
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvGetKernelProcessResult(PST_KERNEL_RESULT pstResult);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvGetTlv
// Func    : get tag data from the EMV kernel
// Paras   : uiTag[in],    TLV-BER format tag
//           pucValue[in/out],  output the tag data 
//           uiSize[in],    input the data Buffer maximum size
//           puiLenth[in/out],   output the tag data length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvGetTlv(unsigned int uiTag, unsigned char *pucValue, unsigned int uiSize, unsigned int *puiLenth);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvSetTlv
// Func    : Set data to the EMV kernel
// Paras   : uiTag[in],    TLV-BER format tag
//           pucValue[in],  tag data 
//           uiLenth[in],   tag data length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvSetTlv(unsigned int uiTag, unsigned char *pucValue, unsigned int uiLenth);

// Card e-cash balance, obtain card electronic cash balance output parameters
typedef struct
{
    unsigned char aucCurrencyCode1[2]; // First e-cash currency code
    unsigned char aucEcBalance1[6];    // First e-cash balance
    unsigned char aucCurrencyCode2[2]; // Second e-cash currency code
    unsigned char aucEcBalance2[6];    // Second e-cash balance
} SDK_ST_EMV_EC;
//-----------------------------------------------------------------------------
// Name    : Lib_EmvGetEcBalance
// Func    : Get the balance of electronic cash
// Paras   : ucChannelType[in],   0: contact card,1:contactless card
//           pstEmvEc[in/out],    
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_EmvGetEcBalance(unsigned char ucChannelType, SDK_ST_EMV_EC *pstEmvEc);

//-----------------------------------------------------------------------------
// Name    : Lib_EmvGetLibVersion
// Func    : Get the version of EMV Kernel
// Paras   : paucEmvLibVer[in/out],    input data buffer and output the version data
//           pusLen[in/out],           output length of version data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
void Lib_EmvGetLibVersion(char *paucEmvLibVer, unsigned short *pusLen);

//-----------------------------------------------------------------------------
// Name    : Lib_EarseTerminalAIDandCapk
// Func    : Erase all AID or CAPK files
// Paras   : ucType[in],  input TERMINAL_AID or TERMINAL_CAPK
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_EarseTerminalAIDandCapk(unsigned char ucType);

//-----------------------------------------------------------------------------
// Name    : Lib_GetAIDandCapkTotal
// Func    : Get the total of AID or CAPK
// Paras   : ucType[in],  input TERMINAL_AID or TERMINAL_CAPK
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
unsigned char Lib_GetAIDandCapkTotal(unsigned char ucTypes);

//-----------------------------------------------------------------------------
// Name    : Lib_GetTerminalAID
// Func    : Get AID based on index
// Paras   : uiAidIndex[in],     input index
//           pucTerAid[in/out],  output AID
//           uiSize[in],         input data buffer length
//           puiTerAidLen[in],   output AID length
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_GetTerminalAID(unsigned int uiAidIndex, unsigned char *pucTerAid, unsigned int uiSize, unsigned int *puiTerAidLen);

//-----------------------------------------------------------------------------
// Name    : Lib_GetTerminalCapk
// Func    : Get CAPK based on index
// Paras   : pucRid[in],          input RID, form TAG 9F06
//           ucCapkIndex[in],     input Certification authority public key index, TAG 8F
//           pucCapk[in/out],     output CA public key
//           pusCapkLen[in/out],  output length of CA public key
//           pucExp[in/out],      output CA public key exponent
//           pucExpLen[in/out],   output length of CA public key exponent
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_GetTerminalCapk(const unsigned char pucRid[5], unsigned char ucCapkIndex, unsigned char *pucCapk, unsigned short *pusCapkLen, 
                         unsigned char *pucExp, unsigned char *pucExpLen);

//-----------------------------------------------------------------------------
// Name    : Lib_SetTerminalAIDAndCapk
// Func    : Set AID or CAPK
// Paras   : pData[in],    input AID or CAPK
//           DataLen[in],  input length
//           ucType[in],   input 0: TERMINAL_AID,1: TERMINAL_CAPK
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_SetTerminalAIDAndCapk(unsigned char *pData, unsigned short DataLen, unsigned char ucType);
       
#endif