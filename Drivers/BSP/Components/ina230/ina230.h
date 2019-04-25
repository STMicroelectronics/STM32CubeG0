/**
  ******************************************************************************
  * @file    ina230.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the ina230.c 
  *          driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INA230_H
#define INA230_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/pwrmon.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup INA230
  * @{
  */
  
/** @defgroup INA230_Exported_Constants
  * @{
  */
/******************************* Register Map  ********************************/
#define INA230_REG_CONFIG        0x00  /*!< Configuration Register      */
#define INA230_REG_VSHUNT        0x01  /*!< Shunt Voltage Register      */
#define INA230_REG_VBUS          0x02  /*!< Bus Voltage Register        */
#define INA230_REG_PWR           0x03  /*!< Power Register              */
#define INA230_REG_CURRENT       0x04  /*!< Current Register            */
#define INA230_REG_CALIBRATION   0x05  /*!< Calibration Register        */
#define INA230_REG_MASK_ENABLE   0x06  /*!< Mask/Enable Register        */
#define INA230_REG_ALERT_LIMIT   0x07  /*!< Alert Limit Register        */
#define INA230_REG_ID            0xFF  /*!< Die identification register */

/*******************  Bit definition for Configuration register  **************/
#define INA230_REG_CFG_RESET_Pos        (15U)
#define INA230_REG_CFG_RESET_Msk        (0x1U << INA230_REG_CFG_RESET_Pos)      /*!< 0x8000 */
#define INA230_REG_CFG_RESET            INA230_REG_CFG_RESET_Msk                /*!< Reset Bit */
#define INA230_REG_CFG_AVG_Pos          (9U)
#define INA230_REG_CFG_AVG_Msk          (0x7U << INA230_REG_CFG_AVG_Pos)        /*!< 0x0E00 */
#define INA230_REG_CFG_AVG              INA230_REG_CFG_AVG_Msk                  /*!< Averaging Mode */
#define INA230_REG_CFG_AVG_0            (0x1U << INA230_REG_CFG_AVG_Pos)        /*!< 0x0200 */
#define INA230_REG_CFG_AVG_1            (0x2U << INA230_REG_CFG_AVG_Pos)        /*!< 0x0400 */
#define INA230_REG_CFG_AVG_2            (0x3U << INA230_REG_CFG_AVG_Pos)        /*!< 0x0800 */
#define INA230_REG_CFG_VBUSCT_Pos       (6U)
#define INA230_REG_CFG_VBUSCT_Msk       (0x7U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x01C0 */
#define INA230_REG_CFG_VBUSCT           INA230_REG_CFG_VBUSCT_Msk               /*!< Bus Voltage Conversion Time */
#define INA230_REG_CFG_VBUSCT_0         (0x1U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0040 */
#define INA230_REG_CFG_VBUSCT_1         (0x2U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0080 */
#define INA230_REG_CFG_VBUSCT_2         (0x4U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0100*/
#define INA230_REG_CFG_VSHUNTCT_Pos     (3U)
#define INA230_REG_CFG_VSHUNTCT_Msk     (0x7U << INA230_REG_CFG_VSHUNTCT_Pos)   /*!< 0x0068 */
#define INA230_REG_CFG_VSHUNTCT         INA230_REG_CFG_VSHUNTCT_Msk             /*!< Shunt Voltage Conversion Time */
#define INA230_REG_CFG_VSHUNTCT_0       (0x1U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0008 */
#define INA230_REG_CFG_VSHUNTCT_1       (0x2U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0020 */
#define INA230_REG_CFG_VSHUNTCT_2       (0x4U << INA230_REG_CFG_VBUSCT_Pos)     /*!< 0x0040 */
#define INA230_REG_CFG_MODE_Pos         (0U)
#define INA230_REG_CFG_MODE_Msk         (0x7U << INA230_REG_CFG_MODE_Pos)       /*!< 0x0007 */
#define INA230_REG_CFG_MODE             INA230_REG_CFG_MODE_Msk                 /*!< Operating Mode */
#define INA230_REG_CFG_MODE_0           (0x1U << INA230_REG_CFG_MODE_Pos)       /*!< 0x0001 */
#define INA230_REG_CFG_MODE_1           (0x2U << INA230_REG_CFG_MODE_Pos)       /*!< 0x0002 */
#define INA230_REG_CFG_MODE_2           (0x4U << INA230_REG_CFG_MODE_Pos)       /*!< 0x0004 */

/******************  Bit definition for Shunt Voltage register  ***************/
#define INA230_REG_VSHUNT_DATA_Pos      (0U)
#define INA230_REG_VSHUNT_DATA_Msk      (0x7FFFU << INA230_REG_VSHUNT_DATA_Pos) /*!< 0x7FFF */
#define INA230_REG_VSHUNT_DATA          INA230_REG_VSHUNT_DATA_Msk              /*!< Measured shunt voltage */
#define INA230_REG_VSHUNT_SIGN_Pos      (15U)
#define INA230_REG_VSHUNT_SIGN_Msk      (0x1U << INA230_REG_VSHUNT_SIGN_Pos)    /*!< 0x8000 */
#define INA230_REG_VSHUNT_SIGN          INA230_REG_VSHUNT_SIGN_Msk              /*!< Sign */

/*******************  Bit definition for Bus Voltage register  ****************/
#define INA230_REG_VBUS_DATA_Pos        (0U)
#define INA230_REG_VBUS_DATA_Msk        (0x7FFFU << INA230_REG_VBUS_DATA_Pos)   /*!< 0x7FFF */
#define INA230_REG_VBUS_DATA            INA230_REG_VBUS_DATA_Msk                /*!< Measured bus voltage */

/*********************  Bit definition for Power register  ********************/
#define INA230_REG_PWR_DATA_Pos         (0U)
#define INA230_REG_PWR_DATA_Msk         (0xFFFFU << INA230_REG_PWR_DATA_Pos)    /*!< 0xFFFF */
#define INA230_REG_PWR_DATA             INA230_REG_PWR_DATA_Msk                 /*!< Measured power */

/*****************  Bit definition for Current register  **********************/
#define INA230_REG_CURRENT_DATA_Pos     (0U)
#define INA230_REG_CURRENT_DATA_Msk     (0x7FFFU << INA230_REG_CURRENT_DATA_Pos) /*!< 0x7FFF */
#define INA230_REG_CURRENT_DATA         INA230_REG_CURRENT_DATA_Msk              /*!< Measured current */
#define INA230_REG_CURRENT_SIGN_Pos     (15U)
#define INA230_REG_CURRENT_SIGN_Msk     (0x1U << INA230_REG_CURRENT_SIGN_Pos)   /*!< 0x8000 */
#define INA230_REG_CURRENT_SIGN         INA230_REG_CURRENT_SIGN_Msk             /*!< Sign */

/*******************  Bit definition for Calibration register  ****************/
#define INA230_REG_CALIBRATION_DATA_Pos (0U)
#define INA230_REG_CALIBRATION_DATA_Msk (0x7FFFU << INA230_REG_CALIBRATION_DATA_Pos) /*!< 0x7FFF */
#define INA230_REG_CALIBRATION_DATA     INA230_REG_CALIBRATION_DATA_Msk              /*!< Measured current */

/*******************  Bit definition for Mask/Enable register  ****************/
#define INA230_REG_MASK_ENABLE_SOL_Pos  (15U)
#define INA230_REG_MASK_ENABLE_SOL_Msk  (0x1U << INA230_REG_MASK_ENABLE_SOL_Pos)  /*!< 0x8000 */
#define INA230_REG_MASK_ENABLE_SOL      INA230_REG_MASK_ENABLE_SOL_Msk            /*!< Shunt Voltage Over-Voltage */
#define INA230_REG_MASK_ENABLE_SUL_Pos  (14U)
#define INA230_REG_MASK_ENABLE_SUL_Msk  (0x1U << INA230_REG_MASK_ENABLE_SUL_Pos)  /*!< 0x4000 */
#define INA230_REG_MASK_ENABLE_SUL      INA230_REG_MASK_ENABLE_SUL_Msk            /*!< Shunt Voltage Under-Voltage */
#define INA230_REG_MASK_ENABLE_BOL_Pos  (13U)
#define INA230_REG_MASK_ENABLE_BOL_Msk  (0x1U << INA230_REG_MASK_ENABLE_BOL_Pos)  /*!< 0x2000 */
#define INA230_REG_MASK_ENABLE_BOL      INA230_REG_MASK_ENABLE_BOL_Msk            /*!< Bus Voltage Over-Voltage */
#define INA230_REG_MASK_ENABLE_BUL_Pos  (12U)
#define INA230_REG_MASK_ENABLE_BUL_Msk  (0x1U << INA230_REG_MASK_ENABLE_BUL_Pos)  /*!< 0x1000 */
#define INA230_REG_MASK_ENABLE_BUL      INA230_REG_MASK_ENABLE_BUL_Msk            /*!< Bus Voltage Under-Voltage */
#define INA230_REG_MASK_ENABLE_POL_Pos  (11U)
#define INA230_REG_MASK_ENABLE_POL_Msk  (0x1U << INA230_REG_MASK_ENABLE_POL_Pos)  /*!< 0x0800 */
#define INA230_REG_MASK_ENABLE_POL      INA230_REG_MASK_ENABLE_POL_Msk            /*!< Over-Limit Power */
#define INA230_REG_MASK_ENABLE_CNVR_Pos (10U)
#define INA230_REG_MASK_ENABLE_CNVR_Msk (0x1U << INA230_REG_MASK_ENABLE_CNVR_Pos) /*!< 0x0400 */
#define INA230_REG_MASK_ENABLE_CNVR     INA230_REG_MASK_ENABLE_CNVR_Msk           /*!< Conversion Ready */
#define INA230_REG_MASK_ENABLE_AFF_Pos  (4U)
#define INA230_REG_MASK_ENABLE_AFF_Msk  (0x1U << INA230_REG_MASK_ENABLE_AFF_Pos)  /*!< 0x0001 */
#define INA230_REG_MASK_ENABLE_AFF      INA230_REG_MASK_ENABLE_AFF_Msk            /*!< Alert Function Flag */
#define INA230_REG_MASK_ENABLE_CVRF_Pos (3U)
#define INA230_REG_MASK_ENABLE_CVRF_Msk (0x1U << INA230_REG_MASK_ENABLE_CVRF_Pos) /*!< 0x0008 */
#define INA230_REG_MASK_ENABLE_CVRF     INA230_REG_MASK_ENABLE_CVRF_Msk           /*!< Conversion Ready Flag */
#define INA230_REG_MASK_ENABLE_OVF_Pos  (2U)
#define INA230_REG_MASK_ENABLE_OVF_Msk  (0x1U << INA230_REG_MASK_ENABLE_OVF_Pos)  /*!< 0x0004 */
#define INA230_REG_MASK_ENABLE_OVF      INA230_REG_MASK_ENABLE_OVF_Msk            /*!< Math Overflow Flag */
#define INA230_REG_MASK_ENABLE_APOL_Pos (1U)
#define INA230_REG_MASK_ENABLE_APOL_Msk (0x1U << INA230_REG_MASK_ENABLE_APOL_Pos) /*!< 0x0002 */
#define INA230_REG_MASK_ENABLE_APOL     INA230_REG_MASK_ENABLE_APOL_Msk           /*!< Alert Polarity */
#define INA230_REG_MASK_ENABLE_LEN_Pos  (0U)
#define INA230_REG_MASK_ENABLE_LEN_Msk  (0x1U << INA230_REG_MASK_ENABLE_LEN_Pos)  /*!< 0x0001 */
#define INA230_REG_MASK_ENABLE_LEN      INA230_REG_MASK_ENABLE_LEN_Msk            /*!< Alert Latch Enable */
  
/*******************  Bit definition for Alert Limit register  ****************/
#define INA230_REG_ALERT_LIMIT_DATA_Pos (0U)
#define INA230_REG_ALERT_LIMIT_DATA_Msk (0xFFFFU << INA230_REG_ALERT_LIMIT_DATA_Pos) /*!< 0xFFFF */
#define INA230_REG_ALERT_LIMIT_DATA     INA230_REG_ALERT_LIMIT_DATA_Msk              /*!< Alert Limit */

   
/** @defgroup INA230_MODE  INA230 operating mode
  * @{
  */
#define INA230_MODE_POWER_DOWN       (uint16_t)0x0000U                                 /*!< Turn off current into the INA230 inputs               */
#define INA230_MODE_TRIGGERED_VSHUNT INA230_REG_CFG_MODE_0                             /*!< Shunt Voltage, Single shot conversion                 */
#define INA230_MODE_TRIGGERED_VBUS   INA230_REG_CFG_MODE_1                             /*!< Bus voltage, Single shot conversion                   */
#define INA230_MODE_TRIGGERED_ALL    (INA230_REG_CFG_MODE_1 | INA230_REG_CFG_MODE_0)   /*!< Shunt Voltage and Bus voltage, Single shot conversion */
#define INA230_MODE_CONTINOUS_VSHUNT (INA230_REG_CFG_MODE_2 | INA230_REG_CFG_MODE_0)   /*!< Shunt Voltage, Continuous conversion                  */
#define INA230_MODE_CONTINOUS_VBUS   (INA230_REG_CFG_MODE_2 | INA230_REG_CFG_MODE_1)   /*!< Bus voltage, Continuous conversion                    */
#define INA230_MODE_CONTINOUS_ALL    INA230_REG_CFG_MODE                               /*!< Shunt Voltage and Bus voltage, Continuous conversion  */
/**
  * @}
  */

/** @defgroup INA230_CT INA230 ADC conversion time
  * @{
  */
#define INA230_CT_140     (uint16_t)0x0000U                                       /*!< ADC conversion time: 140 us  */
#define INA230_CT_204     INA230_REG_CFG_VSHUNTCT_0                               /*!< ADC conversion time: 204 us  */
#define INA230_CT_332     INA230_REG_CFG_VSHUNTCT_1                               /*!< ADC conversion time: 332 us  */
#define INA230_CT_588     (INA230_REG_CFG_VSHUNTCT_1 | INA230_REG_CFG_VSHUNTCT_0) /*!< ADC conversion time: 588 us  */
#define INA230_CT_1100    INA230_REG_CFG_VSHUNTCT_2                               /*!< ADC conversion time: 1100 us */
#define INA230_CT_2116    (INA230_REG_CFG_VSHUNTCT_2| INA230_REG_CFG_VSHUNTCT_0)  /*!< ADC conversion time: 2116 us */
#define INA230_CT_4156    (INA230_REG_CFG_VSHUNTCT_2| INA230_REG_CFG_VSHUNTCT_1)  /*!< ADC conversion time: 4156 us */
#define INA230_CT_8244    INA230_REG_CFG_VSHUNTCT                                 /*!< ADC conversion time: 8244 us */
/**
  * @}
  */

/** @defgroup INA230_AVG INA230 number of averages
  * @{
  */
#define INA230_AVG_1      (uint16_t)0x0000U                                       /*!< Number of averages: 1    */
#define INA230_AVG_4      INA230_REG_AVG_VSHUNTCT_0                               /*!< Number of averages: 4    */
#define INA230_AVG_16     INA230_REG_AVG_VSHUNTCT_1                               /*!< Number of averages: 16   */
#define INA230_AVG_64     (INA230_REG_AVG_VSHUNTCT_1 | INA230_REG_AVG_VSHUNTCT_0) /*!< Number of averages: 64   */
#define INA230_AVG_128    INA230_REG_AVG_VSHUNTCT_2                               /*!< Number of averages: 128  */
#define INA230_AVG_256    (INA230_REG_AVG_VSHUNTCT_2| INA230_REG_AVG_VSHUNTCT_0)  /*!< Number of averages: 256  */
#define INA230_AVG_512    (INA230_REG_AVG_VSHUNTCT_2| INA230_REG_AVG_VSHUNTCT_1)  /*!< Number of averages: 512  */
#define INA230_AVG_1024   INA230_REG_AVG_VSHUNTCT                                 /*!< Number of averages: 1024 */
/**
  * @}
  */

/** @defgroup INA230_FLAG INA230 flags 
  * @{
  */
#define INA230_AFF        INA230_REG_MASK_ENABLE_AFF         /*!< Alert Function Flag   */
#define INA230_CVRF       INA230_REG_MASK_ENABLE_CVRF        /*!< Conversion Ready Flag */
#define INA230_OVRF       INA230_REG_MASK_ENABLE_OVF         /*!< Math Overflow Flag    */
/**
  * @}
  */

/** @defgroup INA230_ALERTFUNC INA230 alert functions 
  * @{
  */
#define INA230_ALERTFUNC_SOL       INA230_REG_MASK_ENABLE_SOL         /*!< Shunt Voltage Over Limit  */
#define INA230_ALERTFUNC_SUL       INA230_REG_MASK_ENABLE_SUL         /*!< Shunt Voltage Under Limit */
#define INA230_ALERTFUNC_BOL       INA230_REG_MASK_ENABLE_BOL         /*!< Bus Voltage Over Limit    */
#define INA230_ALERTFUNC_BUL       INA230_REG_MASK_ENABLE_BUL         /*!< Bus Voltage Under Limit   */
#define INA230_ALERTFUNC_POL       INA230_REG_MASK_ENABLE_POL         /*!< Power Over Limit          */
/**
  * @}
  */
    
/**
  * @}
  */
  
#if defined(INA230_DEBUG)
/** @defgroup INA230_Exported_Structure INA230 Exported Structure
  * @{
  */
#if defined ( __CC_ARM )
#pragma anon_unions
#endif

typedef union {
  uint16_t Register;
  struct {
    uint16_t MODE     :3;
    uint16_t VSHUNTCT :3;
    uint16_t VBUSCT   :3;
    uint16_t AVG      :3;
    uint16_t RESERVED :3;
    uint16_t RST      :1;
  };
} INA230_Config_t;

typedef union {
  uint16_t Register;
  struct {
    uint16_t LEN      :1;
    uint16_t APOL     :1;
    uint16_t OVF      :1;
    uint16_t CVRF     :1;
    uint16_t AFF      :1;
    uint16_t RESERVED :5;
    uint16_t CNVR     :1;
    uint16_t POL      :1;
    uint16_t BUL      :1;
    uint16_t BOL      :1;
    uint16_t SUL      :1;
    uint16_t SOL      :1;
  };
} INA230_MaskEn_t;

typedef struct
{
  INA230_Config_t    Config;      /*!< Configuration register (0x00)    */
  uint16_t           VShunt;      /*!< Shunt voltage Register (0x01)    */
  uint16_t           VBus;        /*!< Bus voltage register   (0x02)    */
  uint16_t           Power;       /*!< Power Register         (0x03)    */
  uint16_t           Current;     /*!< Current Register       (0x04)    */
  uint16_t           Calibration; /*!< Calibration Registers  (0x05)    */
  INA230_MaskEn_t    MaskEnable;  /*!< Mask/Enable Register  (0x06)     */
  uint16_t           AlertLimit;  /*!< Alert Limit Register  (0x07)     */
  uint16_t           DieId;       /*!< Die ID Registers      (0xFF)     */
} INA230_Registers_t;

/**
  * @}
  */
#endif /* INA230_DEBUG */
  
/** @defgroup INA230_Exported_Functions
  * @{
  */
void                        ina230_Init(uint16_t Address, PWRMON_Config_t * pConfig);
void                        ina230_DeInit(uint16_t Address);
uint16_t                    ina230_ReadId(uint16_t Address);
void                        ina230_Reset(uint16_t Address);
void                        ina230_SetCalibration(uint16_t Address, uint16_t Calibration);
uint16_t                    ina230_GetCalibration(uint16_t);
void                        ina230_SetAlertFunction(uint16_t Address, PWRMON_AlertFunction_t AlertFunction);
PWRMON_AlertFunction_t      ina230_GetAlertFunction(uint16_t Address);
void                        ina230_AlertPinConfig(uint16_t Address, PWRMON_AlertPinConfig_t * pAlertPinConfig);
void                        ina230_SetVBusThreshold(uint16_t Address, uint16_t VBusThreshold);
uint16_t                    ina230_GetVBusThreshold(uint16_t Address);
void                        ina230_SetVShuntThreshold(uint16_t Address, int16_t VShuntThreshold);
int16_t                     ina230_GetVShuntThreshold(uint16_t Address);
void                        ina230_SetPowerThreshold(uint16_t Address, uint32_t PowerThreshold);
uint32_t                    ina230_GetPowerThreshold(uint16_t Address);
void                        ina230_AlertThresholdEnableIT(uint16_t Address);
void                        ina230_AlertThresholdDisableIT(uint16_t Address);
void                        ina230_ConversionReadyEnableIT(uint16_t Address);
void                        ina230_ConversionReadyDisableIT(uint16_t Adress);
void                        ina230_StartConversion(uint16_t Address, PWRMON_InputSignal_t InputSignal, PWRMON_OperatingMode_t  Mode);
void                        ina230_StopConversion(uint16_t Address);
uint16_t                    ina230_GetVBus(uint16_t Address);
int16_t                     ina230_GetVShunt(uint16_t Address);
uint16_t                    ina230_GetPower(uint16_t Address);
int16_t                     ina230_GetCurrent(uint16_t Address);
uint8_t                     ina230_GetFlag(uint16_t Address, PWRMON_Flag_t Flag);
/**
  * @}
  */

/** @defgroup INA230_Exported_Variables
  * @{
  */
/* Power Monitor driver structure */
extern PWRMON_Drv_t ina230_drv;

/* PWRMON IO functions */
uint8_t  PWRMON_IO_Init(uint16_t Addr);
void     PWRMON_IO_DeInit(uint16_t Addr);
uint8_t  PWRMON_IO_Write(uint16_t Addr, uint8_t Reg, uint8_t * pData);
uint8_t  PWRMON_IO_Read(uint16_t Addr, uint8_t Reg, uint8_t * pData);
void     PWRMON_IO_EnableIT(uint16_t Addr, uint8_t ActiveEdge);
void     PWRMON_IO_DisableIT(uint16_t Addr);
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
  
#endif /* INA230_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
