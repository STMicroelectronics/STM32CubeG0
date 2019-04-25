/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32G0xx SMBUS HAL API.
  *          The communication is done using an Temperature Sensor STLM75
  *          on STM32G081B-EVAL Eval board.
  *          =================================================================== 
  *          Notes:
  *           - The Temperature Sensor (STLM75) is compatible
  *             with the SMBUS protocol.
  *          ===================================================================
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
#include "stlogo.h"
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
SMBUS_HandleTypeDef hsmbus1;

/* USER CODE BEGIN PV */
/* Private define ------------------------------------------------------------*/
#define TSENSOR_ADDR            0x90    /* STLM75 Address  */
#define TEMPERATURE_LOW         26      /* 26°C             */
#define TEMPERATURE_HIGH        28      /* 28°C             */

/* Private variables ---------------------------------------------------------*/
TSENSOR_InitTypeDef TSENSOR_InitStructure;

/* Useful variables */
uint16_t temperaturevalue = 0;
uint8_t  alertoccurs = 0;
uint8_t  requestsample = 0;
uint32_t tick = 0;
uint8_t  addressalert = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_SMBUS_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void Display_ExampleDescription(void);
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(uint16_t Temperature);

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

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_SMBUS_Init();
  /* USER CODE BEGIN 2 */

  /* Configure Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  BSP_LCD_Init();
   
  /*##-2- Display welcome messages on LCD ####################################*/  
  Display_ExampleDescription();

  /* Wait for Tamper push-button press before starting the Example */
  while (BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_RESET)
  {
  }
  
  /* Wait for Tamper push-button release before starting the Example */
  while (BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_SET)
  {
  }

  /*##-3- Display Example Template ###########################################*/  
  TSENSOR_SetHint();

  /*##-4- Configure the Temperature Sensor ###################################*/
  /* Conversion 12 bits in continuous mode at one conversion per second */
  /* Alert outside range Limit Temperature */
  TSENSOR_InitStructure.AlertMode             = TSENSOR_INTERRUPT_MODE;
  TSENSOR_InitStructure.ConversionMode        = TSENSOR_CONTINUOUS_MODE;
  TSENSOR_InitStructure.TemperatureLimitHigh  = TEMPERATURE_HIGH;
  TSENSOR_InitStructure.TemperatureLimitLow   = TEMPERATURE_LOW;

  if (TSENSOR_Init(TSENSOR_ADDR, &TSENSOR_InitStructure) != TSENSOR_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    Error_Handler();
  }

  /* Initialize tick counter */
  tick = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /*##-5- Main loop to manage Alert and display Temperature Measured #########*/
  while (1)
  {
    /* Check if a new temperature read is requested to display                */
    if (requestsample == 1)
    {
      /* Read and Display the current temperature                             */
      temperaturevalue = TSENSOR_ReadTemp(TSENSOR_ADDR);

      /* If integer temperature is inside threshold LOW and HIGH temperature, clear warning message */
      if (((temperaturevalue >> 4) > TEMPERATURE_LOW) && ((temperaturevalue >> 4) < TEMPERATURE_HIGH))
      {
        /* Clear previous warning message */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"                       ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"                       ", CENTER_MODE);
      }

      TSENSOR_Display_Temperature(temperaturevalue);
      
      /* Reset the sampling request                                           */
      tick = HAL_GetTick();
      requestsample = 0;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms                   */
      if (HAL_GetTick() >= tick + 1000)
      {
        /* Set the sampling request                                           */
        requestsample = 1;
      }
    }
    
    /* Check if an alert occurs                                               */
    if (alertoccurs == 1)
    {
      /* Display warning message depends on Limit value                       */
      if ((TSENSOR_ReadTemp(TSENSOR_ADDR) >> 4) >= TEMPERATURE_HIGH)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        /* Display warning message Temperature high limit exceeded            */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Temperature Limit High", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"has been exceeded", CENTER_MODE);
      }
      else
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        /* Display warning message Temperature is at or blow low limit        */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)" Temperature is at or ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"below the Low Limit", CENTER_MODE);
      }
      alertoccurs = 0;
    }
        
    HAL_Delay(5);
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_SMBUS_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hsmbus1.Instance = I2C1;
  hsmbus1.Init.Timing = 0x00303D5B;
  hsmbus1.Init.AnalogFilter = SMBUS_ANALOGFILTER_ENABLE;
  hsmbus1.Init.OwnAddress1 = 2;
  hsmbus1.Init.AddressingMode = SMBUS_ADDRESSINGMODE_7BIT;
  hsmbus1.Init.DualAddressMode = SMBUS_DUALADDRESS_DISABLE;
  hsmbus1.Init.OwnAddress2 = 0;
  hsmbus1.Init.OwnAddress2Masks = SMBUS_OA2_NOMASK;
  hsmbus1.Init.GeneralCallMode = SMBUS_GENERALCALL_DISABLE;
  hsmbus1.Init.NoStretchMode = SMBUS_NOSTRETCH_DISABLE;
  hsmbus1.Init.PacketErrorCheckMode = SMBUS_PEC_DISABLE;
  hsmbus1.Init.PeripheralMode = SMBUS_PERIPHERAL_MODE_SMBUS_HOST;
  hsmbus1.Init.SMBusTimeout = 0x000080C3;
  if (HAL_SMBUS_Init(&hsmbus1) != HAL_OK)
  {
    Error_Handler();
  }
  /** configuration Alert Mode 
  */
  if (HAL_SMBUS_EnableAlert_IT(&hsmbus1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32G081RBTx", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Example", CENTER_MODE);
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2018", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 15, (uint8_t *)"Press Tamper push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"to start :", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)" TEMPERATURE SENSOR Example", CENTER_MODE);
}

/**
  * @brief  Display TSENSOR Demo Hint
  * @param  None
  * @retval None
  */
static void TSENSOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"TEMPERATURE SENSOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"read a Temperature", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"and manage temperature limit alert", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  /* Prepare LCD to display */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t Temperature)
{
  uint8_t tempcelsiusdisplay[] = "+abc.dddd C";
  uint16_t temperaturevalue = Temperature;
  uint16_t temperaturevaluecelsius = 0;
  uint32_t tempcelsius = 0;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);

  /* Verify the sign of the temperature */
  if (temperaturevalue <= 2048)
  {
    /* Positive temperature measured */
    tempcelsiusdisplay[0] = '+';
    /* Initialize the temperature sensor value*/
    temperaturevaluecelsius = temperaturevalue;
    /* Set Text color to Green */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);    
  }
  else
  {
    /* Negative temperature measured */
    tempcelsiusdisplay[0] = '-';
    /* Remove temperature value sign */
    temperaturevaluecelsius = 0x1000 - temperaturevalue;
    /* Set Text color to Red */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  }

  tempcelsius = 0;
  /* Calculate temperature digits in °C */
  if (temperaturevaluecelsius & 0x01)
  {
    tempcelsius += 625;     
  }
  if (temperaturevaluecelsius & 0x02)
  {
    tempcelsius += 1250;
  }
  if (temperaturevaluecelsius & 0x04)
  {
    tempcelsius += 2500;
  }
  if (temperaturevaluecelsius & 0x08)
  {
    tempcelsius += 5000;
  }
  
  tempcelsiusdisplay[5] = (tempcelsius / 1000) + 0x30;
  tempcelsiusdisplay[6] = ((tempcelsius % 1000) / 100) + 0x30;
  tempcelsiusdisplay[7] = (((tempcelsius % 1000) % 100) / 10)+ 0x30;
  tempcelsiusdisplay[8] = (((tempcelsius % 1000) % 100) % 10) + 0x30;
    
  temperaturevaluecelsius >>= 4;

  tempcelsiusdisplay[1] = (temperaturevaluecelsius / 100) + 0x30;
  tempcelsiusdisplay[2] = ((temperaturevaluecelsius % 100) / 10) + 0x30;
  tempcelsiusdisplay[3] = ((temperaturevaluecelsius % 100) % 10) + 0x30;

  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, tempcelsiusdisplay, CENTER_MODE); 
}

/**
  * @brief  TSENSOR error callbacks.
  * @param  Error : Temperature Sensor Error status.
  * @retval None
  */
void TSENSOR_ErrorCallback(uint16_t Error)
{
  if (Error == TSENSOR_ALERT)
  {
    alertoccurs = 1;
  }
  else
  {
    Error_Handler();
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
  /* Clear Previous message on the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);

  /* Display Communication error message */
  BSP_LCD_SetTextColor(LCD_COLOR_RED);    
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"An error occurs during", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"communication with", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"the temperature sensor", CENTER_MODE);

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
