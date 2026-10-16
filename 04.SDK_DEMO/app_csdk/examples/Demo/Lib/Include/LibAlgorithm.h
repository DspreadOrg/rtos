//-----------------------------------------------------------------------------
// @file    : LibAlgorithm.h
// @brief   : Various security algorithm functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_ALGORITHM_H__
#define __LIB_ALGORITHM_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// Encryption parameter definition
#define DES_SINGLE_KEY		0x00   /* 64 bits */
#define DES_DOUBLE_KEY		0x01   /* 128 bits */
#define DES_TRIPLE_KEY		0x02   /* 192 bits */

#define ENCRYPTION		0x00
#define DECRYPTION		0x04

#define MODE_ECB		0x00
#define MODE_CBC		0x08

#define AES_KEYLEN_128   0x00    /* 128 bits */
#define AES_KEYLEN_192   0x01    /* 192 bits */
#define AES_KEYLEN_256   0x02    /* 256 bits */
//-----------------------------------------------------------------------------
// Name    : Lib_AlgDes
// Func    : Des algorithm operation
// Paras   : pucVector[in],   input initialization vector.It is necessary in CBC 
//                            mode.In ECB mode,it can transmit 0.
//           pucKey[in],      input key
//           pucData[in/out], output the calculation result
//           dwSize[in],      input data length
//           ucMode[in],      input mode , if it is DES,encryption,ECB, the mode 
//                             should be (DES_SINGLE_KEY | ENCRYPTION | MODE_ECB)
// Ret     : 1 - success, 0 - fail
// Desc    : 
//-----------------------------------------------------------------------------
unsigned char Lib_AlgDes(unsigned char *pucVector, unsigned char *pucKey, unsigned char *pucData, 
                          unsigned long dwSize, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgAes
// Func    : AES algorithm operation
// Paras   : pucVector[in],   input initialization vector.It is necessary in CBC 
//                            mode.In ECB mode,it can transmit 0.
//           pucKey[in],      input key
//           pucData[in/out], input data buffer and output result
//           dwSize[in],      input data length
//           ucMode[in],      input mode , if it is AES192,encryption,ECB, the mode 
//                             should be (AES_KEYLEN_192 | ENCRYPTION | MODE_ECB)
// Ret     : 1 - success, 0 - fail
// Desc    :
//-----------------------------------------------------------------------------
unsigned char Lib_AlgAes(unsigned char *pucVector, unsigned char *pucKey, unsigned char *pucData, 
                          unsigned long dwSize, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgAes256gcmEncrypt
// Func    : AES256 GCM Encryption algorithm operation
// Paras   : key256[in],      input 256-bit key(32 bytes)
//           data_length[in], input data length
//           iv[in],          input initialization vector.recommned 12 bytes(96bits),
//                            must be unique for each encryption with the same key.
//           iv_len[in],      input length of initialization vector.(usually 12)
//           aad[in],         input additional authenticated data.It can transmit 0.
//                            This data isn't encrypted but it is authenticated,
//                            integrity is verified during decryption.
//           aad_len[in],     input length of aad in bytes,transmit 0 if aad is 0. 
//           input[in],       input plaintext data(original data to be encrypted)
//           output[out],     output ciphertext data,must be at least data_length bytes
//                             (ciphertext length equals plaintext length)
//           tag_len[in],     input desired authentication tag length in bytes,typically
//                            16(128bits),also 12 or 8 allowed.
//           tag[in],         output authentication tag data,at least tag_len bytes;
//                            The tag is written here after encryption
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgAes256gcmEncrypt(const unsigned char *key256, unsigned int data_length,
                       const unsigned char *iv, unsigned int iv_len, const unsigned char *aad,
                       unsigned int aad_len, const unsigned char *input, unsigned char *output,
                       unsigned int tag_len, unsigned char *tag);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgAes256gcmDecrypt
// Func    : AES256 GCM Decryption algorithm operation
// Paras   : key256[in],      input 256-bit key(32 bytes)
//           data_length[in], input data length
//           iv[in],          input initialization vector.recommned 12 bytes(96bits),
//                            must be unique for each encryption with the same key.
//           iv_len[in],      input length of initialization vector.(usually 12)
//           aad[in],         input additional authenticated data.It can transmit 0.
//                            This data isn't encrypted but it is authenticated,
//                            integrity is verified during decryption.
//           aad_len[in],     input length of aad in bytes,transmit 0 if aad is 0. 
//           tag[in],         input authentication tag(generated by the encryptor and 
//                            transmitted),used to verify data integrity.
//                            The tag is written here after encryption.
//           tag_len[in],     input tag length in bytes,must match the length used 
//                            during encryption
//           input[in],       input ciphertext data(data to be decrypted)
//           output[out],     output plaintext data at least data_length bytes; 
//                            the plaintext is written here after decryption
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgAes256gcmDecrypt(const unsigned char *key256, unsigned int data_length,
                      const unsigned char *iv, unsigned int iv_len, const unsigned char *aad,
                      unsigned int aad_len, const unsigned char *tag, unsigned int tag_len,
                      const unsigned char *input, unsigned char *output);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgHash
// Func    : Output a 20 byte Sha1 hash result for input data of any length
// Paras   : pucDataIn[in],   Input data buffer pointer
//           ulDataInLen[in], Input data length (in bytes)
//           pucOutput[out],  Output data buffer pointer

// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void Lib_AlgHash(unsigned char *pucDataIn, unsigned long ulDataInLen, unsigned char *pucDataOut);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgHash256
// Func    : Calculate Hash256 for any data
// Paras   : pucDataIn[in],    input data buffer 
//           ulDataInLen[in],  input the length of data
//           pucDataOut[out]:  output the Hash256 result
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
void Lib_AlgHash256(unsigned char *pucDataIn, unsigned long ulDataInLen, unsigned char *pucDataOut);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgMD5
// Func    : Calculate MD5 for any data
// Paras   : pucDataIn[in],    input data buffer 
//           ulDataInLen[in],  input the length of data
//           pucDataOut[out]:  output the MD5 result
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgMD5(unsigned char *pucDataIn, unsigned long ulDataInLen, unsigned char *pucDataOut);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgGetRandom
// Func    : Get random numbers of fixed length
// Paras   : pucOut[out],    output random numbers of fixed length
//           ulLen[in],      input the length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
void Lib_AlgGetRandom(unsigned char *pucOut, unsigned long ulLen);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgRsa
// Func    : Perform RSA encryption/decryption operations
// Paras   : pucKey[in],        Key buffer pointer
//           ulKeyLen[in],      Key length (in BYTE)
//           usProtoKeyBit[in], Pointer to the exponential buffer that stores RSA operations.
//           ulExpLen[in],      Exponential length (in BYTE units)
//           ulExpLen[in],      Input data buffer pointer, with the same length as the module length
//           pucDataOut[out],   Output data buffer pointer, with the same length as the module length
//           ucMode[in],        0        encryption
//                             Non-0     decryption
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgRsa(unsigned char *pucKey, unsigned long ulKeyLen, unsigned short usProtoKeyBit, unsigned char *pucDataIn,
                   unsigned char *pucDataOut, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgRsaKeyPairGen
// Func    : Generate RSA public-private key pairs
// Paras   : pucRsaPk[out],      Output the public key buffer pointer.
//           pucRsaSk[out],      Output private key buffer pointer.
//           usProtoKeyBit[int], length of the modulus, 512, 1024 or 2048.
//           ucPubEType[int],    exponent tyepe:
//                               0: 0x00, 0x00, 0x00, 0x03
//                               1: 0x00, 0x01, 0x00, 0x01
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgRsaKeyPairGen(unsigned char *pucRsaPk, unsigned char *pucRsaSk, unsigned short usProtoKeyBit);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgRsaKeyPairVerify
// Func    : Verify RSA public-private key pairs
// Paras   : pucRsaPk[in],      input the public key buffer pointer.
//           pucRsaSk[in],      input private key buffer pointer.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_AlgRsaKeyPairVerify(unsigned char *pucRsaPk, unsigned char *pucRsaSk);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgBase64Encode
// Func    : Base64 Encode data 
// Paras   : dst[in/out],   input data buffer and output base64 encoding data 
//           dlen[in],      input length of data buffer
//           olen[in/out],  output length of base64 encoding data
//           src[in],       input original data
//           slen[in],      input length of original data
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_AlgBase64Encode(unsigned char *dst, unsigned int dlen, unsigned int *olen, const unsigned char *src, unsigned int slen);

//-----------------------------------------------------------------------------
// Name    : Lib_Base64Decode
// Func    : Base64 Decode data 
// Paras   : dst[in/out],   input data buffer and output base64 decoding data 
//           dlen[in],      input length of data buffer
//           olen[in/out],  output length of base64 decoding data
//           src[in],       input original data
//           slen[in],      input length of original data
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_AlgBase64Decode(unsigned char *dst, unsigned int dlen, unsigned int *olen, const unsigned char *src, unsigned int slen);

//-----------------------------------------------------------------------------
// Name    : Lib_AlgGetCrc16
// Func    : Calculate CRC16
// Paras   : pucBuf[in/out],  output CRC16 result
//           ulLen[in],       input data length
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
unsigned short Lib_AlgGetCrc16(unsigned char *pucBuf, unsigned long ulLen);

#endif