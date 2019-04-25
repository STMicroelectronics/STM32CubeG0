/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    RTC/RTC_InternalTimeStamp/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32G0xx RTC HAL API to configure
  *          internal time stamp.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

uint8_t LcdTimeStr[16] = "hh:ms:ss";
uint8_t LcdDateStr[16] = "dd-mm-yyyy";

__IO uint8_t RTCStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void RTC_CalendarShow(void);
static void RTC_TimeStampShow(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* STM32G0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* Turn on LED1 */
  BSP_LED_On(LED1);

  /* Initialize LCD */
  if (LCD_OK != BSP_LCD_Init())
  {
    Error_Handler();
  }

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"RTC Internal", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"TimeStamp Example", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Current Time and date :");

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* Set Date and Time (if not already done before) */
  /* Check whether calendar year field has been set */
  if (__HAL_RTC_GET_FLAG(&hrtc, RTC_FLAG_INITS) != 0x00u)
  {
    /* Check if the Power On Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST) != RESET)
    {
      /* Turn on LED2: Power on reset occured */
      BSP_LED_On(LED2);
    }
    /* Check if Pin Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED4: External reset occured */
      BSP_LED_On(LED4);
    }
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /* Check whether internal time stamp event has been generated */
    if (__HAL_RTC_INTERNAL_TIMESTAMP_GET_FLAG(&hrtc, RTC_FLAG_ITSF) == SET)
    {
      /* Display time stamp Time and Date */
      RTC_TimeStampShow();

      /* Clear Internal TimeStamp flag */
      __HAL_RTC_INTERNAL_TIMESTAMP_CLEAR_FLAG(&hrtc, RTC_FLAG_ITSF);
    }
    else
    {
      BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"No Internal TimeStamp event");
    }
  }
  RTCStatus = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Display the updated Time and Date */
    RTC_CalendarShow();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 70;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV10;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV5;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  /* Set Date and Time (if not already done before) */
  /* Check whether calendar year field has been set */
  if (__HAL_RTC_GET_FLAG(&hrtc, RTC_FLAG_INITS) == 0x00u)
  {
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x2;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.SubSeconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 0x16;
  sDate.Year = 0x18;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

    /* Enable internal time stamp */
    if (HAL_RTCEx_SetInternalTimeStamp(&hrtc) !=  HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }
  /* USER CODE END RTC_Init 2 */

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
static void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)LcdTimeStr, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *) LcdTimeStr);
  /* Display date Format : mm-dd-yyyy */
  sprintf((char *)LcdDateStr, "%02d-%02d-%02d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
  BSP_LCD_DisplayStringAtLine(6, (uint8_t *) LcdDateStr);
}

/**
  * @brief  Display the time stamp
  * @param  None
  * @retval None
  */
void RTC_TimeStampShow(void)
{
  RTC_TimeTypeDef sTimeStampTime;
  RTC_DateTypeDef sTimeStampDate;
  uint8_t LcdTimeStr[16] = "hh:ms:ss";
  uint8_t LcdDateStr[16] = "dd-mm-yyyy";

  HAL_RTCEx_GetTimeStamp(&hrtc, &sTimeStampTime, &sTimeStampDate, RTC_FORMAT_BIN);

  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"Internal TimeStamp event at :");
  /* Display time Format : hh:mm:ss */
  sprintf((char *)LcdTimeStr, "%02d:%02d:%02d", sTimeStampTime.Hours, sTimeStampTime.Minutes, sTimeStampTime.Seconds);
  BSP_LCD_DisplayStringAtLine(9, (uint8_t *) LcdTimeStr);
  /* Display date Format : mm-dd-yyyy */
  sprintf((char *)LcdDateStr, "%02d-%02d-%02d", sTimeStampDate.Month, sTimeStampDate.Date, 2016);
  BSP_LCD_DisplayStringAtLine(10, (uint8_t *) LcdDateStr);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  /* Turn LED3 on */
  RTCStatus = 0xE;
  BSP_LED_On(LED3);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
