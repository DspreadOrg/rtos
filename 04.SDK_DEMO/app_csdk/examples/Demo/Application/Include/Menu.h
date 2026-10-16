/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : menu.h
 *  Version      :
 *  Author       :
 * Description   : Menu function implementation
 **********************************************************************************************************/

#ifndef __MENU_H__
#define __MENU_H__

#include "LibApi.h"

//-----------------------------------------------------------------------------
// type definition
//-----------------------------------------------------------------------------

typedef struct tagOperator
{
    int status;
    int mode;
    char plmn[8];
    char shortname[16];
    char longname[64];
    char view[32];
}t_OperatorTable;

typedef struct
{
    unsigned char ucTest;
    unsigned long iTimeout;
    unsigned long ulType;
} BATTestPara;

typedef enum
{
    E_PRINTER_GRADE,
    E_BRIGHTNESS,
}E_SliderFuntionType;

#define BATTERY_CHARGE_TEST                         1
#define BATTERY_DISCHARGE_TEST                      2

#define MENU_COMMON_BACKGROUND_OFF_X                0
#define MENU_COMMON_BACKGROUND_OFF_Y                0
#define MENU_COMMON_TITLEBACKGROUND_OFF_X           0
#define MENU_COMMON_TITLEBACKGROUND_OFF_Y           0
#define MENU_TITLE_COMMON_OFF_X                     0
#define MENU_TITLE_COMMON_OFF_Y                     0

#define MENU_LEVEL_ONE_COLUMN_ONE_OFF_X             7
#define MENU_LEVEL_ONE_COLUMN_TWO_OFF_X             111
#define MENU_LEVEL_ONE_COLUMN_THREE_OFF_X           215
#define MENU_LEVEL_ONE_LINE_ONE_OFF_Y               7
#define MENU_LEVEL_ONE_LINE_TWO_OFF_Y               85
#define MENU_LEVEL_ONE_LINE_THREE_OFF_Y             163

#define MENU_LEVEL_STYLE1_COLUMN_ONE_OFF_X          12
#define MENU_LEVEL_STYLE1_LINE_ONE_OFF_Y            45
#define MENU_LEVEL_STYLE1_COLUMN_WIDTH              152
#define MENU_LEVEL_STYLE1_LINE_WIDTH                62
#define MENU_LEVEL_STYLE1_COLUMN_NUM                2
#define MENU_LEVEL_STYLE1_SUBICON_OFF_X             23
#define MENU_LEVEL_STYLE1_SUBICON_OFF_Y             10

#define MENU_LEVEL_STYLE2_COLUMN_ONE_OFF_X           12
#define MENU_LEVEL_STYLE2_LINE_ONE_OFF_Y             45
#define MENU_LEVEL_STYLE2_LINE_WIDTH                 38

#define ICON_DEBUG_X                                50
#define ICON_SPK_X                                  250

//-----------------------------------------------------------------------------
// variable declaration
//-----------------------------------------------------------------------------

extern t_OperatorTable s_OperatorList[16];


//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

short UICommParamSet(int iTimeout);
short UIICCardPayMenu(int iTimeout);
void UIShowStatusBar(unsigned char ucY);
short UIOperatorList(int iTimeout);
short UISetHostAddr(char *pInT, unsigned char ucBitMap, int iTimeout);
short UISelfCheckMenu(int timeout);
void UILabelBrightValue(unsigned char ucMax);
void UISlideBrightValue(unsigned char ucValue, unsigned char ucMax);
int UIFuntionSlider(char *title, unsigned char ucMax, E_SliderFuntionType funtionType, int iTimeout);
short UISetParaByTextAreaInfoInput(char *title, char *pInT, unsigned char ucBitMap, unsigned char infoMode, unsigned char infoMax, int iTimeout);

int SystemInit(void);

void LvHomeMenu(void *p);
void mainMenu(int iTimeout);
short UIEnterGprsModeCfg(int timeout);

#endif

