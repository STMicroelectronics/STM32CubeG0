/**
  ******************************************************************************
  * @file    app_help.c
  * @author  MCD Application Team
  * @brief   Help application implementation.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#define __APP_HELP_C

/* Includes ------------------------------------------------------------------*/
#include "stm32g081b_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _HelpDemoExec(void);
KMODULE_RETURN _HelpConfig(void);

void HelpDemo(void);
void HelpMenuUserAction(uint8_t sel);
void HelpUserHeader(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem HelpMenuMenuItems[] =
{
    {"Press down to exit", 14, 30, TYPE_EXEC, MODULE_NONE, HelpDemo, HelpMenuUserAction, NULL, NULL, NULL  },
};

const tMenu HelpMenu = {
  "Help", HelpMenuMenuItems, countof(HelpMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */
static __IO uint8_t user_stop=0;
static __IO uint8_t user_page=0;
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleHelp =
{
  MODULE_HELP,
  _HelpConfig,
  _HelpDemoExec,
  NULL,
  NULL
};  


/**
  * @brief  setup the HW for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _HelpConfig(void)
{
  return KMODULE_OK;
}

/**
  * @brief  Run the applications 
  * @param  None.
  * @retval None.
  */
KMODULE_RETURN _HelpDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(HelpMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


void HelpDemo(void)
{
  uint8_t line =0;
  
  /* Display user information */
  HelpUserHeader();
  
  /* Display temperature until user event */
  user_stop = 0;
  
  while(user_stop == 0) 
  {
    if(user_action == 1)
    {
      /* Cleau LCD screen */
      BSP_LCD_SetFont(&Font24);
      BSP_LCD_ClearStringLine(1);
      BSP_LCD_ClearStringLine(2);
      BSP_LCD_ClearStringLine(3);
      BSP_LCD_ClearStringLine(4);
      BSP_LCD_ClearStringLine(5);
      BSP_LCD_ClearStringLine(6);
      BSP_LCD_ClearStringLine(7);
      BSP_LCD_ClearStringLine(8);
      
      
      if (user_page == 0x01)
      {
        BSP_LCD_SetFont(&Font24);
        line = 1;        
        BSP_LCD_DisplayStringAtLine(line,(uint8_t*)  "Voltage Config:      ");
        BSP_LCD_SetFont(&Font12);
        line = 5;
        kStorage_OpenFileDrawBMP(0, LINE(line) ,(uint8_t*)  "STFILES/ICONJP12.bmp");
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "JP15: VBAT Voltage", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- Vbat pin is connected to VDD when ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  JP15 is set as shown to the left.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- With jumper on [2-3] Vbat is ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  connected to 3V battery.", LEFT_MODE);
        line = 11;                   
        kStorage_OpenFileDrawBMP(0, LINE(line),(uint8_t*)  "STFILES/ICONJP12.bmp");
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "JP16: VDD Voltage", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- VDD pin is connected to 3.3V when  ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  JP16 is set as shown to the left.  ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  Setting mandatory for USB daughter.", LEFT_MODE);      
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- With jumper on [2-3] VDD pin is ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  connected to VDD_ADJ.", LEFT_MODE);
      }
      
      else if (user_page == 0x02)
      {
        BSP_LCD_SetFont(&Font24);
        line = 1;        
        BSP_LCD_DisplayStringAtLine(line,(uint8_t*)  "Audio Config:      ");
        BSP_LCD_SetFont(&Font12);
        line = 5;
        kStorage_OpenFileDrawBMP(0, LINE(line) ,(uint8_t*)  "STFILES/ICONJP12.bmp");
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "JP6: VBAT Voltage", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- Mono playback is enabled when JP6 ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  is set as shown to the left.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- With jumper on [2-3] Stereo ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  playback is enabled.", LEFT_MODE);
        line = 11;                   
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "JP18: Speaker amplifier", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- Speaker amplifier U17 is enabled", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  when JP18 is closed.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- Speaker amplifier U17 is disabled", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  when JP18 is open.", LEFT_MODE);       
      }
      else if (user_page == 0x03)
      {
        BSP_LCD_SetFont(&Font24);
        line = 1;        
        BSP_LCD_DisplayStringAtLine(line,(uint8_t*)  "RS232/485 Config1:  ");
        BSP_LCD_SetFont(&Font12);
        line = 5;
        kStorage_OpenFileDrawBMP(0, LINE(line) ,(uint8_t*)  "STFILES/ICONJP12.bmp");
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "JP14: VBAT Voltage", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- RS232_RX<->RS232 transceiver", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  RS232 communication is enabled  ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  when JP14 is set as shown to ", LEFT_MODE);     
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  the left.", LEFT_MODE);     
        line++;        
        kStorage_OpenFileDrawBMP(0, LINE(line),(uint8_t*)  "STFILES/ICONJP23.bmp");           
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "- RS485_RX<->RS485 transceiver", LEFT_MODE);    
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  RS485 communication is enabled  ", LEFT_MODE);        
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  when JP14 is set as shown to ", LEFT_MODE);     
        BSP_LCD_DisplayStringAt(64, LINE(line++) ,(uint8_t*)  "  the left.", LEFT_MODE);     
     
        
      }else if (user_page == 0x04)
      {
        BSP_LCD_SetFont(&Font24);
        line = 1;        
        BSP_LCD_DisplayStringAtLine(line,(uint8_t*)  "RS232/485 Config2:  ");
        BSP_LCD_SetFont(&Font12);
        line = 5;                
        kStorage_OpenFileDrawBMP(0, LINE(line),(uint8_t*)  "STFILES/ICONJP12.bmp");
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "JP8/JP10: Rx/Tx Boot", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- Rx or TX without bootload when ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  JP8/JP10 are set as shown to   ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  the left.", LEFT_MODE);
        line++;
        kStorage_OpenFileDrawBMP(0, LINE(line),(uint8_t*)  "STFILES/ICONJP23.bmp");        
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "- Rx or TX with bootload when  ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  JP8/JP10 are set as shown to ", LEFT_MODE);
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  the left.", LEFT_MODE);             
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  CN1 motor connector is to be ", LEFT_MODE); 
        BSP_LCD_DisplayStringAt(64, LINE(line++),(uint8_t*)  "  open in this case.", LEFT_MODE);              
        
      }
      user_action = 0;
    }
  }
  BSP_LCD_SetFont(&Font24);
}

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void HelpUserHeader(void)
{
    /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);

  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAtLine(1,(uint8_t*)  "Demo Config:      ");
  
  BSP_LCD_SetFont(&Font12);
  /* Display the Help icon */
  kStorage_OpenFileDrawBMP(0, LINE(5), (uint8_t *)"STFILES/ICON12.bmp");  
  BSP_LCD_DisplayStringAt(64, LINE(5), (uint8_t*)  "Use RIGHT and LEFT  ", LEFT_MODE);  
  BSP_LCD_DisplayStringAt(64, LINE(6), (uint8_t*)  "to go to the next/previous slide", LEFT_MODE);  
  BSP_LCD_DisplayStringAt(64, LINE(7), (uint8_t*)  "and DOWN to exit.      ", LEFT_MODE);  
  
  BSP_LCD_DisplayStringAt(64, LINE(10), (uint8_t*)  "Caution: Daugther board MB1351A ", LEFT_MODE);
  BSP_LCD_DisplayStringAt(64, LINE(11), (uint8_t*)  "must be plugged to run Demonstration", LEFT_MODE);        
  BSP_LCD_DisplayStringAt(64, LINE(12), (uint8_t*)  "Firmware.", LEFT_MODE);    

}  


/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void HelpMenuUserAction(uint8_t sel)
{

  if(user_action == 1)
    return;
  else if(sel != JOY_UP)
    user_action = 1;
  
  switch(sel)
  {
  case JOY_DOWN : 
    user_stop = 1;
    break;
  case JOY_LEFT : 
    if(user_page <= 1)
      user_page=4;  
    else
      user_page--;
    break;    
  case JOY_RIGHT:     
    if(user_page >= 4)
      user_page=1;  
    else
      user_page++;
    break;
  default :
    break;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
