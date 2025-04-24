/**************************************************************************//**
 * @file     board.h
 * @brief    Related MCU clock modules and pins initilization.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __BOARD_H__
#define __BOARD_H__

#include "NuMicro.h"

// Initialize board
void board_init(void);

// Deinitialize board
void board_fini(void);

#endif /* __BOARD_H__ */
