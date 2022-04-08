/**
  ******************************************************************************
  * @file    app_audio.c
  * @author  MCD Application Team
  * @brief   Audio application implementation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#define __APP_AUDIO_C

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_audio.h"
#include "wave_player.h"
#include "wave_recorder.h"
#include "stm32g081b_eval_lcd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"

/* Private typedef -----------------------------------------------------------*/    
/* Private constants ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static KMODULE_RETURN _AudioDemoExec(void);

/* Private Variable ----------------------------------------------------------*/

/* Wave record menu */
//static const tMenuItem WavRecordMenuItems[] =
//{
//  {"Record" , 14, 30, SEL_EXEC, MODULE_NONE, WaveRecorder_Demo, WaveRecorder_UserAction, NULL, NULL, NULL },
//  {"Return" ,  0,  0, SEL_EXIT, MODULE_NONE, NULL            , NULL                  , NULL, NULL, NULL }
//};
//
//static const tMenu WavRecorderMenu = {
//  "Wave Record", WavRecordMenuItems, countof(WavRecordMenuItems), TYPE_TEXT, 1, 1
//};

static const tMenuItem AudioMenuMenuItems[] =
{
    {"Wave Player"  , 14, 30, SEL_EXEC,    MODULE_NONE, WavePlayer_Demo,   WavePlayer_UserAction,   NULL, NULL, NULL },
    {"Wave Recorder", 14, 30, SEL_EXEC,    MODULE_NONE, WaveRecorder_Demo, WaveRecorder_UserAction, NULL, NULL, NULL },
    {"Return"       ,  0,  0, SEL_EXIT,    MODULE_NONE, NULL,              NULL,                    NULL, NULL, NULL }
};

static const tMenu AudioMenu = {
  "Audio", AudioMenuMenuItems, countof(AudioMenuMenuItems), TYPE_TEXT, 1, 1 };

/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAudio =
{
  MODULE_AUDIO,
  NULL,
  _AudioDemoExec,
  NULL,
  NULL
};  

/* WAVE file descriptor */
FIL WaveFile = {0};

/* Peripherals HAL handles */
DAC_HandleTypeDef hdac = {0};
ADC_HandleTypeDef hadc = {0};

/* Audio samples buffer */
uint32_t Buffer1[BUFFER_SIZE_WORD];
uint32_t Buffer2[BUFFER_SIZE_WORD];

/* Active audio buffer  */
__IO uint32_t * BufferSelect;

/* Progress bar handle */
GLOBAL void * hProgressBar = NULL;

/* Exported variables --------------------------------------------------------*/
/**
  * @brief  Run the applications 
  * @param  None.
  * @retval None.
  */
static KMODULE_RETURN _AudioDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(AudioMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}
