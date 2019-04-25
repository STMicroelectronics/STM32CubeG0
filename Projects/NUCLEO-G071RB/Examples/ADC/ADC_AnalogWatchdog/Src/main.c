/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions with analog watchdog and 
  *          interruptions. Other peripherals used: DMA, TIM (ADC group regular
  *          conversions triggered by TIM, ADC group regular conversion data
  *          transfered by DMA).
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

/* Definitions of data related to this example */
  /* Definition of ADCx analog watchdog window thresholds */
  /* Note: Set analog watchdog thresholds in order to be between steps of DAC */
  /*       voltage (if literal WAVEFORM_VOLTAGE_GENERATION_FOR_TEST is        */
  /*       defined below).                                                    */
  #define ADC_AWD_THRESHOLD_HIGH           (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) * 5 /8) /* Analog watchdog threshold high: 5/8 of full range (4095 <=> Vdda=3.3V): 2559 <=> 2.06V */
  #define ADC_AWD_THRESHOLD_LOW            (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) * 1 /8) /* Analog watchdog threshold low: 1/8 of full range (4095 <=> Vdda=3.3V): 512 <=> 0.41V */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* Variables for ADC conversion data */
__IO   uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data (array of data) */
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Variable to report ADC analog watchdog status:   */
/*   RESET <=> voltage into AWD window   */
/*   SET   <=> voltage out of AWD window */
__IO   uint8_t         ubAnalogWatchdogStatus = RESET;  /* Set into analog watchdog interrupt callback */

/* Variable to manage push button on board: interface between ExtLine interruption and main program */
__IO   uint8_t ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);

static void Generate_waveform_SW_update_Config(void);
static void Generate_waveform_SW_update(void);

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize LED on board */
  BSP_LED_Init(LED4);
  
  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
/* Configure the ADCx peripheral */
  ADC_Config();
  
  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /* Configure the DAC peripheral and generate a constant voltage of Vdda/2.  */
  Generate_waveform_SW_update_Config();

  
  /*## Start ADC conversions ###############################################*/
  /* Start ADC group regular conversion with DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Turn-on/off LED4 in function of ADC conversion result */
    /* - Turn-off if voltage is into AWD window */
    /* - Turn-on if voltage is out of AWD window */

    /* Variable of analog watchdog status is set into analog watchdog         */
    /* interrupt callback                                                     */
    if (ubAnalogWatchdogStatus == RESET)
    {
      BSP_LED_Off(LED4);
    }
    else
    {
      BSP_LED_On(LED4);
    }
  
    /* For information: ADC conversion results are stored into array          */
    /* "aADCxConvertedValues" (for debug: check into watch window)            */
  
    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
    }
    /* Reset variable for next loop iteration (with debounce) */
    HAL_Delay(200);
    ubUserButtonClickEvent = RESET;

    /* Modifies modifies the voltage level, to generate a waveform circular,  */
    /* shape of ramp: Voltage is increasing at each press on push button,     */
    /* from 0 to maximum range (Vdda) in 4 steps, then starting back from 0V. */
    /* Voltage is updated incrementally at each call of this function.        */
    Generate_waveform_SW_update();
    
    /* Wait for voltage settling time */
    HAL_Delay(1);
    /* Note: Variable "ubUserButtonClickEvent" is set into push button        */
    /*       IRQ handler, refer to function "HAL_GPIO_EXTI_Callback()".       */

    /* Note: ADC conversions data are stored into array                       */
    /*       "aADCConvertedData"                                              */
    /*       (for debug: see variable content into watch window).             */

    /* Reset analog watchdog status */
    ubAnalogWatchdogStatus = RESET;

    /* Wait for analog watchdog detection upon new voltage */
    HAL_Delay(1);
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
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */
  /** DAC Initialization 
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig;
  
  /* Configuration of AdcHandle init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_ENABLE;               /*!< Sequencer set to fully configurable: sequencer length and each rank affectation to a channel are configurable. */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;

  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */

  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */

  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig of conversion by software start (no external event) */

  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC-DMA continuous requests to match with DMA configured in circular mode */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  AdcHandle.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_39CYCLES_5;
  AdcHandle.Init.OversamplingMode      = DISABLE;
  AdcHandle.Init.TriggerFrequencyMode  = ADC_TRIGGER_FREQ_HIGH;
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion if ADC          */
  /*       conversion is out of the analog watchdog window selected (ADC IT   */
  /*       enabled), select sampling time and ADC clock with sufficient       */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  /* Analog watchdog 1 configuration */
  AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.Channel = ADCx_CHANNELa;
  AnalogWDGConfig.ITMode = ENABLE;
  AnalogWDGConfig.HighThreshold = ADC_AWD_THRESHOLD_HIGH;
  AnalogWDGConfig.LowThreshold = ADC_AWD_THRESHOLD_LOW;
  if (HAL_ADC_AnalogWDGConfig(&AdcHandle, &AnalogWDGConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
}

/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA4) and ADC channel (pin PA4) to run this example.
  *         (this prevents the user from resorting to an external signal
  *         generator).
  *         This function configures the DAC and generates a constant voltage of Vdda/2.
  * @note   Voltage level can be modifying afterwards using function
  *         "Generate_waveform_SW_update()".
  * @param  None
  * @retval None
  */
static void Generate_waveform_SW_update_Config(void)
{
  /* Set DAC Channel data register: channel corresponding to ADC channel ADC_CHANNEL_4 */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V */
  if (HAL_DAC_SetValue(&hdac1, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, DIGITAL_SCALE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel ADC_CHANNEL_4 */
  if (HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

}

/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA4) and ADC channel (pin PA4) to run this example.
  *         (this prevents the user from resorting to an external signal
  *         generator).
  *         This function modifies the voltage level, to generate a
  *         waveform circular, shape of ramp: Voltage is increasing at each 
  *         press on push button, from 0 to maximum range (Vdda) in 4 steps,
  *         then starting back from 0V.
  *         Voltage is updated incrementally at each call of this function.
  * @note   Preliminarily, DAC must be configured once using
  *         function "Generate_waveform_SW_update_Config()".
  * @param  None
  * @retval None
  */
static void Generate_waveform_SW_update(void)
{
  static uint8_t ub_dac_steps_count = 0;      /* Count number of clicks: Incremented after User Button interrupt */
  
  /* Set DAC voltage on channel corresponding to ADC_CHANNEL_4              */
  /* in function of user button clicks count.                                   */
  /* Set DAC output on 5 voltage levels, successively to:                       */
  /*  - minimum of full range (0 <=> ground 0V)                                 */
  /*  - 1/4 of full range (4095 <=> Vdda=3.3V): 1023 <=> 0.825V                 */
  /*  - 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V                  */
  /*  - 3/4 of full range (4095 <=> Vdda=3.3V): 3071 <=> 2.475V                 */
  /*  - maximum of full range (4095 <=> Vdda=3.3V)                              */
  if (HAL_DAC_SetValue(&hdac1,
                       DAC1_CHANNEL_1,
                       DAC_ALIGN_12B_R,
                       ((DIGITAL_SCALE_12BITS * ub_dac_steps_count) / 4)
                      ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* Wait for voltage settling time */
  HAL_Delay(1);
  
  /* Manage ub_dac_steps_count to increment it in 4 steps and circularly.   */
  if (ub_dac_steps_count < 4)
  {
    ub_dac_steps_count++;
  }
  else
  {
    ub_dac_steps_count = 0;
  }

}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == USER_BUTTON_PIN)
 {
   /* Set variable to report push button event to main program */
   ubUserButtonClickEvent = SET;
 }
}
/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{

}
/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}

/**
  * @brief  Analog watchdog callback in non blocking mode. 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Set variable to report analog watchdog out of window status to main      */
  /* program.                                                                 */
  ubAnalogWatchdogStatus = SET;
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
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
  while(1) 
  {
    /* Toggle LED4 */
    BSP_LED_Off(LED4);
    HAL_Delay(800);
    BSP_LED_On(LED4);
    HAL_Delay(10);
    BSP_LED_Off(LED4);
    HAL_Delay(180);
    BSP_LED_On(LED4);
    HAL_Delay(10);
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
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
