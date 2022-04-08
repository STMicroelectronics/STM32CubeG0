/**
  ******************************************************************************
  * @file    SubDemoLegacy.c
  * @author  MCD Application Team
  * @brief   This file implements the Legacy sub-demo API
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
#include "SubDemoLegacy.h"
#include "utils.h"
#include "stm32g0xx_hal.h"
#include "stm32g0c1e_eval.h"
#include "stm32g0c1e_eval_sd.h"
#include "stm32g0c1e_eval_lcd.h"
#include "k_config.h"
#include "k_window.h"
#include "k_demo.h"
#include "k_menu.h"
#include "k_module.h"
#include "k_storage.h"
#include "app_main.h"
#include "app_thermometer_ldr.h"
#include "app_imagesbrowser.h"
#include "app_filesbrowser.h"
#include "app_lowpower.h"
#include "app_audio.h"
#include "app_calendar.h"
#include "app_help.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t SubDemoLegacyInit(void);
static uint32_t SubDemoLegacyDeInit(void);
static void SubDemoLegacyExec(void);
static void     SubDemoLegacyExtiRisingEdgeDetectionManagement(uint16_t GPIO_Pin);

static void RTC_Config(void);

static void BootCheckAndExec(void);

/* Private variables ---------------------------------------------------------*/
static uint32_t RccBootFlags = 0;
static uint32_t PwrSBFlag = 0;

/* Exported variables --------------------------------------------------------*/
SubDemoApi_t SubDemoApiLegacy = 
{
  .pfnSubDemoInit                               = SubDemoLegacyInit,
  .pfnSubDemoDeInit                             = SubDemoLegacyDeInit,
  .pfnSubDemoExec                               = SubDemoLegacyExec,
  .pfnSubDemoExtiRisingEdgeDetectionManagement  = SubDemoLegacyExtiRisingEdgeDetectionManagement,
  .pfnSubDemoExtiFallingEdgeDetectionManagement = NULL
};

/* Exported functions --------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
uint32_t SubDemoLegacyInit(void)
{
  return 0;
}

uint32_t SubDemoLegacyDeInit(void)
{
  return 0;
}

void SubDemoLegacyExec(void)
{
  /* Get back current CPU clock config */
  SystemCoreClockUpdate();

  if (SystemCoreClock != 64000000)
  {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock @ CPU max */
    SystemClock_Config_64MHz();
  }

  /* Enable RTC & backup domain access */
  RTC_Config();

  /* Check Boot Reason */
  BootCheckAndExec();

  /* Start the demo thread */
  kDemo_Start();

  /* we should not reach here */
  while(1);
}

void SubDemoLegacyExtiRisingEdgeDetectionManagement(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
  case SD_DETECT_PIN :
    kStorage_SdDetection(BSP_SD_IsDetected());
    break;
  case DOWN_JOY_PIN :
  case UP_JOY_PIN :
  case SEL_JOY_PIN :
  case RIGHT_JOY_PIN :
  case LEFT_JOY_PIN :
  case TAMPER_BUTTON_PIN :
    kMenu_EventHandler(GPIO_Pin);
    break;
  default :
    Error_Handler();
    break;
  }
}


/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void kDemo_Initialization(void)
{
  uint8_t ret;
  
  /* Initialize the LCD */
  ret = BSP_LCD_Init();
  if (ret!=LCD_OK) while(1);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Initialize the SD */
  ret = BSP_SD_Init();
  if (ret == MSD_ERROR)
  {
  /* Check the SD presence else wait until SD insertion */
    if (BSP_SD_IsDetected() != SD_PRESENT)
    {
      kWindow_Error("sd not present\n...\nwait sd insertion\n");
      while(BSP_SD_IsDetected() != SD_PRESENT);
    }
    else
    {
      while(1);
    }
  }

  /* Check if we return from standby */
  if((PwrSBFlag & PWR_SR1_SBF) != 0x00u)
  {
    kWindow_Popup("Low Power", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\nExit from Standby\n", LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );
    /* Let time user to see information */
    HAL_Delay(2000);
  }

  /* FatFs initialisation */
  if(kStorage_Init() != KSTORAGE_NOERROR)
  {
    kWindow_Error("FatFs initialization failed\nplease check the SD card\n");
    Error_Handler();
  }

  if(RccBootFlags != 0)
  {
    /* Display the demonstration window */
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height*3);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt( 0,              0  , (uint8_t *)EVAL_BOARD     , CENTER_MODE);
    BSP_LCD_DisplayStringAt( 0,  Font24.Height  , (uint8_t *)"DEMONSTRATION", CENTER_MODE);
    BSP_LCD_DisplayStringAt( 0,  Font24.Height*2, (uint8_t *)"LEGACY", CENTER_MODE);
    kStorage_OpenFileDrawPixel(75, 70, (uint8_t *)"STFILES/STLogo.bmp");

    /* Let time user to see information */
    HAL_Delay(2000);
  }

  /* Display the BSP option (REV. A v.s. REV. B) */
  UTILS_DisplayBuildOption(2000);

  /* Module Initialization */
  kModule_Init();

  /* control the resources */
  if(kModule_CheckRessource() != KMODULE_OK)
  {
    kWindow_Error("resource file\nmissing\nplease update\nsd card content\n");
    while(1);
  }
}

void kModule_Init(void)
{
   kModule_Add(MODULE_MAIN_APP    , ModuleAppMain);
   kModule_Add(MODULE_TSENSOR     , ModuleTsensor);
   kModule_Add(MODULE_VIEWBMP     , ModuleImageBrowser);
   kModule_Add(MODULE_FILESBRO    , ModuleFilesBrowser);
   kModule_Add(MODULE_LOWPOWER    , ModuleLowPower);
   kModule_Add(MODULE_AUDIO       , ModuleAudio);
   kModule_Add(MODULE_CALENDAR    , ModuleCalendar);
   kModule_Add(MODULE_HELP        , ModuleHelp);
}

void kDemo_UnInitialization(void)
{
  /* Nothing to do */
}

/**
  * @brief  Configures the RTC & backup domain access
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();

  /* enable RTC & TAMP clock gating */
  __HAL_RCC_RTCAPB_CLK_ENABLE();

  /* Set the RTC time base to 1s */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 0x7Fu;
  hrtc.Init.SynchPrediv = 0x00FFu;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
  if(HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  Check boot reason
  * @param  None
  * @retval None
  */
static void BootCheckAndExec(void)
{
  /* Get reset flags */
  RccBootFlags = RCC->CSR;

  /* Clear reset flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /* Get Standby flag */
  PwrSBFlag = PWR->SR1;

  /* Clear Standby flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
}
