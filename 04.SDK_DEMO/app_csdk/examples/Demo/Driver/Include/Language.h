/**********************************************************************************************************
 * Copyright(C)2011, 
 * Description: Factory language macro definition
 * Change Logs:
 * Date           Author         Notes
 * 2022-03-04     leeon          first version
 **********************************************************************************************************/

#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

#ifdef XML_MENU
#include "LanguageXML.h"
/**********************************************************************************************************
 * Chinese Display Definition
 **********************************************************************************************************/
#elif defined(LANGUAGE_TYPE_CN)
#include "LanguageCN.h"

/**********************************************************************************************************
 * English Display Definition
 **********************************************************************************************************/
#elif defined(LANGUAGE_TYPE_EN) || defined(LANGUAGE_TYPE_FARSI)
// In the future, the macro definition menu format will be cancelled and all will use XML menu configuration format
#include "LanguageEN.h"

/**********************************************************************************************************
 * Portuguese Display Definition
 **********************************************************************************************************/
#elif defined(LANGUAGE_TYPE_PT)
#include "LanguagePT.h"

/**********************************************************************************************************
 * No language macros defined
 **********************************************************************************************************/
#else
#include "LanguageEN.h"
#endif

#endif

