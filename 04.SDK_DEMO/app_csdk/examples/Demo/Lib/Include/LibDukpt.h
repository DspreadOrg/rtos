//-----------------------------------------------------------------------------
// @file    : LibDukpt.h
// @brief   : DUKPT-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_DUKPT_H__
#define	__LIB_DUKPT_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name    : Lib_SetAlgorithmMode
// Func    : Set key algorithm type
// Paras   : algMode[in],  input 0 = 3DES; 1 = Chinese National Security Algorithm; 
//                               2 = DUKPT algorithm
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_SetAlgorithmMode(unsigned char algMode);

//-----------------------------------------------------------------------------
// Name    : Lib_UpdateTransferKey
// Func    : Set transmission key
// Paras   : transferKeyIndex[in],  input transfer key index
//           pucTransKey[in],       input transfer key
//           pucTransKey[in],       input transfer key legnth
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_UpdateTransferKey(unsigned char transferKeyIndex, unsigned char *pucTransKey, unsigned short uTransKeyLen);

//-----------------------------------------------------------------------------
// Name    : Lib_SetCipherMode
// Func    : Set ECB or CBC mode
// Paras   : cipherMode[in],  input 0: ECB mode,1: CBC mode
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_SetCipherMode(unsigned char cipherMode);

//-----------------------------------------------------------------------------
// Name    : Lib_DukptKeyInject
// Func    : Inject BDK or IPEK and KSN
// Paras   : tranfserKeyIndex[in],  input transfer key index
//           ksnIndex[in],          input KSN index
//           pucKsn[in],            input KSN
//           keyType[in],           input key type
//           puckeyData[in],        input BDK or IPEK
//           uKeyDataLen[in],       input key length
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_DukptKeyInject(unsigned char tranfserKeyIndex,unsigned char ksnIndex,unsigned char *pucKsn, 
                                   unsigned char keyType,unsigned char *puckeyData, unsigned short uKeyDataLen);

//-----------------------------------------------------------------------------
// Name    : Lib_SetCipherInitializationVector
// Func    : Set initial vector
// Paras   : puciv[in],  input initial vector in CBC mode.
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_SetCipherInitializationVector(unsigned char * puciv);

//-----------------------------------------------------------------------------
// Name    : Lib_DukptKsnIncrease
// Func    : increase KSN
// Paras   : mKeyIdx[in],  input Master key index
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_DukptKsnIncrease (unsigned char mKeyIdx);

//-----------------------------------------------------------------------------
// Name    : Lib_DukptCurrenKsn
// Func    : Get current KSN
// Paras   : mKeyIdx[in],     input Master key index
//           pucKsn[in/out],  output KSN
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_DukptCurrenKsn(unsigned char mKeyIdx, unsigned char *pucKsn);

//-----------------------------------------------------------------------------
// Name    : Lib_DukptEncryptOrDecrypt
// Func    : Encrypt or decrypt with DUKPT
// Paras   : keyGroudId[in],  input key group index
//           keyId[in],       input key index
//           calMode[in],     intput calculation mode
//           data[in],        intput data
//           dataLen[in],     intput data length
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_DukptEncryptOrDecrypt(unsigned char keyGroudId,unsigned char keyId, unsigned char calMode, 
                                         unsigned char *data,unsigned short *dataLen);

#endif