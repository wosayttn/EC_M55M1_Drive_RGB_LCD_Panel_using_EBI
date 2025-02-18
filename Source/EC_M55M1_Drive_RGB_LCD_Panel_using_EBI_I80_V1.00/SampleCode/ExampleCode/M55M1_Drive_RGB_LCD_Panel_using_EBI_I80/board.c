/**************************************************************************//**
 * @file     board.c
 * @brief    Module clocks initialization and function pin setting.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "NuMicro.h"
#include "component.h"
#include "disp.h"

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/

// Initialize EBI and GPIO modules
static void ebi_init(void)
{
    /* Enable EBI and GPIO modules clock and set pin multi-function. */
    CLK_EnableModuleClock(EBI0_MODULE);
    CLK_EnableModuleClock(GPIOA_MODULE);
    CLK_EnableModuleClock(GPIOC_MODULE);
    CLK_EnableModuleClock(GPIOD_MODULE);
    CLK_EnableModuleClock(GPIOE_MODULE);
    CLK_EnableModuleClock(GPIOH_MODULE);
    CLK_EnableModuleClock(GPIOJ_MODULE);

    // Set pin configurations for EBI address and data lines
    SET_EBI_AD0_PA5();
    SET_EBI_AD1_PA4();
    SET_EBI_AD2_PC2();
    SET_EBI_AD3_PC3();
    SET_EBI_AD4_PC4();
    SET_EBI_AD5_PC5();
    SET_EBI_AD6_PD8();
    SET_EBI_AD7_PD9();
    SET_EBI_AD8_PE14();
    SET_EBI_AD9_PE15();
    SET_EBI_AD10_PE1();
    SET_EBI_AD11_PE0();
    SET_EBI_AD12_PH8();
    SET_EBI_AD13_PH9();
    SET_EBI_AD14_PH10();
    SET_EBI_AD15_PH11();
    SET_EBI_nWR_PJ9();
    SET_EBI_nRD_PJ8();
    SET_EBI_nCS0_PD14();
    SET_EBI_ADR0_PH7();
    SET_EBI_ADR1_PH6();
    SET_EBI_ADR7_PH0();

    // Set slew rate to high for EBI pins
    GPIO_SetSlewCtl(PH, (BIT0 | BIT6 | BIT7 | BIT8 | BIT9 | BIT10 | BIT11), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PA, (BIT4 | BIT5), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PC, (BIT2 | BIT3 | BIT4 | BIT5), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PD, (BIT8 | BIT9), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PE, (BIT14 | BIT15), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PE, (BIT0 | BIT1), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PJ, (BIT8 | BIT9), GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PD, BIT14, GPIO_SLEWCTL_HIGH);

    // Open EBI with specified configuration
    EBI_Open(CONFIG_DISP_EBI, EBI_BUSWIDTH_16BIT, EBI_TIMING_FASTEST, EBI_OPMODE_CACCESS | EBI_OPMODE_ADSEPARATE, EBI_CS_ACTIVE_LOW);

    // Set bus timing for EBI
    EBI_SetBusTiming(CONFIG_DISP_EBI, 0, EBI_MCLKDIV_4);
}

// Deinitialize EBI and GPIO modules
static void ebi_fini(void)
{
    // Close EBI
    EBI_Close(CONFIG_DISP_EBI);

    // Reset pin configurations for EBI address and data lines
    SET_GPIO_PA5();
    SET_GPIO_PA4();
    SET_GPIO_PC2();
    SET_GPIO_PC3();
    SET_GPIO_PC4();
    SET_GPIO_PC5();
    SET_GPIO_PD8();
    SET_GPIO_PD9();
    SET_GPIO_PE14();
    SET_GPIO_PE15();
    SET_GPIO_PE1();
    SET_GPIO_PE0();
    SET_GPIO_PH8();
    SET_GPIO_PH9();
    SET_GPIO_PH10();
    SET_GPIO_PH11();
    SET_GPIO_PJ9();
    SET_GPIO_PJ8();
    SET_GPIO_PD14();
    SET_GPIO_PH7();
    SET_GPIO_PH6();
    SET_GPIO_PH0();

    // Set slew rate to normal for EBI pins
    GPIO_SetSlewCtl(PH, (BIT0 | BIT6 | BIT7 | BIT8 | BIT9 | BIT10 | BIT11), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PA, (BIT4 | BIT5), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PC, (BIT2 | BIT3 | BIT4 | BIT5), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PD, (BIT8 | BIT9), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PE, (BIT14 | BIT15), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PE, (BIT0 | BIT1), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PJ, (BIT8 | BIT9), GPIO_SLEWCTL_NORMAL);
    GPIO_SetSlewCtl(PD, BIT14, GPIO_SLEWCTL_NORMAL);

    // Disable EBI and GPIO modules clock
    CLK_DisableModuleClock(EBI0_MODULE);
    CLK_DisableModuleClock(GPIOA_MODULE);
    CLK_DisableModuleClock(GPIOC_MODULE);
    CLK_DisableModuleClock(GPIOD_MODULE);
    CLK_DisableModuleClock(GPIOE_MODULE);
    CLK_DisableModuleClock(GPIOH_MODULE);
    CLK_DisableModuleClock(GPIOJ_MODULE);
}

// Initialize board
static int board_init(void)
{
    // Enable EBI module clock and set EBI function pins
    ebi_init();

    return 0;
}

// Deinitialize board
static int board_fini(void)
{
    // Disable EBI module clock and reset EBI function pins
    ebi_fini();

    return 0;
}

COMPONENT_EXPORT("BOARD_MODULE", board_init, board_fini);
