/**************************************************************************//**
 * @file     disp_config.h
 * @brief    Sync-type LCD panel configuration file.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __DISP_CONFIG_H__
#define __DISP_CONFIG_H__

#include "NuMicro.h"
#include "component.h"

#define CONFIG_DISP_EBI               EBI_BANK0   /*!< SET EBI Bank */

#define CONFIG_DISP_VPW_ACTIVE_LOW            1   /*!< Enable VPW active low */
#define CONFIG_DISP_HPW_ACTIVE_LOW            1   /*!< Enable HPW active low */
#define CONFIG_DISP_DE_ACTIVE_LOW             0   /*!< Disable DE active low */
#define CONFIG_DISP_VSYNC_BITIDX              1   /*!< Implies SET_EBI_ADR0_PH7 */
#define CONFIG_DISP_HSYNC_BITIDX              2   /*!< Implies SET_EBI_ADR1_PH6 */
#define CONFIG_DISP_DE_BITIDX                 8   /*!< Implies SET_EBI_ADR7_PH0 */
#define CONFIG_TIMING_HACT                  480   /*!< Specify XRES */
#define CONFIG_TIMING_VACT                  272   /*!< Specify YRES */
#define CONFIG_TIMING_HBP                    30   /*!< Specify HBP (Horizontal Back Porch) */
#define CONFIG_TIMING_HFP                     5   /*!< Specify HFP (Horizontal Front Porch) */
#define CONFIG_TIMING_HPW                    41   /*!< Specify HPW (HSYNC plus width) */
#define CONFIG_TIMING_VBP                     2   /*!< Specify VBP (Vertical Back Porch) */
#define CONFIG_TIMING_VFP                    27   /*!< Specify VFP (Vertical Front Porch) */
#define CONFIG_TIMING_VPW                    10   /*!< Specify VPW (VSYNC width) */

#define PATH_IMAGE1_BIN        "..//WQVGA1.bin"   /*!< Specify image1 path */
#define PATH_IMAGE2_BIN        "..//WQVGA2.bin"   /*!< Specify image2 path */

/* Don't touch. */
#define CONFIG_DISP_VSYNC_BITMASK            (1<<CONFIG_DISP_VSYNC_BITIDX)   /*!< Bit mask for VSYNC */
#define CONFIG_DISP_HSYNC_BITMASK            (1<<CONFIG_DISP_HSYNC_BITIDX)   /*!< Bit mask for HSYNC */
#define CONFIG_DISP_DE_BITMASK               (1<<CONFIG_DISP_DE_BITIDX)      /*!< Bit mask for DE */
#define CONFIG_DISP_VSYNC_ACTIVE             (CONFIG_DISP_VPW_ACTIVE_LOW?(-1*CONFIG_DISP_VSYNC_BITMASK):CONFIG_DISP_VSYNC_BITMASK)   /*!< Active state for VSYNC */
#define CONFIG_DISP_HSYNC_ACTIVE             (CONFIG_DISP_HPW_ACTIVE_LOW?(-1*CONFIG_DISP_HSYNC_BITMASK):CONFIG_DISP_HSYNC_BITMASK)   /*!< Active state for HSYNC */
#define CONFIG_DISP_DE_ACTIVE                (CONFIG_DISP_DE_ACTIVE_LOW?(-1*CONFIG_DISP_DE_BITMASK):CONFIG_DISP_DE_BITMASK)      /*!< Active state for DE */
#define CONFIG_DISP_EBI_ADDR                 (EBI_BANK0_BASE_ADDR+(CONFIG_DISP_EBI*EBI_MAX_SIZE) + \
                                             (CONFIG_DISP_VPW_ACTIVE_LOW<<CONFIG_DISP_VSYNC_BITIDX) + \
                                             (CONFIG_DISP_HPW_ACTIVE_LOW<<CONFIG_DISP_HSYNC_BITIDX) + \
                                             (CONFIG_DISP_DE_ACTIVE_LOW<<CONFIG_DISP_DE_BITIDX))   /*!< EBI address configuration */

#define CONFIG_VRAM_BUF_SIZE                 (CONFIG_TIMING_HACT * CONFIG_TIMING_VACT * sizeof(uint16_t))   /*!< Size of VRAM buffer */
#define CONFIG_VRAM_BUF_NUM                   2   /*!< VRAM buffer number */
#define CONFIG_VRAM_TOTAL_ALLOCATED_SIZE      NVT_ALIGN((CONFIG_VRAM_BUF_NUM * CONFIG_VRAM_BUF_SIZE), DCACHE_LINE_SIZE) /*!< Total of VRAM buffer size */

typedef enum
{
    evHStageHFP,    /*!< Horizontal Front Porch stage */
    evHStageHSYNC,  /*!< Horizontal Sync stage */
    evHStageHBP,    /*!< Horizontal Back Porch stage */
    evHStageHACT,   /*!< Horizontal Active stage */
    evHStageCNT     /*!< Number of Horizontal stages */
} E_HSTAGE;

typedef enum
{
    evVStageVFP,    /*!< Vertical Front Porch stage */
    evVStageVSYNC,  /*!< Vertical Sync stage */
    evVStageVBP,    /*!< Vertical Back Porch stage */
    evVStageVACT,   /*!< Vertical Active stage */
    evVStageCNT     /*!< Number of Vertical stages */
} E_VSTAGE;

// Function to set the VRAM buffer address
void disp_set_vrambufaddr(void *pvBufAddr);

// Function to get the VRAM buffer address
void * disp_get_vrambufaddr(void);

// Function to set the blank callback function
typedef void(*DispBlankCb)(void *p);
void disp_set_blankcb(DispBlankCb f);

extern uint8_t g_au8FrameBuf[CONFIG_VRAM_TOTAL_ALLOCATED_SIZE];

#endif /* __DISP_CONFIG_H__ */
