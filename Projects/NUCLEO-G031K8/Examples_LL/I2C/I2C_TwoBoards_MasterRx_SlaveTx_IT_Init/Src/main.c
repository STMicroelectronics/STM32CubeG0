/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over I2C IP using
  *          the STM32G0xx I2C LL API.
  *          Peripheral initialization done using LL unitary services functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
/**
  * @brief I2C devices settings
  */
/* Timing register value is computed with the STM32CubeMX Tool,
  * Fast Mode @400kHz with I2CCLK = 64 MHz,
  * rise time = 100ns, fall time = 10ns
  * Timing Value = (uint32_t)0x00C0216C
  */
#define I2C_TIMING               0x00C0216C

/**
  * @brief Define related to SlaveTransmit process
  */
#define SLAVE_BYTE_TO_SEND       (uint8_t)0xA5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#ifndef SLAVE_BOARD
/**
  * @brief Variables related to MasterReceive process
  */
uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;
__IO uint8_t ubButtonPress       = 0;
#endif /* SLAVE_BOARD */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
#ifndef SLAVE_BOARD
void     Configure_EXTI(void);
#endif /* MASTER_BOARD */
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
#ifdef SLAVE_BOARD
#else /* MASTER_BOARD */
void     WaitForUserButtonPress(void);
void     Handle_I2C_Master(void);
#endif /* SLAVE_BOARD */
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

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
#ifdef SLAVE_BOARD
#else
  /* Configure the EXTI Line on User Button */
  Configure_EXTI();

#endif /* SLAVE_BOARD */

  /* Set LED3 Off */
  LED_Off();

#ifdef SLAVE_BOARD
#else /* MASTER_BOARD */
  /* Wait for put and remove a jumper between PA.15 (Arduino D2) and GND to start transfer */
  WaitForUserButtonPress();

  /* Handle I2C1 events (Master) */
  Handle_I2C_Master();
#endif /* SLAVE_BOARD */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(64000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB9   ------> I2C1_SDA
  PB8   ------> I2C1_SCL
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */
  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x00303D5B;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 2;
  I2C_InitStruct.OwnAddress1 = 180;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */
#ifdef SLAVE_BOARD
  uint32_t timing = 0;
  timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0xC, 0x0, 0x21, 0x6C);
  LL_I2C_SetTiming(I2C1, timing);

  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);
  LL_I2C_EnableOwnAddress1(I2C1);


  LL_I2C_EnableIT_ADDR(I2C1);
#else
  LL_I2C_SetTiming(I2C1, I2C_TIMING);


  LL_I2C_EnableIT_RX(I2C1);
#endif
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);

  /**/
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#ifndef SLAVE_BOARD
/**
  * @brief  This function configures EXTI Line as Button
  * @note   Peripheral configuration is minimal configuration from reset values.
  * @param  None
  * @retval None
  */
void Configure_EXTI()
{
  /* -1- GPIO Config */
  /* Enable GPIO Clock (to be able to program the configuration registers) */
  VIRTUAL_BUTTON_GPIO_CLK_ENABLE();
  /* Configure IO */
  LL_GPIO_SetPinMode(VIRTUAL_BUTTON_GPIO_PORT, VIRTUAL_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(VIRTUAL_BUTTON_GPIO_PORT, VIRTUAL_BUTTON_PIN, LL_GPIO_PULL_UP);

  /* -2- Connect External Line to the GPIO*/
  VIRTUAL_BUTTON_SYSCFG_SET_EXTI();

  /*-3- Enable a rising trigger External line 15 Interrupt */
  VIRTUAL_BUTTON_EXTI_LINE_ENABLE();
  VIRTUAL_BUTTON_EXTI_RISING_TRIG_ENABLE();

  /*-4- Configure NVIC for EXTI4_15_IRQn */
  NVIC_EnableIRQ(VIRTUAL_BUTTON_EXTI_IRQn);
  NVIC_SetPriority(VIRTUAL_BUTTON_EXTI_IRQn, 0);
}

#endif /* MASTER_BOARD */

#ifdef SLAVE_BOARD

#endif /* SLAVE_BOARD */

/**
  * @brief  Turn-on LED3.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED3 on */
  LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

/**
  * @brief  Turn-off LED3.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED3 off */
  LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

/**
  * @brief  Set LED3 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED3 on */
  LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);

  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    LL_mDelay(Period);
  }
}
/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/
#ifdef SLAVE_BOARD
/**
  * @brief  Function called from I2C IRQ Handler when TXIS flag is set
  *         Function is in charge of transmit a byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Ready_To_Transmit_Callback(void)
{
  /* Send the Byte requested by the Master */
  LL_I2C_TransmitData8(I2C1, SLAVE_BYTE_TO_SEND);
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         LED3 is On if data are correct.
  * @param  None
  * @retval None
  */
void Slave_Complete_Callback(void)
{
  /* Turn LED3 On:
   *  - Expected bytes have been sent
   *  - Slave Tx sequence completed successfully
   */
  LED_On();
}
#else /* MASTER_BOARD */
/**
  * @brief  Wait for put and remove a jumper between PA.15 (Arduino D2) and GND to start transfer.
  * @param  None
  * @retval None
  */
/*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LED3 off */
  LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

/**
  * @brief  Function to manage PA.15 (Arduino D2)
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update PA.15 (Arduino D2) variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  This Function handle Master events to perform a reception process
  * @note  This function is composed in one step :
  *        -1- Initiate a Start condition to the Slave device.
  * @param  None
  * @retval None
  */
void Handle_I2C_Master(void)
{
  /* (1) Initiate a Start condition to the Slave device ***********************/

  /* Master Generate Start condition for a read request:
   *  - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *  - with a auto stop condition generation when receive 1 byte
   */
  LL_I2C_HandleTransfer(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
}

/**
  * @brief  Function called from I2C IRQ Handler when RXNE flag is set
  *         Function is in charge of reading byte received on I2C lines.
  * @param  None
  * @retval None
  */
void Master_Reception_Callback(void)
{
  /* Read character in Receive Data register.
  RXNE flag is cleared by reading data in RXDR register */
  aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C1);
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         Function is in charge of checking data received,
  *         LED3 is On if data are correct.
  * @param  None
  * @retval None
  */
void Master_Complete_Callback(void)
{
  /* Read Received character.
  RXNE flag is cleared by reading of RXDR register */
  if (aReceiveBuffer[ubReceiveIndex - 1] == SLAVE_BYTE_TO_SEND)
  {
    /* Turn LED3 On:
     *  - Expected byte has been received
     *  - Master Rx sequence completed successfully
     */
    LED_On();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}
#endif /* SLAVE_BOARD */

/**
  * @brief  Function called in case of error detected in I2C IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
#ifdef SLAVE_BOARD
  /* Disable I2C1_IRQn */
  NVIC_DisableIRQ(I2C1_IRQn);
#else
  /* Disable I2C1_IRQn */
  NVIC_DisableIRQ(I2C1_IRQn);
#endif /* SLAVE_BOARD */

  /* Unexpected event : Set LED3 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
