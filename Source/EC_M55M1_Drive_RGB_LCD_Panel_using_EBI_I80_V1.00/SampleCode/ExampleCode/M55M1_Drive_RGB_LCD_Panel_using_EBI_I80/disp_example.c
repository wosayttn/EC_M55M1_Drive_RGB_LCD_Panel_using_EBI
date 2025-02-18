/**************************************************************************//**
 * @file     disp_example.c
 * @brief    Drive RGB LCD Panel example. Toggle two images displaying on
 *           sync-type LCD panel.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "NuMicro.h"
#include "disp.h"
#include "string.h"

/*---------------------------------------------------------------------------*/
/* Define                                                                    */
/*---------------------------------------------------------------------------*/

#define STR2(x) #x  // Convert the macro argument 'x' into a string literal.
#define STR(x) STR2(x)  // Call STR2 to ensure that the argument is converted into a string.
#define INCBIN(name, file) \
    __asm__(".section .rodata\n" \
            ".global incbin_" STR(name) "_start\n" \
            ".balign 16\n" \
            "incbin_" STR(name) "_start:\n" \
            ".incbin \"" file "\"\n" \
            \
            ".global incbin_" STR(name) "_end\n" \
            ".balign 1\n" \
            "incbin_" STR(name) "_end:\n" \
            ".byte 0\n" \
    ); \
    extern const __attribute__((aligned(32))) void* incbin_ ## name ## _start; \
    extern const void* incbin_ ## name ## _end; \

/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
INCBIN(image1, PATH_IMAGE1_BIN);  // Include binary data for image1 from the specified path.
INCBIN(image2, PATH_IMAGE2_BIN);  // Include binary data for image2 from the specified path.

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/

// Blank event callback function
void disp_example_blankcb(void *p)
{
    static uint32_t u32Counter = 0;

    /* Toggle different image showing after getting 2 event for avoid visual persistence ghosting. */
#define DEF_TOGGLE_COND    (u32Counter & 0x10u)

    /* Toggle between image1 and image2 display based on u32Counter's value. */
    if (DEF_TOGGLE_COND)
    {
        /* If the condition is true, set VRAM buffer to image2 buffer address. */
        disp_set_vrambufaddr((void *)&g_au8FrameBuf[CONFIG_VRAM_BUF_SIZE]);
    }
    else
    {
        /* If the condition is false, set VRAM buffer to image1 buffer address. */
        disp_set_vrambufaddr((void *)g_au8FrameBuf);
    }

    // Increment the counter to alternate the display in the next callback
    u32Counter++;
}


// Initialize the display example
static int disp_example_init(void)
{
    /* Set blank event callback function. */
    disp_set_blankcb(disp_example_blankcb);

    /* Copy image1 and image2 pixel data to VRAM buffer. */
    memcpy(g_au8FrameBuf, (const uint8_t *)&incbin_image1_start, CONFIG_VRAM_BUF_SIZE);
    memcpy(&g_au8FrameBuf[CONFIG_VRAM_BUF_SIZE], (const uint8_t *)&incbin_image2_start, CONFIG_VRAM_BUF_SIZE);

    /* Flush all pixel data in DCache to memory. */
    SCB_CleanDCache_by_Addr(g_au8FrameBuf, 2 * CONFIG_VRAM_BUF_SIZE);

    return 0;
}

// Finalize the display example
static int disp_example_fini(void)
{
    /* Reset blank event callback function. */
    disp_set_blankcb((void *)NULL);

    /* Reset VRAM buffer address. */
    disp_set_vrambufaddr((void *)NULL);

    return 0;
}

COMPONENT_EXPORT("DISP_EXAMPLE", disp_example_init, disp_example_fini);
