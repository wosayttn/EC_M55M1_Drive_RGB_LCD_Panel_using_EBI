/**************************************************************************//**
 * @file     disp_sync_pdma.c
 * @brief    Use EBI-16 with PDMA-M2M to simulate sync-type LCD timing.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "pdma_lib.h"
#include "disp.h"
#include "nu_bitutil.h"

/*---------------------------------------------------------------------------*/
/* Define                                                                    */
/*---------------------------------------------------------------------------*/

// Structure representing the H stage descriptor
typedef struct
{
    DSCT_T m_dscH[evHStageCNT]; // Array of H stage descriptors
} S_DSC_HLINE;

// Structure representing the V stage descriptor
typedef struct
{
#if defined(CONFIG_LCD_PANEL_USE_DE_ONLY)
#define  DEF_TOTAL_VLINES   (CONFIG_TIMING_VACT)
    DSCT_T         m_dscDummy;
    S_DSC_HLINE    m_dscV[DEF_TOTAL_VLINES];
#else
#define  DEF_TOTAL_VLINES   (CONFIG_TIMING_VPW+CONFIG_TIMING_VBP+CONFIG_TIMING_VACT+CONFIG_TIMING_VFP)
    S_DSC_HLINE    m_dscV[DEF_TOTAL_VLINES];
#endif
} S_DSC_LCD;

/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
#if defined(NVT_NONCACHEABLE)
    NVT_NONCACHEABLE static S_DSC_LCD s_sDscLCD;
#else
    static S_DSC_LCD s_sDscLCD;
#endif

uint8_t g_au8FrameBuf[CONFIG_VRAM_TOTAL_ALLOCATED_SIZE] __attribute__((aligned(DCACHE_LINE_SIZE))); // Declare VRAM instance.
static uint32_t s_u32DummyData = 0xffffffff;
static nu_pdma_desc_t s_head = (nu_pdma_desc_t) &s_sDscLCD;
static nu_pdma_desc_t s_end = (nu_pdma_desc_t) &s_sDscLCD + (sizeof(s_sDscLCD) / sizeof(DSCT_T) - 1);
static volatile uint16_t *s_pu16BufAddr = NULL;
static DispBlankCb s_DispBlankCb = NULL;

// Array of H timing values
static const uint32_t s_au32HTiming[evHStageCNT] =
{
#define DEF_HACT_INDEX   (CONFIG_TIMING_HFP+CONFIG_TIMING_HPW+CONFIG_TIMING_HBP)
#if defined(CONFIG_LCD_PANEL_USE_DE_ONLY)
    DEF_HACT_INDEX,
    CONFIG_TIMING_HACT
#else
    CONFIG_TIMING_HFP,
    CONFIG_TIMING_HPW,
    CONFIG_TIMING_HBP,
    CONFIG_TIMING_HACT
#endif
#define DEF_HACT_ALL     (CONFIG_TIMING_HFP+CONFIG_TIMING_HPW+CONFIG_TIMING_HBP+CONFIG_TIMING_HACT)
};

// Array of V timing values
static const uint32_t s_au32VTiming[evVStageCNT] =
{
#if defined(CONFIG_LCD_PANEL_USE_DE_ONLY)
#define DEF_VACT_INDEX   (0)
    (CONFIG_TIMING_VFP + CONFIG_TIMING_VPW + CONFIG_TIMING_VBP),
    CONFIG_TIMING_VACT
#else
#define DEF_VACT_INDEX   (CONFIG_TIMING_VFP+CONFIG_TIMING_VPW+CONFIG_TIMING_VBP)
    CONFIG_TIMING_VFP,
    CONFIG_TIMING_VPW,
    CONFIG_TIMING_VBP,
    CONFIG_TIMING_VACT
#endif
#define DEF_VACT_ALL     (CONFIG_TIMING_VFP+CONFIG_TIMING_VPW+CONFIG_TIMING_VBP+CONFIG_TIMING_VACT)
};

static int s_i32Channel = -1;

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/
// Function to dump the PDMA descriptors
static void disp_pdma_dsc_dump(void)
{
    nu_pdma_desc_t next = s_head;

    printf("s_head: %08X, s_end: %08X\n", (uint32_t)s_head, (uint32_t)s_end);

    do
    {
        printf("next: %08X, CTL: %08X SA: %08X DA: %08X NEXT: %08X\n",
               (uint32_t)next, next->CTL, next->SA, next->DA, next->NEXT);

        next = (nu_pdma_desc_t)next->NEXT;

    } while (s_head != next);
}

// Function to get the current V stage based on the line index
static E_VSTAGE get_current_vstage(int i32LineIdx)
{
    int sum = 0;
    E_VSTAGE i;

    for (i = 0; i < evVStageCNT; i++)
    {
        sum += s_au32VTiming[i];

        if (i32LineIdx < sum)
        {
            return i;
        }
    }

    return 0;
}

// Function to initialize the PDMA descriptors
static void disp_pdma_dsc_init(void)
{
    int i;
    uint16_t *pu16Buf = (uint16_t *)s_pu16BufAddr;
    nu_pdma_desc_t next = s_head; // first descriptor.

#if defined(CONFIG_LCD_PANEL_USE_DE_ONLY)

    /* DE only */

    /* (CONFIG_TIMING_VFP+CONFIG_TIMING_VPW+CONFIG_TIMING_VBP) * (CONFIG_TIMING_HFP+CONFIG_TIMING_HPW+CONFIG_TIMING_HBP+CONFIG_TIMING_HACT) */
    nu_pdma_m2m_desc_setup(next,
                           16,
                           (uint32_t)&s_u32DummyData,
                           CONFIG_DISP_EBI_ADDR,
                           (CONFIG_TIMING_VFP + CONFIG_TIMING_VPW + CONFIG_TIMING_VBP) * (CONFIG_TIMING_HFP + CONFIG_TIMING_HPW + CONFIG_TIMING_HBP + CONFIG_TIMING_HACT),
                           eMemCtl_SrcFix_DstFix,
                           next + 1,
                           1);
    next++;

    for (i = 0; i < CONFIG_TIMING_VACT; i++)
    {
        /* Front descriptor */
        nu_pdma_m2m_desc_setup(next,
                               16,
                               (uint32_t)&s_u32DummyData,
                               CONFIG_DISP_EBI_ADDR,
                               (CONFIG_TIMING_HFP + CONFIG_TIMING_HPW + CONFIG_TIMING_HBP),
                               eMemCtl_SrcFix_DstFix,
                               next + 1,
                               1);
        next++;

        /* Backend descriptor */
        nu_pdma_m2m_desc_setup(next,
                               16,
                               (uint32_t)&pu16Buf[i * CONFIG_TIMING_HACT],
                               CONFIG_DISP_EBI_ADDR + CONFIG_DISP_DE_ACTIVE,
                               CONFIG_TIMING_HACT,
                               eMemCtl_SrcInc_DstFix,
                               next + 1,
                               1);
        next++;

    } // for(i = 0; i < CONFIG_TIMING_VACT; i++)

#else

    for (i = 0; i < DEF_TOTAL_VLINES; i++)
    {
        E_HSTAGE evH;
        E_VSTAGE evV = get_current_vstage(i);

        /* Set each VSYNC lines. */
        for (evH = 0; evH < evHStageCNT; evH++)
        {
            uint32_t u32AddrSrc = (uint32_t)&s_u32DummyData;
            uint32_t u32AddrDst;
            uint32_t u32DataWidth = 16;
            uint32_t u32XferCount = s_au32HTiming[evH];
            nu_pdma_memctrl_t evMemCtrl = eMemCtl_SrcFix_DstFix;

            switch (evV)
            {
                case evVStageVSYNC:
                    /* Set each H stage in a VSYNC line. */
                    /* Set source memory address is fixed and destination memory address is fixed. */
                    u32AddrDst = (evH == evHStageHSYNC) ? (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_VSYNC_ACTIVE + CONFIG_DISP_HSYNC_ACTIVE) :
                                 (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_VSYNC_ACTIVE);
                    break;

                case evVStageVBP:
                    /* Set each H stage in a VBP line. */
                    /* Set source memory address is fixed and destination memory address is fixed. */
                    u32AddrDst = (evH == evHStageHSYNC) ? (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_HSYNC_ACTIVE) : (CONFIG_DISP_EBI_ADDR);
                    break;

                case evVStageVACT:

                    /* Set each H stage in a VACT line. */
                    /* evHStageHACT stage: Set source memory address is incremented and destination memory address is fixed. */
                    /* Others stage: Set source memory address is fixed and destination memory address is fixed. */
                    if (evH == evHStageHACT)
                    {
                        u32AddrSrc = (uint32_t)pu16Buf;
                        pu16Buf = pu16Buf + CONFIG_TIMING_HACT;
                    }

                    u32AddrDst = (evH == evHStageHSYNC) ? (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_HSYNC_ACTIVE) :
                                 (evH == evHStageHACT) ? (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_DE_ACTIVE) :
                                 (CONFIG_DISP_EBI_ADDR);
                    evMemCtrl = (evH == evHStageHACT) ? eMemCtl_SrcInc_DstFix : eMemCtl_SrcFix_DstFix;
                    break;

                case evVStageVFP:
                    /* Set each H stage in a VFP line. */
                    /* Set source memory address is fixed and destination memory address is fixed. */
                    u32AddrDst = (evH == evHStageHSYNC) ? (CONFIG_DISP_EBI_ADDR + CONFIG_DISP_HSYNC_ACTIVE) : (CONFIG_DISP_EBI_ADDR);
                    break;

                default:
                    break;
            }

            nu_pdma_m2m_desc_setup(next,
                                   u32DataWidth,
                                   u32AddrSrc,
                                   u32AddrDst,
                                   u32XferCount,
                                   evMemCtrl,
                                   next + 1,
                                   1);
            next++;

        } // for (evH = 0; evH < evHStageCNT; evH++)

    } // for (i = 0; i < DEF_TOTAL_VLINES; i++)

#endif

    /* Update NEXT of last descriptor to link head. */
    s_end->NEXT = (uint32_t)s_head;

    /* Raise a blank-interrupt for switch data buffer if necessary. */
    s_end->CTL &= ~PDMA_DSCT_CTL_TBINTDIS_Msk;
}

// Callback function for PDMA transfer completion
static void nu_pdma_memfun_cb(void *pvUserData, uint32_t u32Events)
{
    if ((u32Events == NU_PDMA_EVENT_TRANSFER_DONE))
    {
        if (s_sDscLCD.m_dscV[DEF_VACT_INDEX].m_dscH[evHStageHACT].SA != (uint32_t)s_pu16BufAddr)
        {
            // Switch new VRAM buffer address.
            int i;

            for (i = 0; i < s_au32VTiming[evVStageVACT]; i++)
            {
                /* Update every lines. */
                s_sDscLCD.m_dscV[DEF_VACT_INDEX + i].m_dscH[evHStageHACT].SA = (uint32_t)&s_pu16BufAddr[i * CONFIG_TIMING_HACT];
            }
        }
        else
        {
        }

        if (s_DispBlankCb)
            s_DispBlankCb((void *)s_pu16BufAddr);
    }
}

// Function to initialize the PDMA module
static void pdma_init(void)
{
    uint32_t u32RegLocked = SYS_IsRegLocked();

    /* Unlock protected registers */
    if (u32RegLocked)
        SYS_UnlockReg();

    /* Enable PDMA0/PDMA1 clock source. */
    CLK_EnableModuleClock(PDMA0_MODULE);
    CLK_EnableModuleClock(PDMA1_MODULE);

    /* Reset PDMA0/PDMA1 modules. */
    SYS_ResetModule(SYS_PDMA0RST);
    SYS_ResetModule(SYS_PDMA1RST);

    /* Unlock protected registers */
    if (u32RegLocked)
        SYS_LockReg();
}

// Function to deinitialize the PDMA module
static void pdma_fini(void)
{
    uint32_t u32RegLocked = SYS_IsRegLocked();

    /* Unlock protected registers */
    if (u32RegLocked)
        SYS_UnlockReg();

    /* Reset PDMA0/PDMA1 modules. */
    SYS_ResetModule(SYS_PDMA0RST);
    SYS_ResetModule(SYS_PDMA1RST);

    /* Disable PDMA0/PDMA1 clock source. */
    CLK_DisableModuleClock(PDMA0_MODULE);
    CLK_DisableModuleClock(PDMA1_MODULE);

    /* Unlock protected registers */
    if (u32RegLocked)
        SYS_LockReg();
}

// Function to initialize the EBI sync PDMA
static int disp_sync_pdma_init(void)
{
    struct nu_pdma_chn_cb sChnCB;

    /* Set the VRAM address by default. */
    s_pu16BufAddr = (uint16_t *)g_au8FrameBuf;

    pdma_init();

    if (s_i32Channel < 0)
    {
        /* Allocate a PDMA channel resource. */
        s_i32Channel = nu_pdma_channel_allocate(PDMA_MEM);

        if (s_i32Channel < 0)
            return -1;
    }

    /* Initial all Lines descriptor-link. */
    disp_pdma_dsc_init();

    /* Dump all Lines descriptor-link. */
    // disp_pdma_dsc_dump();

    /* Register ISR callback function */
    sChnCB.m_eCBType = eCBType_Event;
    sChnCB.m_pfnCBHandler = nu_pdma_memfun_cb;
    sChnCB.m_pvUserData = (void *)NULL;

    nu_pdma_filtering_set(s_i32Channel, NU_PDMA_EVENT_TRANSFER_DONE);
    nu_pdma_callback_register(s_i32Channel, &sChnCB);

    /* Trigger scatter-gather transferring. */
    return nu_pdma_sg_transfer(s_i32Channel, s_head, 0);
}

// Function to deinitialize the EBI sync PDMA
static int disp_sync_pdma_fini(void)
{
    if (s_i32Channel >= 0)
    {
        /* Free allocated PDMA channel resource. */
        nu_pdma_channel_free(s_i32Channel);

        s_i32Channel = -1;
    }

    pdma_fini();

    return 0;
}

// Function to set the VRAM buffer address
void disp_set_vrambufaddr(void *pvBufAddr)
{
    s_pu16BufAddr = (volatile uint16_t *)pvBufAddr;
}

// Function to get the VRAM buffer address
void *disp_get_vrambufaddr(void)
{
    return (void *)s_pu16BufAddr;
}

// Function to set the blank callback function
void disp_set_blankcb(DispBlankCb f)
{
    s_DispBlankCb = f;
}

COMPONENT_EXPORT("DISP_SYNC_PDMA", disp_sync_pdma_init, disp_sync_pdma_fini);
