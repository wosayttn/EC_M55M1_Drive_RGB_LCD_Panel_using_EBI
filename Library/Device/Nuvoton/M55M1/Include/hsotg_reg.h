/**************************************************************************//**
 * @file     hsotg_reg.h
 * @version  V1.00
 * @brief    HSOTG register definition header file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#ifndef __HSOTG_REG_H__
#define __HSOTG_REG_H__

#if defined ( __CC_ARM   )
    #pragma anon_unions
#endif

/**
    @addtogroup REGISTER Control Register
    @{
*/

/**
    @addtogroup HSOTG High Speed USB 2.0 On-The-Go (HSOTG)
    Memory Mapped Structure for HSOTG Controller
    @{
*/

typedef struct
{
    /**
     * @var HSOTG_T::CTL
     * Offset: 0x00  HSOTG Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |VBUSDROP  |Drop VBUS Control
     * |        |          |If user application running on this OTG A-device wants to conserve power, set this bit to drop VBUS
     * |        |          |BUSREQ (HSOTG_CTL[1]) will be also cleared no matter A-device or B-device.
     * |        |          |0 = Not drop the VBUS.
     * |        |          |1 = Drop the VBUS.
     * |[1]     |BUSREQ    |OTG Bus Request
     * |        |          |If OTG A-device wants to do data transfers via USB bus, setting this bit will drive VBUS high to detect USB device connection
     * |        |          |If user won't use the bus any more, clearing this bit will drop VBUS to save power
     * |        |          |This bit will be cleared when A-device goes to A_wait_vfall state
     * |        |          |This bit will be also cleared if VBUSDROP (HSOTG_CTL[0]) bit is set or IDSTS (HSOTG_STATUS[1]) changed.
     * |        |          |If user of an OTG-B Device wants to request VBUS, setting this bit will run SRP protocol
     * |        |          |This bit will be cleared if SRP failure (OTG A-device does not provide VBUS after B-device issues SRP in specified interval, defined in OTG specification)
     * |        |          |This bit will be also cleared if VBUSDROP (HSOTG_CTL[0]) bit is set or IDSTS (HSOTG_STATUS[1]) changed.
     * |        |          |0 = Not launch VBUS in OTG A-device or not request SRP in OTG B-device.
     * |        |          |1 = Launch VBUS in OTG A-device or request SRP in OTG B-device.
     * |[2]     |HNPREQEN  |OTG HNP Request Enable Bit
     * |        |          |When the USB frame acts as A-device, set this bit when A-device allows to process HNP protocol -- A-device changes role from Host to Peripheral
     * |        |          |This bit will be cleared when OTG state changes from a_suspend to a_peripheral or goes back to a_idle state
     * |        |          |When the USB frame acts as B-device, set this bit after the OTG A-device successfully sends a SetFeature (b_hnp_enable) command to the OTG B-device to start role change -- B-device changes role from Peripheral to Host
     * |        |          |This bit will be cleared when OTG state changes from b_peripheral to b_wait_acon or goes back to b_idle state.
     * |        |          |0 = HNP request Disabled.
     * |        |          |1 = HNP request Enabled (A-device can change role from Host to Peripheral or B-device can change role from Peripheral to Host).
     * |        |          |Note: Refer to OTG specification to get a_suspend, a_peripheral, a_idle and b_idle state.
     * |[4]     |OTGEN     |OTG Function Enable Bit
     * |        |          |User needs to set this bit to enable OTG function while the USB frame configured as OTG device
     * |        |          |When the USB frame is not configured as OTG device, this bit is must be low.
     * |        |          |0 = OTG function Disabled.
     * |        |          |1 = OTG function Enabled.
     * |[5]     |WKEN      |OTG ID Pin Wake-up Enable Bit
     * |        |          |0 = OTG ID pin status change wake-up function Disabled.
     * |        |          |1 = OTG ID pin status change wake-up function Enabled.
     * @var HSOTG_T::PHYCTL
     * Offset: 0x04  HSOTG PHY Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |OTGPHYEN  |OTG PHY Enable Bit
     * |        |          |When the USB frame is configured as either OTG device or ID dependent, user needs to set this bit before using OTG function
     * |        |          |If device is configured as neither OTG device nor ID dependent, this bit is "don't care".
     * |        |          |0 = OTG PHY Disabled.
     * |        |          |1 = OTG PHY Enabled.
     * |[1]     |IDDETEN   |ID Detection Enable Bit
     * |        |          |0 = Detect ID pin status Disabled.
     * |        |          |1 = Detect ID pin status Enabled.
     * |[4]     |VBENPOL   |Off-chip USB VBUS Power Switch Enable Polarity
     * |        |          |The OTG controller will enable off-chip USB VBUS power switch to provide VBUS power when need
     * |        |          |A USB_VBUS_EN pin is used to control the off-chip USB VBUS power switch.
     * |        |          |The polarity of enabling off-chip USB VBUS power switch (high active or low active) depends on the selected component
     * |        |          |Set this bit as following according to the polarity of off-chip USB VBUS power switch.
     * |        |          |0 = The off-chip USB VBUS power switch enable is active high.
     * |        |          |1 = The off-chip USB VBUS power switch enable is active low.
     * |[5]     |VBSTSPOL  |Off-chip USB VBUS Power Switch Status Polarity
     * |        |          |The polarity of off-chip USB VBUS power switch valid signal depends on the selected component
     * |        |          |A USB_VBUS_ST pin is used to monitor the valid signal of the off-chip USB VBUS power switch
     * |        |          |Set this bit as following according to the polarity of off-chip USB VBUS power switch.
     * |        |          |0 = The polarity of off-chip USB VBUS power switch valid status is high.
     * |        |          |1 = The polarity of off-chip USB VBUS power switch valid status is low.
     * |[10:8]  |FSEL      |Reference Clock Frequency Select
     * |        |          |Selects OTG PHY reference clock frequency .
     * |        |          |000 = Reference clock is 19.2 MHz .
     * |        |          |001 = Reference clock is 20 MHz .
     * |        |          |010 = Reference clock is 24 MHz .
     * |        |          |011 = Reference clock is 16 MHz .
     * |        |          |100 = Reserved .
     * |        |          |101 = Reserved .
     * |        |          |110 = Reference clock is 26MHz .
     * |        |          |111 = Reference clock is 32 MHz .
     * |[12]    |OCPOL     |Over Current Polarity
     * |        |          |The polarity of off-chip USB VBUS power switch's overcurrent signal depends on the selected component
     * |        |          |A USB_VBUS_ST pin is used to monitor the signal of the off-chip USB VBUS power switch's overcurrent signal.Set this bit as following according to the polarity of off-chip USB VBUS power switch's overcurrent
     * |        |          |The overcurrent signal be used in USB HOST.
     * |        |          |0 = The polarity of off-chip USB VBUS power switch's overcurrent is low.
     * |        |          |1 = The polarity of off-chip USB VBUS power switch's overcurrent is high.
     * @var HSOTG_T::INTEN
     * Offset: 0x08  HSOTG Interrupt Enable Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ROLECHGIEN|Role Changed Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |        |          |Note: The role can be Host or Peripheral.
     * |[1]     |VBEIEN    |VBUS Error Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |        |          |Note: VBUS error means going to a_vbus_err state
     * |        |          |Please refer to A-device state diagram in OTG specification.
     * |[2]     |SRPFIEN   |SRP Fail Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[3]     |HNPFIEN   |HNP Fail Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[4]     |GOIDLEIEN |OTG Device Going to IDLE State Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |        |          |Note: Going to idle state means going to a_idle or b_idle state
     * |        |          |Please refer to A-device state diagram and B-device state diagram in OTG specification.
     * |[5]     |IDCHGIEN  |IDSTS Changed Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and IDSTS (HSOTG_STATUS[1]) status is changed from high to low or from low to high, an interrupt will be asserted.
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[6]     |PDEVIEN   |Act As Peripheral Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and the device is changed as a peripheral, an interrupt will be asserted.
     * |        |          |0 = This device as a peripheral interrupt Disabled.
     * |        |          |1 = This device as a peripheral interrupt Enabled.
     * |[7]     |HOSTIEN   |Act As Host Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and the device is changed as a host, an interrupt will be asserted.
     * |        |          |0 = This device as a host interrupt Disabled.
     * |        |          |1 = This device as a host interrupt Enabled.
     * |[8]     |BVLDCHGIEN|B-device Session Valid Status Changed Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and BVLD (HSOTG_STATUS[3]) status is changed from high to low or from low to high, an interrupt will be asserted.
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[9]     |AVLDCHGIEN|A-device Session Valid Status Changed Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and AVLD (HSOTG_STATUS[4]) status is changed from high to low or from low to high, an interrupt will be asserted.
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[10]    |VBCHGIEN  |VBUSVLD Status Changed Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and VBUSVLD (HSOTG_STATUS[5]) status is changed from high to low or from low to high, an interrupt will be asserted.
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[11]    |SECHGIEN  |SESSEND Status Changed Interrupt Enable Bit
     * |        |          |If this bit is set to 1 and SESSEND (HSOTG_STATUS[2]) status is changed from high to low or from low to high, an interrupt will be asserted.
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * |[13]    |SRPDETIEN |SRP Detected Interrupt Enable Bit
     * |        |          |0 = Interrupt Disabled.
     * |        |          |1 = Interrupt Enabled.
     * @var HSOTG_T::INTSTS
     * Offset: 0x0C  HSOTG Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |ROLECHGIF |OTG Role Change Interrupt Status
     * |        |          |This flag is set when the role of an OTG device changed from a host to a peripheral, or changed from a peripheral to a host while USB_ID pin status does not change.
     * |        |          |0 = OTG device role not changed.
     * |        |          |1 = OTG device role changed.
     * |        |          |Note: Write 1 to clear this flag.
     * |[1]     |VBEIF     |VBUS Error Interrupt Status
     * |        |          |This bit will be set when voltage on VBUS cannot reach a minimum valid threshold 4.4V within a maximum time of 100ms after OTG A-device starting to drive VBUS high.
     * |        |          |0 = OTG A-device drives VBUS over threshold voltage before this interval expires.
     * |        |          |1 = OTG A-device cannot drive VBUS over threshold voltage before this interval expires.
     * |        |          |Note: Write 1 to clear this flag and recover from the VBUS error state.
     * |[2]     |SRPFIF    |SRP Fail Interrupt Status
     * |        |          |After initiating SRP, an OTG B-device will wait for the OTG A-device to drive VBUS high at least TB_SRP_FAIL minimum, defined in OTG specification
     * |        |          |This flag is set when the OTG B-device does not get VBUS high after this interval.
     * |        |          |0 = OTG B-device gets VBUS high before this interval.
     * |        |          |1 = OTG B-device does not get VBUS high before this interval.
     * |        |          |Note: Write 1 to clear this flag.
     * |[3]     |HNPFIF    |HNP Fail Interrupt Status
     * |        |          |When A-device has granted B-device to be host and USB bus is in SE0 (both HSUSB_D+ and HSUSB_D- low) state, this bit will be set when A-device does not connect after specified interval expires.
     * |        |          |0 = A-device connects to B-device before specified interval expires.
     * |        |          |1 = A-device does not connect to B-device before specified interval expires.
     * |        |          |Note: Write 1 to clear this flag.
     * |[4]     |GOIDLEIF  |OTG Device Goes to IDLE Interrupt Status
     * |        |          |Flag is set if the OTG device transfers from non-idle state to idle state
     * |        |          |The OTG device will be neither a host nor a peripheral.
     * |        |          |0 = OTG device does not go back to idle state (a_idle or b_idle).
     * |        |          |1 = OTG device goes back to idle state(a_idle or b_idle).
     * |        |          |Note 1: Going to idle state means going to a_idle or b_idle state. Please refer to OTG specification.
     * |        |          |Note 2: Write 1 to clear this flag.
     * |[5]     |IDCHGIF   |ID State Change Interrupt Status
     * |        |          |0 = IDSTS (HSOTG_STATUS[1]) not toggled.
     * |        |          |1 = IDSTS (HSOTG_STATUS[1]) from high to low or from low to high.
     * |        |          |Note: Write 1 to clear this flag.
     * |[6]     |PDEVIF    |Act As Peripheral Interrupt Status
     * |        |          |0= This device does not act as a peripheral.
     * |        |          |1 = This device acts as a peripheral.
     * |        |          |Note: Write 1 to clear this flag.
     * |[7]     |HOSTIF    |Act As Host Interrupt Status
     * |        |          |0 = This device does not act as a host.
     * |        |          |1 = This device acts as a host.
     * |        |          |Note: Write 1 to clear this flag.
     * |[8]     |BVLDCHGIF |B-device Session Valid State Change Interrupt Status
     * |        |          |0 = BVLD (HSOTG_STATUS[3]) not toggled.
     * |        |          |1 = BVLD (HSOTG_STATUS[3]) from high to low or low to high.
     * |        |          |Note: Write 1 to clear this status.
     * |[9]     |AVLDCHGIF |A-device Session Valid State Change Interrupt Status
     * |        |          |0 = AVLD (HSOTG_STATUS[4]) not toggled.
     * |        |          |1 = AVLD (HSOTG_STATUS[4]) from high to low or low to high.
     * |        |          |Note: Write 1 to clear this status.
     * |[10]    |VBCHGIF   |VBUSVLD State Change Interrupt Status
     * |        |          |0 = VBUSVLD (HSOTG_STATUS[5]) not toggled.
     * |        |          |1 = VBUSVLD (HSOTG_STATUS[5]) from high to low or from low to high.
     * |        |          |Note: Write 1 to clear this status.
     * |[11]    |SECHGIF   |SESSEND State Change Interrupt Status
     * |        |          |0 = SESSEND (HSOTG_STATUS[2]) not toggled.
     * |        |          |1 = SESSEND (HSOTG_STATUS[2]) from high to low or from low to high.
     * |        |          |Note: Write 1 to clear this flag.
     * |[13]    |SRPDETIF  |SRP Detected Interrupt Status
     * |        |          |0 = SRP not detected.
     * |        |          |1 = SRP detected.
     * |        |          |Note: Write 1 to clear this status.
     * @var HSOTG_T::STATUS
     * Offset: 0x10  HSOTG Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |OVERCUR   |Overcurrent Condition
     * |        |          |The voltage on VBUS cannot reach a minimum VBUS valid threshold, 4.4V minimum, within a maximum time of 100ms after OTG A-device drives VBUS high.
     * |        |          |0 = OTG A-device drives VBUS successfully.
     * |        |          |1 = OTG A-device cannot drive VBUS high in this interval.
     * |[1]     |IDSTS     |USB_ID Pin State of Mini-/Micro-Plug
     * |        |          |0 = Mini-A/Micro-A plug is attached.
     * |        |          |1 = Mini-B/Micro-B plug is attached.
     * |[2]     |SESSEND   |Session End Status
     * |        |          |When VBUS voltage is lower than 0.4V, this bit will be set to 1
     * |        |          |Session end means no meaningful power on VBUS.
     * |        |          |0 = Session is not end.
     * |        |          |1 = Session is end.
     * |[3]     |BVLD      |B-device Session Valid Status
     * |        |          |0 = B-device session is not valid.
     * |        |          |1 = B-device session is valid.
     * |[4]     |AVLD      |A-Device Session Valid Status
     * |        |          |0 = A-device session is not valid.
     * |        |          |1 = A-device session is valid.
     * |[5]     |VBUSVLD   |VBUS Valid Status
     * |        |          |When VBUS is larger than 4.7V, this bit will be set to 1.
     * |        |          |0 = VBUS is not valid.
     * |        |          |1 = VBUS is valid.
     * |[6]     |ASPERI    |As Peripheral Status
     * |        |          |When OTG acts as peripheral, this bit is set.
     * |        |          |0 = OTG not as peripheral.
     * |        |          |1 = OTG as peripheral.
     * |[7]     |ASHOST    |As Host Status
     * |        |          |When OTG acts as Host, this bit is set.
     * |        |          |0 = OTG not as Host.
     * |        |          |1 = OTG as Host.
     */
    __IO uint32_t CTL;                   /*!< [0x0000] HSOTG Control Register                                           */
    __IO uint32_t PHYCTL;                /*!< [0x0004] HSOTG PHY Control Register                                       */
    __IO uint32_t INTEN;                 /*!< [0x0008] HSOTG Interrupt Enable Register                                  */
    __IO uint32_t INTSTS;                /*!< [0x000c] HSOTG Interrupt Status Register                                  */
    __I  uint32_t STATUS;                /*!< [0x0010] HSOTG Status Register                                            */

} HSOTG_T;

/**
    @addtogroup HSOTG_CONST HSOTG Bit Field Definition
    Constant Definitions for HSOTG Controller
    @{
*/

#define HSOTG_CTL_VBUSDROP_Pos           (0)                                               /*!< HSOTG_T::CTL: VBUSDROP Position        */
#define HSOTG_CTL_VBUSDROP_Msk           (0x1ul << HSOTG_CTL_VBUSDROP_Pos)                 /*!< HSOTG_T::CTL: VBUSDROP Mask            */

#define HSOTG_CTL_BUSREQ_Pos             (1)                                               /*!< HSOTG_T::CTL: BUSREQ Position          */
#define HSOTG_CTL_BUSREQ_Msk             (0x1ul << HSOTG_CTL_BUSREQ_Pos)                   /*!< HSOTG_T::CTL: BUSREQ Mask              */

#define HSOTG_CTL_HNPREQEN_Pos           (2)                                               /*!< HSOTG_T::CTL: HNPREQEN Position        */
#define HSOTG_CTL_HNPREQEN_Msk           (0x1ul << HSOTG_CTL_HNPREQEN_Pos)                 /*!< HSOTG_T::CTL: HNPREQEN Mask            */

#define HSOTG_CTL_OTGEN_Pos              (4)                                               /*!< HSOTG_T::CTL: OTGEN Position           */
#define HSOTG_CTL_OTGEN_Msk              (0x1ul << HSOTG_CTL_OTGEN_Pos)                    /*!< HSOTG_T::CTL: OTGEN Mask               */

#define HSOTG_CTL_WKEN_Pos               (5)                                               /*!< HSOTG_T::CTL: WKEN Position            */
#define HSOTG_CTL_WKEN_Msk               (0x1ul << HSOTG_CTL_WKEN_Pos)                     /*!< HSOTG_T::CTL: WKEN Mask                */

#define HSOTG_PHYCTL_OTGPHYEN_Pos        (0)                                               /*!< HSOTG_T::PHYCTL: OTGPHYEN Position     */
#define HSOTG_PHYCTL_OTGPHYEN_Msk        (0x1ul << HSOTG_PHYCTL_OTGPHYEN_Pos)              /*!< HSOTG_T::PHYCTL: OTGPHYEN Mask         */

#define HSOTG_PHYCTL_IDDETEN_Pos         (1)                                               /*!< HSOTG_T::PHYCTL: IDDETEN Position      */
#define HSOTG_PHYCTL_IDDETEN_Msk         (0x1ul << HSOTG_PHYCTL_IDDETEN_Pos)               /*!< HSOTG_T::PHYCTL: IDDETEN Mask          */

#define HSOTG_PHYCTL_VBENPOL_Pos         (4)                                               /*!< HSOTG_T::PHYCTL: VBENPOL Position      */
#define HSOTG_PHYCTL_VBENPOL_Msk         (0x1ul << HSOTG_PHYCTL_VBENPOL_Pos)               /*!< HSOTG_T::PHYCTL: VBENPOL Mask          */

#define HSOTG_PHYCTL_VBSTSPOL_Pos        (5)                                               /*!< HSOTG_T::PHYCTL: VBSTSPOL Position     */
#define HSOTG_PHYCTL_VBSTSPOL_Msk        (0x1ul << HSOTG_PHYCTL_VBSTSPOL_Pos)              /*!< HSOTG_T::PHYCTL: VBSTSPOL Mask         */

#define HSOTG_PHYCTL_FSEL_Pos            (8)                                               /*!< HSOTG_T::PHYCTL: FSEL Position         */
#define HSOTG_PHYCTL_FSEL_Msk            (0x7ul << HSOTG_PHYCTL_FSEL_Pos)                  /*!< HSOTG_T::PHYCTL: FSEL Mask             */

#define HSOTG_PHYCTL_OCPOL_Pos           (12)                                              /*!< HSOTG_T::PHYCTL: OCPOL Position        */
#define HSOTG_PHYCTL_OCPOL_Msk           (0x1ul << HSOTG_PHYCTL_OCPOL_Pos)                 /*!< HSOTG_T::PHYCTL: OCPOL Mask            */

#define HSOTG_INTEN_ROLECHGIEN_Pos       (0)                                               /*!< HSOTG_T::INTEN: ROLECHGIEN Position    */
#define HSOTG_INTEN_ROLECHGIEN_Msk       (0x1ul << HSOTG_INTEN_ROLECHGIEN_Pos)             /*!< HSOTG_T::INTEN: ROLECHGIEN Mask        */

#define HSOTG_INTEN_VBEIEN_Pos           (1)                                               /*!< HSOTG_T::INTEN: VBEIEN Position        */
#define HSOTG_INTEN_VBEIEN_Msk           (0x1ul << HSOTG_INTEN_VBEIEN_Pos)                 /*!< HSOTG_T::INTEN: VBEIEN Mask            */

#define HSOTG_INTEN_SRPFIEN_Pos          (2)                                               /*!< HSOTG_T::INTEN: SRPFIEN Position       */
#define HSOTG_INTEN_SRPFIEN_Msk          (0x1ul << HSOTG_INTEN_SRPFIEN_Pos)                /*!< HSOTG_T::INTEN: SRPFIEN Mask           */

#define HSOTG_INTEN_HNPFIEN_Pos          (3)                                               /*!< HSOTG_T::INTEN: HNPFIEN Position       */
#define HSOTG_INTEN_HNPFIEN_Msk          (0x1ul << HSOTG_INTEN_HNPFIEN_Pos)                /*!< HSOTG_T::INTEN: HNPFIEN Mask           */

#define HSOTG_INTEN_GOIDLEIEN_Pos        (4)                                               /*!< HSOTG_T::INTEN: GOIDLEIEN Position     */
#define HSOTG_INTEN_GOIDLEIEN_Msk        (0x1ul << HSOTG_INTEN_GOIDLEIEN_Pos)              /*!< HSOTG_T::INTEN: GOIDLEIEN Mask         */

#define HSOTG_INTEN_IDCHGIEN_Pos         (5)                                               /*!< HSOTG_T::INTEN: IDCHGIEN Position      */
#define HSOTG_INTEN_IDCHGIEN_Msk         (0x1ul << HSOTG_INTEN_IDCHGIEN_Pos)               /*!< HSOTG_T::INTEN: IDCHGIEN Mask          */

#define HSOTG_INTEN_PDEVIEN_Pos          (6)                                               /*!< HSOTG_T::INTEN: PDEVIEN Position       */
#define HSOTG_INTEN_PDEVIEN_Msk          (0x1ul << HSOTG_INTEN_PDEVIEN_Pos)                /*!< HSOTG_T::INTEN: PDEVIEN Mask           */

#define HSOTG_INTEN_HOSTIEN_Pos          (7)                                               /*!< HSOTG_T::INTEN: HOSTIEN Position       */
#define HSOTG_INTEN_HOSTIEN_Msk          (0x1ul << HSOTG_INTEN_HOSTIEN_Pos)                /*!< HSOTG_T::INTEN: HOSTIEN Mask           */

#define HSOTG_INTEN_BVLDCHGIEN_Pos       (8)                                               /*!< HSOTG_T::INTEN: BVLDCHGIEN Position    */
#define HSOTG_INTEN_BVLDCHGIEN_Msk       (0x1ul << HSOTG_INTEN_BVLDCHGIEN_Pos)             /*!< HSOTG_T::INTEN: BVLDCHGIEN Mask        */

#define HSOTG_INTEN_AVLDCHGIEN_Pos       (9)                                               /*!< HSOTG_T::INTEN: AVLDCHGIEN Position    */
#define HSOTG_INTEN_AVLDCHGIEN_Msk       (0x1ul << HSOTG_INTEN_AVLDCHGIEN_Pos)             /*!< HSOTG_T::INTEN: AVLDCHGIEN Mask        */

#define HSOTG_INTEN_VBCHGIEN_Pos         (10)                                              /*!< HSOTG_T::INTEN: VBCHGIEN Position      */
#define HSOTG_INTEN_VBCHGIEN_Msk         (0x1ul << HSOTG_INTEN_VBCHGIEN_Pos)               /*!< HSOTG_T::INTEN: VBCHGIEN Mask          */

#define HSOTG_INTEN_SECHGIEN_Pos         (11)                                              /*!< HSOTG_T::INTEN: SECHGIEN Position      */
#define HSOTG_INTEN_SECHGIEN_Msk         (0x1ul << HSOTG_INTEN_SECHGIEN_Pos)               /*!< HSOTG_T::INTEN: SECHGIEN Mask          */

#define HSOTG_INTEN_SRPDETIEN_Pos        (13)                                              /*!< HSOTG_T::INTEN: SRPDETIEN Position     */
#define HSOTG_INTEN_SRPDETIEN_Msk        (0x1ul << HSOTG_INTEN_SRPDETIEN_Pos)              /*!< HSOTG_T::INTEN: SRPDETIEN Mask         */

#define HSOTG_INTSTS_ROLECHGIF_Pos       (0)                                               /*!< HSOTG_T::INTSTS: ROLECHGIF Position    */
#define HSOTG_INTSTS_ROLECHGIF_Msk       (0x1ul << HSOTG_INTSTS_ROLECHGIF_Pos)             /*!< HSOTG_T::INTSTS: ROLECHGIF Mask        */

#define HSOTG_INTSTS_VBEIF_Pos           (1)                                               /*!< HSOTG_T::INTSTS: VBEIF Position        */
#define HSOTG_INTSTS_VBEIF_Msk           (0x1ul << HSOTG_INTSTS_VBEIF_Pos)                 /*!< HSOTG_T::INTSTS: VBEIF Mask            */

#define HSOTG_INTSTS_SRPFIF_Pos          (2)                                               /*!< HSOTG_T::INTSTS: SRPFIF Position       */
#define HSOTG_INTSTS_SRPFIF_Msk          (0x1ul << HSOTG_INTSTS_SRPFIF_Pos)                /*!< HSOTG_T::INTSTS: SRPFIF Mask           */

#define HSOTG_INTSTS_HNPFIF_Pos          (3)                                               /*!< HSOTG_T::INTSTS: HNPFIF Position       */
#define HSOTG_INTSTS_HNPFIF_Msk          (0x1ul << HSOTG_INTSTS_HNPFIF_Pos)                /*!< HSOTG_T::INTSTS: HNPFIF Mask           */

#define HSOTG_INTSTS_GOIDLEIF_Pos        (4)                                               /*!< HSOTG_T::INTSTS: GOIDLEIF Position     */
#define HSOTG_INTSTS_GOIDLEIF_Msk        (0x1ul << HSOTG_INTSTS_GOIDLEIF_Pos)              /*!< HSOTG_T::INTSTS: GOIDLEIF Mask         */

#define HSOTG_INTSTS_IDCHGIF_Pos         (5)                                               /*!< HSOTG_T::INTSTS: IDCHGIF Position      */
#define HSOTG_INTSTS_IDCHGIF_Msk         (0x1ul << HSOTG_INTSTS_IDCHGIF_Pos)               /*!< HSOTG_T::INTSTS: IDCHGIF Mask          */

#define HSOTG_INTSTS_PDEVIF_Pos          (6)                                               /*!< HSOTG_T::INTSTS: PDEVIF Position       */
#define HSOTG_INTSTS_PDEVIF_Msk          (0x1ul << HSOTG_INTSTS_PDEVIF_Pos)                /*!< HSOTG_T::INTSTS: PDEVIF Mask           */

#define HSOTG_INTSTS_HOSTIF_Pos          (7)                                               /*!< HSOTG_T::INTSTS: HOSTIF Position       */
#define HSOTG_INTSTS_HOSTIF_Msk          (0x1ul << HSOTG_INTSTS_HOSTIF_Pos)                /*!< HSOTG_T::INTSTS: HOSTIF Mask           */

#define HSOTG_INTSTS_BVLDCHGIF_Pos       (8)                                               /*!< HSOTG_T::INTSTS: BVLDCHGIF Position    */
#define HSOTG_INTSTS_BVLDCHGIF_Msk       (0x1ul << HSOTG_INTSTS_BVLDCHGIF_Pos)             /*!< HSOTG_T::INTSTS: BVLDCHGIF Mask        */

#define HSOTG_INTSTS_AVLDCHGIF_Pos       (9)                                               /*!< HSOTG_T::INTSTS: AVLDCHGIF Position    */
#define HSOTG_INTSTS_AVLDCHGIF_Msk       (0x1ul << HSOTG_INTSTS_AVLDCHGIF_Pos)             /*!< HSOTG_T::INTSTS: AVLDCHGIF Mask        */

#define HSOTG_INTSTS_VBCHGIF_Pos         (10)                                              /*!< HSOTG_T::INTSTS: VBCHGIF Position      */
#define HSOTG_INTSTS_VBCHGIF_Msk         (0x1ul << HSOTG_INTSTS_VBCHGIF_Pos)               /*!< HSOTG_T::INTSTS: VBCHGIF Mask          */

#define HSOTG_INTSTS_SECHGIF_Pos         (11)                                              /*!< HSOTG_T::INTSTS: SECHGIF Position      */
#define HSOTG_INTSTS_SECHGIF_Msk         (0x1ul << HSOTG_INTSTS_SECHGIF_Pos)               /*!< HSOTG_T::INTSTS: SECHGIF Mask          */

#define HSOTG_INTSTS_SRPDETIF_Pos        (13)                                              /*!< HSOTG_T::INTSTS: SRPDETIF Position     */
#define HSOTG_INTSTS_SRPDETIF_Msk        (0x1ul << HSOTG_INTSTS_SRPDETIF_Pos)              /*!< HSOTG_T::INTSTS: SRPDETIF Mask         */

#define HSOTG_STATUS_OVERCUR_Pos         (0)                                               /*!< HSOTG_T::STATUS: OVERCUR Position      */
#define HSOTG_STATUS_OVERCUR_Msk         (0x1ul << HSOTG_STATUS_OVERCUR_Pos)               /*!< HSOTG_T::STATUS: OVERCUR Mask          */

#define HSOTG_STATUS_IDSTS_Pos           (1)                                               /*!< HSOTG_T::STATUS: IDSTS Position        */
#define HSOTG_STATUS_IDSTS_Msk           (0x1ul << HSOTG_STATUS_IDSTS_Pos)                 /*!< HSOTG_T::STATUS: IDSTS Mask            */

#define HSOTG_STATUS_SESSEND_Pos         (2)                                               /*!< HSOTG_T::STATUS: SESSEND Position      */
#define HSOTG_STATUS_SESSEND_Msk         (0x1ul << HSOTG_STATUS_SESSEND_Pos)               /*!< HSOTG_T::STATUS: SESSEND Mask          */

#define HSOTG_STATUS_BVLD_Pos            (3)                                               /*!< HSOTG_T::STATUS: BVLD Position         */
#define HSOTG_STATUS_BVLD_Msk            (0x1ul << HSOTG_STATUS_BVLD_Pos)                  /*!< HSOTG_T::STATUS: BVLD Mask             */

#define HSOTG_STATUS_AVLD_Pos            (4)                                               /*!< HSOTG_T::STATUS: AVLD Position         */
#define HSOTG_STATUS_AVLD_Msk            (0x1ul << HSOTG_STATUS_AVLD_Pos)                  /*!< HSOTG_T::STATUS: AVLD Mask             */

#define HSOTG_STATUS_VBUSVLD_Pos         (5)                                               /*!< HSOTG_T::STATUS: VBUSVLD Position      */
#define HSOTG_STATUS_VBUSVLD_Msk         (0x1ul << HSOTG_STATUS_VBUSVLD_Pos)               /*!< HSOTG_T::STATUS: VBUSVLD Mask          */

#define HSOTG_STATUS_ASPERI_Pos          (6)                                               /*!< HSOTG_T::STATUS: ASPERI Position       */
#define HSOTG_STATUS_ASPERI_Msk          (0x1ul << HSOTG_STATUS_ASPERI_Pos)                /*!< HSOTG_T::STATUS: ASPERI Mask           */

#define HSOTG_STATUS_ASHOST_Pos          (7)                                               /*!< HSOTG_T::STATUS: ASHOST Position       */
#define HSOTG_STATUS_ASHOST_Msk          (0x1ul << HSOTG_STATUS_ASHOST_Pos)                /*!< HSOTG_T::STATUS: ASHOST Mask           */

/** @} HSOTG_CONST */
/** @} end of HSOTG register group */
/** @} end of REGISTER group */

#if defined ( __CC_ARM   )
    #pragma no_anon_unions
#endif

#endif /* __HSOTG_REG_H__ */
