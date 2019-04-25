/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "utils.h"
#include "stm32g0xx_hal.h"
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_sd.h"
#include "stm32g081b_eval_lcd.h"
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
static void RTC_Config(void);
static void BootCheckAndExec(void);
static void Error_Handler(void);

/* Private variables ---------------------------------------------------------*/
uint32_t RccBootFlags = 0;
uint32_t PwrSBFlag = 0;

/* Exported variables --------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle = {0};

/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  /* Get back current CPU clock config */
  SystemCoreClockUpdate();

  if(SystemCoreClock != 64000000)
  {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock @ CPU max */
    SystemClock_Config();
  }

  /* LED init */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Enable RTC & backup domain access */
  RTC_Config();

  /* Check Boot Reason */
  BootCheckAndExec();

  /* Start the demo thread */
  kDemo_Start();

  /* we should not reach here */
  while(1);
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
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
  /* Initialize the SD */
  BSP_SD_Init();

  /* Initialize the LCD */
  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Check if we return from standby */
  if((PwrSBFlag & PWR_SR1_SBF) != 0x00u)
  {
    kWindow_Popup("Low Power", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\nExit from Standby\n", LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );
    /* Let time user to see information */
    HAL_Delay(2000);
  }

  /* Check the SD presence else wait until SD insertion */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    kWindow_Error("sd not present\n...\nwait sd insertion\n");
    while(BSP_SD_IsDetected() != SD_PRESENT);
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

  /* control the ressources */
  if(kModule_CheckRessource() != KMODULE_OK)
  {
    kWindow_Error("ressource file\nmissing\nplease update\nsd card content\n");
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

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLP                           = 16
  *            PLLQ                           = 5
  *            PLLR                           = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Set HSI as PLL source to use it for max 64MHz frequency as SYSCLK source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN            = 64;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV16;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    while(1);
  }
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
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 0x7Fu;
  RtcHandle.Init.SynchPrediv = 0x00FFu;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
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


static void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
