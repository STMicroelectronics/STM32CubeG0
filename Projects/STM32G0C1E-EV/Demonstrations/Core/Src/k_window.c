/**
  ******************************************************************************
  * @file    k_window.c
  * @author  MCD Application Team
  * @brief   This file contains the Hex dumps of the images available
  ******************************************************************************
   * @attention
   *
   * Copyright (c) 2020 STMicroelectronics.
   * All rights reserved.
   *
   * This software is licensed under terms that can be found in the LICENSE file
   * in the root directory of this software component.
   * If no LICENSE file comes with this software, it is provided AS-IS.
   *
   ******************************************************************************
   */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0c1e_eval_lcd.h"
#include "k_window.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Handle a window display.
  * @param  title to display a message, \n is used for multiple line.
  * @param  title_tc : title text color.
  * @param  title_bc : title backgroun color.
  * @param  Msg to display a message, \n is used for multiple line.
  * @param  msg_tc : title text color.
  * @param  msg_bc : title backgroun color.
  * @retval None
  */
void kWindow_Popup(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc )
{
  uint8_t substring[25];
  uint8_t lineindex;
  uint16_t index,subindex;
  
  /* Clear the LCD Screen */
  BSP_LCD_Clear(title_bc);
  BSP_LCD_SetBackColor(title_bc);
  BSP_LCD_SetTextColor(title_tc);
  
  /* Set the Back Color */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  BSP_LCD_SetTextColor(title_tc);  
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)title, CENTER_MODE);

  /* Prepare the msg area */
  BSP_LCD_SetBackColor(msg_bc);
  BSP_LCD_SetTextColor(msg_bc);
  BSP_LCD_FillRect(0, LCD_DEFAULT_FONT.Height, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - LCD_DEFAULT_FONT.Height);
  BSP_LCD_SetTextColor(msg_tc);
  
  lineindex = subindex = index = 0;
  do
  {
    substring[subindex]=Msg[index];
    if((Msg[index] == '\n') || (Msg[subindex] == '\0'))
    {
      substring[subindex] = '\0';
      BSP_LCD_DisplayStringAt(0, (2+lineindex) * LCD_DEFAULT_FONT.Height, substring, CENTER_MODE);
      lineindex++;
      subindex = 0;
    }
    else
    {
      subindex++;
    }
    
    if(Msg[index] != '\0')
    {
      index++;
    }
  }
  while(Msg[index] != '\0');
  
}

/**
  * @brief  Handle a window display.
  * @param  Msg to display a message, \n is used for multiple line.
  * @retval None
  */
void kWindow_Error(char *msg)
{
  kWindow_Popup("Error popup",LCD_COLOR_BLACK,LCD_COLOR_RED,msg,LCD_COLOR_BLACK,LCD_COLOR_RED);
}
