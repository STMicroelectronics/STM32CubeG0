/**
  ******************************************************************************
  * @file    usbpd_devices_conf.h
  * @author  MCD Application Team
  * @brief   This file contains the device define.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef TRACER_EMB_CONF_H
#define TRACER_EMB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_dma.h"
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
      Definitions for TRACE feature
-------------------------------------------------------------------------------*/
#define TRACER_EMB_BAUDRATE                          921600UL

#define TRACER_EMB_DMA_MODE                          1UL
#define TRACER_EMB_IT_MODE                           0UL

#define TRACER_EMB_BUFFER_SIZE                       1024UL

/* -----------------------------------------------------------------------------
      Definitions for TRACE Hw information
-------------------------------------------------------------------------------*/

#define TRACER_EMB_IS_INSTANCE_LPUART_TYPE           0UL /* set to 1UL if LPUART is used instead of USART */
#define TRACER_EMB_USART_INSTANCE                    USART3

#define TRACER_EMB_TX_GPIO                           GPIOC
#define TRACER_EMB_TX_PIN                            LL_GPIO_PIN_10
#define TRACER_EMB_TX_AF                             LL_GPIO_AF_0
#define TRACER_EMB_TX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)
#define TRACER_EMB_RX_GPIO                           GPIOC
#define TRACER_EMB_RX_PIN                            LL_GPIO_PIN_11
#define TRACER_EMB_RX_AF                             LL_GPIO_AF_0
#define TRACER_EMB_RX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)

#define TRACER_EMB_ENABLE_CLK_USART()                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define TRACER_EMB_SET_CLK_SOURCE_USART()            /* No need for clock source selection in case of USART3 // LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK2) */
#define TRACER_EMB_USART_IRQ                         USART3_4_LPUART1_IRQn
#define TRACER_EMB_TX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#define TRACER_EMB_RX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#define TRACER_EMB_DMA_INSTANCE                      DMA1
#define TRACER_EMB_ENABLE_CLK_DMA()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define TRACER_EMB_TX_DMA_REQUEST                    LL_DMAMUX_REQ_USART3_TX
#define TRACER_EMB_TX_DMA_CHANNEL                    LL_DMA_CHANNEL_7
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Ch4_7_DMAMUX1_OVR_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler
#define TRACER_EMB_TX_DMA_ACTIVE_FLAG                LL_DMA_IsActiveFlag_TC7
#define TRACER_EMB_TX_DMA_CLEAR_FLAG                 LL_DMA_ClearFlag_GI7

#if (TRACER_EMB_IS_INSTANCE_LPUART_TYPE == 1UL)
#define TRACER_EMB_ENABLEDIRECTIONRX    LL_LPUART_EnableDirectionRx
#define TRACER_EMB_RECEIVE_DATA8        LL_LPUART_ReceiveData8
#define TRACER_EMB_TRANSMIT_DATA8       LL_LPUART_TransmitData8
#define TRACER_EMB_DMA_GETREGADDR       LL_LPUART_DMA_GetRegAddr
#define TRACER_EMB_ENABLEDMAREQ_TX      LL_LPUART_EnableDMAReq_TX
 
#define TRACER_EMB_ENABLE_IT_RXNE       LL_LPUART_EnableIT_RXNE
#define TRACER_EMB_ENABLE_IT_ERROR      LL_LPUART_EnableIT_ERROR
#define TRACER_EMB_ENABLE_IT_TXE        LL_LPUART_EnableIT_TXE

#define TRACER_EMB_DISABLEIT_TXE        LL_LPUART_DisableIT_TXE

#define TRACER_EMB_ISACTIVEFLAG_TXE     LL_LPUART_IsActiveFlag_TXE
#define TRACER_EMB_ISACTIVEFLAG_PE      LL_LPUART_IsActiveFlag_PE
#define TRACER_EMB_ISACTIVEFLAG_RXNE    LL_LPUART_IsActiveFlag_RXNE
#define TRACER_EMB_ISACTIVEFLAG_FE      LL_LPUART_IsActiveFlag_FE
#define TRACER_EMB_ISACTIVEFLAG_ORE     LL_LPUART_IsActiveFlag_ORE
#define TRACER_EMB_ISACTIVEFLAG_NE      LL_LPUART_IsActiveFlag_NE

#define TRACER_EMB_ISENABLEDIT_TXE      LL_LPUART_IsEnabledIT_TXE
#define TRACER_EMB_ISENABLEDIT_RXNE     LL_LPUART_IsEnabledIT_RXNE
#define TRACER_EMB_ISENABLEDIT_PE       LL_LPUART_IsEnabledIT_PE
#define TRACER_EMB_ISENABLEDIT_ERROR    LL_LPUART_IsEnabledIT_ERROR

#define TRACER_EMB_CLEARFLAG_PE         LL_LPUART_ClearFlag_PE
#define TRACER_EMB_CLEARFLAG_FE         LL_LPUART_ClearFlag_FE
#define TRACER_EMB_CLEARFLAG_ORE        LL_LPUART_ClearFlag_ORE
#define TRACER_EMB_CLEARFLAG_NE         LL_LPUART_ClearFlag_NE
#else
#define TRACER_EMB_ENABLEDIRECTIONRX    LL_USART_EnableDirectionRx
#define TRACER_EMB_RECEIVE_DATA8        LL_USART_ReceiveData8
#define TRACER_EMB_TRANSMIT_DATA8       LL_USART_TransmitData8
#define TRACER_EMB_DMA_GETREGADDR       LL_USART_DMA_GetRegAddr
#define TRACER_EMB_ENABLEDMAREQ_TX      LL_USART_EnableDMAReq_TX
 
#define TRACER_EMB_ENABLE_IT_RXNE       LL_USART_EnableIT_RXNE
#define TRACER_EMB_ENABLE_IT_ERROR      LL_USART_EnableIT_ERROR
#define TRACER_EMB_ENABLE_IT_TXE        LL_USART_EnableIT_TXE

#define TRACER_EMB_DISABLEIT_TXE        LL_USART_DisableIT_TXE

#define TRACER_EMB_ISACTIVEFLAG_TXE     LL_USART_IsActiveFlag_TXE
#define TRACER_EMB_ISACTIVEFLAG_PE      LL_USART_IsActiveFlag_PE
#define TRACER_EMB_ISACTIVEFLAG_RXNE    LL_USART_IsActiveFlag_RXNE
#define TRACER_EMB_ISACTIVEFLAG_RTO     LL_USART_IsActiveFlag_RTO
#define TRACER_EMB_ISACTIVEFLAG_FE      LL_USART_IsActiveFlag_FE
#define TRACER_EMB_ISACTIVEFLAG_ORE     LL_USART_IsActiveFlag_ORE
#define TRACER_EMB_ISACTIVEFLAG_NE      LL_USART_IsActiveFlag_NE

#define TRACER_EMB_ISENABLEDIT_TXE      LL_USART_IsEnabledIT_TXE
#define TRACER_EMB_ISENABLEDIT_RXNE     LL_USART_IsEnabledIT_RXNE
#define TRACER_EMB_ISENABLEDIT_PE       LL_USART_IsEnabledIT_PE
#define TRACER_EMB_ISENABLEDIT_RTO      LL_USART_IsEnabledIT_RTO
#define TRACER_EMB_ISENABLEDIT_ERROR    LL_USART_IsEnabledIT_ERROR

#define TRACER_EMB_CLEARFLAG_PE         LL_USART_ClearFlag_PE
#define TRACER_EMB_CLEARFLAG_RTO        LL_USART_ClearFlag_RTO
#define TRACER_EMB_CLEARFLAG_FE         LL_USART_ClearFlag_FE
#define TRACER_EMB_CLEARFLAG_ORE        LL_USART_ClearFlag_ORE
#define TRACER_EMB_CLEARFLAG_NE         LL_USART_ClearFlag_NE
#endif

#ifdef __cplusplus
}
#endif

#endif /* USBPD_DEVICE_CONF_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

