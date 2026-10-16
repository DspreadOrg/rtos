/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : Input.c
 *  Version      :
 *  Author       :
 * Description   : Implement of input apis related letters and special characters on menu
 **********************************************************************************************************/

#include "Input.h"
#include "Common.h"
#include "LibApi.h"
#include "Drv.h"
#include "Key.h"

#define DEBUG_UI_INPUT    0
#define UI_INPUT_MAX_TIMEOUT    (0x7FFFFFFF)
#define UI_INPUT_WINDOW_WIDTH    128



const char *s_pacKeypadMap[]=
{
	"11 2Q 3q 4Z 5z      ",		//1
	"12 2A 3a 4B 5b 6C 7c",		//2
	"13 2D 3d 4E 5e 6F 7f",		//3
	"14 2G 3g 4H 5h 6I 7i",
	"15 2J 3j 4K 5k 6L 7l",
	"16 2M 3m 4N 5n 6O 7o",
	"17 2P 3p 4R 5r 6S 7s",
	"18 2T 3t 4U 5u 6V 7v",
	"19 2W 3w 4X 5x 6Y 7y",
};


// Special symbols
const char *s_pacKey0Map[]=
{
#if 1
	"10 2. 3! 4@ 5# 6  7$",
	"1^ 2& 3* 4# 5( 6) 7-",
	"1_ 2= 3+ 4[ 5{ 6] 7}",
	"1; 2: 3? 4| 5, 6< 7~",
	"1> 2/ 3` 4% 5\\ 6\" 7\'",
#else
	"10 2. 3! 4@ 5# 6$ 7%%",
	"1^ 2& 3* 4# 5( 6) 7-",
	"1_ 2= 3+ 4[ 5{ 6] 7}",
 	"1; 2: 3\\ 4| 5, 6< 7~",
 	"1> 2/ 3? 4\" 5`      ",
#endif
};


// Character input, support special symbols or not? 0- indicates not supported, 1- indicates supported
static unsigned char s_ucAscF = 0;
//-----------------------------------------------------------------------------
// Name    : KbWaitZM
// Func    : Get key input (only supports letters and numbers), exit after timeout
// Paras   : pucKey, Key values to be read
//           ucClewY, Y-coordinate of prompt, 2-4
// Ret     : KB_RETURN_TIMEOUT
//           KB_RETURN_OK
//           KB_RETURN_ERR
// Desc    :
//-----------------------------------------------------------------------------
signed short KbWaitZM(unsigned char *pucKey, int iTimeout, unsigned char ucClewY)
{
	unsigned char ucKey, ucKeyLast;
	unsigned char ucOffset, ucKey0PageOff=0;
	unsigned char ucInputState=0;
	unsigned long ulStartTime=0;


	if (0 == iTimeout)
	{
		return KB_RETURN_TIMEOUT;
	}

	if ((ucClewY<2)||(ucClewY>MAX_COM_LINES-1))
	{
		return KB_RETURN_ERR;
	}

	ucKeyLast = KB_NULL;
	ulStartTime = Lib_SysGetTimer();
	while(1)
	{
		Lib_WDGReloadCounter();

		// Read key inputs
		ucKey = KB_NULL;
		ucKey = KbReadKey();
		if (KB_NULL == ucKey)
			continue;

		if ((ucKey>KB_DIGIT_NINE)&&((KB_UP!=ucKey)&&(KB_DOWN!=ucKey)))
		{
			*pucKey = ucKey;
			UIClearRect(0, ucClewY, LCD_MAX_SEG, 1);
			return KB_RETURN_ERR;
		}

		// In the initial state, press the number key to immediately display the letter extension line
		if (0 == ucInputState)
		{
			if (KB_DIGIT_ONE == ucKey)
			{
				UITextAlign(ALIGN_L, ucClewY, DISP_MODE_NORMAL, s_pacKeypadMap[0]);
				//UIText(0, ucClewY, DISP_MODE_DISXOR, " _  _  _  _  _");

				ucInputState = 1;
				ucKeyLast = ucKey;
			}
			else if((ucKey >= KB_DIGIT_TWO)&&(ucKey <= KB_DIGIT_NINE))
			{
				UITextAlign(ALIGN_L, ucClewY, DISP_MODE_NORMAL, s_pacKeypadMap[ucKey-'0'-1]);
				//UIText(0, ucClewY, DISP_MODE_DISXOR, " _  _  _  _  _  _  _");

				ucInputState = 1;
				ucKeyLast = ucKey;
			}
			else if(KB_DIGIT_ZERO == ucKey)
			{
				if (s_ucAscF)
				{
					// Support special symbol input
					UITextAlign(ALIGN_L, ucClewY, DISP_MODE_NORMAL, s_pacKey0Map[0]);

					ucInputState = 1;
					ucKeyLast = ucKey;
					ucKey0PageOff = 0;
				}
				else
				{
					// Not supported, directly output a 0
					*pucKey = '0';
					return KB_RETURN_OK;
				}
			}
		}
		else if (1 == ucInputState)
		{
            // Select letter extension rows
			//if((ucKey >= KB_DIGIT_ZERO)&&(ucKey <= KB_DIGIT_SEVEN))
			if((ucKey >= KB_DIGIT_ONE)&&(ucKey <= KB_DIGIT_SEVEN))
			{
				if (KB_DIGIT_ONE == ucKeyLast)
				{
					if((ucKey >= KB_DIGIT_ONE)&&(ucKey <= KB_DIGIT_FIVE))
					{
						ucOffset = 1 + 3*(ucKey-'0'-1);
						*pucKey = s_pacKeypadMap[0][ucOffset];

						UIClearRect(0, ucClewY, LCD_MAX_SEG, 1);
						return KB_RETURN_OK;
					}
				}
				else if((ucKeyLast >= KB_DIGIT_TWO)&&(ucKeyLast <= KB_DIGIT_NINE))
				{
					ucOffset = 1 + 3*(ucKey-'0'-1);
					*pucKey = s_pacKeypadMap[ucKeyLast-'0'-1][ucOffset];

					UIClearRect(0, ucClewY, LCD_MAX_SEG, 1);
					return KB_RETURN_OK;
				}
				else if(KB_DIGIT_ZERO == ucKeyLast)
				{
					// Special treatment on the fourth page of symbols
					if(4 == ucKey0PageOff)
					{
						if (ucKey <= KB_DIGIT_SEVEN)
						{
							ucOffset = 1 + 3*(ucKey-'0'-1);
							*pucKey = s_pacKey0Map[ucKey0PageOff][ucOffset];

							UIClearRect(0, ucClewY, LCD_MAX_SEG, 1);
							return KB_RETURN_OK;
						}
					}
					else
					{
						ucOffset = 1 + 3*(ucKey-'0'-1);
						*pucKey = s_pacKey0Map[ucKey0PageOff][ucOffset];

						UIClearRect(0, ucClewY, LCD_MAX_SEG, 1);
						return KB_RETURN_OK;
					}
				}
			}
			else if ((KB_UP == ucKey) || (KB_DOWN == ucKey))
			{
				// Only special characters support flipping up and down
				if (KB_DIGIT_ZERO == ucKeyLast)
				{
					// Press up and down keys to flip pages
					if (KB_UP == ucKey)
					{
						if (ucKey0PageOff)
						{
							ucKey0PageOff--;
						}
						else
						{
							ucKey0PageOff = 4;
						}

						UITextAlign(ALIGN_L, ucClewY, DISP_MODE_NORMAL, s_pacKey0Map[ucKey0PageOff]);
					}
					else if (KB_DOWN == ucKey)
					{
						ucKey0PageOff++;

						if (ucKey0PageOff >= 5)
						{
							ucKey0PageOff = 0;
						}

						UITextAlign(ALIGN_L, ucClewY, DISP_MODE_NORMAL, s_pacKey0Map[ucKey0PageOff]);
					}
				}
			}
		}

		if (IS_TIME_OUT(ulStartTime, iTimeout))
		{
			return KB_RETURN_TIMEOUT;
		}
	}
}


//-----------------------------------------------------------------------------
// Name    : KbWaitAsc
// Func    : Get key input (supports letter+special symbol input), exit after timeout
// Paras   : pucKey   : Key values to be read
//           iTimeout : Time out, in ms
//           ucClewY  : Y-coordinate of prompt, 2-4
// Ret     : KB_RETURN_TIMEOUT
//           KB_RETURN_OK
//           KB_RETURN_ERR
// Desc    :
//-----------------------------------------------------------------------------
signed short KbWaitAsc(unsigned char *pucKey, int iTimeout, unsigned char ucClewY)
{
    short sRC;

    // Support input of special characters
    s_ucAscF = 1;
    sRC = KbWaitZM(pucKey, iTimeout, ucClewY);
    //Restore
    s_ucAscF = 0;

    return sRC;
}

