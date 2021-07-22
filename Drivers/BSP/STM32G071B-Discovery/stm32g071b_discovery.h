/**
  ******************************************************************************
  * @file    stm32g071b_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32G071B_DISCOVERY's LEDs,
  *          push-buttons hardware resources (MB1378).
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G071B_DISCOVERY_H
#define STM32G071B_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BSP BSP
  * @{
  */

/** @defgroup STM32G071B-DISCO STM32G071B-DISCO
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"

/** @defgroup STM32G071B_DISCOVERY_Common STM32G071B-DISCO Common
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_Private_Constants Private Constants
 * @{
 */
/**
* @}
*/

/** @defgroup STM32G071B_DISCOVERY_Exported_Types Exported Types
  * @{
  */

/**
 * @brief ALERT Types Definition
 */
typedef enum
{
  ALERT_VBUS = 0,
  ALERT_CC1 = 1,
  ALERT_CC2 = 2
}Alert_TypeDef;

/**
 * @brief LED Types Definition
 */
typedef enum
{
  LED4 = 0,
  LED5 = 1,
  LED6 = 2,
  LED7 = 3,
  LED_ORANGE  = LED4,
  LED_GREEN   = LED6,
}Led_TypeDef;

/**
 * @brief JOYSTICK Types Definition
 */
typedef enum
{
  JOY_SEL   = 0,
  JOY_LEFT  = 1,
  JOY_RIGHT = 2,
  JOY_DOWN  = 3,
  JOY_UP    = 4,
  JOY_NONE  = 5
}JOYState_TypeDef;

typedef enum
{
  JOY_MODE_GPIO = 0,
  JOY_MODE_EXTI = 1
}JOYMode_TypeDef;

/**
 * @brief DOOR SENSE Types Definition
 */
typedef enum
{
  DOOR_OPEN  = 0,
  DOOR_CLOSE = 1
}DOORState_TypeDef;

typedef enum
{
  DOOR_MODE_GPIO = 0,
  DOOR_MODE_EXTI = 1
}DOORMode_TypeDef;

/**
 * @brief MOSFET Types Definition
 */
typedef enum
{
  MOSFET_ENCC1 = 0,
  MOSFET_ENCC2,
  MOSFET_RD_CC1,
}Mosfet_TypeDef;

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup STM32G071B_DISCOVERY_ALERT ALERT Constants
  * @{
  */
#define ALERTn                            3

#define ALERT_VBUS_PIN                    GPIO_PIN_5
#define ALERT_VBUS_GPIO_PORT              GPIOC
#define ALERT_VBUS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define ALERT_VBUS_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define ALERT_VBUS_EXTI_IRQn              EXTI4_15_IRQn

#define ALERT_CC1_PIN                    GPIO_PIN_6
#define ALERT_CC1_GPIO_PORT              GPIOC
#define ALERT_CC1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define ALERT_CC1_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define ALERT_CC1_EXTI_IRQn              EXTI4_15_IRQn

#define ALERT_CC2_PIN                    GPIO_PIN_7
#define ALERT_CC2_GPIO_PORT              GPIOC
#define ALERT_CC2_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define ALERT_CC2_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define ALERT_CC2_EXTI_IRQn              EXTI4_15_IRQn

#define ALERTx_GPIO_CLK_ENABLE(__ALERT__)     do { if((__ALERT__) == ALERT_VBUS) { ALERT_VBUS_GPIO_CLK_ENABLE(); } else \
                                                   if((__ALERT__) == ALERT_CC1)  { ALERT_CC1_GPIO_CLK_ENABLE(); } else \
                                                   if((__ALERT__) == ALERT_CC2)  { ALERT_CC2_GPIO_CLK_ENABLE(); } } while(0)

#define ALERTx_GPIO_CLK_DISABLE(__ALERT__)    do { if((__ALERT__) == ALERT_VBUS) { ALERT_VBUS_GPIO_CLK_DISABLE(); } else \
                                                   if((__ALERT__) == ALERT_CC1)  { ALERT_CC1_GPIO_CLK_DISABLE();  } else \
                                                   if((__ALERT__) == ALERT_CC2)  { ALERT_CC2_GPIO_CLK_DISABLE(); } } while(0)

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_MOSFET MOSFET Constants
  * @{
  */
#define MOSFETn                            3

#define MOSFET_ENCC1_PIN                   GPIO_PIN_10
#define MOSFET_ENCC1_GPIO_PORT             GPIOB
#define MOSFET_ENCC1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define MOSFET_ENCC1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()

#define MOSFET_ENCC2_PIN                   GPIO_PIN_11
#define MOSFET_ENCC2_GPIO_PORT             GPIOB
#define MOSFET_ENCC2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define MOSFET_ENCC2_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()

#define MOSFET_RD_CC1_PIN                   GPIO_PIN_12
#define MOSFET_RD_CC1_GPIO_PORT             GPIOB
#define MOSFET_RD_CC1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define MOSFET_RD_CC1_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()

#define MOSFETx_GPIO_CLK_ENABLE(__MOSFET__)     do { if((__MOSFET__) == MOSFET_ENCC1)  { MOSFET_ENCC1_GPIO_CLK_ENABLE();  } else \
                                                     if((__MOSFET__) == MOSFET_ENCC2)  { MOSFET_ENCC2_GPIO_CLK_ENABLE();  } else \
                                                     if((__MOSFET__) == MOSFET_RD_CC1) { MOSFET_RD_CC1_GPIO_CLK_ENABLE(); } } while(0)

#define MOSFETx_GPIO_CLK_DISABLE(__MOSFET__)    do { if((__MOSFET__) == MOSFET_ENCC1)  { MOSFET_ENCC1_GPIO_CLK_DISABLE();  } else \
                                                     if((__MOSFET__) == MOSFET_ENCC2)  { MOSFET_ENCC2_GPIO_CLK_DISABLE();  } else \
                                                     if((__MOSFET__) == MOSFET_RD_CC1) { MOSFET_RD_CC1_GPIO_CLK_DISABLE(); } } while(0)

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_LED LED Constants
  * @{
  */
#define LEDn                              4

#define LED4_PIN                          GPIO_PIN_9
#define LED4_GPIO_PORT                    GPIOD
#define LED4_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED5_PIN                          GPIO_PIN_8
#define LED5_GPIO_PORT                    GPIOD
#define LED5_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED7_PIN                          GPIO_PIN_5
#define LED7_GPIO_PORT                    GPIOD
#define LED7_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED7_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED6_PIN                          GPIO_PIN_12
#define LED6_GPIO_PORT                    GPIOC
#define LED6_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)     do { if((__LED__) == LED4) { LED4_GPIO_CLK_ENABLE(); } else \
                                               if((__LED__) == LED5) { LED5_GPIO_CLK_ENABLE(); } else \
                                               if((__LED__) == LED6) { LED6_GPIO_CLK_ENABLE(); } else \
                                               if((__LED__) == LED7) { LED7_GPIO_CLK_ENABLE(); } } while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)    do { if((__LED__) == LED4) { LED4_GPIO_CLK_DISABLE(); } else \
                                               if((__LED__) == LED5) { LED5_GPIO_CLK_DISABLE(); } else \
                                               if((__LED__) == LED6) { LED6_GPIO_CLK_DISABLE(); } else \
                                               if((__LED__) == LED7) { LED7_GPIO_CLK_DISABLE(); } } while(0)

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_BUTTON  BUTTON Constants
  * @{
  */
#define JOYn                              5

/**
* @brief Joystick Right push-button
*/
#define RIGHT_JOY_PIN                     GPIO_PIN_3  /* PC.03 */
#define RIGHT_JOY_GPIO_PORT               GPIOC
#define RIGHT_JOY_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define RIGHT_JOY_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOC_CLK_DISABLE()
#define RIGHT_JOY_EXTI_IRQn               EXTI2_3_IRQn

/**
* @brief Joystick Left push-button
*/
#define LEFT_JOY_PIN                      GPIO_PIN_1  /* PC.01 */
#define LEFT_JOY_GPIO_PORT                GPIOC
#define LEFT_JOY_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define LEFT_JOY_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()
#define LEFT_JOY_EXTI_IRQn                EXTI0_1_IRQn

/**
* @brief Joystick Up push-button
*/
#define UP_JOY_PIN                        GPIO_PIN_4  /* PC.04 */
#define UP_JOY_GPIO_PORT                  GPIOC
#define UP_JOY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define UP_JOY_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOC_CLK_DISABLE()
#define UP_JOY_EXTI_IRQn                  EXTI4_15_IRQn

/**
 * @brief Joystick Down push-button
 */
#define DOWN_JOY_PIN                      GPIO_PIN_2   /* PC.02 */
#define DOWN_JOY_GPIO_PORT                GPIOC
#define DOWN_JOY_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define DOWN_JOY_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()
#define DOWN_JOY_EXTI_IRQn                EXTI2_3_IRQn

/**
 * @brief Joystick Sel push-button
 */
#define SEL_JOY_PIN                       GPIO_PIN_0   /* PC.00 */
#define SEL_JOY_GPIO_PORT                 GPIOC
#define SEL_JOY_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define SEL_JOY_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()
#define SEL_JOY_EXTI_IRQn                 EXTI0_1_IRQn

#define JOYx_GPIO_CLK_ENABLE(__JOY__)     do { if((__JOY__) == JOY_SEL)   { SEL_JOY_GPIO_CLK_ENABLE();   } else \
                                               if((__JOY__) == JOY_DOWN)  { DOWN_JOY_GPIO_CLK_ENABLE();  } else \
                                               if((__JOY__) == JOY_LEFT)  { LEFT_JOY_GPIO_CLK_ENABLE();  } else \
                                               if((__JOY__) == JOY_RIGHT) { RIGHT_JOY_GPIO_CLK_ENABLE(); } else \
                                               if((__JOY__) == JOY_UP)    { UP_JOY_GPIO_CLK_ENABLE(); }  } while(0)

#define JOYx_GPIO_CLK_DISABLE(__JOY__)    do { if((__JOY__) == JOY_SEL)   { SEL_JOY_GPIO_CLK_DISABLE();   } else \
                                               if((__JOY__) == JOY_DOWN)  { DOWN_JOY_GPIO_CLK_DISABLE();  } else \
                                               if((__JOY__) == JOY_LEFT)  { LEFT_JOY_GPIO_CLK_DISABLE();  } else \
                                               if((__JOY__) == JOY_RIGHT) { RIGHT_JOY_GPIO_CLK_DISABLE(); } else \
                                               if((__JOY__) == JOY_UP)    { UP_JOY_GPIO_CLK_DISABLE(); }  } while(0)

#define JOY_ALL_PINS                      (RIGHT_JOY_PIN | LEFT_JOY_PIN | UP_JOY_PIN | DOWN_JOY_PIN | SEL_JOY_PIN)

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_DOOR_SENSOR  DOOR SENSOR Constants
  * @{
  */
// GPIO in input with internal pull-up to read the state of the magnetic sensor
#define DOOR_SENSE_PIN                    GPIO_PIN_8
#define DOOR_SENSE_PORT                   GPIOC
#define DOOR_SENSE_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define DOOR_SENSE_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define DOOR_SENSE_EXTI_IRQn              EXTI4_15_IRQn
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_USBPD  USBPD Constants
  * @{
  */

/* STLink input signal
   Give the input 5V source availability From USB STLink */
#define STLK5V_ON_PIN                     GPIO_PIN_11
#define STLK5V_ON_PORT                    GPIOA
#define STLK5V_ON_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define STLK5V_ON_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

/* SMPS input signal
   Give the input 5V source availability From USB type C and SMPS */
#define SMPS5V_ON_PIN                     GPIO_PIN_12
#define SMPS5V_ON_PORT                    GPIOA
#define SMPS5V_ON_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SMPS5V_ON_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

/* SMPS Enable signal
   GPIO in output to enable the 5V SMPS (from USB type C) */
#define EN_SMPS_PIN                       GPIO_PIN_0
#define EN_SMPS_PORT                      GPIOA
#define EN_SMPS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define EN_SMPS_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

/* USBPD CC lines
   GPIO for USB CCx lines (USBPD1_CC1 USBPD1_CC2) */
#define USBPD1_CC1_PIN                    GPIO_PIN_8
#define USBPD1_CC1_PORT                   GPIOA
#define USBPD1_CC1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USBPD1_CC1_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()
#define USBPD1_CC2_PIN                    GPIO_PIN_15
#define USBPD1_CC2_PORT                   GPIOB
#define USBPD1_CC2_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USBPD1_CC2_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_BUS  BUS Constants
  * @{
  */
#if defined(HAL_SPI_MODULE_ENABLED)
/*##################### SPI1 ###################################*/
#define DISCOVERY_SPIx                          SPI1
#define DISCOVERY_SPIx_CLOCK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define DISCOVERY_SPIx_CLOCK_DISABLE()          __HAL_RCC_SPI1_CLK_DISABLE()
#define DISCOVERY_SPIx_GPIO_PORT                GPIOA                      /* GPIOA */
#define DISCOVERY_SPIx_AF                       GPIO_AF0_SPI1
#define DISCOVERY_SPIx_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DISCOVERY_SPIx_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()
#define DISCOVERY_SPIx_GPIO_FORCE_RESET()       __HAL_RCC_SPI1_FORCE_RESET()
#define DISCOVERY_SPIx_GPIO_RELEASE_RESET()     __HAL_RCC_SPI1_RELEASE_RESET()
#define DISCOVERY_SPIx_SCK_PIN                  GPIO_PIN_1                 /* PA.01*/
#define DISCOVERY_SPIx_MOSI_PIN                 GPIO_PIN_2                 /* PA.02 */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define SPIx_TIMEOUT_MAX                        ((uint32_t)0x1000)
/* Read/Write command */
#define READWRITE_CMD                           ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD                        ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                              ((uint8_t)0x00)

#endif /* HAL_SPI_MODULE_ENABLED */

#if defined(HAL_I2C_MODULE_ENABLED)
/*##################### I2C1 ###################################*/
/* User can use this section to tailor I2C1 instance used and associated
   resources */
/* Definition for I2C1 Pins */
#define DISCOVERY_I2C1                        I2C1
#define DISCOVERY_I2C1_CLK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define DISCOVERY_I2C1_CLK_DISABLE()          __HAL_RCC_I2C1_CLK_DISABLE()
#define DISCOVERY_I2C1_FORCE_RESET()          __HAL_RCC_I2C1_FORCE_RESET()
#define DISCOVERY_I2C1_RELEASE_RESET()        __HAL_RCC_I2C1_RELEASE_RESET()

#define DISCOVERY_I2C1_SCL_PIN                GPIO_PIN_6		/* PB.6 */
#define DISCOVERY_I2C1_SDA_PIN                GPIO_PIN_7		/* PB.7 */

#define DISCOVERY_I2C1_GPIO_PORT              GPIOB			/* GPIOB */
#define DISCOVERY_I2C1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define DISCOVERY_I2C1_SCL_SDA_AF             GPIO_AF6_I2C1

/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 56 MHz */
/* Set TIMING to 0x10806DA3 to reach 100 KHz speed (Rise time = 50ns, Fall time = 10ns) */
#define DISCOVERY_I2C1_TIMING_100Khz               0x10806DA3
#if 0
/* Set TIMING to 0x00801C61 to reach 400 KHz speed (Rise time = 50ns, Fall time = 10ns) */
#define DISCOVERY_I2C1_TIMING_400Khz               0x00801C61
#else
//JCC adding
// With 2.2K PU on the board, measured Rise_time=150ns, measured Fall_time=5ns
// So with these new input parameters, register timing value is updated to reach 400KHz.
#define DISCOVERY_I2C1_TIMING_400Khz               0x00D01C5C
// There is timing violation with 2.2K PU due to too high Rise_time !!!!
#define DISCOVERY_I2C1_TIMING_1Mhz                 0x0090091C
#endif

/* I2C clock speed configuration (in Hz)
   WARNING:
   Make sure that this define is not already declared in other files (ie.
   stm324xg_discovery.h file). It can be used in parallel by other modules. */
#ifndef BSP_I2C_SPEED
#define BSP_I2C_SPEED                              100000
#endif /* BSP_I2C_SPEED */


/* VBUS sensing I2C address */
#define VBUS_SENSING_I2C_ADDRESS                      ((uint16_t) 0x80)

/* CC1 sensing I2C address */
#define CC1_SENSING_I2C_ADDRESS                       ((uint16_t) 0x82)

/* CC2 sensing I2C address */
#define CC2_SENSING_I2C_ADDRESS                       ((uint16_t) 0x84)

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define DISCOVERY_I2C1_TIMEOUT_MAX              3000

#endif /* HAL_I2C_MODULE_ENABLED */
/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_COMPONENT COMPONENT Constants
  * @{
  */
/*##################### LCD ###################################*/
/**
  * @brief  LCD Chip Select macro definition
  */
#define LCD_CS_LOW()                    HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()                   HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Control pins
  */
#define LCD_CS_PIN                      GPIO_PIN_3		/* PA. 03*/
#define LCD_CS_GPIO_PORT                GPIOA			/* GPIOA */
#define LCD_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

/**
  * @brief  LCD Reset macro definition
  */
#define LCD_RST_LOW()                    HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()                   HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Reset pins
  */
#define LCD_RST_PIN                     GPIO_PIN_6		/* PA. 06*/
#define LCD_RST_GPIO_PORT               GPIOA			/* GPIOA */
#define LCD_RST_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_RST_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

/**
  * @brief  LCD Data/Command macro definition
  */
#define LCD_DC_LOW()                    HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()                   HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)

/**
  * @brief  LCD Reset pins
  */
#define LCD_DC_PIN                      GPIO_PIN_7		/* PA. 07*/
#define LCD_DC_GPIO_PORT                GPIOA    		/* GPIOA */
#define LCD_DC_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()


/*##################### PWRMON ###################################*/
/* Value (in mOhms) of the shunt resistor (external shunt used to develop the
 * differential voltage across the input pins).
 */
#define SHUNT_RESISTOR_VALUE 15 // 15 for MB1378 revB, 37 for MB1378 RevA.

/* Value (in mA) of the maximum expected current */
#define MAXIMUM_EXPECTED_CURRENT 3000

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32G071B_DISCOVERY_Exported_Functions Exported Functions
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
uint8_t                 BSP_JOY_Init(JOYMode_TypeDef Joy_Mode);
void                    BSP_JOY_DeInit(void);
JOYState_TypeDef        BSP_JOY_GetState(void);
uint8_t                 BSP_DOOR_Init(DOORMode_TypeDef Door_Mode);
void                    BSP_DOOR_DeInit(void);
DOORState_TypeDef       BSP_DOOR_GetState(void);
void                    BSP_MOSFET_Init(Mosfet_TypeDef Mosfet);
void                    BSP_MOSFET_DeInit(Mosfet_TypeDef Mosfet);
void                    BSP_MOSFET_On(Mosfet_TypeDef Mosfet);
void                    BSP_MOSFET_Off(Mosfet_TypeDef Mosfet);
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

#endif /* STM32G071B_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
