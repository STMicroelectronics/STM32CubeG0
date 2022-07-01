/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_os_port_mx.h
  * @author  MCD Application Team
  * @brief   This file contains the core os portability macro definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef USBPD_CORE_OSPORT_H_
#define USBPD_CORE_OSPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#if (osCMSIS >= 0x20000U)
#include "task.h"
#endif /* osCMSIS >= 0x20000U */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CORE_OS
  * @{
  */

/** @addtogroup USBPD_CORE_OS_Macro
  * @{
  */
/* Exported define -----------------------------------------------------------*/
/**
  * @brief macro definition used to define the task function
  */
#if (osCMSIS < 0x20000U)
#define DEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(void const *argument)
#else
#define DEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(void *argument)
#endif /* (osCMSIS < 0x20000U)*/

/**
  * @brief macro definition used to initialize the OS environment
  */
#define OS_INIT()   USBPD_StatusTypeDef _retr = USBPD_OK;

/**
  * @brief macro definition the define a queue type
  */
#define OS_QUEUE_ID osMessageQId

/**
  * @brief macro definition the define a queue type
  */
#define OS_ELEMENT_SIZE sizeof(uint16_t)

/**
  * @brief macro definition used to define a queue
  */
#if (osCMSIS < 0x20000U)
#define OS_CREATE_QUEUE(_ID_,_NAME_,_ELT_,_ELTSIZE_)   do {                                                      \
                                                            osMessageQDef(queuetmp, (_ELT_), (_ELTSIZE_));       \
                                                            (_ID_) = osMessageCreate(osMessageQ(queuetmp), NULL);\
                                                            if((_ID_) == 0)                                      \
                                                            {                                                    \
                                                              _retr = USBPD_ERROR;                               \
                                                              goto error;                                        \
                                                            }                                                    \
                                                          } while(0)
#else
#define OS_CREATE_QUEUE(_ID_,_NAME_,_ELT_,_ELTSIZE_) do {                                                       \
                                                          (_ID_) = osMessageQueueNew((_ELT_),(_ELTSIZE_), NULL);\
                                                        }while(0)
#endif /* osCMSIS < 0x20000U */

/**
  * @brief macro definition used to read a queue message
  */
#if (osCMSIS < 0x20000U)
#define OS_GETMESSAGE_QUEUE(_ID_, _TIME_)  osMessageGet((_ID_),(_TIME_))
#else
#define OS_GETMESSAGE_QUEUE(_ID_, _TIME_)  do {                                                      \
                                                uint32_t event;                                      \
                                                (void)osMessageQueueGet((_ID_),&event,NULL,(_TIME_));\
                                              } while(0)
#endif /* (osCMSIS < 0x20000U) */

/**
  * @brief macro definition used to define put a message inside the queue
  */
#if (osCMSIS < 0x20000U)
#define OS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_)  do{                                                \
                                                        (void)osMessagePut((_ID_),(_MSG_),(_TIMEOUT_));\
                                                      }while(0)
#else
#define OS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_)  do {                                                         \
                                                         uint32_t event = (_MSG_);                               \
                                                         (void)osMessageQueuePut((_ID_), &event, 0U,(_TIMEOUT_));\
                                                       } while(0)
#endif /* osCMSIS < 0x20000U */

/**
  * @brief macro definition used to define a task
  */
#if (osCMSIS < 0x20000U)
#define OS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)
#else
#define OS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)
#endif /* osCMSIS < 0x20000U */

/**
  * @brief macro definition of the TASK id
  */
#define OS_TASK_ID   osThreadId

/**
  * @brief macro definition used to create a task
  */
#if (osCMSIS < 0x20000U)
#define OS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_) \
  do {                                                           \
    osThreadDef(_NAME_, _FUNC_, _PRIORITY_, 0, _STACK_SIZE_);    \
    (_ID_) = osThreadCreate(osThread(_NAME_), (void *)(_PARAM_));\
    if (NULL == (_ID_))                                          \
    {                                                            \
      _retr = USBPD_ERROR;                                       \
      goto error;                                                \
    }                                                            \
  } while(0)
#else
#define OS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_) \
  do {                                                 \
    osThreadAttr_t Thread_Atrr =                       \
    {                                                  \
      .name       = #_NAME_,                           \
      .priority   = (_PRIORITY_),                      \
      .stack_size = (_STACK_SIZE_)                     \
    };                                                 \
    (_ID_) = osThreadNew(_FUNC_, (void *)(_PARAM_),    \
                         &Thread_Atrr);                \
    if (NULL == (_ID_))                                \
    {                                                  \
      _retr = USBPD_ERROR;                             \
      goto error;                                      \
    }                                                  \
  } while(0)
#endif /* osCMSIS < 0x20000U */

/**
  * @brief macro definition used to check is task is suspended
  */
#define OS_TASK_IS_SUPENDED(_ID_) (eSuspended == eTaskGetState((_ID_)))

/**
  * @brief macro definition used to get the task ID
  */
#define OS_TASK_GETID()          osThreadGetId()

/**
  * @brief macro definition used to suspend a task
  */
#define OS_TASK_SUSPEND(_ID_)    osThreadSuspend(_ID_)

/**
  * @brief macro definition used to resume a task
  */
#define OS_TASK_RESUME(_ID_)     osThreadResume(_ID_)

/**
  * @brief macro definition used to manage the delay
  */
#define OS_DELAY(_TIME_)   osDelay(_TIME_)

/**
  * @brief macro definition used to start the task scheduling
  */
#if (osCMSIS >= 0x20000U)
#define OS_KERNEL_START()  do { (void)osKernelInitialize(); \
                                (void)osKernelStart();      \
                              } while(0)
#else
#define OS_KERNEL_START()  (void)osKernelStart()
#endif /* osCMSIS >= 0x20000U */

/* Exported types ------------------------------------------------------------*/
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
#endif /* USBPD_CORE_OSPORT_H_ */

