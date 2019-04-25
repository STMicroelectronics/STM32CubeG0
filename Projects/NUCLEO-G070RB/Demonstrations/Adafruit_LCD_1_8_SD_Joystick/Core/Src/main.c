/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Demonstrations/Adafruit_LCD_1_8_SD_Joystick/Src/main.c 
  * @author  MCD Application Team
  * @brief   This demo describes how display bmp images from SD card on LCD using
             the Adafruit 1.8" TFT shield with Joystick and microSD mounted on top
             of the STM32 Nucleo board.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum 
{
  SHIELD_NOT_DETECTED = 0, 
  SHIELD_DETECTED
}ShieldStatus;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t BlinkSpeed = 0, str[20];
__IO uint8_t JoystickValue = 0;
char* pDirectoryFiles[MAX_BMP_FILES];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void LED4_Blink(void);
static ShieldStatus TFT_ShieldDetect(void);
static void TFT_DisplayMenu(void);
static void TFT_DisplayImages(void);
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
  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PB.01 pin (mapped to JoyStick
     available on adafruit 1.8" TFT shield). If the state of PB.01 is high then
     the adafruit 1.8" TFT shield is available. */  
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
    /* Initialize the LCD */
    BSP_LCD_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  if (MX_FATFS_Init() != APP_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN 2 */
    
    /* Display on TFT Images existing on SD card */
    TFT_DisplayImages();
  }  
  else /* Shield not mounted */
  {   
    LED4_Blink();
  } 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if (MX_FATFS_Process() != APP_OK)
	{
      Error_Handler();
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
  * @brief  Displays demonstration menu.
  * @param  None
  * @retval None
  */
static void TFT_DisplayMenu(void)
{
  JOYState_TypeDef tmp = JOY_NONE;
  
  /* Set Menu font */
  BSP_LCD_SetFont(&Font12);

  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)" NUCLEO-STM32G0xx   ");
  BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"       DEMO         ");
  
  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */  
  BSP_LCD_DisplayStringAtLine(4, (uint8_t*)" Display images      ");
  BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" stored under uSD    ");
  BSP_LCD_DisplayStringAtLine(8, (uint8_t*)" on TFT LCD          ");
  
  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  /* Display message */ 
  BSP_LCD_DisplayStringAtLine(11, (uint8_t*)"  Press JOY DOWN   ");
  BSP_LCD_DisplayStringAtLine(12, (uint8_t*)"  to continue...   ");
 
  /* Wait for JOY_DOWN is pressed */
  while (BSP_JOY_GetState() != JOY_DOWN)
  {
  }
  
  /* Wait for JOY_DOWN is released */
  while (BSP_JOY_GetState() == JOY_DOWN)
  {
  }
  
  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);  
  /* Display message */ 
  BSP_LCD_DisplayStringAtLine(4,  (uint8_t*)"                   ");
  BSP_LCD_DisplayStringAtLine(6,  (uint8_t*)"  Press Joystick   ");
  
  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */ 
  BSP_LCD_DisplayStringAtLine(8,  (uint8_t*)"  UP for:          ");
  BSP_LCD_DisplayStringAtLine(9,  (uint8_t*)"  Manual Mode      ");
  BSP_LCD_DisplayStringAtLine(11, (uint8_t*)"  DOWN for:        ");
  BSP_LCD_DisplayStringAtLine(12, (uint8_t*)"  Automatic Mode   ");

  /* Wait for JOY_DOWN or JOY_UP is pressed */
  tmp = JOY_RIGHT;
  while ((tmp != JOY_DOWN) && (tmp != JOY_UP))
  {
    tmp = BSP_JOY_GetState();
  }
  
  /* LCD Clear */
  BSP_LCD_Clear(LCD_COLOR_WHITE); 
  
  /* JOY_UP is pressed: Display Manual mode menu #############################*/
  if(tmp == JOY_UP)
  {
    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    /* Display message */ 
    BSP_LCD_DisplayStringAtLine(3,  (uint8_t*)"   Manual Mode   ");
    BSP_LCD_DisplayStringAtLine(5,  (uint8_t*)"    Selected     "); 
    
    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);      
    /* Display message */
    BSP_LCD_DisplayStringAtLine(9,  (uint8_t*)" RIGHT: Next image"); 
    BSP_LCD_DisplayStringAtLine(10, (uint8_t*)" LEFT : Previous  ");
    BSP_LCD_DisplayStringAtLine(11, (uint8_t*)" SEL  : Switch to ");
    BSP_LCD_DisplayStringAtLine(12, (uint8_t*)" automatic mode   ");    
    JoystickValue = 2;  
  }
  /* JOY_DOWN is pressed: Display Automatic mode menu ########################*/
  else if (tmp == JOY_DOWN)    
  {
    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    /* Display message */ 
    BSP_LCD_DisplayStringAtLine(3,  (uint8_t*)"  Automatic Mode  ");
    BSP_LCD_DisplayStringAtLine(5,  (uint8_t*)"     Selected     ");
    

    JoystickValue = 1;  
    HAL_Delay(200);
  }
}

/**
  * @brief  Displays on TFT Images or error messages when error occurred.
  * @param  None
  * @retval None
  */
static void TFT_DisplayImages(void)
{    
  uint32_t bmplen = 0x00;
  uint32_t checkstatus = 0x00;
  uint32_t filesnumbers = 0x00;
  uint32_t joystickstatus = JOY_NONE;
  uint32_t bmpcounter = 0x00;
  uint32_t tickstart;
  DIR directory;
  FRESULT res;
  
  /* Initialize the Joystick available on adafruit 1.8" TFT shield */
  BSP_JOY_Init();
  
  /* Welcome message */
  TFT_DisplayMenu(); 
  
  /* Open directory */
  res = f_opendir(&directory, "/");
  if((res != FR_OK))
  {
    if(res == FR_NO_FILESYSTEM)
    {
      /* Display message: SD card not FAT formated */
      TFT_DisplayErrorMessage(SD_CARD_NOT_FORMATTED);    
    }
    else
    {
      /* Display message: Fail to open directory */
      TFT_DisplayErrorMessage(SD_CARD_OPEN_FAIL);  
    }
  }
  
  /* Get number of bitmap files */
  filesnumbers = Storage_GetDirectoryBitmapFiles ("/", pDirectoryFiles);    
  /* Set bitmap counter to display first image */
  bmpcounter = 1;
  tickstart = HAL_GetTick();
  
  while (1)
  {     
    /* Ensure a small tempo between images display unless a user action occurs */
    while ( ((HAL_GetTick() - tickstart) < 1500) && (joystickstatus == JOY_NONE) )
    {
      /* Get JoyStick status */    
      joystickstatus = BSP_JOY_GetState();
      HAL_Delay(200);
    }
    
    if(joystickstatus == JOY_SEL)
    {      
      JoystickValue++;
      if (JoystickValue > 2)
      {
        JoystickValue = 1;
      }
    }
    
    tickstart = HAL_GetTick();

    /*## Display BMP pictures in Automatic mode ##############################*/
    if(JoystickValue == 1) 
    {
      sprintf((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter -1]);
      
      checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
      
      if(checkstatus == 0)
      {
        /* Format the string */
        checkstatus = Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str);
      }
      
      if (checkstatus == 1)
      {
        /* Display message: File not supported */
        TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
      }

      bmpcounter++;
      if(bmpcounter > filesnumbers)
      {
        bmpcounter = 1;
      }
    }   
    
    /*## Display BMP pictures in Manual mode #################################*/
    if(JoystickValue == 2)
    {
      if(joystickstatus == JOY_RIGHT)
      {
        if((bmpcounter + 1) > filesnumbers)
        {
          bmpcounter = 1;
        }
        else
        {
          bmpcounter++;
        }
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter - 1]);
        
        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
        
        if(checkstatus == 0)
        {
          /* Format the string */
          Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str); 
        }
        
        if(checkstatus == 1)
        {
          /* Display message: File not supported */
          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED); 
        }
        JoystickValue = 2;          
      }
      else if(joystickstatus == JOY_LEFT)
      {
        if((bmpcounter - 1) == 0)
        {
          bmpcounter = filesnumbers;
        }
        else
        {
          bmpcounter--;
        }
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter - 1]); 
        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
        
        if(checkstatus == 0)
        {
          /* Format the string */
          Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str); 
        }
        
        if (checkstatus == 1)
        {
          /* Display message: File not supported */
          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
        }
        JoystickValue = 2;
      }
    }

    joystickstatus = JOY_NONE;
  }  
}

/**
  * @brief  Displays adequate message on TFT available on adafruit 1.8" TFT shield  
  * @param  message: Error message to be displayed on the LCD.
  *   This parameter can be one of following values:   
  *     @arg SD_CARD_NOT_FORMATTED: SD CARD is not FAT formatted
  *     @arg SD_CARD_FILE_NOT_SUPPORTED: File is not supported
  *     @arg SD_CARD_OPEN_FAIL: Failure to open directory
  *     @arg FATFS_NOT_MOUNTED: FatFs is not mounted
  * @retval None
  */
void TFT_DisplayErrorMessage(uint8_t message)
{
  /* LCD Clear */
  BSP_LCD_Clear(LCD_COLOR_WHITE); 
  /* Set Error Message Font */
  BSP_LCD_SetFont(&Font12);
  /* Set Text and Back colors */
  BSP_LCD_SetBackColor(LCD_COLOR_GREY); 
  BSP_LCD_SetTextColor(LCD_COLOR_RED);

  if(message == SD_CARD_NOT_FORMATTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)" SD Card is not    ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" FAT formatted.    ");  
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" Please Format the ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)" microSD card.     ");
    while (1)
    {
    }    
  }
  if(message == SD_CARD_FILE_NOT_SUPPORTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" File type is not  ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" supported.        ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }    
  }
  if(message == SD_CARD_OPEN_FAIL)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" Open directory    ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" fails.            ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }     
  }
  if(message == FATFS_NOT_MOUNTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" Cannot mount      ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" FatFs on Drive.   "); 
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while (1)
    {
    }  
  }
  if(message == BSP_SD_INIT_FAILED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" SD Init           ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" fails.            ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }     
  }
}

/**
  * @brief  Blinks LED4 with two frequencies depending on User press button.
  * @param  None
  * @retval None
  */
static void LED4_Blink(void)
{
  /* Configure LED4 on Nucleo */
  BSP_LED_Init(LED4);

  /* Configure the User Button in EXTI Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  /* Initiate BlinkSpeed variable */ 
  BlinkSpeed = 0;  
  
  /* Infinite loop */
  while(1)
  {
    /* Test on blink speed */
    if(BlinkSpeed == 0)
    {
      BSP_LED_Toggle(LED4);
      /* Wait for 500ms */      
      HAL_Delay(500);      
    }      
    else if(BlinkSpeed == 1)
    {
      BSP_LED_Toggle(LED4);
      /* Wait for 100ms */
      HAL_Delay(100); 
    }
    else if(BlinkSpeed == 2)
    {
      BSP_LED_Toggle(LED4);    
      /* wait for 50ms */
      HAL_Delay(50);  
    }
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if(BlinkSpeed == 2)
  {
    BlinkSpeed = 0;
  }
  else
  {
    BlinkSpeed ++;
  }
}

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PB.01 pin (mapped to 
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PB.01 
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0}; 

  /* Enable GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) != 0)  
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
