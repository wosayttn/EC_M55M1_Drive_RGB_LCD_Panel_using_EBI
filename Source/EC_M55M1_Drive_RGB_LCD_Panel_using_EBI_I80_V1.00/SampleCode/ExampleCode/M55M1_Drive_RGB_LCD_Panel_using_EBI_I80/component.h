/**************************************************************************//**
 * @file     component.h
 * @version  V1.00
 * @brief    Related component helper functions
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2025 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __BOARD_H__
#define __BOARD_H__

/**
 * component_export
 *
 * @brief Component initilization data structure.
 *        Will export the data to 'CompInitTab' section in flash.
 *
 * @member name        system component name.
 * @member initialize  Necessary component or module initialization before executing.
 * @member finalize    Finalize component resources.
*/
struct component_export
{
    const char  *name;
    int (*initialize)(void);
    int (*finalize)(void);
};
typedef struct component_export *component_export_t;

/**
 * COMPONENT_EXPORT
 *
 * @brief Export component data structure to 'CompInitTab' section in flash.
 *        Used in application layer.
 *
 * @member name        system component name.
 * @member initialize  Necessary component or module initialization before executing.
 * @member finalize    Finalize component resources.
 *
 * @return None
 *
*/
#define COMPONENT_EXPORT(name, initialize, finalize)                           \
    __attribute__((used)) static const struct component_export _component      \
    __attribute__((section("CompInitTab"))) =                                  \
    {                                                                          \
        name,                                                                  \
        initialize,                                                            \
        finalize                                                               \
    }

#define NVT_ALIGN(size, align)        (((size) + (align) - 1) & ~((align) - 1))
#define NVT_ALIGN_DOWN(size, align)   ((size) & ~((align) - 1))

extern const int CompInitTab$$Base;
extern const int CompInitTab$$Limit;

#endif  // __BOARD_H__
