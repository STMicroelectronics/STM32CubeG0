 /**
  ******************************************************************************
  * @file    stm32g071b_discovery_lcd.h
  * @author  MCD Application Team
  * @brief   Header file for stm32g071b_discovery_lcd.c module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G071B_DISCOVERY_LCD_H
#define STM32G071B_DISCOVERY_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"
/* Include LCD component Driver */
#include "../Components/ssd1315/ssd1315.h"
#include "../../../Utilities/Fonts/fonts.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_LCD STM32G071B-DISCO LCD
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Exported_Types  Exported Types
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

/** @defgroup STM32G071B_DISCOVERY_LCD_Exported_Constants Exported Constants
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
  CENTER_MODE          = 0x01,    /*!< Center mode */
  RIGHT_MODE           = 0x02,    /*!< Right mode  */
  LEFT_MODE            = 0x03     /*!< Left mode   */
} Line_ModeTypdef;

/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLACK          0x00
#define LCD_COLOR_WHITE          0xFF

/**
  * @brief  LCD scroll mode
  */
#define LCD_SCROLL_RIGHT         SSD1315_SCROLL_RIGHT
#define LCD_SCROLL_LEFT          SSD1315_SCROLL_LEFT

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font16

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LCD_Exported_Functions  Exported Functions
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

void     BSP_LCD_ScrollingSetup(uint16_t StartLine, uint16_t EndLine, uint16_t Mode);
void     BSP_LCD_ScrollingStart(void);
void     BSP_LCD_ScrollingStop(void);

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_DrawPolygon(pPoint pPoints, uint16_t PointCount);
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);
void     BSP_LCD_Refresh(void);

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

#endif /* STM32G071B_DISCOVERY_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
