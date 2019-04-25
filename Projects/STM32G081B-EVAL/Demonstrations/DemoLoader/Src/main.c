/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "utils.h"
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_lcd.h"
#include "stm32g081b_eval_sd.h"
#include "k_config.h"
#include "k_window.h"
#include "k_demo.h"
#include "k_storage.h"
#include "k_widgets.h"
#include "app_lowpower.h"

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Flash start address */
#define FLASH_START_ADRESS    0x08000000U
/* Flash end address */
#define FLASH_END_ADRESS      0x0801FFFFU
/* Start user code address */
#define APPLICATION_ADDRESS   0x08008000U

/* Fast program ROW size : 64 x words = 256 bytes */
#define FLASH_ROW_SIZE        0x100u

/* Fast program usage: number of row in block, and blocks in a page */
#define ROW_NB_IN_BLOCK       (SD_BLOCK_SIZE / FLASH_ROW_SIZE)
#define BLOCK_NB_IN_PAGE      (FLASH_PAGE_SIZE / SD_BLOCK_SIZE)

/* Demonstration firmware images file names */
#define BIN_FILE_NAME_LEGACY "BIN/LEGACY.bin"
#define BIN_FILE_NAME_UCPD   "BIN/UCPD.bin"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config_64MHz(void);

static void     RTC_Config(void);
static void     BootCheckAndExec(void);
static void     Error_Handler(void);
static uint8_t  LoadDemoFimware(char * FileName, uint32_t AppliAddress);
static void     GetImageFileName(DBId_TypeDef DaughterBoardId, char * ImageFileName);
static void     ProgressBarInit(char * Title, uint32_t ImageFileSize);
static void     PrepareForOperation(void);
static uint32_t PageErase(uint32_t PageNb);
uint32_t        RowFastProg(uint32_t StartAddr, uint32_t *DataToWrite);

/* Private variables ---------------------------------------------------------*/
static pFunction JumpToApplication;
static uint32_t JumpAddress;
uint32_t RccBootFlags = 0;
uint32_t PwrSBFlag = 0;

/* Progress bar handle */
static void * hProgressBar = NULL;

static uint32_t LoaderTruthTable[4][4] =
{                                    /*           Image ID          */
                                     /* NONE       LEGACY      UCPD */
/*                 None   */           {SET,         RESET,      SET},
/* DB ID           Legacy */           {SET,         RESET,      SET},
/*                 UCPD Rev A/B  */    {SET,         SET,        RESET},
/*                 UCPD Rev C   */     {SET,         SET,        RESET}

};

/* Exported variables --------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle = {0};

/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  FILINFO      fileinfo = {0};
  uint32_t     image_id = 0;
  DBId_TypeDef daughter_board_id;
  char         image_file_name[16];

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock @ 64 MHz (PLL ON) */
  SystemClock_Config_64MHz();

  /* LED init */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Enable RTC & backup domain access */
  RTC_Config();

  /* Check Boot Reason */
  BootCheckAndExec();

  /* Start the demo thread */
  kDemo_Initialization();

  /* Get the daugther board id */
  if (BSP_DB_GetId(&daughter_board_id) !=0)
  {
    kWindow_Error("Failed to identify\ndaughter board !\n");
    Error_Handler();
  }

  /* Check if Application address has been erased */
  if ((*(__IO uint32_t *)APPLICATION_ADDRESS) == 0xFFFFFFFFu)
  {
    image_id = BIN_ID_NONE;
  }
  else
  {
    /* Get stored demonstration firmware identifier */
    image_id = UTILS_GetImageId();
  }

  /* If the demonstration firmware isn't already stored in flash then copy it */
  if(LoaderTruthTable[daughter_board_id][image_id] != RESET)
  {
    /* Get the name of the binary file to load */
    GetImageFileName(daughter_board_id, image_file_name);

    /* Check whether the binary image is present */
    if (kStorage_GetFileInfo((uint8_t *)image_file_name, &fileinfo) != KSTORAGE_NOERROR)
    {
      kWindow_Error("Binary image\nnot founded !\n");
      Error_Handler();
    }
    else
    {
      /* Display download progress bar */
      ProgressBarInit("  Loading...  ", (uint32_t)fileinfo.fsize);

      /* Copy demonstration firmware image from SD card to flash memory */
      if (LoadDemoFimware(image_file_name, APPLICATION_ADDRESS) != 0)
      {
        kWindow_Error("Failed to load\ndemonstration\nfirmware !\n");
        Error_Handler();
      }
      else
      {
        if((daughter_board_id == DB_ID_UCPD_C) || (daughter_board_id == DB_ID_UCPD_AB))
        {
          image_id = BIN_ID_UCPD;
        }
        else
        {
          image_id = BIN_ID_LEGACY;
        }

        /* Set new image ID in backup reg */
        UTILS_SetImageId(image_id);
      }
    }
  }
  else
  {
    /* wait for any display to be seen (return from STDBY or splash screen) */
    HAL_Delay(500);
  }

  /* Destroy the loading progress bar */
  kWidgets_ProgressBarDestroy(hProgressBar);

  /* Temporary correction for Ticket 53265 */
  HAL_Delay(200);

  /* Launch the demo */
  if (((*(__IO uint32_t *)APPLICATION_ADDRESS) & 0x2FFF0000 ) == 0x20000000)
  {
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    JumpToApplication = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    JumpToApplication();
  }
  else
  {
    kWindow_Error("Loader\nfails !\n");
    Error_Handler();
  }

  /* we should not reach here */
  while(1);
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
  case SD_DETECT_PIN :
    BSP_SD_Init();
    kStorage_SdDetection(BSP_SD_IsDetected());
    break;
  default :
    Error_Handler();
    break;
  }
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void kDemo_Initialization(void)
{
  /* Initialize the SD */
  BSP_SD_Init();

  /* Initialize the LCD */
  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Check if we return from standby */
  if((PwrSBFlag & PWR_SR1_SBF) != 0x00u)
  {
    kWindow_Popup("Low Power", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE,\
                "\nExit from Standby\n", LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE );
    /* Let time user to see information */
    HAL_Delay(2000);
  }

  /* Check the SD presence else wait until SD insertion */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    kWindow_Error("sd not present\n...\nwait sd insertion\n");
    while(BSP_SD_IsDetected() != SD_PRESENT);
    BSP_SD_Init();
  }

  /* FatFs initialisation */
  if(kStorage_Init() != KSTORAGE_NOERROR)
  {
    kWindow_Error("FatFs initialization failed\nplease check the SD card\n");
    Error_Handler();
  }

  if(RccBootFlags != 0)
  {
    /* Display the demonstration window */
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
    BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), LCD_DEFAULT_FONT.Height*2);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt( 0,              0,      (uint8_t *)EVAL_BOARD, CENTER_MODE);
    BSP_LCD_DisplayStringAt( 0,  LCD_DEFAULT_FONT.Height, (uint8_t *)"DEMONSTRATION", CENTER_MODE);
    kStorage_OpenFileDrawPixel(((LCD_DEFAULT_FONT.Height*2) + 1), ((BSP_LCD_GetXSize() - 170) / 2), (uint8_t *)"STFILES/STLogo.bmp");
  }
}

void kDemo_UnInitialization(void)
{
  /* Nothing to do */
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLP                           = 16
  *            PLLQ                           = 5
  *            PLLR                           = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config_64MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Set HSI as PLL source to use it for max 64MHz frequency as SYSCLK source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN            = 64;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV16;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    while(1);
  }
}


/**
  * @brief  Configures the RTC & backup domain access
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();

  /* enable RTC & TAMP clock gating */
  __HAL_RCC_RTCAPB_CLK_ENABLE();

  /* Set the RTC time base to 1s */
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 0x7Fu;
  RtcHandle.Init.SynchPrediv = 0x00FFu;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  Check boot reason
  * @param  None
  * @retval None
  */
static void BootCheckAndExec(void)
{
  /* Get reset flags */
  RccBootFlags = RCC->CSR;

  /* Clear reset flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /* Get Standby flag */
  PwrSBFlag = PWR->SR1;

  /* Clear Standby flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

}


static void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
  }
}

/**
  * @brief  Load the demonstration firmware to the flash memomry
  * @param  FileName Name of the binary image
  * @retval error 0 : Demo firmware loading successfull
  *               else : Demo firmware loading failed
  */
static uint8_t LoadDemoFimware(char * FileName, uint32_t AppliAddress)
{
  uint32_t block[(SD_BLOCK_SIZE / 4)];
  uint32_t *pdata;
  FIL file;
  UINT bytes_read;
  uint32_t bytes_to_read = 0;
  uint32_t flash_address = AppliAddress;
  uint32_t pagenb = ((AppliAddress - FLASH_START_ADRESS) / FLASH_PAGE_SIZE);
  uint32_t total_bytes;
  uint8_t rowcount;
  uint8_t blockcount = 0;

  /* Unlock Flash */
  HAL_FLASH_Unlock();

  /* Fill in buffer from SD card */
  if( f_open(&file, FileName, FA_READ) != FR_OK)
  {
    return 0x01u;
  }

  /* check number of byte to read */
  bytes_to_read = f_size(&file);
  total_bytes = bytes_to_read;

  /* Make sure user area size is big enough */
  if(total_bytes > (FLASH_END_ADRESS - flash_address + 1))
  {
    return 0xFFu;
  }

  /* loop */
  while (bytes_to_read > 0)
  {
    if(blockcount == 0)
    {
      /* reset count */
      blockcount = BLOCK_NB_IN_PAGE;

      /* prepare Flash for operations */
      PrepareForOperation();

      /* Erase current page */
      if(PageErase(pagenb) != 0x00u)
      {
        return 0x02u;
      }

      /* increase pagenb */
      pagenb++;
    }

    /* Reset buffer content */
    memset(block, 0xFF, sizeof(block));

    /* Fill in block from file content */
    if (f_read(&file, block, sizeof(block), &bytes_read) != FR_OK)
    {
      return 0x03u;
    }

    /* reset pointer at beginnng of block and initialized ROW counter */
    pdata = block;
    rowcount = ROW_NB_IN_BLOCK;

    while(rowcount > 0)
    {
      /* prepare Flash for operations */
      PrepareForOperation();

      /* program a row */
      if(RowFastProg(flash_address, pdata) != 0x00u)
      {
        return 0x04u;
      }

      /* Decrease row counter, increase pointer and flash address of a row */
      rowcount--;
      pdata += (FLASH_ROW_SIZE / 4);
      flash_address += FLASH_ROW_SIZE;
    }

    /* Decrease number of bytes to be read */
    bytes_to_read -= bytes_read;

    /* Update the loading progress bar */
    kWidgets_ProgressBarUpdate(hProgressBar, (total_bytes - bytes_to_read));

    /* decrease block counter */
    blockcount--;
  }


  if( f_close(&file) != FR_OK)
  {
    return 0x05u;
  }
  else
  {
    return 0x00u;
  }
}


/**
  * @brief  Get the image file name from the daughter board identifier.
  * @param  DaughterBoardId: Daughter board identifier
  * @param  ImageFileName: Demonstration firmware image file name
  * @retval None
  */
static void GetImageFileName(DBId_TypeDef DaughterBoardId, char * ImageFileName)
{
 switch (DaughterBoardId)
  {
  case DB_ID_LEGACY:
    strcpy(ImageFileName, BIN_FILE_NAME_LEGACY);
    break;
  case DB_ID_UCPD_AB:
  case DB_ID_UCPD_C:
    strcpy(ImageFileName, BIN_FILE_NAME_UCPD);
    break;
  default:
    strcpy(ImageFileName, BIN_FILE_NAME_LEGACY);
    break;
  }

  return;
}

/**
  * @brief  Initialize the loading process progress bar.
  * @param  Title: progress bar title
  * @param  ImageFileSize: size (in byrtes) of the binary image
  * @retval None
  */
static void ProgressBarInit(char * Title, uint32_t ImageFileSize)
{
  kWidgetsProgressBar_t progress_bar =
  {
    .textColor       = LCD_COLOR_ST_PINK,
    .backgroungColor = LCD_COLOR_WHITE,
    .xpos            = 10,
    .ypos            = (LINE(9) - 1),
    .width           = BSP_LCD_GetXSize() - 20,
    .height          = LCD_DEFAULT_FONT.Height,
  };

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)Title, CENTER_MODE);

  /* Display loading progress bar */
  hProgressBar = kWidgets_ProgressBarCreate(&progress_bar, ImageFileSize);
}


/**
  * @brief  Prepare for flash programming or erase
  * @param  None
  * @retval None
  */
static void PrepareForOperation(void)
{
  /* check no operation is on going */
  while((FLASH->SR & FLASH_SR_BSY1) != 0);

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  /* check cfgbsy is cleared */
  while((FLASH->SR & FLASH_SR_CFGBSY) != 0);
}


/**
  * @brief  Perform Pages Erase
  * @param  PageNb page to erase
  * @retval 0 if ok else error
  */
static uint32_t PageErase(uint32_t PageNb)
{
  FLASH->CR &= ~FLASH_CR_PNB;
  FLASH->CR |= (FLASH_CR_STRT | (PageNb << 3) | FLASH_CR_PER);

  while((FLASH->SR & (FLASH_SR_BSY1 | FLASH_SR_CFGBSY)) != 0);

  /* clear page erase bit */
  FLASH->CR &= ~FLASH_CR_PER;

  /* wait for cfgbsy */
  while((FLASH->SR & FLASH_SR_CFGBSY) != 0);

  if(FLASH->SR != 0)
  {
    /* NOK */
    return (0x01);
  }
  else
  {
    /* OK */
    return 0x00;
  }
}


/* Start placing functions in section ramfunc */
#if defined (__CC_ARM)
#pragma arm section code="ramfunc"
#elif defined (__ICCARM__)
#pragma default_function_attributes = @ ".ramfunc"
#else
__attribute__((__section__(".ramfunc")))
#endif
/**
  * @brief  Double word Programming
  * @param  StartAddr row address to write
  * @param  DataToWrite pointer on a data of row size
  * @retval 0 if ok else error
  */
uint32_t RowFastProg(uint32_t StartAddr, uint32_t *DataToWrite)
{
  uint32_t address = StartAddr;
  uint8_t i = 0;

  __disable_irq();

  /* start fast programming */
  FLASH->CR |= FLASH_CR_FSTPG;

  /* Fast Program : 64 words */
  while(i < 64)
  {
    *(uint32_t*)address = DataToWrite[i];
    address += 4;
    i++;
  }

  /* wait for cfgbsy */
  while((FLASH->SR & FLASH_SR_CFGBSY) != 0);

  /* End fast programming */
  FLASH->CR &= ~FLASH_CR_FSTPG;

  __enable_irq();

  if(FLASH->SR != 0)
  {
    /* NOK */
    return (0x01);
  }
  else
  {
    /* OK */
    return 0x00;
  }
}

/* Stop placing functions in section ramfunc */
#if defined (__CC_ARM)
#pragma arm section code
#elif defined (__ICCARM__)
#pragma default_function_attributes =
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
