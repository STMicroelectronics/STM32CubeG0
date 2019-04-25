/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel menu functions 
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
  
/* Includes ------------------------------------------------------------------*/
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_lcd.h"
#include "k_menu.h"
#include "k_storage.h"
#include "k_module.h"


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
enum {
  KMENU_HEADER,
  KMENU_ICON,
  KMENU_TEXT,
  KMENU_EXEC,
  KMENU_WAITEVENT,
  KMENU_EXIT
};
  
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable used to forward an user event to an application */
static tExecAction kMenuEventForward = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel);

/**
  * @brief  Function to initialize the module menu
  * @param  menu 
  * @retval None
  */
void kMenu_Init(void) {
  /* Initialise the JOystick in polling mode */ 
  BSP_JOY_Init(JOY_MODE_GPIO);
}

/**
  * @brief  Function in charge to hanlde the menu selection
  * @param  menu
  * @param  sel : slected item
  * @retval None
  */
void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel)
{
  uint8_t exit = 0;
  JOYState_TypeDef joyState = JOY_NONE;
  tMenu psCurrentMenu = MainMenu;
  uint8_t index;
  
  switch(psCurrentMenu.nType)
  {
  case TYPE_ICON :
    {
      /* display the default selection */
      kStorage_OpenFileDrawBMP(psCurrentMenu.psItems[*sel].x+1, psCurrentMenu.psItems[*sel].y+1, (uint8_t *)psCurrentMenu.psItems[*sel].pIconSelectedPath);
     
      do{
        /* Poolling on joystick event */
        joyState = BSP_JOY_GetState();
        if(joyState != JOY_NONE)
        {  
          /* Remove previous selection */
          kStorage_OpenFileDrawBMP(psCurrentMenu.psItems[*sel].x+1, psCurrentMenu.psItems[*sel].y+1, (uint8_t *)psCurrentMenu.psItems[*sel].pIconPath);

          /* Wait to have good MMI behavior */
          HAL_Delay(50);

          switch(joyState)
          {
          case JOY_SEL :
            exit = 1;
            break;
          case JOY_DOWN :
            /* check if sel is on the last line position */
            if(*sel >=  ((psCurrentMenu.line - 1)*psCurrentMenu.column))
            {
              *sel = ((*sel) % psCurrentMenu.column);
            }
            else if (*sel > psCurrentMenu.column)
            {
              *sel -= psCurrentMenu.column;
            }
            else
            {
              (*sel)+= psCurrentMenu.column;
            }
            break;
          case JOY_LEFT :
            if(*sel == 0 )
            {
              *sel = psCurrentMenu.nItems -1;
            }
            else
            {
              (*sel)--;
            }
            break;    
          case JOY_RIGHT :
            (*sel)++;
            if ((*sel) >= (psCurrentMenu.nItems))
            {
              (*sel) = 0;
            }
            break;
          case JOY_UP :
            /** check if sel is on the first line position */
            if ((*sel) == 0)
            {
              (*sel) = psCurrentMenu.nItems - 1;
            }
            else if ((*sel) < psCurrentMenu.column)
            {
              (*sel)+= psCurrentMenu.column;
            }
            else
            {
              (*sel)-= psCurrentMenu.column;
            }
            break;
          default:
        	break;
          }
          
          /* To avoid lection of ison outside range 0-7 */
          if((*sel) >= 8)(*sel) = 0;
          
          /* display the new selection */
          kStorage_OpenFileDrawBMP(psCurrentMenu.psItems[*sel].x+1, psCurrentMenu.psItems[*sel].y+1, (uint8_t *)psCurrentMenu.psItems[*sel].pIconSelectedPath);          
          
          /* display the new header file */
          BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
          BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
          BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height-1);
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.psItems[*sel].pszTitle, CENTER_MODE);
        }
      } while(exit == 0);
    }
    break;
  case TYPE_TEXT :
    {
      do
      {
        index = 0;
        while (!(index >= psCurrentMenu.nItems))
        {
          if (index == (*sel))
          {
            /* Set the Back Color */
            BSP_LCD_SetBackColor(LCD_COLOR_ST_GREEN_LIGHT);
            /* Set the Text Color */
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          }
          else
          {
            /* Set the Back Color */
            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
            /* Set the Text Color */
            BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
          }
          /* Get the current menu */
          BSP_LCD_DisplayStringAt(0, Font24.Height*(index+1), (uint8_t *)psCurrentMenu.psItems[index].pszTitle,LEFT_MODE);
          index++;
        }
        
        /* Wait to have good MMI behavior */     
        HAL_Delay(400);
        while((joyState = BSP_JOY_GetState()) == JOY_NONE);
        
        switch(joyState)
        {
        case JOY_SEL :
          exit = 1;
          break;
        case JOY_DOWN :
          (*sel)++;
          /* check if sel is on the latest line position */
          if(*sel >= psCurrentMenu.nItems)
          {
            *sel = psCurrentMenu.nItems - 1;
          }
          break;
        case JOY_UP :
          /** check if sel is on the first line */
          if ((*sel) != 0) { (*sel)--; }
          break;
        default:
          break;
        }
      } while(exit == 0);
    }
    break;
  }
  
  return;
}

/**
  * @brief  Function in charge to execture a menu 
  * @param  menu 
  * @retval None
  */
void kMenu_Execute(tMenu psCurrentMenu) 
{
  uint32_t index = 0, exit = 1;
  uint32_t k_MenuState = KMENU_HEADER;
  uint8_t sel = 0;
  
  do 
  {  
    switch(k_MenuState)
    {
    case KMENU_HEADER :
      {
        /****************************** Display Header *************************/  
        /* Clear the LCD Screen */
        BSP_LCD_Clear(LCD_COLOR_WHITE);
        
        if(psCurrentMenu.pszTitle != NULL)
        {
          /* Set the Back Color */
          BSP_LCD_SetFont(&Font24);
          BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
          BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);  
          BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height-1);
          
          /* Set the Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
        switch(psCurrentMenu.nType)
        {
        case TYPE_ICON :
          if(psCurrentMenu.psItems[sel].pszTitle != NULL)
          { 
            BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.psItems[sel].pszTitle, CENTER_MODE);
          }
          k_MenuState = KMENU_ICON;
          break;
        case TYPE_TEXT :
        case TYPE_EXEC :
          if(psCurrentMenu.pszTitle != NULL)
          { 
            BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.pszTitle, CENTER_MODE);
          }
          k_MenuState = psCurrentMenu.nType;
          break;
        default : 
          k_MenuState = KMENU_EXIT;
          break;
        }
      }
      break;
    case KMENU_ICON :
      {
        for(index = 0; index < psCurrentMenu.nItems; index++)
        {
          if(psCurrentMenu.psItems[index].pIconPath != NULL)
          {
            kStorage_OpenFileDrawBMP(psCurrentMenu.psItems[index].x+1, psCurrentMenu.psItems[index].y+1, (uint8_t *)psCurrentMenu.psItems[index].pIconPath);
          }
        }
        k_MenuState = KMENU_WAITEVENT;
      }
      break;
    case KMENU_TEXT :
      {
        /* Set the Back Color */
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
        k_MenuState = KMENU_WAITEVENT;
      }
      break;
    case KMENU_EXEC :
      {
        while(BSP_JOY_GetState() != JOY_NONE);
        /* if the function need user feedback enable JOY interrupt and set callback function */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* start the Joystick interrupt */
          BSP_JOY_Init(JOY_MODE_EXTI);        
          HAL_Delay(300);
          /* set the function to report joystick event */
          kMenuEventForward = psCurrentMenu.psItems[0].pfActionFunc;
        }
        
        kMenu_Header(psCurrentMenu.psItems[0].pszTitle);
        /* Execute the test */
        psCurrentMenu.psItems[0].pfExecFunc();
        
        /* rest user feedback, in polling mode */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* stop the Joystick interrupt */
          BSP_JOY_Init(JOY_MODE_GPIO);
          HAL_Delay(300);
          /* set the function to report to NULL */
          kMenuEventForward = NULL;
        }
        k_MenuState = KMENU_EXIT;
      }
      break;
    case KMENU_WAITEVENT:
      {
        kMenu_HandleSelection(psCurrentMenu,&sel);
        /* The user has selected an execution menu */
        switch(psCurrentMenu.psItems[sel].SelType)
        {
        case SEL_MODULE:
          /* start the module execution */
          kModule_Execute(psCurrentMenu.psItems[sel].ModuleId);
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXEC :
          
          while(BSP_JOY_GetState() != JOY_NONE);
          /* if the function need user feedback enable JOY interrupt and set callback function */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* start the Joystick interrupt */
            BSP_JOY_Init(JOY_MODE_EXTI);        
            HAL_Delay(300);
            /* set the function to report joystick event */
            kMenuEventForward = psCurrentMenu.psItems[sel].pfActionFunc;
          }
          /* start the function execution */
          psCurrentMenu.psItems[sel].pfExecFunc();
          
          /* rest user feedback, in polling mode */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* stop the Joystick interrupt */
            BSP_JOY_Init(JOY_MODE_GPIO);
            HAL_Delay(300);
            /* set the function to report to NULL */
            kMenuEventForward = NULL;
          }
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_SUBMENU :
          /* Select submenu or return on the main module menu */
          kMenu_Execute(*(psCurrentMenu.psItems[sel].psSubMenu));
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXIT :
          k_MenuState = KMENU_EXIT;
          break;
        }
      }
      break;
    case KMENU_EXIT :
      exit = 0;
      break;
      
    }
  }while(exit);
}


/**
  * @brief  Function to display header information 
  * @param  menu 
  * @retval None
  */
void kMenu_Header(char *string)
{
  if(string == NULL) return;
  
  BSP_LCD_FillRect(0, 30, BSP_LCD_GetXSize(), Font24.Height-1);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);  
  BSP_LCD_SetBackColor(LCD_COLOR_ST_BLUE);  
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize() - Font24.Height, BSP_LCD_GetXSize(), Font24.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0,  BSP_LCD_GetYSize() - Font24.Height, (uint8_t *)string , CENTER_MODE);
}    


/**
  * @brief  Function in charge to hanlde user event and forward them to the module
  * @param  GPIO_Pin
  * @retval None
  */
void kMenu_EventHandler(uint16_t GPIO_Pin) {
  
  if(kMenuEventForward != NULL)
  {
    switch(GPIO_Pin)
    {
    case DOWN_JOY_PIN :
      (kMenuEventForward)(JOY_DOWN);
      break;
    case UP_JOY_PIN :
      (kMenuEventForward)(JOY_UP);
      break;
    case SEL_JOY_PIN :
      (kMenuEventForward)(JOY_SEL);
      break;
    case RIGHT_JOY_PIN :
      (kMenuEventForward)(JOY_RIGHT);
      break;
    case LEFT_JOY_PIN :
      (kMenuEventForward)(JOY_LEFT);
      break;
    default :
      break;
    }
  }
}
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
