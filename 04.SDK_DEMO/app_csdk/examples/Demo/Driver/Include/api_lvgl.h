#ifndef _LIBAPI_LVGL_H__
#define _LIBAPI_LVGL_H__

#include "lvgl.h"

#define PAGE_BG_COLOR                   lv_color_hex(0xFFFFFF)

#define LV_COLOR_WHITE                  lv_color_hex(0xFFFFFF)
#define LV_COLOR_SILVER                 lv_color_hex(0xC0C0C0)
#define LV_COLOR_GRAY                   lv_color_hex(0x808080)
#define LV_COLOR_BLACK                  lv_color_hex(0x000000)
#define LV_COLOR_RED                    lv_color_hex(0xFF0000)
#define LV_COLOR_MAROON                 lv_color_hex(0x800000)
#define LV_COLOR_YELLOW                 lv_color_hex(0xFFFF00)
#define LV_COLOR_OLIVE                  lv_color_hex(0x808000)
#define LV_COLOR_LIME                   lv_color_hex(0x00FF00)
#define LV_COLOR_GREEN                  lv_color_hex(0x008000)
#define LV_COLOR_CYAN                   lv_color_hex(0x00FFFF)
#define LV_COLOR_AQUA                   LV_COLOR_CYAN
#define LV_COLOR_TEAL                   lv_color_hex(0x008080)
#define LV_COLOR_BLUE                   lv_color_hex(0x0000FF)
#define LV_COLOR_NAVY                   lv_color_hex(0x000080)
#define LV_COLOR_MAGENTA                lv_color_hex(0xFF00FF)
#define LV_COLOR_PURPLE                 lv_color_hex(0x800080)
#define LV_COLOR_ORANGE                 lv_color_hex(0xFFA500)

#define FONT_SIZE_NORMAL                                    22
#define FONT_SIZE_SMALL                                     18
#define FONT_SIZE_TIP                                       14

#define FONT_SIZE_STATUSBAR_TIME                            18
#define FONT_SIZE_TRANSACTION_AMOUNT                        28
#define FONT_SIZE_TRANSACTION_AMOUNT_TIP                    24
#define FONT_SIZE_TRANSACTION_RESULT                        20
#define FONT_SIZE_TRANSACTION_RESULT_IMFORMATION            16
#define FONT_SIZE_TIMER_MSG                                 22
#define FONT_SIZE_TIMER_TIME                                24
#define FONT_SIZE_TRANSACTION_DETAILS_IMFORMATION           18
#define FONT_SIZE_TRANSACTION_DETAILS_AMOUNT                26
#define FONT_SIZE_TEXT_INPUT_TITLE                          22
#define FONT_SIZE_TEXT_INPUT_IMFORMATION                    18
#define FONT_SIZE_TEXT_INPUT                                18
#define FONT_SIZE_MENU_PACK_Y                               20
#define FONT_SIZE_LIST_TITLE                                24
#define FONT_SIZE_WIFI_INFO_TITLE                           24
#define FONT_SIZE_WIFI_INFO                                 14
#define FONT_SIZE_CONTROLER_TITLE                           14
#define FONT_SIZE_CONTROLER_ARC_RANGE                       14
#define FONT_SIZE_CONTROLER_ARC_VALUE                       16
#define FONT_SIZE_CONTROLER_SLIDER_RANGE                    14
#define FONT_SIZE_CONTROLER_SLIDER_VALUE                    16
#define FONT_SIZE_TIMEZONE                                  18
#define FONT_SIZE_DATETIME_TITLE                            22
#define FONT_SIZE_DATETIME                                  16
#define FONT_SIZE_MENU_ITEM_TEXT                            16
#define FONT_SIZE_MENU_ID                                   14

#define STATUSBAR_BG_COLOR              LV_COLOR_GRAY
#define PAGE_TEXT_COLOR                 LV_COLOR_BLACK
#define BUTTON_BG_COLOR                 LV_COLOR_BLUE
#define BUTTON_TEXT_COLOR               LV_COLOR_WHITE
#define IMAGE_TEXT_COLOR                LV_COLOR_BLACK
#define MAX_DISP_LINE                   16
#define MAX_BUTTON_PAGE                 32
#define MAX_IMAGEBUF_PAGE               32
#define MAX_USER_STATUSBAR_ICON         5

#define NORMAL_TIME_OUT                 300

typedef void (*status_bar_cb)();

#define LVGL_OPER_SUCC                  0
#define LVGL_OPER_FAIL                  -1
#define LVGL_OPER_CANCEL                -2
#define LVGL_OPER_TIMEOUT               -3
#define LVGL_OPER_BACKSPACE             -4

#define LVGL_ALIGN_RIGHT                2
#define LVGL_ALIGN_CENTER               1
#define LVGL_ALIGN_LEFT                 0

#define LVGL_INPUT_MODE_NUM             0
#define LVGL_INPUT_MODE_TEXT            1
#define LVGL_INPUT_MODE_IP              2
#define LVGL_INPUT_MODE_PWD             3
#define LVGL_INPUT_MODE_AMOUNT          4

#define PAGE_MAX_SHOW_MENU_COUNT        (4)
#define PAGE_MAX_SHOW_MENU_COUNT_LVGL   (6)

#define PAGE_MAX_SHOW_MENU_STYLE2       (5)

#define LVGL_BUTTON_SELECTED_FOR_REFRESH    (0xFF)

typedef struct
{
    char *bgImg;
    char *noImg;
    char *icon;
    char *text;
    int textXOff;
    bool noRecolor;
} t_lvgl_image;

typedef struct
{
    int font;
    char *title;
    char *msg;
    char *gif;
    int timeout;
    unsigned char isCountDown;
} LVGL_DIALOG;

typedef struct
{
    char *img;
    char *title;
    char *msg;
    int timeout;
    lv_color_t fontColor;
} LVGL_RESULT;

#define LIB_EXPORT

LIB_EXPORT int LvUiInit(void);
LIB_EXPORT void LvUilCls(void);
LIB_EXPORT lv_obj_t* LvUiGetPageBody();
LIB_EXPORT void LvUiCtrlStatusBar(int status);
LIB_EXPORT int LvUiWaitKey(int timeout);
LIB_EXPORT int LvUiShowImage(int x, int y, t_lvgl_image *img, int id);
LIB_EXPORT int LvUiDialog(LVGL_DIALOG *pDialog, void **handle);
LIB_EXPORT int LvUiRefreshDialog(void *handle, LVGL_DIALOG *pDialog);
LIB_EXPORT int LvUiCloseDialog(void* handle);
LIB_EXPORT int LvUiShowResult(LVGL_RESULT *pResult);
LIB_EXPORT void LvUiCreateStatusBarTask(status_bar_cb task_cb);
LIB_EXPORT int LvUiSetStatusBarIcon(int x, char* iconname, int group);
LIB_EXPORT void LvUiSetStatusBar(int height, lv_color_t bgColor);
LIB_EXPORT unsigned int LvUiGetStatusBarHeight(void);
LIB_EXPORT int LvUiInputAmt(char* str_amt, int max, int timeout);
LIB_EXPORT lv_obj_t* LvUiCreateInputTextBody(int font, char* title, char* msg);
LIB_EXPORT lv_obj_t* LvUiCreateInputTextArea(lv_obj_t *body, char* text, int font, int mode, int min, int max);
LIB_EXPORT int LvUiText(int align, int x, int y, int font, char* text, lv_color_t textColor);
LIB_EXPORT lv_obj_t* LvUiCreateInputIPTextArea(lv_obj_t *body, int font);
LIB_EXPORT int LvUiDate(char *Date, char *title, int iTimeout);
LIB_EXPORT int LvUiSwitch(char *title, int iTimeout, unsigned char ucParamId, char *imgOnSrc, char *imgOffSrc);

extern void LvUiBtnClickedCb ( lv_event_t *event );
extern void LvUiBtnKeyCb ( lv_event_t *event );
extern void LvUiSetCb ( lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data );
extern void LvUilistButEventhandler(lv_event_t *e);
extern void LvUiSwitchEventHandler (lv_event_t *e);
extern void LvUiSliderEventHandler (lv_event_t *e);
extern int LvUiInputText(lv_obj_t* ta, int mode, int min, int max, int timeout);
extern bool LvUiStatusBarIsExist(void);
extern void LvUiFunctionRegisterToSdk(void);
extern int LvUiMessageBox(char* pcDisp, unsigned int uiType, char *title, int iTimeDly);
extern void LvUiShowUiMenuPackY(char *pText, char **ppcMenuInfo, unsigned char ucTotalLines, unsigned char ucHighlightIndex, unsigned char isPicture);
extern unsigned char LvGetKey(void);
#endif

