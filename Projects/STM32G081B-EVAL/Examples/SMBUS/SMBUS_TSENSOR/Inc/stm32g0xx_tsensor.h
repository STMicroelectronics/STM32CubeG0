/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/stm32g0xx_tsensor.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for
  *          the stm32g0xx_tsensor.c temperature sensor driver.
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
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSENSOR_H
#define __TSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup TSENSOR
  * @{
  */

/** @defgroup TSENSOR_Exported_Types
  * @{
  */ 
/** 
  * @brief  Temperature Sensor driver structure definition  
  */
typedef struct
{
  uint8_t AlertMode;            /* Alert Mode Temperature out of range*/
  uint8_t ConversionMode;       /* Continuous/One Shot Mode */
  uint8_t TemperatureLimitHigh; /* High Temperature Limit Range */
  uint8_t TemperatureLimitLow;  /* Low Temperature Limit Range */
}TSENSOR_InitTypeDef;

/** 
  * @brief  TSENSOR Status structures definition  
  */  
typedef enum 
{
  TSENSOR_OK       = 0x00,
  TSENSOR_ERROR    = 0x01,
  TSENSOR_ALERT    = 0x02
} TSENSOR_StatusTypeDef;

/** @defgroup TSENSOR_Exported_Constants
  * @{
  */
/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/***************************** Read Access Only *******************************/
#define TSENSOR_REG_TEMP       0x00  /*!< Temperature Register of STLM75 */


   
/***************************** Read/Write Access ******************************/
#define TSENSOR_REG_CONF       0x01  /*!< Configuration Register of STLM75 */
#define TSENSOR_REG_THYS       0x02  /*!< Temperature Register of STLM75 */
#define TSENSOR_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of STLM75 */


/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
/** @defgroup Conversion_Mode_Selection 
  * @{
  */
#define TSENSOR_CONTINUOUS_MODE                  ((uint8_t)0x00)
#define TSENSOR_ONE_SHOT_MODE                    ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup Operation_Mode 
  * @{
  */
#define TSENSOR_COMPARATOR_MODE                  ((uint8_t)0x00)
#define TSENSOR_INTERRUPT_MODE                   ((uint8_t)0x02)
/**
  * @}
  */

/**
  * @}
  */
 
/** @defgroup TSENSOR_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
TSENSOR_StatusTypeDef   TSENSOR_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct);
TSENSOR_StatusTypeDef   TSENSOR_IsReady(uint16_t DeviceAddr, uint32_t Trials);

/* Sensor Request Functions */
uint8_t                 TSENSOR_ReadStatus(uint16_t DeviceAddr);
uint16_t                TSENSOR_ReadTemp(uint16_t DeviceAddr);
uint8_t                 TSENSOR_AlerteResponseAddressRead(void);

/* Sensor Callbacks */
void                    TSENSOR_ErrorCallback(uint16_t Error);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
 }
#endif
  
#endif /* __TSENSOR_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
