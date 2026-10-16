/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : VAN_security.h
 *  Version      :
 *  Author       :
 * Description   :
 **********************************************************************************************************/

#ifndef __VAN_SECURITY_H__
#define __VAN_SECURITY_H__
#include <stdio.h>

#ifndef u8
#define u8   unsigned char
#endif

#ifndef u16
#define u16   unsigned short
#endif

// DES algorithm
#define ALGORITHM_3DES              0
// Chinese National Security Algorithm - SM4
#define ALGORITHM_SM4               1
// DUKPT
#define ALGORITHM_DUKPT             2

#define DES3_2KEY_ECB_ENC   0x01
#define DES3_2KEY_ECB_DEC   0x03
#define DES_ECB_ENC         0x00
#define DES_CBC_ENC         0x08
#define DES3_3KEY_ECB_ENC   0x05
#define DES3_3KEY_ECB_DEC   0x07

#define DES3_2KEY_CBC_ENC   0x09
#define DES3_2KEY_CBC_DEC   0x0B

#define DUKPT_WK_TPK    			0
#define DUKPT_WK_TAK_REQUIRE      	1
#define DUKPT_WK_TAK_RESPONSE     	2
#define DUKPT_WK_TDK_REQUIRE      	3
#define DUKPT_WK_TDK_RESPONSE     	4

// Financial card transaction working key
#define KEYID_ZMK                   11
#define KEYID_TMK                   1
#define KEYID_TPK                   2
#define KEYID_TSK                   3
#define KEY_MAX_LEN                 32


extern u8 g_ucSubAlgFlag;
extern u8 g_SubEcbOrCbc;


extern u8 GetUnionMac1(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);
extern u8 GetUnionMac2(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);
extern u8 GetXorEncryptMac1(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);
extern u8 GetXorEncryptMac2(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);
extern u8 GetX919Mac(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);
extern u8 GetX919Mac3DES(u8 *pucSourceData, u16 usSourceDataLen, u8 *pucDestData);

extern u8 Sam3DesNoRandomEncript(u8 *pucPlaintext, u8 *pucCiphertext, u16 ucTextLen, u8 ucKeyID);
extern u8 Se3DesProcess(u8 ucP1_KeyID, u8 ucP2_EnOrDe, u8 *pucInBuf, u8 ucInBufLen, u8 *pucOutBuf);
extern u8 DataEncryptProcess(u8 *pucSourceBuf, u8 *pucDestBuf, u16 usSourceLen, u8 ucKeyID);
extern u8 VANCalcMac(u8 *key, u8 *inputData, u16 inputDataLen, u8 *outputData);
extern u8 SetAlgorithmMode(u8 mode);
extern u8 SetEcbOrCbcMode(u8 mode);
extern u8 SetCipherInitializationVector(u8 *IV);
extern u8 GetRandom(u8 *pucRecv, u8 *pucRandom, u8 *pucRandomNum);
#endif
