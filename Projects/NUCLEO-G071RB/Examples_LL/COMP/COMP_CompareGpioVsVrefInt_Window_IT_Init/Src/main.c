/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use a pair of comparator peripherals
  *          to compare a voltage level applied on a GPIO pin
  *          versus 2 thresholds: the internal voltage reference (VrefInt)
  *          and a fraction the internal voltage reference (VrefInt/2),
  *          in interrupt mode.
  *          This example is based on the STM32G0xx COMP LL API;
  *          Peripheral initialization done using LL unitary services functions.
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


/* Constant voltage generation for this example purpose,                      */
/* using peripherals: DAC.                                                    */
/* Goal is to provide to user an intermediate voltage level to check          */
/* comparator window thresholds.                                              */
#define CONSTANT_VOLTAGE_GENERATION

#if defined(CONSTANT_VOLTAGE_GENERATION)
/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)
  
  /* Value of comparator thresholds based on VrefInt */
  /* (VrefInt voltage is approximatively 1.22V, refer to device datasheet     */
  /* for min/typ/max values).                                                 */
  /* (unit: mV).                                                              */
  #define VREFINT                          ((uint32_t)1220)
  #define COMP_WINDOW_THRESHOLD_HIGH       (VREFINT)        /* COMP1 input plus is selected to VrefInt in this exemple */
  #define COMP_WINDOW_THRESHOLD_LOW        (VREFINT /2)     /* COMP2 input plus is selected to 1/2 VrefInt in this exemple */
  
/* Definitions of data related to this example */
  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B))

/* Definitions of constant voltage generation values */
  /* Waveform generation: parameters of constant voltage */
  /* Constant voltage amplitude                                               */
  /* (unit: mV)                                                               */
  #define CONSTANT_VOLTAGE_AMPLITUDE       ((COMP_WINDOW_THRESHOLD_HIGH + COMP_WINDOW_THRESHOLD_LOW) / 2)

#endif /* CONSTANT_VOLTAGE_GENERATION */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_COMP1_Init(void);
static void MX_COMP2_Init(void);
/* USER CODE BEGIN PFP */
void     SystemClock_Config(void);
void     Activate_COMP_master_slave(void);
void     LED_On(void);
void     LED_Off(void);
#if defined(CONSTANT_VOLTAGE_GENERATION)
void     Generate_constant_voltage(void);
#endif /* CONSTANT_VOLTAGE_GENERATION */
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
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_COMP1_Init();
  MX_COMP2_Init();
  /* USER CODE BEGIN 2 */
  
#if defined(CONSTANT_VOLTAGE_GENERATION)
  /* Generate constant voltage to provide to user an intermediate voltage     */
  /* level to check comparator window thresholds.                             */
  Generate_constant_voltage();
#endif /* CONSTANT_VOLTAGE_GENERATION */
  
  /* Activate pair of comparators */
  Activate_COMP_master_slave();
  
  /* Lock comparator instance */
  //LL_COMP_Lock(COMP1);
  

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Note: LED state depending on COMP status is set into COMP IRQ handler, */
    /*        refer to function "ComparatorTrigger_Callback()".               */
    

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
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  };

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(16000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}

/**
  * @brief COMP1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_COMP1_Init(void)
{

  /* USER CODE BEGIN COMP1_Init 0 */

  /* USER CODE END COMP1_Init 0 */

  LL_COMP_InitTypeDef COMP_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  /**COMP1 GPIO Configuration  
  PC5   ------> COMP1_INP 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* COMP1 interrupt Init */
  NVIC_SetPriority(ADC1_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC1_COMP_IRQn);

  /* USER CODE BEGIN COMP1_Init 1 */

  /* USER CODE END COMP1_Init 1 */
  COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO1;
  COMP_InitStruct.InputMinus = LL_COMP_INPUT_MINUS_VREFINT;
  COMP_InitStruct.InputHysteresis = LL_COMP_HYSTERESIS_NONE;
  COMP_InitStruct.OutputPolarity = LL_COMP_OUTPUTPOL_NONINVERTED;
  COMP_InitStruct.OutputBlankingSource = LL_COMP_BLANKINGSRC_NONE;
  LL_COMP_Init(COMP1, &COMP_InitStruct);
  LL_COMP_SetPowerMode(COMP1, LL_COMP_POWERMODE_MEDIUMSPEED);
  LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP1), LL_COMP_WINDOWMODE_DISABLE);
  LL_COMP_SetCommonWindowOutput(__LL_COMP_COMMON_INSTANCE(COMP1), LL_COMP_WINDOWOUTPUT_EACH_COMP);

  /* Wait loop initialization and execution */
  /* Note: Variable divided by 2 to compensate partially CPU processing cycles */
  __IO uint32_t wait_loop_index = 0;
  wait_loop_index = (LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (1000000 * 2)));
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_17);
  LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
  LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
  /* USER CODE BEGIN COMP1_Init 2 */

  /* USER CODE END COMP1_Init 2 */

}

/**
  * @brief COMP2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_COMP2_Init(void)
{

  /* USER CODE BEGIN COMP2_Init 0 */

  /* USER CODE END COMP2_Init 0 */

  LL_COMP_InitTypeDef COMP_InitStruct = {0};

  /* COMP2 interrupt Init */
  NVIC_SetPriority(ADC1_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC1_COMP_IRQn);

  /* USER CODE BEGIN COMP2_Init 1 */

  /* USER CODE END COMP2_Init 1 */
  COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO1;
  COMP_InitStruct.InputMinus = LL_COMP_INPUT_MINUS_1_2VREFINT;
  COMP_InitStruct.InputHysteresis = LL_COMP_HYSTERESIS_LOW;
  COMP_InitStruct.OutputPolarity = LL_COMP_OUTPUTPOL_NONINVERTED;
  COMP_InitStruct.OutputBlankingSource = LL_COMP_BLANKINGSRC_NONE;
  LL_COMP_Init(COMP2, &COMP_InitStruct);
  LL_COMP_SetPowerMode(COMP2, LL_COMP_POWERMODE_MEDIUMSPEED);
  LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP2), LL_COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON);
  LL_COMP_SetCommonWindowOutput(__LL_COMP_COMMON_INSTANCE(COMP2), LL_COMP_WINDOWOUTPUT_EACH_COMP);

  /* Wait loop initialization and execution */
  /* Note: Variable divided by 2 to compensate partially CPU processing cycles */
  __IO uint32_t wait_loop_index = 0;
  wait_loop_index = (LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (1000000 * 2)));
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_18);
  LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_18);
  LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_18);
  /* USER CODE BEGIN COMP2_Init 2 */

  /* USER CODE END COMP2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

  /**/
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Perform comparator activation procedure of the pair of comparators
  *         for window mode (COMP instances: COMP1, COMP2).
  * @note   Operations:
  *         - Enable comparator instance master
  *         - Enable comparator instance slave
  *         - Wait for comparator startup time
  *           (required to reach propagation delay specification)
  * @param  None
  * @retval None
  */
void Activate_COMP_master_slave(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /* Enable comparator instance master */
  LL_COMP_Enable(COMP1);
  
  /* Enable comparator instance slave */
  LL_COMP_Enable(COMP2);
  
  /* Delay for comparator startup time.                                       */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_COMP_DELAY_STARTUP_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
}

#if defined(CONSTANT_VOLTAGE_GENERATION)
/**
  * @brief  Generate constant voltage for this example purpose,
  *         to provide to user a voltage available on board connectors.
  *         Generation using peripherals:
  *         - GPIO (analog output): pin PA.04
  *         - DAC: DAC1 channel1
  * @param  None
  * @retval None
  */
void Generate_constant_voltage(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by DAC channels ############################*/
  
  /* Enable GPIO Clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); 
  
  /* Configure GPIO in analog mode to be used as DAC output */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/ 
  /* Note: In this example, DAC interruptions are not activated.              */
  /*       If needed to be activated, refer to DAC examples.                  */
  
  /*## Configuration of DAC ##################################################*/
  
  /* Enable DAC clock (core clock) */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
  
  /* Set the mode for the selected DAC channel */
  // LL_DAC_SetMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_MODE_NORMAL_OPERATION);
  
  /* Select trigger source */
  LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIG_SOFTWARE);
  
  /* Set the output for the selected DAC channel */
  LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL, LL_DAC_OUTPUT_BUFFER_ENABLE, LL_DAC_OUTPUT_CONNECT_GPIO);
  
  /* Note: DAC channel output configuration can also be done using            */
  /*       LL unitary functions:                                              */
  // LL_DAC_SetOutputMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL);
  // LL_DAC_SetOutputBuffer(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_BUFFER_ENABLE);
  // LL_DAC_SetOutputConnection(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_CONNECT_GPIO);
  
  /* Set DAC mode sample-and-hold timings */
  // LL_DAC_SetSampleAndHoldSampleTime (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldHoldTime   (DAC1, LL_DAC_CHANNEL_1, 0x3FF);
  // LL_DAC_SetSampleAndHoldRefreshTime(DAC1, LL_DAC_CHANNEL_1, 0xFF);
  
  /* Disable DAC channel DMA request */
  // LL_DAC_DisableDMAReq(DAC1, LL_DAC_CHANNEL_1);
  
  /* Set the data to be loaded in the data holding register */
  //LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, 0x000);
  
  /*## Activation of DAC #####################################################*/
  
  /* Enable DAC channel */
  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
  
  /* Delay for DAC channel voltage settling time from DAC channel startup.    */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /* Enable DAC channel trigger */
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
  
  /*## Set DAC output voltage ################################################*/
  
  /* Set the data to be loaded in the data holding register */
  LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, CONSTANT_VOLTAGE_AMPLITUDE);
  
  /* Trig DAC conversion by software */
  LL_DAC_TrigSWConversion(DAC1, LL_DAC_CHANNEL_1);
  
}
#endif /* CONSTANT_VOLTAGE_GENERATION */


/**
  * @brief  Turn-on LED4.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED4 on */
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
}

/**
  * @brief  Turn-off LED4.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED4 off */
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
}



/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  Comparator interruption callback
  *         This function is executed when the comparator threshold
  *         is triggered.
  * @retval None
  */
void ComparatorTrigger_Callback()
{
  __IO uint32_t COMP1OutputLevel = LL_COMP_ReadOutputLevel(COMP1);
  __IO uint32_t COMP2OutputLevel = LL_COMP_ReadOutputLevel(COMP2);
  
  /* Set LED state in function of comparator output level */
  /* Case of voltage above comparators thresholds window */
  if (   (COMP1OutputLevel == LL_COMP_OUTPUT_LEVEL_LOW)
      && (COMP2OutputLevel == LL_COMP_OUTPUT_LEVEL_LOW)
     )
  {
    LED_Off();
  }
  /* Case of voltage below comparators thresholds window */
  else if (   (COMP1OutputLevel == LL_COMP_OUTPUT_LEVEL_HIGH)
           && (COMP2OutputLevel == LL_COMP_OUTPUT_LEVEL_HIGH)
     )
  {
    LED_Off();
  }
  /* Case of voltage inside comparators thresholds window */
  else /* COMP1 output high, COMP2 output low */
  {
    LED_On();
  }
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
