/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Ui.c
 *  Version      :
 *  Author       :
 * Description   : Implement of display related functions
 **********************************************************************************************************/
#include "Language.h"
#include "Ui.h"
#include "Input.h"
#include "Common.h"
#include "Drv.h"
#include "Communication.h"
#include "MessageBox.h"
#include "Battery.h"
#include "Key.h"
#include "ParamManage.h"
#include "M_NetInterface.h"
#include "LanguageProcess.h"
#include "LanguageResource.h"
#include "api_lvgl.h"
#include "Menu.h"
#include "custom_Lvgl.h"
#include "lv_txt_ap.h"

#define RGB24toRGB16(r, g, b)  (((uint32_t)r >> 3) << 11) | (((uint32_t)g >> 2) << 5) | (b >> 3)


#define PAGE_MAX_SHOW_MENU_COUNT        (4)

const uint16_t MenuXY[][2] =
{
    { 20, 79},
    {166, 79},
    { 20, 117},
    {166, 117},
    { 20, 155},
    {166, 155},
    { 20, 193},
    {166, 193},
};

const uint16_t MenuPTXY[][2] =
{
    {166, 79},
    { 20, 79},
    {166, 117},
    { 20, 117},
    {166, 155},
    { 20, 155},
    {166, 193},
    { 20, 193},
};

const uint16_t SubMenuXY[][2] =
{
    { 20,  79},
    { 20, 117},
    { 20, 155},
    { 20, 193},
};

// Status bar refresh control switch
static unsigned char s_barRefreshFlag = 1;

extern void *hDialog;

char *bgImgIcons[] =
{
    (char *)"P:lvgl/option1.png",
    (char *)"P:lvgl/option2.png",
    (char *)"P:lvgl/option3.png",
    (char *)"P:lvgl/option4.png",
    (char *)"P:lvgl/option5.png",
    (char *)"P:lvgl/option6.png",
};

void UISetBarRefreshFlag(unsigned char flag)
{
    s_barRefreshFlag = flag;
	LvUiCtrlStatusBar(s_barRefreshFlag);
    Lib_LcdClrScreen();
    if (s_barRefreshFlag != 0)
    {
        UiRefreshNow();
    }
}

void UIText(unsigned short usX, unsigned short usY, unsigned char ucMode, const char *pcFmt, ...)
{
	char str[110];
    unsigned char language = 0;

	memset(str, 0, sizeof(str));

//	if (!s_ucUpdataLcdEnable)
//	{
//		// LCD display refresh switch is turned off
//		return;
//	}

	if ((usX > LCD_MAX_SEG-1) || (usY > MAX_COM_LINES-1))
	{
		return;
	}

	if (!pcFmt)
	{
		return;
	}

	if (strlen(pcFmt) > sizeof(str))
	{
		FUN_IN;
		return;
	}

#if 1
	{
		va_list aptr;
		va_start(aptr, pcFmt);
		vsprintf(str, pcFmt, aptr);
		va_end(aptr);
	}
#endif

	if (usY > MAX_COM_LINES-1)
	{
		return;
	}

    GetLanguageParam(&language, sizeof(language));
    if (language != FARSI_ID)
    {
        UITextAlign(ALIGN_L, usY, ucMode, str);
    }
    else
    {
        UITextAlign(ALIGN_R, usY, ucMode, str);
    }
}

//-----------------------------------------------------------------------------
// Name    : UiRefreshNow
// Func    : Ui Refresh Now
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
unsigned char UiRefreshNow(void)
{
    return 0;
}

//-----------------------------------------------------------------------------
// Name    : UIWaitKey
// Func    : UI Menu waiting for key press
// Paras   : iTimeout, timeout value, unit: milliseconds. range: -1~655350
//                        >0,  Wait for the keys to be pressed, and return after timeout
 //                       ==0, return immediately
 //                       <0,  Always wait until the key is pressed.
//           ucBeepFlag,  0 - When pressing, the buzzer does not sound;
//                        1 - Buzzer sound
// Ret     : Returns the UI event of a key press.
//           UI_INPUT_TIMEOUT
//           UI_INPUT_CANCEL
//           UI_INPUT_UP
//           UI_INPUT_DOWN
//           UI_INPUT_ENTER
// Desc    :
//-----------------------------------------------------------------------------
short UIWaitKey(int iTimeout, unsigned char ucBeepFlag)
{
    unsigned char ucKey;
    short sRC;

    sRC = KbWaitingKey(&ucKey, iTimeout);

    if (KB_RETURN_TIMEOUT == sRC)
    {
        return UI_INPUT_TIMEOUT;
    }
    else
    {
        switch (ucKey)
        {
        case KB_CANCEL :    return UI_INPUT_CANCEL;
        case KB_UP :        return UI_INPUT_UP;
        case KB_DOWN :      return UI_INPUT_DOWN;
        case KB_ENTER :     return UI_INPUT_ENTER;
        case KB_CLEAR :     return UI_INPUT_CLEAR;
        case KB_POWER_OFF : return UI_POWER_OFF;
        case KB_ZM   :      return UI_INPUT_ZM;
        case KB_FUN:        return UI_INPUT_FUN;
        case KB_MENU :      return UI_INPUT_MENU;
        default :           return (short)ucKey;
        }
    }
//	return RET_ERR;
}


//-----------------------------------------------------------------------------
// Name    : Str2Ip
// Func    : Convert the input IP string into 4 segments string output
// Paras   : pcIp[in],    Source IP string
//           pcOut1[out], Output IP string1
//           pcOut2[out], Output IP string2
//           pcOut3[out], Output IP string3
//           pcOut4[out], Output IP string4
// Ret     : RET_OK,       Conversion successful
//           RET_ERR,      Conversion fail
// Desc    :
//-----------------------------------------------------------------------------
static short Str2Ip(char* pcIp, char* pcOut1, char* pcOut2, char* pcOut3, char* pcOut4)
{
	unsigned char i;
	char acIp[24] = {0};
	char *pcTemp = 0, *pcStart = 0;


	if (strlen(pcIp) > sizeof(acIp)-1)
	{
		return RET_ERR;
	}

	memcpy(acIp, pcIp, strlen(pcIp));
	pcStart = acIp;

	for (i=0; i<3; i++)
	{
		pcTemp = strchr(pcStart, '.');
		if (NULL == pcTemp)
		{
			return RET_ERR;
		}
		pcTemp[0] = 0;

		if (strlen(pcStart) > 3)
		{
			return RET_ERR;
		}

		if (0 == i)
		{
			memcpy(pcOut1, pcStart, strlen(pcStart));
		}
		else if (1 == i)
		{
			memcpy(pcOut2, pcStart, strlen(pcStart));
		}
		else if (2 == i)
		{
			memcpy(pcOut3, pcStart, strlen(pcStart));
		}

		pcStart = pcTemp+1;
	}

	memcpy(pcOut4, pcStart, strlen(pcStart));
	return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UIInput
// Func    : Input information through key on the menu
// Paras   : ucX[in],                   Starting cursor X coordinate. range: 0 ~ 127
//           ucY[in],                   Starting cursor Y coordinate. range: 0 ~ 4
//           pcDefaultInputStr[in],     Default input string information.
//           pucBuff[out],              Input data cache pointer,the cache length should be one byte longer
//                                      than the desired input information length, used to save string terminators.
//           pucLen[in][out],           Input parameter: desired length of input data
//                                      Output parameter: get the actual length of input data.
//           ucInputMode[in],           Input mode. Normal mode or password input mode; Specify the length mode or any length mode.
//           iTimeout,                  the timeout of iput in milliseconds. Value range: -1~655350
//                                      >0,  Wait for the keys to be pressed, and return after timeout
//                                      ==0, return immediately
//                                      <0,  Always wait until the key is pressed.
//           ucBeepFlag,  0 - When pressing, the buzzer does not sound;
//                        1 - Buzzer sound
// ucOutXmode[in],    Not used yet, reserved
//           pcOut[out],        Cache pointer of amount input
//           pucOutLen[out],    Amount input length
//           iTimeout[out],     input timeout value, unit: millisecond, srange: -1~655350
//                              >0,  Wait for the keys to be pressed, and return after timeout
//                              ==0, return immediately
//                              <0,  Always wait until the key is pressed.
// Ret     : Input information result
//-          0, Indicates that the input is valid, UI_INPUT_ENTER
// -         <0, Indicates that the input is invalid.
//           RET_ERR_PARAMETER
//           UI_INPUT_CANCEL
//           UI_INPUT_TIMEOUT
// Desc    : PucBuff must be large enough.
//-----------------------------------------------------------------------------
short UIInput(unsigned char ucX, unsigned char ucY,
            char *pcDefaultInputStr,
            unsigned char *pucBuff, unsigned char *pucLen,
            unsigned char ucInputMode, int iTimeout,
            unsigned char ucBeepFlag)
{
    unsigned char ucInputLenMax;    // Maximum input length
    unsigned char ucInputCnt;       // Input counter
    //unsigned char ucBlankX;       // The position of the character display
    short sRC;
    unsigned char *p;
    unsigned char ucStrLen;
    unsigned char ucStarCnt;
    //unsigned char ucByteWidth;
    unsigned char aucTemp[30];

    // Check if the parameters are valid or not
    if((ucX > 127) || (ucY > MAX_COM_LINES-1))
    {
        return RET_ERR_PARAMETER;
    }
    if((pucBuff == NULL) || (pucLen == NULL))
    {
        return RET_ERR_PARAMETER;
    }
    if(iTimeout > 0x7FFFFFFF)
    {
        return RET_ERR_PARAMETER;
    }

    //
    if(iTimeout == 0)
    {
        return UI_INPUT_TIMEOUT;
    }

    //
    ucInputLenMax = *pucLen;

    // display default incoming strings
    if(pcDefaultInputStr == NULL)
    {
        ucInputCnt = 0;
    }
    else
    {
        ucInputCnt = strlen(pcDefaultInputStr);
        if(ucInputCnt > ucInputLenMax)
        {
            ucInputCnt = ucInputLenMax;
        }
        memcpy(pucBuff, pcDefaultInputStr, ucInputCnt);
    }

    // When the maximum input length is not reached, followed by an underline prompt at the end
    if(ucInputCnt < ucInputLenMax)
    {
        pucBuff[ucInputCnt] = '_';
        // Add string terminator
        pucBuff[ucInputCnt+1] = 0;
    }
    else
    {
        // Add string terminator
        pucBuff[ucInputCnt] = 0;
    }

    //UIDispStr(ucX, ucY, (char *)pucBuff, ucInputLenMax);
    UITextAlign(ALIGN_L, ucY, DISP_MODE_NORMAL, (char *)pucBuff);
    //ucByteWidth = 6;

    while(1)
    {
        // Read keyboard inputs
        sRC = UIWaitKey(iTimeout, 1);

        if((UI_INPUT_TIMEOUT == sRC)||(UI_POWER_OFF == sRC))
        //if(UI_INPUT_TIMEOUT == sRC)
        {
            return sRC;
        }

        if((sRC >= '0') && (sRC <= '9'))
        {
            if((INPUT_ZERO_ONE & ucInputMode) && (sRC >= '2'))
            {
                ;
            }
            else if(ucInputCnt < ucInputLenMax)
            {
                pucBuff[ucInputCnt++] = sRC;

                // When the maximum input length is not reached, followed by an underline prompt at the end
                if(ucInputCnt < ucInputLenMax)
                {
                    pucBuff[ucInputCnt] = '_';
                    // Add string terminator
                    pucBuff[ucInputCnt+1] = 0;
                }
                else
                {
                    // Add string terminator
                    pucBuff[ucInputCnt] = 0;
                }

                // Display keyboard input
                // Password mode displays' * '
                ucStrLen = strlen((const char *)pucBuff);
                if(ucStrLen >= (LCD_LINE_CHAR_ML-ucX/FONT_ASCII_WIDTH))
                {
                    p = pucBuff + ucStrLen - (LCD_LINE_CHAR_ML - ucX/FONT_ASCII_WIDTH);
                }
                else
                {
                    p = pucBuff;
                }

                if(ucInputMode & INPUT_PWD_MODE)
                {
                    // Replace the display with the character '*'
                    if(pucBuff[ucInputCnt] == '_')
                    {
                        // There is an input prompt at the end
                        ucStarCnt = strlen((char *)p) - 1;
                        memset(aucTemp, '*', ucStarCnt);
                        aucTemp[ucStarCnt] = '_';
                        aucTemp[ucStarCnt+1] = 0;
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)aucTemp);
                    }
                    else
                    {
                        // There is an input prompt at the end
                        ucStarCnt = strlen((char *)p);
                        memset(aucTemp, '*', ucStarCnt);
                        aucTemp[ucStarCnt] = 0;
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)aucTemp);
                    }
                }
                else
                {
                    UIText(ucX, ucY, DISP_MODE_NORMAL, (const char *)p);
                }

                if(    (ucInputCnt == ucInputLenMax)
                    && (INPUT_AUTO_RETURN & ucInputMode)
                    )
                {
                    return UI_INPUT_ENTER;
                }
            }
        }
        else if(UI_INPUT_CLEAR == sRC)
        {
            if(ucInputCnt > 0)
            {
                ucInputCnt--;
                pucBuff[ucInputCnt] = '_';
                // Add string terminator
                pucBuff[ucInputCnt+1] = 0;

                // Display keyboard input
                // Password mode displays' * '
                if(ucInputMode & INPUT_PWD_MODE)
                {
                    // Replace the display with the character '*'
                    ucStrLen = strlen((const char *)pucBuff);

                    if(ucStrLen >= (LCD_LINE_CHAR_ML-ucX/FONT_ASCII_WIDTH))
                    {
                        p = pucBuff + ucStrLen - (LCD_LINE_CHAR_ML - ucX/FONT_ASCII_WIDTH);
                        ucStarCnt = strlen((char *)p) - 1;

                        memset(aucTemp, '*', ucStarCnt);
                        aucTemp[ucStarCnt] = '_';
                        aucTemp[ucStarCnt+1] = 0;
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)aucTemp);
                    }
                    else
                    {
                        p = pucBuff;
                        ucStarCnt = strlen((char *)p) - 1;

                        // When pressing the clear button, the previously character "_"
                        // needs to be cleared in order to be displayed normally
                        memset(aucTemp, '*', ucStarCnt);
                        aucTemp[ucStarCnt] = '_';
                        aucTemp[ucStarCnt+1] = ' ';
                        aucTemp[ucStarCnt+2] = 0;
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)aucTemp);
                    }
                }
                else
                {
                    ucStrLen = strlen((const char *)pucBuff);

                    if(ucStrLen >= (LCD_LINE_CHAR_ML-ucX/FONT_ASCII_WIDTH))
                    {
                        p = pucBuff + ucStrLen - (LCD_LINE_CHAR_ML - ucX/FONT_ASCII_WIDTH);
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)p);
                    }
                    else
                    {
                        p = pucBuff;
                        UIText(ucX, ucY, DISP_MODE_NORMAL, (const char*)p);

                        //ucBlankX = ucX + (ucStrLen * ucByteWidth);
                        //UIText(ucBlankX, ucY, DISP_MODE_NORMAL, " ");
                    }
                }
            }
        }
        else if(UI_INPUT_ENTER == sRC)
        {
            if(ucInputMode & INPUT_MATCH_LEN)
            {
                if(ucInputCnt == *pucLen)
                {
                    // Add string terminator
                    pucBuff[ucInputCnt] = 0;
                    return UI_INPUT_ENTER;
                }
                else if(   (ucInputMode & INPUT_EN_ZERO_LEN)
                        && (ucInputCnt == 0)
                        )
                {
                    // Add string terminator
                    pucBuff[ucInputCnt] = 0;
                    *pucLen = ucInputCnt;
                    return UI_INPUT_ENTER;
                }
            }
            else
            {
                // Add string terminator
                pucBuff[ucInputCnt] = 0;
                *pucLen = ucInputCnt;
                return UI_INPUT_ENTER;
            }
        }
        else if(UI_INPUT_CANCEL == sRC)
        {
            *pucLen = 0;
            return UI_INPUT_CANCEL;
        }
        else
        {
            ;//do nothing
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : UIDefaultComposition_RtoL
// Func    : The default menu layout function, display from right to left
// Paras   : id[in],     Menu index
//           pBuf[in],   Menu pixel data
//           w[in],      width
//           h[in],     height
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void UIDefaultComposition_RtoL(int id, uint16_t *pBuf, uint16_t w, uint16_t h)
{
	switch (id)
	{
	case 01:
		Lib_LcdDispMemcpy(pBuf, 166, 79, w, h);
		break;

	case 0x02:
		Lib_LcdDispMemcpy(pBuf, 20, 79, w, h);
		break;

	case 0x03:
		Lib_LcdDispMemcpy(pBuf, 166, 117, w, h);
		break;

	case 0x04:
		Lib_LcdDispMemcpy(pBuf, 20, 117, w, h);
		break;

	case 0x05:
		Lib_LcdDispMemcpy(pBuf, 166, 155, w, h);
		break;

	case 0x06:
		Lib_LcdDispMemcpy(pBuf, 20, 155, w, h);
		break;

	case 0x07:
		Lib_LcdDispMemcpy(pBuf, 166, 193, w, h);
		break;

	case 0x08:
		Lib_LcdDispMemcpy(pBuf, 20, 193, w, h);
		break;

    default:
        break;
    }
}

void UiMenuGetLayout(int id, uint16_t *x, uint16_t *y)
{
    id = (id - 1) % 8;

    if (g_ucLanguage == FARSI_ID)
    {
        *x = MenuPTXY[id][0], *y = MenuPTXY[id][1];
    }
    else
    {
        *x = MenuXY[id][0], *y = MenuXY[id][1];
    }
}

void UiSubMenuGetLayout(int id, uint16_t *x, uint16_t *y)
{
    id = (id - 1) % 4;

    *x = SubMenuXY[id][0], *y = SubMenuXY[id][1];
}

//-----------------------------------------------------------------------------
// Name    : UIDefaultComposition
// Func    : The default menu layout funciont, displayed from left to right
// Paras   : id[in],     Menu index
//           pBuf[in],   Menu pixel data
//           w[in],      width
//           h[in],     height
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void UIDefaultComposition(int id, uint16_t *pBuf, uint16_t w, uint16_t h)
{
	switch (id)
	{
	case 01:
		Lib_LcdDispMemcpy(pBuf, 20, 79, w, h);
		break;

	case 0x02:
		Lib_LcdDispMemcpy(pBuf, 166, 79, w, h);
		break;

	case 0x03:
		Lib_LcdDispMemcpy(pBuf, 20, 117, w, h);
		break;

	case 0x04:
		Lib_LcdDispMemcpy(pBuf, 166, 117, w, h);
		break;

	case 0x05:
		Lib_LcdDispMemcpy(pBuf, 20, 155, w, h);
		break;

	case 0x06:
		Lib_LcdDispMemcpy(pBuf, 166, 155, w, h);
		break;

	case 0x07:
		Lib_LcdDispMemcpy(pBuf, 20, 193, w, h);
		break;

	case 0x08:
		Lib_LcdDispMemcpy(pBuf, 166, 193, w, h);
		break;

    default:
        break;
    }
}



//-----------------------------------------------------------------------------
// Name    : UIMenuSelect
// Func    : The default menu layout funciont, displayed from left to right
// Paras   : iTimeout[in],  timeout value, in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIMenuSelect(int iTimeout)
{
    short sRC;
    short Selected;
    unsigned char ucCurPage = 0;

    // Display menu items and wait for input
    while (1)
    {
        // Waiting for keyboard input selection
        sRC = UIWaitKey(iTimeout, 1);

        if (UI_INPUT_TIMEOUT == sRC)
        {
            return UI_INPUT_TIMEOUT;
        }

        switch (sRC)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            Selected = ((ucCurPage << 4) | (sRC & 0x0F));
            return Selected;

        case UI_INPUT_UP:
        case UI_INPUT_DOWN:
            return sRC;

        case UI_INPUT_CANCEL:
            return UI_INPUT_CANCEL;

        default:
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : ShowUiMenuPackY
// Func    : Menu Layout Function
// Paras   : pText[in],             Menu title string
//           ppcMenuInfo[in],       Menu structure data
//           ucTotalLines[in],      Total number of rows of the menu page
//           ucHighlightIndex[in],  Highlight line number
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
void ShowUiMenuPackY(char *pText, char **ppcMenuInfo, unsigned char ucTotalLines, unsigned char ucHighlightIndex)
{
    LvUiShowUiMenuPackY(pText, ppcMenuInfo, ucTotalLines, ucHighlightIndex, FALSE);

    return;
}

//-----------------------------------------------------------------------------
// Name    : UIUnsupport
// Func    : Display TIP_NOT_SUPPORT
// Paras   : timeout[in], timeout value, in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIUnsupport(int timeout)
{
    MessageBox(TIP_NOT_SUPPORT, MB_CENTER|MB_COLOR_RED, 3000);
    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : UITextSimplify
// Func    : Simplify the display of long text by displaying it on one line,
//           supporting the insertion of specific replacement characters at a certain position
// Paras   : pText[in],         Original text
//         : replacePosit[in],  The position of simplification from the original text,
//                              corresponding to the index of the original text string
//         : replaceTxt[in],    Replace character or string
// Ret     : No return
// Desc    : eg: pText              " SN: 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//               replacePosit       4
//               replacePosit       "..."
//               The original text was simplified as follows: " SN: ...GHIJKLMNOPQRSTUVWXYZ"
//           Note: After successful invocation, the original content of pText will change
//-----------------------------------------------------------------------------
void UITextSimplify(char *const pText, unsigned char replacePosit, const char *replaceTxt)
{
    const unsigned short maxShowCharCount = 25;
    unsigned short w, h, font, forecolor, backcolor;
    unsigned short len = 0;
    unsigned short replaceLen = 0;
    unsigned short offset = 0;
    unsigned char flag = 0;
    unsigned char simplyText[128] = {0};
    lv_point_t size;

    if (pText == NULL)
    {
        return;
    }

    len = strlen(pText);
    // Get the text length of the replacement character
    if (replaceTxt != NULL)
    {
        replaceLen = strlen(replaceTxt);
    }

    // If the position replaced by replacePost is longer than the original text or the parameter is
    // abnormal (not allowed to be larger than the defined cache, 3 is reserved because when there is
    // no substitute, it is replaced by... by default)
    if (replacePosit >= len || (replacePosit + replaceLen + 3) > sizeof(simplyText))
    {
        return;
    }

    // Get Lcd screen display parameters
    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);
    // the length not exceed one line, exit immediately after display

    lv_txt_get_size(&size, pText, Lib_LvglGetFont(font), 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    if (size.x <= TEXT_LVGL_WIDTH)
    {
        return;
    }

    // the length exceeds one line, copy to simplified text
    if (replacePosit)
    {
        memcpy(simplyText, pText, replacePosit);
    }
    TRACE_LOG("simplyText = %s", simplyText);
    // If exist simplified parameters
    if (replaceLen)
    {
        memcpy(simplyText + replacePosit, replaceTxt, replaceLen);
    }
    else
    {
        // if no simplified parameter default usage simplify
        replaceLen = 3;
        memcpy(simplyText + replacePosit, "...", replaceLen);
    }

    // Calculate the line width before the replacement character
    lv_txt_get_size(&size, (const char *)simplyText, Lib_LvglGetFont(font), 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    w = size.x;
    if (w > TEXT_LVGL_WIDTH)
    {
        return;
    }

    //At this point, the simplified string length is replacePost+replaceLen
    offset = replacePosit;
    if (len >= maxShowCharCount)
    {
        offset = len + offset - maxShowCharCount;
    }

    while (1)
    {
        // The offset position is illegal
        if (offset >= len || offset <= 0)
        {
            return;
        }

        // can be displayed in one line
        lv_txt_get_size(&size, pText + offset, Lib_LvglGetFont(font), 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        h = size.x;
        //TRACE_LOG("flag = %d, offset = %d, width = %d", flag, offset, w + h);
        if (h + w < TEXT_LVGL_WIDTH)
        {
            // last time multiple lines were needed to display
            if (flag == 2)
            {
                break;
            }
            flag = 1;
            offset--;
        }
        else
        {
            offset++;
            // The last time one line was displayed completely
            if (flag == 1)
            {
                break;
            }
            flag = 2;
        }
    }

    memcpy(simplyText + replacePosit + replaceLen, pText + offset, len - offset);
    len = strlen((char *)simplyText);
    memcpy(pText, simplyText, len);
    pText[len] = 0;
    //TRACE_LOG("offset = %d, width = %d", offset, Lib_LcdGetTextWidth((unsigned char *)pText, len, font));
}

extern lv_obj_t* gTextList[8];
//-----------------------------------------------------------------------------
// Name    : UITextAlign
// Func    : UI Universal Alignment Display Function
// Paras   : ucAlignMode[in],   Display alignment mode
//         : ucY[in],           Display starting line number
//         : ucMode[in],        Display mode, normal display reversed display
//         : pText[in],         Display string
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
void UITextAlign(unsigned char ucAlignMode, unsigned short ucY, unsigned char ucMode, const char *pText)
{
    unsigned short y;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    if (LvUiStatusBarIsExist())
    {
        if (ucY > 6)
        {
            FUN_IN;
            return;
        }
        y = LINE_HEIGTH * ucY;
    }
    else
    {
        if (ucY > 7)
        {
            FUN_IN;
            return;
        }
        y = LINE_HEIGTH * ucY + BAR_HEIGTH;
    }

    if (gTextList[ucY] != NULL)
    {
        lv_obj_del(gTextList[ucY]);
    }

    //Write codes screen_1_label_1
    gTextList[ucY] = lv_label_create(LvUiGetPageBody());
    stCtrlObjAttr.m_ucBidiFlag = TRUE;
    stCtrlObjAttr.m_stTextAttr.m_pcTextContent = (char *)pText;
    stCtrlObjAttr.m_stTextAttr.m_textLongMode = LV_LABEL_LONG_WRAP;
    stCtrlObjAttr.m_x = 0;
    stCtrlObjAttr.m_y = y;
    stCtrlObjAttr.m_usWidth = LCD_MAX_SEG;
    stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stTextAttr.m_pvTextFont = Lib_LvglGetFont(font);
//    stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace = 1;
//    stCtrlObjAttr.m_stTextAttr.m_usTextLineSpace = 1;
    switch (ucAlignMode)
    {
    case ALIGN_L:
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
        break;
    case ALIGN_M:
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_CENTER;
        break;
    case ALIGN_R:
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_RIGHT;
        break;
    default:
        stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_AUTO;
        break;
    }

    stCtrlObjAttr.m_stPadAttr.m_usTop = 1;
    stCtrlObjAttr.m_stPadAttr.m_usRight = 0;
    stCtrlObjAttr.m_stPadAttr.m_usBottom = 1;
    stCtrlObjAttr.m_stPadAttr.m_usLeft = 0;

    lv_point_t size;
    lv_txt_get_size(&size, pText,
        stCtrlObjAttr.m_stTextAttr.m_pvTextFont,
        stCtrlObjAttr.m_stTextAttr.m_usTextLetterSpace,
        stCtrlObjAttr.m_stTextAttr.m_usTextLineSpace,
        LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    TRACE_LOG("UITextAlign = %s %d %d", pText, size.x, size.y);

    if (size.y < LINE_HEIGTH)
    {
        stCtrlObjAttr.m_usHeight = LINE_HEIGTH * ((size.x + LCD_MAX_SEG - 1) / LCD_MAX_SEG);
    }
    else
    {
        stCtrlObjAttr.m_usHeight = LINE_HEIGTH * ((size.y + LINE_HEIGTH - 1) / LINE_HEIGTH);
    }

    LvSetCtrlObj(gTextList[ucY], &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    Lib_LvglTask();

    return;
}

//-----------------------------------------------------------------------------
// Name    : UIUpdataRect
// Func    : UI rectangle update display function
// Paras   : usStartX[in],   Starting coordinate in X direction, range: 0~319
//         : usStartY[in],   Starting coordinate in Y direction, range: 0~6
//         : usXsize[in],    X-direction width, range: 1~320
//         : usYsize[in],    Y-direction height, range: 1~7
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
void UIUpdataRect(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize)
{
    if ((usXsize == 0) || (usYsize == 0))
    {
        return;
    }

	if ((usStartX+usXsize > LCD_MAX_SEG) || (usStartY+usYsize > MAX_COM_LINES))
	{
        TRACE("UIUpdataRect= %d %d %d %d\r\n", usStartX, usStartY, usXsize, usYsize);
		return;
	}
    Lib_LcdRefresh();
}

typedef struct LvListNode
{
    int m_iChildIndex;
    lv_obj_t * m_pParent;
    struct LvListNode *m_pstPrior;
}LV_LIST_NODE;

#define LV_TYPE_CHILD_OBJ      0
#define LV_TYPE_LIST_OBJ       1

//-----------------------------------------------------------------------------
// Name    : UIClearRect
// Func    : UI rectangle clear display function
// Paras   : usStartX[in],   Starting coordinate in X direction, range: 0~319
//         : usStartY[in],   Starting coordinate in Y direction, range: 0~6
//         : usXsize[in],    X-direction width, range: 1~320
//         : usYsize[in],    Y-direction height, range: 1~7
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
void UIClearRect(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize)
{
    FUN_IN;
    LV_LIST_NODE *headNode = NULL;
    LV_LIST_NODE *curNode;
    LV_LIST_NODE *tempNode;
    lv_obj_t *page = NULL;
    lv_obj_t *childObj = NULL;
    lv_obj_t *curObj = NULL;
    lv_area_t coordsIn;
    lv_area_t coordsOut;
    int curChildIndex = 0;
    int childCnt = 0;
    int curObjType;
    int i;

    if ((usXsize == 0) || (usYsize == 0))
    {
        return;
    }

	if ((usStartX+usXsize > LCD_MAX_SEG) || (usStartY+usYsize > (MAX_COM_LINES+1)))
	{
        TRACE("UIUpdataRect= %d %d %d %d\r\n", usStartX, usStartY, usXsize, usYsize);
		return;
	}

    if (hDialog)
    {
        LvUiCloseDialog(hDialog);
    }
    hDialog = NULL;

    if (!LvUiStatusBarIsExist() && usStartX == 0 && usStartY == 1 && usXsize == LCD_MAX_SEG && usYsize == MAX_COM_LINES - 1)
    {
        LvUilCls();
        return;
    }

	usYsize += 1;

    //the coords out
    coordsOut.x1 = usStartX;
    coordsOut.x2 = usStartX + usXsize - 1;
    coordsOut.y1 = BAR_HEIGTH + (usStartY-1)*LINE_HEIGTH;
    coordsOut.y2 = BAR_HEIGTH + (usStartY-1)*LINE_HEIGTH + usYsize*(LINE_HEIGTH) - 1;

    TRACE("%d, %d, %d, %d", coordsOut.x1, coordsOut.x2, coordsOut.y1, coordsOut.y2);

    //init the queue head
    headNode = (LV_LIST_NODE *)Lib_malloc(sizeof(LV_LIST_NODE));
    if (headNode == NULL)
    {
        return;
    }
    TRACE("malloc:%p", headNode);
    headNode->m_pstPrior = NULL;
    headNode->m_iChildIndex = -1;
    headNode->m_pParent = NULL;
    curNode = headNode;

    page = LvUiGetPageBody();

    //the first node contains the page
    tempNode = (LV_LIST_NODE *)Lib_malloc(sizeof(LV_LIST_NODE));
    if (tempNode == NULL)
    {
        Lib_free(headNode);
        return;
    }
    TRACE("malloc:%p", tempNode);
    tempNode->m_pParent = page;
    tempNode->m_iChildIndex = lv_obj_get_child_cnt(page) - 1;
    tempNode->m_pstPrior = curNode;
    //record the current node
    curNode = tempNode;
    curObj = curNode->m_pParent;
    curChildIndex = curNode->m_iChildIndex;
    curObjType = LV_TYPE_LIST_OBJ;

    while (1)
    {
        //traverse the child obj from the end to the begin
        for (i = curChildIndex; i >= 0; i--)
        {
            childObj = lv_obj_get_child(curObj, i);
            lv_obj_get_coords(childObj, &coordsIn);
            //if the obj is inside the clear rectangle, then delete the obj and its child
            if (_lv_area_is_in(&coordsIn, &coordsOut, 0))
            {
                for (int j = 0; j < 8; j ++)
                {
                    if (childObj == gTextList[j])
                    {
                        gTextList[j] = NULL;
                    }
                }
                lv_obj_del(childObj);
                continue;
            }

            childCnt = lv_obj_get_child_cnt(childObj);
            //otherwise, if the obj has child
            if (childCnt != 0)
            {
                //record the pre-obj's position
                tempNode = (LV_LIST_NODE *)Lib_malloc(sizeof(LV_LIST_NODE));
                if (tempNode == NULL)
                {
                    for (; curNode != NULL; curNode = curNode->m_pstPrior)
                    {
                        Lib_free(curNode);
                    }
                    return;
                }
                TRACE("malloc:%p", tempNode);
                tempNode->m_pParent = curNode->m_pParent;
                tempNode->m_iChildIndex = i - 1;
                tempNode->m_pstPrior = curNode;
                curNode = tempNode;
                break;
            }
        }
        //if the obj has child, then search the child
        if (i >= 0)
        {
            curObj = childObj;
            curChildIndex = lv_obj_get_child_cnt(childObj) - 1;
            curObjType = LV_TYPE_CHILD_OBJ;
        }
        else
        {
            //if the obj has not child or all the child traverse done, get the pre-obj from the queue
            if (curObjType == LV_TYPE_LIST_OBJ)
            {
                //if curobj is obtained from list, then the remove the node
                if (curNode ->m_pstPrior != NULL)
                {
                    tempNode = curNode;
                    curNode = curNode ->m_pstPrior;
                    Lib_free(tempNode);
                    TRACE("free:%p", tempNode);
                }
                else
                {
                    //this branch indicates that all done
                    Lib_free(curNode);
                    TRACE("free:%p", curNode);
                    break;
                }
            }

            curObj = curNode->m_pParent;
            curChildIndex = curNode->m_iChildIndex;
            curObjType = LV_TYPE_LIST_OBJ;
        }
    }
}

//-----------------------------------------------------------------------------
// Name    : UISelectThemeColor
// Func    : Color Theme Configuration Function
// Paras   : themeColor[in],   themeColor, RGB565
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
short UISelectThemeColor(int themeColor)
{
    ST_COLOR_PARAM themePara = {0};
    short ret = RET_OK;

    ret = GetColorParam(&themePara, sizeof(themePara));
    if (ret != RET_OK)
    {
        return RET_ERR;
    }

    switch (themeColor)
    {
    case THEME_COLOR_BLUE:
        themePara.mainForeColor = BACK565_BLUE;
        themePara.menu1BackColor = BACK565_BLUE;
        themePara.tipsForeColor= BACK565_BLUE;
        break;
    case THEME_COLOR_ORANGE:
        themePara.mainForeColor = BACK565_ORANGE;
        themePara.menu1BackColor = BACK565_ORANGE;
        themePara.tipsForeColor= BACK565_ORANGE;
        break;
    case THEME_COLOR_RED:
        themePara.mainForeColor = BACK565_RED;
        themePara.menu1BackColor = BACK565_RED;
        themePara.tipsForeColor= BACK565_RED;
        break;
    default:
        return RET_ERR;
        break;
    }

    ret = SetColorParam(&themePara, sizeof(themePara));
    UiRefreshNow();

    return ret;
}

//-----------------------------------------------------------------------------
// Name    : UITextAlignEx
// Func    : UI Universal Alignment Display extended Function
// Paras   : ulEncoding[in],    Encoding format
//         : ucAlignMode[in],   Display alignment mode
//         : ucY[in],           Display starting line number
//         : ucMode[in],        Display mode, normal display reversed display
//         : pText[in],         Display string
// Ret     : No return
// Desc    :
//-----------------------------------------------------------------------------
void UITextAlignEx(unsigned long ulEncoding, unsigned char ucAlignMode, unsigned short ucY, unsigned char ucMode, const char *pText)
{
    unsigned long ulOldEncodingType, ulLen;

    ulLen = sizeof(unsigned long);
    Lib_GetInfo(E_CHARACTER_ENCODING_TYPE, &ulOldEncodingType, &ulLen);

    Lib_SetInfo(E_CHARACTER_ENCODING_TYPE, &ulEncoding, sizeof(unsigned long));
    UITextAlign(ucAlignMode, ucY, ucMode, pText);
    Lib_SetInfo(E_CHARACTER_ENCODING_TYPE, &ulOldEncodingType, sizeof(unsigned long));
}

short UILvglMenuStyle1(int timeout, LVGL_MENU *pLvglMenu)
{
    short ret = RET_OK;
    unsigned char i = 0;
    unsigned char curPage = 1;
    unsigned char maxPage = 0;
    unsigned char curShowSize = 0;
    unsigned char curMenuIndex = 0;
    unsigned char curIconPositionNum = 0;
    unsigned char positionX = 0;
    unsigned char positionY = 0;
    char dispBuff[48] = {0};
    t_lvgl_image img = { 0 };

    //Calculate the maximum number of pages
    maxPage = (pLvglMenu->menuSize - 1) / PAGE_MAX_SHOW_MENU_COUNT_LVGL + 1;

    while (1)
    {
        //clear the screen
        LvUilCls();

        //Display a background image
        if (pLvglMenu->mainBgImg)
        {
            img.bgImg = (char *)pLvglMenu->mainBgImg;
            img.text = NULL;
            LvUiShowImage(MENU_COMMON_BACKGROUND_OFF_X, MENU_COMMON_BACKGROUND_OFF_Y, &img, -1);
        }

        BUFCLR ( dispBuff );
        sprintf(dispBuff, "%s[%d/%d]", pLvglMenu->title, curPage, maxPage);

        if (pLvglMenu->titltBgImg)
        {
            img.bgImg = (char *)pLvglMenu->titltBgImg;
            img.text = NULL;
            LvUiShowImage(MENU_COMMON_TITLEBACKGROUND_OFF_X, MENU_COMMON_TITLEBACKGROUND_OFF_Y, &img, -1);
        }
        LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char*)dispBuff, lv_color_hex(0xFFFFFF));

        Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

        //Counts the number of icons on the currently displayed page
        curShowSize = PAGE_MAX_SHOW_MENU_COUNT_LVGL;
        if (curPage >= maxPage && (pLvglMenu->menuSize % PAGE_MAX_SHOW_MENU_COUNT_LVGL))
        {
            curShowSize = pLvglMenu->menuSize % PAGE_MAX_SHOW_MENU_COUNT_LVGL;
        }

        for (i = 0; i < curShowSize; i++)
        {
            //Subscript for the current menu
            curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_COUNT_LVGL + i;
            //The position number on the screen where the picture is currently displayed
            curIconPositionNum = curMenuIndex % PAGE_MAX_SHOW_MENU_COUNT_LVGL;

            //Calculate the position of x and y
            positionX = MENU_LEVEL_STYLE1_COLUMN_ONE_OFF_X + MENU_LEVEL_STYLE1_COLUMN_WIDTH * (curIconPositionNum % MENU_LEVEL_STYLE1_COLUMN_NUM);
            positionY = MENU_LEVEL_STYLE1_LINE_ONE_OFF_Y + MENU_LEVEL_STYLE1_LINE_WIDTH * (curIconPositionNum / MENU_LEVEL_STYLE1_COLUMN_NUM);

            img.bgImg = (char *)bgImgIcons[i];
            img.text = pLvglMenu->menuList[curMenuIndex].title;
            img.noImg = (char *)OPTION_ICON;
            img.icon = pLvglMenu->menuList[curMenuIndex].icon;

            LvUiShowImage(positionX, positionY, &img, i + '1');
        }

        //Get the key
        ret = LvUiWaitKey(timeout);

        if (ret == KB_CANCEL || ret == LVGL_OPER_TIMEOUT)
        {
            return ret;
        }
        else if (KB_UP == ret)
        {
           (curPage > 1) ? (curPage--) : (curPage = maxPage);
           continue;
        }
        else if (KB_DOWN == ret)
        {
           (curPage < maxPage) ? (curPage++) : (curPage = 1);
           continue;
        }
        else if (ret < KB_DIGIT_ONE || ret > curShowSize + KB_DIGIT_ZERO)
        {
           continue;
        }

        curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_COUNT_LVGL + ret - '0' - 1;
        if (pLvglMenu->menuList[curMenuIndex].MenuCall != NULL)
        {
            pLvglMenu->menuList[curMenuIndex].MenuCall(pLvglMenu->menuList[curMenuIndex].para);
        }
    }

    return ret;

}

short UILvglMenuStyle2(int timeout, LVGL_MENU *pLvglMenu)
{
    short ret = RET_OK;
    unsigned char i = 0;
    unsigned char curPage = 1;
    unsigned char maxPage = 0;
    unsigned char curShowSize = 0;
    unsigned char curMenuIndex = 0;
    signed char curIndex = -1;
    unsigned char positionX = 0;
    unsigned char positionY = 0;
    char dispBuff[48] = {0};
    t_lvgl_image img = { 0 };

    //Calculate the maximum number of pages
    maxPage = (pLvglMenu->menuSize - 1) / PAGE_MAX_SHOW_MENU_STYLE2 + 1;

    while (1)
    {
        //clear the screen
        LvUilCls();

        Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR);

        BUFCLR(dispBuff);
        sprintf(dispBuff, "%s[%d/%d]", pLvglMenu->title, curPage, maxPage);

        img.bgImg = (char *)pLvglMenu->titltBgImg;
        img.text = NULL;
        LvUiShowImage(MENU_COMMON_BACKGROUND_OFF_X, MENU_COMMON_BACKGROUND_OFF_Y, &img, -1);

        LvUiText(LVGL_ALIGN_CENTER, MENU_TITLE_COMMON_OFF_X, MENU_TITLE_COMMON_OFF_Y, FONT_SIZE_NORMAL, (char*)dispBuff, LV_COLOR_WHITE);

        //Counts the number of icons on the currently displayed page
        curShowSize = PAGE_MAX_SHOW_MENU_STYLE2;
        if (curPage >= maxPage && (pLvglMenu->menuSize % PAGE_MAX_SHOW_MENU_STYLE2))
        {
            curShowSize = pLvglMenu->menuSize % PAGE_MAX_SHOW_MENU_STYLE2;
        }

        for (i = 0; i < curShowSize; i++)
        {
            //Subscript for the current menu
            curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_STYLE2 + i;

            //Calculate the position of x and y
            positionX = MENU_LEVEL_STYLE2_COLUMN_ONE_OFF_X;
            positionY = MENU_LEVEL_STYLE2_LINE_ONE_OFF_Y + MENU_LEVEL_STYLE2_LINE_WIDTH * i;

            if (i == curIndex)
            {
                img.bgImg = (char *)MENU_STYLE2_BG1_ICON;
            }
            else
            {
                img.bgImg = (char *)MENU_STYLE2_BG0_ICON;
            }

            img.text = pLvglMenu->menuList[curMenuIndex].title;
            img.noImg = (char *)MENU_STYLE2_ICON;

            LvUiShowImage(positionX, positionY, &img, i + '1');
        }
        //Get the key
        ret = LvUiWaitKey(timeout);

        if (ret == KB_CANCEL || ret == LVGL_OPER_TIMEOUT)
        {
            return ret;
        }
        else if (KB_UP == ret)
        {
            curIndex--;

            if (curIndex < 0)
            {
                (curPage > 1) ? (curPage--) : (curPage = maxPage);
                if (curPage == maxPage)
                {
                    curIndex = pLvglMenu->menuSize - (curPage - 1) * PAGE_MAX_SHOW_MENU_STYLE2 - 1;
                }
                else
                {
                    curIndex = PAGE_MAX_SHOW_MENU_STYLE2 - 1;
                }
            }
            continue;
        }
        else if (KB_DOWN == ret)
        {
            curIndex++;

            if (curPage == maxPage)
            {
                if (maxPage > 1)
                {
                    if (curIndex >= (pLvglMenu->menuSize % PAGE_MAX_SHOW_MENU_STYLE2))
                    {
                        curPage = 1;
                        curIndex = 0;
                    }
                }
                else
                {
                    if (curIndex >= pLvglMenu->menuSize)
                    {
                        curIndex = 0;
                    }
                }
            }
            else
            {
                if (curIndex >= PAGE_MAX_SHOW_MENU_STYLE2)
                {
                    curPage++;
                    curIndex = 0;
                }
            }
            continue;
        }
        else if (ret == KB_ENTER)
        {
            if (curIndex >= 0)
            {
                curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_STYLE2 + curIndex;
                if (pLvglMenu->menuList[curMenuIndex].MenuCall != NULL)
                {
                    pLvglMenu->menuList[curMenuIndex].MenuCall(pLvglMenu->menuList[curMenuIndex].para);
                }
            }
            continue;
        }
        else if (ret < KB_DIGIT_ONE || ret > curShowSize + KB_DIGIT_ZERO)
        {
            continue;
        }

        curMenuIndex = (curPage - 1) * PAGE_MAX_SHOW_MENU_STYLE2 + + ret - KB_DIGIT_ONE;
        if (pLvglMenu->menuList[curMenuIndex].MenuCall != NULL)
        {
            pLvglMenu->menuList[curMenuIndex].MenuCall(pLvglMenu->menuList[curMenuIndex].para);
        }
    }

    return ret;

}

short UICreatLvglMenu(int timeout, LVGL_MENU *pLvglMenu)
{
    if (pLvglMenu == NULL)
    {
        return RET_ERR;
    }

    switch (pLvglMenu->style)
    {
    case LVGL_MENU_STYLE_1:
        return UILvglMenuStyle1(timeout, pLvglMenu);
    case LVGL_MENU_STYLE_2:
        return UILvglMenuStyle2(timeout, pLvglMenu);
    default:
        break;
    }

    return UILvglMenuStyle1(timeout, pLvglMenu);
}

//-----------------------------------------------------------------------------
// Name    : UISetParaByIpInput
// Func    : UI input to set ip parameter
// Paras   : ucBitMap[in],    ip type
//         : title[in],   shows on the top of the screen
//         : iTimeout[in],     timeOut in ms
// Desc    :
//-----------------------------------------------------------------------------
#define IP_SIZE               15
short UISetParaByIpInput(unsigned char ucBitMap, char *title, int iTimeout)
{
    int sRC = RET_OK;
    int value = 0;
    unsigned char ucParamLen = 0;
    char *pcP = NULL;
    char acParam[IP_SIZE + 1] = {0};
    char acDisplayBuff[22] = {0};
    char acParam1[IP_SIZE + 1];
    char textAreaTextBuf[10] = {0};
    char ipSegmentStringArray[4][4];
    char pDispBuf[100];
    lv_obj_t *group = NULL;
    lv_obj_t *body = NULL;
    lv_obj_t *label = NULL;
    lv_obj_t *textAreaContainer = NULL;
    lv_obj_t *textArea[4];
    lv_obj_t *curTextArea = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};
    int parentWidth;

    FUN_IN;

    switch (ucBitMap)
    {
    case 0:// DNS host
        pcP = TIP_DNS_MAIN;
        break;

    case 1:// DNS backup
        pcP = TIP_DNS_BACK;
        break;

    case HOST1IP_BIT:
        pcP = TIP_SERVER_IP_MAIN;
        break;

    case HOST2IP_BIT:
        pcP = TIP_SERVER_IP_BACK;
        break;

    case TMS1IP_BIT:
        pcP = TIP_TMS_SERVER_IP_MAIN;
        break;

    case TMS2IP_BIT:
        pcP = TIP_TMS_SERVER_IP_BACK;
        break;
    default:
        return RET_ERR;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    while (1)
    {
        if (body == NULL)
        {
            // TPDU (10 digit) string
            ucParamLen = IP_SIZE;
            if(ucBitMap == 0)
            {
                sRC = Lib_WlsGetDns(acParam, sizeof(acParam), acParam1, sizeof(acParam1));
            }
            else if(ucBitMap == 1)
            {
                sRC = Lib_WlsGetDns(acParam1, sizeof(acParam1), acParam, sizeof(acParam));
            }
            else
            {
                sRC = ServerParamGet(ucBitMap, (unsigned char*)acParam, &ucParamLen);
            }
            TRACE("UISetParaByIpInput %s,%s, ucBitMap = %d, sRC = %d",acParam,acParam1, ucBitMap, sRC);
            if (sRC != RET_OK)
            {
                ucParamLen = 0;
            }

            if (!ucParamLen)
            {
                sprintf(acDisplayBuff, "%s\n%s", pcP, TIP_NONE);
            }
            else
            {
                sprintf(acDisplayBuff, "%s\n%s", pcP, acParam);
            }

            body = LvUiCreateInputTextBody(FONT_SIZE_NORMAL, title, acDisplayBuff);

        }

        if (label == NULL)
        {
            label = lv_label_create(body);
            lv_obj_update_layout(body);
            parentWidth = lv_obj_get_width(body);
            memset((char*)&stCtrlObjAttr, 0x00, sizeof(stCtrlObjAttr));
            stCtrlObjAttr.m_ucBidiFlag = TRUE;
            stCtrlObjAttr.m_stTextAttr.m_pcTextContent = TIP_ENTER_TO_EDIT;
            stCtrlObjAttr.m_stTextAttr.m_color = (lv_color_t)forecolor;
            stCtrlObjAttr.m_usWidth = parentWidth - parentWidth / 8;
            stCtrlObjAttr.m_stTextAttr.m_ucTextAlign = LV_TEXT_ALIGN_LEFT;
            LvSetCtrlObj(label, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        sRC = LvUiWaitKey(iTimeout);
        if(sRC == KB_UP || sRC == KB_DOWN || sRC == KB_CANCEL || sRC == LVGL_OPER_TIMEOUT)
        {
            lv_obj_del(body);
            return sRC;
        }
        else if (sRC == KB_ENTER)
        {
            lv_obj_del(label);
            label = NULL;
            textAreaContainer = LvUiCreateInputIPTextArea(body, FONT_SIZE_TEXT_INPUT_IMFORMATION);
            textArea[0] = lv_obj_get_child(textAreaContainer, 0);
            textArea[1] = lv_obj_get_child(textAreaContainer, 2);
            textArea[2] = lv_obj_get_child(textAreaContainer, 4);
            textArea[3] = lv_obj_get_child(textAreaContainer, 6);
            group = lv_obj_get_group(textArea[0]);
            curTextArea = textArea[0];
            lv_group_focus_obj(curTextArea);
            lv_textarea_set_cursor_pos(curTextArea, 0);

            memset(ipSegmentStringArray, 0, sizeof(ipSegmentStringArray));
            sRC = Str2Ip(acParam, ipSegmentStringArray[0], ipSegmentStringArray[1], ipSegmentStringArray[2], ipSegmentStringArray[3]);

            for (int i = 0; i < 4; i++)
            {
                if (sRC == RET_OK && (ucBitMap == HOST1IP_BIT || ucBitMap == HOST2IP_BIT || ucBitMap == TMS1IP_BIT || ucBitMap == TMS2IP_BIT))
                {
                    lv_textarea_set_text(textArea[i], ipSegmentStringArray[i]);
                }
                else
                {
                    lv_textarea_set_text(textArea[i], "0");
                }
            }

            while (1)
            {
                memset(textAreaTextBuf, 0, sizeof(textAreaTextBuf));
                strcpy(textAreaTextBuf, lv_textarea_get_text(curTextArea));
                value = atoi(textAreaTextBuf);

                //the first num can not be zero
                if (strlen(textAreaTextBuf) != 0 && value == 0)
                {
                    lv_textarea_del_char(curTextArea);
                }

                //num can not exceed 255
                if (value > 255)
                {
                    sprintf((char*)acDisplayBuff, "%s %d!", TIP_PARA_CANNOT_MORE, 255);
                    LvUiMessageBox(acDisplayBuff, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, "TIP", 1000);
                    lv_textarea_del_char(curTextArea);
                }

                sRC = LvUiWaitKey(iTimeout);
                if (sRC == KB_CANCEL)
                {
                    lv_obj_del(textAreaContainer);
                    break;
                }
                else if (sRC == LVGL_OPER_TIMEOUT)
                {
                    lv_obj_del(body);
                    return sRC;
                }
                else if (sRC == KB_UP)
                {
                    if (lv_textarea_get_cursor_pos(curTextArea) == 0)
                    {
                        lv_group_focus_prev((lv_group_t *)group);
                        curTextArea = lv_group_get_focused((const lv_group_t *)group);
                        lv_textarea_set_cursor_pos(curTextArea, strlen(lv_textarea_get_text(curTextArea)));
                    }
                    else
                    {
                        lv_textarea_cursor_left(curTextArea);
                    }
                    continue;
                }
                else if (sRC == KB_DOWN)
                {
                    if (lv_textarea_get_cursor_pos(curTextArea) == strlen(lv_textarea_get_text(curTextArea)))
                    {
                        lv_group_focus_next((lv_group_t *)group);
                        curTextArea = lv_group_get_focused((const lv_group_t *)group);
                        lv_textarea_set_cursor_pos(curTextArea, 0);
                    }
                    else
                    {
                        lv_textarea_cursor_right(curTextArea);
                    }
                    continue;
                }
                else if (sRC == KB_ENTER)
                {

                    if (strlen(lv_textarea_get_text(textArea[0])) == 0 ||
                        strlen(lv_textarea_get_text(textArea[1])) == 0 ||
                        strlen(lv_textarea_get_text(textArea[2])) == 0 ||
                        strlen(lv_textarea_get_text(textArea[3])) == 0)
                    {
                        continue;
                    }

                    sprintf(acParam, "%s.%s.%s.%s", lv_textarea_get_text(textArea[0]),
                                                    lv_textarea_get_text(textArea[1]),
                                                    lv_textarea_get_text(textArea[2]),
                                                    lv_textarea_get_text(textArea[3]));
                    ucParamLen = strlen(acParam);
                    if ((ucParamLen < 6) || (ucParamLen > IP_SIZE))
                    {
                        if (0 == ucParamLen)
                        {
                            LvUiMessageBox(TIP_PARA_CANNOT_EMPTY, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, "TIP", 3000);
                        }
                        else
                        {
                            LvUiMessageBox(TIP_PARA_LEN_ERR, MB_CENTER | MB_COLOR_RED | MB_ICONERROR | MB_ANYKEY, "TIP", 3000);
                        }
                        continue;
                    }
                    else
                    {
        //              sRC = TerminalParamSet(ucBitMap, (unsigned char*)acDisplayBuff, ucInputLen);
                        if(ucBitMap == 0)
                        {
                            sRC = Lib_WlsSetDns((unsigned char *)acParam, (unsigned char *)acParam1);
                        }
                        else if(ucBitMap == 1)
                        {
                            sRC = Lib_WlsSetDns((unsigned char *)acParam1, (unsigned char *)acParam);
                        }
                        else
                        {
                            sRC = ServerParamSet(ucBitMap, (unsigned char*)acParam, ucParamLen);
                        }

                        if (sRC < 0)
                        {
                            LvUiMessageBox(TIP_SET_FAIL, MB_COLOR_RED | MB_CENTER | MB_NOKEY, "TIP", 3000);
                            return sRC;
                        }

                        BUFCLR(pDispBuf);
                        strcat(pDispBuf, pcP);
                        strcat(pDispBuf, "\n");
                        strcat(pDispBuf, acParam);
                        LvUiMessageBox(pDispBuf, MB_COLOR_GREEN | MB_CENTER | MB_NOKEY, "TIP", 3000);
                        lv_obj_del(body);
                        body = NULL;
                        break;
                    }
                }
                else if (sRC == KB_CLEAR)
                {
                    curTextArea = lv_group_get_focused((const lv_group_t *)group);
                    lv_textarea_del_char(curTextArea);
                    continue;
                }
                else if (sRC >= KB_DIGIT_ZERO && sRC <= KB_DIGIT_NINE)
                {
                    curTextArea = lv_group_get_focused((const lv_group_t *)group);
                    lv_textarea_add_char(curTextArea, sRC);
                    continue;
                }
                else
                {
                    continue;
                }
            }
        }
        else
        {
            continue;
        }
    }
}

/*duplicate SDK ui function*/
int UILcdDrawAreaLvgl(unsigned short color, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
{
    FUN_IN;
    lv_obj_t *page = NULL;
    lv_obj_t *area = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    page = LvUiGetPageBody();

    area = lv_obj_create(page);

    if ((x + w > LCD_W) || (y + h > LCD_H))
    {
        TRACE("DispMemset error %x,%x",x + w, y + h);
        return RET_ERR;
    }

    if (LvUiStatusBarIsExist() && y - BAR_HEIGTH >= 0)
    {
        y -= BAR_HEIGTH;
    }

    stCtrlObjAttr.m_x = x;
    stCtrlObjAttr.m_y = y;
    stCtrlObjAttr.m_usWidth = w;
    stCtrlObjAttr.m_usHeight = h;
    stCtrlObjAttr.m_stBgAttr.m_color = (lv_color_t)color;
    stCtrlObjAttr.m_stBgAttr.m_ucOpa = LV_OPA_100;
    LvSetCtrlObj(area, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);

    return RET_OK;
}

void UILcdClrScreenLvgl(void)
{
    FUN_IN;
    LvUilCls();
}

void UIClrScreenLvgl(void)
{
    FUN_IN;
    LvUilCls();
}


#define SETP   				1
#define CLEARP 				0

void LcdDisSingleRecentLvgl(unsigned short x,unsigned short y,unsigned short width ,unsigned short hight ,unsigned char Action)
{
    unsigned short w, h, font, forecolor, backcolor;

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    FUN_IN;
    if ((x + width/2 - hight/2 < 0) || (y - width/2 + hight/2 < 0))
    {
        if (Action == CLEARP)
        {
            UILcdClrScreenLvgl();
        }
        else
        {
            TRACE("parameter setting error\r\n");
        }
    }
    else
    {
        if (Action == SETP)
        {
            //Draw horizontal and vertical lines
            UILcdDrawAreaLvgl(forecolor, x, y, width, hight);
            UILcdDrawAreaLvgl(forecolor, x + width/2 - hight/2, y - width/2 + hight/2, hight, width);
        }
        else
        {
            UILcdDrawAreaLvgl(RGB565_WHITE, x, y, width, hight);
            UILcdDrawAreaLvgl(RGB565_WHITE, x + width/2 - hight/2, y - width/2 + hight/2, hight, width);
        }
    }
}

static void line_delete_cb(lv_event_t *e)
{
    lv_point_t *pts = lv_event_get_user_data(e);
    if (pts)
    {
        Lib_free(pts);
    }
}

//-----------------------------------------------------------------------------
// Name    : UIDrawLine
// Func    : UI draw line
// Paras   : ucStartX[in],  range in 0~319
//         : ucStartY[in],  range in 0~239
// Desc    :
//-----------------------------------------------------------------------------
void UIDrawLineLvgl(unsigned short usX1, unsigned short usY1, unsigned short usX2, unsigned short usY2)
{
    //when the line width is larger than 1, scale up first and scale down second in lvgl
    FUN_IN;
    int lineWidth = 1;
    unsigned short w, h, font, forecolor, backcolor;
    lv_obj_t *page = NULL;
    lv_obj_t *line = NULL;
    lv_point_t *linePointArray = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if ((usX1 > LCD_MAX_SEG-1) || (usX2 > LCD_MAX_SEG-1))
    {
        FUN_IN_ERR;
        TRACE("%d, %d", usX1, usX2);
        return;
    }

    if ((usY1 > LCD_MAX_COM-1) || (usY2 > LCD_MAX_COM-1))
    {
        FUN_IN_ERR;
        TRACE("%d, %d", usY1, usY2);
        return;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();

    linePointArray = (lv_point_t *)Lib_malloc(sizeof(lv_point_t) * 2);
    if (linePointArray == NULL)
    {
        FUN_IN_ERR;
        return;
    }

    //only support draw horizontal and vertical lines
    linePointArray[0].x = 0;
    linePointArray[0].y = 0;
    if (usX2 - usX1 == 0)
    {
        linePointArray[1].x = 0;
        linePointArray[1].y = usY2 - usY1;
    }
    else if (usY2 - usY1 == 0)
    {
        linePointArray[1].x = usX2 - usX1;
        linePointArray[1].y = 0;
    }
    else
    {
        if (usX2 - usX1 < usY2 - usY1)
        {
            linePointArray[1].x = 0;
            linePointArray[1].y = usY2 - usY1;
            lineWidth = usX2 - usX1;
        }
        else
        {
            linePointArray[1].x = usX2 - usX1;
            linePointArray[1].y = 0;
            lineWidth = usY2 - usY1;
        }
    }

    line = lv_line_create(page);

    stCtrlObjAttr.m_stLineAttr.m_pLinePointArray = linePointArray;
    stCtrlObjAttr.m_stLineAttr.m_usPointNum = 2;
    TRACE("%d, %d", LvUiStatusBarIsExist(), usY1);
    if (LvUiStatusBarIsExist() && usY1 - BAR_HEIGTH >= 0)
    {
        stCtrlObjAttr.m_x = usX1;
        stCtrlObjAttr.m_y = usY1 - BAR_HEIGTH;
    }
    else
    {
        stCtrlObjAttr.m_x = usX1;
        stCtrlObjAttr.m_y = usY1;
    }

    stCtrlObjAttr.m_stLineAttr.m_usLineWidth = lineWidth;
    stCtrlObjAttr.m_stLineAttr.m_lineColor = (lv_color_t)forecolor;
    stCtrlObjAttr.m_stLineAttr.m_ucLineOpa = LV_OPA_100;

    LvSetCtrlObj(line, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(line, line_delete_cb, LV_EVENT_DELETE, linePointArray);
}

static void canvas_delete_cb(lv_event_t * e)
{
    unsigned char *buf = lv_event_get_user_data(e);
    if (buf)
    {
        Lib_free(buf);
    }
}

void UIIconLvgl(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char ucMode, unsigned char *pucIcon)
{
    unsigned short usCnt = 0;
    unsigned short *imageVarBuf = NULL;
    lv_obj_t *page = NULL;
    lv_obj_t *canvas = NULL;
    unsigned short w, h, font, forecolor, backcolor;
    lv_color_t pageColor;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if ((usXsize == 0) || (usYsize == 0))
    {
        FUN_IN_ERR;
        return;
    }

    if (NULL == pucIcon)
    {
        FUN_IN_ERR;
        return;
    }

    if ((usStartX+usXsize > LCD_MAX_SEG) || (usStartY+usYsize > LCD_MAX_COM))
    {
        FUN_IN_ERR;
        return;
    }

    if ((usXsize == 0) || (usYsize == 0))
    {
        FUN_IN_ERR;
        return;
    }

    Lib_LcdParaGet(&w, &h, &font, &forecolor, &backcolor);

    page = LvUiGetPageBody();
    pageColor = lv_obj_get_style_bg_color(page, LV_PART_MAIN);

    canvas = lv_canvas_create(page);

    //The effective data length of the dot matrix data is width * (height/8), and the dot matrix is
    //arranged vertically for each byte (i.e., the first 320 bytes of data are 320 * 8 dot matrix data)
    //If the height of the output image is >8, one more row of dot matrix data (320*8) will be added.
    imageVarBuf = (unsigned short *)Lib_malloc(usXsize * usYsize * 2);

    //Iterate through the incoming array of dots
    for (int j = 0; j < usYsize; j++)
    {
        for (int i = 0; i < usXsize; i++)
        {
            //The traversal order is the first bit of the first byte of the dot matrix data
            //corresponding to the first point of the output data, and the traversal is transversely sequential
            if ((pucIcon[(j / 8) * usXsize + i] & (0x1 << (j % 8))) == 0)
            {
                imageVarBuf[usCnt++] = pageColor.full;
            }
            else
            {
               imageVarBuf[usCnt++] = forecolor;
            }
        }
    }

    stCtrlObjAttr.m_stCanvasAttr.m_pvCanvasBuf = imageVarBuf;
    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufWidth = usXsize;
    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufHeight = usYsize;
    stCtrlObjAttr.m_stCanvasAttr.m_imgCf = LV_IMG_CF_RGB565;

    if (LvUiStatusBarIsExist() && usStartY - BAR_HEIGTH >= 0)
    {
        stCtrlObjAttr.m_x = usStartX;
        stCtrlObjAttr.m_y = usStartY - BAR_HEIGTH;
    }
    else
    {
        stCtrlObjAttr.m_x = usStartX;
        stCtrlObjAttr.m_y = usStartY;
    }
    stCtrlObjAttr.m_usWidth = usXsize;
    stCtrlObjAttr.m_usHeight = usYsize;

    LvSetCtrlObj(canvas, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(canvas, canvas_delete_cb, LV_EVENT_DELETE, imageVarBuf);
}


void UIClearAllScreenLvgl(void)
{
    FUN_IN;
    LvUilCls();
}

unsigned char UIGetKeyWithTaskHandlerLvgl(void)
{
    unsigned char ret;

    ret = LvGetKey();

    return ret;
}

void UIDisPlayTimerReadyLvgl(void)
{
    lv_disp_t * disp_def_tmp = NULL;

    disp_def_tmp = lv_disp_get_default();

    lv_timer_ready(disp_def_tmp->refr_timer);
}

int UIGetTextWidthLvgl(unsigned char *pText, unsigned short TextLen, unsigned short FontTextEn)
{
    lv_point_t size;
    int len;
    char *buf = NULL;
    char *newBuf = NULL;

    buf = Lib_malloc(TextLen + 1);
    if (buf == NULL)
    {
        return RET_ERR_OUT_OF_MEMORY;
    }

    memcpy(buf, pText, TextLen);
    buf[TextLen] = 0;

    if (FARSI_ID == GetLanguageType())
    {
        len = _lv_txt_ap_calc_bytes_cnt(buf);

        newBuf = Lib_malloc(len);

        _lv_txt_ap_proc(buf, newBuf);
    }

    if (newBuf)
    {
        lv_txt_get_size(&size, newBuf, Lib_LvglGetFont(FontTextEn), 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        Lib_free(newBuf);
    }
    else
    {
        lv_txt_get_size(&size, buf, Lib_LvglGetFont(FontTextEn), 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
    }

    Lib_free(buf);

    return size.x;
}

void UICreateImgByArrayData(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucData)
{
    unsigned short *imageVarBuf = NULL;
    lv_obj_t *page = NULL;
    lv_obj_t *canvas = NULL;
    CTRL_OBJ_ATTR stCtrlObjAttr = {0};

    if ((usXsize == 0) || (usYsize == 0))
    {
        FUN_IN_ERR;
        return;
    }

    if (NULL == pucData)
    {
        FUN_IN_ERR;
        return;
    }

    if ((usStartX+usXsize > LCD_MAX_SEG) || (usStartY+usYsize > LCD_MAX_COM))
    {
        FUN_IN_ERR;
        return;
    }

    if ((usXsize == 0) || (usYsize == 0))
    {
        FUN_IN_ERR;
        return;
    }

    page = LvUiGetPageBody();
    canvas = lv_canvas_create(page);

    imageVarBuf = (unsigned short *)Lib_malloc(usXsize * usYsize * 2);

    memcpy(imageVarBuf, pucData, usXsize * usYsize * 2);

    stCtrlObjAttr.m_stCanvasAttr.m_pvCanvasBuf = imageVarBuf;
    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufWidth = usXsize;
    stCtrlObjAttr.m_stCanvasAttr.m_usCanvasBufHeight = usYsize;
    stCtrlObjAttr.m_stCanvasAttr.m_imgCf = LV_IMG_CF_RGB565;

    if (LvUiStatusBarIsExist() && usStartY - BAR_HEIGTH >= 0)
    {
        stCtrlObjAttr.m_x = usStartX;
        stCtrlObjAttr.m_y = usStartY - BAR_HEIGTH;
    }
    else
    {
        stCtrlObjAttr.m_x = usStartX;
        stCtrlObjAttr.m_y = usStartY;
    }
    stCtrlObjAttr.m_usWidth = usXsize;
    stCtrlObjAttr.m_usHeight = usYsize;

    LvSetCtrlObj(canvas, &stCtrlObjAttr, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(canvas, canvas_delete_cb, LV_EVENT_DELETE, imageVarBuf);
}


void UIShowXCodeLvglProcess1(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucIcon)
{
    FUN_IN;
    UIIconLvgl(usStartX, usStartY, usXsize, usYsize, 0, pucIcon);
}

void UIShowXCodeLvglProcess2(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucData)
{
    FUN_IN;
    UICreateImgByArrayData(usStartX, usStartY, usXsize, usYsize, pucData);
}

