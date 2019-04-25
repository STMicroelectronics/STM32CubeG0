/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CEC/CEC_ListenMode_Device_1/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use the CEC through 
  *          the STM32G0xx HAL API.
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
CEC_HandleTypeDef hcec;

uint8_t Tab_Rx[16];
/* USER CODE BEGIN PV */
uint8_t  Tab_Tx[CEC_MAX_PAYLOAD-1]; /* Transmitted data buffer. 
                                     * header is not included in Tab_Tx.
                                     *  Max size = 15 bytes.                                   
                                     *  one opcode followed by up to 14 operands.
                                     *  When payload size = 0, only the header is sent
                                     *  (ping operation) */                                   
uint8_t ReceivedFrame       = 0x0;  /* Set when a reception occurs */
uint16_t NbOfReceivedBytes  = 0x0;  /* Number of received bytes in addition to the header. 
                                     * when a ping message has been received (header 
                                     * only), NbOfReceivedBytes = 0 */
uint8_t StartSending        = 0x0;  /* Set when a transmission is triggered by the user */
uint32_t TxSize             = 0x0;  /* Number of bytes to transmit in addition to the header. 
                                     * In case of ping operation (only the header sent),
                                     * TxSize = 0 */
uint8_t DestinationAddress = 0;     /* Destination logical address */  
uint8_t InitiatorAddress = 0;       /* Initiator logical address */  
__IO uint8_t TxStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_HDMI_CEC_Init(void);
/* USER CODE BEGIN PFP */
static void CEC_FlushRxBuffer(void);
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
  MX_GPIO_Init();
  MX_HDMI_CEC_Init();
  /* USER CODE BEGIN 2 */
  /* -1- Initialize LEDs mounted on EVAL board */
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* -2- Configure Tamper push-button in Interrupt mode */
  /* button-triggered interruptions initialization */
  BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);
  
  /* -3- Configure Joystick Selection push-button in Interrupt mode */
  BSP_JOY_Init(JOY_MODE_EXTI);

  /* CEC device initialization */
  
  DestinationAddress = 0x3; /* follower address */
  InitiatorAddress = 0x1;

  /* -4- CEC transfer general variables initialization */
  ReceivedFrame = 0;
  StartSending = 0;
  NbOfReceivedBytes = 0;
  CEC_FlushRxBuffer();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* if no reception has occurred and no error has been detected,
     * transmit a message if the user has pushed a button */
     while( (StartSending == 1) && (ReceivedFrame == 0))
    { 
      HAL_CEC_Transmit_IT(&hcec,InitiatorAddress ,DestinationAddress, (uint8_t *)&Tab_Tx, TxSize);
      /* loop until TX ends or TX error reported */
        while (TxStatus != 1);
      StartSending = 0;
      TxStatus = 0;
    }  
    
    /* if a frame has been received */
    if (ReceivedFrame == 1)
    { 
      if (Tab_Rx[1] == 0x44) /* Test on the opcode value */
      {
        /* Receive command is equal to Volume Up(Button Up) */
        if (Tab_Rx[2] == 0x41) /* Test on the operand value */
        {
          BSP_LED_On(LED1);
          BSP_LED_On(LED2);
          BSP_LED_On(LED3);
          BSP_LED_On(LED4);
        }
        else if (Tab_Rx[2] == 0x42) /* Receive command is equal to Volume Down(Button Down) */
        {
          BSP_LED_Off(LED1);
          BSP_LED_Off(LED2);
          BSP_LED_Off(LED3);
          BSP_LED_Off(LED4);
        }
      }
      else if (Tab_Rx[1] == 0x46) /* Test on the opcode value */
      {
        BSP_LED_On(LED1);
        BSP_LED_On(LED2);
        BSP_LED_Off(LED3);
        BSP_LED_Off(LED4);
      }
      else if (Tab_Rx[1] == 0x9F) /* Test on the opcode value */
      {
        BSP_LED_Off(LED1);
        BSP_LED_Off(LED2);
        BSP_LED_On(LED3);
        BSP_LED_On(LED4);
      }
      ReceivedFrame = 0;
      CEC_FlushRxBuffer();
    }
    else if (ReceivedFrame == 2) /* means CEC error detected */
    { 
      /* Turn on LED3 */
      BSP_LED_On(LED3);
      ReceivedFrame = 0;
    }

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
  * @brief HDMI_CEC Initialization Function
  * @param None
  * @retval None
  */
static void MX_HDMI_CEC_Init(void)
{

  /* USER CODE BEGIN HDMI_CEC_Init 0 */

  /* USER CODE END HDMI_CEC_Init 0 */

  /* USER CODE BEGIN HDMI_CEC_Init 1 */

  /* USER CODE END HDMI_CEC_Init 1 */
  hcec.Instance = CEC;
  hcec.Init.SignalFreeTime = CEC_DEFAULT_SFT;
  hcec.Init.Tolerance = CEC_STANDARD_TOLERANCE;
  hcec.Init.BRERxStop = CEC_NO_RX_STOP_ON_BRE;
  hcec.Init.BREErrorBitGen = CEC_BRE_ERRORBIT_NO_GENERATION;
  hcec.Init.LBPEErrorBitGen = CEC_LBPE_ERRORBIT_NO_GENERATION;
  hcec.Init.BroadcastMsgNoErrorBitGen = CEC_BROADCASTERROR_NO_ERRORBIT_GENERATION;
  hcec.Init.SignalFreeTimeOption = CEC_SFT_START_ON_TXSOM;
  hcec.Init.ListenMode = CEC_REDUCED_LISTENING_MODE;
  hcec.Init.OwnAddress = CEC_OWN_ADDRESS_1;
  hcec.Init.RxBuffer = Tab_Rx;
  if (HAL_CEC_Init(&hcec) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN HDMI_CEC_Init 2 */

  /* USER CODE END HDMI_CEC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/**
  * @brief Tx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_TxCpltCallback(CEC_HandleTypeDef *hcec)
{
  /* End of transmission */
  TxStatus =1;
}

/**
  * @brief Rx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_RxCpltCallback(CEC_HandleTypeDef *hcec, uint32_t RxFrameSize)
{
  ReceivedFrame = 1;
}

/**
  * @brief CEC error callbacks
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_ErrorCallback(CEC_HandleTypeDef *hcec)
{
  ReceivedFrame = 2;
}

/**
  * @brief  Reset CEC reception buffer
  * @param  None
  * @retval None
  */
static void CEC_FlushRxBuffer(void)
{
  uint32_t cpt;
  
  for (cpt = CEC_MAX_PAYLOAD; cpt > 0; cpt--)
  {
    Tab_Rx[cpt-1] = 0;
  }
}

/**
  * @brief EXTI line Falling detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

  if(GPIO_Pin == TAMPER_BUTTON_PIN)
  {
    /* Toggle GREEN LED1 */
    BSP_LED_Toggle(LED1);

    Tab_Tx[0] = 0x44;
    Tab_Tx[1] = 0x42;
    TxSize    = 0x02;
    StartSending = 1;
  }
}

/**
  * @brief EXTI line Riding detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == DOWN_JOY_PIN)
  {
    /* Toggle BLUE LED4 */
    BSP_LED_Toggle(LED4);
    
    Tab_Tx[0] = 0x9F;
    TxSize    = 0x01;
    StartSending = 1;
  }
  if(GPIO_Pin == SEL_JOY_PIN)
  {
    /* Toggle ORANGE LED2 */
    BSP_LED_Toggle(LED2);
    
    Tab_Tx[0] = 0x46;
    TxSize    = 0x01;
    StartSending = 1;
  }
  
  if(GPIO_Pin == UP_JOY_PIN)
  {
    /* Toggle RED LED3 */
    BSP_LED_Toggle(LED3);
    
    Tab_Tx[0] = 0x44;
    Tab_Tx[1] = 0x41;
    TxSize    = 0x02;
    StartSending = 1;    
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);

  while(1) 
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
