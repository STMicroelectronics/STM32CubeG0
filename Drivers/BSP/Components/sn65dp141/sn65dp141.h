/**
  ******************************************************************************
  * @file    sn65dp141.h
  * @author  MCD Application Team
  * @brief   Header for sn65dp141.h module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef SN65DP141_H
#define SN65DP141_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../Common/dpredriver.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup SN65DP141
  * @{
  */

/** @defgroup SN65DP141_Exported_Constants
  * @{
  */
/******************************* Register Map  ********************************/
#define SN65DP141_REG_CFG      0x00  /*!< General Device Settings register    */
#define SN65DP141_REG_CHEN     0x01  /*!< Channel Enable register             */
#define SN65DP141_REG_CH0_CFG  0x02  /*!< Channel 0 Control Settings register */
#define SN65DP141_REG_CH0_EN   0x03  /*!< Channel 0 Enable Settings register  */
#define SN65DP141_REG_CH1_CFG  0x05  /*!< Channel 1 Control Settings register */
#define SN65DP141_REG_CH1_EN   0x06  /*!< Channel 1 Enable Settings register  */
#define SN65DP141_REG_CH2_CFG  0x08  /*!< Channel 2 Control Settings register */
#define SN65DP141_REG_CH2_EN   0x09  /*!< Channel 2 Enable Settings register  */
#define SN65DP141_REG_CH3_CFG  0x0B  /*!< Channel 3 Control Settings register */
#define SN65DP141_REG_CH3_EN   0x0C  /*!< Channel 3 Enable Settings register  */

/***********  Bit definition for General Device Settings register  *************/
#define SN65DP141_REG_CFG_EQ_MODE_Pos         (1U)
#define SN65DP141_REG_CFG_EQ_MODE_Msk         (0x1U << SN65DP141_REG_CFG_EQ_MODE_Pos)              /*!< 0x04*/
#define SN65DP141_REG_CFG_EQ_MODE             SN65DP141_REG_CFG_EQ_MODE_Msk                        /*!< EQ mode (Cable mode v.s. Trace mode) */
#define SN65DP141_REG_CFG_SYNC_ALL_Pos        (3U)
#define SN65DP141_REG_CFG_SYNC_ALL_Msk        (0x1U << SN65DP141_REG_CFG_SYNC_ALL_Pos)             /*!< 0x08*/
#define SN65DP141_REG_CFG_SYNC_ALL             SN65DP141_REG_CFG_SYNC_ALL_Msk                      /*!< All settings from channel 1 will be used on all channels */
#define SN65DP141_REG_CFG_SYNC_23_Pos         (4U)
#define SN65DP141_REG_CFG_SYNC_23_Msk         (0x1U << SN65DP141_REG_CFG_SYNC23_Pos)               /*!< 0x10*/
#define SN65DP141_REG_CFG_SYNC_23             SN65DP141_REG_CFG_SYNC_23_Msk                        /*!< All settings from channel 2 will be used for channel 2 and 3 */
#define SN65DP141_REG_CFG_SYNC_01_Pos         (5U)
#define SN65DP141_REG_CFG_SYNC_01_Msk         (0x1U << SN65DP141_REG_CFG_SYNC01_Pos)               /*!< 0x20*/
#define SN65DP141_REG_CFG_SYNC_01             SN65DP141_REG_CFG_SYNC_01_Msk                        /*!< All settings from channel 1 will be used for channel 0 and 1 */
#define SN65DP141_REG_CFG_PWRDOWN_Pos         (6U)
#define SN65DP141_REG_CFG_PWRDOWN_Msk         (0x1U << SN65DP141_REG_CFG_PWRDOWN_Pos)              /*!< 0x40*/
#define SN65DP141_REG_CFG_PWRDOWN             SN65DP141_REG_CFG_PWRDOWN_Msk                        /*!< Power down the device */
#define SN65DP141_REG_CFG_SW_GPIO_Pos         (7U)
#define SN65DP141_REG_CFG_SW_GPIO_Msk         (0x1U << SN65DP141_REG_CFG_SW_GPIO_Pos)              /*!< 0x80*/
#define SN65DP141_REG_CFG_SW_GPIO             SN65DP141_REG_CFG_SW_GPIO_Msk                        /*!< Switching logic is controlled by GPIO or I2C */

/***********  Bit definition for Channel Enable register  *************/
#define SN65DP141_REG_CHEN_LN_EN_CH0_Pos      (0U)
#define SN65DP141_REG_CHEN_LN_EN_CH0_Msk      (0x1U << SN65DP141_REG_CHEN_LN_EN_CH0_Pos)        /*!< 0x01 */
#define SN65DP141_REG_CHEN_LN_EN_CH0          SN65DP141_REG_CHEN_LN_EN_CH0_Msk                  /*!< Channel 0 enable */
#define SN65DP141_REG_CHEN_LN_EN_CH1_Pos      (1U)
#define SN65DP141_REG_CHEN_LN_EN_CH1_Msk      (0x1U << SN65DP141_REG_CHEN_LN_EN_CH1_Pos)        /*!< 0x02 */
#define SN65DP141_REG_CHEN_LN_EN_CH1          SN65DP141_REG_CHEN_LN_EN_CH1_Msk                  /*!< Channel 1 enable */
#define SN65DP141_REG_CHEN_LN_EN_CH2_Pos      (2U)
#define SN65DP141_REG_CHEN_LN_EN_CH2_Msk      (0x1U << SN65DP141_REG_CHEN_LN_EN_CH2_Pos)        /*!< 0x04 */
#define SN65DP141_REG_CHEN_LN_EN_CH2          SN65DP141_REG_CHEN_LN_EN_CH2_Msk                  /*!< Channel 2 enable */
#define SN65DP141_REG_CHEN_LN_EN_CH3_Pos      (3U)
#define SN65DP141_REG_CHEN_LN_EN_CH3_Msk      (0x1U << SN65DP141_REG_CHEN_LN_EN_CH3_Pos)        /*!< 0x08 */
#define SN65DP141_REG_CHEN_LN_EN_CH3          SN65DP141_REG_CHEN_LN_EN_CH3_Msk                  /*!< Channel 3 enable */

/***********  Bit definition for Channel x Control Settings register  *************/
#define SN65DP141_REG_CHxCFG_RX_GAIN_Pos      (0U)
#define SN65DP141_REG_CHxCFG_RX_GAIN_Msk      (0x3U << SN65DP141_REG_CHxCFG_RX_GAIN_Pos)           /*!< 0x03 */
#define SN65DP141_REG_CHxCFG_RX_GAIN          SN65DP141_REG_CHxCFG_RX_GAIN_Msk                     /*!< Channel[x] RX_GAIN control */
#define SN65DP141_REG_CHxCFG_RX_GAIN_0        (0x1U << SN65DP141_REG_CHxCFG_RX_GAIN_Pos)           /*!< 0x01 */
#define SN65DP141_REG_CHxCFG_RX_GAIN_1        (0x2U << SN65DP141_REG_CHxCFG_RX_GAIN_Pos)           /*!< 0x02 */
#define SN65DP141_REG_CHxCFG_EQ_DC_GAIN_Pos   (2U)
#define SN65DP141_REG_CHxCFG_EQ_DC_GAIN_Msk   (0x1U << SN65DP141_REG_CHxCFG_EQ_DC_GAIN_Pos)        /*!< 0x04 */
#define SN65DP141_REG_CHxCFG_EQ_DC_GAIN       SN65DP141_REG_CHxCFG_EQ_DC_GAIN_Msk                  /*!< Channel[x] EQ DC gain */
#define SN65DP141_REG_CHxCFG_TX_GAIN_Pos      (3U)
#define SN65DP141_REG_CHxCFG_TX_GAIN_Msk      (0x1U << SN65DP141_REG_CHxCFG_TX_GAIN_Pos)           /*!< 0x08 */
#define SN65DP141_REG_CHxCFG_TX_GAIN          SN65DP141_REG_CHxCFG_TX_GAIN_Msk                     /*!< Channel[x] TX_DC_GAIN control */
#define SN65DP141_REG_CHxCFG_EQ_SETTING_Pos   (4U)
#define SN65DP141_REG_CHxCFG_EQ_SETTING_Msk   (0x7U << SN65DP141_REG_CHxCFG_EQ_SETTING_Pos)        /*!< 0x70 */
#define SN65DP141_REG_CHxCFG_EQ_SETTING       SN65DP141_REG_CHxCFG_EQ_SETTING_Msk                  /*!< Channel[x] TX_DC_GAIN control */
#define SN65DP141_REG_CHxCFG_EQ_SETTING_0     (0x1U << SN65DP141_REG_CHxCFG_EQ_SETTING_Pos)        /*!< 0x10 */
#define SN65DP141_REG_CHxCFG_EQ_SETTING_1     (0x2U << SN65DP141_REG_CHxCFG_EQ_SETTING_Pos)        /*!< 0x20 */
#define SN65DP141_REG_CHxCFG_EQ_SETTING_2     (0x4U << SN65DP141_REG_CHxCFG_EQ_SETTING_Pos)        /*!< 0x40 */

/***********  Bit definition for Channel x Enable Settings register  *************/
#define SN65DP141_REG_CHxCTL_DRV_EN_Pos       (0U)
#define SN65DP141_REG_CHxCTL_DRV_EN_Msk       (0x1U << SN65DP141_REG_CHxCTL_DRV_EN_Pos)              /*!< 0x01 */
#define SN65DP141_REG_CHxCTL_DRV_EN           SN65DP141_REG_CHxCTL_DRV_EN_Msk                        /*!< Channel[0] driver stage enable */
#define SN65DP141_REG_CHxCTL_EQ_EN_Pos        (1U)
#define SN65DP141_REG_CHxCTL_EQ_EN_Msk        (0x1U << SN65DP141_REG_CHxCTL_EQ_EN_Pos)              /*!< 0x02 */
#define SN65DP141_REG_CHxCTL_EQ_EN            SN65DP141_REG_CHxCTL_EQ_EN_Msk                        /*!< Channel[0] EQ stage enable */
#define SN65DP141_REG_CHxCTL_DRV_PEAK_Pos     (2U)
#define SN65DP141_REG_CHxCTL_DRV_PEAK_Msk     (0x1U << SN65DP141_REG_CHxCTL_DRV_PEAK_Pos)           /*!< 0x04 */
#define SN65DP141_REG_CHxCTL_DRV_PEAK         SN65DP141_REG_CHxCTL_DRV_PEAK_Msk                     /*!< Channel[0] driver peaking */

/** @defgroup SN65DP141_Channel_Identifier
  * @{
  */
#define SN65DP141_CHANNEL_1                  0x00U
#define SN65DP141_CHANNEL_2                  0x01U
#define SN65DP141_CHANNEL_3                  0x04U
#define SN65DP141_CHANNEL_4                  0x08U
#define SN65DP141_CHANNEL_ALL                0x0FU
/**
  * @}
  */

/** @defgroup SN65DP141_EQ_Mode
  * @{
  */
#define SN65DP141_EQ_MODE_CABLE              0x00U
#define SN65DP141_EQ_MODE_TRACE              SN65DP141_REG_CFG_EQ_MODE
/**
  * @}
  */

/** @defgroup SN65DP141_Channel_Tracking
  * @{
  */
#define SN65DP141_CHANNEL_TRACKING_NONE      0x00U
#define SN65DP141_CHANNEL_TRACKING_01        SN65DP141_REG_CFG_SYNC_01
#define SN65DP141_CHANNEL_TRACKING_23        SN65DP141_REG_CFG_SYNC_23
#define SN65DP141_CHANNEL_TRACKING_ALL       SN65DP141_REG_CFG_SYNC_ALL
/**
  * @}
  */

/** @defgroup SN65DP141_EQGAIN  SN65DP141 Equalizer Gain
  * @{
  */
#define SN65DP141_EQGAIN_0      0x0U
#define SN65DP141_EQGAIN_1      0x1U
#define SN65DP141_EQGAIN_2      0x2U
#define SN65DP141_EQGAIN_3      0x3U
#define SN65DP141_EQGAIN_4      0x4U
#define SN65DP141_EQGAIN_5      0x5U
#define SN65DP141_EQGAIN_6      0x6U
#define SN65DP141_EQGAIN_7      0x7U
/**
  * @}
  */

/** @defgroup SN65DP141_Channel_RXGAIN
  * @{
  */
#define SN65DP141_CHANNEL_RXGAIN_LOW         0x00U
#define SN65DP141_CHANNEL_RXGAIN_HIZ         SN65DP141_REG_CHxCFG_RX_GAIN_0
#define SN65DP141_CHANNEL_RXGAIN_HIGH        SN65DP141_REG_CHxCFG_RX_GAIN_1
/**
  * @}
  */

/** @defgroup SN65DP141_Channel_EQDCGAIN
  * @{
  */
#define SN65DP141_CHANNEL_EQDCGAIN_MINUS6DB  0x00U
#define SN65DP141_CHANNEL_EQDCGAIN_0DB       SN65DP141_REG_CHxCFG_EQ_DC_GAIN
/**
  * @}
  */

/** @defgroup SN65DP141_Channel_TXGAIN
  * @{
  */
#define SN65DP141_CHANNEL_RXGAIN_0DB         0x00U
#define SN65DP141_CHANNEL_RXGAIN_6DB         SN65DP141_REG_CHxCFG_TX_GAIN
/**
  * @}
  */

/**
  * @}
  */

#if defined(SN65DP141_DEBUG)
/** @defgroup SN65DP141_Exported_Structure SN65DP141 Exported Structure
  * @{
  */

typedef union {
  uint8_t Register;
  struct {
    uint8_t RESERVED  :2;
    uint8_t EQ_MODE   :1;
    uint8_t SYNC_ALL  :1;
    uint8_t SYNC_23   :1;
    uint8_t SYNC_01   :1;
    uint8_t PWRDOWN   :1;
    uint8_t SW_GPIO   :1;
  };
} SN65DP141_GeneralTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t LN_EN_CH0  :1;
    uint8_t LN_EN_CH1  :1;
    uint8_t LN_EN_CH2  :1;
    uint8_t LN_EN_CH3  :1;
    uint8_t RESERVED   :4;
  };
} SN65DP141_ChannelEnableTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RX_GAIN_0     :1;
    uint8_t RX_GAIN_1     :1;
    uint8_t EQ_DC_GAIN    :1;
    uint8_t TX_GAIN       :1;
    uint8_t EQ_Setting_0  :1;
    uint8_t EQ_Setting_1  :1;
    uint8_t EQ_Setting_2  :1;
    uint8_t RESERVED      :1;
  };
} SN65DP141_Channel0CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RSVDRV_EN :1;
    uint8_t EQ_EN     :1;
    uint8_t DRV_PEAK  :1;
    uint8_t RESERVED  :5;
  };
} SN65DP141_Channel0EnableTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RX_GAIN_0     :1;
    uint8_t RX_GAIN_1     :1;
    uint8_t EQ_DC_GAIN    :1;
    uint8_t TX_GAIN       :1;
    uint8_t EQ_Setting_0  :1;
    uint8_t EQ_Setting_1  :1;
    uint8_t EQ_Setting_2  :1;
    uint8_t RESERVED      :1;
  };
} SN65DP141_Channel1CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RSVDRV_EN :1;
    uint8_t EQ_EN     :1;
    uint8_t DRV_PEAK  :1;
    uint8_t RESERVED  :5;
  };
} SN65DP141_Channel1EnableTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RX_GAIN_0     :1;
    uint8_t RX_GAIN_1     :1;
    uint8_t EQ_DC_GAIN    :1;
    uint8_t TX_GAIN       :1;
    uint8_t EQ_Setting_0  :1;
    uint8_t EQ_Setting_1  :1;
    uint8_t EQ_Setting_2  :1;
    uint8_t RESERVED      :1;
  };
} SN65DP141_Channel2CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RSVDRV_EN :1;
    uint8_t EQ_EN     :1;
    uint8_t DRV_PEAK  :1;
    uint8_t RESERVED  :5;
  };
} SN65DP141_Channel2EnableTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RX_GAIN_0     :1;
    uint8_t RX_GAIN_1     :1;
    uint8_t EQ_DC_GAIN    :1;
    uint8_t TX_GAIN       :1;
    uint8_t EQ_Setting_0  :1;
    uint8_t EQ_Setting_1  :1;
    uint8_t EQ_Setting_2  :1;
    uint8_t RESERVED      :1;
  };
} SN65DP141_Channel3CtrlTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t RSVDRV_EN :1;
    uint8_t EQ_EN     :1;
    uint8_t DRV_PEAK  :1;
    uint8_t RESERVED  :5;
  };
} SN65DP141_Channel3EnableTypeDef;

typedef struct
{
  SN65DP141_GeneralTypeDef        General;        /*!< 0x00 (General Device Settings) */
  SN65DP141_ChannelEnableTypeDef  ChannelEnable;  /*!< 0x01 (Channel Enable) */
  SN65DP141_Channel0CtrlTypeDef   Channel0Ctrl;   /*!< 0x02 (Channel 0 Control Settings) */
  SN65DP141_Channel0EnableTypeDef Channel0Enable; /*!< 0x03 (Channel 0 Enable Settings) */
  SN65DP141_Channel1CtrlTypeDef   Channel1Ctrl;   /*!< 0x05 (Channel 1 Control Settings) */
  SN65DP141_Channel1EnableTypeDef Channel1Enable; /*!< 0x06 (Channel 1 Enable Settings) */
  SN65DP141_Channel2CtrlTypeDef   Channel2Ctrl;   /*!< 0x08 (Channel 2 Control Settings) */
  SN65DP141_Channel2EnableTypeDef Channel2Enable; /*!< 0x09 (Channel 2 Enable Settings) */
  SN65DP141_Channel3CtrlTypeDef   Channel3Ctrl;   /*!< 0x0B (Channel 3 Control Settings) */
  SN65DP141_Channel3EnableTypeDef Channel3Enable; /*!< 0x0C (Channel 3 Enable Settings) */
} SN65DP141_RegistersTypeDef;
/**
  * @}
  */
#endif /* SN65DP141_DEBUG */

/** @defgroup SN65DP141_Exported_Functions
  * @{
  */
uint32_t sn65dp141_Init(uint16_t Address);
void     sn65dp141_DeInit(uint16_t Address);
uint32_t sn65dp141_PowerOn(uint16_t Address);
uint32_t sn65dp141_PowerOff(uint16_t Address);
uint32_t sn65dp141_SetEQGain(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId, uint8_t EQGain);
uint32_t sn65dp141_EnableChannel(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId);
uint32_t sn65dp141_DisableChannel(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId);

/* MUX IO functions */
uint8_t   MUX_IO_Init(void);
void      MUX_IO_DeInit(void);
uint8_t   MUX_IO_Write(uint16_t Addr, uint16_t Reg, uint8_t Data);
uint8_t   MUX_IO_Read(uint16_t Addr, uint16_t Reg, uint8_t *pData);
uint32_t  MUX_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/**
  * @}
  */

/** @defgroup SN65DP141_Exported_Variables
  * @{
  */
/* DisplayPort Linear Redriver Driver structure */
extern DPREDRIVER_Drv_t sn65dp141_drv;
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

#endif /* SN65DP141_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

