/**
  ******************************************************************************
  * @file    wave_recorder.c
  * @author  MCD Application Team
  * @brief   WAVE recorder implementation
  ******************************************************************************
   * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
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
#include "stm32g0xx_hal.h"
#include "app_audio.h"
#include "wave_recorder.h"
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_lcd.h"
#include "ff.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"
#include "k_window.h"
#include "k_widgets.h"

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/
/* WAVE player states */
enum {
  WAVERECORDER_INIT,
  WAVERECORDER_WAIT,
  WAVERECORDER_RECORDING,
  WAVERECORDER_STOPPED,
  WAVERECORDER_ERROR,
  WAVERECORDER_EXIT,
  WAVERECORDER_END
};

/* User messages */
#define MSG_RECORD_INIT      "SEL: Start Rec - RIGHT: Quit"
#define MSG_RECORD_ON_GOING  "DOWN: Stop - RIGHT: Quit   "
#define MSG_RECORD_STOPPED   "SEL: Restart Rec - RIGHT: Quit"

/* Record information */
#define MSG_FILE_NAME        (uint8_t *)"rec.wav"
#define MSG_SAMPLE_RATE      (uint8_t *)"16000 Hz"
#define MSG_NOF_CHANNELS     (uint8_t *)"Stereo"
#define MSG_BITS_PER_SAMPLES (uint8_t *)"16 bits"

/* Sample rate (16 kHz)*/
#define SAMPLE_RATE         16000

/* Maximum data chunk size */
#define MAX_DATA_CHUNCK_SIZE  0x003A9800U

/* Position of the cksize field of the wave header */
#define CKSIZE_POS            4

/* position of the cksize field of the data chunk */
#define DATA_CKSIZE_POS       40

/* Private function prototypes -----------------------------------------------*/
static uint8_t RecordInit(void);
static uint8_t RecordDeInit(void);
static uint8_t RecordStart(void);
static uint8_t RecordStop(void);

static uint8_t CopyToSD(void);

static uint8_t WaveFileCreate(void);
static uint8_t WaveFileFinalize(void);

static void DisplayRecordHeaderScreen(void);
static void DisplayRecordStoppedScreen(void);
static void DisplayRecordOnGoingScreen(void);
static void DisplayBottomMessage(char * msg);
static void RecordStateUpdate(char * msg);

/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserAction=0;

/* Peripherals HAL handles */
static TIM_HandleTypeDef htim  = {0};
static DMA_HandleTypeDef hdma_adc = {0};

/* Actual size of the chunk that contains the sampled data */
uint32_t DataCkSize = 0;

/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void WaveRecorder_Demo(void)
{
  uint8_t application_state = WAVERECORDER_INIT;
  
  do
  {
    switch(application_state)
    {
    case WAVERECORDER_INIT:
      if (RecordInit() == 0)
      {
        DisplayRecordHeaderScreen();
        DisplayBottomMessage(MSG_RECORD_INIT);
        application_state = WAVERECORDER_WAIT;
      }
      else
      {
        application_state = WAVERECORDER_ERROR;
      }
      break;
      
    /* Wait for an user event */
    case WAVERECORDER_WAIT:
      /* User action init */
      UserEvent        = JOY_NONE;
      UserAction       = 0;
      while(UserEvent == JOY_NONE);
      
      /* treatment of the user request */
      switch(UserEvent)
      {
      case JOY_SEL:
        /* Clear audio buffers */
        memset(Buffer1, 0, BUFFER_SIZE_BYTE);
        memset(Buffer2, 0, BUFFER_SIZE_BYTE);
        
        /* Initialize active buffer pointer */
        BufferSelect = Buffer1;
        
        /* Reset DMATxComplete flag */
        DMATxComplete = 0;
        
        if (WaveFileCreate() == 0)
        {
          DisplayRecordOnGoingScreen();
          if (RecordStart() == 0)
          {
            application_state = WAVERECORDER_RECORDING;
          }
          else
          {
            application_state = WAVERECORDER_ERROR;
          }
        }
        else
        {
          application_state = WAVERECORDER_ERROR;
        }
        UserAction = 0;      
        UserEvent = JOY_NONE;
        break;
        
      case JOY_RIGHT:
        application_state = WAVERECORDER_EXIT;
        break;

      default:
        /* don't care */
        break;
      }
      break;
      
    case WAVERECORDER_RECORDING:
      switch (UserEvent)
      {
      case JOY_NONE:
      case JOY_LEFT:
      case JOY_UP:
      case JOY_SEL:
        if (DataCkSize < MAX_DATA_CHUNCK_SIZE)
        {
          if (DMATxComplete == 1)
          {
            /* Reset DMATxComplete flag */
            DMATxComplete = 0;
            
            if (CopyToSD() != 0)
            {
              application_state = WAVERECORDER_ERROR;
            }   
          }
        }
        else
        {
          application_state = WAVERECORDER_EXIT;
        }
        UserAction = 0;      
        break;
        
      case JOY_RIGHT:
        application_state = WAVERECORDER_EXIT;
        break;
        
      case JOY_DOWN:
        application_state = WAVERECORDER_STOPPED;
        break;        
      }
      
      break;

    case WAVERECORDER_STOPPED:
      if (hProgressBar != NULL)
      {
        kWidgets_ProgressBarReset(hProgressBar); 
      }
      WaveFileFinalize();
      RecordStop();
      RecordDeInit();
      DisplayRecordStoppedScreen();   
      RecordInit();       
      application_state = WAVERECORDER_WAIT;
      break;      
      
    case WAVERECORDER_ERROR:
      RecordStop();
      RecordDeInit();
      f_close(&WaveFile);
      f_unlink (REC_FILE_NAME);
      application_state = WAVERECORDER_END;
      break;
      
    case WAVERECORDER_EXIT :
      if (hProgressBar != NULL)
      {
        kWidgets_ProgressBarDestroy(hProgressBar);
        hProgressBar = NULL;        
      }
      WaveFileFinalize();
      RecordStop();
      RecordDeInit();
      application_state = WAVERECORDER_END;
      break;
    }
  } while(application_state != WAVERECORDER_END);
}

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void WaveRecorder_UserAction(uint8_t sel)
{
  if (UserAction == 0)
  {
    UserEvent = sel;
    UserAction = 1;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the hardware resources used by the audio recorder 
  * @retval None
  */
static uint8_t RecordInit(void)
{
  uint8_t                 ret = 0;
  TIM_MasterConfigTypeDef master_config = {0};
  GPIO_InitTypeDef        gpio_init = {0};
  ADC_ChannelConfTypeDef  adc_channel_config = {0};
  
  /* Enable TIM6 clock */
  __HAL_RCC_TIM6_CLK_ENABLE();
  
  /* TIM6 initialisation */
  htim.Instance = TIM6;
  htim.Init.Period            = (HAL_RCC_GetPCLK1Freq() / (SAMPLE_RATE)) - 1;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim.Init.RepetitionCounter = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim) != HAL_OK)
  {
    ret++;
  }
  
  /* Master synchro config */
  master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
  master_config.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim, &master_config) != HAL_OK)
  {
    ret++;
  } 
  
  /* GPIOA clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA6 (ADC_IN6) in analog mode */
  gpio_init.Pin   = GPIO_PIN_6;
  gpio_init.Mode  = GPIO_MODE_ANALOG;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &gpio_init);
  
  /* Enable ADC clock */
  __HAL_RCC_ADC_CLK_ENABLE();
  
  /* ADC initialisation */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler             = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution                 = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign                  = ADC_DATAALIGN_LEFT;
  hadc.Init.ScanConvMode               = DISABLE;
  hadc.Init.EOCSelection               = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait           = DISABLE;
  hadc.Init.LowPowerAutoPowerOff       = DISABLE;
  hadc.Init.ContinuousConvMode         = DISABLE;
  hadc.Init.NbrOfConversion            = 1;
  hadc.Init.DiscontinuousConvMode      = DISABLE;
  hadc.Init.ExternalTrigConv           = ADC_EXTERNALTRIG_T6_TRGO;
  hadc.Init.ExternalTrigConvEdge       = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc.Init.DMAContinuousRequests      = DISABLE;
  hadc.Init.Overrun                    = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.SamplingTimeCommon1        = ADC_SAMPLETIME_39CYCLES_5;
  hadc.Init.SamplingTimeCommon2        = ADC_SAMPLETIME_39CYCLES_5;
  hadc.Init.OversamplingMode           = ENABLE;
  hadc.Init.Oversampling.Ratio         = ADC_OVERSAMPLING_RATIO_4;
  hadc.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_2;
  hadc.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc.Init.TriggerFrequencyMode       = ADC_TRIGGER_FREQ_HIGH;
  
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    ret++;
  }
  
  if (HAL_ADCEx_Calibration_Start(&hadc) !=  HAL_OK)
  {
    ret++;
  }

  adc_channel_config.Channel      = ADC_CHANNEL_6;
  adc_channel_config.Rank         = ADC_REGULAR_RANK_1;
  adc_channel_config.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc, &adc_channel_config) != HAL_OK)
  {
    ret++;
  }
  
    /* DMA1 clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Set DMA descriptor for DAC channel 1 */
  hdma_adc.Instance = DMA1_Channel1; 
  hdma_adc.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode                = DMA_NORMAL;
  hdma_adc.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_adc.Init.Request             = DMA_REQUEST_ADC1;
  
  __HAL_LINKDMA(&hadc, DMA_Handle, hdma_adc);
  
  if (HAL_DMA_Init(hadc.DMA_Handle) != HAL_OK)
  {
    ret++;
  }
  
  /* NVIC configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  return ret;
}

/**
  * @brief  Releases the hardware resources used by the audio recorder 
  * @retval None
  */
static uint8_t RecordDeInit(void)
{
  /* Reset NVIC configuration */
  HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
  
  /* Deinitialize DMA channel */
  HAL_DMA_DeInit(hadc.DMA_Handle);
  
  /* DMA1 clock disable */
  __HAL_RCC_DMA1_CLK_DISABLE();
  
  /* De-initialize ADC */
  HAL_ADC_DeInit(&hadc);
  
  /* Reset ADC handle */
  memset(&hadc, 0, sizeof(ADC_HandleTypeDef));
  
  /* Disable ADC clock */
  __HAL_RCC_ADC_CLK_DISABLE();
  __HAL_RCC_ADC_FORCE_RESET();
  __HAL_RCC_ADC_RELEASE_RESET();

  /* Reset PA6 (ADC_IN6) configuration */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
  
  /* De-initialize TIM6 */
  HAL_TIM_Base_DeInit(&htim);
  
  /* Disable TIM6 clock */
  __HAL_RCC_TIM6_CLK_DISABLE();
  __HAL_RCC_TIM6_FORCE_RESET();
  __HAL_RCC_TIM6_RELEASE_RESET();

  
  return 0;
}

/**
  * @brief  Starts audio recording 
  * @retval Error code
  */
static uint8_t RecordStart(void)
{
  uint8_t ret = 0;

  if (HAL_ADC_Start_DMA(&hadc, (uint32_t *)BufferSelect, BUFFER_SIZE_WORD) != HAL_OK)
  {
    ret++;
  }
  
  if (HAL_TIM_Base_Start(&htim) != HAL_OK)
  {
    ret++;
  }
  
    
  return ret;
}

/**
  * @brief  Stops audio recording 
  * @retval None
  */
static uint8_t RecordStop(void)
{
  uint8_t ret = 0;

  if (HAL_TIM_Base_Stop(&htim) != HAL_OK)
  {
    ret++;
  }
    
  if (HAL_ADC_Stop_DMA(&hadc) != HAL_OK)
  {
    ret++;
  }
  
  return ret;
}

/**
  * @brief  Copy audio buffer to SD card 
  * @retval Error code
  */
static uint8_t CopyToSD(void)
{
  uint8_t    ret = 0;
  uint16_t   sample;
  uint32_t   bytes_written;
  uint32_t * pBuffer;
  
  pBuffer = (uint32_t *)BufferSelect;
  
  /* Launch next ADC conversion */
  if (BufferSelect == Buffer1)
  {
    if (HAL_ADC_Start_DMA(&hadc, (uint32_t *)Buffer2, BUFFER_SIZE_WORD) != HAL_OK)
    {
      ret++;
    }
    
    /* Swap active buffer */
    BufferSelect = Buffer2;
  }
  else if (BufferSelect == Buffer2)
  {
    if (HAL_ADC_Start_DMA(&hadc, (uint32_t *)Buffer1, BUFFER_SIZE_WORD) != HAL_OK)
    {
      ret++;
    }
    
    /* Swap active buffer */
    BufferSelect = Buffer1;
  }

  /* Audio samples conversion from mono to dual mono */
  for (int i = 0 ; i < BUFFER_SIZE_WORD ; i++)
  {
    sample = (uint16_t)(pBuffer[i] & 0xFFFFU);
    pBuffer[i] = (uint32_t)(sample << 16) | sample;
  }
  
  /* Copy audio buffer to SD card */
  if (f_write(&WaveFile, pBuffer, BUFFER_SIZE_BYTE, (UINT*)&bytes_written) != FR_OK )
  {
    ret++;
  }
   
  /* Update record progress bar */
  DataCkSize += bytes_written;
  
  kWidgets_ProgressBarUpdate(hProgressBar, DataCkSize);
  
  return ret;
}

/**
  * @brief  Create the record wave file
  * @retval Error code
  */
static uint8_t WaveFileCreate(void)
{
  uint8_t           ret = 0;
  uint32_t          bytes_written;
  uint8_t header_buffer[_MAX_SS] = {0};
  
  /* Delete previous wave file */
  f_unlink (REC_FILE_NAME);

  /* Create a new wave file */
  if (f_open(&WaveFile, REC_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    ret++;
  }
  
  /* Initialize the wave header */
  if (ret == 0)
  {
    /* write chunkID, must be 'RIFF'  ------------------------------------------*/
    header_buffer[0] = 'R';
    header_buffer[1] = 'I';
    header_buffer[2] = 'F';
    header_buffer[3] = 'F';
    
    /* Write the file length ---------------------------------------------------*/
    /* The sampling time 16000 Hz
    To record 30s we need 16000 x 30 x 4 = 0x001D4C00 */
    header_buffer[4] = 0x00;
    header_buffer[5] = 0x4C;
    header_buffer[6] = 0x1D;
    header_buffer[7] = 0x00;
    
    /* Write the file format, must be 'WAVE' -----------------------------------*/
    header_buffer[8]  = 'W';
    header_buffer[9]  = 'A';
    header_buffer[10] = 'V';
    header_buffer[11] = 'E';
    
    /* Write the format chunk, must be'fmt ' -----------------------------------*/
    header_buffer[12]  = 'f';
    header_buffer[13]  = 'm';
    header_buffer[14]  = 't';
    header_buffer[15]  = ' ';
    
    /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
    header_buffer[16]  = 0x10;
    header_buffer[17]  = 0x00;
    header_buffer[18]  = 0x00;
    header_buffer[19]  = 0x00;
    
    /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
    header_buffer[20]  = 0x01;
    header_buffer[21]  = 0x00;
    
    /* Write the number of channels, must be 0x02 (Stereo) ---------------------*/
    header_buffer[22]  = 0x02;
    header_buffer[23]  = 0x00;
    
    /* Write the Sample Rate 16000 Hz ------------------------------------------*/
    header_buffer[24]  = (uint8_t)((SAMPLE_RATE & 0xFF));
    header_buffer[25]  = (uint8_t)((SAMPLE_RATE >> 8) & 0xFF);
    header_buffer[26]  = (uint8_t)((SAMPLE_RATE >> 16) & 0xFF);
    header_buffer[27]  = (uint8_t)((SAMPLE_RATE >> 24) & 0xFF);
    
    /* Write the Byte Rate: Sampling rate * BitsPerSample * NumChannel / 8
    = 16000 * 16 * 2 / 8 
    =  16000 * 4 */
    header_buffer[28]  = (uint8_t)(((2 * SAMPLE_RATE * 16/8) & 0xFF));
    header_buffer[29]  = (uint8_t)(((2 * SAMPLE_RATE * 16/8) >> 8) & 0xFF);
    header_buffer[30]  = (uint8_t)(((2 * SAMPLE_RATE * 16/8) >> 16) & 0xFF);
    header_buffer[31]  = (uint8_t)(((2 * SAMPLE_RATE * 16/8) >> 24) & 0xFF);
    
    /* Write the block alignment -----------------------------------------------*/
    header_buffer[32]  = 0x04;
    header_buffer[33]  = 0x00;
    
    /* Write the number of bits per sample -------------------------------------*/
    header_buffer[34]  = 0x10; 
    header_buffer[35]  = 0x00;
    
    /* Write the Data chunk, must be 'data' ------------------------------------*/
    header_buffer[36]  = 'd';
    header_buffer[37]  = 'a';
    header_buffer[38]  = 't';
    header_buffer[39]  = 'a';
    
    /* Write the number of sample data -----------------------------------------*/
    header_buffer[40]  = 0x00;
    header_buffer[41]  = 0x4C;
    header_buffer[42]  = 0x1D;
    header_buffer[43]  = 0x00;
    
    if (f_write(&WaveFile, &header_buffer, _MAX_SS, (UINT*)&bytes_written) != FR_OK)
    {
      f_close(&WaveFile);
      ret++;
    }
  }
  
  /* Initialize the data chunk */
  if (ret == 0)
  {
    /* Reset Data chunk size */
    DataCkSize = 0;
    
    if ((f_write(&WaveFile, "data", 4, (UINT*)&bytes_written) != FR_OK) ||
        (f_write(&WaveFile, &DataCkSize, sizeof(uint32_t), (UINT*)&bytes_written) != FR_OK))
    {
      f_close(&WaveFile);
      ret++;
    }
  }
 
  return ret;
}

/**
  * @brief  Finalize the record wave file
  * @retval Error code
  */
static uint8_t WaveFileFinalize(void)
{
  uint8_t  ret = 0;
  uint32_t bytes_written;
  uint32_t cksize;
  
  /* Update the cksize field of the wave header */
  cksize = 36 + DataCkSize;
  f_lseek(&WaveFile, CKSIZE_POS);
  f_write(&WaveFile, &cksize, sizeof(cksize), (UINT*)&bytes_written);

  /* Update the cksize field of the data chunk */
  f_lseek(&WaveFile, DATA_CKSIZE_POS);
  f_write(&WaveFile, &DataCkSize, sizeof(DataCkSize), (UINT*)&bytes_written);
  
  /* Close the wave file */
  f_close(&WaveFile);
  
  return ret;
}

/**
  * @brief  Conversion complete callback
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Set DMATxComplete flag */
  DMATxComplete = 1; 
}

/**
  * @brief  Displays the record stopped screen
  * @param Select: title file name
  * @retval None
  */
static void DisplayRecordStoppedScreen(void)
{
  /* Display Record stopped Icon */
  kStorage_OpenFileDrawBMP(35, 46, (uint8_t *)"STFILES/RECC2.bmp");

  DisplayBottomMessage(MSG_RECORD_STOPPED);
  RecordStateUpdate(" RECORD STOPPED "); 
}

/**
  * @brief  Displays the record on going screen
  * @param Select: title file name
  * @retval None
  */
static void DisplayRecordOnGoingScreen(void)
{
  /* Display Record on going Icon */
  kStorage_OpenFileDrawBMP(35, 46, (uint8_t *)"STFILES/RECC.bmp");

  DisplayBottomMessage(MSG_RECORD_ON_GOING);
  RecordStateUpdate(" RECORD ON GOING ");
}

/**
  * @brief  Displays the record header screen
  * @param  None
  * @retval None
  */
static void DisplayRecordHeaderScreen(void)
{
  kWidgetsProgressBar_t progress_bar =
  {
    .textColor       = LCD_COLOR_ST_PINK,
    .backgroungColor = LCD_COLOR_WHITE,
    .xpos            = 10,
    .ypos            = 150,
    .width           = BSP_LCD_GetXSize() - 20,
    .height          = 5,  
  };

  /* Black screen */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Display Title */
  BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  BSP_LCD_FillRect(0, LINE(0), BSP_LCD_GetXSize(), Font24.Height);   
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);  
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Wave Recorder", CENTER_MODE);

  /* Display Record on going Icon */
  kStorage_OpenFileDrawBMP(35, 46, (uint8_t *)"STFILES/RECC2.bmp");
  
  RecordStateUpdate("'SEL' TO RECORD");
  
  /* Display record info */
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
  BSP_LCD_DisplayStringAt(150, 46,                     MSG_FILE_NAME,        LEFT_MODE);
  BSP_LCD_DisplayStringAt(150, 46 + Font16.Height,     MSG_SAMPLE_RATE,      LEFT_MODE);
  BSP_LCD_DisplayStringAt(150, 46 + 2 * Font16.Height, MSG_NOF_CHANNELS,     LEFT_MODE);
  BSP_LCD_DisplayStringAt(150, 46 + 3 * Font16.Height, MSG_BITS_PER_SAMPLES, LEFT_MODE);

  /* Display playback progress bar */
  hProgressBar = kWidgets_ProgressBarCreate(&progress_bar, MAX_DATA_CHUNCK_SIZE);
  
}

/**
  * @brief  Displays the message at the bottom of the screen
  * @param msg: bottom message
  * @retval None
  */
static void DisplayBottomMessage(char * msg)
{
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetBackColor(LCD_COLOR_ST_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
  BSP_LCD_FillRect(0, LINE(19), BSP_LCD_GetXSize(), Font20.Height);        
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, LINE(19), (uint8_t *)msg, CENTER_MODE);
  BSP_LCD_SetFont(&Font24);
}

/**
  * @brief  Update the displayed record state 
  * @param  PlaybackState: playback state
  * @retval None
  */
static void RecordStateUpdate(char * RecordState)
{
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)RecordState, CENTER_MODE);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
