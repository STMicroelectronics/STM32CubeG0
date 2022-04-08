/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0xx_nucleo.h"
#include "stm32g0xx_ll_i2c.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_utils.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx_MASTER's NVIC */
#define I2Cx_MASTER_IRQn                        I2C1_IRQn
#define I2Cx_MASTER_IRQHandler                  I2C1_IRQHandler

/* Definition for I2Cx_SLAVE's NVIC */
#define I2Cx_SLAVE_IRQn                         I2C2_3_IRQn
#define I2Cx_SLAVE_IRQHandler                   I2C2_3_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                            (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                            TXBUFFERSIZE

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS1 0x13E
#define I2C_SLAVE_ADDRESS2 0xC8
#define I2C_TIMING 0x00303D5B
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
