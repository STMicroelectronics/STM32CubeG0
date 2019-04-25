/**
  ******************************************************************************
  * @file    ssd1315.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the ssd1315.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SSD1315_H
#define SSD1315_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/lcd.h"
#include <string.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup ssd1315
  * @{
  */

/** @defgroup ssd1315_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup ssd1315_Exported_Constants
  * @{
  */

/**
  * @brief  ssd1315 Size
  */
#define  SSD1315_LCD_PIXEL_WIDTH    ((uint16_t)128)
#define  SSD1315_LCD_PIXEL_HEIGHT   ((uint16_t)64)

#define  SSD1315_LCD_COLUMN_NUMBER  ((uint16_t)128)
#define  SSD1315_LCD_PAGE_NUMBER    ((uint16_t)8)

/**
  * @brief  ssd1315 Color
  */
#define  SSD1315_COLOR_WHITE  0xFF
#define  SSD1315_COLOR_BLACK  0x00

/**
  * @brief  ssd1315 Scrolling
  */
#define  SSD1315_SCROLL_RIGHT 0x26
#define  SSD1315_SCROLL_LEFT  0x27

#define  SSD1315_SCROLL_FREQ_2FRAMES    0x07
#define  SSD1315_SCROLL_FREQ_3FRAMES    0x04
#define  SSD1315_SCROLL_FREQ_4FRAMES    0x05
#define  SSD1315_SCROLL_FREQ_5FRAMES    0x00
#define  SSD1315_SCROLL_FREQ_25FRAMES   0x06
#define  SSD1315_SCROLL_FREQ_64FRAMES   0x01
#define  SSD1315_SCROLL_FREQ_128FRAMES  0x02
#define  SSD1315_SCROLL_FREQ_256FRAMES  0x03

/**
  * @}
  */

/** @defgroup ssd1315_Exported_Functions
  * @{
  */
void     ssd1315_Init(void);

void     ssd1315_DisplayOn(void);
void     ssd1315_DisplayOff(void);
void     ssd1315_Clear(uint16_t RGBCode);
void     ssd1315_Refresh(void);

void     ssd1315_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
uint16_t ssd1315_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     ssd1315_SetPage(uint16_t Page);
void     ssd1315_SetColumn(uint16_t Column);

void     ssd1315_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     ssd1315_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     ssd1315_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void     ssd1315_ShiftBitmap(uint16_t Xpos, uint16_t Ypos, int16_t Xshift, int16_t Yshift,uint8_t *pbmp);

uint16_t ssd1315_GetLcdPixelWidth(void);
uint16_t ssd1315_GetLcdPixelHeight(void);

void     ssd1315_ScrollingSetup(uint16_t ScrollMode, uint16_t StartPage, uint16_t EndPage, uint16_t Frequency);
void     ssd1315_ScrollingStart(void);
void     ssd1315_ScrollingStop(void);

/* LCD driver structure */
extern LCD_DrvTypeDef   ssd1315_drv;

/* LCD IO functions */
/* Link function for LCD peripheral */
void    LCD_IO_Init(void);
void    LCD_IO_DeInit(void);
void    LCD_IO_WriteCommand(uint8_t Cmd);
void    LCD_IO_WriteData(uint8_t Value);
void    LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void    LCD_Delay (uint32_t delay);
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

#endif /* SSD1315_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
