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

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function configures 12 MHz system clock.
 */
void BOARD_BootClockFRO12M(void);

/*!
 * @brief This function configures 24 MHz system clock.
 */
void BOARD_BootClockFRO24M(void);

/*!
 * @brief This function configures 36 MHz system clock.
 */
void BOARD_BootClockFRO36M(void);

/*!
 * @brief This function configures 48 MHz system clock.
 */
void BOARD_BootClockFRO48M(void);

/*!
 * @brief This function configures 64 MHz system clock.
 */
void BOARD_BootClockFRO64M(void);

/*!
 * @brief This function configures 72 MHz system clock.
 */
void BOARD_BootClockFRO72M(void);

/*!
 * @brief This function configures 96 MHz system clock.
 */
void BOARD_BootClockFRO96M(void);

/*!
 * @brief This function configures 192 MHz system clock.
 */
void BOARD_BootClockFRO192M(void);

#endif /* _CLOCK_CONFIG_H_ */
