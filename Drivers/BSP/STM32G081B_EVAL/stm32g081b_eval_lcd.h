/**
  ******************************************************************************
  * @file    STM32G081B_EVAL_lcd.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          STM32G081B_EVAL_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G081B_EVAL_LCD_H
#define STM32G081B_EVAL_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g081b_eval.h"
/* Include LCD component Driver */
#include "../Components/hx8347d/hx8347d.h"
#include "../../../Utilities/Fonts/fonts.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G081B_EVAL
  * @{
  */

/** @defgroup STM32G081B_EVAL_LCD STM32G081B_EVAL LCD
  * @{
  */

/** @defgroup STM32G081B_EVAL_LCD_Exported_Types Exported Types
  * @{
  */
typedef struct
{
    uint32_t TextColor;
    uint32_t BackColor;
    sFONT    *pFont;

} LCD_DrawPropTypeDef;
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_LCD_Exported_Constants Exported Constants
  * @{
  */
/**
  * @brief  LCD status structure definition
  */
#define   LCD_OK         0x00
#define   LCD_ERROR      0x01
#define   LCD_TIMEOUT    0x02

typedef struct
{
    int16_t X;
    int16_t Y;

} Point, * pPoint;

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
    CENTER_MODE             = 0x01,    /*!< Center mode */
    RIGHT_MODE              = 0x02,    /*!< Right mode  */
    LEFT_MODE               = 0x03,    /*!< Left mode   */
    NO_MODE

} Line_ModeTypdef;

/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_LIGHTBLUE     0x841F
#define LCD_COLOR_LIGHTGREEN    0x87F0
#define LCD_COLOR_LIGHTRED      0xFC10
#define LCD_COLOR_LIGHTCYAN     0x87FF
#define LCD_COLOR_LIGHTMAGENTA  0xFC1F
#define LCD_COLOR_LIGHTYELLOW   0xFFF0
#define LCD_COLOR_DARKBLUE      0x0010
#define LCD_COLOR_DARKGREEN     0x0400
#define LCD_COLOR_DARKRED       0x8000
#define LCD_COLOR_DARKCYAN      0x0410
#define LCD_COLOR_DARKMAGENTA   0x8010
#define LCD_COLOR_DARKYELLOW    0x8400
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_LIGHTGRAY     0xD69A
#define LCD_COLOR_GRAY          0x8410
#define LCD_COLOR_DARKGRAY      0x4208
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_BROWN         0xA145
#define LCD_COLOR_ORANGE        0xFD20

/* Macro to convert ST COLOR in RGB 24BIT definition to 16BIT RGB (R5bit,G6bit,B5bit) */
#define LCD_R(__R_COLOR__)       (((__R_COLOR__>>3)&0x1F)<<11)
#define LCD_G(__G_COLOR__)       (((__G_COLOR__>>2)&0x3F)<<5)
#define LCD_B(__B_COLOR__)       ((__B_COLOR__>>3)&0x1F)

/* Definition of Official ST COLOR */
#define LCD_COLOR_ST_BLUE_DARK   (LCD_R(0x00)|LCD_G(0x20)|LCD_B(0x52))
#define LCD_COLOR_ST_BLUE        (LCD_R(0x39)|LCD_G(0xA9)|LCD_B(0xDC))
#define LCD_COLOR_ST_BLUE_LIGHT  (LCD_R(0xD1)|LCD_G(0xE4)|LCD_B(0xF3))
#define LCD_COLOR_ST_GREEN_LIGHT (LCD_R(0xBB)|LCD_G(0xCC)|LCD_B(0x01))
#define LCD_COLOR_ST_GREEN_DARK  (LCD_R(0x00)|LCD_G(0x3D)|LCD_B(0x14))
#define LCD_COLOR_ST_YELLOW      (LCD_R(0xFF)|LCD_G(0xD3)|LCD_B(0x00))
#define LCD_COLOR_ST_BROWN       (LCD_R(0x5C)|LCD_G(0x09)|LCD_B(0x15))
#define LCD_COLOR_ST_PINK        (LCD_R(0xD4)|LCD_G(0x00)|LCD_B(0x7A))
#define LCD_COLOR_ST_PURPLE      (LCD_R(0x59)|LCD_G(0x0D)|LCD_B(0x58))
#define LCD_COLOR_ST_GRAY_DARK   (LCD_R(0x4F)|LCD_G(0x52)|LCD_B(0x51))
#define LCD_COLOR_ST_GRAY        (LCD_R(0x90)|LCD_G(0x98)|LCD_B(0x9E))
#define LCD_COLOR_ST_GRAY_LIGHT  (LCD_R(0xB9)|LCD_G(0xC4)|LCD_B(0xCA))

/**
  * @brief LCD default font
  */
#ifndef LCD_DEFAULT_FONT
#define LCD_DEFAULT_FONT         Font8
#endif

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_LCD_Exported_Functions Exported Functions
  * @{
  */
uint8_t  BSP_LCD_Init(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);

uint16_t BSP_LCD_GetTextColor(void);
uint16_t BSP_LCD_GetBackColor(void);
void     BSP_LCD_SetTextColor(__IO uint16_t Color);
void     BSP_LCD_SetBackColor(__IO uint16_t Color);
void     BSP_LCD_SetFont(sFONT *pFonts);
sFONT    *BSP_LCD_GetFont(void);

void     BSP_LCD_Clear(uint16_t Color);
void     BSP_LCD_ClearStringLine(uint16_t Line);
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *pText);
void     BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *pText, Line_ModeTypdef Mode);
void     BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);
void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_DrawPolygon(pPoint pPoints, uint16_t PointCount);
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);

void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     BSP_LCD_DisplayOff(void);
void     BSP_LCD_DisplayOn(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32G081B_EVAL_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
