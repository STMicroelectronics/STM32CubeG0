/**
  ******************************************************************************
  * @file    demo_application.h
  * @author  MCD Application Team
  * @brief   Header file for demo_application.h file
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

#ifndef __DEMO_APPLICATION_H_
#define __DEMO_APPLICATION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/*  MMI
     - 15-0 id event                       */
#define DEMO_MMI_ACTION_Pos                   (0U)
#define DEMO_MMI_ACTION_Msk                   (0xFF << DEMO_MMI_ACTION_Pos) /*!< 0x00FF */
#define DEMO_MMI_ACTION                       DPM_USER_ACTION_Msk
#define DEMO_MMI_ACTION_NONE                  (0u)
#define DEMO_MMI_ACTION_TAMPER_PRESS          (1u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_RIGHT_PRESS           (2u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_LEFT_PRESS            (3u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_UP_PRESS              (4u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DOWN_PRESS            (5u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_SEL_PRESS             (6u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_HIGH_PORT1 (7u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_LOW_PORT1  (8u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DISPLAY_VBUS_IBUS     (9u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_JOY_PRESS             (10u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_HIGH_PORT0 (11u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_HPD_DETECT_LOW_PORT0  (12u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_ERROR                 (13u << DEMO_MMI_ACTION_Pos)

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
DEMO_ErrorCode DEMO_InitBSP(void);
DEMO_ErrorCode DEMO_InitTask(void);
void DEMO_PostCADMessage(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc);
void DEMO_PostNotificationMessage(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal);
void DEMO_PostMMIMessage(uint32_t EventVal);
void DEMO_SetCableInfo(uint8_t PortNum, USBPD_DiscoveryIdentity_TypeDef *pIdentity);
void DEMO_Display_Error(uint8_t PortNum);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_APPLICATION_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
