//-----------------------------------------------------------------------------
// @file    : LibCharConvert.h
// @brief   : Data format conversion
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_CHARCONVERT_H__
#define __LIB_CHARCONVERT_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name    : Lib_Asc2Bcd
// Func    : change Ascii to BCD
// Paras   : pcAsc[in],    input the original data
//           pucBcd[in/out],  input data buffer and output the destination data
//           usLen[in]:  length of the original data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_Asc2Bcd(char *pcAsc, unsigned char *pucBcd, unsigned short usLen);

//-----------------------------------------------------------------------------
// Name    : Lib_Bcd2Asc
// Func    : change BCD to Ascii
// Paras   : pucBcd[in],    input the original data
//           pcAsc[in/out],  input data buffer and output the destination data
//           usLen[in]: length of the original data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_Bcd2Asc(unsigned char *pucBcd, char *pcAsc, unsigned short usLen);

//-----------------------------------------------------------------------------
// Name    : Lib_Bcd2Int
// Func    : change BCD to Int
// Paras   : pucBcd[in],    input the original data
//           ucBcdLen[in]: length of the original data
// Ret     : return the Integer data
// Desc    :
//-----------------------------------------------------------------------------
int Lib_Bcd2Int(unsigned char *pucBcd, unsigned char ucBcdLen);

//-----------------------------------------------------------------------------
// Name    : Lib_Int2Bcd
// Func    : change int to BCD
// Paras   : uiNum[in],    input the original data
//           pucBcd[in/out],  input data buffer and output the destination data
//           ucBcdLen[in]:  the BCD data length 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_Int2Bcd(unsigned int uiNum, unsigned char *pucBcd, unsigned char ucBcdLen);

//-----------------------------------------------------------------------------
// Name    : Lib_HextoStr
// Func    : change hexadecimal to String
// Paras   : src[in],    input the original data
//           dst[in/out],  input data buffer and output the destination data
//           Src_Len[in]:  the hexadecimal data length 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_HextoStr(unsigned char *src, unsigned char *dst, unsigned char Src_Len);

//-----------------------------------------------------------------------------
// Name    : Lib_AscNum2bcd
// Func    : change Ascii Number to BCD
// Paras   : pucAsc[in],    input the original data
//           pucBcd[in/out],  input data buffer and output the destination data
//           ucAscLen[in]:  length of the original data
//           pucBcdLen[in]:  output the destination data length
//           ucAdd[in]:  fill data
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_AscNum2bcd(unsigned char *pucAsc, unsigned char *pucBcd, unsigned char ucAscLen, unsigned char *pucBcdLen, unsigned char ucAdd);

#endif