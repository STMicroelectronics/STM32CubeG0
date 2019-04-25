/**
  ******************************************************************************
  * @file    wave_player.h
  * @author  MCD Application Team
  * @brief   WAVE player implementation
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
#include "wave_player.h"
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
  WAVEPLAYER_INIT,
  WAVEPLAYER_SELECT_TITLE,
  WAVEPLAYER_PLAYING,
  WAVEPLAYER_STOPPED,
  WAVEPLAYER_PAUSED,
  WAVEPLAYER_WAIT_SELECTION,
  WAVEPLAYER_PLAYING_WAITEVENT,
  WAVEPLAYER_EXIT,
  WAVEPLAYER_END
};

/* Playlist table dimensionning */
#define MAX_TITLES            10
#define MAX_TITLE_NAME_LENGTH 16

/* User messages */
#define MSG_TITLE_SELECTION   "SEL: Play - UP/DOWN: Select - RIGHT: Quit"
#define MSG_PLAYBACK_PLAYING  "  SEL: Pause - DOWN: Stop - RIGHT: Quit  "
#define MSG_PLAYBACK_PAUSED   "  SEL: Play - DOWN: Stop - RIGHT: Quit   "
#define MSG_PLAYBACK_STOPPED  "         SEL: Play - RIGHT: Quit         "

/* Audio ramp up step */
#define STEP 1

/* Private function prototypes -----------------------------------------------*/
static void     WavePlayer_PlaybackInit(uint32_t SampleRate);
static void     WavePlayer_PlaybackDeInit(void);

static void     WavePlayer_PlaybackStart(uint8_t sel);
static void     WavePlayer_PlaybackStop(void);
static void     WavePlayer_PlaybackPause(void);
static void     WavePlayer_PlaybackResume(void);

static void     WavePlayer_PlaybackPlay(void);

static uint32_t WavePlayer_JumpToWaveData(FIL * file, WAV_HeaderTypeDef * WAV_header);
static void     WavePlayer_Prepare(FIL* fp, uint32_t * pBuffer, uint32_t Size);
static void     WavePlayer_Play(uint32_t * pBuffer, uint32_t Size);
static uint8_t  WavePlayer_InitializePlayList(void);
static void     WavePlayer_DisplayTitleSelectionScreen(void);
static void     WavePlayer_DisplayPlayList(uint8_t Select, uint8_t NbTitles);
static void     WavePlayer_DisplayPlaybackScreen(uint8_t Select);
static void     WavePlayer_DisplayBottomMessage(char * msg);
static uint8_t  WavePlayer_IsValidWAVHeader(WAV_HeaderTypeDef* WAV_header);

static void PlaybackStateUpdate(char * PlaybackState);

/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;

/* Play list */
static char PlayList[MAX_TITLES][MAX_TITLE_NAME_LENGTH] = {0};

/* Wave file information */
static FILINFO WaveFileInfo;

/* Peripherals HAL handles */
static TIM_HandleTypeDef htim = {0};
static DMA_HandleTypeDef hdma_dac_ch1 = {0};

/* Signed to unsigned conversion flag */
static uint8_t SignedToUnsignedConversion = 1;

/* Audio ramp up flag*/
uint8_t AudioRampUpRequested = 0;

/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void WavePlayer_Demo(void)
{
  uint8_t nof_titles = 0;
  uint8_t application_state = WAVEPLAYER_INIT;
  uint8_t sel = 1;

  do
  {
    switch(application_state)
    {
    case WAVEPLAYER_INIT:
      nof_titles = WavePlayer_InitializePlayList();
      if (nof_titles == 0)
      {
        kWindow_Popup("PLAY LIST EMPTY", LCD_COLOR_WHITE, LCD_COLOR_ST_BLUE , "Push JoyStick\nto exit.\n", LCD_COLOR_ST_BLUE, LCD_COLOR_WHITE);
        UserEvent  = JOY_NONE;
        UserEntry = 0;
        while(UserEvent == JOY_NONE);
        application_state = WAVEPLAYER_EXIT;
      }
      else
      {
        application_state = WAVEPLAYER_SELECT_TITLE;
      }
      DMATxComplete = 0;
      break;

    case WAVEPLAYER_SELECT_TITLE:
      WavePlayer_DisplayTitleSelectionScreen();
      WavePlayer_DisplayPlayList(sel, nof_titles);
      WavePlayer_DisplayBottomMessage(MSG_TITLE_SELECTION);
      application_state = WAVEPLAYER_WAIT_SELECTION;
      break;

    /* Wait for an user event */
    case WAVEPLAYER_WAIT_SELECTION:
      /* User action init */
      UserEvent        = JOY_NONE;
      UserEntry       = 0;
      while(UserEvent == JOY_NONE);

      /* treatment of the user request */
      switch(UserEvent)
      {
      case JOY_DOWN:
        if (sel == nof_titles)
        {
          sel = 1;
        }
        else
        {
          sel ++;
        }
        WavePlayer_DisplayPlayList(sel, nof_titles);
        break;

      case JOY_UP:
        if (sel == 1)
        {
          sel = nof_titles;
        }
        else
        {
          sel --;
        }
        WavePlayer_DisplayPlayList(sel, nof_titles);
      break;

      case JOY_SEL:
        AudioRampUpRequested = 1;
        WavePlayer_DisplayPlaybackScreen(sel);
        WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_PLAYING);
        PlaybackStateUpdate("PLAYING");
        WavePlayer_PlaybackStart(sel);
        application_state = WAVEPLAYER_PLAYING;
        break;

      case JOY_LEFT:
        /* don't care */
        break;

      case JOY_RIGHT:
        application_state = WAVEPLAYER_EXIT;
        break;
      }
      /* Reset the Userevent */
      UserEvent = JOY_NONE;
      UserEntry = 0;
      break;

    case WAVEPLAYER_PLAYING:
      {
        /* Update playing information */
        if (WaveFile.fptr < (WaveFileInfo.fsize - 2 * BUFFER_SIZE_BYTE))
        {
          if (DMATxComplete != 0)
          {
            WavePlayer_PlaybackPlay();
          }
        }
        else
        {
          kWidgets_ProgressBarUpdate(hProgressBar, WaveFileInfo.fsize);
          WavePlayer_PlaybackStop();
          WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_STOPPED);
          PlaybackStateUpdate("  STOPPED  ");
          application_state = WAVEPLAYER_STOPPED;
        }

        switch (UserEvent)
        {
        case JOY_SEL:
          WavePlayer_PlaybackPause();
          WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_PAUSED);
          PlaybackStateUpdate("  PAUSED  ");
          application_state = WAVEPLAYER_PAUSED;
          break;

        case JOY_DOWN:
          WavePlayer_PlaybackStop();
          WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_STOPPED);
          PlaybackStateUpdate("  STOPPED  ");
          application_state = WAVEPLAYER_STOPPED;
          break;

        case JOY_RIGHT:
          WavePlayer_PlaybackStop();
          WavePlayer_PlaybackDeInit();
          application_state = WAVEPLAYER_SELECT_TITLE;
          break;
        }
        UserEvent = JOY_NONE;
        UserEntry = 0;
        break;
      }

    case WAVEPLAYER_PAUSED:
      {
        if(1 == UserEntry)
        {
          switch (UserEvent)
          {
          case JOY_SEL:
            WavePlayer_PlaybackResume();
            WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_PLAYING);
            PlaybackStateUpdate("PLAYING");
            application_state = WAVEPLAYER_PLAYING;
            break;

          case JOY_RIGHT:
            WavePlayer_PlaybackStop();
            WavePlayer_PlaybackDeInit();
            application_state = WAVEPLAYER_SELECT_TITLE;
            break;

          case JOY_DOWN:
            WavePlayer_PlaybackStop();
            WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_STOPPED);
            PlaybackStateUpdate("  STOPPED  ");
            application_state = WAVEPLAYER_STOPPED;
            break;

          default:
            break;
          }
          /* Reset the user event */
          UserEvent = JOY_NONE;
          UserEntry = 0;
        }
        break;
      }

    case WAVEPLAYER_STOPPED:
      switch (UserEvent)
      {
      case JOY_SEL:
        kWidgets_ProgressBarReset(hProgressBar);
        WavePlayer_PlaybackStart(sel);
        WavePlayer_DisplayBottomMessage(MSG_PLAYBACK_PLAYING);
        PlaybackStateUpdate("PLAYING");
        application_state = WAVEPLAYER_PLAYING;
        UserEvent = JOY_NONE;
        UserEntry = 0;
        break;

      case JOY_RIGHT:
        WavePlayer_PlaybackDeInit();
        application_state = WAVEPLAYER_SELECT_TITLE;
        UserEvent = JOY_NONE;
        UserEntry = 0;
        break;

      default:
        /* do noting */
        UserEvent = JOY_NONE;
        UserEntry = 0;
        break;
      }

      break;

    case WAVEPLAYER_EXIT :
      if (hProgressBar != NULL)
      {
        kWidgets_ProgressBarDestroy(hProgressBar);
        hProgressBar = NULL;
      }
      application_state = WAVEPLAYER_END;
      break;
    }
  } while(application_state != WAVEPLAYER_END);
}

/**
  * @brief  Get User action
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.
  * @retval None
  */
void WavePlayer_UserAction(uint8_t sel)
{
  if (UserEntry == 0)
  {
    UserEvent = sel;
    UserEntry = 1;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the hardware resources used by the audio player
  * @retval None
  */
static void WavePlayer_PlaybackInit(uint32_t SampleRate)
{
  TIM_MasterConfigTypeDef master_config = {0};
  GPIO_InitTypeDef        gpio_init = {0};
  DAC_ChannelConfTypeDef  dac_channel_config = {0};

  /* Enable TIM6 clock */
  __HAL_RCC_TIM6_CLK_ENABLE();

  /* TIM6 initialisation */
  htim.Instance = TIM6;
  htim.Init.Period            = (HAL_RCC_GetPCLK1Freq() / SampleRate) - 1;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim.Init.RepetitionCounter = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim);

  /* Master synchro config */
  master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
  master_config.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim, &master_config);

  /* GPIOA clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA4 (DAC_OUT1) and PA5 (DAC_OUT2)in analog mode */
  gpio_init.Pin   = GPIO_PIN_4 | GPIO_PIN_5;
  gpio_init.Mode  = GPIO_MODE_ANALOG;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &gpio_init);

  /* Enable DAC clock */
  __HAL_RCC_DAC1_CLK_ENABLE();

  /* DAC initialisation */
  hdac.Instance = DAC;
  HAL_DAC_Init(&hdac);

  /* Configure DAC channels */
  dac_channel_config.DAC_Trigger                 = DAC_TRIGGER_T6_TRGO;
  dac_channel_config.DAC_SampleAndHold           = DAC_SAMPLEANDHOLD_DISABLE;
  dac_channel_config.DAC_OutputBuffer            = DAC_OUTPUTBUFFER_ENABLE;
  dac_channel_config.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  dac_channel_config.DAC_UserTrimming            = DAC_TRIMMING_FACTORY;
  HAL_DAC_ConfigChannel(&hdac, &dac_channel_config, DAC_CHANNEL_1);
  HAL_DAC_ConfigChannel(&hdac, &dac_channel_config, DAC_CHANNEL_2);

  /* Enable DAC channels */
  __HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_1);
  __HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_2);

  /* DMA1 clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Set DMA descriptor for DAC channel 1 */
  hdma_dac_ch1.Instance = DMA1_Channel1;
  hdma_dac_ch1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_dac_ch1.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_dac_ch1.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_dac_ch1.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_dac_ch1.Init.Mode                = DMA_NORMAL;
  hdma_dac_ch1.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_dac_ch1.Init.Request             = DMA_REQUEST_DAC1_CHANNEL1;
  __HAL_LINKDMA(&hdac, DMA_Handle1, hdma_dac_ch1);
  HAL_DMA_Init(hdac.DMA_Handle1);

  /* NVIC configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
  * @brief  Releases the hardware resources used by the audio player
  * @retval None
  */
static void WavePlayer_PlaybackDeInit(void)
{
  /* Reset NVIC configuration */
  HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);

  /* Release DMA1 */
  HAL_DMA_DeInit(hdac.DMA_Handle1);
  __HAL_RCC_DMA1_CLK_DISABLE();

  /* Release PA4 (DAC_OUT1) and PA5 (DAC_OUT2) */
  HAL_GPIO_DeInit(GPIOA, (GPIO_PIN_4 | GPIO_PIN_5));

  /* Release DAC */
  HAL_DAC_DeInit(&hdac);
  __HAL_RCC_DAC1_CLK_DISABLE();
  __HAL_RCC_DAC1_FORCE_RESET();
  __HAL_RCC_DAC1_RELEASE_RESET();

  /* Reset DAC handle */
  memset(&hdac, 0, sizeof(DAC_HandleTypeDef));

  /* Release TIM6 */
  HAL_TIM_Base_DeInit(&htim);
  __HAL_RCC_TIM6_CLK_DISABLE();
  __HAL_RCC_TIM6_FORCE_RESET();
  __HAL_RCC_TIM6_RELEASE_RESET();
}

/**
  * @brief  Starts audio playback
  * @retval None
  */
static void WavePlayer_PlaybackStart(uint8_t Select)
{
  WAV_HeaderTypeDef wav_header;
  uint32_t          bytes_read;
  char              file_name[MAX_TITLE_NAME_LENGTH+5];

  sprintf(file_name, "USER/%s", PlayList[Select-1]);

  if (f_stat (file_name, &WaveFileInfo) != FR_OK)
  {
    while(1);
  }
  
  if (f_open(&WaveFile, file_name, FA_READ) != FR_OK)
  {
    while(1);
  }

  if (f_read(&WaveFile, (void*)&wav_header, WAV_HEADER_SIZE, (UINT*)&bytes_read) != FR_OK)
  {
    while(1);
  }

  if (WavePlayer_IsValidWAVHeader(&wav_header))
  {
    /* Initialize hardware resources */
    WavePlayer_PlaybackInit(wav_header.SampleRate);

    /* Jump to wave data */
    WavePlayer_JumpToWaveData(&WaveFile, &wav_header);

    if (strcmp(file_name, REC_FILE_NAME) != 0)
    {
      SignedToUnsignedConversion = 1;
    }
    else
    {
      SignedToUnsignedConversion = 0;
    }
    
    /* Prepare Buffers */
    WavePlayer_Prepare(&WaveFile, Buffer1, BUFFER_SIZE_WORD);
    WavePlayer_Prepare(&WaveFile, Buffer2, BUFFER_SIZE_WORD);

    /* Play buffer */
    WavePlayer_Play(Buffer1, BUFFER_SIZE_WORD);

    BufferSelect = Buffer2;
  }
}

/**
  * @brief  Stops audio playback
  * @retval None
  */
static void WavePlayer_PlaybackStop(void)
{
  /* Stop TIM6 */
  if (HAL_TIM_Base_Stop(&htim) != HAL_OK)
  {
    while(1);
  }

  /* Stop DAC conversions */
  if (HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1) != HAL_OK)
  {
    while(1);
  }

  /* Close Wave file */
  if (f_close(&WaveFile) != FR_OK)

  {
    while(1);
  }

}

/**
  * @brief  Pauses audio playback
  * @retval None
  */
static void WavePlayer_PlaybackPause(void)
{
  /* Stop TIM6 */
  if (HAL_TIM_Base_Stop(&htim) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @brief  Resumes audio playback
  * @retval None
  */
static void WavePlayer_PlaybackResume(void)
{
  /* Start TIM6 */
  if (HAL_TIM_Base_Start(&htim) != HAL_OK)
  {
    while(1);
  }
}

static void WavePlayer_PlaybackPlay(void)
{
  if (BufferSelect == Buffer2)
  {
    /* Play buffer 2 */
    WavePlayer_Play(Buffer2, BUFFER_SIZE_WORD);

    /* Prepare Buffer 1 */
    WavePlayer_Prepare(&WaveFile, Buffer1, BUFFER_SIZE_WORD);
  }
  else if (BufferSelect == Buffer1)
  {
    /* Play buffer 1 */
    WavePlayer_Play(Buffer1, BUFFER_SIZE_WORD);

    /* Prepare Buffer 2 */
    WavePlayer_Prepare(&WaveFile, Buffer2, BUFFER_SIZE_WORD);
  }

  DMATxComplete = 0;

  kWidgets_ProgressBarUpdate(hProgressBar, WaveFile.fptr);
}

/**
  * @brief  Fill in playback buffer from audio file
  * @param fp: audio file pointer
  * @param pBuffer: audio samples buffer
  * @param Size: audio samples buffer size (in words)
  * @retval None
  */
static void WavePlayer_Prepare(FIL* fp, uint32_t * pBuffer, uint32_t Size)
{
  uint32_t   bytes_read;
  uint32_t * pdata = pBuffer;
  uint32_t   i = 0;
  static uint16_t sample = 0;

  if (AudioRampUpRequested == 1)
  {
    /* Ramp-up audio reduce pop noise when audio playback is started */
    for (i = 0 ; i < BUFFER_SIZE_WORD ; i++)
    {
      (*pdata) = (sample << 16) | sample;
      pdata++;

      if (sample < (0x7FFF - STEP))
      {
        sample += STEP;
      }
      else
      {
        AudioRampUpRequested = 0;
      }
    }
  }
  else
  {
    sample = 0;

    /* Read audio samples */
    if (f_read(fp, (void*)pBuffer, Size*4, (UINT*)&bytes_read) != FR_OK)
    {
      while(1);
    }

    /* Invert sign bit (if required) */
    if (SignedToUnsignedConversion == 1)
    {
      /* PCM format is 16-bit signed while DAC is 12-bit unsigned */
      for(i = 0 ; i < Size ; i++)
      {
        (*pdata) ^= 0x80008000;
        pdata++;
      }
    }
  }
}

/**
  * @brief  Play the audio samples stored in the playback buffer
  * @param pBuffer: audio samples buffer
  * @param Size: audio samples buffer size (in words)
  * @retval None
  */
static void WavePlayer_Play(uint32_t * pBuffer, uint32_t Size)
{
  if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pBuffer, Size, DAC_ALIGN_12B_L) != HAL_OK)
  {
    while(1);
  }

  /* Overwrite DMA configuration to combine DMA transfers with dual channel conversion */
  hdac.DMA_Handle1->Instance->CPAR = (uint32_t)(&hdac.Instance->DHR12LD);

  if (HAL_TIM_Base_Start(&htim) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @brief  Initializes the playlist
  * @retval nof_titles: Number of titles
  */
static uint8_t WavePlayer_InitializePlayList(void)
{
  uint8_t nof_titles = 0;

  if (kStorage_GetDirectoryFiles((uint8_t *)"USER", KSTORAGE_FINDFIRST, (uint8_t *)PlayList[nof_titles], (uint8_t *)"wav") == KSTORAGE_NOERROR)
  {
    nof_titles++;

    /* Get next titles (if any) */
    while (kStorage_GetDirectoryFiles((uint8_t *)"USER", KSTORAGE_FINDNEXT, (uint8_t *)PlayList[nof_titles], (uint8_t *)"wav") == KSTORAGE_NOERROR)
    {
      nof_titles++;
    }
  }
  return nof_titles;
}

/**
  * @brief  Displays the title selection screen
  * @retval None
  */
static void WavePlayer_DisplayTitleSelectionScreen(void)
{
  uint16_t x      = 140;
  uint16_t y      = 35;
  uint16_t width  = 180;
  uint16_t height = 160;

  /* Black screen */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Display Title */
  BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  BSP_LCD_FillRect(0, LINE(0), BSP_LCD_GetXSize(), Font24.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)"Wave Player", CENTER_MODE);

  /* Display playback Icon */
  kStorage_OpenFileDrawBMP(35, 46, (uint8_t *)"STFILES/Music2.bmp");

  /* Display playlist list box */
  BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
  BSP_LCD_FillRect(x, y, width, height);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(x + 3, y + 3, width - 6, height - 6);
}

/**
  * @brief  Displays the play list
  * @param Select: actual selection
  * @param NbTitles: total number of titles
  * @retval None
  */
static void WavePlayer_DisplayPlayList(uint8_t Select, uint8_t NbTitles)
{
  uint8_t i;

  BSP_LCD_SetFont(&Font16);

  for (i = 0; i < NbTitles; i++)
  {
    if (i == Select - 1)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_ST_PINK);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
    else
    {
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
    }

    BSP_LCD_DisplayStringAt(145, 40 + Font16.Height*i, (uint8_t *)PlayList[i], NO_MODE);
  }

  BSP_LCD_SetFont(&Font24);
}

/**
  * @brief  Displays the playback screen
  * @param Select: title file name
  * @retval None
  */
static void WavePlayer_DisplayPlaybackScreen(uint8_t Select)
{
  char                  file_name[MAX_TITLE_NAME_LENGTH+5];
  FILINFO               fno;
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
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Wave Player", CENTER_MODE);

  /* Display playback Icon */
  kStorage_OpenFileDrawBMP(35, 46, (uint8_t *)"STFILES/Music2.bmp");

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_BLUE);
  BSP_LCD_DisplayStringAt(35, 46, (uint8_t *)PlayList[Select-1], CENTER_MODE);

  /* Retreive file size */
  sprintf(file_name, "USER/%s", PlayList[Select-1]);

  if (f_stat (file_name, &fno) != FR_OK)
  {
    while(1);
  }

  /* Display playback progress bar */
  hProgressBar = kWidgets_ProgressBarCreate(&progress_bar, fno.fsize);
}

/**
  * @brief  Displays the message at the bottom of the screen
  * @param msg: bottom message
  * @retval None
  */
static void WavePlayer_DisplayBottomMessage(char * msg)
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
  * @brief  Indicates whether or not the wav header of the audio file is valid
  * @param  WAV_header: WAV header
  * @retval 1 if the wav header is valid, 0 otherwise
  */
static uint8_t WavePlayer_IsValidWAVHeader(WAV_HeaderTypeDef * WAV_header)
{
  uint8_t status = 1;

  if (   (strncmp(WAV_header->RIFF, "RIFF", SUB_CHUNK_ID_LENGHT) != 0)
      || (strncmp(WAV_header->WAVE, "WAVE", SUB_CHUNK_ID_LENGHT) != 0)
      || (WAV_header->AudioFormat != 1))
  {
    status = 0;
  }
  return status;
}

/**
  * @brief  Move the file pointer to the first audio sample
  * @param  fp: audio file pointer
  * @param  WAV_header: WAV header
  * @retval AudioDataSize: size of the raw audio data
  */
static uint32_t WavePlayer_JumpToWaveData(FIL * fp, WAV_HeaderTypeDef * WAV_header)
{
  uint32_t bytes_read;
  char     sub_chunk_id[SUB_CHUNK_ID_LENGHT];
  uint32_t sub_chunk_size;
  uint32_t audio_data_size = 0;
  FSIZE_t  offset = 0;

  /* Move file pointer after fmt chunk */
  offset = FMT_CHK_POS + SUB_CHUNK_ID_LENGHT +  sizeof(uint32_t) + WAV_header->SubChunk1Size;
  f_lseek(fp, offset);

  /* Read the next sub-chunk id */
  f_read(fp, (void *)sub_chunk_id, SUB_CHUNK_ID_LENGHT, (UINT*)&bytes_read);
  offset += bytes_read;

  /* Skip next sub-chunk if not a 'data' sub-chunk */
  if (strncmp(sub_chunk_id, "data", SUB_CHUNK_ID_LENGHT) == 0)
  {
    f_read(fp, (void *)&audio_data_size, sizeof(uint32_t), (UINT*)&bytes_read);
  }
  else
  {
    f_read(fp, (void *)&sub_chunk_size, sizeof(uint32_t), (UINT*)&bytes_read);
    offset += (bytes_read + sub_chunk_size);
    f_lseek(fp, offset);

    /* Read the next sub-chunk id */
    f_read(fp, (void *)sub_chunk_id, SUB_CHUNK_ID_LENGHT, (UINT*)&bytes_read);

    if (strncmp(sub_chunk_id, "data", SUB_CHUNK_ID_LENGHT) == 0)
    {
      f_read(fp, (void *)&audio_data_size, sizeof(uint32_t), (UINT*)&bytes_read);;
    }
  }

  return audio_data_size;
}

/**
  * @brief  Conversion complete callback in non-blocking mode for Channel1
  * @param  hdac: DAC handle.
  * @retval None
  */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
  if (BufferSelect == Buffer2)
  {
    BufferSelect  = Buffer1;
    DMATxComplete = 1;
  }
  else
  {
    BufferSelect  = Buffer2;
    DMATxComplete = 1;
  }
}

/**
  * @brief  Update the displayed playback state
  * @param  PlaybackState: playback state
  * @retval None
  */
static void PlaybackStateUpdate(char * PlaybackState)
{
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_ST_PINK);
  BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)PlaybackState, CENTER_MODE);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
