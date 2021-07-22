/**
  ******************************************************************************
  * @file    stm32g081b_eval_mux.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the Type-C
  *          MUX associated to a Type-C receptacle. These functions allow for
  *          reconfiguring the pins of the Type-C receptacle when the related
  *          Type-C port operate in DisplayPort alternate mode or
  *          when it is used as a USB 3.1 to USB Type-C adapter.
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32g081b_eval.h"
#include "stm32g081b_eval_mux.h"
#include "stm32g0xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32G081B_EVAL
  * @{
  */

/** @addtogroup STM32G081B_EVAL_MUX
  * @{
  */

/** @defgroup STM32G081B_EVAL_MUX_Private_Typedef Private Typedef
  * @{
  */
typedef enum {
    MUX_NOT_INITIALIZED = 0,
    MUX_INITIALIZED
} MUX_StateTypedef;

typedef struct {
    MUX_StateTypedef    State;
    TYPECSWITCH_Drv_t * TypeCSwitch_Drv;
    uint16_t            I2C_Address_TypeCSwitch;
    DPREDRIVER_Drv_t  * DPRedriver_Drv;
    uint16_t            I2C_Address_DPRedriver;
} MuxInfoTypeDef;

typedef struct {
#if defined(TUSB546_DEBUG)
    TUSB546_RegistersTypeDef            TUSB546_Registers;
#endif /* TUSB546_DEBUG */
#if defined(CBTL08GP053_DEBUG)
    CBTL08GP053_RegistersTypeDef        CBTL08GP053_Registers;
#endif /* CBTL08GP053_DEBUG */
#if defined(SN65DP141_DEBUG)
    SN65DP141_RegistersTypeDef          SN65DP141_Registers;
#endif /* CBTL08GP053_DEBUG */
    MUX_HPDCallbackFuncTypeDef *        pfnHPDCallbackFunc;
    MUX_HPDStateTypeDef                 HPDState;
    MUX_USB3DetectStateTypeDef          USB3DetectState;
    MUX_USB3DetectCallbackFuncTypeDef * pfnUSB3DetectCallback;
    MuxInfoTypeDef                      MuxInfo[TYPE_C_MUX_NB];
} ContextTypeDef;

typedef enum
{
    DET_HPD_SOURCE = 0,
    DET_USB3_DET   = 1
} DetectId_TypeDef;

typedef enum
{
    SEL_HPDIN      = 0
} SelectId_TypeDef;

typedef enum {
    DET_STATE_LOW = 0,
    DET_STATE_HIGH
} MUX_DETState_TypeDef;

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_MUX_Private_Macros Private Macros
  * @{
  */
#define DETx_GPIO_CLK_ENABLE(__DET__)    do { if((__DET__) == DET_HPD_SOURCE) DET_HPD_SOURCE_GPIO_CLK_ENABLE(); else \
                                              if((__DET__) == DET_USB3_DET)   DET_USB3_DET_GPIO_CLK_ENABLE();} while(0)

#define SELx_GPIO_CLK_ENABLE(__SEL__)    do { if((__SEL__) == SEL_HPDIN)     SEL_HPDIN_GPIO_CLK_ENABLE();} while(0)
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_MUX_Private_Variables Private Variables
  * @{
  */
static const TYPECSWITCH_Mode_t ModeSelect[2][13] =
{
    /* PLUG_ORIENTATION_NORMAL */
    {
        DFP_D_PIN_ASSIGNMENT_A_NORMAL,  /* DFP_PIN_ASSIGNMMENT_A    */
        DFP_D_PIN_ASSIGNMENT_B_NORMAL,  /* DFP_PIN_ASSIGNMMENT_B    */
        DFP_D_PIN_ASSIGNMENT_C_NORMAL,  /* DFP_PIN_ASSIGNMMENT_C    */
        DFP_D_PIN_ASSIGNMENT_D_NORMAL,  /* DFP_PIN_ASSIGNMMENT_D    */
        DFP_D_PIN_ASSIGNMENT_E_NORMAL,  /* DFP_PIN_ASSIGNMMENT_E    */
        DFP_D_PIN_ASSIGNMENT_F_NORMAL,  /* DFP_PIN_ASSIGNMMENT_F    */
        UFP_D_PIN_ASSIGNMENT_A_NORMAL,  /* UFP_PIN_ASSIGNMMENT_A    */
        UFP_D_PIN_ASSIGNMENT_B_NORMAL,  /* UFP_PIN_ASSIGNMMENT_B    */
        UFP_D_PIN_ASSIGNMENT_C_NORMAL,  /* UFP_PIN_ASSIGNMMENT_C    */
        UFP_D_PIN_ASSIGNMENT_D_NORMAL,  /* UFP_PIN_ASSIGNMMENT_D    */
        UFP_D_PIN_ASSIGNMENT_E_NORMAL,  /* UFP_PIN_ASSIGNMMENT_E    */
        UFP_D_PIN_ASSIGNMENT_F_NORMAL,  /* UFP_PIN_ASSIGNMMENT_F    */
        USB_NORMAL                      /* USB_ONLY_PIN_ASSIGNMMENT */
    },

    /* PLUG_ORIENTATION_FLIPPED */
    {

        DFP_D_PIN_ASSIGNMENT_A_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_A    */
        DFP_D_PIN_ASSIGNMENT_B_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_B    */
        DFP_D_PIN_ASSIGNMENT_C_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_C    */
        DFP_D_PIN_ASSIGNMENT_D_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_D    */
        DFP_D_PIN_ASSIGNMENT_E_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_E    */
        DFP_D_PIN_ASSIGNMENT_F_FLIPPED,  /* DFP_PIN_ASSIGNMMENT_F    */
        UFP_D_PIN_ASSIGNMENT_A_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_A    */
        UFP_D_PIN_ASSIGNMENT_B_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_B    */
        UFP_D_PIN_ASSIGNMENT_C_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_C    */
        UFP_D_PIN_ASSIGNMENT_D_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_D    */
        UFP_D_PIN_ASSIGNMENT_E_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_E    */
        UFP_D_PIN_ASSIGNMENT_F_FLIPPED,  /* UFP_PIN_ASSIGNMMENT_F    */
        USB_FLIPPED                      /* USB_ONLY_PIN_ASSIGNMMENT */
    }
};

/**
* @brief DET variables
*/
static GPIO_TypeDef* DET_PORT[DETn] = {
    DET_HPD_SOURCE_GPIO_PORT,
    DET_USB3_DET_GPIO_PORT
};

static uint16_t DET_PIN[DETn] = {
    DET_HPD_SOURCE_PIN,
    DET_USB3_DET_PIN
};

static IRQn_Type DET_IRQn[DETn] = {
#if defined(USE_STM32G081B_EVAL_REVA)  
    DET_HPD_SOURCE_EXTI_IRQn,
    DET_USB3_DET_EXTI_IRQn
#else
    DET_EXTI_IRQn,
    DET_EXTI_IRQn
#endif
};

/**
* @brief SEL variables
*/
static GPIO_TypeDef* SEL_PORT[SELn] = {
    SEL_HPDIN_GPIO_PORT,
};

static uint16_t SEL_PIN[SELn] = {
    SEL_HPDIN_PIN,
};

/* BSP PWR contextual data */
static ContextTypeDef Context =
{
    .HPDState              = HPD_STATE_LOW,
    .USB3DetectState       = USB3_NOT_CONNECTED,
    .pfnHPDCallbackFunc    = (MUX_HPDCallbackFuncTypeDef *)NULL,
    .pfnUSB3DetectCallback = (MUX_USB3DetectCallbackFuncTypeDef *)NULL,
    .MuxInfo =
    {
        /* TYPE_C_MUX_1 */
        {
            .State                   = MUX_NOT_INITIALIZED,
            .TypeCSwitch_Drv         = &tusb546_drv_CrossSwitch,
            .I2C_Address_TypeCSwitch = MUX_1_TYPEC_SWITCH_I2C_ADDRESS,
            .DPRedriver_Drv          = &tusb546_drv_LinearRedriver,
            .I2C_Address_DPRedriver  = MUX_1_DP_REDRIVER_I2C_ADDRESS
        },
        /* TYPE_C_MUX_2 */
        {
            .State                   = MUX_NOT_INITIALIZED,
            .TypeCSwitch_Drv         = &cbtl08gp053_drv,
            .I2C_Address_TypeCSwitch = MUX_2_TYPEC_SWITCH_I2C_ADDRESS,
            .DPRedriver_Drv          = &sn65dp141_drv,
            .I2C_Address_DPRedriver  = MUX_2_DP_REDRIVER_I2C_ADDRESS
        }
    }
};

/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_MUX_Private_Functions Private Functions
  * @{
  */
static void MUX_DET_Init(DetectId_TypeDef Det);
static void MUX_DET_DeInit(DetectId_TypeDef Det);
static MUX_DETState_TypeDef MUX_DET_GetState(DetectId_TypeDef Det);
static void MUX_SEL_Init(SelectId_TypeDef Sel);
static void MUX_SEL_DeInit(SelectId_TypeDef Sel);
static void MUX_SEL_On(SelectId_TypeDef Sel);
static void MUX_SEL_Off(SelectId_TypeDef Sel);
static void MUX_DebounceTimerSetConfig(uint32_t DebounceTime);
static void MUX_DebounceTimerResetConfig(void);
static void TIMx_Base_MspInit(TIM_HandleTypeDef *htim);
static void TIMx_Base_MspDeInit(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @defgroup STM32G081B_EVAL_MUX_Exported_Variables Exported Variables
  * @{
  */
TIM_HandleTypeDef htim = {.Instance = DEBOUNCE_TIM_INSTANCE};
/**
  * @}
  */

/** @addtogroup STM32G081B_EVAL_MUX_Exported_Functions
  * @{
  */

/**
 * @brief  Initialize the hardware resources used by the Type-C MUX
 *         assigned to a given Type-C MUX.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_Init(MUX_TypeCMuxIdTypeDef TypeCMuxId)
{
    uint32_t ret = 0;

    if (Context.MuxInfo[TypeCMuxId].State == MUX_NOT_INITIALIZED)
    {
        switch (TypeCMuxId)
        {
        case TYPE_C_MUX_1:
        case TYPE_C_MUX_2:
            /* USB Type-C Crossbar Switch initialization */
            ret += Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->Init(Context.MuxInfo[TypeCMuxId].I2C_Address_TypeCSwitch);

            /* DisplayPort Linear Redriver initialization */
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->Init(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver);

            if (TypeCMuxId == TYPE_C_MUX_1)
            {
                MUX_DET_Init(DET_USB3_DET);
                MUX_SEL_Init(SEL_HPDIN);
            }
            else
            {
                MUX_DET_Init(DET_HPD_SOURCE);
            }

            /* Initialize the debounce timer (if not done already) */
            if ((Context.MuxInfo[TYPE_C_MUX_1].State == MUX_NOT_INITIALIZED) &&
                    (Context.MuxInfo[TYPE_C_MUX_2].State == MUX_NOT_INITIALIZED))
            {
                MUX_DebounceTimerSetConfig(DEBOUNCE_TIME);
            }

            /* Update Context */
            Context.MuxInfo[TypeCMuxId].State = MUX_INITIALIZED;

            break;

        default:
            ret++;
            break;
        }
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Free the hardware resources used by the Type-C MUX
 *         assigned to a given Type-C MUX.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_DeInit(MUX_TypeCMuxIdTypeDef TypeCMuxId)
{
    uint32_t ret = 0;

    if (Context.MuxInfo[TypeCMuxId].State == MUX_INITIALIZED)
    {
        switch (TypeCMuxId)
        {
        case TYPE_C_MUX_1:
        case TYPE_C_MUX_2:
            /* USB Type-C Crossbar Switch de-initialization */
            Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->DeInit(Context.MuxInfo[TypeCMuxId].I2C_Address_TypeCSwitch);

            /* DisplayPort Linear Redriver de-initialization */
            Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DeInit(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver);

            if (TypeCMuxId == TYPE_C_MUX_1)
            {
                MUX_DET_DeInit(DET_USB3_DET);
                MUX_DebounceTimerResetConfig();
                MUX_SEL_DeInit(SEL_HPDIN);
            }
            else
            {
                MUX_DET_DeInit(DET_HPD_SOURCE);
            }

            /* Update Context */
            Context.MuxInfo[TypeCMuxId].State = MUX_NOT_INITIALIZED;

            /* De-initialize the debounce timer (if not required anymore)*/
            if ((Context.MuxInfo[TYPE_C_MUX_1].State == MUX_NOT_INITIALIZED) &&
                    (Context.MuxInfo[TYPE_C_MUX_2].State == MUX_NOT_INITIALIZED))
            {
                MUX_DebounceTimerResetConfig();
            }

            break;

        default:
            ret++;
            break;
        }
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Power on the Type-C MUX.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_Enable(MUX_TypeCMuxIdTypeDef TypeCMuxId)
{
    uint32_t ret = 0;

    switch (TypeCMuxId)
    {
    case TYPE_C_MUX_1:
    case TYPE_C_MUX_2:
        /* Power on USB Type-C Crossbar Switch */
        ret += Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->PowerOn(Context.MuxInfo[TypeCMuxId].I2C_Address_TypeCSwitch);

        /* Power on  DisplayPort Linear Redriver */
        ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->PowerOn(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver);

        break;

    default:
        ret++;
        break;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}
/**
 * @brief  Power down the Type-C MUX.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_Disable(MUX_TypeCMuxIdTypeDef TypeCMuxId)
{
    uint32_t ret = 0;

    switch (TypeCMuxId)
    {
    case TYPE_C_MUX_1:
    case TYPE_C_MUX_2:
        /* Power off USB Type-C Crossbar Switch */
        ret += Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->PowerOff(Context.MuxInfo[TypeCMuxId].I2C_Address_TypeCSwitch);

        /* Power down DisplayPort Linear Redriver */
        ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->PowerOff(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver);

        break;

    default:
        ret++;
        break;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Set the pin assignment of the USB Type-C connector when it operates
 *         in one of the following mode:
 *         DFP_D: Downstream Facing Port associated with a DisplayPort Source device
 *         UFP_D: Upstream Facing Port associated with a DisplayPort Source device
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  TypeCPlugOrientation Type-C plug orientation
 *         This parameter can be take one of the following values:
 *         @arg PLUG_ORIENTATION_NORMAL
 *         @arg PLUG_ORIENTATION_FLIPPED
 * @param  TypeCConnectorPinAssignment Type-C connector pin assignment
 *         This parameter can be take one of the following values:
 *         @arg DFP_PIN_ASSIGNMMENT_A
 *         @arg DFP_PIN_ASSIGNMMENT_B
 *         @arg DFP_PIN_ASSIGNMMENT_C
 *         @arg DFP_PIN_ASSIGNMMENT_D
 *         @arg DFP_PIN_ASSIGNMMENT_E
 *         @arg DFP_PIN_ASSIGNMMENT_F
 *         @arg UFP_PIN_ASSIGNMMENT_A
 *         @arg UFP_PIN_ASSIGNMMENT_B
 *         @arg UFP_PIN_ASSIGNMMENT_C
 *         @arg UFP_PIN_ASSIGNMMENT_D
 *         @arg UFP_PIN_ASSIGNMMENT_E
 *         @arg UFP_PIN_ASSIGNMMENT_F
 *         @arg USB_ONLY_PIN_ASSIGNMMENT
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_SetDPPinAssignment(MUX_TypeCMuxIdTypeDef                  TypeCMuxId,
        MUX_TypeCPlugOrientationTypeDef        TypeCPlugOrientation,
        MUX_TypeCConnectorPinAssignmentTypeDef TypeCConnectorPinAssignment)
{
    uint32_t ret = 0;
    TYPECSWITCH_Mode_t Mode;

    Mode = ModeSelect[TypeCPlugOrientation][TypeCConnectorPinAssignment];

    if (Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->IsSupportedMode(Mode))
    {
        /* Set Alt. mode */
        ret += Context.MuxInfo[TypeCMuxId].TypeCSwitch_Drv->SetMode(Context.MuxInfo[TypeCMuxId].I2C_Address_TypeCSwitch, Mode);

        /* Enable Display Port channels (if required) */
        switch(TypeCConnectorPinAssignment)
        {
        case USB_ONLY_PIN_ASSIGNMMENT:
            /* no DP channel enabled */
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP0);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP1);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP2);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP3);
            break;
        case DFP_PIN_ASSIGNMMENT_B:
        case DFP_PIN_ASSIGNMMENT_D:
        case DFP_PIN_ASSIGNMMENT_F:
        case UFP_PIN_ASSIGNMMENT_B:
        case UFP_PIN_ASSIGNMMENT_D:
        case UFP_PIN_ASSIGNMMENT_F:
            /* Enable Display Port ML0 and ML1  */
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP0);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP1);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP2);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->DisableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP3);
            break;
        case DFP_PIN_ASSIGNMMENT_A:
        case DFP_PIN_ASSIGNMMENT_C:
        case DFP_PIN_ASSIGNMMENT_E:
        case UFP_PIN_ASSIGNMMENT_A:
        case UFP_PIN_ASSIGNMMENT_C:
        case UFP_PIN_ASSIGNMMENT_E:
            /* Enable Display Port ML0, ML1, ML2 and ML3  */
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP0);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP1);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP2);
            ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->EnableChannel(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP3);
            break;
        }
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Set the equalizer gain for all the channels
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  EQGain Equalizer gain.
 *          TYPE_C_MUX_1: This parameter must be a value between 0x00 and @ref MUX_1_EQGAIN_MAX.
 *          TYPE_C_MUX_2: This parameter must be a value between 0x00 and @ref MUX_2_EQGAIN_MAX.
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_SetEQGain(MUX_TypeCMuxIdTypeDef                  TypeCMuxId,
        uint8_t                     EQGain)
{
  uint32_t ret = 1;

  if (((TYPE_C_MUX_1 == TypeCMuxId) && (MUX_1_EQGAIN_MAX >= EQGain))
   || ((TYPE_C_MUX_2 == TypeCMuxId) && (MUX_2_EQGAIN_MAX >= EQGain)))
  {
    ret = 0;
    /* Set EQ gain for Port ML0, ML1, ML2 and ML3  */
    ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->SetEQGain(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP0, EQGain);
    ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->SetEQGain(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP1, EQGain);
    ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->SetEQGain(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP2, EQGain);
    ret += Context.MuxInfo[TypeCMuxId].DPRedriver_Drv->SetEQGain(Context.MuxInfo[TypeCMuxId].I2C_Address_DPRedriver, CHANNEL_DP3, EQGain);
  }

  return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Set the HPD level seen by the DP source device.
 * @note This function is used to enable DisplayPort HPD signaling
 *       through PD messaging.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  HPDState Hot Plud Detection (HPD) state
 *         This parameter can be take one of the following values:
 *         @arg HPD_STATE_LOW
 *         @arg HPD_STATE_HIGH
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_SetHPDState(MUX_TypeCMuxIdTypeDef  TypeCMuxId,
                                      MUX_HPDStateTypeDef    HPDState)
{
    uint32_t ret = 0;

    switch (TypeCMuxId)
    {
    case TYPE_C_MUX_1:
        if(HPD_STATE_LOW == HPDState)
        {
            MUX_SEL_Off(SEL_HPDIN);
        }
        else
        {
            MUX_SEL_On(SEL_HPDIN);
        }
        break;

    default:
        ret++;
        break;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Retrieve actual HPD level of the DP source device.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  pHPDState pointer to the Hot Plud Detection (HPD) state
 *         This parameter can be take one of the following values:
 *         @arg HPD_STATE_LOW
 *         @arg HPD_STATE_HIGH
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_GetHPDState(MUX_TypeCMuxIdTypeDef  TypeCMuxId,
                                      MUX_HPDStateTypeDef *  pHPDState)
{
    uint32_t ret = 0;

    /* Initialize returned HPDState */
    *pHPDState = HPD_STATE_LOW;

    /* Read HPDState */
    switch (TypeCMuxId)
    {
    case TYPE_C_MUX_2:
        *pHPDState = (MUX_HPDStateTypeDef)MUX_DET_GetState(DET_HPD_SOURCE);
        break;

    default:
        ret++;
        break;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Retrieve actual USB3 connection state.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  pUSB3DetectState pointer to the USB3 connection state
 *         This parameter can be take one of the following values:
 *         @arg USB3_NOTCONNECTED
 *         @arg USB3_CONNECTED
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_GetUSB3DetectState(MUX_TypeCMuxIdTypeDef         TypeCMuxId,
        MUX_USB3DetectStateTypeDef *  pUSB3DetectState)
{
    uint32_t ret = 0;

    /* Initialize returned USB3DetectState */
    *pUSB3DetectState = USB3_NOT_CONNECTED;

    /* Read HPDState */
    switch (TypeCMuxId)
    {
    case TYPE_C_MUX_1:
        *pUSB3DetectState = (MUX_USB3DetectStateTypeDef)MUX_DET_GetState(DET_USB3_DET);
        break;

    default:
        ret++;
        break;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  Generate an HPD_IRQ towards the DP source device.
 * @note This function must be called every time an HPD_IRQ is detected by the
 *       PD communication stack.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_HPDIRQ(MUX_TypeCMuxIdTypeDef  TypeCMuxId)
{
    MUX_StatusTypeDef ret = MUX_ERROR;

    return ret;
}

/**
 * @brief  Display port hot plug detection (HPD)function.
 * @note This function must be called when a display bort capable device
 *       is connected to the Display Port source receptacle of the MB1352
 *       daughter board.
 * @retval none
 */
void BSP_MUX_Detect_HPD(void)
{
  if (Context.pfnHPDCallbackFunc != (MUX_HPDCallbackFuncTypeDef *)NULL)
  {
    HAL_TIM_Base_Start_IT(&htim);
  }
}

/**
 * @brief  USB3 detection function.
 * @note This function must be called when a USB3 capable device
 *       is connected to the USB3 receptacle of the MB1352 daughter board.
 * @retval none
 */
void BSP_MUX_Detect_USB3(void)
{
  if (Context.pfnUSB3DetectCallback != (MUX_USB3DetectCallbackFuncTypeDef *)NULL)
  {
    HAL_TIM_Base_Start_IT(&htim);
  }
}

/**
 * @brief  HPD callback function registration.
 * @note The sink device drives a hot plug detect (HPD) signal to notify the
 *       source that a sink is present.
 * @note Once registered, HPD callback function will be called upon connection
 *       disconnection of a DP Sink device.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  pHPDCallbackFunc HPD callback function pointer
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_RegisterHPDCallbackFunc(MUX_TypeCMuxIdTypeDef        TypeCMuxId,
        MUX_HPDCallbackFuncTypeDef * pHPDCallbackFunc)
{
    uint8_t ret = 0;

    if (TypeCMuxId == TYPE_C_MUX_2)
    {
        Context.pfnHPDCallbackFunc = pHPDCallbackFunc;
    }
    else
    {
        ret++;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
 * @brief  USB3 detection callback function registration.
 * @note   Connection/Disconnection detection of a USB host on the USB3 Type-B
 *         receptacle is based on the voltage level detected on the VBUS pin.
 * @param  TypeCMuxId Type-C MUX identifier
 *         This parameter can be take one of the following values:
 *         @arg TYPE_C_MUX_1
 *         @arg TYPE_C_MUX_2
 * @param  pUSB3DetectCallbackFunc USB3 detection callback function pointer
 * @retval mux status
 */
MUX_StatusTypeDef BSP_MUX_RegisterUSB3DetectCallbackFunc(MUX_TypeCMuxIdTypeDef               TypeCMuxId,
        MUX_USB3DetectCallbackFuncTypeDef * pUSB3DetectCallbackFunc)
{
    uint8_t ret = 0;

    if (TypeCMuxId == TYPE_C_MUX_1)
    {
        Context.pfnUSB3DetectCallback = pUSB3DetectCallbackFunc;
    }
    else
    {
        ret++;
    }

    return (ret == 0) ? MUX_OK : MUX_ERROR;
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM handle
  * @retval None
  */
static void TIMx_Base_MspInit(TIM_HandleTypeDef *htim)
{
    /* Enable Debounce Timer clock */
    DEBOUNCE_TIM_CLK_ENABLE();

    /* NVIC configuration for debounce timer interrupt */
    HAL_NVIC_SetPriority(DEBOUNCE_TIM_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DEBOUNCE_TIM_IRQn);
}

/**
  * @brief  DeInitialize TIM Base MSP.
  * @param  htim TIM handle
  * @retval None
  */
static void TIMx_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
    /* Enable Debounce Timer clock */
    DEBOUNCE_TIM_CLK_DISABLE();

    /* Disable Debounce timer interrupt */
    HAL_NVIC_DisableIRQ(DEBOUNCE_TIM_IRQn);
}

#if defined(TUSB546_DEBUG) || defined(CBTL08GP053_DEBUG) || defined(SN65DP141_DEBUG)
/**
 * @brief  Dump the register content of a device .
 * @param  Device device(s)
 *          This parameter can be any combination of the following values:
 *            DEVICE_CBTL08GP053
 *            DEVICE_SN65DP141
 *            DEVICE_TUSB546
 * @retval none
 */
void BSP_MUX_DumpDeviceRegisters(uint32_t Device)
{
#if defined(CBTL08GP053_DEBUG)
    if ((Device & DEVICE_CBTL08GP053) == DEVICE_CBTL08GP053)
    {
        printf("\n\tCBTL08GP053 registers dump:\n");
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_SYS_CTRL, &Context.CBTL08GP053_Registers.SysCtrl.Register);
        printf("\t\tSYS_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.SysCtrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_OP1_CTRL, &Context.CBTL08GP053_Registers.Op1Ctrl.Register);
        printf("\t\tOP1_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Op1Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_OP2_CTRL, &Context.CBTL08GP053_Registers.Op2Ctrl.Register);
        printf("\t\tOP2_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Op2Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_OP3_CTRL, &Context.CBTL08GP053_Registers.Op3Ctrl.Register);
        printf("\t\tOP3_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Op3Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_OP4_CTRL, &Context.CBTL08GP053_Registers.Op4Ctrl.Register);
        printf("\t\tOP4_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Op4Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_OP5_CTRL, &Context.CBTL08GP053_Registers.Op5Ctrl.Register);
        printf("\t\tOP5_CTRL   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Op5Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_CROSS5_CTRL, &Context.CBTL08GP053_Registers.Cross5Ctrl.Register);
        printf("\t\tCROSS5_CTRL: 0x%0.2x\n", Context.CBTL08GP053_Registers.Cross5Ctrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_SW_CTRL, &Context.CBTL08GP053_Registers.SwCtrl.Register);
        printf("\t\tSW_CTRL    : 0x%0.2x\n", Context.CBTL08GP053_Registers.SwCtrl.Register);
        MUX_IO_Read(MUX_2_TYPEC_SWITCH_I2C_ADDRESS, CBTL08GP053_REG_REVISION, &Context.CBTL08GP053_Registers.Revision);
        printf("\t\tREVISION   : 0x%0.2x\n", Context.CBTL08GP053_Registers.Revision);
    }
#endif /*  CBTL08GP053_DEBUG */

#if defined(SN65DP141_DEBUG)
    if ((Device & DEVICE_SN65DP141) == DEVICE_SN65DP141)
    {
        printf("\n\tSN65DP141 registers dump:\n");
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CFG, &Context.SN65DP141_Registers.General.Register);
        printf("\t\tGeneral Device Settings   : 0x%0.2x\n", Context.SN65DP141_Registers.General.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CHEN, &Context.SN65DP141_Registers.ChannelEnable.Register);
        printf("\t\tChannel Enable            : 0x%0.2x\n", Context.SN65DP141_Registers.ChannelEnable.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH0_CFG, &Context.SN65DP141_Registers.Channel0Ctrl.Register);
        printf("\t\tChannel 0 Control Settings: 0x%0.2x\n", Context.SN65DP141_Registers.Channel0Ctrl.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH0_EN, &Context.SN65DP141_Registers.Channel0Enable.Register);
        printf("\t\tChannel 0 Enable Settings : 0x%0.2x\n", Context.SN65DP141_Registers.Channel0Enable.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH1_CFG, &Context.SN65DP141_Registers.Channel1Ctrl.Register);
        printf("\t\tChannel 1 Control Settings: 0x%0.2x\n", Context.SN65DP141_Registers.Channel1Ctrl.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH1_EN, &Context.SN65DP141_Registers.Channel1Enable.Register);
        printf("\t\tChannel 1 Enable Settings : 0x%0.2x\n", Context.SN65DP141_Registers.Channel1Enable.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH2_CFG, &Context.SN65DP141_Registers.Channel2Ctrl.Register);
        printf("\t\tChannel 2 Control Settings: 0x%0.2x\n", Context.SN65DP141_Registers.Channel2Ctrl.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH2_EN, &Context.SN65DP141_Registers.Channel2Enable.Register);
        printf("\t\tChannel 2 Enable Settings : 0x%0.2x\n", Context.SN65DP141_Registers.Channel2Enable.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH3_CFG, &Context.SN65DP141_Registers.Channel3Ctrl.Register);
        printf("\t\tChannel 3 Control Settings: 0x%0.2x\n", Context.SN65DP141_Registers.Channel3Ctrl.Register);
        MUX_IO_Read(MUX_2_DP_REDRIVER_I2C_ADDRESS, SN65DP141_REG_CH3_EN, &Context.SN65DP141_Registers.Channel3Enable.Register);
        printf("\t\tChannel 3 Enable Settings : 0x%0.2x\n", Context.SN65DP141_Registers.Channel3Enable.Register);
    }
#endif /* SN65DP141_DEBUG */

#if defined(TUSB546_DEBUG)
    if ((Device & DEVICE_TUSB546) == DEVICE_TUSB546)
    {
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_CTRL, &Context.TUSB546_Registers.General.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_DP_CTRL1, &Context.TUSB546_Registers.DPCtrlStatus10.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_DP_CTRL2, &Context.TUSB546_Registers.DPCtrlStatus11.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_DP_CTRL3, &Context.TUSB546_Registers.DPCtrlStatus12.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_DP_CTRL4, &Context.TUSB546_Registers.DPCtrlStatus13.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_USB3_CTRL1, &Context.TUSB546_Registers.USBCtrlStatus20.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_USB3_CTRL2, &Context.TUSB546_Registers.USBCtrlStatus21.Register);
        MUX_IO_Read(MUX_1_TYPEC_SWITCH_I2C_ADDRESS, TUSB546_REG_USB3_CTRL3, &Context.TUSB546_Registers.USBCtrlStatus22.Register);
    }
#endif /* TUSB546_DEBUG */
}
#endif /* TUSB546_DEBUG || CBTL08GP053_DEBUG || SN65DP141_DEBUG */
/**
  * @}
  */


/** @addtogroup STM32G081B_EVAL_MUX_Private_Functions
  * @{
  */
/**
* @brief  Configures DET GPIO.
* @param  Det Specifies the Detection Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg DET_HPD_SOURCE
*     @arg DET_USB3_DET
* @retval None
*/
static void MUX_DET_Init(DetectId_TypeDef Det)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_DET clock */
    DETx_GPIO_CLK_ENABLE(Det);

    /* Configure the GPIO_MOS pin */
    GPIO_InitStruct.Pin = DET_PIN[Det];
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(DET_PORT[Det], &GPIO_InitStruct);

    /* Enable and set EXTI lines Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(DET_IRQn[Det], 2, 0);
    HAL_NVIC_EnableIRQ(DET_IRQn[Det]);
}

/**
* @brief  Reset DET GPIO configuration.
* @param  Det Specifies the Detection Pin .
*   This parameter can be one of following parameters:
*     @arg DET_HPD_SOURCE
*     @arg DET_USB3_DET
* @retval None
*/
static void MUX_DET_DeInit(DetectId_TypeDef Det)
{
    /* Disable EXTI lines Interrupt */
    HAL_NVIC_DisableIRQ(DET_IRQn[Det]);

    HAL_GPIO_DeInit(DET_PORT[Det], DET_PIN[Det]);
}

/**
* @brief  Returns the selected Detect state.
* @param  Det Pin to be checked.
*   This parameter can be one of following parameters:
*     @arg DET_HPD_SOURCE
*     @arg DET_USB3_DET
* @retval The Detect GPIO pin value
*/
static MUX_DETState_TypeDef MUX_DET_GetState(DetectId_TypeDef Det)
{
  GPIO_PinState PinState;

  PinState = HAL_GPIO_ReadPin(DET_PORT[Det], DET_PIN[Det]);

  if (DET_HPD_SOURCE == Det)
  {
    /* Update Context */
    Context.HPDState = HPD_STATE_LOW;
    if (GPIO_PIN_SET == PinState)
    {
      Context.HPDState = HPD_STATE_HIGH;
    }
  }
  else
  {
    /* Update Context */
    Context.USB3DetectState = USB3_NOT_CONNECTED;
    if (GPIO_PIN_SET == PinState)
    {
      Context.USB3DetectState = USB3_CONNECTED;
    }
  }

  return (PinState == GPIO_PIN_RESET) ? DET_STATE_LOW : DET_STATE_HIGH;
}

/**
* @brief  Configures SEL GPIO.
* @param  Sel Specifies the Select Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg SEL_HPDIN
* @retval None
*/
static void MUX_SEL_Init(SelectId_TypeDef Sel)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_DET clock */
    SELx_GPIO_CLK_ENABLE(Sel);

    /* Configure the GPIO_MOS pin */
    GPIO_InitStruct.Pin = SEL_PIN[Sel];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(SEL_PORT[Sel], &GPIO_InitStruct);

    HAL_GPIO_WritePin(SEL_PORT[Sel], SEL_PIN[Sel], GPIO_PIN_RESET);
}

/**
* @brief  Reset SEL GPIO configuration.
* @param  Sel Specifies the Select Pin.
*   This parameter can be one of following parameters:
*     @arg SEL_HPDIN
* @retval None
*/
static void MUX_SEL_DeInit(SelectId_TypeDef Sel)
{
    HAL_GPIO_DeInit(SEL_PORT[Sel], SEL_PIN[Sel]);
}

/**
* @brief  Turns selected Sel On.
* @param  Sel Specifies the Select Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg SEL_HPDIN
* @retval None
*/
static void MUX_SEL_On(SelectId_TypeDef Sel)
{
    HAL_GPIO_WritePin(SEL_PORT[Sel], SEL_PIN[Sel], GPIO_PIN_SET);
}

/**
* @brief  Turns selected Det Off.
* @param  Sel Specifies the Select Pin to be configured.
*   This parameter can be one of following parameters:
*     @arg SEL_HPDIN
* @retval None
*/
static void MUX_SEL_Off(SelectId_TypeDef Sel)
{
    HAL_GPIO_WritePin(SEL_PORT[Sel], SEL_PIN[Sel], GPIO_PIN_RESET);
}

/**
* @brief  Set debounce timer configuration.
* @param  DebounceTime Debounce time (in us)
* @retval None
*/
static void MUX_DebounceTimerSetConfig(uint32_t DebounceTime)
{
    __HAL_TIM_RESET_HANDLE_STATE(&htim);

    htim.Init.Prescaler         = (DEBOUNCE_TIM_COUNTER_CLK_FREQ() / 1000000) -1;
    htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim.Init.Period            = DebounceTime - 1;
    htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    TIMx_Base_MspInit(&htim);
    HAL_TIM_Base_Init(&htim);
    /* Clear update flag */
    __HAL_TIM_CLEAR_FLAG(&htim, TIM_FLAG_UPDATE);
}

/**
* @brief  Reset debounce timer configuration.
* @retval None
*/
static void MUX_DebounceTimerResetConfig(void)
{
  TIMx_Base_MspDeInit(&htim);
  HAL_TIM_Base_DeInit(&htim);
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_Base_Stop_IT(htim);

    if ((Context.USB3DetectState == USB3_NOT_CONNECTED) && (DET_STATE_HIGH == MUX_DET_GetState(DET_USB3_DET)))
    {
        /* Invoke registered callback function */
        Context.pfnUSB3DetectCallback(TYPE_C_MUX_1, USB3_CONNECTED);
    }

    if ((Context.USB3DetectState == USB3_CONNECTED) && (DET_STATE_LOW == MUX_DET_GetState(DET_USB3_DET)))
    {
        /* Invoke registered callback function */
        Context.pfnUSB3DetectCallback(TYPE_C_MUX_1, USB3_NOT_CONNECTED);
    }

    if ((Context.HPDState == HPD_STATE_LOW) && (DET_STATE_HIGH == MUX_DET_GetState(DET_HPD_SOURCE)))
    {
        /* Invoke registered callback function */
        Context.pfnHPDCallbackFunc(TYPE_C_MUX_2, HPD_STATE_HIGH);
    }

    if ((Context.HPDState == HPD_STATE_HIGH) && (DET_STATE_LOW == MUX_DET_GetState(DET_HPD_SOURCE)))
    {
        /* Invoke registered callback function */
        Context.pfnHPDCallbackFunc(TYPE_C_MUX_2, HPD_STATE_LOW);
    }
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
