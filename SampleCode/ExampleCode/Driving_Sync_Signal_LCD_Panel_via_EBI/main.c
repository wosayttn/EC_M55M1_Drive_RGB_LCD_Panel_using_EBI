/**************************************************************************//**
 * @file     main.c
 * @brief    Drive RGB LCD Panel using EBI i80 for M55M1
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "NuMicro.h"
#include "component.h"
#include "board.h"

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/
/**
  * @brief Initializes major clocks and UART-console function.
  * @param[in]  None
  * @return     None
  * @details    Prevent using C runtime library functions (e.g. printf) or global variables in this function.
  */
void Reset_Handler_PreInit(void)
{
    /* Clock Setting is only available in secure mode */
    /* Enable the default APLL0 frequency and switch the SCLK clock source to APLL0. */
    CLK_SetBusClock(CLK_SCLKSEL_SCLKSEL_APLL0, CLK_APLLCTL_APLLSRC_HIRC, __HSI);

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Enable Debug Uart module clock */
    SetDebugUartCLK();

    /* Set Debug UART pins */
    SetDebugUartMFP();

    /* Initilize Debugging console */
    InitDebugUart();
}

/**
  * @brief Initializes these components.
  * @param[in]  None
  * @return     None
  */
static void components_initialize(void)
{
    int i;

    // Get the starting address of the component initialization table
    component_export_t asCompInitTbl = (component_export_t)&CompInitTab$$Base;

    // Calculate the number of components in the initialization table
    uint32_t u32CompInitNum = (component_export_t)&CompInitTab$$Limit - asCompInitTbl;

    // Iterate over all components and call their initialize function if defined
    for (i = 0; i < u32CompInitNum; i++)
    {
        // Check if the component has an initialize method
        if (asCompInitTbl[i].initialize)
        {
            // Print the component name being initialized
            printf("Initialize %s\n", asCompInitTbl[i].name);

            // Call the initialize method for component setup
            if (asCompInitTbl[i].initialize() < 0)
            {
                // Print message if return of initialize function is small than zero.
                printf("Initialize %s failure.\n", asCompInitTbl[i].name);
            }
        }
    }
}

/**
  * @brief      Finalizes these components.
  * @param[in]  None
  * @return     None
  */
static void components_finalize(void)
{
    int i;

    // Get the starting address of the component initialization table
    component_export_t asCompInitTbl = (component_export_t)&CompInitTab$$Base;

    // Calculate the number of components in the initialization table
    uint32_t u32CompInitNum = (component_export_t)&CompInitTab$$Limit - asCompInitTbl;

    // Iterate over all components and call their finalize function if defined
    for (i = 0; i < u32CompInitNum; i++)
    {
        // Check if the component has a finalize method
        if (asCompInitTbl[i].finalize)
        {
            // Print the component name being finalized
            printf("Finalize %s\n", asCompInitTbl[i].name);

            // Call the finalize method for cleanup
            if (asCompInitTbl[i].finalize() < 0)
            {
                // Print message if return of finalize function is small than zero.
                printf("Initialize %s failure.\n", asCompInitTbl[i].name);
            }
        }
    }
}

int main(void)
{
    // Module clocks and function pin setting initialization.
    board_init();

    /* Initialize all components */
    components_initialize();

    /* Placeholder for your code */
    while (1)
    {
        __WFI();  // Wait For Interrupt.
    }

    /* This will never execute due to the infinite loop. */
    /* Just keep for orthogonal implementation. */
    components_finalize();

    // Module clocks and function pin setting finalization.
    board_fini();

    return 0;
}
