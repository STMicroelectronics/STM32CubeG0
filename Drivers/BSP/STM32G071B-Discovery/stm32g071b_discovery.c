/**
  ******************************************************************************
  * @file    stm32g071b_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage Leds,
  *          push-button and joystick of STM32G071B-DISCO board (MB1378)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery.h"

/** @addtogroup BSP
 * @{
 */

/** @addtogroup STM32G071B-DISCO
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        LCD display and power sensors (INA230) available on
  *        STM32G071B-DISCO board from STMicroelectronics.
  * @{
  */

/** @addtogroup STM32G071B_DISCOVERY_Common
  * @{
  */

/** @addtogroup STM32G071B_DISCOVERY_Private_Constants
  * @{
  */

/**
 * @brief STM32G071B DISCOVERY BSP Driver version number
   */
#define __STM32G071B_DISCOVERY_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32G071B_DISCOVERY_BSP_VERSION_SUB1   (0x02U) /*!< [23:16] sub1 version */
#define __STM32G071B_DISCOVERY_BSP_VERSION_SUB2   (0x01U) /*!< [15:8]  sub2 version */
#define __STM32G071B_DISCOVERY_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32G071B_DISCOVERY_BSP_VERSION            ((__STM32G071B_DISCOVERY_BSP_VERSION_MAIN << 24)\
                                                       |(__STM32G071B_DISCOVERY_BSP_VERSION_SUB1 << 16)\
                                                       |(__STM32G071B_DISCOVERY_BSP_VERSION_SUB2 << 8 )\
                                                       |(__STM32G071B_DISCOVERY_BSP_VERSION_RC))

#if defined(_GUI_INTERFACE)
const uint8_t HWBoardVersionName[] = "STM32G071B-DISCO";
const uint8_t PDTypeName[] = "MB1378B";
#endif /* _GUI_INTERFACE */

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_Exported_Variables Exported Variables
  * @{
  */

/**
 * @brief ALERT variables
 */
GPIO_TypeDef*   ALERT_PORT[ALERTn] =  {ALERT_VBUS_GPIO_PORT,
                                       ALERT_CC1_GPIO_PORT,
                                       ALERT_CC2_GPIO_PORT};

const uint16_t  ALERT_PIN[ALERTn] =   {ALERT_VBUS_PIN,
                                       ALERT_CC1_PIN,
                                       ALERT_CC2_PIN};

const uint8_t ALERT_IRQn[JOYn] =   {ALERT_VBUS_EXTI_IRQn,
                                    ALERT_CC1_EXTI_IRQn,
                                    ALERT_CC2_EXTI_IRQn};
/**
 * @brief LED variables
 */
GPIO_TypeDef*   LED_PORT[LEDn] =  {LED4_GPIO_PORT,
                                   LED5_GPIO_PORT,
                                   LED6_GPIO_PORT,
                                   LED7_GPIO_PORT};

const uint16_t  LED_PIN[LEDn] =   {LED4_PIN,
                                   LED5_PIN,
                                   LED6_PIN,
                                   LED7_PIN};


/**
 * @brief JOYSTICK variables
 */
 GPIO_TypeDef* JOY_PORT[JOYn] =  {SEL_JOY_GPIO_PORT,
                                  DOWN_JOY_GPIO_PORT,
                                  LEFT_JOY_GPIO_PORT,
                                  RIGHT_JOY_GPIO_PORT,
                                  UP_JOY_GPIO_PORT};

const uint16_t JOY_PIN[JOYn] =   {SEL_JOY_PIN,
                                  LEFT_JOY_PIN,
                                  RIGHT_JOY_PIN,
                                  DOWN_JOY_PIN,
                                  UP_JOY_PIN};

const uint8_t JOY_IRQn[JOYn] =   {SEL_JOY_EXTI_IRQn,
                                  LEFT_JOY_EXTI_IRQn,
                                  RIGHT_JOY_EXTI_IRQn,
                                  DOWN_JOY_EXTI_IRQn,
                                  UP_JOY_EXTI_IRQn};

/**
 * @brief MOSFET variables
 */
GPIO_TypeDef*   MOSFET_PORT[MOSFETn] = {MOSFET_ENCC1_GPIO_PORT,
                                        MOSFET_ENCC2_GPIO_PORT,
                                        MOSFET_RD_CC1_GPIO_PORT};

const uint16_t  MOSFET_PIN[MOSFETn]  =  {MOSFET_ENCC1_PIN,
                                         MOSFET_ENCC2_PIN,
                                         MOSFET_RD_CC1_PIN};

/**
 * @brief BUS variables
 */
#if defined(HAL_I2C_MODULE_ENABLED)
uint32_t I2c1Timeout = DISCOVERY_I2C1_TIMEOUT_MAX;  /*<! Value of Timeout when I2C1 communication fails */
static I2C_HandleTypeDef I2c1Handle;
static uint8_t I2C1RefCounter = 0;
#endif /* HAL_I2C_MODULE_ENABLED */

#if defined(HAL_SPI_MODULE_ENABLED)
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;            /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef SpiHandle;
#endif /* HAL_SPI_MODULE_ENABLED */
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_Private_macros Private macros
  * @{
  */
#if defined(HAL_SPI_MODULE_ENABLED)

/* LL definition */
#define __SPI_DIRECTION_2LINES(__HANDLE__)   do{\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             }while(0);

#define __SPI_DIRECTION_2LINES_RXONLY(__HANDLE__)   do{\
                                                   CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                                   SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY);\
                                                   }while(0);

#define __SPI_DIRECTION_1LINE_TX(__HANDLE__) do{\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             }while(0);

#define __SPI_DIRECTION_1LINE_RX(__HANDLE__) do {\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE);\
                                             } while(0);
#endif /* HAL_SPI_MODULE_ENABLED */
/**
  * @}
  */

/** @addtogroup STM32G071B_DISCOVERY_BUS_Operations_Functions
  * @{
  */
/**************************** Bus functions ************************************/
/* I2C1 bus function */
#if defined(HAL_I2C_MODULE_ENABLED)
static HAL_StatusTypeDef  I2C1_Init(void);
static void               I2C1_MspInit(I2C_HandleTypeDef *hi2c);
static void               I2C1_DeInit(void);
static void               I2C1_MspDeInit(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef  I2C1_WriteBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
static HAL_StatusTypeDef  I2C1_ReadBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
static void               I2C1_Error(void);
#endif/* HAL_I2C_MODULE_ENABLED */

/* SPIx bus function */
#if defined(HAL_SPI_MODULE_ENABLED)
static void               SPIx_Init(void);
static void               SPIx_MspInit(SPI_HandleTypeDef *hspi);
static void               SPIx_DeInit(void);
static void               SPIx_MspDeInit(void);
static void               SPIx_Write(uint8_t byte);
#endif
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LINK_Operations_Functions LINK Operations Functions
  * @{
  */
/**************************** Link functions ***********************************/
#if defined(HAL_I2C_MODULE_ENABLED)
/* Link functions for Power Monitoring peripheral */
uint8_t                   PWRMON_IO_Init(uint16_t Addr);
void                      PWRMON_IO_DeInit(uint16_t Addr);
uint8_t                   PWRMON_IO_Write(uint16_t Addr, uint8_t Reg, uint8_t * pData);
uint8_t                   PWRMON_IO_Read(uint16_t Addr, uint8_t Reg, uint8_t *pData);
void                      PWRMON_IO_EnableIT(uint16_t Addr, uint8_t ActiveEdge);
void                      PWRMON_IO_DisableIT(uint16_t Addr);
#endif/* HAL_I2C_MODULE_ENABLED */

#if defined(HAL_SPI_MODULE_ENABLED)
/* Link function for LCD peripheral */
void                      LCD_IO_Init(void);
void                      LCD_IO_DeInit(void);
void                      LCD_IO_WriteCommand(uint8_t Cmd);
void                      LCD_IO_WriteData(uint8_t Value);
void                      LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
#endif

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32G071B DISCOVERY BSP Driver revision
  * @retval version 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32G071B_DISCOVERY_BSP_VERSION;
}

#if defined(_GUI_INTERFACE)
/**
  * @brief  This method returns HW board version name
  * @retval HW Board version name
  */
const uint8_t* BSP_GetHWBoardVersionName(void)
{
  return HWBoardVersionName;
}

/**
  * @brief  This method returns HW PD Type name
  * @retval HW Board version name
  */
const uint8_t* BSP_GetPDTypeName(void)
{
  return PDTypeName;
}
#endif /* _GUI_INTERFACE */

/**
  * @brief  Configures LED GPIOs.
  * @param  Led Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = LED_PIN[Led];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);

  HAL_GPIO_WritePin(LED_PORT[Led], GPIO_InitStructure.Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Unconfigures LED GPIOs.
  * @param  Led Specifies the Led to be unconfigured.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
  HAL_GPIO_DeInit(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Configures all buttons of the joystick in GPIO or EXTI modes.
  * @param  Joy_Mode Joystick mode.
  *    This parameter can be one of the following values:
  *     @arg  JOY_MODE_GPIO: Joystick pins will be used as simple IOs
  *     @arg  JOY_MODE_EXTI: Joystick pins will be connected to EXTI line
  *                                 with interrupt generation capability
  * @retval HAL_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_JOY_Init(JOYMode_TypeDef Joy_Mode)
{
  JOYState_TypeDef joykey;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Initialized the Joystick. */
  for(joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    /* Enable the JOY clock */
    JOYx_GPIO_CLK_ENABLE(joykey);

    GPIO_InitStruct.Pin = JOY_PIN[joykey];
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    if (Joy_Mode == JOY_MODE_GPIO)
    {
      /* Configure Joy pin as input */
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);
    }
    else if (Joy_Mode == JOY_MODE_EXTI)
    {
      /* Configure Joy pin as input with External interrupt */
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);

      /* Enable and set Joy EXTI Interrupt to the lowest priority */
      HAL_NVIC_SetPriority((IRQn_Type)(JOY_IRQn[joykey]), 0x0F, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(JOY_IRQn[joykey]));
    }
  }

  return HAL_OK;
}

/**
  * @brief  Unonfigures all GPIOs used as buttons of the joystick.
  * @retval None.
  */
void BSP_JOY_DeInit(void)
{
  JOYState_TypeDef joykey;

  /* Initialized the Joystick. */
  for(joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    HAL_GPIO_DeInit(JOY_PORT[joykey], JOY_PIN[joykey]);
  }
}

/**
* @brief  Returns the current joystick status.
* @retval Code of the joystick key pressed
*          This code can be one of the following values:
*            @arg  JOY_NONE
*            @arg  JOY_SEL
*            @arg  JOY_DOWN
*            @arg  JOY_LEFT
*            @arg  JOY_RIGHT
*            @arg  JOY_UP
*/
JOYState_TypeDef BSP_JOY_GetState(void)
{
  JOYState_TypeDef joykey;

  for (joykey = JOY_SEL; joykey < (JOY_SEL + JOYn) ; joykey++)
  {
    if (HAL_GPIO_ReadPin(JOY_PORT[joykey], JOY_PIN[joykey]) == GPIO_PIN_SET)
    {
      /* Return Code Joystick key pressed */
      return joykey;
    }
  }

  /* No Joystick key pressed */
  return JOY_NONE;
}

/**
  * @brief  Configures MOSFET GPIOs.
  * @param  Mosfet Specifies the Mosfet GPIO to be configured.
  *   This parameter can be one of following parameters:
  *     @arg MOSFET_ENCC1
  *     @arg MOSFET_ENCC2
  *     @arg MOSFET_RD_CC1
  *     @arg MOSFET_RD_CC2
  * @retval None
  */
void BSP_MOSFET_Init(Mosfet_TypeDef Mosfet)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_MOSFET clock */
  MOSFETx_GPIO_CLK_ENABLE(Mosfet);

  /* Configure the GPIO_MOSFET pin */
  GPIO_InitStructure.Pin = MOSFET_PIN[Mosfet];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(MOSFET_PORT[Mosfet], &GPIO_InitStructure);

  HAL_GPIO_WritePin(MOSFET_PORT[Mosfet], GPIO_InitStructure.Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Unconfigures MOSFET GPIOs.
  * @param  Mosfet Specifies the Mosfet GPIO to be unconfigured.
  *   This parameter can be one of following parameters:
  *     @arg MOSFET_ENCC1
  *     @arg MOSFET_ENCC2
  *     @arg MOSFET_RD_CC1
  *     @arg MOSFET_RD_CC2
  * @retval None
  */
void BSP_MOSFET_DeInit(Mosfet_TypeDef Mosfet)
{
  HAL_GPIO_DeInit(MOSFET_PORT[Mosfet], MOSFET_PIN[Mosfet]);
}

/**
  * @brief  Turns selected MOSFET On.
  * @param  Mosfet Specifies the Mosfet to be set on.
  *   This parameter can be one of following parameters:
  *     @arg MOSFET_ENCC1
  *     @arg MOSFET_ENCC2
  *     @arg MOSFET_RD_CC1
  *     @arg MOSFET_RD_CC2
  * @retval None
  */
void BSP_MOSFET_On(Mosfet_TypeDef Mosfet)
{
  HAL_GPIO_WritePin(MOSFET_PORT[Mosfet], MOSFET_PIN[Mosfet], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected MOSFET Off.
  * @param  Mosfet Specifies the Mosfet to be set on.
  *   This parameter can be one of following parameters:
  *     @arg MOSFET_ENCC1
  *     @arg MOSFET_ENCC2
  *     @arg MOSFET_RD_CC1
  *     @arg MOSFET_RD_CC2
  * @retval None
  */
void BSP_MOSFET_Off(Mosfet_TypeDef Mosfet)
{
  HAL_GPIO_WritePin(MOSFET_PORT[Mosfet], MOSFET_PIN[Mosfet], GPIO_PIN_RESET);
}

/**
  * @brief  Configures Door Sensor in GPIO or EXTI modes.
  * @param  Door_Mode door sense mode.
  *    This parameter can be one of the following values:
  *     @arg  DOOR_MODE_GPIO: door sense pins will be used as simple IOs
  *     @arg  DOOR_MODE_EXTI: door sense pins will be connected to EXTI line
  *                                 with interrupt generation capability
  * @retval HAL_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_DOOR_Init(DOORMode_TypeDef Door_Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the DOOR sense clock */
  DOOR_SENSE_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = DOOR_SENSE_PIN;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  if (Door_Mode == DOOR_MODE_GPIO)
  {
    /* Configure Door sense pin as input */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DOOR_SENSE_PORT, &GPIO_InitStruct);
  }
  else if (Door_Mode == DOOR_MODE_EXTI)
  {
    /* Configure Door sensor pin as input with External interrupt */
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    HAL_GPIO_Init(DOOR_SENSE_PORT, &GPIO_InitStruct);

    /* Enable and set Door Sense EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(DOOR_SENSE_EXTI_IRQn), 0x03, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(DOOR_SENSE_EXTI_IRQn));
  }
  return HAL_OK;
}

/**
  * @brief  Unonfigures GPIO used for door sense.
  * @retval None.
  */
void BSP_DOOR_DeInit(void)
{
  /* UnInitialized the door sense. */
  HAL_GPIO_DeInit(DOOR_SENSE_PORT, DOOR_SENSE_PIN);
}

/**
* @brief  Returns the current Door sense position.
* @retval Code of the door sensor
*         This code can be one of the following values:
*            @arg  DOOR_OPEN
*            @arg  DOOR_CLOSE
*/
DOORState_TypeDef BSP_DOOR_GetState(void)
{
  if (HAL_GPIO_ReadPin(DOOR_SENSE_PORT, DOOR_SENSE_PIN) == GPIO_PIN_SET)
  {
    return DOOR_OPEN;
  }
  else
  {
    return DOOR_CLOSE;
  }
}

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_BUS_Operations_Functions BUS Operations Functions
  * @{
  */

/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
#if defined(HAL_SPI_MODULE_ENABLED)
/******************************* SPI Routines**********************************/
/**
  * @brief SPIx Bus initialization
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    SpiHandle.Instance = DISCOVERY_SPIx;
    /* SPI baudrate is set to 8 MHz (PCLK1/SPI_BaudRatePrescaler = 64/8 = 8 MHz)
      to verify these constraints:
      lsm303c SPI interface max baudrate is 10MHz for write/read
      PCLK1 max frequency is set to 64 MHz
      */
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;

    SPIx_MspInit(&SpiHandle);
    HAL_SPI_Init(&SpiHandle);
  }
}

/**
  * @brief SPI MSP Init
  * @param hspi SPI handle
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable SPIx clock  */
  DISCOVERY_SPIx_CLOCK_ENABLE();

  /* enable SPIx gpio clock */
  DISCOVERY_SPIx_GPIO_CLK_ENABLE();

  /* configure SPIx SCK, MOSI and MISO */
  GPIO_InitStructure.Pin       = (DISCOVERY_SPIx_SCK_PIN | DISCOVERY_SPIx_MOSI_PIN);
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = DISCOVERY_SPIx_AF;
  HAL_GPIO_Init(DISCOVERY_SPIx_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief SPIx Bus Deinitialization
  * @retval None
  */
void SPIx_DeInit(void)
{
  if(HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_RESET)
  {
    /* SPI Deinit */
    HAL_SPI_DeInit(&SpiHandle);
    SPIx_MspDeInit();
  }
}

/**
  * @brief SPI MSP DeInit
  * @retval None
  */
static void SPIx_MspDeInit(void)
{
  /* Unconfigure SPIx SCK, MOSI and MISO */
  HAL_GPIO_DeInit(DISCOVERY_SPIx_GPIO_PORT, (DISCOVERY_SPIx_SCK_PIN | DISCOVERY_SPIx_MOSI_PIN));

  DISCOVERY_SPIx_GPIO_FORCE_RESET();
  DISCOVERY_SPIx_GPIO_RELEASE_RESET();

  /* Disable SPIx clock  */
  DISCOVERY_SPIx_CLOCK_DISABLE();
}

/**
  * @brief  Sends a Byte through the SPI interface.
  * @param  Byte Byte to send.
  * @retval none
  */
static void SPIx_Write(uint8_t Byte)
{
  /* Enable the SPI */
  __HAL_SPI_ENABLE(&SpiHandle);
  /* check TXE flag */
  while((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

  /* Write the data */
  *((__IO uint8_t*)&SpiHandle.Instance->DR) = Byte;

  /* Wait BSY flag */
  while((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

  /* disable the SPI */
  __HAL_SPI_DISABLE(&SpiHandle);
}
#endif /* HAL_SPI_MODULE_ENABLED */


#if defined(HAL_I2C_MODULE_ENABLED)
/******************************* I2C Routines**********************************/
/**
  * @brief Discovery I2C1 Bus initialization
  * @retval None
  */
static HAL_StatusTypeDef I2C1_Init(void)
{
  HAL_StatusTypeDef result = HAL_OK;

  if(HAL_I2C_GetState(&I2c1Handle) == HAL_I2C_STATE_RESET)
  {
    I2c1Handle.Instance              = DISCOVERY_I2C1;
    I2c1Handle.Init.Timing           = DISCOVERY_I2C1_TIMING_400Khz;
    I2c1Handle.Init.OwnAddress1      = 0;
    I2c1Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2c1Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    I2c1Handle.Init.OwnAddress2      = 0;
    I2c1Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2c1Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    /* Init the I2C */
    I2C1_MspInit(&I2c1Handle);
    result = HAL_I2C_Init(&I2c1Handle);
  }

  /* Increment I2C1 reference counter */
  I2C1RefCounter++;

  return result;
}

/**
  * @brief Discovery I2C1 MSP Initialization
  * @param hi2c I2C handle
  * @retval None
  */
static void I2C1_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /*##-1- Set source clock to SYSCLK for I2C1 ################################################*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /*##-2- Configure the GPIOs ################################################*/

  /* Enable GPIO clock */
  DISCOVERY_I2C1_GPIO_CLK_ENABLE();

  /* Configure I2C SCL & SDA as alternate function  */
  GPIO_InitStruct.Pin       = (DISCOVERY_I2C1_SCL_PIN| DISCOVERY_I2C1_SDA_PIN);
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DISCOVERY_I2C1_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_I2C1_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the Discovery I2C peripheral #######################################*/
  /* Enable I2C clock */
  DISCOVERY_I2C1_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  DISCOVERY_I2C1_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  DISCOVERY_I2C1_RELEASE_RESET();
}

/**
  * @brief Discovery I2C1 Bus Deitialization
  * @retval None
  */
static void I2C1_DeInit(void)
{
  /* Decrement I2C1 reference counter */
  I2C1RefCounter--;

  if (I2C1RefCounter== 0)
  {
    /* Deinit the I2C */
    HAL_I2C_DeInit(&I2c1Handle);
    I2C1_MspDeInit(&I2c1Handle);
  }
}

/**
  * @brief Discovery I2C1 MSP Deinitialization
  * @param hi2c I2C handle
  * @retval None
  */
static void I2C1_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /*##-1- Unconfigure the GPIOs ################################################*/
  HAL_GPIO_DeInit(DISCOVERY_I2C1_GPIO_PORT, (DISCOVERY_I2C1_SCL_PIN | DISCOVERY_I2C1_SDA_PIN));

  /*##-2- Unconfigure the Discovery I2C1 peripheral ############################*/
  /* Force & Release the I2C Peripheral Clock Reset */
  DISCOVERY_I2C1_FORCE_RESET();
  DISCOVERY_I2C1_RELEASE_RESET();

  /* Disable Discovery I2C1 clock */
  DISCOVERY_I2C1_CLK_DISABLE();
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  Addr Device address on BUS Bus.
  * @param  Reg The target register address to write
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @param  pBuffer The target register value to be written
  * @param  Length buffer size to be written
  * @retval None
  */
static HAL_StatusTypeDef I2C1_WriteBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(&I2c1Handle, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2c1Timeout);

/* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the BUS */
    I2C1_Error();
  }
  return status;
}

/**
  * @brief  Reads multiple data on the BUS.
  * @param  Addr I2C Address
  * @param  Reg Reg Address
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @param  pBuffer pointer to read data buffer
  * @param  Length length of the data
  * @retval 0 if no problems to read multiple data
  */
static HAL_StatusTypeDef I2C1_ReadBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&I2c1Handle, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2c1Timeout);

/* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the BUS */
    I2C1_Error();
  }
  return status;
}

/**
  * @brief Discovery I2C1 error treatment function
  * @retval None
  */
static void I2C1_Error (void)
{
  /* De-initialize the I2C communication BUS */
  HAL_I2C_DeInit(&I2c1Handle);

  /* Re- Initiaize the I2C communication BUS */
  I2C1_Init();
}
#endif /*HAL_I2C_MODULE_ENABLED*/
/**
  * @}
  */

/** @addtogroup STM32G071B_DISCOVERY_LINK_Operations_Functions
  * @{
  */
/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/
#if defined(HAL_SPI_MODULE_ENABLED)
/*********************** LINK LCD ***********************************/
/**
  * @brief  Initialize LCD IO interface.
  * @retval None
  */
void LCD_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure the LCD Chip Select pin --------------------------------------*/
  LCD_CS_GPIO_CLK_ENABLE();

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_CS_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStruct);

  /* Set or Reset the control line */
  LCD_CS_LOW();

  /* Configure the LCD Data/Control pin -------------------------------------*/
  LCD_DC_GPIO_CLK_ENABLE();

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_DC_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStruct);

  /* Set or Reset the control line */
  LCD_DC_LOW();

  /* Configure the LCD Reset pin --------------------------------------------*/
  LCD_RST_GPIO_CLK_ENABLE();

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_RST_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);

  /* Initialized SPI Module */
  SPIx_Init();

  /* Reset LCD */
  LCD_RST_LOW();
  HAL_Delay(1);
  LCD_RST_HIGH();
}

/**
  * @brief  De-configures LCD IO interface.
  * @retval None
  */
void LCD_IO_DeInit(void)
{
  /* Set or Reset the control line */
  LCD_CS_LOW();
  
  /* Set or Reset the control line */
  LCD_DC_LOW();  
  
  /* Reset LCD */
  LCD_RST_LOW();
  HAL_Delay(1);
  LCD_RST_HIGH();

  /* Uninitialize SPI bus */
  SPIx_DeInit();

  /* Uninitialize LCD Chip Select Pin */  
  HAL_GPIO_DeInit(LCD_CS_GPIO_PORT, LCD_CS_PIN);
  /* Uninitialize LCD Data/Control Pin */  
  HAL_GPIO_DeInit(LCD_DC_GPIO_PORT, LCD_DC_PIN);
  /* Uninitialize LCD Reset Pin */  
  HAL_GPIO_DeInit(LCD_RST_GPIO_PORT, LCD_RST_PIN);
}

/**
  * @brief  Writes command on LCD register.
  * @param  Cmd Register to be written
  * @retval None
  */
void LCD_IO_WriteCommand(uint8_t Cmd)
{
  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();

  LCD_DC_LOW();

  SPIx_Write(Cmd);

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Writes one byte to the LCD
  * @param  Value  Data to be written
  * @retval None
 */
void LCD_IO_WriteData(uint8_t Value)
{
  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();

  LCD_DC_HIGH();

  SPIx_Write(Value);

  LCD_DC_LOW();

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Writes multiple data to the LCD
  * @param  pData pointer to the data to be written
  * @param  Size of data to be written
  * @retval none
  */
void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
  uint32_t counter = 0;

  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();

  LCD_DC_HIGH();

  /* disable the SPI */
  __HAL_SPI_DISABLE(&SpiHandle);

  if (Size == 1)
  {
    /* Only 1 byte to be sent to LCD - general interface can be used */
    /* Send Data */
    SPIx_Write(*pData);
  }
  else
  {
      /* Enable the SPI */
  __HAL_SPI_ENABLE(&SpiHandle);

    for (counter = Size; counter != 0; counter--)
    {
      while(((SpiHandle.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
      {
      }
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&SpiHandle.Instance->DR) = *(pData);

      pData ++;
    }

    /* Wait until the bus is ready before releasing Chip select */
    while(((SpiHandle.Instance->SR) & SPI_FLAG_BSY) != RESET)
    {
    }
  }

  /* disable the SPI */
  __HAL_SPI_DISABLE(&SpiHandle);

  LCD_DC_LOW();

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}
#endif /* HAL_SPI_MODULE_ENABLED */

#if defined(HAL_I2C_MODULE_ENABLED)
/****************************** LINK POWER MONITOR *****************************/
/**
  * @brief  Initializes Power Monitor low level.
  * @retval None
  */
uint8_t PWRMON_IO_Init(uint16_t Addr)
{
  uint8_t ret;
  ret = (I2C1_Init() == HAL_OK) ? 0 : 1;
  return ret;
}

/**
  * @brief  Deinitializes Power Monitor low level.
  * @retval None
  */
void PWRMON_IO_DeInit(uint16_t Addr)
{
  I2C1_DeInit();
}

/**
  * @brief  Writes a single data.
  * @param  Addr I2C address
  * @param  Reg Reg address
  * @param  pData pointer to the data to be written
  * @retval 0 success, else failed
  */
uint8_t PWRMON_IO_Write(uint16_t Addr, uint8_t Reg, uint8_t * pData)
{
  uint8_t ret;
  ret = (I2C1_WriteBuffer(Addr, Reg, I2C_MEMADD_SIZE_8BIT, pData, sizeof(uint16_t)) == HAL_OK) ? 0 : 1;
  return ret;
}

/**
  * @brief  Reads a single data.
  * @param  Addr I2C address
  * @param  Reg Reg address
  * @param  pData pointer to the data
  * @retval 0 success, else failed
  */
uint8_t PWRMON_IO_Read(uint16_t Addr, uint8_t Reg, uint8_t *pData)
{
  uint8_t ret;
  ret = (I2C1_ReadBuffer(Addr, Reg, I2C_MEMADD_SIZE_8BIT, pData, sizeof(uint16_t)) == HAL_OK) ? 0 : 1;
  return ret;
}

/**
  * @brief  Enable ALERT Interrupt.
  * @param  Addr I2C address
  * @param  ActiveEdge active edge (0: falling - 1: Rising)
  * @retval None
  */
void PWRMON_IO_EnableIT(uint16_t Addr, uint8_t ActiveEdge)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  Alert_TypeDef     alert = ALERT_VBUS;

  if (Addr == VBUS_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_VBUS;
  }
  else if (Addr == CC1_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_CC1;
  }
  else if (Addr == CC2_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_CC2;
  }

  /* Enable the GPIO clock */
  ALERTx_GPIO_CLK_ENABLE(alert);

  /* Configure ALERT GPIO pin */
  GPIO_InitStruct.Pin   = ALERT_PIN[alert];
  GPIO_InitStruct.Pull  = GPIO_PULLUP;//GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  if (ActiveEdge == 0)
  {
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
  }
  else
  {
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
  }
  HAL_GPIO_Init(ALERT_PORT[alert], &GPIO_InitStruct);

  /* Enable and set GPIO EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)ALERT_IRQn[alert], 0x0F, 0x0F);
  HAL_NVIC_EnableIRQ((IRQn_Type)ALERT_IRQn[alert]);
}

/**
  * @brief  Disable ALERT Interrupt.
  * @retval None
  */
void PWRMON_IO_DisableIT(uint16_t Addr)
{
  Alert_TypeDef     alert = ALERT_VBUS;

  if (Addr == VBUS_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_VBUS;
  }
  else if (Addr == CC1_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_CC1;
  }
  else if (Addr == CC2_SENSING_I2C_ADDRESS)
  {
    alert = ALERT_CC2;
  }

  HAL_GPIO_DeInit(ALERT_PORT[alert], ALERT_PIN[alert]);

  /* Do not disable GPIO EXTI Interrupt as the same EXTI line may be shared by
   * several ALERT pins.
   */
}

#endif /* HAL_I2C_MODULE_ENABLED */
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
