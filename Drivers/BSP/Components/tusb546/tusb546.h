/**
  ******************************************************************************
  * @file    tusb546.h
  * @author  MCD Application Team
  * @brief   Header for tusb546.h module
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
#ifndef TUSB546_H
#define TUSB546_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../Common/usbtypecswitch.h"
#include "../Common/dpredriver.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup TUSB546
  * @{
  */

/** @defgroup TUSB546_Exported_Constants
  * @{
  */
/******************************* Register Map  ********************************/
#define TUSB546_REG_CTRL         0x0A  /*!< */
#define TUSB546_REG_DP_CTRL1     0x10  /*!< */
#define TUSB546_REG_DP_CTRL2     0x11  /*!< */
#define TUSB546_REG_DP_CTRL3     0x12  /*!< */
#define TUSB546_REG_DP_CTRL4     0x13  /*!< */
#define TUSB546_REG_USB3_CTRL1   0x20  /*!< */
#define TUSB546_REG_USB3_CTRL2   0x21  /*!< */
#define TUSB546_REG_USB3_CTRL3   0x22  /*!< */

/***********************  Bit definition for CTRL register  *******************/
#define TUSB546_REG_CTRL_CTLSEL_Pos         (0U)
#define TUSB546_REG_CTRL_CTLSEL_Msk         (0x3U << TUSB546_REG_CTRL_CTLSEL_Pos)         /*!< 0x03*/
#define TUSB546_REG_CTRL_CTLSEL             TUSB546_REG_CTRL_CTLSEL_Msk                   /*!< DP Alt mode and USB 3.1 Switch Control */
#define TUSB546_REG_CTRL_CTLSEL_0           (0x1 << TUSB546_REG_CTRL_CTLSEL_Pos)          /*!< 0x01 */
#define TUSB546_REG_CTRL_CTLSEL_1           (0x2 << TUSB546_REG_CTRL_CTLSEL_Pos)          /*!< 0x02 */
#define TUSB546_REG_CTRL_FLIPSEL_Pos        (2U)
#define TUSB546_REG_CTRL_FLIPSEL_Msk        (0x1U << TUSB546_REG_CTRL_FLIPSEL_Pos)        /*!< 0x04*/
#define TUSB546_REG_CTRL_FLIPSEL            TUSB546_REG_CTRL_FLIPSEL_Msk                  /*!< Flip Control */
#define TUSB546_REG_CTRL_HPDIN_OVERRIDE_Pos (3U)
#define TUSB546_REG_CTRL_HPDIN_OVERRIDE_Msk (0x1U << TUSB546_REG_CTRL_HPDIN_OVERRIDE_Pos) /*!< 0x08*/
#define TUSB546_REG_CTRL_HPDIN_OVERRIDE     TUSB546_REG_CTRL_HPDIN_OVERRIDE_Msk           /*!< HPDIN control */
#define TUSB546_REG_CTRL_EQ_OVERRIDE_Pos    (4U)
#define TUSB546_REG_CTRL_EQ_OVERRIDE_Msk    (0x1U << TUSB546_REG_CTRL_EQ_OVERRIDE_Pos)    /*!< 0x10*/
#define TUSB546_REG_CTRL_EQ_OVERRIDE        TUSB546_REG_CTRL_EQ_OVERRIDE_Msk              /*!< EQ settings control */
#define TUSB546_REG_CTRL_SWAP_HPDIN_Pos     (5U)
#define TUSB546_REG_CTRL_SWAP_HPDIN_Msk     (0x1U << TUSB546_REG_CTRL_SWAP_HPDIN_Pos)     /*!< 0x20*/
#define TUSB546_REG_CTRL_SWAP_HPDIN         TUSB546_REG_CTRL_SWAP_HPDIN_Msk               /*!< HPDIN pin control */

/*********************  Bit definition for DP_CTRL1 register  *****************/
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos  (0U)
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Msk  (0xFU << TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos)  /*!< 0x0F */
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL      TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Msk            /*!< Field selects between 0 to 14dB of EQ for DP lane 0 */
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_0    (0x1 << TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos)   /*!< 0x01 */
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_1    (0x2 << TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos)   /*!< 0x02 */
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_2    (0x4 << TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos)   /*!< 0x04 */
#define TUSB546_REG_DP_CTRL1_DP0EQ_SEL_4    (0x8 << TUSB546_REG_DP_CTRL1_DP0EQ_SEL_Pos)   /*!< 0x08 */
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos  (4U)
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Msk  (0xFU << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos)  /*!< 0xF0*/
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL      TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Msk            /*!< Field selects between 0 to 14dB of EQ for DP lane 1 */
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_0    (0x1 << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos)   /*!< 0x10 */
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_1    (0x2 << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos)   /*!< 0x20 */
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_2    (0x4 << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos)   /*!< 0x40 */
#define TUSB546_REG_DP_CTRL1_DP1EQ_SEL_4    (0x8 << TUSB546_REG_DP_CTRL1_DP1EQ_SEL_Pos)   /*!< 0x80 */

/*********************  Bit definition for DP_CTRL2 register  *****************/
#define TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos  (0U)
#define TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Msk  (0xFU << TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos)  /*!< 0x0F */
#define TUSB546_REG_DP_CTRL2_DP2EQ_SEL      TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Msk            /*!< Field selects between 0 to 14dB of EQ for DP lane 2 */
#define TUSB546_REG_DP_CTRL1_DP2EQ_SEL_0    (0x1 << TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos)   /*!< 0x01 */
#define TUSB546_REG_DP_CTRL1_DP2EQ_SEL_1    (0x2 << TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos)   /*!< 0x02 */
#define TUSB546_REG_DP_CTRL1_DP2EQ_SEL_2    (0x4 << TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos)   /*!< 0x04 */
#define TUSB546_REG_DP_CTRL1_DP2EQ_SEL_4    (0x8 << TUSB546_REG_DP_CTRL2_DP2EQ_SEL_Pos)   /*!< 0x08 */
#define TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos  (4U)
#define TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Msk  (0xFU << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos)  /*!< 0xF0*/
#define TUSB546_REG_DP_CTRL2_DP3EQ_SEL      TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Msk            /*!< Field selects between 0 to 14dB of EQ for DP lane 3 */
#define TUSB546_REG_DP_CTRL1_DP3EQ_SEL_0    (0x1 << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos)   /*!< 0x10 */
#define TUSB546_REG_DP_CTRL1_DP3EQ_SEL_1    (0x2 << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos)   /*!< 0x20 */
#define TUSB546_REG_DP_CTRL1_DP3EQ_SEL_2    (0x4 << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos)   /*!< 0x40 */
#define TUSB546_REG_DP_CTRL1_DP3EQ_SEL_4    (0x8 << TUSB546_REG_DP_CTRL2_DP3EQ_SEL_Pos)   /*!< 0x80 */

/*********************  Bit definition for DP_CTRL3 register  *****************/
#define TUSB546_REG_DP_CTRL3_LANE_COUNT_SET_Pos   (0U)
#define TUSB546_REG_DP_CTRL3_LANE_COUNT_SET_Msk   (0x1FU << TUSB546_REG_DP_CTRL3_LANE_COUNT_SET_Pos)    /*!< 0x1F */
#define TUSB546_REG_DP_CTRL3_LANE_COUNT_SET       TUSB546_REG_DP_CTRL3_LANE_COUNT_SET_Msk               /*!< */
#define TUSB546_REG_DP_CTRL3_SET_POWER_STATE_Pos  (5U)
#define TUSB546_REG_DP_CTRL3_SET_POWER_STATE_Msk  (0x3U << TUSB546_REG_DP_CTRL3_SET_POWER_STATE5_Pos)   /*!< 0x60 */
#define TUSB546_REG_DP_CTRL3_SET_POWER_STATE      TUSB546_REG_DP_CTRL3_SET_POWER_STATE5_Msk             /*!< */
#define TUSB546_REG_DP_CTRL3_SET_POWER_STATE_0    (0x1 << TUSB546_REG_DP_CTRL3_SET_POWER_STATE_Pos)     /*!< 0x20 */
#define TUSB546_REG_DP_CTRL3_SET_POWER_STATE_1    (0x2 << TUSB546_REG_DP_CTRL3_SET_POWER_STATE_Pos)     /*!< 0x40 */

/*********************  Bit definition for DP_CTRL4 register  *****************/
#define TUSB546_REG_DP_CTRL4_DP0_DISABLE_Pos      (0U)
#define TUSB546_REG_DP_CTRL4_DP0_DISABLE_Msk      (0x1U << TUSB546_REG_DP_CTRL4_DP0_DISABLE_Pos)    /*!< 0x01 */
#define TUSB546_REG_DP_CTRL4_DP0_DISABLE          TUSB546_REG_DP_CTRL4_DP0_DISABLE_Msk              /*!< DP Lane 0 disable */
#define TUSB546_REG_DP_CTRL4_DP1_DISABLE_Pos      (1U)
#define TUSB546_REG_DP_CTRL4_DP1_DISABLE_Msk      (0x1U << TUSB546_REG_DP_CTRL4_DP1_DISABLE_Pos)    /*!< 0x02 */
#define TUSB546_REG_DP_CTRL4_DP1_DISABLE          TUSB546_REG_DP_CTRL4_DP1_DISABLE_Msk              /*!< DP Lane 1 disable */
#define TUSB546_REG_DP_CTRL4_DP2_DISABLE_Pos      (2U)
#define TUSB546_REG_DP_CTRL4_DP2_DISABLE_Msk      (0x1U << TUSB546_REG_DP_CTRL4_DP2_DISABLE_Pos)    /*!< 0x04 */
#define TUSB546_REG_DP_CTRL4_DP2_DISABLE          TUSB546_REG_DP_CTRL4_DP2_DISABLE_Msk              /*!< DP Lane 2 disable */
#define TUSB546_REG_DP_CTRL4_DP3_DISABLE_Pos      (3U)
#define TUSB546_REG_DP_CTRL4_DP3_DISABLE_Msk      (0x1U << TUSB546_REG_DP_CTRL4_DP3_DISABLE_Pos)    /*!< 0x08 */
#define TUSB546_REG_DP_CTRL4_DP3_DISABLE          TUSB546_REG_DP_CTRL4_DP3_DISABLE_Msk              /*!< DP Lane 3 disable */
#define TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Pos      (4U)
#define TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Msk      (0x3U << TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Pos)    /*!< 0x30 */
#define TUSB546_REG_DP_CTRL4_AUX_SBU_OVR          TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Msk              /*!< AUX to SBU config */
#define TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_0        (0x1 << TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Pos)     /*!< 0x10 */
#define TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_1        (0x2 << TUSB546_REG_DP_CTRL4_AUX_SBU_OVR_Pos)     /*!< 0x20 */
#define TUSB546_REG_DP_CTRL4_SNOOP_DISABLE_Pos    (7U)
#define TUSB546_REG_DP_CTRL4_SNOOP_DISABLE_Msk    (0x1U << TUSB546_REG_DP_CTRL4_SNOOP_DISABLE_Pos)  /*!< 0x80 */
#define TUSB546_REG_DP_CTRL4_SNOOP_DISABLE        TUSB546_REG_DP_CTRL4_SNOOP_DISABLE_Msk            /*!< AUX snoop disable */

/********************  Bit definition for USB3_CTRL1 register  ****************/
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos          (0U)
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_Msk          (0xFU << TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos)          /*!< 0x0F */
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL              TUSB546_REG_USB3_CTRL1_EQ1_SEL_Msk                    /*!< Field selects between 0 to 11 dB of EQ for USB3.1 RX1 receiver */
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_0            (0x1 << TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos)           /*!< 0x01 */
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_1            (0x2 << TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos)           /*!< 0x02 */
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_2            (0x4 << TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos)           /*!< 0x04 */
#define TUSB546_REG_USB3_CTRL1_EQ1_SEL_4            (0x8 << TUSB546_REG_USB3_CTRL1_EQ1_SEL_Pos)           /*!< 0x08 */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos          (4U)
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_Msk          (0xFU << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos)          /*!< 0xF0 */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL              TUSB546_REG_USB3_CTRL1_EQ2_SEL_Msk                    /*!< Field selects between 0 to 11 dB of EQ for USB3.1 RX2 receiver */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_0            (0x1 << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos)           /*!< 0x10 */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_1            (0x2 << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos)           /*!< 0x20 */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_2            (0x4 << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos)           /*!< 0x40 */
#define TUSB546_REG_USB3_CTRL1_EQ2_SEL_4            (0x8 << TUSB546_REG_USB3_CTRL1_EQ2_SEL_Pos)           /*!< 0x80 */

/********************  Bit definition for USB3_CTRL2 register  ****************/
#define TUSB546_USB3_CTRL2_SSEQ_SEL_Pos             (0U)
#define TUSB546_USB3_CTRL2_SSEQ_SEL_Msk             (0xFU << TUSB546_USB3_CTRL2_SSEQ_SEL_Pos)             /*!< 0x0F */
#define TUSB546_USB3_CTRL2_SSEQ_SEL                 TUSB546_USB3_CTRL2_SSEQ_SEL_Msk                       /*!< Field selects between 0 to 9 dB of EQ for USB3.1 SSTXP/N receiver */
#define TUSB546_USB3_CTRL2_SSEQ_SEL_0               (0x1 << TUSB546_USB3_CTRL2_SSEQ_SEL_Pos)              /*!< 0x01 */
#define TUSB546_USB3_CTRL2_SSEQ_SEL_1               (0x2 << TUSB546_USB3_CTRL2_SSEQ_SEL_Pos)              /*!< 0x02 */
#define TUSB546_USB3_CTRL2_SSEQ_SEL_2               (0x4 << TUSB546_USB3_CTRL2_SSEQ_SEL_Pos)              /*!< 0x04 */
#define TUSB546_USB3_CTRL2_SSEQ_SEL_4               (0x8 << TUSB546_USB3_CTRL2_SSEQ_SEL_Pos)              /*!< 0x08 */

/********************  Bit definition for USB3_CTRL3 register  ****************/
#define TUSB546_REG_USB3_CTRL3_COMPLIANCE_Pos         (0U)
#define TUSB546_REG_USB3_CTRL3_COMPLIANCE_Msk         (0x3U << TUSB546_REG_USB3_CTRL3_COMPLIANCE_Pos)          /*!< 0x03 */
#define TUSB546_REG_USB3_CTRL3_COMPLIANCE             TUSB546_REG_USB3_CTRL3_COMPLIANCE_Msk                    /*!< Compliance mode */
#define TUSB546_REG_USB3_CTRL3_COMPLIANCE_0           (0x1 << TUSB546_REG_USB3_CTRL3_COMPLIANCE_Pos)           /*!< 0x01 */
#define TUSB546_REG_USB3_CTRL3_COMPLIANCE_1           (0x2 << TUSB546_REG_USB3_CTRL3_COMPLIANCE_Pos)           /*!< 0x02 */
#define TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Pos (2U)
#define TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Msk (0x3U << TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Pos)  /*!< 0x0C */
#define TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL     TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Msk            /*!< Rx.Detect interval for the Downstream facing port */
#define TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_0   (0x1 << TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Pos)   /*!< 0x04 */
#define TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_1   (0x2 << TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_Pos)   /*!< 0x08 */
#define TUSB546_REG_USB3_CTRL3_DISABLE_U2U3_RXDET_Pos (4U)
#define TUSB546_REG_USB3_CTRL3_DISABLE_U2U3_RXDET_Msk (0x1U << TUSB546_REG_USB3_CTRL3_DISABLE_U2U3_RXDET_Pos)  /*!< 0x10 */
#define TUSB546_REG_USB3_CTRL3_DISABLE_U2U3_RXDET     TUSB546_REG_USB3_CTRL3_DISABLE_U2U3_RXDET_Msk            /*!< Rx.Detect in U2/U3 disabled */
#define TUSB546_REG_USB3_CTRL3_U2U3_LFPS_DEBOUNCE_Pos (5U)
#define TUSB546_REG_USB3_CTRL3_U2U3_LFPS_DEBOUNCE_Msk (0x1U << TUSB546_REG_USB3_CTRL3_U2U3_LFPS_DEBOUNCE_Pos)  /*!< 0x20 */
#define TUSB546_REG_USB3_CTRL3_U2U3_LFPS_DEBOUNCE     TUSB546_REG_USB3_CTRL3_U2U3_LFPS_DEBOUNCE_Msk            /*!< Debounce of LFPS before U2/U3 exit enabled */
#define TUSB546_REG_USB3_CTRL3_LFPS_EQ_Pos            (6U)
#define TUSB546_REG_USB3_CTRL3_LFPS_EQ_Msk            (0x1U << TUSB546_REG_USB3_CTRL3_LFPS_EQ_Pos)             /*!< 0x40 */
#define TUSB546_REG_USB3_CTRL3_LFPS_EQ                TUSB546_REG_USB3_CTRL3_LFPS_EQ_Msk                       /*!< EQ settings control */
#define TUSB546_REG_USB3_CM_ACTIVE_EQ_Pos             (7U)
#define TUSB546_REG_USB3_CM_ACTIVE_EQ_Msk             (0x1U << TUSB546_REG_USB3_CM_ACTIVE_EQ_Pos)              /*!< 0x80 */
#define TUSB546_REG_USB3_CM_ACTIVE_EQ                 TUSB546_REG_USB3_CM_ACTIVE_EQ_Msk                        /*!< USB 3.1 compliance mode */

/** @defgroup TUSB546_MODE  TUSB546 functioning mode
  * @{
  */
#define TUSB546_MODE_DISABLED         0x0U
#define TUSB546_MODE_USB3_ONLY        TUSB546_REG_CTRL_CTLSEL_0
#define TUSB546_MODE_FOUR_DPLANES     TUSB546_REG_CTRL_CTLSEL_1
#define TUSB546_MODE_TWO_DPLANES_USB3 TUSB546_REG_CTRL_CTLSEL
/**
  * @}
  */

/** @defgroup TUSB546_ORIENTATION  TUSB546 Type-C Cable Orientation
  * @{
  */
#define TUSB546_ORIENTATION_NORMAL    0x0U
#define TUSB546_ORIENTATION_FLIPPED   TUSB546_REG_CTRL_FLIPSEL
/**
  * @}
  */

/** @defgroup TUSB546_DPLANE  DP Lane Identifier
  * @{
  */
#define TUSB546_DPLANE_0     0x0U
#define TUSB546_DPLANE_1     0x2U
#define TUSB546_DPLANE_2     0x4U
#define TUSB546_DPLANE_3     0x8U
#define TUSB546_DPLANE_ALL   0xFU
/**
  * @}
  */

/** @defgroup TUSB546_USBSIGNAL  TUSB546 USB 3.1 signal identifier
  * @{
  */
#define TUSB546_USBSIGNAL_RX1   0x0U
#define TUSB546_USBSIGNAL_RX2   0x1U
#define TUSB546_USBSIGNAL_SSTX  0x2U
/**
  * @}
  */

/** @defgroup TUSB546_EQGAIN  TUSB546 Equalizer Gain
  * @{
   .------------------------.-------------------------.-------------------------.------------------------.
   | Equalization setting # | USB3.1 DFP EQ Gain (dB) | USB3.1 UFP EQ Gain (dB) | DISPLAYPORT LANES (dB) |
   '------------------------'-------------------------'-------------------------'------------------------'
   | 0                      |        0.2              |        -1.6             |          1.0           |
   | 1                      |        1.2              |        -0.5             |          3.3           |
   | 2                      |        2.2              |         0.5             |          4.9           |
   | 3                      |        3.3              |         1.6             |          6.5           |
   | 4                      |        4.2              |         2.4             |          7.5           |
   | 5                      |        5.1              |         3.4             |          8.6           |
   | 6                      |        5.9              |         4.1             |          9.5           |
   | 7                      |        6.7              |         4.9             |         10.4           |
   | 8                      |        7.4              |         5.7             |         11.1           |
   | 9                      |        8.1              |         6.4             |         11.7           |
   | 10                     |        8.7              |         6.9             |         12.3           |
   | 11                     |        9.3              |         7.5             |         12.8           |
   | 12                     |        9.7              |         8.0             |         13.2           |
   | 13                     |       10.2              |         8.5             |         13.6           |
   | 14                     |       10.6              |         8.9             |         14.0           |
   | 15                     |       11.1              |         9.4             |         14.4           |
   '------------------------'-------------------------'-------------------------'------------------------'
  */
#define TUSB546_EQGAIN_0      0x0U
#define TUSB546_EQGAIN_1      0x1U
#define TUSB546_EQGAIN_2      0x2U
#define TUSB546_EQGAIN_3      0x3U
#define TUSB546_EQGAIN_4      0x4U
#define TUSB546_EQGAIN_5      0x5U
#define TUSB546_EQGAIN_6      0x6U
#define TUSB546_EQGAIN_7      0x7U
#define TUSB546_EQGAIN_8      0x8U
#define TUSB546_EQGAIN_9      0x9U
#define TUSB546_EQGAIN_10     0xAU
#define TUSB546_EQGAIN_11     0xBU
#define TUSB546_EQGAIN_12     0xCU
#define TUSB546_EQGAIN_13     0xDU
#define TUSB546_EQGAIN_14     0xEU
#define TUSB546_EQGAIN_15     0xFU
/**
  * @}
  */

/** @defgroup TUSB546_DPCDREG  TUSB546 Display Port Configuration Data (DPCD) register identifier
  * @{
  */
#define TUSB546_DPCDREG_LANE_COUNT_SET   0x0U
#define TUSB546_DPCDREG_SET_POWER        0x1U
/**
  * @}
  */

/** @defgroup TUSB546_AUXTOSBU  TUSB546 AUXp or AUXn to SBU1 or SBU2 connect/disconnect override
  * @{
  */
#define TUSB546_AUXTOSBU_DEFAULT   0x0U
#define TUSB546_AUXTOSBU_OVERRIDE  0x1U
#define TUSB546_AUXTOSBU_OPEN      0x2U
/**
  * @}
  */

/** @defgroup TUSB546_USB3_COMPLIANCEMODE  USB 3.1 Compliance mode
  * @{
  */
#define TUSB546_USB3_COMPLIANCEMODE_FSM      0x0U
#define TUSB546_USB3_COMPLIANCEMODE_DFP      TUSB546_REG_USB3_CTRL3_COMPLIANCE_0
#define TUSB546_USB3_COMPLIANCEMODE_UFP      TUSB546_REG_USB3_CTRL3_COMPLIANCE_1
#define TUSB546_USB3_COMPLIANCEMODE_DISABLED TUSB546_REG_USB3_CTRL3_COMPLIANCE
/**
  * @}
  */

/** @defgroup TUSB546_RXDET_INTERVAL  USB 3.1 Rx.Detect interval for the Downstream facing port (TX1P/N and TX2P/N)
  * @{
  */
#define TUSB546_RXDET_INTERVAL_8MS      0x0U
#define TUSB546_RXDET_INTERVAL_12MS     TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_0
#define TUSB546_RXDET_INTERVAL_48MS     TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL_1
#define TUSB546_RXDET_INTERVAL_96MS     TUSB546_REG_USB3_CTRL3_DFP_RXDET_INTERVAL
/**
  * @}
  */

/**
  * @}
  */

#if defined(TUSB546_DEBUG)
/** @defgroup TUSB546_Exported_Structure TUSB546 Exported Structure
  * @{
  */

typedef union {
  uint8_t Register;
  struct {
    uint8_t CTLSEL:2;
    uint8_t FLIPSEL:1;
    uint8_t HPDIN_OVRRIDE:1;
    uint8_t EQ_OVERRIDE:1;
    uint8_t SWAP_HPDIN :1;
    uint8_t Reserved:2;
  };
} TUSB546_GeneralRegTypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t DP0EQ_SEL:4;
    uint8_t DP1EQ_SEL:4;
  };
} TUSB546_DPCtrlStatusReg10TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t DP2EQ_SEL:4;
    uint8_t DP3EQ_SEL:4;
  };
} TUSB546_DPCtrlStatusReg11TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t LANE_COUNT_SET    :5;
    uint8_t SET_POWER_STATE   :2;
    uint8_t Reserved          :1;
  };
} TUSB546_DPCtrlStatusReg12TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t DP0_DISABLE       :1;
    uint8_t DP1_DISABLE       :1;
    uint8_t DP2_DISABLE       :1;
    uint8_t DP3_DISABLE       :1;
    uint8_t AUX_SBU_OVR       :2;
    uint8_t Reserved          :1;
    uint8_t AUX_SNOOP_DISABLE :1;
  };
} TUSB546_DPCtrlStatusReg13TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t Reserved          :8;
  };
} TUSB546_USBCtrlStatusReg20TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t Reserved          :8;
  };
} TUSB546_USBCtrlStatusReg21TypeDef;

typedef union {
  uint8_t Register;
  struct {
    uint8_t Reserved          :8;
  };
} TUSB546_USBCtrlStatusReg22TypeDef;

typedef struct
{
  TUSB546_GeneralRegTypeDef         General;        /*!< General Registers (0x0A) */
  TUSB546_DPCtrlStatusReg10TypeDef  DPCtrlStatus10; /*!< DisplayPort Control/Status Registers (0x10)*/
  TUSB546_DPCtrlStatusReg11TypeDef  DPCtrlStatus11; /*!< DisplayPort Control/Status Registers (0x11)*/
  TUSB546_DPCtrlStatusReg12TypeDef  DPCtrlStatus12; /*!< DisplayPort Control/Status Registers (0x12) */
  TUSB546_DPCtrlStatusReg13TypeDef  DPCtrlStatus13; /*!< DisplayPort Control/Status Registers (0x13) */
  TUSB546_USBCtrlStatusReg20TypeDef USBCtrlStatus20;/*!< USB3.1 Control/Status Registers (0x20) */
  TUSB546_USBCtrlStatusReg21TypeDef USBCtrlStatus21;/*!< USB3.1 Control/Status Registers (0x21) */
  TUSB546_USBCtrlStatusReg22TypeDef USBCtrlStatus22;/*!< USB3.1 Control/Status Registers (0x22) */
} TUSB546_RegistersTypeDef;
/**
  * @}
  */
#endif /* TUSB546_DEBUG */

/** @defgroup TUSB546_Exported_Functions
  * @{
  */
/* USB Type-C cross switch management functions */
uint32_t  tusb546_CrossSwitch_Init(uint16_t Address);
void      tusb546_CrossSwitch_DeInit(uint16_t Address);
uint32_t  tusb546_CrossSwitch_PowerOn(uint16_t Address);
uint32_t  tusb546_CrossSwitch_PowerOff(uint16_t Address);
uint32_t  tusb546_CrossSwitch_SetMode(uint16_t Address, TYPECSWITCH_Mode_t Mode);
uint32_t  tusb546_CrossSwitch_IsSupportedMode(TYPECSWITCH_Mode_t Mode);

/* DisplayPort Linear Redriver management functions */
uint32_t  tusb546_DPRedriver_Init(uint16_t Address);
void      tusb546_DPRedriver_DeInit(uint16_t Address);
uint32_t  tusb546_DPRedriver_PowerOn(uint16_t Address);
uint32_t  tusb546_DPRedriver_PowerOff(uint16_t Address);
uint32_t  tusb546_DPRedriver_SetEQGain(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId, uint8_t EQGain);
uint32_t  tusb546_DPRedriver_EnableChannel(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId);
uint32_t  tusb546_DPRedriver_DisableChannel(uint16_t Address, DPREDRIVER_ChannelId_t ChannelId);

/* MUX IO functions */
uint8_t   MUX_IO_Init(void);
void      MUX_IO_DeInit(void);
uint8_t   MUX_IO_Write(uint16_t Addr, uint16_t Reg, uint8_t Data);
uint8_t   MUX_IO_Read(uint16_t Addr, uint16_t Reg, uint8_t *pData);
uint32_t  MUX_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/**
  * @}
  */

/** @defgroup TUSB546_Exported_Variables
  * @{
  */
/* USB Type-C crossbar switch driver structure */
extern TYPECSWITCH_Drv_t tusb546_drv_CrossSwitch;

/* Displayport Linear Redriver driver structure */
extern DPREDRIVER_Drv_t tusb546_drv_LinearRedriver;
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

#endif /* TUSB546_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
