/**********************************************************************************************************
 * Copyright(C)2018, 
 * File Name     : EmvInterface.h
 * Version       : v1.0
 * Author        :
 * Description   : External Apis of EMV kernel
 **********************************************************************************************************/

#ifndef __EMV_INTERFACE_H__
#define __EMV_INTERFACE_H__

#include "EmvType.h"
#include "EmvError.h"
#include "TLV.h"
//#include "EmvData.h"
//#include "char_convert.h"
#include "LibEmvKernel.h"

// Kernel transaction card channel
// Standard process
#define KERNEL_PROC_STANDRAD        0
// Simple process
#define KERNEL_PROC_SIMPLE          1


// Kernel transaction card type
// Contact card
#define KERNEL_CARD_CONTACT         0
// contactless card
#define KERNEL_CARD_CONTACTLESS     1

// ICC LOG Maximum number
#define ICC_LOG_NUM_MAX 10

// single card transaction log
typedef struct
{
    unsigned int uiLen;      // Log length
    unsigned char *pucvalue; // TLV format log data
} CARD_LOG;

// Card transaction log, get card transaction log output parameters
typedef struct
{
    unsigned int uiNum;                    // Number of logs, maximum ICC_LOG_NUM_MAX (10)
    CARD_LOG cardLogList[ICC_LOG_NUM_MAX]; // Card log
} ST_EMV_LOG;

// Card e-cash balance, obtain card electronic cash balance output parameters
typedef struct
{
    unsigned char aucCurrencyCode1[2]; // First e-cash currency code
    unsigned char aucEcBalance1[6];    // First e-cash balance
    unsigned char aucCurrencyCode2[2]; // Second e-cash currency code
    unsigned char aucEcBalance2[6];    // Second e-cash balance
} ST_EMV_EC;

// Kernel callback API
extern ST_KERNEL_CALLBACK *g_pKernelCallback;
//------------------------------------------------------------------------------
// Function     : EmvGetVersion
// Description  : Get kernel version
// Param IN     : pucVersion        - Kernel version, ending with '\ 0'
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Modify 1     :
//------------------------------------------------------------------------------
void EmvGetVersion(char *pucVersion);

//------------------------------------------------------------------------------
// Function     : EmvKernelInit
// Description  : kernel initialization
// Param IN     :
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
// Modify 1     :
//------------------------------------------------------------------------------
ERR EmvKernelInit(void);

//------------------------------------------------------------------------------
// Function     : EmvProcess
// Description  : Start kernel processing, kernel process main process
// Param IN     : pstEmvTrans       - Transaction parameters
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvProcess(const ST_EMV_TRANS *pstEmvTrans);

//------------------------------------------------------------------------------
// Function     : EmvAfterOnlineProcess
// Description  : Kernel processing flow after online process
// Param IN     : pucData           - Background response data
//              : ulLen             - Response data length
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvAfterOnlineProcess(uint8 *pucData, uint32 ulLen);

//------------------------------------------------------------------------------
// Function     : EmvGetKernelProcessResult
// Description  : get kernel processing results
// Param IN     :
// Param OUT    : pstResult         -Kernel processing result
// Param IN/OUT :
// Return       :
// Other        : Must be called after calling EmvProcess() or EmvAfterOnlineProcess()
// Create       :
//------------------------------------------------------------------------------
ERR EmvGetKernelProcessResult(ST_KERNEL_RESULT *pstResult);

//------------------------------------------------------------------------------
// Function     : EMVGetCardLog
// Description  : get card transaction logs
// Param IN     : ucChannelType     - transaction channel 0- Contact, 1- Contactless
// Param OUT    : pstEmvLog         - Card log
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EMVGetCardLog(unsigned char ucChannelType, ST_EMV_LOG *pstEmvLog);

//------------------------------------------------------------------------------
// Function     : EmvGetEcBalance
// Description  : Query card e-cash balance
// Param IN     : ucChannelType     - transaction channel 0- Contact, 1- Contactless
// Param OUT    : pstEmvLog         - Card e-cash balance
// Param IN/OUT :
// Return       :
// Other        : The e-cash currency code is 0000, indicating that the electronic cash does not exist
// Create       :
//------------------------------------------------------------------------------
ERR EmvGetEcBalance(unsigned char ucChannelType, ST_EMV_EC *pstEmvEc);

//------------------------------------------------------------------------------
// Function     : EmvGetTlv
// Description  : get TAG values from the kernel based on TAG
// Param IN     : uiTag             - TAG
//              : uiSize            - Buffer  size
// Param OUT    : puiLenth          - TAG value length
//              : pucValue          - TAG value
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvGetTlv(unsigned int uiTag, unsigned char *pucValue, unsigned int uiSize, unsigned int *puiLenth);

//------------------------------------------------------------------------------
// Function     : EmvSetTlv
// Description  : According to TAG, set the TAG value to the kernel
// Param IN     : uiTag             - TAG
//              : uiLenth           - TAG value length
//              : pucValue          - TAG value
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvSetTlv(unsigned int uiTag, unsigned char *pucValue, unsigned int uiLenth);

//------------------------------------------------------------------------------
// Function     : EmvFindTlv
// Description  : Find TLV from kernel
// Param IN     : uiTag             - TAG to be searched
// Param OUT    : pTlv              - Found TLV
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvFindTlv(unsigned int uiTag, TLV *pTlv);

//------------------------------------------------------------------------------
// Function     : EmvAddTlv
// Description  : Write TLV to the kernel
// Param IN     : pTlv              - TLV to be written
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvAddTlv(const TLV *pTlv);

//------------------------------------------------------------------------------
// Function     : EmvAddTlvList
// Description  : Write TLV LIST to the kernel
// Param IN     : pSubList              - TLV LIST to be written
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvAddTlvList(const TLV_LIST *pSubList);

//------------------------------------------------------------------------------
// Function     : EmvAddTlvListData
// Description  : Write TLV LIST data to the kernel
// Param IN     : pData             - TLV LIST data to be written
//              : ulLen             - data Length
// Param OUT    :
// Param IN/OUT :
// Return       :
// Other        :
// Create       :
//------------------------------------------------------------------------------
ERR EmvAddTlvListData(uint8 *pData, uint32 ulLen);
#endif
