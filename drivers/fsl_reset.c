/*
 * Copyright 2023, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_reset.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.reset"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define GET_REG_INDEX(x) (((uint32_t)(x)&0xFF00) >> 8)
#define GET_BIT_INDEX(x) ((uint32_t)(x)&0x00FF)

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(FSL_FEATURE_SOC_SYSCON_COUNT) && (FSL_FEATURE_SOC_SYSCON_COUNT > 0))

/*!
 * brief Assert reset to peripheral.
 *
 * Asserts reset signal to specified peripheral module.
 *
 * param peripheral Assert reset to this peripheral. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_SetPeripheralReset(reset_ip_name_t peripheral)
{
    uint32_t regIndex             = GET_REG_INDEX(peripheral);
    uint32_t bitPos               = GET_BIT_INDEX(peripheral);
    uint32_t bitMask              = 1UL << bitPos;
    volatile uint32_t *pResetCtrl = &(MRCC0->MRCC_GLB_RST0);

    if (peripheral == NotAvail_RSTn)
    {
        return;
    }

    assert(bitPos < 32u);
    assert(regIndex < 2u);

    /* Unlock clock configuration */
    SYSCON->CLKUNLOCK &= ~SYSCON_CLKUNLOCK_UNLOCK_MASK;

    /* reset register is in MRCC */
    /* set bit */
    if (regIndex == 0)
    {
        MRCC0->MRCC_GLB_RSTSET0 = bitMask;
        pResetCtrl              = &(MRCC0->MRCC_GLB_RST0);
    }
    else if (regIndex == 1)
    {
        MRCC0->MRCC_GLB_RSTSET1 = bitMask;
        pResetCtrl              = &(MRCC0->MRCC_GLB_RST1);
    }
    /* wait until it reads 0b1 */
    while (0u == ((*pResetCtrl) & bitMask))
    {
    }

    /* Freeze clock configuration */
    SYSCON->CLKUNLOCK |= SYSCON_CLKUNLOCK_UNLOCK_MASK;
}

/*!
 * brief Clear reset to peripheral.
 *
 * Clears reset signal to specified peripheral module, allows it to operate.
 *
 * param peripheral Clear reset to this peripheral. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_ClearPeripheralReset(reset_ip_name_t peripheral)
{
    uint32_t regIndex             = GET_REG_INDEX(peripheral);
    uint32_t bitPos               = GET_BIT_INDEX(peripheral);
    uint32_t bitMask              = 1UL << bitPos;
    volatile uint32_t *pResetCtrl = &(MRCC0->MRCC_GLB_RST0);

    assert(bitPos < 32u);

    /* Unlock clock configuration */
    SYSCON->CLKUNLOCK &= ~SYSCON_CLKUNLOCK_UNLOCK_MASK;

    /* reset register is in MRCC */
    /* clear bit */
    if (regIndex == 0)
    {
        MRCC0->MRCC_GLB_RSTCLR0 = bitMask;
        pResetCtrl              = &(MRCC0->MRCC_GLB_RST0);
    }
    else if (regIndex == 1)
    {
        MRCC0->MRCC_GLB_RSTCLR1 = bitMask;
        pResetCtrl              = &(MRCC0->MRCC_GLB_RST1);
    }
    /* wait until it reads 0b0 */
    while (bitMask == ((*pResetCtrl) & bitMask))
    {
    }

    /* Freeze clock configuration */
    SYSCON->CLKUNLOCK |= SYSCON_CLKUNLOCK_UNLOCK_MASK;
}

/*!
 * brief Reset peripheral module.
 *
 * Reset peripheral module.
 *
 * param peripheral Peripheral to reset. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_PeripheralReset(reset_ip_name_t peripheral)
{
    RESET_ClearPeripheralReset(peripheral);
    RESET_SetPeripheralReset(peripheral);
}

#endif /* FSL_FEATURE_SOC_SYSCON_COUNT || FSL_FEATURE_SOC_ASYNC_SYSCON_COUNT */
