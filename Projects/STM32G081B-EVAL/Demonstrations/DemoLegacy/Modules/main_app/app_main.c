/**
  ******************************************************************************
  * @file    app_main.c
  * @author  MCD Application Team
  * @brief   Main application (root menu) implementation
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
#define __APP_MAIN_C

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g081b_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/* Private constants ---------------------------------------------------------*/
#define ICON_WIDTH  64
#define ICON_HEIGHT 64

/* Display columns positions */
#define X1_POS    32
#define X2_POS    X1_POS + ICON_WIDTH + 3
#define X3_POS    X2_POS + ICON_WIDTH + 3
#define X4_POS    X3_POS + ICON_WIDTH + 3

/* Display ranks positions */
#define Y1_POS    32
#define Y2_POS    Y1_POS + ICON_HEIGHT + 3
#define Y3_POS    Y2_POS + ICON_HEIGHT + 3

/* Private function prototypes -----------------------------------------------*/
static KMODULE_RETURN AppMainExec(void);
static KMODULE_RETURN AppMainExecCheckRessource(void);
static void AppMain_About(void);
static void AppMain_UserEvent(uint8_t sel);

/* Private Variable ----------------------------------------------------------*/
/* Variable used to exit an execution context */
__IO uint8_t exit_func;

tMenuItem MainMenuItems[] =
{
    {"Calendar"          , X1_POS, Y1_POS, SEL_MODULE, MODULE_CALENDAR    , NULL         , NULL             , NULL, "STFILES/Icon2.bmp",  "STFILES/Icon2_pink.bmp"  },
    {"Images Viewer"     , X2_POS, Y1_POS, SEL_MODULE, MODULE_VIEWBMP     , NULL         , NULL             , NULL, "STFILES/Icon3.bmp",  "STFILES/Icon3_pink.bmp"  },
    {"Audio"             , X3_POS, Y1_POS, SEL_MODULE, MODULE_AUDIO       , NULL         , NULL             , NULL, "STFILES/Icon4.bmp",  "STFILES/Icon4_pink.bmp"  },
    {"Thermometer & LDR" , X4_POS, Y1_POS, SEL_MODULE, MODULE_TSENSOR     , NULL         , NULL             , NULL, "STFILES/Icon6.bmp",  "STFILES/Icon6_pink.bmp"  },
    {"Low Power Mode"    , X2_POS, Y2_POS, SEL_MODULE, MODULE_LOWPOWER    , NULL         , NULL             , NULL, "STFILES/Icon7.bmp",  "STFILES/Icon7_pink.bmp"  },
    {"Files Browser"     , X3_POS, Y2_POS, SEL_MODULE, MODULE_FILESBRO    , NULL         , NULL             , NULL, "STFILES/filebw.bmp", "STFILES/filebw_pink.bmp" },
    {"Help"              , X4_POS, Y2_POS, SEL_MODULE, MODULE_HELP        , NULL         , NULL             , NULL, "STFILES/Icon12.bmp", "STFILES/Icon12_pink.bmp" },
    {"About"             , X1_POS, Y3_POS, SEL_EXEC,   0                  , AppMain_About, AppMain_UserEvent, NULL, "STFILES/Icon13.bmp", "STFILES/Icon13_pink.bmp" },
};

const tMenu MainMenu = {
  "Main menu", MainMenuItems, countof(MainMenuItems), TYPE_ICON, 3, 4
  };

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAppMain =
{
  MODULE_MAIN_APP,
  NULL,
  AppMainExec,
  NULL,
  AppMainExecCheckRessource
};  

/* Private function ----------------------------------------------------------*/
/**
  * @brief  Run the main application 
  * @param  None.
  * @note   run and display main menu.  
  * @retval None.
  */
static KMODULE_RETURN AppMainExec(void)
{
  uint8_t index;
  uint8_t indexcol;
  uint8_t indexlin;
  uint8_t iconnb = countof(MainMenuItems);
  uint8_t x = BSP_LCD_GetYSize();
  uint8_t column_nb;
  uint8_t line_nb;
  uint8_t inter_x;
  uint8_t inter_y;

  /* compute nb of icon on a line */
  column_nb = (x / ICON_WIDTH);

  /* compute nb of icon lines */
  line_nb = (iconnb / column_nb);

  if((iconnb % column_nb) != 0)
  {
    line_nb++;
  }

  /* compute space between incons on x axis */
  inter_x = (BSP_LCD_GetYSize() - (column_nb * ICON_WIDTH)) / (column_nb + 1);

  /* compute space between incons on x axis */
  inter_y = ((BSP_LCD_GetXSize() - BSP_LCD_GetFont()->Height) - (line_nb * ICON_HEIGHT)) / (line_nb + 1);

  /* fill menu item icon position */
  indexcol = 0;
  indexlin = 0;
  for(index = 0; index < iconnb; index++)
  {
    MainMenuItems[index].x = (inter_x + (indexcol * (inter_x + ICON_WIDTH)));
    MainMenuItems[index].y = (inter_y + (BSP_LCD_GetFont()->Height) + (indexlin * (inter_y + ICON_HEIGHT)));

    /* increment colomn index */
    indexcol++;
    if(indexcol > column_nb)
    {
      indexcol = 0;
      indexlin++;
    }
  }

  kMenu_Execute(MainMenu);
  return KMODULE_OK;
}

/**
  * @brief  check the main application ressources 
  * @param  None.
  * @note   None.
  * @retval None.
  */
static KMODULE_RETURN AppMainExecCheckRessource(void)
{
 uint8_t index;

  /* check icon menu */
  for(index = 0; index < countof(MainMenuItems); index++)
  {
    if(kStorage_FileExist((uint8_t *)MainMenuItems[index].pIconPath) != KSTORAGE_NOERROR)
    {
      return KMODULE_ERROR_ICON;
    }
  }

  return KMODULE_OK;
}

/**
  * @brief  Run the About information 
  * @param  None.
  * @note   display About information.  
  * @retval None.
  */
static void AppMain_About(void)
{
  exit_func = 0;
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_Clear(LCD_COLOR_ST_PINK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  BSP_LCD_DisplayStringAt(0,2*Font20.Height, (uint8_t *)EVAL_BOARD, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,3*Font20.Height, (uint8_t *)"Demonstration", CENTER_MODE);
  
  BSP_LCD_DisplayStringAt(0,5*Font20.Height, (uint8_t *)DEMO_VERSION, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,6*Font20.Height, (uint8_t *)DEMO_DATE, CENTER_MODE);
  
  BSP_LCD_DisplayStringAt(0,8*Font20.Height, (uint8_t *)DEMO_INFO1, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,9*Font20.Height, (uint8_t *)DEMO_INFO2, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0,10*Font20.Height, (uint8_t *)DEMO_INFO3, CENTER_MODE);
  while(exit_func==0);
}

/**
  * @brief  Callback used to get user feedback 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
static void AppMain_UserEvent(uint8_t sel)
{
  exit_func = 1;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
