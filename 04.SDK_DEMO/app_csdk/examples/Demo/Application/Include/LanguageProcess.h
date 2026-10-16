/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : LanguageProcess.h
 *  Version      :
 *  Author       :
 * Description   : Implement multilingual display function
 **********************************************************************************************************/

#ifndef __LANGUAGE_PROCESS_H__
#define __LANGUAGE_PROCESS_H__

#include "LibApi.h"

/**********************************************************************************************************
 * Macro Definition
 **********************************************************************************************************/
// Is the multilingual display function enabled?
// Need to add XML display content configuration
#ifdef XML_MENU
#define MULTI_LANGUAGE_EN       1
#else
#define MULTI_LANGUAGE_EN       0
#endif


/**********************************************************************************************************
 * Language ID Definition
 **********************************************************************************************************/
typedef enum
{
    ENGLISH_ID      = 0,
    PORTUGUESE_ID   = 1,
    FARSI_ID        = 2,

    MAX_LANG_ID,
    // To add language, please notify the SDK side

}E_LANG_ID;


/**********************************************************************************************************
 * variable declaration
 **********************************************************************************************************/
extern unsigned char g_ucLanguage;

/**********************************************************************************************************
 * Function declaration
 **********************************************************************************************************/
extern short LanguageMenuSet(int iTimeout);
extern int ResourceInit(void);
extern unsigned char GetLanguageType(void);


#endif

