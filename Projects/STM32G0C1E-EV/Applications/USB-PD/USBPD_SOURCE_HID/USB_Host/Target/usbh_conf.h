/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/HID_Standalone/USB_Host/Target/usbh_conf.h
  * @author  : MCD Application Team
  * @brief   : General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_TRACE)
#include "usbpd_trace.h"
#endif /* _TRACE */

/** @defgroup USBH_CONF
  * @brief usb low level driver configuration file
  * @{
  */

/** @defgroup USBH_CONF_Exported_Variables USBH_CONF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#define USB_USER_DEBUG_TRACE_SIZE       100u
extern char _usb_str[USB_USER_DEBUG_TRACE_SIZE];
/* USER CODE END PV */
/**
  * @}
  */
   
/** @defgroup USBH_CONF_Exported_Defines
  * @{
  */

#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          1
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      2
#define USBH_USE_OS                           1

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup USBH_Exported_Macros
  * @{
  */

#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO          osPriorityNormal
  #define   USBH_PROCESS_STACK_SIZE    (8 * configMINIMAL_STACK_SIZE)
#endif

/* Memory management macros */
#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy



/* DEBUG macros */
#if (USBH_DEBUG_LEVEL > 0)                                                                   

#define USBH_UsrLog(...)  do {                                                                    \
      uint8_t _size = snprintf(_usb_str, USB_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                 \
      if (_size < USB_USER_DEBUG_TRACE_SIZE)                                                      \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, strlen(_usb_str));           \
      else                                                                                        \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, USB_USER_DEBUG_TRACE_SIZE);  \
  } while(0)

#else
#define USBH_UsrLog(...)
#endif

#if (USBH_DEBUG_LEVEL > 1)

#define  USBH_ErrLog(...)   do {                                                                    \
        uint8_t _size = snprintf(_usb_str, USB_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                 \
        if (_size < USB_USER_DEBUG_TRACE_SIZE)                                                      \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, strlen(_usb_str));           \
        else                                                                                        \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, USB_USER_DEBUG_TRACE_SIZE);  \
  } while(0)

#else
#define USBH_ErrLog(...)
#endif

#if (USBH_DEBUG_LEVEL > 2)
#define  USBH_DbgLog(...)   do {                                                                    \
        uint8_t _size = snprintf(_usb_str, USB_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                 \
        if (_size < USB_USER_DEBUG_TRACE_SIZE)                                                      \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, strlen(_usb_str));           \
        else                                                                                        \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0, 0, (uint8_t*)_usb_str, USB_USER_DEBUG_TRACE_SIZE);  \
  } while(0)

#else
#define USBH_DbgLog(...)
#endif

/**
  * @}
  */

/**
  * @}
  */


/** @defgroup USBH_CONF_Exported_Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/

/**
  * @}
  */


/** @defgroup USBH_CONF_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */


#endif  /* __USBH_CONF__H__ */


/**
  * @}
  */
