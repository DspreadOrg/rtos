/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : LanguageResource.c
 *  Version      :
 *  Author       :
 * Description   : Define display default XML file
 **********************************************************************************************************/

#include "LanguageResource.h"
#include "Common.h"
#include "Drv.h"

static char s_ascResourceBuf[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
<language description=\"English\" version=\"1.0.0\">\
	<resources>\
		<res><ID>1</ID><CON>11Select language</CON></res>\
		<res><ID>2</ID><CON>11Initialize language</CON></res>\
		<res><ID>3</ID><CON>11Setting done</CON></res>\
		<res><ID>4</ID><CON>11Initialize fail</CON></res>\
        <res><ID>5</ID><CON>11Use default menu</CON></res>\
	</resources>\
</language>";


char* GetResourceAddr(void)
{
    return s_ascResourceBuf;
}

unsigned long GetResourceLen(void)
{
    return sizeof(s_ascResourceBuf);
}

