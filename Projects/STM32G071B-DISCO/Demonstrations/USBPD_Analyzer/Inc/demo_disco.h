/**
  ******************************************************************************
  * @file    demo_disco.h
  * @author  MCD Application Team
  * @brief   Header file for demo_application.h file
  ******************************************************************************
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

#ifndef __DEMO_APPLICATION_H_
#define __DEMO_APPLICATION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DISCO
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/
typedef enum {
  MODE_SPY,
  MODE_STANDALONE,
} DEMO_MODE;

/* Exported define -----------------------------------------------------------*/
/*  MMI
     - 15-0 id event                       */
#define DEMO_MMI_ACTION_Pos                   (0U)
#define DEMO_MMI_ACTION_Msk                   (0xFF << DEMO_MMI_ACTION_Pos) /*!< 0x00FF */
#define DEMO_MMI_ACTION                       DPM_USER_ACTION_Msk
#define DEMO_MMI_ACTION_NONE                  (0u)
#define DEMO_MMI_ACTION_RIGHT_PRESS           (2u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_LEFT_PRESS            (3u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_UP_PRESS              (4u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DOWN_PRESS            (5u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_SEL_PRESS             (6u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_HIGH_PORT1 (7u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_LOW_PORT1  (8u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DISPLAY_VBUS_IBUS     (9u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_JOY_PRESS             (10u << DEMO_MMI_ACTION_Pos)

/*
 * Number af thread defined by user to include in the low power control
 */
#define USBPD_USER_THREAD_COUNT    0
/* Exported constants --------------------------------------------------------*/
typedef enum{
     DEMO_OK,
     DEMO_ERROR
} DEMO_ErrorCode;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

DEMO_ErrorCode  DEMO_InitBSP(void);
DEMO_ErrorCode  DEMO_InitTask(DEMO_MODE mode);
void            DEMO_SPY_Handler(void);
uint8_t         DEMO_IsSpyMode(void);
void            DEMO_PostCADMessage(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc);
void            DEMO_PostNotificationMessage(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal);
void            DEMO_PostMMIMessage(uint32_t EventVal);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_APPLICATION_H_ */
