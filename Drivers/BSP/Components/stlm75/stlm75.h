/**
  ******************************************************************************
  * @file    stlm75.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the stlm75.c
  *          temperature sensor driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2014 STMicroelectronics.
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
#ifndef __STLM75_H
#define __STLM75_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/tsensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup STLM75
  * @{
  */
  
/** @defgroup STLM75_Exported_Constants
  * @{
  */
/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/***************************** Read Access Only *******************************/
#define LM75_REG_TEMP       0x00  /*!< Temperature Register of LM75 */

/***************************** Read/Write Access ******************************/
#define LM75_REG_CONF       0x01  /*!< Configuration Register of LM75 */
#define LM75_REG_THYS       0x02  /*!< Temperature Register of LM75 */
#define LM75_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of LM75 */

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
/** @defgroup Conversion_Mode_Selection 
  * @{
  */
#define STLM75_CONTINUOUS_MODE                  ((uint8_t)0x00)
#define STLM75_ONE_SHOT_MODE                    ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup Operation_Mode 
  * @{
  */
#define STLM75_COMPARATOR_MODE                  ((uint8_t)0x00)
#define STLM75_INTERRUPT_MODE                   ((uint8_t)0x02)
/**
  * @}
  */

/**
  * @}
  */
 
/** @defgroup STLM75_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
void                      STLM75_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct);
uint8_t                   STLM75_IsReady(uint16_t DeviceAddr, uint32_t Trials);
/* Sensor Request Functions */
uint8_t                   STLM75_ReadStatus(uint16_t DeviceAddr);
float                     STLM75_ReadTemp(uint16_t DeviceAddr);

/* Temperature Sensor driver structure */
extern TSENSOR_DrvTypeDef Stlm75Drv;

/* Temperature Sensor IO functions */
void                      TSENSOR_IO_Init(void);
void                      TSENSOR_IO_Write(uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t Length);
void                      TSENSOR_IO_Read(uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t Length);
uint16_t                  TSENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
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
  
#endif /* __STTS751_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
