/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : VAN_security.c
 *  Version      :
 *  Author       :
 * Description   : Security algorithm processing module
 **********************************************************************************************************/

#include "VAN_security.h"
#include "LibApi.h"
#include "Common.h"

u8 g_ucSubAlgFlag = 0;
u8 g_SubEcbOrCbc = 0;

// DUKPT KSN
u8 g_aucDUKPTKSN[10] = {0xFF};
// DUPKT KEY
u8 g_aucDUPKTKEY[5][16];
u8 s_SubVector[8]={0};
// random number
u8 g_aucRandom[16];

u8 Sam3DesNoRandomEncript(u8 *pucPlaintext, u8 *pucCiphertext, u16 ucTextLen, u8 ucKeyID)
{
	u8 mode = 0;

	if (g_SubEcbOrCbc)
	{
		mode |= MODE_CBC;
		TRACE("3des-g_SubEcbOrCbc!\r\n");
	}
	TRACE("pucPlaintext!\r\n");
	TRACEBUF(pucPlaintext, ucTextLen);
	TRACE("ucTextLen! %d\r\n", ucTextLen);
    //Lib_PciGetDes(ucKeyID, ucTextLen, pucPlaintext, pucCiphertext, mode);
    Lib_PciAlgDes(ucKeyID, s_SubVector, pucPlaintext, ucTextLen, pucCiphertext, mode);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Name    : DataEncryptProcess
// Func    : Data encryption processing
// Paras   : pucSourceBuf[in], Source data cache
//           usSourceLen[in],  Source data length,
//           ucKeyID[in],      encryption key
// Ret     : 1-Success, 0-Failure
// Desc    :
//-----------------------------------------------------------------------------
u8 DataEncryptProcess(u8 *pucSourceBuf, u8 *pucDestBuf, u16 usSourceLen, u8 ucKeyID)
{
	switch(g_ucSubAlgFlag)
	{
		case ALGORITHM_3DES:
			TRACE("3DES process!\r\n");
			if (FALSE == Sam3DesNoRandomEncript(pucSourceBuf, pucDestBuf, usSourceLen, ucKeyID))
			{
				return 1;
			}
			memcpy(pucDestBuf, pucSourceBuf, usSourceLen);
			break;

		case ALGORITHM_SM4:
			return 1;
		break;

		case ALGORITHM_DUKPT:
			if (ucKeyID >= 5)
			{
				TRACE("DUKPT-workkey-id-err!\r\n");
				return 1;
			}
			TRACE("DUKPT-encrypt!\r\n");

			if (FALSE == Lib_DukptEncryptOrDecrypt(0,ucKeyID,0,pucSourceBuf,&usSourceLen))
			{
				return 1;
			}
			memcpy(pucDestBuf, pucSourceBuf, usSourceLen);
			break;

		default:
			break;
	}
	TRACE("Encrypt Result: ", pucDestBuf, usSourceLen);
    return 1;
}

u8 SetAlgorithmMode(u8 mode)
{
	if(mode <3)
	{
		g_ucSubAlgFlag = mode;
		Lib_SetAlgorithmMode(mode);
		return TRUE;
	}
	return FALSE;
}

u8 SetEcbOrCbcMode(u8 mode)
{
	if(mode <1)
	{
		g_SubEcbOrCbc = mode;
		Lib_SetCipherMode(mode);
		return TRUE;
	}
	return FALSE;
}
u8 SetCipherInitializationVector(u8 *IV)
{
	memcpy(s_SubVector,IV,8);
	return Lib_SetCipherInitializationVector(IV);
}

#if 0
u8 CalcMac(u8 *key, u8 *inputData, u16 inputDataLen, u8 *outputData)
{
    return TRUE;
}
#endif

