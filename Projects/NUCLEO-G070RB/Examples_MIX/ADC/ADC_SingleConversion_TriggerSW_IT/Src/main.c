/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a ADC peripheral to perform
  *          a single  ADC conversion of a channel, at each software start.
  *          Example using programming model: interrupt 
  *          (for programming models polling or DMA transfer, refer to
  *          other examples).
  *          This example is based on the STM32G0xx ADC HAL & LL API
  *          (LL API is used for performance improvement).
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
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3300U)

/* Definitions of data related to this example */
  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B))

  /* Init variable out of ADC expected conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* Peripherals handlers declaration */
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;


/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

/* Variables for ADC conversion data computation to physical values */
__IO uint16_t uhADCxConvertedData_Voltage_mVolt = 0;  /* Value of voltage calculated from ADC conversion data (unit: mV) */

/* Variable to report status of ADC group regular unitary conversion          */
/*  0: ADC group regular unitary conversion is not completed                  */
/*  1: ADC group regular unitary conversion is completed                      */
/*  2: ADC group regular unitary conversion has not been started yet          */
/*     (initial state)                                                        */
__IO uint8_t ubAdcGrpRegularUnitaryConvStatus = 2; /* Variable set into ADC interruption callback */

/* Variable to manage push button on board: interface between ExtLine interruption and main program */
__IO   uint8_t ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void Configure_ADC(void);

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
 
  /*## Configure peripherals #################################################*/
  
  /* Initialize LED on board */
  BSP_LED_Init(LED4);
  
  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
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
  
  
  
  
  /*## Enable peripherals ####################################################*/
  
  /* Note: ADC is enabled afterwards when starting ADC conversion using       */
  /*       function "HAL_ADC_Start_xxx()".                                    */
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Note: At this step, a voltage can be supplied to ADC channel input     */
    /*       (by connecting an external signal voltage generator to the       */
    /*       analog input pin) to perform a ADC conversion on a determined    */
    /*       voltage level.                                                   */
    /*       Otherwise, ADC channel input can be let floating, in this case   */
    /*       ADC conversion data will be undetermined.                        */
    
    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
    }
    /* Reset variable for next loop iteration */
    ubUserButtonClickEvent = RESET;
    
    /* Turn LED off before performing a new ADC conversion start */
    BSP_LED_Off(LED4);
    
    /* Reset status variable of ADC group regular unitary conversion before   */
    /* performing a new ADC group regular conversion start.                   */
    if (ubAdcGrpRegularUnitaryConvStatus != 0)
    {
      ubAdcGrpRegularUnitaryConvStatus = 0;
    }
    
    /* Init variable containing ADC conversion data */
    uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE;
    
    /*## Start ADC conversions ###############################################*/
    
    /* Start ADC group regular conversion with IT */
    /* Note: Perform initial ADC conversion start using driver HAL,           */
    /*       then following ADC conversion start using driver LL.             */
    /*       (mandatory to use driver LL after the first call of              */
    /*       ADC IRQ handler, implemented with driver LL).                    */
    if (LL_ADC_IsEnabled(ADCx) == 0)
    {
      if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
      {
        /* ADC conversion start error */
        Error_Handler();
      }
    }
    /* ########## Starting from this point HAL API must not be used ########## */
    else
    {
      /* Start ADC group regular conversion */
      /* Note: Hardware constraint (refer to description of the function          */
      /*       below):                                                            */
      /*       On this STM32 serie, setting of this feature is conditioned to     */
      /*       ADC state:                                                         */
      /*       ADC must be enabled without conversion on going on group regular,  */
      /*       without conversion stop command on going on group regular.         */
      /* Note: In this example, all these checks are not necessary but are        */
      /*       implemented anyway to show the best practice usages                */
      /*       corresponding to reference manual procedure.                       */
      /*       Software can be optimized by removing some of these checks, if     */
      /*       they are not relevant considering previous settings and actions    */
      /*       in user application.                                               */
      if ((LL_ADC_IsEnabled(ADCx) == 1)               &&
          (LL_ADC_IsDisableOngoing(ADCx) == 0)        &&
          (LL_ADC_REG_IsConversionOngoing(ADCx) == 0)   )
      {
        LL_ADC_REG_StartConversion(ADCx);
      }
      else
      {
        /* Error: ADC conversion start could not be performed */
        Error_Handler();
      }
    }
    
    /* Note: Variable "ubUserButtonClickEvent" is set into push button        */
    /*       IRQ handler, refer to function "HAL_GPIO_EXTI_Callback()".       */
    
    /* Note: ADC conversions data are stored into variable                    */
    /*       "uhADCxConvertedData".                                           */
    /*       (for debug: see variable content into watch window).             */
    

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

/* USER CODE BEGIN 4 */

/**
  * @brief  Configure ADC (ADC instance: ADCx) and GPIO used by ADC channels.
  * @note   Using HAL driver, configuration of GPIO used by ADC channels,
  *         NVIC and clock source at top level (RCC)
  *         are not implemented into this function,
  *         must be implemented into function "HAL_ADC_MspInit()".
  * @param  None
  * @retval None
  */
void Configure_ADC(void)
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
  AdcHandle.Init.OversamplingMode      = DISABLE;
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
  /* Note: Considering IT occurring after each ADC conversion                 */
  /*       (IT by ADC group regular end of unitary conversion),               */
  /*       select sampling time and ADC clock with sufficient                 */
  /*       duration to not create an overhead situation in IRQHandler.        */
  /* Note: On this STM32 serie, sampling time is common to                    */
  /*       groups of channels.                                                */
  /*       Refer to sampling time configured above, at ADC instance scope.    */
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
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Note: ADC oversampling not used and not configured in this example.      */
  /*       Refer to other ADC examples using this feature.                    */
  
}





/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief EXTI line detection callbacks
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
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

/* Note: Lines of code commented below correspond to the example using        */
/*       HAL driver only.                                                     */
/*       This example demonstrating a mix of HAL and LL drivers has replaced  */
/*       these lines using LL driver.                                         */
// /**
//   * @brief  Conversion complete callback in non blocking mode
//   * @param  AdcHandle : ADC handle
//   * @note   This function is executed when the ADC group regular 
//   *         sequencer has converted one rank of the sequence.
//   *         Therefore, this function is executed as many times as number
//   *         of ranks in the sequence.
//   * @note   This example shows a simple way to report end of conversion
//   *         and get conversion result. You can add your own implementation.
//   * @retval None
//   */
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
// {
//   /* Retrieve ADC conversion data */
//   uhADCxConvertedData = HAL_ADC_GetValue(AdcHandle);
//
//   /* Computation of ADC conversions raw data to physical values               */
//   /* using LL ADC driver helper macro.                                        */
//   uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);
//
//   /* Update status variable of ADC unitary conversion */
//   ubAdcGrpRegularUnitaryConvStatus = 1;
//
//   /* Set LED depending on ADC unitary conversion status */
//   /* - Turn-on if ADC group regular unitary conversion is completed */
//   /* - Turn-off if ADC group regular unitary conversion is not completed */
//   BSP_LED_On(LED4);
// }
//
// /**
//   * @brief  ADC error callback in non blocking mode
//   *        (ADC conversion with interruption or transfer by DMA)
//   * @param  hadc: ADC handle
//   * @retval None
//   */
// void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
// {
//   /* In case of ADC error, call main error handler */
//   Error_Handler();
// }

/**
  * @brief  ADC group regular end of unitary conversion interruption callback
  * @note   This function is executed when the ADC group regular 
  *         sequencer has converted one rank of the sequence.
  *         Therefore, this function is executed as many times as number
  *         of ranks in the sequence.
  * @retval None
  */
void AdcGrpRegularUnitaryConvComplete_Callback()
{
  /* Retrieve ADC conversion data */
  /* (data maximum amplitude corresponds to ADC resolution: 12 bits) */
  uhADCxConvertedData = LL_ADC_REG_ReadConversionData12(ADCx);
  
  /* Computation of ADC conversions raw data to physical values               */
  /* using LL ADC driver helper macro.                                        */
  uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);
  
  /* Update status variable of ADC unitary conversion */
  ubAdcGrpRegularUnitaryConvStatus = 1;
  
  /* Set LED depending on ADC unitary conversion status */
  /* - Turn-on if ADC group regular unitary conversion is completed */
  /* - Turn-off if ADC group regular unitary conversion is not completed */
  BSP_LED_On(LED4);
  
}

/**
  * @brief  ADC group regular overrun interruption callback
  * @note   This function is executed when ADC group regular
  *         overrun error occurs.
  * @retval None
  */
void AdcGrpRegularOverrunError_Callback(void)
{
  /* Note: Disable ADC interruption that caused this error before entering in */
  /*       infinite loop below.                                               */
  
  /* Disable ADC group regular overrun interruption */
  LL_ADC_DisableIT_OVR(ADCx);
  
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
  
  /* In case of error, LED4 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(500);
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
