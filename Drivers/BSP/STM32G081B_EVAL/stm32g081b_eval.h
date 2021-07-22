/**
  ******************************************************************************
  * @file    stm32g081b_eval.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32G081B_EVAL's Leds, push-buttons
  *          and COM port hardware resources.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G081B_EVAL_H
#define STM32G081B_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32G081B_EVAL STM32G081B EVAL
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include <stdio.h>

/** @defgroup STM32G081B_EVAL_Common STM32G081B EVAL Common
  * @{
  */

/** @defgroup STM32G081B_EVAL_Private_Constants Private Constants
 * @{
 */
/**
* @}
*/

/** @defgroup STM32G081B_EVAL_Private_Variables Private Variables
 * @{
 */
/**
* @}
*/

/** @defgroup STM32G081B_EVAL_Exported_Types Exported Types
  * @{
  */

/**
 * @brief LED Types Definition
 */
typedef enum
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3,
    /* Color led aliases */
    LED_GREEN  = LED1,
    LED_ORANGE = LED2,
    LED_RED    = LED3,
    LED_BLUE   = LED4
} Led_TypeDef;

/**
 * @brief BUTTON Types Definition
 */
typedef enum
{
    BUTTON_TAMPER = 0
} Button_TypeDef;

typedef enum
{
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/**
 * @brief JOYSTICK Types Definition
 */
typedef enum
{
    JOY_SEL = 0,
    JOY_DOWN = 1,
    JOY_LEFT = 2,
    JOY_RIGHT = 3,
    JOY_UP = 4,
    JOY_NONE = 5
} JOYState_TypeDef;

typedef enum
{
    JOY_MODE_GPIO = 0,
    JOY_MODE_EXTI = 1
} JOYMode_TypeDef;

/**
 * @brief COM Types Definition
 */
typedef enum
{
    COM1 = 0
} COM_TypeDef;

/**
 * @brief Daughter Board (DB) Types Definition
 */
typedef enum
{
    DB_ID_NONE = 0,
    DB_ID_LEGACY,
    DB_ID_UCPD_AB,
    DB_ID_UCPD_C
} DBId_TypeDef;

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @brief  Define for STM32G081B_EVAL board
  */
#if !defined (USE_STM32G081B_EVAL)
#define USE_STM32G081B_EVAL
#endif

#if defined(USE_STM32G081B_EVAL_REVA)
#define EVAL_BOARD_REVISION ((uint8_t *)"REV. A")
#elif defined(USE_STM32G081B_EVAL_REVB)
#define EVAL_BOARD_REVISION ((uint8_t *)"REV. B")
#else
#define EVAL_BOARD_REVISION ((uint8_t *)"REV. C")
#endif

/** @defgroup STM32G081B_EVAL_LED STM32G081B-EVAL LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_5  /* PD.05 */
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED2_PIN                         GPIO_PIN_6  /* PD.06 */
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_8  /* PD.08 */
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED4_PIN                         GPIO_PIN_9  /* PD.09 */
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)    do { if((__LED__) == LED1) LED1_GPIO_CLK_ENABLE(); else \
                                          if((__LED__) == LED2) LED2_GPIO_CLK_ENABLE(); else \
                                          if((__LED__) == LED3) LED3_GPIO_CLK_ENABLE(); else \
                                          if((__LED__) == LED4) LED4_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)   (((__LED__) == LED1) ? LED1_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED2) ? LED2_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED3) ? LED3_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED4) ? LED4_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_BUTTON STM32G081B-EVAL BUTTON
  * @{
  */
#define JOYn                             5
#define BUTTONn                          1

/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PIN                   GPIO_PIN_13  /* PC.13 */
#define TAMPER_BUTTON_GPIO_PORT             GPIOC
#define TAMPER_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define TAMPER_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define TAMPER_BUTTON_EXTI_IRQn             EXTI4_15_IRQn

#define TAMPERx_GPIO_CLK_ENABLE(__BUTTON__)    do { if((__BUTTON__) == BUTTON_TAMPER) TAMPER_BUTTON_GPIO_CLK_ENABLE();} while(0)

#define TAMPERx_GPIO_CLK_DISABLE(__BUTTON__)   (((__BUTTON__) == BUTTON_TAMPER) ? TAMPER_BUTTON_GPIO_CLK_DISABLE(): 0 )

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_JOY_PIN                 GPIO_PIN_7  /* PC.07 */
#define RIGHT_JOY_GPIO_PORT           GPIOC
#define RIGHT_JOY_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define RIGHT_JOY_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define RIGHT_JOY_EXTI_IRQn           EXTI4_15_IRQn

/**
 * @brief Joystick Left push-button
 */
#define LEFT_JOY_PIN                  GPIO_PIN_8  /* PC.08 */
#define LEFT_JOY_GPIO_PORT            GPIOC
#define LEFT_JOY_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define LEFT_JOY_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISABLE()
#define LEFT_JOY_EXTI_IRQn            EXTI4_15_IRQn

/**
 * @brief Joystick Up push-button
 */
#define UP_JOY_PIN                    GPIO_PIN_2  /* PC.02 */
#define UP_JOY_GPIO_PORT              GPIOC
#define UP_JOY_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UP_JOY_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define UP_JOY_EXTI_IRQn              EXTI2_3_IRQn

/**
 * @brief Joystick Down push-button
 */
#define DOWN_JOY_PIN                  GPIO_PIN_3   /* PC.03 */
#define DOWN_JOY_GPIO_PORT            GPIOC
#define DOWN_JOY_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define DOWN_JOY_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISABLE()
#define DOWN_JOY_EXTI_IRQn            EXTI2_3_IRQn

/**
 * @brief Joystick Sel push-button
 */
#define SEL_JOY_PIN                   GPIO_PIN_0   /* PA.00 */
#define SEL_JOY_GPIO_PORT             GPIOA
#define SEL_JOY_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define SEL_JOY_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define SEL_JOY_EXTI_IRQn             EXTI0_1_IRQn

#define JOYx_GPIO_CLK_ENABLE(__JOY__)    do { if((__JOY__) == JOY_SEL) SEL_JOY_GPIO_CLK_ENABLE(); else \
                                          if((__JOY__) == JOY_DOWN) DOWN_JOY_GPIO_CLK_ENABLE(); else \
                                          if((__JOY__) == JOY_LEFT) LEFT_JOY_GPIO_CLK_ENABLE(); else \
                                          if((__JOY__) == JOY_RIGHT) RIGHT_JOY_GPIO_CLK_ENABLE(); else \
                                          if((__JOY__) == JOY_UP) UP_JOY_GPIO_CLK_ENABLE();} while(0)

#define JOYx_GPIO_CLK_DISABLE(__JOY__)   (((__JOY__) == JOY_SEL) ? SEL_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_DOWN) ? DOWN_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_LEFT) ? LEFT_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_RIGHT) ? RIGHT_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_UP) ? UP_JOY_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_COM STM32G081B-EVAL COM
  * @{
  */
#define COMn                            1

/**
 * @brief Definition for COM port1, connected to USART1
 */
#define EVAL_COM1                       USART1
#define EVAL_COM1_CLK_ENABLE()          __HAL_RCC_USART1_CLK_ENABLE()
#define EVAL_COM1_CLK_DISABLE()         __HAL_RCC_USART1_CLK_DISABLE()

#define EVAL_COM1_TX_PIN                GPIO_PIN_4 /* PC.04 */
#define EVAL_COM1_TX_GPIO_PORT          GPIOC
#define EVAL_COM1_TX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_COM1_TX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()
#define EVAL_COM1_TX_AF                 GPIO_AF1_USART1

#define EVAL_COM1_RX_PIN                GPIO_PIN_5  /* PC.05 */
#define EVAL_COM1_RX_GPIO_PORT          GPIOC
#define EVAL_COM1_RX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_COM1_RX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()
#define EVAL_COM1_RX_AF                 GPIO_AF1_USART1

#define EVAL_COM1_CTS_PIN               GPIO_PIN_11 /* PA.11 */
#define EVAL_COM1_CTS_GPIO_PORT         GPIOA
#define EVAL_COM1_CTS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM1_CTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define EVAL_COM1_CTS_AF                GPIO_AF1_USART1

#define EVAL_COM1_RTS_PIN               GPIO_PIN_12 /* PA.12 */
#define EVAL_COM1_RTS_GPIO_PORT         GPIOA
#define EVAL_COM1_RTS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM1_RTS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define EVAL_COM1_RTS_AF                GPIO_AF1_USART1

#define EVAL_COM1_IRQn                  USART1_IRQn

#define COMx_CLK_ENABLE(__COM__)        do { if((__COM__) == COM1) EVAL_COM1_CLK_ENABLE();} while(0)
#define COMx_CLK_DISABLE(__COM__)       (((__COM__) == COM1) ? EVAL_COM1_CLK_DISABLE() : 0)

#define COMx_TX_GPIO_CLK_ENABLE(__COM__) do { if((__COM__) == COM1) EVAL_COM1_TX_GPIO_CLK_ENABLE();} while(0)
#define COMx_TX_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? EVAL_COM1_TX_GPIO_CLK_DISABLE() : 0)

#define COMx_RX_GPIO_CLK_ENABLE(__COM__) do { if((__COM__) == COM1) EVAL_COM1_RX_GPIO_CLK_ENABLE();} while(0)
#define COMx_RX_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? EVAL_COM1_RX_GPIO_CLK_DISABLE() : 0)

#define COMx_CTS_GPIO_CLK_ENABLE(__COM__) do { if((__COM__) == COM1) EVAL_COM1_CTS_GPIO_CLK_ENABLE();} while(0)
#define COMx_CTS_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? EVAL_COM1_CTS_GPIO_CLK_DISABLE() : 0)

#define COMx_RTS_GPIO_CLK_ENABLE(__COM__) do { if((__COM__) == COM1) EVAL_COM1_RTS_GPIO_CLK_ENABLE();} while(0)
#define COMx_RTS_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? EVAL_COM1_RTS_GPIO_CLK_DISABLE() : 0)

#if defined(HAL_I2C_MODULE_ENABLED)
/*##################### I2Cx ###################################*/
/* User can use this section to tailor I2Cx instance used and associated resources */
/* Definition for I2C1 Pins */
#define EVAL_I2C1                        I2C1
#define EVAL_I2C1_CLK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define EVAL_I2C1_CLK_DISABLE()          __HAL_RCC_I2C1_CLK_DISABLE()
#define EVAL_I2C1_FORCE_RESET()          __HAL_RCC_I2C1_FORCE_RESET()
#define EVAL_I2C1_RELEASE_RESET()        __HAL_RCC_I2C1_RELEASE_RESET()

#define EVAL_I2C1_SCL_PIN                GPIO_PIN_6		/* PB.6 */
#define EVAL_I2C1_SDA_PIN                GPIO_PIN_7		/* PB.7 */

#define EVAL_I2C1_GPIO_PORT		 GPIOB			/* GPIOB */
#define EVAL_I2C1_GPIO_CLK_ENABLE()    	 __HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_I2C1_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()
#define EVAL_I2C1_SCL_SDA_AF		 GPIO_AF6_I2C1

/* Definition for I2C2 Pins */
#define EVAL_I2C2                       I2C2
#define EVAL_I2C2_CLK_ENABLE()          __HAL_RCC_I2C2_CLK_ENABLE()
#define EVAL_I2C2_CLK_DISABLE()         __HAL_RCC_I2C2_CLK_DISABLE()
#define EVAL_I2C2_FORCE_RESET()         __HAL_RCC_I2C2_FORCE_RESET()
#define EVAL_I2C2_RELEASE_RESET()       __HAL_RCC_I2C2_RELEASE_RESET()

#define EVAL_I2C2_SCL_PIN               GPIO_PIN_13		/* PB.13 */
#define EVAL_I2C2_SDA_PIN               GPIO_PIN_14		/* PB.14 */

#define EVAL_I2C2_GPIO_PORT           	GPIOB                   /* GPIOB */
#define EVAL_I2C2_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_I2C2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define EVAL_I2C2_AF                    GPIO_AF6_I2C2

/* Definition for I2C2 NVIC */
#define EVAL_I2C2_IRQn                  I2C2_IRQn

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define EVAL_I2C1_TIMEOUT_MAX            1000
#define EVAL_I2C2_TIMEOUT_MAX            1000

/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 48 MHz */
/* Set TIMING to 0x00E0D3FF to reach 100 KHz speed (Rise time = 50ns, Fall time = 10ns) */
#define I2C2_TIMING                      0x00E0D3FF
#define I2C1_TIMING                      0x00E0D3FF

/* MUX 1 - USB Type-C Crossbar Switch I2C address (0b1000100x) */
#define MUX_1_TYPEC_SWITCH_I2C_ADDRESS                      0x88U

/* MUX 1 - DisplayPort Linear Redriver I2C address (0b1000100x) */
#define MUX_1_DP_REDRIVER_I2C_ADDRESS                       0x88U

/* MUX 2 - USB Type-C Crossbar Switch I2C address (0b0110000x) */
#define MUX_2_TYPEC_SWITCH_I2C_ADDRESS                      0x60U

/* MUX 2 - DisplayPort Linear Redriver I2C address (0b0000000x) */
#define MUX_2_DP_REDRIVER_I2C_ADDRESS                       0x00U

#endif /* HAL_I2C_MODULE_ENABLED */

#if defined(HAL_SPI_MODULE_ENABLED)
/**
  * @brief  Definition for SPI Interface pins (SPI1 used)
  */
#define EVAL_SPIx                             SPI1
#define EVAL_SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define EVAL_SPIx_CLK_DISABLE()               __HAL_RCC_SPI1_CLK_DISABLE()
#define EVAL_SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define EVAL_SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

#define EVAL_SPIx_SCK_PIN                     GPIO_PIN_3             /* PB.03 */
#define EVAL_SPIx_SCK_GPIO_PORT               GPIOB                  /* GPIOB */
#define EVAL_SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_SPIx_SCK_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOB_CLK_DISABLE()
#define EVAL_SPIx_SCK_AF                      GPIO_AF0_SPI1

#define EVAL_SPIx_MISO_PIN                    GPIO_PIN_4             /* PB.04 */
#define EVAL_SPIx_MISO_GPIO_PORT              GPIOB                  /* GPIOB */
#define EVAL_SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_SPIx_MISO_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()
#define EVAL_SPIx_MISO_AF                     GPIO_AF0_SPI1

#define EVAL_SPIx_MOSI_PIN                    GPIO_PIN_7             /* PA.07 */
#define EVAL_SPIx_MOSI_GPIO_PORT              GPIOA                  /* GPIOA */
#define EVAL_SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_SPIx_MOSI_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()
#define EVAL_SPIx_MOSI_AF                     GPIO_AF0_SPI1

#define EVAL_SPIx_MOSI_DIR_PIN                GPIO_PIN_12             /* PC.12 */
#define EVAL_SPIx_MOSI_DIR_GPIO_PORT          GPIOC                  /* GPIOC */
#define EVAL_SPIx_MOSI_DIR_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_SPIx_MOSI_DIR_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define EVAL_SPIx_TIMEOUT_MAX                 1000

#endif /* HAL_SPI_MODULE_ENABLED */
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_COMPONENT STM32G081B_EVAL COMPONENT
  * @{
  */
/*##################### LCD ###################################*/
/* Chip Select macro definition */
#define LCD_CS_LOW()                    HAL_GPIO_WritePin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()                   HAL_GPIO_WritePin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, GPIO_PIN_SET)
/**
  * @brief  LCD Control pins
  */
#define LCD_NCS_PIN                     GPIO_PIN_8              /* PB. 08*/
#define LCD_NCS_GPIO_PORT               GPIOB                   /* GPIOB */
#define LCD_NCS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_NCS_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOB_CLK_DISABLE()


/*##################### SD ###################################*/
/* Chip Select macro definition */
#define SD_CS_LOW()                     HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()                    HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)
/**
  * @brief SD card Control pin
  */
#define SD_CS_PIN                       GPIO_PIN_1              /* PD.01 */
#define SD_CS_GPIO_PORT                 GPIOD                   /* GPIOD */
#define SD_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()

/**
  * @brief  SD Detect Interface pins
  */
#define SD_DETECT_PIN                   GPIO_PIN_9              /* PC.09 */
#define SD_DETECT_GPIO_PORT             GPIOC                   /* GPIOC */
#define SD_DETECT_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn             EXTI4_15_IRQn


/*##################### HDMI-CEC ###################################*/
/**
  * @brief  I2C HDMI CEC Interface pins
  */
#define HDMI_CEC_HPD_SINK_PIN             GPIO_PIN_2  /* PD.02 */
#define HDMI_CEC_HPD_SINK_GPIO_PORT       GPIOD
#define HDMI_CEC_HPD_SINK_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
#define HDMI_CEC_HPD_SINK_CLK_DISABLE()   __HAL_RCC_GPIOD_CLK_DISABLE()

#if defined(USE_STM32G081B_EVAL_REVA)
#define HDMI_CEC_HPD_SOURCE_PIN           GPIO_PIN_3  /* PD.03 */
#define HDMI_CEC_HPD_SOURCE_GPIO_PORT     GPIOD
#define HDMI_CEC_HPD_SOURCE_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()
#define HDMI_CEC_HPD_SOURCE_CLK_DISABLE() __HAL_RCC_GPIOD_CLK_DISABLE()
#else
#define HDMI_CEC_HPD_SOURCE_PIN           GPIO_PIN_6  /* PC.06 */
#define HDMI_CEC_HPD_SOURCE_GPIO_PORT     GPIOC
#define HDMI_CEC_HPD_SOURCE_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define HDMI_CEC_HPD_SOURCE_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()
#endif

#define HDMI_CEC_LINE_PIN                 GPIO_PIN_10 /* PB.10 */
#define HDMI_CEC_LINE_GPIO_PORT           GPIOB
#define HDMI_CEC_LINE_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define HDMI_CEC_LINE_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()
#define HDMI_CEC_LINE_AF                  GPIO_AF0_CEC
#define HDMI_CEC_IRQn                     CEC_IRQn

/* HDMI-CEC hardware I2C address */
#define HDMI_CEC_I2C_ADDRESS              0xA0

/**
  * @}
  */


/** @defgroup STM32G081B_EVAL_DB STM32G081B_EVAL DAUGHTER BOARD
  * @{
  */
#if defined(HAL_ADC_MODULE_ENABLED)
/**
  * @brief Daughter Board detection pin
  */
#define DB_DETECT_PIN                   GPIO_PIN_11              /* PB.11 */
#define DB_DETECT_GPIO_PORT             GPIOB                    /* GPIOB */
#define DB_DETECT_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define DB_DETECT_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()

#define DB_ADC                          ADC1
#define DB_ADC_CLK_ENABLE()             __HAL_RCC_ADC_CLK_ENABLE();
#define DB_ADC_CLK_DISABLE()            __HAL_RCC_ADC_CLK_DISABLE();

#define DB_ADC_CHANNEL                  ADC_CHANNEL_15

#endif /* HAL_ADC_MODULE_ENABLED*/

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_Exported_Functions Exported Functions
  * @{
  */
uint32_t                BSP_GetVersion(void);
#if defined(_GUI_INTERFACE)
const uint8_t*          BSP_GetHWBoardVersionName(void);
const uint8_t*          BSP_GetPDTypeName(void);
#endif /* _GUI_INTERFACE */
void                    BSP_LED_Init(Led_TypeDef Led);
void                    BSP_LED_DeInit(Led_TypeDef Led);
void                    BSP_LED_On(Led_TypeDef Led);
void                    BSP_LED_Off(Led_TypeDef Led);
void                    BSP_LED_Toggle(Led_TypeDef Led);
void                    BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t                BSP_PB_GetState(Button_TypeDef Button);
uint8_t                 BSP_JOY_Init(JOYMode_TypeDef Joy_Mode);
void                    BSP_JOY_DeInit(void);
JOYState_TypeDef        BSP_JOY_GetState(void);
#if defined(HAL_UART_MODULE_ENABLED)
void                    BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef* huart);
#endif /* HAL_UART_MODULE_ENABLED */
#if defined(HAL_ADC_MODULE_ENABLED)
uint8_t BSP_DB_GetId(DBId_TypeDef *pDaughterBoardId);
#endif /* HAL_ADC_MODULE_ENABLED */
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

#endif /* STM32G081B_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
