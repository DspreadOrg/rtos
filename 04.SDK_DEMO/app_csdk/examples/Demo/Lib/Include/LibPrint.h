//-----------------------------------------------------------------------------
// @file    : LibPrint.h
// @brief   : Print functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_PRINT_H__
#define __LIB_PRINT_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// Maximum printing density level
#define PT_PRINT_GRADE_MAX              5

// Reverse printing
#define PRINT_REVERSE                   0x10
// Character border
#define PRINT_BORDER                    0x20

#define PT_BORDER_WIDTH                 3       // Print Border Width


#define LTP_MAX_DOT                     384
#define LTP_MAX_WORD                    (LTP_MAX_DOT/32)
#define LTP_MAX_BYTE                    (LTP_MAX_DOT/8)
#define TPH_BUF_MAX                     32

//-----------------------------------------------------------------------------
// Name    : Lib_PrnParaDefaultInit
// Func    : Initialize Printer
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnParaDefaultInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnOpen
// Func    : Open Printer
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnOpen (void);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnClose
// Func    : Close Printer
// Paras   : 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnClose(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnCheckStatus
// Func    : Get the Printer Status
// Paras   : 
// Ret     : return status,for example,0: normal,less than 0 is paper shortage
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnCheckStatus(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnSetSpace
// Func    : Set line space
// Paras   : ucSpace[in],  input line space
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_PrnSetSpace(unsigned char ucSpace);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnSetFont
// Func    : Set Print Font
// Paras   : ucFont[in],   input Font,it should be 16 or 24.
//           ucZoomH[in],  input horizontal zoom
//           ucZoomV[in],  input vertical zoom
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnSetFont(unsigned char ucFont , unsigned char ucZoomH, unsigned char ucZoomV);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnGetFont
// Func    : Get Print Font
// Paras   : pucFont[out],   output Font
//           pucZoomH[out],  output horizontal zoom
//           pucZoomV[out],  output vertical zoom
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnGetFont(unsigned char *pucFont , unsigned char *pucZoomH, unsigned char *pucZoomV);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnSetGrade
// Func    : Set Printer density grade
// Paras   : ucGrade[in],  input Printer Density, grade - 1 ~ 5, 1 is lightest, 5 is darkest
// Ret     : 
// Desc    : Before using printer parameters,you must call this function.
//-----------------------------------------------------------------------------
void Lib_PrnSetGrade(unsigned char ucGrade);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnGetGrade
// Func    : Get Printer density grade
// Paras   : 
// Ret     : 1 ~ 5  - printer density grade (1 = lightest, 5 = darkest)
// Desc    : Returns the current printer density grade.
//-----------------------------------------------------------------------------
unsigned char Lib_PrnGetGrade(void);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnPaperForward
// Func    : Print blank lines of a specified length
// Paras   : iPaperDistance[in],  input length, unit is millimeters
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_PrnPaperForward(int iPaperDistance);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnGetStrWidth
// Func    : Get the width of the string to be printed
// Paras   : prnBuf[in],   input string
//           prnLen[in],   input length
//           ucFont[in],   input Font
//           ucZoomH[in],  input Horizontal zoom
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnGetStrWidth(char *prnBuf, unsigned short prnLen, unsigned char ucFont , unsigned char ucZoomH);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnStr
// Func    : Print String
// Paras   : fmt[in],        input format
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnStr(unsigned char *fmt, ...);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnStrEx
// Func    : Print String
// Paras   : mode[in],    input mode
//           prnBuf[in],  input data buffer
//           prnLen[in],  input data length
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnStrEx(unsigned char mode, char *prnBuf, unsigned short prnLen);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnEsig
// Func    : Print Electronic Signature
// Paras   : pucEsig[in],  input Electronic Signature
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnEsig(unsigned char *pucEsig);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnBmp
// Func    : Print BMP
// Paras   : usStartX[in],  input starting position
//           pucLogo[in],   input Logo BMP
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnBmp(unsigned short usStartX, unsigned char *pucLogo);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnXCode
// Func    : Print image in the specified area
// Paras   : usStartX[in],  input starting coordinate of horizontal axis
//           prnBuf[in],    input data buffer
//           usXSize[in],   input width
//           usYSize[in],   input height
//           pucLogo[in],   input Logo 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_PrnXCode(unsigned short usStartX, char *prnBuf, unsigned short usXSize, unsigned short usYSize, 
                  unsigned char *pucLogo);

//-----------------------------------------------------------------------------
// Name    : Lib_PrnSetForwardDistance
// Func    : Set the default paper feed length when the printer is off.
// Paras   : usPaperLen[in],  input paper feed length, unit is millimeters
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_PrnSetForwardDistance(unsigned short usPaperLen);

#endif