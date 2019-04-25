/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples/ADC/ADC_Oversampling/Src/main.c
  * @author  MCD Application Team
  * @brief   Use ADC to convert a single channel at each trig from timer.
  *          Conversion data are transferred by DMA into a table,
  *          indefinitely (circular mode).
  *          Example using the STM32G0xx ADC HAL API.
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
 /* Definitions of environment analog values */
/* Definitions of data related to this example */
  /* ADC unitary conversion timeout */
  /* Considering ADC settings, duration of 1 ADC conversion should always    */
  /* be lower than 1ms.                                                      */
  #define ADC_UNITARY_CONVERSION_TIMEOUT_MS (   1U)

  /* Init variable out of ADC expected conversion data range for data         */
  /* on 16 bits (oversampling enabled).                                       */
  #define VAR_CONVERTED_DATA_INIT_VALUE_16BITS    (0xFFFF + 1U)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* Peripherals handlers declaration */
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Variables for ADC conversion data */
__IO   uint16_t   uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

/* Variables for ADC conversion data computation to physical values */
uint16_t   uhADCxConvertedData_Voltage_mVolt = 0;  /* Value of voltage calculated from ADC conversion data (unit: mV) */
/* Variable to manage push button on board: interface between ExtLine interruption and main program */
__IO   uint8_t ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void Configure_ADC(void);

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
  BSP_LED_Init(LED1);
  
  /* Configure Tamper push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  /* Configure ADC */
  /* Note: This function configures the ADC but does not enable it.           */
  /*       Only ADC internal voltage regulator is enabled by function         */
  /*       "HAL_ADC_Init()".                                                  */
  /*       To activate ADC (ADC enable and ADC conversion start), use         */
  /*       function "HAL_ADC_Start_xxx()".                                    */
  /*       This is intended to optimize power consumption:                    */
  /*       1. ADC configuration can be done once at the beginning             */
  /*          (ADC disabled, minimal power consumption)                       */
  /*       2. ADC enable (higher power consumption) can be done just before   */
  /*          ADC conversions needed.                                         */
  /*          Then, possible to perform successive ADC activation and         */
  /*          deactivation without having to set again ADC configuration.     */
  Configure_ADC();
  
  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /* Configure the DAC peripheral and generate a constant voltage of Vdda/2.  */
  Generate_waveform_SW_update_Config();

 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Check for event on push button to perform following actions */
    if ((ubUserButtonClickEvent) == SET)
    {
      /* Reset variable for next loop iteration (with debounce) */
      HAL_Delay(200);
      ubUserButtonClickEvent = RESET;
      
      /* Modifies modifies the voltage level, to generate a waveform circular,  */
      /* shape of ramp: Voltage is increasing at each press on push button,     */
      /* from 0 to maximum range (Vdda) in 4 steps, then starting back from 0V. */
      /* Voltage is updated incrementally at each call of this function.        */
      Generate_waveform_SW_update();
      
      /* Note: Variable "ubUserButtonClickEvent" is set into push button        */
      /*       IRQ handler, refer to function "HAL_GPIO_EXTI_Callback()".       */
    }
 
   /*## Enable peripherals ####################################################*/

  /* Start ADC group regular conversion */
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* ADC conversion start error */
    Error_Handler();
  }


    /* Wait till conversion is done */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
    {
      /* End Of Conversion flag not set on time */
      Error_Handler();
    }
    else
    {
      /* Retrieve ADC conversion data */
      uhADCxConvertedData = HAL_ADC_GetValue(&AdcHandle);
      
      /* Computation of ADC conversions raw data to physical values           */
      /* using helper macro.                                                  */
      uhADCxConvertedData_Voltage_mVolt = __ADC_CALC_DATA_VOLTAGE(VDDA_APPLI, uhADCxConvertedData)/1;
      
      /* Toggle LED1 as heart beat */
      BSP_LED_Toggle(LED1);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   
    /* Note: ADC conversion data are computed to physical values              */
    /*       into variable "uhADCxConvertedData_Voltage_mVolt"                */
    /*       using helper macro "__ADC_CALC_DATA_VOLTAGE()".                  */
    /*       (for debug: see variable content into watch window).             */

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
  * @brief  Configure ADC (ADC instance: ADC1) and GPIO used by ADC channels.
  *         Configuration of GPIO:
  *           - Pin:                    PA.04 (on this STM32 device, ADC1 channel 4 is mapped on this GPIO)
  *           - Mode:                   analog
  *         Configuration of ADC:
  *         - Common to several ADC:
  *           - Conversion clock:       Synchronous from PCLK
  *           - Internal path:          None                         (default configuration from reset state)
  *         - Multimode
  *           Feature not available                                  (feature not available on this STM32 serie)
  *         - ADC instance
  *           - Resolution:             12 bits                      (default configuration from reset state)
  *           - Data alignment:         right aligned                (default configuration from reset state)
  *           - Low power mode:         disabled                     (default configuration from reset state)
  *         - Group regular
  *           - Trigger source:         SW start
  *           - Trigger edge:           not applicable with SW start
  *           - Continuous mode:        single conversion            (default configuration from reset state)
  *           - DMA transfer:           disabled                     (default configuration from reset state)
  *           - Overrun:                data overwritten
  *           - Sequencer length:       disabled: 1 rank             (default configuration from reset state)
  *           - Sequencer discont:      disabled: sequence done in 1 scan (default configuration from reset state)
  *           - Sequencer rank 1:       ADCx ADCx_CHANNELa
  *         - Group injected
  *           Feature not available                                  (feature not available on this STM32 serie)
  *         - Channel
  *           - Sampling time:          ADCx ADCx_CHANNELa set to sampling time 39.5 ADC clock cycles (on this STM32 serie, sampling time is common to group of channels)
  *           - Differential mode:      single ended                 (feature fixed on this STM32 serie: channels single ended)
  *         - Analog watchdog
  *           Feature not used: all parameters let to default configuration from reset state
  *           - AWD number:             1
  *           - Monitored channels:     none                         (default configuration from reset state)
  *           - Threshold high:         0x000                        (default configuration from reset state)
  *           - Threshold low:          0xFFF                        (default configuration from reset state)
  *         - Oversampling
  *           - Scope:                  ADC group regular            (feature fixed on this STM32 serie: only ADC group regular is available)
  *           - Discontinuous mode:     disabled
  *           - Ratio:                  16
  *           - Shift:                  4
  *         - Interruptions
  *           None: with HAL driver, ADC interruptions are set using
  *           function "HAL_ADC_start_xxx()".
  * @note   Using HAL driver, configuration of GPIO used by ADC channels,
  *         NVIC and clock source at top level (RCC)
  *         are not implemented into this function,
  *         must be implemented into function "HAL_ADC_MspInit()".
  * @param  None
  * @retval None
  */
__STATIC_INLINE void Configure_ADC(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## common to several ADC, ADC instance, ADC group regular  ###############*/
  
  /* Set ADC instance of HAL ADC handle AdcHandle */
  AdcHandle.Instance = ADCx;
  
  /* Configuration of HAL ADC handle init structure:                          */
  /* parameters of scope ADC instance and ADC group regular.                  */
  /* Note: On this STM32 serie, ADC group regular sequencer has               */
  /*       two settings depending on parameter "ScanConvMode":                */
  /*       - sequencer configured to fully configurable:                      */
  /*         sequencer length and each rank                                   */
  /*         affectation to a channel are configurable.                       */
  /*       - sequencer configured to not fully configurable:                  */
  /*         sequencer length and each rank affectation to a channel          */
  /*         are fixed by channel HW number.                                  */
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer set to fully configurable: only the rank 1 is enabled (no scan sequence on several ranks) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled. Parameter relevancy depending on setting of parameter "ScanConvMode" */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig of conversion by software start (no external event) */
  AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* ADC with DMA transfer: continuous requests to DMA disabled (default state) since DMA is not used in this example. */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  AdcHandle.Init.SamplingTimeCommon1   = LL_ADC_SAMPLINGTIME_39CYCLES_5;
  AdcHandle.Init.OversamplingMode      = ENABLE;
  AdcHandle.Init.Oversampling.Ratio         = ADC_OVERSAMPLING_RATIO_16;
  AdcHandle.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
  AdcHandle.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  AdcHandle.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## ADC group injected and channels mapped on group injected ##############*/
  
  /* Note: Feature not available on this STM32 serie */ 
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## channels mapped on group regular         ##############################*/
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: On this STM32 serie, ADC group regular sequencer has               */
  /*       two settings depending on parameter "ScanConvMode":                */
  /*       Refer to comment above.                                            */
  /*       This setting will impact configuration of channels in              */
  /*       ADC group regular sequencer.                                       */
  sConfig.Channel      = ADCx_CHANNELa;               /* ADC channel selection */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* ADC group regular rank in which is mapped the selected ADC channel */
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;   /* ADC channel sampling time */
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  /* Note: Feature not available on this STM32 serie */ 
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: ADC analog watchdog not used and not configured in this example.   */
  /*       Refer to other ADC examples using this feature.                    */
  
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
  /* Set DAC Channel data register: channel corresponding to ADC channel ADC_CHANNEL_9 */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V */
  if (HAL_DAC_SetValue(&hdac1, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, DIGITAL_SCALE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel ADC_CHANNEL_9 */
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
  
  /* Set DAC voltage on channel corresponding to ADC_CHANNEL_9              */
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

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == TAMPER_BUTTON_PIN)
 {
   /* Set variable to report push button event to main program */
   ubUserButtonClickEvent = SET;
 }
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
    /* Toggle LED1 */
    BSP_LED_Off(LED1);
    HAL_Delay(800);
    BSP_LED_On(LED1);
    HAL_Delay(10);
    BSP_LED_Off(LED1);
    HAL_Delay(180);
    BSP_LED_On(LED1);
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
