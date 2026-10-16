//-----------------------------------------------------------------------------
// @file    : LibSelfCheck.h
// @brief   : SelfCheck functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef _LIB_SELFCHECK_H_
#define _LIB_SELFCHECK_H_

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef enum
{
    SELF_CHECK_MODE_WHOLE,         // whole device self-check
    SELF_CHECK_MODE_AUTO,          // Power on self-check, only new devices will perform it
    SELF_CHECK_MODE_AGING,         // Aging self-check
    SELF_CHECK_MODE_SINGLE,        // Single item self-check
    SELF_CHECK_MODE_MANUAL,        // Force manual self-check
} enSelfCheckMode;

//-----------------------------------------------------------------------------
// Name    : Lib_SelfCheckTest
// Func    : SelfCheck Test
// Paras   : mode[in],  input enSelfCheckMode
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_SelfCheckTest(enSelfCheckMode mode);

//-----------------------------------------------------------------------------
// Name    : Lib_SelfCheckGetLibVersion
// Func    : Get self-check library version
// Paras   : paucLibVer[in/out],  output version
//           pusLen[in/out],      output version length
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_SelfCheckGetLibVersion(char *paucLibVer, unsigned short *pusLen);

#endif