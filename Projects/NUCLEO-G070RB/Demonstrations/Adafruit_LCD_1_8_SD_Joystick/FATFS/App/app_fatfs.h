/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Adafruit_LCD_1_8_SD_Joystick/FatFs/App/app_fatfs.h
  * @author  MCD Application Team
  * @brief   Header for FatFs_uSD_Standalone.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_FATFS_H
#define __APP_FATFS_H

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* Header of a bitmap file */
#pragma pack(1) /* Mandatory to remove any padding */
typedef struct BmpHeader
{
  uint8_t  B;
  uint8_t  M;
  uint32_t fsize;
  uint16_t res1;
  uint16_t res2;
  uint32_t offset;
  uint32_t hsize;
  uint32_t w;
  uint32_t h;
  uint16_t planes;
  uint16_t bpp;
  uint32_t ctype;
  uint32_t dsize;
  uint32_t hppm;
  uint32_t vppm;
  uint32_t colorsused;
  uint32_t colorreq;
}BmpHeader;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
int32_t MX_FATFS_Init(void);
int32_t MX_FATFS_Process(void);
/* USER CODE BEGIN EFP */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char *BmpName);
uint32_t Storage_CopyFile(const char *BmpName1, const char *BmpName2);
uint32_t Storage_GetDirectoryBitmapFiles(const char* DirName, char* Files[]);
uint32_t Storage_CheckBitmapFile(const char *BmpName, uint32_t *FileLen);
uint8_t  Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

extern FATFS SDFatFs;    /* File system object for SD logical drive */
extern FIL SDFile;       /* File  object for SD */
extern char SDPath[4];   /* SD logical drive path */

#endif /*__APP_FATFS_H */
