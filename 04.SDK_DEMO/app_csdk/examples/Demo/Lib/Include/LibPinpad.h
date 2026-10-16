//-----------------------------------------------------------------------------
// @file    : LibPinPad.h
// @brief   : PinPad-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_PINPAD_H__
#define __LIB_PINPAD_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef struct
{
    unsigned char timeout;              // Input timeout, in seconds
    unsigned char min_input_count;      // Minimum input length
    unsigned char max_input_count;      // Maximum input length
    unsigned char is_bypass;            // Is bypass allowed
} pin_param;

typedef void (*pinpad_callback_t)(unsigned char input_count, const unsigned char ucLastTimes);

//-----------------------------------------------------------------------------
// Name    : Lib_PciGetMac
// Func    : Calculate MAC
// Paras   : ucMacKeyId[in],  input key ID 
//           usInLen[in],     input data length
//           pucInData[in],   input data buffer
//           pucMacOut[in],   output MAC
//           ucMode[in],      input calculation mode
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------                   
int Lib_PciGetMac(unsigned char ucMacKeyId, unsigned short usInLen, unsigned char *pucInData, unsigned char *pucMacOut, 
                   unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_PciAlgDes
// Func    : Des algorithm operation
// Paras   : ucKeyId[in],        input key ID
//           pucIv[in],          input initialization vector.It is necessary in CBC 
//                               mode.In ECB mode,it can transmit 0.
//           pucInData[in],      input data buffer
//           usInLen[int],       input data length
//           pucOutData[in/out], output result
//           ucMode[in],         input mode , if it is DES,encryption,ECB, the mode 
//                             should be (DES_SINGLE_KEY | ENCRYPTION | MODE_ECB)
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PciAlgDes(unsigned char ucKeyId, unsigned char* pucIv, unsigned char *pucInData, unsigned short usInLen, 
                   unsigned char *pucOutData, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_PciAlgAes
// Func    : Aes algorithm operation
// Paras   : ucKeyId[in],        input key ID
//           pucIv[in],          input initialization vector.It is necessary in CBC 
//                               mode.In ECB mode,it can transmit 0.
//           pucInData[in],      input data buffer
//           usInLen[int],       input data length
//           pucOutData[in/out], output result
//           ucMode[in],         input mode , if it is AES192,encryption,ECB, the mode 
//                               should be (AES_KEYLEN_192 | ENCRYPTION | MODE_ECB)
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------                   
int Lib_PciAlgAes(unsigned char ucKeyId, unsigned char* pucIv, unsigned char *pucInData, unsigned short usInLen, 
                   unsigned char *pucOutData, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_PciWriteKey
// Func    : Inject work key
// Paras   : ucKeyId[in],       input key ID
//           ucKeyLen[int],     input key length
//           pucKey[in],        input key
//           ucMKeyId[in],      input Master Key ID
//           pucCheckValue[in], input check value
//           ucCVLen[in],       input check vaule length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------                    
int Lib_PciWriteKey(unsigned char ucKeyId, unsigned char ucKeyLen, unsigned char *pucKey, unsigned char ucMKeyId, 
                   unsigned char* pucCheckValue, unsigned char ucCVLen);

//-----------------------------------------------------------------------------
// Name    : Lib_PciGetPin
// Func    : Get PinBlock
// Paras   : ucPinKeyId[in],      input key ID
//           pucCardNo[int],      input card number
//           ucMode[in],          input mode
//           pucPinBlock[in/out], output PinBlock
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------    
int Lib_PciGetPin(unsigned char ucPinKeyId,  unsigned char *pucCardNo, unsigned char ucMode, unsigned char *pucPinBlock);

//-----------------------------------------------------------------------------
// Name    : Lib_PciSetPinpadUI
// Func    : Set password input UI callback
// Paras   : cb[in],            UI callback
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void Lib_PciSetPinpadUI(pinpad_callback_t cb);

//-----------------------------------------------------------------------------
// Name    : Lib_PciOfflinePinpad
// Func    : Offline password input
// Paras   : param[in],         Password keyboard parameters
//           ucLastTimes[in],   The number of attempts left to enter the password
//           pucPinBlock[out],  output PinBlock
// Ret     : ERR_OK, ERR_USER_CANCEL, ERR_CVM_PIN_BYPASS
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PciOfflinePinpad(pin_param param, const unsigned char ucLastTimes, unsigned char *pucPinBlock);

//-----------------------------------------------------------------------------
// Name    : Lib_PciOnlinePinpad
// Func    : Online password input
// Paras   : param[in],         Password keyboard parameters
//           ucLastTimes[in],   The number of attempts left to enter the password
// Ret     : ERR_OK, ERR_USER_CANCEL, ERR_CVM_PIN_BYPASS
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PciOnlinePinpad(pin_param param, const unsigned char ucLastTimes);

#endif