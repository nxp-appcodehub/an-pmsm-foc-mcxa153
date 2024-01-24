/*
 * Copyright 2017-2018, 2021-2022 NXP.
 *  * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

void BOARD_InitBootClocks(void)
{
    BOARD_BootClockFRO192M();
}

void BOARD_BootClockFRO12M(void)
{
    CLOCK_SetupFRO12MClocking();
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 12000000U;
}

void BOARD_BootClockFRO24M(void)
{
    CLOCK_SetupFROHFClocking(48000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 2U);
    SystemCoreClock = 24000000U;
}

void BOARD_BootClockFRO36M(void)
{
    CLOCK_SetupFROHFClocking(36000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 36000000U;
}

void BOARD_BootClockFRO48M(void)
{
    CLOCK_SetupFROHFClocking(48000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 48000000U;
}

void BOARD_BootClockFRO64M(void)
{
    CLOCK_SetupFROHFClocking(64000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 64000000U;
}

void BOARD_BootClockFRO72M(void)
{
    CLOCK_SetupFROHFClocking(72000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 72000000U;
}

void BOARD_BootClockFRO96M(void)
{
    CLOCK_SetupFROHFClocking(96000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    SystemCoreClock = 96000000U;
}

void BOARD_BootClockFRO192M(void)
{
    CLOCK_SetupFROHFClocking(96000000U);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 2U);   
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetupFROHFClocking(192000000U);
    SystemCoreClock = 96000000U;
}
