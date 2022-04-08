/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

#define AES_LOW_PRIO_TEXT_SIZE        128  /* in 32-bit long words */
#define AES_HIGH_PRIO_TEXT_SIZE        16  /* in 32-bit long words */

#define KEY_128_SIZE                    4  /* in 32-bit long words */
#define KEY_256_SIZE                    8  /* in 32-bit long words */
#define IV_SIZE                         4  /* in 32-bit long words */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyAES[8] __ALIGN_END = {
                            0x603DEB10,0x15CA71BE,0x2B73AEF0,0x857D7781,0x1F352C07,0x3B6108D7,0x2D9810A3,0x0914DFF4};
__ALIGN_BEGIN static const uint32_t pInitVectAES[4] __ALIGN_END = {
                            0x00010203,0x04050607,0x08090A0B,0x0C0D0E0F};

/* USER CODE BEGIN PV */
uint32_t suspension_requested;
uint32_t IT_call_nb;              /* Counter of CRYP interruptions, used
                                   to indicate suspension occurrence when
                                   low priority data are entered
                                   under interruption                      */

 uint32_t AESKey128[KEY_128_SIZE] = {
                          0x2B7E1516, 0x28AED2A6, 0xABF71588, 0x09CF4F3C};

/* Initialization vector */
uint32_t AESIV_HighPrio[IV_SIZE] = {0xF0F1F2F3 , 0xF4F5F6F7 , 0xF8F9FAFB , 0xFCFDFEFF};

/* Low priority message  ===================================================================*/

static uint32_t CiphertextAESCBC256_16[AES_LOW_PRIO_TEXT_SIZE] = {
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A,
                                       0x4C04F58C ,0xF1BAD6E5 ,0xABFB779E ,0xFBD65F7B , 0x4E969CFC ,0x808D7EDB ,0x777B679F ,0x2C7DC670 ,
                                       0x336939F2 ,0xBACFA9D9 ,0xE263A530 ,0x14610423 , 0x05E2B2EB ,0xE9FCC39B ,0x1907DA6C ,0x9D1B8C6A};

static uint32_t Plaintext_16[AES_LOW_PRIO_TEXT_SIZE] = {
                                      0xBEE26BC1, 0x9F962E40, 0x7E11E93D, 0x172A7393, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C,
                                      0xB903D92B, 0x706DE9DE, 0x6D1D3B58, 0x843EF3F4, 0x8A57AE2D, 0xAC9C1E03, 0x6FAC9EB7, 0x8E5145AF,
                                      0x1C4630C8, 0xE411A35C, 0xC119E5FB, 0x52EF1A0A, 0x2445F69F, 0x9B17DF4F, 0x417BAD2B, 0x3710E66C};


/* High priority message  ===================================================================*/

uint32_t Plaintext_HighPrio[AES_HIGH_PRIO_TEXT_SIZE] = {
                          0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,
                          0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                          0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                          0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

uint32_t CiphertextAESCTR256_HighPrio[AES_HIGH_PRIO_TEXT_SIZE] = {
                          0x874D6191 ,0xB620E326 ,0x1BEF6864 ,0x990DB6CE ,
                          0x9806F66B ,0x7970FDFF ,0x8617187B ,0xB9FFFDFF ,
                          0x5AE4DF3E ,0xDBD5D35E ,0x5B4F0902 ,0x0DB03EAB ,
                          0x1E031DDA ,0x2FBE03D1 ,0x792170A0 ,0xF3009CEE};


/*============================================================================*/

/* Used to store the low-priority decrypted text */
uint32_t Decryptedtext[AES_LOW_PRIO_TEXT_SIZE]={0};

/* Used to store the high-priority encrypted text */
uint32_t Encryptedtext_HighPrio[AES_HIGH_PRIO_TEXT_SIZE]={0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_AES_Init(void);
/* USER CODE BEGIN PFP */
static void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint32_t Size);
static void HighPriorityMessage_Processing(void);
/* Private functions ---------------------------------------------------------*/
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
extern void initialise_monitor_handles(void);
#endif
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
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
  initialise_monitor_handles();
#endif

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
  /* Configure LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_AES_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize interruption counter and suspension request */
  IT_call_nb = 0;
  suspension_requested = 0;

  /*==========================================================================*/
  /* Start CBC deciphering of low priority message                            */
  /*==========================================================================*/
  if (HAL_CRYP_Decrypt_IT(&hcryp, CiphertextAESCBC256_16, AES_LOW_PRIO_TEXT_SIZE, Decryptedtext) != HAL_OK)
  {
    Error_Handler();
  }

  /*===================================================*/
  /* Wait for the CRYP handle to end up in READY state */
  /* (indicates low priority data processing end)      */
  /*===================================================*/
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY)
  {
    /*==========================================================*/
    /* If low priority data processing suspension is requested  */
    /*==========================================================*/
    if (suspension_requested == 1)
    {
      /* Carry out low prio data processing suspension and context saving */
      suspension_requested = 0;
      if (HAL_OK != HAL_CRYP_Suspend(&hcryp))
      {
        Error_Handler();
      }

      /*================================================*/
      /* Process high priority data (in interrupt mode) */
      /*================================================*/
      HighPriorityMessage_Processing();

      /* When done, resume low prio data processing
         - Context restore
         - processing resume */
      if (HAL_OK != HAL_CRYP_Resume(&hcryp))
      {
        Error_Handler();
      }
    /*============================================================*/
    /* End of low priority data processing suspension/resumption  */
    /*============================================================*/
    }
  }

  /* Compare the low priority message decrypted text with the expected one *****/
  data_cmp(Decryptedtext, Plaintext_16, AES_LOW_PRIO_TEXT_SIZE);

  /* No issue detected if code reaches this point */
  BSP_LED_On(LED_GREEN);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_DATATYPE_16B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_256B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_CBC;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  buffer data comparison
  * @param
  * @retval None
  */
static void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint32_t Size)
{
  /*  Before starting a new process, you need to check the current state of the peripheral;
      if it’s busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      process, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY)
  {
  }

  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(EncryptedText, RefText, 4*Size) != 0)
  {
    Error_Handler();
  }
  else
  {
    /* Right encryption */
  }
}

/**
  * @brief  High priority message processing: CTR-enciphering with a 128-bit long key
  * @param
  * @retval None
  */
static void HighPriorityMessage_Processing(void)
{
   /* Set the CRYP parameters */
  hcryp.Init.DataType  = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize   = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey      = AESKey128;
  hcryp.Init.pInitVect = AESIV_HighPrio;
  hcryp.Init.Algorithm = CRYP_AES_CTR;
  if (HAL_OK != HAL_CRYP_Init(&hcryp))
  {
    Error_Handler();
  }

  /*  Encrypt */
  if (HAL_OK != HAL_CRYP_Encrypt_IT(&hcryp, Plaintext_HighPrio, AES_HIGH_PRIO_TEXT_SIZE, Encryptedtext_HighPrio))
  {
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /* Compare the high priority message encrypted text with the expected one *****/
  data_cmp(Encryptedtext_HighPrio, CiphertextAESCTR256_HighPrio, AES_HIGH_PRIO_TEXT_SIZE);
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

  /* Turn LED_RED on */
  BSP_LED_On(LED_RED);

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
