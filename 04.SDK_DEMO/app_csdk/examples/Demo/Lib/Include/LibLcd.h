//-----------------------------------------------------------------------------
// @file    : LibLcd.h
// @brief   : LCD functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_LCD_H__
#define __LIB_LCD_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

#define LCD_HIGH		(240)
#define LCD_WIDTH		(320)

// LCD display alignment
#define ALIGN_L                         0
#define ALIGN_M                         1
#define ALIGN_R                         2
#define ALIGN_J                         3

typedef struct
{
	unsigned short w1;    // seq width
	unsigned short w2;    // text width
	unsigned short h;
	unsigned short seq;   // Seq Number

	unsigned char *pText;
	unsigned short TextLen;

	unsigned short FontSeq;			 // Seq Number font
	unsigned short FontTextEn;		 // English Text font
	unsigned short FontTextCn;		 // Chinese Text font

	// Seq Number Foreground and Background Colors
	unsigned short ForegroundColorSeq;
	unsigned short BackgroundColorSeq;

	// Text content foreground and background colors
	unsigned short ForegroundColorText;
	unsigned short BackgroundColorText;

	// Align Type
	unsigned short Align;

    unsigned short x;
    unsigned short y;
    unsigned short Mode;
}WIDGET_MENU;

typedef struct
{
	unsigned char *pText;
	unsigned short TextLen;

	unsigned short w;    //text width
	unsigned short h;

	unsigned short FontTextEn;		 // English Text font
	unsigned short FontTextCn;		 // Chinese Text font

	// Text content foreground and background colors
	unsigned short ForegroundColorText;
	unsigned short BackgroundColorText;

	// Align Type
	unsigned short Align;

    unsigned short x;
    unsigned short y;
    unsigned short Mode;
    unsigned short LineType;
}WIDGET_TEXT;

typedef struct RECT
{
    unsigned short m_usX;                 // X-axis starting coordinate
    unsigned short m_usY;                 // Y-axis starting coordinate

    unsigned short m_usWidth;             // width of rectangle
    unsigned short m_usHeight;            // height of rectangle

    unsigned short m_usBackgroundColor;   // fill color

    unsigned short m_usBorderColor;       // Border Color
    unsigned short m_usBorderWidth;       // Border width
    unsigned short m_usRadius;            // radius of rectangle
    unsigned short m_usGrandient;         // Gradient effect of rectangles

    unsigned short m_usTransparent;       // Transparent or not
    unsigned short  m_usRefreshSw;        // refresh or not
    unsigned short  m_usAlignMode;        // refresh or not
}WIDGET_RECT;

//-----------------------------------------------------------------------------
// Name    : Lib_LcdSetBrightLevel
// Func    : Set the brightness percentage of the display screen.
// Paras   : ucLevel[in],  input level, the range is from 0 to 8
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LcdSetBrightLevel(unsigned char ucLevel);

//-----------------------------------------------------------------------------
// Name    : Lib_LcdClrScreen
// Func    : Clear Screen
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LcdClrScreen(void);

//-----------------------------------------------------------------------------
// Name    : Lib_LcdRefresh
// Func    : refresh screen
// Paras   : 
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LcdRefresh(void);

//-----------------------------------------------------------------------------
// Name    : Lib_LcdDispMemcpy
// Func    : copy display information
// Paras   : src[in],  input data buffer
//           x[in],    input x-axis coordinate
//           y[in],    input y-axis coordinate
//           w[in]:    input width
//           h[in]:    input height
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_LcdDispMemcpy(unsigned short *src, unsigned short x, unsigned short y, unsigned short w, unsigned short h);

//-----------------------------------------------------------------------------
// Name    : Lib_LcdParaSet
// Func    : LCD set Parameters
// Paras   : w[in]:                    input width
//           h[in]:                    input height
//           FontTextEn[in]:           input English Font
//           ForegroundColorText[in]:  input Foreground Color
//           BackgroundColorText[in]:  input Background Color
// Ret     : 
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LcdParaSet(unsigned short w, unsigned short h, unsigned short FontTextEn, unsigned short ForegroundColorText, unsigned short BackgroundColorText);

//-----------------------------------------------------------------------------
// Name    : Lib_LcdParaGet
// Func    : LCD get Parameters
// Paras   : w[in/out]:                    output width
//           h[in/out]:                    output height
//           FontTextEn[in/out]:           output English Font
//           ForegroundColorText[in/out]:  output Foreground Color
//           BackgroundColorText[in/out]:  output Background Color
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
void Lib_LcdParaGet(unsigned short *w, unsigned short *h, unsigned short *FontTextEn, unsigned short *ForegroundColorText, unsigned short *BackgroundColorText);

#endif