//-----------------------------------------------------------------------------
// @file    : LibLvgl.h
// @brief   : LVGL-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_LVGL_H__
#define __LIB_LVGL_H_
#include "lvgl.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#define SHOW_ERROR_NO_DATA_DEFAULT_W        35
#define SHOW_ERROR_NO_DATA_DEFAULT_H        30

typedef struct BorderAttr
{
    unsigned short  m_usWidth;
    unsigned char   m_ucOpa;
    lv_color_t      m_color;
    unsigned char   m_ucSide;
}LV_BORDER_ATTR;

typedef struct BGAttr
{
    unsigned char   m_ucOpa;
    lv_color_t      m_color;
}LV_BG_ATTR;

typedef struct PadAttr
{
    unsigned short m_usTop;
    unsigned short m_usLeft;
    unsigned short m_usRight;
    unsigned short m_usBottom;
}LV_PAD_ATTR;

typedef struct TextAttr
{
    char *m_pcTextContent;
    void *m_pvTextFont;
    unsigned short  m_usTextLetterSpace;
    unsigned char   m_ucTextAlign;
    lv_color_t      m_color;
    lv_label_long_mode_t m_textLongMode;
    unsigned short  m_usTextLineSpace;
}LV_TEXT_ATTR;

typedef struct ImageAttr
{
    char *              m_pcImageSrc;
    char *              m_pcGifSrc;
    char *              m_pcImageBtnLeftSrc;
    char *              m_pcImageBtnMiddleSrc;
    char *              m_pcImageBtnRightSrc;
    lv_imgbtn_state_t   m_imageBtnstate;
    lv_color_t          m_imageRecolor;
    unsigned char       m_ucimageRecolorOpa;
}LV_IMG_ATTR;

typedef struct TextAreaAttr
{
    char *m_pcAcceptChars;
    unsigned short m_usMaxLength;
    bool m_bPasswordMode;
    bool m_bOneLineMode;
}LV_TEXTAREA_ATTR;

typedef struct ArcAttr
{
    lv_arc_mode_t   m_arcMode;
    unsigned short  m_usMaxValue;
    unsigned short  m_usMinValue;
    unsigned short  m_us_CurValue;
    unsigned short  m_usAngleStartValue;
    unsigned short  m_usAngleEndValue;
    bool            m_bRoundedMode;
    unsigned short  m_usWidth;
    unsigned char   m_ucOpa;
    lv_color_t      m_color;
}LV_ARC_ATTR;

typedef struct SliderAttr
{
    lv_slider_mode_t    m_sliderMode;
    unsigned short      m_usMaxValue;
    unsigned short      m_usMinValue;
}LV_SLIDER_ATTR;


typedef struct RollerAttr
{
    lv_roller_mode_t    m_rollerMode;
    char *              m_pcRollerOptions;
    char                m_cVisibleRowCount;
    unsigned short      m_usSelOpt;
    lv_anim_enable_t    m_isAnim;
}LV_ROLLER_ATTR;

typedef struct LineAttr
{
    lv_point_t *        m_pLinePointArray;
    unsigned short      m_usPointNum;
    unsigned short      m_usLineWidth;
    lv_color_t          m_lineColor;
    unsigned char       m_ucLineOpa;
}LV_LINE_ATTR;

typedef struct CanvasAttr
{
    void *              m_pvCanvasBuf;
    unsigned short      m_usCanvasBufWidth;
    unsigned short      m_usCanvasBufHeight;
    lv_img_cf_t         m_imgCf;
}LV_CANVAS_ATTR;

typedef struct LabelAttr
{
    bool                m_bLabelIsRecolor;
}LV_LABEL_ATTR;

typedef struct BaseDirAttr
{
    bool                m_bIsSet;
    lv_base_dir_t       m_baseDir;
}LV_BASEDIR_ATTR;

typedef struct StyleAttr
{
    unsigned short      m_usWidth;
    unsigned short      m_usHeight;
    lv_align_t          m_align;
    lv_coord_t          m_x;
    lv_coord_t          m_y;
    unsigned short      m_usRadius;

    LV_PAD_ATTR         m_stPadAttr;
    LV_BORDER_ATTR      m_stBorderAttr;
    LV_TEXT_ATTR        m_stTextAttr;
    LV_BG_ATTR          m_stBgAttr;
    LV_TEXTAREA_ATTR    m_stTextAreaAttr;
}LV_STYLE_ATTR;

typedef struct CtrolObjectAttr
{
    unsigned short      m_usWidth;
    unsigned short      m_usHeight;
    lv_obj_t *          m_pAlignObj;
    lv_align_t          m_align;
    lv_coord_t          m_x;
    lv_coord_t          m_y;
    unsigned short      m_usRadius;
    lv_scrollbar_mode_t m_scrollbarMode;
    unsigned long       m_ulAnimTime;

    LV_PAD_ATTR         m_stPadAttr;
    LV_BORDER_ATTR      m_stBorderAttr;
    LV_TEXT_ATTR        m_stTextAttr;
    LV_BG_ATTR          m_stBgAttr;
    LV_IMG_ATTR         m_stImgAttr;
    LV_TEXTAREA_ATTR    m_stTextAreaAttr;
    LV_ARC_ATTR         m_stArcAttr;
    LV_SLIDER_ATTR      m_stSliderAttr;
    LV_ROLLER_ATTR      m_stRollerAttr;
    LV_LINE_ATTR        m_stLineAttr;
    LV_CANVAS_ATTR      m_stCanvasAttr;
    LV_LABEL_ATTR       m_stLabelAttr;
    LV_BASEDIR_ATTR     m_stBaseDirAttr;

    unsigned char       m_ucBidiFlag;
}CTRL_OBJ_ATTR;

//-----------------------------------------------------------------------------
// Name    : Lib_LvglInit
// Func    : Initialize Lvgl
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LvglInit(void);

//-----------------------------------------------------------------------------
// Name    : Lib_LvglGetFont
// Func    : Get Font based on weight
// Paras   : weight[in],  input weight
// Ret     : return font
// Desc    :
//-----------------------------------------------------------------------------
lv_font_t *Lib_LvglGetFont(unsigned short weight);

//-----------------------------------------------------------------------------
// Name    : Lib_LvglTask
// Func    : Process Lvgl Task
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LvglTask(void);

//-----------------------------------------------------------------------------
// Name    : Lib_LvglLock
// Func    : lock or unlock Lvgl Task
// Paras   : enable[in],  input enable,0: enable,1: disable
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LvglLock(bool enable);

//-----------------------------------------------------------------------------
// Name    : Lib_LvglSleep
// Func    : Put LVGL into low power mode.
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LvglSleep(void);

//-----------------------------------------------------------------------------
// Name    : Lib_LvglWakeUp
// Func    : WakeUp Lvgl
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LvglWakeUp(void);

#endif