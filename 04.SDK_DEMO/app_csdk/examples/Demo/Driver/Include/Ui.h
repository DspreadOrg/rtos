/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Ui.h
 *  Version      :
 *  Author       :
 * Description   : Implement of display related functions
 **********************************************************************************************************/

#ifndef __UI_H__
#define __UI_H__
#include "LibApi.h"
#include "ParamManage.h"

//---------------------------------------------------------------------------
// macro definition
//---------------------------------------------------------------------------

// LCD resolution
#define LCD_MAX_SEG                     320
#define LCD_MAX_COM                     240
#define LCD_W                           LCD_MAX_SEG
#define LCD_H                           LCD_MAX_COM

#define ESIGN_WIDTH                     (LCD_MAX_SEG)
#define ESIGN_HEIGHT                    64

#define TEXT_LVGL_WIDTH                 280

#define FONT_HZ_WIDTH                   24
#define FONT_ASCII_WIDTH                12
// The maximum number of characters displayed in one line is 20, considering the widest character
#define LCD_LINE_CHAR_ML                20
// The height displayed in the first line status bar
#define BAR_HEIGTH                      30
// Display height for each row
#define LINE_HEIGTH                     30


// Maximum number of supported rows
#define MAX_COM_LINES                   ((LCD_H-BAR_HEIGTH)/LINE_HEIGTH)	//7

// Single row
#define TEXT_ONE_LINE                   0
// first row
#define TEXT_FIRST_LINE                 1
// Middle row
#define TEXT_MIDDLE_LINE                2
// Last row
#define TEXT_LAST_LINE                  3

// LCD display alignment
#define ALIGN_L                         0
#define ALIGN_M                         1
#define ALIGN_R                         2
#define ALIGN_J                         3

// Display Mode DISPLAY MODE, BIT SELECT
#define DISP_MODE_NORMAL                0x00
#define DISP_MODE_REVERSE               0x01
#define DISP_MODE_UNDERLINE             0x02
#define DISP_MODE_REFRESH               0x04
#define DISP_MODE_DISXOR                0x10
#define DISP_MODE_TRANSP_R              0x20


// The starting line number of the operation prompt information is 3
#define LCD_OP_Y_S                      3

#define LCD_RGB(r, g, b)                (unsigned short)((((r<< 8) & 0xF800) | ((g << 3) & 0x7E0) | (b >> 3) ))


#define FONT_BUF_SIZE                   (196)
#define FONT_6X12                       0x060c
#define FONT_12X12                      0x0c0c
#define FONT_8X16                       0x0810
#define FONT_16X16                      0x1010
#define FONT_9X18                       0x0912
#define FONT_18X18                      0x1212
#define FONT_10X20                      0x0a14
#define FONT_20X20                      0x1414
#define FONT_12X24                      0x0C18
#define FONT_24X24                      0x1818
#define FONT_24X48                      0x1830
#define FONT_28X56                      0x1C38
#define FONT_56X56                      0x3838

#define DEFAULT_CN                      (0x1414)
#define DEFAULT_EN                      (0x0a14)

#define DEFAULT_LIST_CN                 (0x0a14)
#define DEFAULT_LIST_EN                 (0x1414)

#define BACK565_ORANGE                  LCD_RGB(255,133,39)
#define BACK565_BLUE                    0x0A33
#define BACK565_GRAY                    0xef5d
#define BACK565_GREEN                   0x0543
#define BACK565_YELLOW                  0xf5e6
#define BACK565_RED                     LCD_RGB(236,81,81)


// LCD color standard color
// white
#define RGB565_WHITE                    0xFFFF
// black
#define RGB565_BLACK                    0x0000
// grey
#define RGB565_GREY                     0xF7DE

// red
#define RGB565_RED                      0xF800
// Green
#define RGB565_GREEN                    BACK565_GREEN
// Blue
#define RGB565_BLUE                     0x001F

// orange
#define RGB565_ORANGE                   0xFD20
// Yellow
#define RGB565_YELLOW                   0xFFE0
// cyan
#define RGB565_CYAN                     0x7FFF
// Purple
#define RGB565_MAGENTA                  0xF81F

// RGB Color Definition
#define RGB_USER_DARK_GREY              LCD_RGB(153, 153, 153)
#define RGB_USER_LIGHT_GREY             LCD_RGB(247, 247, 247)
#define RGB_USER_RED                    LCD_RGB(236, 81, 81)
#define RGB_USER_BLACK                  LCD_RGB(51, 51, 51)

#define DEFAULT_FORG_COLOR              GetColorParamEx(MAIN_FORE_COLOR_ID)
#define DEFAULT_BACK_COLOR              GetColorParamEx(MAIN_BACK_COLOR_ID)
#define FORE565_MENU1                   GetColorParamEx(MENU1_FORE_COLOR_ID)
#define BACK565_MENU1                   GetColorParamEx(MENU1_BACK_COLOR_ID)
#define FORE565_MENU2                   GetColorParamEx(MENU2_FORE_COLOR_ID)
#define BACK565_MENU2                   GetColorParamEx(MENU2_BACK_COLOR_ID)
#define TIPS_FORE_COLOR                 GetColorParamEx(TIPS_FORE_COLOR_ID)
#define TIPS_BACK_COLOR                 GetColorParamEx(TIPS_BACK_COLOR_ID)


#define DRAW_TITLE(X) Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_10X20, DEFAULT_BACK_COLOR, DEFAULT_FORG_COLOR);\
UITextAlign(ALIGN_M, 0, DISP_MODE_NORMAL, X);\
Lib_LcdParaSet(LCD_W, LINE_HEIGTH, DEFAULT_EN, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR)

#define DRAW_DEFAULT()                  Lib_LcdParaSet(LCD_W, LINE_HEIGTH, FONT_SIZE_SMALL, DEFAULT_FORG_COLOR, DEFAULT_BACK_COLOR)

#define BUFCLR(szBuf)                   memset(szBuf, 0, sizeof(szBuf))

// Invalid number of rows
#define LINE_INDEX_INVALID              0xFF

typedef struct
{
    // Point to the title
    char *title;
    // Functions for menu registration
    short (*MenuCall)(int );
    // Menu Parameters
    int para;
    // Menu Icon
    char *icon;
} STMenu;

typedef struct
{
    const char *title;
    const char *titltBgImg;

    STMenu *menuList;
    unsigned short menuSize;

    const char *mainBgImg;

    unsigned long style;
} LVGL_MENU;

typedef enum
{
    LVGL_MENU_STYLE_1,
    LVGL_MENU_STYLE_2,
} LVGL_MENU_STYLE;

typedef enum
{
    THEME_COLOR_BLUE,
    THEME_COLOR_ORANGE,
    THEME_COLOR_RED,
} EThemeColor;

typedef struct
{
    char *ButtonName;
    char *OptionName;
    unsigned short ButtonForeColor;
    unsigned short ButtonBackColor;
    unsigned short OptForeColor;
    unsigned short OptBackColor;
} stWiFiButtonOptions;

#define CON_LETTER(s)                   ("P:lvgl/"#s)

#define BG_IMG                          CON_LETTER(main_bg.png)
#define MAINBG_IMG                      CON_LETTER(mainmenu_bg.png)
#define CONSUME_ICON                    CON_LETTER(1.png)
#define REFUND_ICON                     CON_LETTER(2.png)
#define QROCDEPAY_ICON                  CON_LETTER(3.png)
#define POSMANAGE_ICON                  CON_LETTER(4.png)
#define SYSSETTING_ICON                 CON_LETTER(5.png)
#define SYSMANAGE_ICON                  CON_LETTER(6.png)
#define TEST_ICON                       CON_LETTER(7.png)
#define ABOUT_ICON                      CON_LETTER(8.png)
#define MORE_ICON                       CON_LETTER(9.png)
#define WALLET_BALANCE_ICON             CON_LETTER(wallet_balance.png)
#define WALLET_ACTIVIES_ICON            CON_LETTER(wallet_activities.png)
#define FUNCTION_ICON                   CON_LETTER(function.png)
#define CONNECT_WIFI_ICON               CON_LETTER(connect_wifi.png)
#define APP_DOWNLOAD_IMG                CON_LETTER(appdownload.png)
#define ICON_LOGIN                      CON_LETTER(login.png)
#define TITLE_BG_IMG                    CON_LETTER(title_bg.png)
#define WIFI0_ICON                      CON_LETTER(wifi0.png)
#define WIFI1_ICON                      CON_LETTER(wifi1.png)
#define WIFI2_ICON                      CON_LETTER(wifi2.png)
#define WIFI3_ICON                      CON_LETTER(wifi3.png)
#define WIFI_CHECK_ICON                 CON_LETTER(check.png)
#define BEEPON_ICON                     CON_LETTER(beepOn.png)
#define BEEPOFF_ICON                    CON_LETTER(beepOff.png)
#define SYNCON_ICON                     CON_LETTER(syncOn.png)
#define SYNCOFF_ICON                    CON_LETTER(syncOff.png)
#define SPKON_ICON                      CON_LETTER(spkOn.png)
#define SPKOFF_ICON                     CON_LETTER(spkOff.png)
#define ENGLISH_ICON                    CON_LETTER(English.png)
#define PORTUGUESE_ICON                 CON_LETTER(Portuguese.png)
#define FARSI_ICON                      CON_LETTER(Farsi.png)
#define LOGIN_IN_ICON                   CON_LETTER(LogIn.png)
#define LOGIN_OUT_ICON                  CON_LETTER(LogOut.png)
#define PAY_QUERY_ICON                  CON_LETTER(PayQuery.png)
#define SETTLEMENT_ICON                 CON_LETTER(Settlement.png)
#define MAINTENNANCE_ICON               CON_LETTER(Maintenance.png)
#define DEBUG_SW_ICON                   CON_LETTER(DebugSwitch.png)
#define DEVICE_INFO_ICON                CON_LETTER(DeviceInfo.png)
#define VERSION_INFO_ICON               CON_LETTER(VersionInfo.png)
#define SERVERS_ICON                    CON_LETTER(Servers.png)
#define OTA_ICON                        CON_LETTER(OTA.png)
#define KEYINJECT_ICON                  CON_LETTER(KeyInject.png)
#define NETWORK_ICON                    CON_LETTER(Network.png)
#define POWERMANAGE_ICON                CON_LETTER(PowerManage.png)
#define SYSTEM_TIME_ICON                CON_LETTER(SystemTime.png)
#define BEEP_ICON                       CON_LETTER(Beep.png)
#define THEMECOLOR_ICON                 CON_LETTER(ThemeColor.png)
#define LANGUAGE_ICON                   CON_LETTER(Language.png)
#define CALIBATESCREEN_ICON             CON_LETTER(CalibateScreen.png)
#define PRINTERGRADE_ICON               CON_LETTER(PrinterGrade.png)
#define USBMODE_ICON                    CON_LETTER(USBMode.png)
#define ECHOTEST_ICON                   CON_LETTER(EchoTest.png)
#define ENDURANCE_ICON                  CON_LETTER(Endurance.png)
#define PRINT_ICON                      CON_LETTER(Print.png)
#define FILESYSTEM_ICON                 CON_LETTER(FileSystem.png)
#define LAYERDEMO_ICON                  CON_LETTER(LayerDemo.png)
#define MENU_STYLE2_ICON                CON_LETTER(menu_s2.png)
#define MENU_STYLE2_BG0_ICON            CON_LETTER(menu_s2_bg0.png)
#define MENU_STYLE2_BG1_ICON            CON_LETTER(menu_s2_bg1.png)
#define ENTERAMT_ICON                   CON_LETTER(enter_amt_bg.png)
#define ICC_GIF                         CON_LETTER(Icc.gif)
#define LOAD_DATA_GIF                   CON_LETTER(loaddata.gif)
#define COUNT_TIME_IMG                  CON_LETTER(counttime_bg.png)
#define SUCCESS_IMG                     CON_LETTER(success.png)
#define FAILURE_IMG                     CON_LETTER(failed.png)
#define BOOT_GIF                        CON_LETTER(g0.gif)
#define WIFICONNECTTING_GIF             CON_LETTER(wifiConnecting.gif)
#define OPTION_ICON                     CON_LETTER(opt.png)
#define GPS_ICON                        CON_LETTER(gps.png)
#define DEBUG_ICON                      CON_LETTER(icon_w_bug.png)

extern char *bgImgIcons[];

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------
void UISetBarRefreshFlag(unsigned char flag);
void UIClearRect(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize);
void UIDefaultComposition(int id, uint16_t *pBuf, uint16_t w, uint16_t h);
short UIMenuSelect(int iTimeout);
int UITimeCnt(unsigned char *pStr, unsigned short usY, unsigned char mode);
short UIInputSum(unsigned char ucOutXmode, unsigned char*pcOut, unsigned char *pucOutLen, int iTimeout);
int UIIpDisp(unsigned short usYLine, char *pIpStr);
short UIUnsupport(int timeout);
short UIWaitKey(int iTimeout, unsigned char ucBeepFlag);
void UILabelShopName(char *pText);
void UILabelNFC(char *pText);
unsigned char UiRefreshNow(void);
void UIShowBMP(unsigned short usX, unsigned short usY, char *name);
void UIText(unsigned short usX, unsigned short usY, unsigned char ucMode, const char *pcFmt, ...);
void UITextAlign(unsigned char ucAlignMode, unsigned short ucY, unsigned char ucMode, const char *pText);
void UIUpdataRect(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize);
short ShowAmount(char *pcAmt, unsigned short usY, unsigned short usForeCl, unsigned short usBlackCl);
void ShowUiMenuDefault(char *pText, const char **ppcMenuInfo, unsigned char ucTotalLines);
void ShowUiMenuPackY(char *pText, char **ppcMenuInfo, unsigned char ucTotalLines, unsigned char ucHighlightIndex);
void TimeHook(unsigned char ucMode);
void LayoutPackY(int id, unsigned short *pBuf, unsigned short w, unsigned short h);
short UIInput(unsigned char ucX, unsigned char ucY, char *pcDefaultInputStr, unsigned char *pucBuff,
    unsigned char *pucLen, unsigned char ucInputMode, int iTimeout, unsigned char ucBeepFlag);
short UICreatSubMenu(int timeout, const char *const title, STMenu *menuList, unsigned short menuSize);
short UISelectThemeColor(int themeColor);
short CreatButtonOptions(stWiFiButtonOptions *left, stWiFiButtonOptions *right);
void UITextSimplify(char *const pText, unsigned char replacePosit, const char *replaceTxt);


void UIDefaultComposition_RtoL(int id, uint16_t *pBuf, uint16_t w, uint16_t h);
void UITextAlignEx(unsigned long ulEncoding, unsigned char ucAlignMode, unsigned short ucY, unsigned char ucMode, const char *pText);
short UISetParaBySpinBoxInfoInput(unsigned char ucBitMap, int iTimeout);
short UISetParaByIpInput(unsigned char ucBitMap, char *title, int iTimeout);
int UILcdDrawAreaLvgl(unsigned short color, unsigned short x, unsigned short y, unsigned short w, unsigned short h);
void UILcdClrScreenLvgl(void);
void UIClrScreenLvgl(void);
void LcdDisSingleRecentLvgl(unsigned short x,unsigned short y,unsigned short width ,unsigned short hight ,unsigned char Action);
void UIDrawLineLvgl(unsigned short usX1, unsigned short usY1, unsigned short usX2, unsigned short usY2);
void UIIconLvgl(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char ucMode, unsigned char *pucIcon);
void UIClearAllScreenLvgl(void);
unsigned char UIGetKeyWithTaskHandlerLvgl(void);
void UIDisPlayTimerReadyLvgl(void);
int UIGetTextWidthLvgl(unsigned char *pText, unsigned short TextLen, unsigned short FontTextEn);
void UICreateImgByArrayData(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucData);
void UIShowXCodeLvglProcess1(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucIcon);
void UIShowXCodeLvglProcess2(unsigned short usStartX, unsigned short usStartY, unsigned short usXsize, unsigned short usYsize, unsigned char *pucData);
short UICreatLvglMenu(int timeout, LVGL_MENU *pLvglMenu);
void UiSubMenuGetLayout(int id, uint16_t *x, uint16_t *y);
void UiMenuGetLayout(int id, uint16_t *x, uint16_t *y);
#endif //__UI_H__

