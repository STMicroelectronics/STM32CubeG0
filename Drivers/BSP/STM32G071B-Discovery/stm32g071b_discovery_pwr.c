/**
  ******************************************************************************
  * @file    stm32g071b_discovery_pwr.c
  * @author  MCD Application Team
  * @brief   This file contains pwr control functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g071b_discovery_pwr.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_exti.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G071B-DISCO
  * @{
  */

/** @addtogroup STM32G071B_DISCOVERY_POWER
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup STM32G071B_DISCOVERY_POWER_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize VBUS power
  * @param  PortNum current port number
  * @retval None
  */
void BSP_PWR_VBUSInit(uint8_t PortNum)
{
  PWRMON_Config_t DefaultConfig =
  {
    .ShuntConvertTime  = CONVERT_TIME_1100,
    .BusConvertTime    = CONVERT_TIME_1100,
    .AveragingMode     = AVERAGING_MODE_1,
  };
  BSP_PWRMON_Init(ALERT_VBUS, &DefaultConfig);
  BSP_PWRMON_StartMeasure(ALERT_VBUS, OPERATING_MODE_CONTINUOUS);
}

/**
  * @brief  DeInitialize VBUS power
  * @param  PortNum current port number
  * @retval None
  */
void BSP_PWR_VBUSDeInit(uint8_t PortNum)
{
  BSP_PWRMON_DeInit(ALERT_VBUS);
}

/**
  * @brief  Get VBUS power voltage
  * @param  PortNum current port number
  * @retval VBUS voltage
  */
uint32_t BSP_PWR_VBUSGetVoltage(uint8_t PortNum)
{
  uint32_t val=0;

  BSP_PWRMON_GetVoltage(ALERT_VBUS, &val);

  return val;
}

/**
  * @brief  Get Current value
  * @param  PortNum     current port number
  * @retval VBUS Current
  */
int32_t BSP_PWR_VBUSGetCurrent(uint8_t PortNum)
{
  int32_t  ret = 0;

  BSP_PWRMON_GetCurrent(ALERT_VBUS, &ret);

  return ret;
}


/**
  * @brief  Captive cable configuration
  * @param  PortNum     current port number
  * @retval None.
  */
void BSP_PWR_CaptiveCablePreConfig(uint8_t PortNum)
{
  /* Patch SMPS to avoid over current test 4.10.1    */
  /* disable SMPS put output 0 for SMPS in opendrain */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
  
  if(LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11) == 1) //&& (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12) == 0)) /* PA11 = STLNK_ON, PA12 = SMPS_ON */
  {
    /* disable SMPS */
//    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
//    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
//    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
  }
    
  /* Remove the EN1 enable CC1 CC2 */
  BSP_MOSFET_Init(MOSFET_ENCC1);
  BSP_MOSFET_Init(MOSFET_ENCC2);
  BSP_MOSFET_On(MOSFET_ENCC1);
  BSP_MOSFET_Off(MOSFET_ENCC2);
  
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* set Programmable Voltage Detector (PVD)  */
//  LL_PWR_SetPVDLowLevel(LL_PWR_PVDLLEVEL_6);
//  LL_PWR_SetPVDHighLevel(LL_PWR_PVDHLEVEL_6);
//  LL_PWR_EnablePVD();
//  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_16);
//  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_16);
//  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_16);  
//  
//    HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(PVD_IRQn);
}


/**
  * @brief  Captive cable configuration
  * @param  PortNum     current port number
  * @retval None.
  */
void BSP_PWR_CaptiveCablePostConfig(uint8_t PortNum)
{
  BSP_MOSFET_Init(MOSFET_RD_CC1);
  BSP_MOSFET_Off(MOSFET_RD_CC1);
}

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

