/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Key.c
 *  Version      :
 *  Author       :
 * Description   : Used for processing keyboard related functions
 **********************************************************************************************************/

#include "Key.h"
#include "Drv.h"
#include "Power.h"
#include "ParamManage.h"
#include "Input.h"
#include "Common.h"
#include "Idle.h"
#include "api_lvgl.h"

// Initial timing of backlight
unsigned long s_ulDelayStart = 0;
// Backlight timing time
unsigned long s_ulDelaySetCnt = 0;

static unsigned char s_KbSetKey = KB_NULL;
// LCD constant on/off switch, defaults off
unsigned char s_ucLcdBacklightAllOn = FALSE;


//-----------------------------------------------------------------------------
// Name    : KbReadKey
// Func    : Reading pressed key value
// Paras   :
// Ret     : The Key value of the pressed key
// Desc    :
//-----------------------------------------------------------------------------
unsigned char KbReadKey(void)
{
    unsigned char ucTemp;
    short sRC;

    Lib_WDGReloadCounter();
    sRC = LvUiWaitKey(200);

    if (sRC < 0)
    {
        ucTemp = 0xff;
    }
    else
    {
        ucTemp = sRC;
    }

    return ucTemp;
}


//-----------------------------------------------------------------------------
// Name    : KbSetKey
// Func    : Software settings key values
// Paras   : UcKey, the key value to be set
// Ret     : The Key value of the pressed key
// Desc    : Start guide key for English and Chinese input
//-----------------------------------------------------------------------------
void KbSetKey(unsigned char ucKey)
{
    s_KbSetKey = ucKey;
}


//-----------------------------------------------------------------------------
// Name    : KbWaitingKey
// Func    : Get key input, timeout exit
// Paras   : pucKey   : Store key values to be read
//           iTimeout : Timeout, in ms
// Ret     : KB_RETURN_OK
//           KB_RETURN_TIMEOUT
// Desc    :
//-----------------------------------------------------------------------------
signed short KbWaitingKey(unsigned char *pucKey, int iTimeout)
{
    unsigned char ucKey;
    unsigned long ulStartTime=0;

    if (0 == iTimeout)
    {
        return KB_RETURN_TIMEOUT;
    }

    ulStartTime = Lib_SysGetTimer();
    while (1)
    {
        // Read the value of the pressed key
        ucKey = KB_NULL;

        ucKey = KbReadKey();

        if (ucKey > 0 && ucKey != 0xff)
        {
            if(pucKey != NULL)
            {
                *pucKey = ucKey;
            }
            return KB_RETURN_OK;
        }

        if (IS_TIME_OUT(ulStartTime, iTimeout))
        {
            return KB_RETURN_TIMEOUT;
        }
    }
}

