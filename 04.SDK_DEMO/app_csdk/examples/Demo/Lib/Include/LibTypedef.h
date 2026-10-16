//-----------------------------------------------------------------------------
// @file    : LibTypedef.h
// @brief   : Define various data types
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_TYPEDEF_
#define __LIB_TYPEDEF_

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------------
// constant
//---------------------------------------------------------------------------
#ifndef NULL
#define NULL (void *)0
#endif

#define RET_OK                           (0)
#define RET_ERR                          (-1)
#define RET_ERR_PARAMETER                 (-2)
#define RET_ERR_OUT_OF_MEMORY       (-3)
#define RET_ERR_TIMEOUT          (-4)

// Boolean type
#ifndef TRUE
#define TRUE 			1
#endif
#ifndef FALSE
#define FALSE 			0
#endif

// Constant bit
#define BIT0     (1<<0) /**< BIT0 macro definition */
#define BIT1     (1<<1) /**< BIT1 macro definition */
#define BIT2     (1<<2) /**< BIT2 macro definition */
#define BIT3     (1<<3) /**< BIT3 macro definition */
#define BIT4     (1<<4) /**< BIT4 macro definition */
#define BIT5     (1<<5) /**< BIT5 macro definition */
#define BIT6     (1<<6) /**< BIT6 macro definition */
#define BIT7     (1<<7) /**< BIT7 macro definition */
#define BIT8     (1<<8) /**< BIT8 macro definition */
#define BIT9     (1<<9) /**< BIT9 macro definition */
#define BIT10    (1<<10) /**< BIT10 macro definition */
#define BIT11    (1<<11) /**< BIT11 macro definition */
#define BIT12    (1<<12) /**< BIT12 macro definition */
#define BIT13    (1<<13) /**< BIT13 macro definition */
#define BIT14    (1<<14) /**< BIT14 macro definition */
#define BIT15    (1<<15) /**< BIT15 macro definition */
#define BIT16    (1<<16) /**< BIT16 macro definition */
#define BIT17    (1<<17) /**< BIT17 macro definition */
#define BIT18    (1<<18) /**< BIT18 macro definition */
#define BIT19    (1<<19) /**< BIT19 macro definition */
#define BIT20    (1<<20) /**< BIT20 macro definition */
#define BIT21    (1<<21) /**< BIT21 macro definition */
#define BIT22    (1<<22) /**< BIT22 macro definition */
#define BIT23    (1<<23) /**< BIT23 macro definition */
#define BIT24    (1<<24) /**< BIT24 macro definition */
#define BIT25    (1<<25) /**< BIT25 macro definition */
#define BIT26    (1<<26) /**< BIT26 macro definition */
#define BIT27    (1<<27) /**< BIT27 macro definition */
#define BIT28    (1<<28) /**< BIT28 macro definition */
#define BIT29    (1<<29) /**< BIT29 macro definition */
#define BIT30    (1<<30) /**< BIT30 macro definition */
#define BIT31    (1<<31) /**< BIT31 macro definition */

#ifndef LONGLONG
typedef long long               LONGLONG;
#endif

#ifndef ULONGLONG
typedef unsigned long long      ULONGLONG;
#endif

// macro function
#ifndef MAKEWORD
#define MAKEWORD(HiByte, LoByte)    ((((unsigned short)(HiByte) << 8) & 0xFF00) |\
                                     ((unsigned short)(LoByte) & 0x00FF))
#endif

#ifndef WORD2BUF
#define WORD2BUF(l, buf)                {*(buf) = (unsigned char)((l)>>8)&0xFF; *((buf)+1) = (unsigned char)(l)&0xFF; }
#endif

#ifndef MAKEDWORD
#define MAKEDWORD(FHiByte, SHiByte, FLoByte, SLoByte)    ((((unsigned long)(FHiByte)<<24)&0xFF000000) | \
                                                         (((unsigned long)(SHiByte)<<16)&0xFF0000) | \
                                                         (((unsigned long)(FLoByte)<<8)&0xFF00) | (((unsigned long)(SLoByte))&0xFF))
#endif

#endif