/**
  ******************************************************************************
  * @file    utils.c
  * @author  MCD Application Team
  * @brief   STM32G081B_EVAL demonstration firmware utilities
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "stm32g0xx_hal.h"
#include "stm32g081b_eval_lcd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Backup register management */
#define BKUP_IMAGE_ID       RTC_BKP_DR0
#define BKUP_BOOT_REASON    RTC_BKP_DR1
#define BKUP_RTC_INIT       RTC_BKP_DR2

/* Private function prototypes -----------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern RTC_HandleTypeDef RtcHandle;

/* Exported functions --------------------------------------------------------*/
void UTILS_SetImageId(uint32_t ImageId)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle, BKUP_IMAGE_ID, ImageId);
}

uint32_t UTILS_GetImageId(void)
{
  return (HAL_RTCEx_BKUPRead(&RtcHandle, BKUP_IMAGE_ID));
}

void UTILS_DisplayBuildOption(uint32_t Timeout)
{
  uint8_t line;
  
  /* Clear the LCD Screen */
  BSP_LCD_Clear(LCD_COLOR_ST_PINK);
  BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  /* Set the Back Color */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);  
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAUTION", CENTER_MODE);
  
  /* Prepare the msg area */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, LCD_DEFAULT_FONT.Height, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - LCD_DEFAULT_FONT.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  
  line = 0;
  BSP_LCD_DisplayStringAt(0, (2+line) * LCD_DEFAULT_FONT.Height, (uint8_t *)"BSP built for", CENTER_MODE);
  line++;
  BSP_LCD_DisplayStringAt(0, (2+line) * LCD_DEFAULT_FONT.Height, (uint8_t *)"STM32G081B-EVAL", CENTER_MODE);
  line++;
  BSP_LCD_DisplayStringAt(0, (2+line) * LCD_DEFAULT_FONT.Height, EVAL_BOARD_REVISION, CENTER_MODE);
  
  HAL_Delay(Timeout);
}
