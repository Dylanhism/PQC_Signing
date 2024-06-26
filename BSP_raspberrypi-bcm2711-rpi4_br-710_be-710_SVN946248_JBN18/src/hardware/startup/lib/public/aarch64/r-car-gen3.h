/*
 * $QNXLicenseC:
 * Copyright 2016, 2017 QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */


/*
 * Renesas R-Car processor with ARMv8
 */

#ifndef __AARCH64_RCAR_GEN3_H_INCLUDED
#define __AARCH64_RCAR_GEN3_H_INCLUDED

/*
 * Board-Specific Defines for Gen3 Reference Boards
 */
/* External Crystal Clock */
#define RCAR_GEN3_EXT_CLK                16666666
#define RCAR_GEN3_EXT_CLK_XS             16640000        /* Salvator-XS */
#define RCAR_GEN3_EXT_CLK_D3             48000000        /* D3*/

/* Hyperflash */
#define RCAR_GEN3_RPC_DEV_ADDR           0x08000000
#define RCAR_GEN3_RPC_DEV_SIZE           0x04000000

/*
 * SOC shared Defines
 */

/*
 * GPIO
 */
#define RCAR_GEN3_GPIO_BASE              0xE6050000
#define RCAR_GEN3_GPIO_SIZE              0x50

#define RCAR_GEN3_GPIO0_BASE             0xE6050000
#define RCAR_GEN3_GPIO1_BASE             0xE6051000
#define RCAR_GEN3_GPIO2_BASE             0xE6052000
#define RCAR_GEN3_GPIO3_BASE             0xE6053000
#define RCAR_GEN3_GPIO4_BASE             0xE6054000
#define RCAR_GEN3_GPIO5_BASE             0xE6055000
#define RCAR_GEN3_GPIO6_BASE             0xE6055400
#define RCAR_GEN3_GPIO7_BASE             0xE6055800

 // GPIO Registers, offset from GPIO_BASE
 #define RCAR_GEN3_GPIO_IOINTSEL         0x0000
 #define RCAR_GEN3_GPIO_INOUTSEL         0x0004
 #define RCAR_GEN3_GPIO_OUTDT            0x0008
 #define RCAR_GEN3_GPIO_INDT             0x000C
 #define RCAR_GEN3_GPIO_INTDT            0x0010
 #define RCAR_GEN3_GPIO_INTCLR           0x0014
 #define RCAR_GEN3_GPIO_INTMSK           0x0018
 #define RCAR_GEN3_GPIO_MSKCLR           0x001C
 #define RCAR_GEN3_GPIO_POSNEG           0x0020
 #define RCAR_GEN3_GPIO_EDGLEVEL         0x0024
 #define RCAR_GEN3_GPIO_FILONOFF         0x0028
 #define RCAR_GEN3_GPIO_INTMSKS          0x0038
 #define RCAR_GEN3_GPIO_MSKCLRS          0x003C
 #define RCAR_GEN3_GPIO_OUTDTSEL         0x0040
 #define RCAR_GEN3_GPIO_OUTDTH           0x0044
 #define RCAR_GEN3_GPIO_OUTDTL           0x0048
 #define RCAR_GEN3_GPIO_BOTHEDGE         0x004C


/*
 * Pin Function Control
 */
#define RCAR_GEN3_PFC_BASE               0xE6060000
#define RCAR_GEN3_PFC_SIZE               0x50C

 /* PFC registers, offset from RCAR_GEN3_PFC_BASE */
 #define RCAR_GEN3_PFC_PMMR              0x000
 #define RCAR_GEN3_PFC_GPSR0             0x100
 #define RCAR_GEN3_PFC_GPSR1             0x104
 #define RCAR_GEN3_PFC_GPSR2             0x108
 #define RCAR_GEN3_PFC_GPSR3             0x10C
 #define RCAR_GEN3_PFC_GPSR4             0x110
 #define RCAR_GEN3_PFC_GPSR5             0x114
 #define RCAR_GEN3_PFC_GPSR6             0x118
 #define RCAR_GEN3_PFC_GPSR7             0x11C
 #define RCAR_GEN3_PFC_IPSR0             0x200
 #define RCAR_GEN3_PFC_IPSR1             0x204
 #define RCAR_GEN3_PFC_IPSR2             0x208
 #define RCAR_GEN3_PFC_IPSR3             0x20C
 #define RCAR_GEN3_PFC_IPSR4             0x210
 #define RCAR_GEN3_PFC_IPSR5             0x214
 #define RCAR_GEN3_PFC_IPSR6             0x218
 #define RCAR_GEN3_PFC_IPSR7             0x21C
 #define RCAR_GEN3_PFC_IPSR8             0x220
 #define RCAR_GEN3_PFC_IPSR9             0x224
 #define RCAR_GEN3_PFC_IPSR10            0x228
 #define RCAR_GEN3_PFC_IPSR11            0x22C
 #define RCAR_GEN3_PFC_IPSR12            0x230
 #define RCAR_GEN3_PFC_IPSR13            0x234
 #define RCAR_GEN3_PFC_IPSR14            0x238
 #define RCAR_GEN3_PFC_IPSR15            0x23C
 #define RCAR_GEN3_PFC_IPSR16            0x240
 #define RCAR_GEN3_PFC_IPSR17            0x244
 #define RCAR_GEN3_PFC_IPSR18            0x248
 #define RCAR_GEN3_PFC_DRVCTRL0          0x300
 #define RCAR_GEN3_PFC_DRVCTRL1          0x304
 #define RCAR_GEN3_PFC_DRVCTRL2          0x308
 #define RCAR_GEN3_PFC_DRVCTRL3          0x30C
 #define RCAR_GEN3_PFC_DRVCTRL4          0x310
 #define RCAR_GEN3_PFC_DRVCTRL5          0x314
 #define RCAR_GEN3_PFC_DRVCTRL6          0x318
 #define RCAR_GEN3_PFC_DRVCTRL7          0x31C
 #define RCAR_GEN3_PFC_DRVCTRL8          0x320
 #define RCAR_GEN3_PFC_DRVCTRL9          0x324
 #define RCAR_GEN3_PFC_DRVCTRL10         0x328
 #define RCAR_GEN3_PFC_DRVCTRL11         0x32C
 #define RCAR_GEN3_PFC_DRVCTRL12         0x330
 #define RCAR_GEN3_PFC_DRVCTRL13         0x334
 #define RCAR_GEN3_PFC_DRVCTRL14         0x338
 #define RCAR_GEN3_PFC_DRVCTRL15         0x33C
 #define RCAR_GEN3_PFC_DRVCTRL16         0x340
 #define RCAR_GEN3_PFC_DRVCTRL17         0x344
 #define RCAR_GEN3_PFC_DRVCTRL18         0x348
 #define RCAR_GEN3_PFC_DRVCTRL19         0x34C
 #define RCAR_GEN3_PFC_DRVCTRL20         0x350
 #define RCAR_GEN3_PFC_DRVCTRL21         0x354
 #define RCAR_GEN3_PFC_DRVCTRL22         0x358
 #define RCAR_GEN3_PFC_DRVCTRL23         0x35C
 #define RCAR_GEN3_PFC_DRVCTRL24         0x360
 #define RCAR_GEN3_PFC_POCCTRL0          0x380
 #define RCAR_GEN3_PFC_POCCTRL1          0x384
 #define RCAR_GEN3_PFC_POCCTRL2          0x388
 #define RCAR_GEN3_PFC_TDSELCTRL0        0x3C0
 #define RCAR_GEN3_PFC_IOCTRL            0x3E0
 #define RCAR_GEN3_PFC_FUSEMON0          0x3E4
 #define RCAR_GEN3_PFC_PUEN0             0x400
 #define RCAR_GEN3_PFC_PUEN1             0x404
 #define RCAR_GEN3_PFC_PUEN2             0x408
 #define RCAR_GEN3_PFC_PUEN3             0x40C
 #define RCAR_GEN3_PFC_PUEN4             0x410
 #define RCAR_GEN3_PFC_PUEN5             0x414
 #define RCAR_GEN3_PFC_PUEN6             0x418
 #define RCAR_GEN3_PFC_PUD0              0x440
 #define RCAR_GEN3_PFC_PUD1              0x444
 #define RCAR_GEN3_PFC_PUD2              0x448
 #define RCAR_GEN3_PFC_PUD3              0x44C
 #define RCAR_GEN3_PFC_PUD4              0x450
 #define RCAR_GEN3_PFC_PUD5              0x454
 #define RCAR_GEN3_PFC_PUD6              0x458
 #define RCAR_GEN3_PFC_MODSEL0           0x500
 #define RCAR_GEN3_PFC_MODSEL1           0x504
 #define RCAR_GEN3_PFC_MODSEL2           0x508

/*
 * Clock Pulse Generator
 */
#define RCAR_GEN3_CPG_BASE               0xE6150000
#define RCAR_GEN3_CPG_SIZE               0x1000

 /* CPG registers, offset from RCAR_GEN3_CPG_BASE */
 #define RCAR_GEN3_CPG_CPGWPCR           0x904       // CPG Write Protect Control Register
 #define RCAR_GEN3_CPG_CPGWPR            0x900       // CPG Write Protect Register
 #define RCAR_GEN3_CPG_FRQCRB            0x004       // Frequency Control Register B
 #define RCAR_GEN3_CPG_FRQCRC            0x0E0       // Frequency Control Register C
 #define RCAR_GEN3_CPG_PLLECR            0x0D0       // PLL Enable Control Register
 #define RCAR_GEN3_CPG_PLL0CR            0x0D8       // PLL0 Control Register
 #define RCAR_GEN3_CPG_PLL2CR            0x02C       // PLL2 Control Register
 #define RCAR_GEN3_CPG_PLL0STPCR         0x0F0       // PLL0 Stop Condition Register
 #define RCAR_GEN3_CPG_PLL2STPCR         0x0F8       // PLL2 Stop Condition Register
 #define RCAR_GEN3_CPG_PLL3STPCR         0x0FC       // PLL3 Stop Condition Register
 #define RCAR_GEN3_CPG_PLL4STPCR         0x1F8       // PLL4 Stop Condition Register

 #define RCAR_GEN3_CPG_CSI0CKCR          0x00C       // CSI0 clock frequency control register
 #define RCAR_GEN3_CPG_MSOCKCR           0x014       // MSIOF clock frequency control register
 #define RCAR_GEN3_CPG_SD0CKCR           0x074       // SDHI 0 Clock Frequency Control Register
 #define RCAR_GEN3_CPG_SD1CKCR           0x078       // SDHI 1 Clock Frequency Control Register
 #define RCAR_GEN3_CPG_SD2CKCR           0x268       // SDHI 2 Clock Frequency Control Register
 #define RCAR_GEN3_CPG_SD3CKCR           0x26C       // SDHI 3 Clock Frequency Control Register
 #define RCAR_GEN3_CPG_CANFDCKCR         0x244       // CAN-FD Clock Frequency Control Register
 #define RCAR_GEN3_CPG_SSPCKCR           0x248       // SSP Clock Frequency Control Register
 #define RCAR_GEN3_CPG_SSPRSCKCR         0x24C       // SSPRS Clock Frequency Control Register
 #define RCAR_GEN3_CPG_HDMICKCR          0x250       // HDMI-IF Clock Frequency Control Register
 #define RCAR_GEN3_CPG_RPCCKCR           0x238       // RPC Clock Frequency Control Register
 #define RCAR_GEN3_CPG_DVFSCR0           0x058       // DVFS Control Register 0
 #define RCAR_GEN3_CPG_DVFSCR1           0x05C       // DVFS Control Register 1
 #define RCAR_GEN3_CPG_ZA2CKCR           0x4DC       // ZA2CKCR Clock Frequency Control Register

 #define RCAR_GEN3_CPG_MSTPSR0           0x030       // Module Stop Status Register 0
 #define RCAR_GEN3_CPG_MSTPSR1           0x038       // Module Stop Status Register 1
 #define RCAR_GEN3_CPG_MSTPSR2           0x040       // Module Stop Status Register 2
 #define RCAR_GEN3_CPG_MSTPSR3           0x048       // Module Stop Status Register 3
 #define RCAR_GEN3_CPG_MSTPSR4           0x04C       // Module Stop Status Register 4
 #define RCAR_GEN3_CPG_MSTPSR5           0x03C       // Module Stop Status Register 5
 #define RCAR_GEN3_CPG_MSTPSR6           0x1C0       // Module Stop Status Register 6
 #define RCAR_GEN3_CPG_MSTPSR7           0x1C4       // Module Stop Status Register 7
 #define RCAR_GEN3_CPG_MSTPSR8           0x9A0       // Module Stop Status Register 8
 #define RCAR_GEN3_CPG_MSTPSR9           0x9A4       // Module Stop Status Register 9
 #define RCAR_GEN3_CPG_MSTPSR10          0x9A8       // Module Stop Status Register 10
 #define RCAR_GEN3_CPG_MSTPSR11          0x9AC       // Module Stop Status Register 11

 #define RCAR_GEN3_CPG_RMSTPCR0          0x110       // Realtime Module Stop Control Register 0
 #define RCAR_GEN3_CPG_RMSTPCR1          0x114       // Realtime Module Stop Control Register 1
 #define RCAR_GEN3_CPG_RMSTPCR2          0x118       // Realtime Module Stop Control Register 2
 #define RCAR_GEN3_CPG_RMSTPCR3          0x11C       // Realtime Module Stop Control Register 3
 #define RCAR_GEN3_CPG_RMSTPCR4          0x120       // Realtime Module Stop Control Register 4
 #define RCAR_GEN3_CPG_RMSTPCR5          0x124       // Realtime Module Stop Control Register 5
 #define RCAR_GEN3_CPG_RMSTPCR6          0x128       // Realtime Module Stop Control Register 6
 #define RCAR_GEN3_CPG_RMSTPCR7          0x12C       // Realtime Module Stop Control Register 7
 #define RCAR_GEN3_CPG_RMSTPCR8          0x980       // Realtime Module Stop Control Register 8
 #define RCAR_GEN3_CPG_RMSTPCR9          0x984       // Realtime Module Stop Control Register 9
 #define RCAR_GEN3_CPG_RMSTPCR10         0x988       // Realtime Module Stop Control Register 10
 #define RCAR_GEN3_CPG_RMSTPCR11         0x98C       // Realtime Module Stop Control Register 11

 #define RCAR_GEN3_CPG_SMSTPCR0          0x130       // System Module Stop Control Register 0
 #define RCAR_GEN3_CPG_SMSTPCR1          0x134       // System Module Stop Control Register 1
 #define RCAR_GEN3_CPG_SMSTPCR2          0x138       // System Module Stop Control Register 2
 #define RCAR_GEN3_CPG_SMSTPCR3          0x13C       // System Module Stop Control Register 3
 #define RCAR_GEN3_CPG_SMSTPCR4          0x140       // System Module Stop Control Register 4
 #define RCAR_GEN3_CPG_SMSTPCR5          0x144       // System Module Stop Control Register 5
 #define RCAR_GEN3_CPG_SMSTPCR6          0x148       // System Module Stop Control Register 6
 #define RCAR_GEN3_CPG_SMSTPCR7          0x14C       // System Module Stop Control Register 7
 #define RCAR_GEN3_CPG_SMSTPCR8          0x990       // System Module Stop Control Register 8
 #define RCAR_GEN3_CPG_SMSTPCR9          0x994       // System Module Stop Control Register 9
 #define RCAR_GEN3_CPG_SMSTPCR10         0x998       // System Module Stop Control Register 10
 #define RCAR_GEN3_CPG_SMSTPCR11         0x99C       // System Module Stop Control Register 11

 #define RCAR_GEN3_CPG_SRCR0             0x0A0       // Software Reset Register 0
 #define RCAR_GEN3_CPG_SRCR1             0x0A8       // Software Reset Register 1
 #define RCAR_GEN3_CPG_SRCR2             0x0B0       // Software Reset Register 2
 #define RCAR_GEN3_CPG_SRCR3             0x0B8       // Software Reset Register 3
 #define RCAR_GEN3_CPG_SRCR4             0x0BC       // Software Reset Register 4
 #define RCAR_GEN3_CPG_SRCR5             0x0C4       // Software Reset Register 5
 #define RCAR_GEN3_CPG_SRCR6             0x1C8       // Software Reset Register 6
 #define RCAR_GEN3_CPG_SRCR7             0x1CC       // Software Reset Register 7
 #define RCAR_GEN3_CPG_SRCR8             0x920       // Software Reset Register 8
 #define RCAR_GEN3_CPG_SRCR9             0x924       // Software Reset Register 9
 #define RCAR_GEN3_CPG_SRCR10            0x928       // Software Reset Register 10
 #define RCAR_GEN3_CPG_SRCR11            0x92C       // Software Reset Register 11

 #define RCAR_GEN3_CPG_SRSTCLR0          0x940       // Software Reset Clear Register 0
 #define RCAR_GEN3_CPG_SRSTCLR1          0x944       // Software Reset Clear Register 1
 #define RCAR_GEN3_CPG_SRSTCLR2          0x948       // Software Reset Clear Register 2
 #define RCAR_GEN3_CPG_SRSTCLR3          0x94C       // Software Reset Clear Register 3
 #define RCAR_GEN3_CPG_SRSTCLR4          0x950       // Software Reset Clear Register 4
 #define RCAR_GEN3_CPG_SRSTCLR5          0x954       // Software Reset Clear Register 5
 #define RCAR_GEN3_CPG_SRSTCLR6          0x958       // Software Reset Clear Register 6
 #define RCAR_GEN3_CPG_SRSTCLR7          0x95C       // Software Reset Clear Register 7
 #define RCAR_GEN3_CPG_SRSTCLR8          0x960       // Software Reset Clear Register 8
 #define RCAR_GEN3_CPG_SRSTCLR9          0x964       // Software Reset Clear Register 9
 #define RCAR_GEN3_CPG_SRSTCLR10         0x968       // Software Reset Clear Register 10
 #define RCAR_GEN3_CPG_SRSTCLR11         0x96C       // Software Reset Clear Register 11

/*
 * Advanced Power Management Unit registers
 */
#define RCAR_GEN3_APMU_BASE               0xE6150000
#define RCAR_GEN3_APMU_SIZE               0x4000

 #define RCAR_GEN3_APMU_CA53WUPCR         0x1010
 #define RCAR_GEN3_APMU_CA57WUPCR         0x2010

 #define RCAR_GEN3_APMU_CA53CPUCMCR       0x1184      // Common power control register for A53
 #define RCAR_GEN3_APMU_CA57CPUCMCR       0x2184      // Common power control register for A57

/*
 * Reset registers
 */
#define RCAR_GEN3_RESET_BASE             0xE6160000
 #define RCAR_GEN3_RESET_CA57RESCNT      0x40
 #define RCAR_GEN3_RESET_CA53RESCNT      0x44
 #define RCAR_GEN3_WDTRSTCR              0x54        // Watchdog reset control register
 #define RCAR_GEN3_MODEMR                0x60        // Mode Monitor Register
  #define RCAR_GEN3_MODEMR_BOOT_MASK           0x1E
  #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH160  0x4
  #define RCAR_GEN3_MODEMR_BOOT_HYPERFLASH80   0x6
  #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH40   0x8
  #define RCAR_GEN3_MODEMR_BOOT_QSPI_FLASH80   0xC
 #define RCAR_GEN3_RESET_CA53CPUxBARH(x) (0x80 + (0x10 * x))
 #define RCAR_GEN3_RESET_CA53CPUxBARL(x) (0x84 + (0x10 * x))
 #define RCAR_GEN3_RESET_CA57CPUxBARH(x) (0xC0 + (0x10 * x))
 #define RCAR_GEN3_RESET_CA57CPUxBARL(x) (0xC4 + (0x10 * x))


/*
 * System Controller
 */
#define RCAR_GEN3_SYSC_BASE              0xE6180000
 #define RCAR_GEN3_SYSC_SR               0x00        // Status Register
 #define RCAR_GEN3_SYSC_ISR              0x04        // Interrupt Status Register
 #define RCAR_GEN3_SYSC_ISCR             0x08        // Interrupt Status Clear Register
 #define RCAR_GEN3_SYSC_IER              0x0C        // Interrupt Enable Register
 #define RCAR_GEN3_SYSC_IMR              0x10        // Interrupt Mask Register
 #define RCAR_GEN3_SYSC_WUPMSKCA57       0x14
 #define RCAR_GEN3_SYSC_WUPMSKCA53       0x18
 #define RCAR_GEN3_SYSC_EERSR            0x20        // External Event Request Status Register
 #define RCAR_GEN3_SYSC_EERSCR           0x24        // External Event Request Status Clear Register
 #define RCAR_GEN3_SYSC_EERSER           0x28        // External Event Request Status Enable Register
 #define RCAR_GEN3_SYSC_EERSR2           0x2C        // External Event Request Status Register 2
 #define RCAR_GEN3_SYSC_EERSCR2          0x30        // External Event Request Status Clear register 2
 #define RCAR_GEN3_SYSC_EERSER2          0x34        // External Event Request Status Enable Register 2
 #define RCAR_GEN3_SYSC_EERSR3           0x40        // External Event Request Status Register 3
 #define RCAR_GEN3_SYSC_EERSCR3          0x44        // External Event Request Status Clear register 3
 #define RCAR_GEN3_SYSC_EERSER3          0x48        // External Event Request Status Enable Register 3

 /* CA57 */
 #define RCAR_GEN3_SYSC_PWRSR0           0x80        // Power status register 0 (CA57)
 #define RCAR_GEN3_SYSC_PWROFFSR0        0x88        // Power shutoff status register 0 (CA57)
 #define RCAR_GEN3_SYSC_PWRONSR0         0x90        // Power resume status register 0 (CA57)
 #define RCAR_GEN3_SYSC_PWRER0           0x94        // Power shutoff/resume error register 0 (CA57)
 #define RCAR_GEN3_SYSC_PWRPSEU0         0xb8        // Power pseudo shutoff register 0 (CA57)
 #define RCAR_GEN3_SYSC_PWRISOER0        0xbc        // Power isolation error detection register 0 (CA57)

 /* 3DG */
 #define RCAR_GEN3_SYSC_PWRSR2           0x100        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR2        0x104        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR2        0x108        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR2         0x10C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR2         0x110        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER2           0x114        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU2         0x138        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER2        0x13C        // Power shutoff/resume Error Register

 /* CA53-SCU */
 #define RCAR_GEN3_SYSC_PWRSR3           0x140        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR3        0x144        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR3        0x148        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR3         0x14C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR3         0x150        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER3           0x154        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU3         0x178        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER3        0x17C        // Power shutoff/resume Error Register

 /* IMP */
 #define RCAR_GEN3_SYSC_PWRSR4           0x180        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR4        0x184        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR4        0x188        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR4         0x18C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR4         0x190        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER4           0x194        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_EXTMASK          0x1B0        // Power control protection error register
 #define RCAR_GEN3_SYSC_PWRPSEU4         0x1B8        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER4        0x1BC        // Power shutoff/resume Error Register

 /* CA57-SCU */
 #define RCAR_GEN3_SYSC_PWRSR5           0x1C0        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR5        0x1C4        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR5        0x1C8        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR5         0x1CC        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR5         0x1D0        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER5           0x1D4        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU5         0x1F8        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER5        0x1FC        // Power shutoff/resume Error Register

 /* CA53-SCU */
 #define RCAR_GEN3_SYSC_PWRSR6           0x200        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFSR6        0x208        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONSR6         0x210        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER6           0x214        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU6         0x238        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER6        0x23C        // Power shutoff/resume Error Register

 /* CR7 */
 #define RCAR_GEN3_SYSC_PWRSR7           0x240        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR7        0x244        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR7        0x248        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR7         0x24C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR7         0x250        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER7           0x254        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU7         0x278        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER7        0x27C        // Power shutoff/resume Error Register

 /* A3VP */
 #define RCAR_GEN3_SYSC_PWRSR8           0x340        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR8        0x344        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR8        0x348        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR8         0x34C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR8         0x350        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER8           0x354        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU8         0x378        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER8        0x37C        // Power shutoff/resume Error Register

 /* A3VC */
 #define RCAR_GEN3_SYSC_PWRSR9           0x380        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR9        0x384        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR9        0x388        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR9         0x38C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR9         0x390        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER9           0x394        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU9         0x3B8        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER9        0x3BC        // Power shutoff/resume Error Register

 /* A2VC */
 #define RCAR_GEN3_SYSC_PWRSR10          0x3C0        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR10       0x3C4        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR10       0x3C8        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR10        0x3CC        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR10        0x3D0        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER10          0x3D4        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU10        0x3F8        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRISOER10       0x3FC        // Power shutoff/resume Error Register

 /* A2IR/A2SC */
 #define RCAR_GEN3_SYSC_PWRSR11          0x400        // Power Status Register
 #define RCAR_GEN3_SYSC_PWROFFCR11       0x404        // Power Shutoff Control Register
 #define RCAR_GEN3_SYSC_PWROFFSR11       0x408        // Power Shutoff Status Register
 #define RCAR_GEN3_SYSC_PWRONCR11        0x40C        // Power Resume Control Register
 #define RCAR_GEN3_SYSC_PWRONSR11        0x410        // Power Resume Status Register
 #define RCAR_GEN3_SYSC_PWRER11          0x414        // Power shutoff/resume Error Register
 #define RCAR_GEN3_SYSC_PWRPSEU11        0x438        // Power pseudo shutoff Register
 #define RCAR_GEN3_SYSC_PWRISOER11       0x43C        // Power isolation error detection register

 #define RCAR_GEN3_SYSC_PWRSRx(_x)       (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40))
 #define RCAR_GEN3_SYSC_PWROFFCRx(_x)    (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40) + 0x4)
 #define RCAR_GEN3_SYSC_PWROFFSRx(_x)    (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40) + 0x8)
 #define RCAR_GEN3_SYSC_PWRONCRx(_x)     (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40) + 0xC)
 #define RCAR_GEN3_SYSC_PWRONSRx(_x)     (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40) + 0x10)
 #define RCAR_GEN3_SYSC_PWRERx(_x)       (RCAR_GEN3_SYSC_PWRSR0 + ((_x) * 0x40) + 0x14)

/*
 * Watchdog Timer
 */
#define RCAR_GEN3_RWDT_BASE              0xE6020000  // RCLK Watchdog Timer
#define RCAR_GEN3_SWDT_BASE              0xE6030000  // Secure Watchdog Timer
 #define RCAR_GEN3_WDT_CNT               0x00        // Count Register
 #define RCAR_GEN3_WDT_CSRA              0x04        // Control/Status Register A
 #define RCAR_GEN3_WDT_CSRB              0x08        // Control/Status Register B
#define RCAR_GEN3_WDT_SIZE               0x0C

/*
 * ARM Generic Timer
 */
#define RCAR_GEN3_ARM_CNTR_BASE          0xE6080000  // ARM Generic Timer
 #define RCAR_GEN3_ARM_CNTR_CNTCR        0x00
  #define RCAR_GEN3_ARM_CNTR_CNTCR_EN    (1 << 0)
 #define RCAR_GEN3_ARM_CNTR_CNTFID       0x20


/*
 * SCU
 */
#define RCAR_GEN3_SCU_BASE               0xEC500000
#define RCAR_GEN3_SCU_SIZE               0x1000

/*
 * SSIU
 */
#define RCAR_GEN3_SSIU_BASE              0xEC540000
#define RCAR_GEN3_SSIU_SIZE              0x1000
 #define RCAR_GEN3_SSIU_MODE0            0x800
 #define RCAR_GEN3_SSIU_MODE1            0x804
 #define RCAR_GEN3_SSIU_MODE2            0x808
 #define RCAR_GEN3_SSIU_MODE3            0x80C
 #define RCAR_GEN3_SSIU_CONTROL          0x810


/*
 * SSI
 */
#define RCAR_GEN3_SSI_BASE               0xEC541000
#define RCAR_GEN3_SSI_SIZE               0x280       // 10 Modules * 0x40 each

/*
 * Interrupt Controller (INTC-AP)
 */
#define RCAR_GEN3_GIC_BASE               0xF1000000
#define RCAR_GEN3_GIC_CPU_BASE           0xF1020000
#define RCAR_GEN3_GIC_DIST_BASE          0xF1010000

/*
 * Interrupt Controller (INTC-EX)
 */
#define RCAR_GEN3_IRQC0_BASE             0xE61C0000
 #define RCAR_GEN3_INTREQ_STS0           0x000       /* R */
 #define RCAR_GEN3_INTEN_STS0            0x004       /* R/WC1 */
 #define RCAR_GEN3_INTEN_SET0            0x008       /* W */
 #define RCAR_GEN3_INTREQ_STS1           0x010       /* R */
 #define RCAR_GEN3_INTEN_STS1            0x014       /* R/WC1 */
 #define RCAR_GEN3_INTEN_SET1            0x018       /* W */
 #define RCAR_GEN3_DETECT_STATUS         0x100       /* R/WC1 */
 #define RCAR_GEN3_CONFIG_00             0x180       /* R/W */


/*
 * SDHI
 */
#define RCAR_GEN3_SDHI0_BASE             0xEE100000  // SDHI0 Base
#define RCAR_GEN3_SDHI1_BASE             0xEE120000  // SDHI1 Base
#define RCAR_GEN3_SDHI2_BASE             0xEE140000  // SDHI2 Base
#define RCAR_GEN3_SDHI3_BASE             0xEE160000  // SDHI3 Base
#define RCAR_GEN3_SDHI_SIZE              0x2000

/*
 * MMCIF
 */
#define RCAR_GEN3_MMCIF0_BASE            0xEE140000
#define RCAR_GEN3_MMCIF1_BASE            0xEE160000
#define RCAR_GEN3_MMCIF_SIZE             0x2000

/*
 * RTDMAC
 */
#define RCAR_GEN3_RTDMAC1_BASE           0xFFC10000

/*
 * SYSDMAC
 */
#define RCAR_GEN3_SYSDMAC0_BASE          0xE6700000
#define RCAR_GEN3_SYSDMAC1_BASE          0xE7300000
#define RCAR_GEN3_SYSDMAC2_BASE          0xE7310000
#define RCAR_GEN3_SYSDMAC_SIZE           0x3000      // BASE+REGS to DESC

 /* SYSDMAC global registers, offset from BASE */
 #define RCAR_GEN3_SYSDMAC_DMAISTA       0x0020      // DMA interrupt status register
 #define RCAR_GEN3_SYSDMAC_DMASEC        0x0030      // DMA secure control register
 #define RCAR_GEN3_SYSDMAC_DMAOR         0x0060      // DMA operation register
 #define RCAR_GEN3_SYSDMAC_DMACHCLR      0x0080      // DMA channel clear register
 #define RCAR_GEN3_SYSDMAC_DMADPSEC      0x00A0      // DPRAM secure control register
 #define RCAR_GEN3_SYSDMAC_REGS          0x8000

 /* SYSDMAC register, offset from BASE + REGS */
 #define RCAR_GEN3_SYSDMAC_REGSIZE       0x80
 #define RCAR_GEN3_SYSDMAC_DESC          (0xA000 - RCAR_GEN3_SYSDMAC_REGS)

/*
 * AUDIODMAC
 */
#define RCAR_GEN3_AUDIODMAC0_BASE        0xEC700000
#define RCAR_GEN3_AUDIODMAC1_BASE        0xEC720000

/*
 * AUDIODMACPP
 */
#define RCAR_GEN3_AUDIODMACPP_BASE       0xEC740000
#define RCAR_GEN3_AUDIODMACPP_EXT_BASE   0xEC760000
#define RCAR_GEN3_AUDIODMACPP_SIZE       0x1000

/*
 * I2C
 */
#define RCAR_GEN3_I2C0_BASE              0xE6500000
#define RCAR_GEN3_I2C1_BASE              0xE6508000
#define RCAR_GEN3_I2C2_BASE              0xE6510000
#define RCAR_GEN3_I2C3_BASE              0xE66D0000
#define RCAR_GEN3_I2C4_BASE              0xE66D8000
#define RCAR_GEN3_I2C5_BASE              0xE66E0000
#define RCAR_GEN3_I2C6_BASE              0xE66E8000

#define RCAR_GEN3_I2C_SIZE               0x100

/*
 * I2C Bus Interface for DVFS IIC
 */
#define RCAR_GEN3_IIC_BASE               0xE60B0000

#define RCAR_GEN3_IIC_SIZE               0x100

/*
 * Clock Synchronized Serial Interface with FIFO
 */
#define RCAR_GEN3_MSIOF0_BASE            0xE6E90000
#define RCAR_GEN3_MSIOF1_BASE            0xE6EA0000
#define RCAR_GEN3_MSIOF2_BASE            0xE6C00000
#define RCAR_GEN3_MSIOF3_BASE            0xE6C10000

#define RCAR_GEN3_MSIOF_SIZE             0x100

 /* MSIOF registers, offset from base */
 #define RCAR_GEN3_MSIOF_TMDR1           0x00
 #define RCAR_GEN3_MSIOF_TMDR2           0x04
 #define RCAR_GEN3_MSIOF_TMDR3           0x08
 #define RCAR_GEN3_MSIOF_RMDR1           0x10
 #define RCAR_GEN3_MSIOF_RMDR2           0x14
 #define RCAR_GEN3_MSIOF_RMDR3           0x18
 #define RCAR_GEN3_MSIOF_TSCR            0x20
 #define RCAR_GEN3_MSIOF_CTR             0x28
 #define RCAR_GEN3_MSIOF_FCTR            0x30
 #define RCAR_GEN3_MSIOF_STR             0x40
 #define RCAR_GEN3_MSIOF_IER             0x44
 #define RCAR_GEN3_MSIOF_TFDR            0x50
 #define RCAR_GEN3_MSIOF_RFDR            0x60

/* MSIOF DMA Request IDs */
#define RCAR_GEN3_MSIOF0_DMA_ID_RX       0x40
#define RCAR_GEN3_MSIOF0_DMA_ID_TX       0x41
#define RCAR_GEN3_MSIOF1_DMA_ID_RX       0x42
#define RCAR_GEN3_MSIOF1_DMA_ID_TX       0x43

/*
 * SCIF
 */
#define RCAR_GEN3_SCIF0_BASE             0xE6E60000
#define RCAR_GEN3_SCIF1_BASE             0xE6E68000
#define RCAR_GEN3_SCIF2_BASE             0xE6E88000
#define RCAR_GEN3_SCIF3_BASE             0xE6C50000
#define RCAR_GEN3_SCIF4_BASE             0xE6C40000
#define RCAR_GEN3_SCIF5_BASE             0xE6F30000

#define RCAR_GEN3_SCIF_SIZE              0x100

#define RCAR_GEN3_SCIF_FIFO_SIZE         16

 /* SCIF registers, offset from SCIF_BASE */
 #define RCAR_GEN3_SCIF_SCSMR            0x00
 #define RCAR_GEN3_SCIF_SCBRR            0x04
 #define RCAR_GEN3_SCIF_SCSCR            0x08
 #define RCAR_GEN3_SCIF_SCFTDR           0x0C
 #define RCAR_GEN3_SCIF_SCFSR            0x10
 #define RCAR_GEN3_SCIF_SCFRDR           0x14
 #define RCAR_GEN3_SCIF_SCFCR            0x18
 #define RCAR_GEN3_SCIF_SCFDR            0x1C
 #define RCAR_GEN3_SCIF_SCSPTR           0x20
 #define RCAR_GEN3_SCIF_SCLSR            0x24
 #define RCAR_GEN3_SCIF_BRG_DL           0x30
 #define RCAR_GEN3_SCIF_BRG_CKS          0x34

 /* SCSMR bit definition */
 #define RCAR_GEN3_SCIF_SCSMR_CHR        (1 << 6)
 #define RCAR_GEN3_SCIF_SCSMR_PE         (1 << 5)
 #define RCAR_GEN3_SCIF_SCSMR_OE         (1 << 4)
 #define RCAR_GEN3_SCIF_SCSMR_STOP       (1 << 3)
 #define RCAR_GEN3_SCIF_SCSMR_CKS_0      (0 << 0)
 #define RCAR_GEN3_SCIF_SCSMR_CKS_4      (1 << 0)
 #define RCAR_GEN3_SCIF_SCSMR_CKS_16     (2 << 0)
 #define RCAR_GEN3_SCIF_SCSMR_CKS_64     (3 << 0)

  /* SCSCR bit definition */
  #define RCAR_GEN3_SCIF_SCSCR_TEIE      (1 << 11)
  #define RCAR_GEN3_SCIF_SCSCR_TIE       (1 << 7)
  #define RCAR_GEN3_SCIF_SCSCR_RIE       (1 << 6)
  #define RCAR_GEN3_SCIF_SCSCR_TE        (1 << 5)
  #define RCAR_GEN3_SCIF_SCSCR_RE        (1 << 4)
  #define RCAR_GEN3_SCIF_SCSCR_REIE      (1 << 3)
  #define RCAR_GEN3_SCIF_SCSCR_CKE_MASK  (3 << 0)

  /* SCFSR bit definition */
  #define RCAR_GEN3_SCIF_SCFSR_PERF(x)   (((x) >> 12) & 0xF)
  #define RCAR_GEN3_SCIF_SCFSR_FERF(x)   (((x) >> 8) & 0xF)
  #define RCAR_GEN3_SCIF_SCFSR_ER        (1 << 7)
  #define RCAR_GEN3_SCIF_SCFSR_TEND      (1 << 6)
  #define RCAR_GEN3_SCIF_SCFSR_TDFE      (1 << 5)
  #define RCAR_GEN3_SCIF_SCFSR_BRK       (1 << 4)
  #define RCAR_GEN3_SCIF_SCFSR_FER       (1 << 3)
  #define RCAR_GEN3_SCIF_SCFSR_PER       (1 << 2)
  #define RCAR_GEN3_SCIF_SCFSR_RDF       (1 << 1)
  #define RCAR_GEN3_SCIF_SCFSR_DR        (1 << 0)

  /* SCFCR bit definition */
  #define RCAR_GEN3_SCIF_SCFCR_RTRG_1    (0 << 6)
  #define RCAR_GEN3_SCIF_SCFCR_RTRG_4    (1 << 6)
  #define RCAR_GEN3_SCIF_SCFCR_RTRG_8    (2 << 6)
  #define RCAR_GEN3_SCIF_SCFCR_RTRG_14   (3 << 6)
  #define RCAR_GEN3_SCIF_SCFCR_TTRG_0    (3 << 4)
  #define RCAR_GEN3_SCIF_SCFCR_TTRG_2    (2 << 4)
  #define RCAR_GEN3_SCIF_SCFCR_TTRG_4    (1 << 4)
  #define RCAR_GEN3_SCIF_SCFCR_TTRG_8    (0 << 4)
  #define RCAR_GEN3_SCIF_SCFCR_MCE       (1 << 3)
  #define RCAR_GEN3_SCIF_SCFCR_TFRST     (1 << 2)
  #define RCAR_GEN3_SCIF_SCFCR_RFRST     (1 << 1)
  #define RCAR_GEN3_SCIF_SCFCR_LOOP      (1 << 0)

  /* SCFDR bit definition */
  #define RCAR_GEN3_SCIF_SCFDR_TX(x)     (((x) >> 8) & 0x1F)
  #define RCAR_GEN3_SCIF_SCFDR_RX(x)     ((x) & 0x1F)

  /* SCSPTR bit definition */
  #define RCAR_GEN3_SCIF_SCSPTR_RTSIO    (1 << 7)
  #define RCAR_GEN3_SCIF_SCSPTR_RTSDT    (1 << 6)
  #define RCAR_GEN3_SCIF_SCSPTR_CTSIO    (1 << 5)
  #define RCAR_GEN3_SCIF_SCSPTR_CTSDT    (1 << 4)
  #define RCAR_GEN3_SCIF_SCSPTR_SPB2IO   (1 << 1)
  #define RCAR_GEN3_SCIF_SCSPTR_SPB2DT   (1 << 0)

  /* SCLSR bit definition */
  #define RCAR_GEN3_SCIF_SCLSR_ORER      (1 << 0)

/*
 * HSCIF
 */
#define RCAR_GEN3_HSCIF0_BASE            0xE6540000
#define RCAR_GEN3_HSCIF1_BASE            0xE6550000
#define RCAR_GEN3_HSCIF2_BASE            0xE6560000
#define RCAR_GEN3_HSCIF3_BASE            0xE66A0000
#define RCAR_GEN3_HSCIF4_BASE            0xE66B0000

/*
* RPC (SPI)
*/
#define RCAR_GEN3_RPC_BASE               0xEE200000
#define RCAR_GEN3_RPC_SIZE               0xAC

/* Register offset */
#define RCAR_GEN3_RPC_BUFFER             0x8000
#define RCAR_GEN3_RPC_BUFFER_SIZE        0xFF

/*
 * USB 2.0
 */
#define RCAR_GEN3_USB0_BASE              0xEE080000
#define RCAR_GEN3_USB1_BASE              0xEE0A0000
/*
 * USB 2.0 (H3 ONLY)
 */
#define RCAR_GEN3_USB2_BASE              0xEE0C0000
#define RCAR_GEN3_USB3_BASE              0xEE0E0000

/* Register offset */
 /* AHB bridge registers */
 #define RCAR_GEN3_USB_AHB_INT_ENABLE    0x200
 #define RCAR_GEN3_USB_AHB_USBCTR        0x20C

 #define RCAR_GEN3_USB_AHB_PLL_RST       (1 << 1)
 #define RCAR_GEN3_USB_AHB_USBH_INTBEN   (1 << 2)
 #define RCAR_GEN3_USB_AHB_USBH_INTAEN   (1 << 1)

/* UCOM registers */
#define RCAR_GEN3_USB_UCOM_COMMCTRL		 0x800

#define RCAR_GEN3_USB_UCOM_OTG_PERI      (1 << 31)
#define RCAR_GEN3_USB_UCOM_OTG_PERI_HOST (0 << 31)
#define RCAR_GEN3_USB_UCOM_OTG_PERI_PERI (1 << 31)

 /* Core defined registers */
 #define RCAR_GEN3_USB_CORE_SPD_RSM_TIMSET     0x30C
 #define RCAR_GEN3_USB_CORE_OC_TIMSET          0x310

/*
 * HSUSB
 */
#define RCAR_GEN3_HSUSB_BASE             0xE6590000
/*
 * HSUSB3 (H3 ONLY)
  */
#define RCAR_GEN3_HSUSB3_BASE            0xE659C000

/* Register offset */
#define RCAR_GEN3_HSUSB_SYSCFG           0x0000
#define RCAR_GEN3_HSUSB_INTENB0          0x0030
#define RCAR_GEN3_HSUSB_LPSTS            0x0102   /* 16bit */
#define RCAR_GEN3_HSUSB_UGCTRL           0x0180
#define RCAR_GEN3_HSUSB_UGCTRL2          0x0184

 #define RCAR_GEN3_HSUSB_SUSPM           (1 << 14)
 #define RCAR_GEN3_HSUSB_SUSPM_NORMAL    (1 << 14)

 #define RCAR_GEN3_HSUSB_USB0SEL         (0x3 << 4)
 #define RCAR_GEN3_HSUSB_USB0SEL_EHCI    (1 << 4)

/*
 * USB 3.0
 */
#define RCAR_GEN3_USB30_BASE             0xEE000000
#define RCAR_GEN3_USB31_BASE             0xEE040000
#define RCAR_GEN3_USB3_SIZE              0xBFF

/*** Register Offset ***/
#define RCAR_GEN3_USB3_CLASSCODE         0x204
#define RCAR_GEN3_USB3_RELEASE_NUMBER    0x210

#define RCAR_GEN3_USB3_INT_ENA           0x224   /* Interrupt Enable */
#define RCAR_GEN3_USB3_DL_CTRL           0x250   /* FW Download Control & Status */
#define RCAR_GEN3_USB3_FW_DATA0          0x258   /* FW Data0 */

#define RCAR_GEN3_USB3_LCLK              0xa44   /* LCLK Select */
#define RCAR_GEN3_USB3_CONF1             0xa48   /* USB3.0 Configuration1 */
#define RCAR_GEN3_USB3_CONF2             0xa5c   /* USB3.0 Configuration2 */
#define RCAR_GEN3_USB3_CONF3             0xaa8   /* USB3.0 Configuration3 */
#define RCAR_GEN3_USB3_RX_POL            0xab0   /* USB3.0 RX Polarity */
#define RCAR_GEN3_USB3_TX_POL            0xab8   /* USB3.0 TX Polarity */

/*** Register Settings ***/
 /* Interrupt Enable */
 #define RCAR_GEN3_USB3_INT_SMI_ENA      (1 << 4)
 #define RCAR_GEN3_USB3_INT_LTM_ENA      (1 << 3)
 #define RCAR_GEN3_USB3_INT_HSE_ENA      (1 << 2)
 #define RCAR_GEN3_USB3_INT_PME_ENA      (1 << 1)
 #define RCAR_GEN3_USB3_INT_XHC_ENA      (1 << 0)
 #define RCAR_GEN3_USB3_INT_ENA_VAL      (RCAR_GEN3_USB3_INT_XHC_ENA | RCAR_GEN3_USB3_INT_PME_ENA | RCAR_GEN3_USB3_INT_HSE_ENA)

 /* FW Download Control & Status */
 #define RCAR_GEN3_USB3_DL_CTRL_ENABLE        (1 << 0)
 #define RCAR_GEN3_USB3_DL_CTRL_FW_SUCCESS    (1 << 4)
 #define RCAR_GEN3_USB3_DL_CTRL_FW_SET_DATA0  (1 << 8)

 /* LCLK Select */
 #define RCAR_GEN3_USB3_PTPWER_CTRL      (1 << 24)
 #define RCAR_GEN3_USB3_RENESAS_PRIV     (0x3 << 16)
 #define RCAR_GEN3_USB3_BC_MODE          (1 << 0)

 /* USB3.0 Configuration */
 #define RCAR_GEN3_USB3_CONF1_VAL        0x00030204
 #define RCAR_GEN3_USB3_CONF2_VAL        0x00030300
 #define RCAR_GEN3_USB3_CONF3_VAL        0x13802007

 /* USB3.0 Polarity */
 #define RCAR_GEN3_USB3_RX_POL_VAL       (1 << 21)
 #define RCAR_GEN3_USB3_TX_POL_VAL       (1 << 4)

/*
 * DBSC4 Controller
 */
#define RCAR_GEN3_DBSC4_BASE             0xE6790000
 #define RCAR_GEN3_SDRAM_0               0x40000000
 #define RCAR_GEN3_SDRAM_0_1             0x480000000
 #define RCAR_GEN3_SDRAM_1               0x500000000
 #define RCAR_GEN3_SDRAM_2               0x600000000
 #define RCAR_GEN3_SDRAM_3               0x700000000

/* DBSC4 registers, offset from DBSC4_BASE */

 #define RCAR_GEN3_DBSC4_DBSYSCONF1      0x0004
 #define RCAR_GEN3_DBSC4_DBPHYCONF0      0x0010
 #define RCAR_GEN3_DBSC4_DBKIND          0x0020
 #define RCAR_GEN3_DBSC4_DBMEMCONF_0_0   0x0030
 #define RCAR_GEN3_DBSC4_DBMEMCONF_0_1   0x0034
 #define RCAR_GEN3_DBSC4_DBMEMCONF_1_0   0x0040
 #define RCAR_GEN3_DBSC4_DBMEMCONF_1_1   0x0044
 #define RCAR_GEN3_DBSC4_DBMEMCONF_2_0   0x0050
 #define RCAR_GEN3_DBSC4_DBMEMCONF_2_1   0x0054
 #define RCAR_GEN3_DBSC4_DBMEMCONF_3_0   0x0060
 #define RCAR_GEN3_DBSC4_DBMEMCONF_3_1   0x0064

 #define RCAR_GEN3_DBSC4_DBMEMCONF_0_2   0x0038
 #define RCAR_GEN3_DBSC4_DBMEMCONF_0_3   0x003C
 #define RCAR_GEN3_DBSC4_DBMEMCONF_1_2   0x0048
 #define RCAR_GEN3_DBSC4_DBMEMCONF_1_3   0x004C
 #define RCAR_GEN3_DBSC4_DBMEMCONF_2_2   0x0058
 #define RCAR_GEN3_DBSC4_DBMEMCONF_2_3   0x005C
 #define RCAR_GEN3_DBSC4_DBMEMCONF_3_2   0x0068
 #define RCAR_GEN3_DBSC4_DBMEMCONF_3_3   0x006C
 #define RCAR_GEN3_MAX_RANK              4

 #define RCAR_GEN3_DBSC4_DBSYSCNT0       0x0100
 #define RCAR_GEN3_DBSC4_DBSVCR1         0x0104
 #define RCAR_GEN3_DBSC4_DBSTATE0        0x0108
 #define RCAR_GEN3_DBSC4_DBSTATE1        0x010C

 #define RCAR_GEN3_DBSC4_DBACEN          0x0200
 #define RCAR_GEN3_DBSC4_DBRFEN          0x0204
 #define RCAR_GEN3_DBSC4_DBCMD           0x0208
 #define RCAR_GEN3_DBSC4_DBWAIT          0x0210 //wait DBCMD 1=busy, 0=ready
 #define RCAR_GEN3_DBSC4_DBSYSCTRL0      0x0280

 #define RCAR_GEN3_DBSC4_DBTR0           0x0300
 #define RCAR_GEN3_DBSC4_DBTR1           0x0304
 #define RCAR_GEN3_DBSC4_DBTR2           0x0308
 #define RCAR_GEN3_DBSC4_DBTR3           0x030C
 #define RCAR_GEN3_DBSC4_DBTR4           0x0310
 #define RCAR_GEN3_DBSC4_DBTR5           0x0314
 #define RCAR_GEN3_DBSC4_DBTR6           0x0318
 #define RCAR_GEN3_DBSC4_DBTR7           0x031C
 #define RCAR_GEN3_DBSC4_DBTR8           0x0320
 #define RCAR_GEN3_DBSC4_DBTR9           0x0324
 #define RCAR_GEN3_DBSC4_DBTR10          0x0328
 #define RCAR_GEN3_DBSC4_DBTR11          0x032C
 #define RCAR_GEN3_DBSC4_DBTR12          0x0330
 #define RCAR_GEN3_DBSC4_DBTR13          0x0334
 #define RCAR_GEN3_DBSC4_DBTR14          0x0338
 #define RCAR_GEN3_DBSC4_DBTR15          0x033C
 #define RCAR_GEN3_DBSC4_DBTR16          0x0340
 #define RCAR_GEN3_DBSC4_DBTR17          0x0344
 #define RCAR_GEN3_DBSC4_DBTR18          0x0348
 #define RCAR_GEN3_DBSC4_DBTR19          0x034C
 #define RCAR_GEN3_DBSC4_DBTR20          0x0350
 #define RCAR_GEN3_DBSC4_DBTR21          0x0354
 #define RCAR_GEN3_DBSC4_DBTR22          0x0358
 #define RCAR_GEN3_DBSC4_DBTR23          0x035C
 #define RCAR_GEN3_DBSC4_DBTR24          0x0360
 #define RCAR_GEN3_DBSC4_DBTR25          0x0364

 #define RCAR_GEN3_DBSC4_DBBL            0x0400
 #define RCAR_GEN3_DBSC4_DBRFCNF1        0x0414
 #define RCAR_GEN3_DBSC4_DBRFCNF2        0x0418
 #define RCAR_GEN3_DBSC4_DBCALCNF        0x0424

 #define RCAR_GEN3_DBSC4_DBRNK2          0x0438
 #define RCAR_GEN3_DBSC4_DBRNK3          0x043C
 #define RCAR_GEN3_DBSC4_DBRNK4          0x0440
 #define RCAR_GEN3_DBSC4_DBRNK5          0x0444
 #define RCAR_GEN3_DBSC4_DBPDNCNF        0x0450
 #define RCAR_GEN3_DBSC4_DBODT0          0x0460
 #define RCAR_GEN3_DBSC4_DBODT1          0x0464
 #define RCAR_GEN3_DBSC4_DBODT2          0x0468
 #define RCAR_GEN3_DBSC4_DBODT3          0x046C

 #define RCAR_GEN3_DBSC4_DBADJ0          0x0500
 #define RCAR_GEN3_DBSC4_DBDBICNT        0x0518
 #define RCAR_GEN3_DBSC4_DBDFIPMSTRCNF   0x0520
 #define RCAR_GEN3_DBSC4_DBDFICPDCNF     0x052C

 #define RCAR_GEN3_DBSC4_DBDFISTAT_0     0x0600
 #define RCAR_GEN3_DBSC4_DBDFISTAT_1     0x0640
 #define RCAR_GEN3_DBSC4_DBDFISTAT_2     0x0680
 #define RCAR_GEN3_DBSC4_DBDFISTAT_3     0x06C0

 #define RCAR_GEN3_DBSC4_DBDFICNT_0      0x0604
 #define RCAR_GEN3_DBSC4_DBDFICNT_1      0x0644
 #define RCAR_GEN3_DBSC4_DBDFICNT_2      0x0684
 #define RCAR_GEN3_DBSC4_DBDFICNT_3      0x06C4

 #define RCAR_GEN3_DBSC4_DBPDCNT0_0      0x0610
 #define RCAR_GEN3_DBSC4_DBPDCNT0_1      0x0650
 #define RCAR_GEN3_DBSC4_DBPDCNT0_2      0x0690
 #define RCAR_GEN3_DBSC4_DBPDCNT0_3      0x06D0

 #define RCAR_GEN3_DBSC4_DBPDCNT3_0      0x061C
 #define RCAR_GEN3_DBSC4_DBPDCNT3_1      0x065C
 #define RCAR_GEN3_DBSC4_DBPDCNT3_2      0x069C
 #define RCAR_GEN3_DBSC4_DBPDCNT3_3      0x06DC

 #define RCAR_GEN3_DBSC4_DBPDLK_0        0x0620
 #define RCAR_GEN3_DBSC4_DBPDLK_1        0x0660
 #define RCAR_GEN3_DBSC4_DBPDLK_2        0x06A0
 #define RCAR_GEN3_DBSC4_DBPDLK_3        0x06E0

 #define RCAR_GEN3_DBSC4_DBPDRGA_0       0x0624
 #define RCAR_GEN3_DBSC4_DBPDRGD_0       0x0628
 #define RCAR_GEN3_DBSC4_DBPDRGA_1       0x0664
 #define RCAR_GEN3_DBSC4_DBPDRGD_1       0x0668
 #define RCAR_GEN3_DBSC4_DBPDRGA_2       0x06A4
 #define RCAR_GEN3_DBSC4_DBPDRGD_2       0x06A8
 #define RCAR_GEN3_DBSC4_DBPDRGA_3       0x06E4
 #define RCAR_GEN3_DBSC4_DBPDRGD_3       0x06E8

 #define RCAR_GEN3_DBSC4_DBPDSTAT00      0x0630
 #define RCAR_GEN3_DBSC4_DBPDSTAT20      0x06B0

 #define RCAR_GEN3_DBSC4_DBBUS0CNF0      0x0800
 #define RCAR_GEN3_DBSC4_DBBUS0CNF1      0x0804

 #define RCAR_GEN3_DBSC4_DBCAM0CNF1      0x0904 //CAM Unit Configuration Register 1
 #define RCAR_GEN3_DBSC4_DBCAM0CNF2      0x0908 //CAM Unit Configuration Register 2
 #define RCAR_GEN3_DBSC4_DBCAM0CNF3      0x090C //CAM Unit Configuration Register 3
 #define RCAR_GEN3_DBSC4_DBCAM0STAT0     0x0980
 #define RCAR_GEN3_DBSC4_DBCAM1STAT0     0x0990
 #define RCAR_GEN3_DBSC4_DBSCHCNT0       0x1000 //Scheduler Unit Operation Setting Register 0
 #define RCAR_GEN3_DBSC4_DBSCHCNT1       0x1004 //Scheduler Unit Operation Setting Register 1
 #define RCAR_GEN3_DBSC4_DBSCHSZ0        0x1010 //Size Miss Scheduling Setting Register 0
 #define RCAR_GEN3_DBSC4_DBSCHRW0        0x1020 //Read/Write Scheduling Setting Register 0
 #define RCAR_GEN3_DBSC4_DBSCHRW1        0x1024 //Read/Write Scheduling Setting Register 1

 #define RCAR_GEN3_DBSC4_DBSCHQOS00      0x1030
 #define RCAR_GEN3_DBSC4_DBSCHQOS01      0x1034
 #define RCAR_GEN3_DBSC4_DBSCHQOS02      0x1038
 #define RCAR_GEN3_DBSC4_DBSCHQOS03      0x103C
 #define RCAR_GEN3_DBSC4_DBSCHQOS10      0x1040
 #define RCAR_GEN3_DBSC4_DBSCHQOS11      0x1044
 #define RCAR_GEN3_DBSC4_DBSCHQOS12      0x1048
 #define RCAR_GEN3_DBSC4_DBSCHQOS13      0x104C
 #define RCAR_GEN3_DBSC4_DBSCHQOS20      0x1050
 #define RCAR_GEN3_DBSC4_DBSCHQOS21      0x1054
 #define RCAR_GEN3_DBSC4_DBSCHQOS22      0x1058
 #define RCAR_GEN3_DBSC4_DBSCHQOS23      0x105C
 #define RCAR_GEN3_DBSC4_DBSCHQOS30      0x1060
 #define RCAR_GEN3_DBSC4_DBSCHQOS31      0x1064
 #define RCAR_GEN3_DBSC4_DBSCHQOS32      0x1068
 #define RCAR_GEN3_DBSC4_DBSCHQOS33      0x106C
 #define RCAR_GEN3_DBSC4_DBSCHQOS40      0x1070
 #define RCAR_GEN3_DBSC4_DBSCHQOS41      0x1074
 #define RCAR_GEN3_DBSC4_DBSCHQOS42      0x1078
 #define RCAR_GEN3_DBSC4_DBSCHQOS43      0x107C
 #define RCAR_GEN3_DBSC4_DBSCHQOS50      0x1080
 #define RCAR_GEN3_DBSC4_DBSCHQOS51      0x1084
 #define RCAR_GEN3_DBSC4_DBSCHQOS52      0x1088
 #define RCAR_GEN3_DBSC4_DBSCHQOS53      0x108C
 #define RCAR_GEN3_DBSC4_DBSCHQOS60      0x1090
 #define RCAR_GEN3_DBSC4_DBSCHQOS61      0x1094
 #define RCAR_GEN3_DBSC4_DBSCHQOS62      0x1098
 #define RCAR_GEN3_DBSC4_DBSCHQOS63      0x109C
 #define RCAR_GEN3_DBSC4_DBSCHQOS70      0x10A0
 #define RCAR_GEN3_DBSC4_DBSCHQOS71      0x10A4
 #define RCAR_GEN3_DBSC4_DBSCHQOS72      0x10A8
 #define RCAR_GEN3_DBSC4_DBSCHQOS73      0x10AC
 #define RCAR_GEN3_DBSC4_DBSCHQOS80      0x10B0
 #define RCAR_GEN3_DBSC4_DBSCHQOS81      0x10B4
 #define RCAR_GEN3_DBSC4_DBSCHQOS82      0x10B8
 #define RCAR_GEN3_DBSC4_DBSCHQOS83      0x10BC
 #define RCAR_GEN3_DBSC4_DBSCHQOS90      0x10C0
 #define RCAR_GEN3_DBSC4_DBSCHQOS91      0x10C4
 #define RCAR_GEN3_DBSC4_DBSCHQOS92      0x10C8
 #define RCAR_GEN3_DBSC4_DBSCHQOS93      0x10CC
 #define RCAR_GEN3_DBSC4_DBSCHQOS100     0x10D0
 #define RCAR_GEN3_DBSC4_DBSCHQOS101     0x10D4
 #define RCAR_GEN3_DBSC4_DBSCHQOS102     0x10D8
 #define RCAR_GEN3_DBSC4_DBSCHQOS103     0x10DC
 #define RCAR_GEN3_DBSC4_DBSCHQOS110     0x10E0
 #define RCAR_GEN3_DBSC4_DBSCHQOS111     0x10E4
 #define RCAR_GEN3_DBSC4_DBSCHQOS112     0x10E8
 #define RCAR_GEN3_DBSC4_DBSCHQOS113     0x10EC
 #define RCAR_GEN3_DBSC4_DBSCHQOS120     0x10F0
 #define RCAR_GEN3_DBSC4_DBSCHQOS121     0x10F4
 #define RCAR_GEN3_DBSC4_DBSCHQOS122     0x10F8
 #define RCAR_GEN3_DBSC4_DBSCHQOS123     0x10FC
 #define RCAR_GEN3_DBSC4_DBSCHQOS130     0x1100
 #define RCAR_GEN3_DBSC4_DBSCHQOS131     0x1104
 #define RCAR_GEN3_DBSC4_DBSCHQOS132     0x1108
 #define RCAR_GEN3_DBSC4_DBSCHQOS133     0x110C
 #define RCAR_GEN3_DBSC4_DBSCHQOS140     0x1110
 #define RCAR_GEN3_DBSC4_DBSCHQOS141     0x1114
 #define RCAR_GEN3_DBSC4_DBSCHQOS142     0x1118
 #define RCAR_GEN3_DBSC4_DBSCHQOS143     0x111C
 #define RCAR_GEN3_DBSC4_DBSCHQOS150     0x1120
 #define RCAR_GEN3_DBSC4_DBSCHQOS151     0x1124
 #define RCAR_GEN3_DBSC4_DBSCHQOS152     0x1128
 #define RCAR_GEN3_DBSC4_DBSCHQOS153     0x112C

 #define RCAR_GEN3_DBSC4_SCFCTST0        0x1700 //Schedule timing setting register 0
 #define RCAR_GEN3_DBSC4_SCFCTST1        0x1708 //Schedule timing setting register 1
 #define RCAR_GEN3_DBSC4_SCFCTST2        0x170C //Schedule timing setting register 2
 #define RCAR_GEN3_DBSC4_DBMRRDR0        0x1800
 #define RCAR_GEN3_DBSC4_DBMRRDR1        0x1804
 #define RCAR_GEN3_DBSC4_DBMRRDR2        0x1808
 #define RCAR_GEN3_DBSC4_DBMRRDR3        0x180C
 #define RCAR_GEN3_DBSC4_DBMRRDR4        0x1810
 #define RCAR_GEN3_DBSC4_DBMRRDR5        0x1814
 #define RCAR_GEN3_DBSC4_DBMRRDR6        0x1818
 #define RCAR_GEN3_DBSC4_DBMRRDR7        0x181C

/*
 * Thermal Sensor
 */
#define RCAR_GEN3_THS_TSC0               0xE6190000
#define RCAR_GEN3_THS_TSC1               0xE6198000
#define RCAR_GEN3_THS_TSC2               0xE61A0000
#define RCAR_GEN3_THS_TSC3               0xE61A8000
#define RCAR_GEN3_THS_SIZE               0x8000

/*
 * Audio Clock Generator
 */
#define RCAR_GEN3_ADG_BASE               0xEC5A0000
#define RCAR_GEN3_ADG_SIZE               0x200

 #define RCAR_GEN3_ADG_BRRA              0x00
 #define RCAR_GEN3_ADG_BRRB              0x04
 #define RCAR_GEN3_ADG_SSICKR            0x08
 #define RCAR_GEN3_ADG_CKSEL0            0x0C
 #define RCAR_GEN3_ADG_CKSEL1            0x10
 #define RCAR_GEN3_ADG_CKSEL2            0x14
 #define RCAR_GEN3_ADG_TIM_EN            0x30
 #define RCAR_GEN3_ADG_SRCIN_TIMSEL0     0x34
 #define RCAR_GEN3_ADG_SRCIN_TIMSEL1     0x38
 #define RCAR_GEN3_ADG_SRCIN_TIMSEL2     0x3C
 #define RCAR_GEN3_ADG_SRCIN_TIMSEL3     0x40
 #define RCAR_GEN3_ADG_SRCIN_TIMSEL4     0x44
 #define RCAR_GEN3_ADG_SRCOUT_TIMSEL0    0x48
 #define RCAR_GEN3_ADG_SRCOUT_TIMSEL1    0x4C
 #define RCAR_GEN3_ADG_SRCOUT_TIMSEL2    0x50
 #define RCAR_GEN3_ADG_SRCOUT_TIMSEL3    0x54
 #define RCAR_GEN3_ADG_SRCOUT_TIMSEL4    0x58
 #define RCAR_GEN3_ADG_CMDOUT_TIMSEL     0x5C
 #define RCAR_GEN3_ADG_DTCP_TIMSEL       0x64
 #define RCAR_GEN3_ADG_ADSP_TIMSEL1      0x80
 #define RCAR_GEN3_ADG_ADSP_TIMSEL2      0x84
 #define RCAR_GEN3_ADG_ADSP_TIMSEL3      0x88
 #define RCAR_GEN3_ADG_SSICKR1           0x100
 #define RCAR_GEN3_ADG_AVB_SYNC_SEL0     0x104
 #define RCAR_GEN3_ADG_AVB_SYNC_SEL1     0x10C
 #define RCAR_GEN3_ADG_AVB_SYNC_SEL2     0x110
 #define RCAR_GEN3_ADG_AVB_SYNC_DIV0     0x114
 #define RCAR_GEN3_ADG_AVB_SYNC_DIV1     0x118
 #define RCAR_GEN3_ADG_AVB_CLK_DIV0      0x11C
 #define RCAR_GEN3_ADG_AVB_CLK_DIV1      0x120
 #define RCAR_GEN3_ADG_AVB_CLK_DIV2      0x124
 #define RCAR_GEN3_ADG_AVB_CLK_DIV3      0x128
 #define RCAR_GEN3_ADG_AVB_CLK_DIV4      0x12C
 #define RCAR_GEN3_ADG_AVB_CLK_DIV5      0x130
 #define RCAR_GEN3_ADG_AVB_CLK_DIV6      0x134
 #define RCAR_GEN3_ADG_AVB_CLK_DIV7      0x138
 #define RCAR_GEN3_ADG_AVB_CLK_CONFIG    0x13C
 #define RCAR_GEN3_ADG_AVB_CLK_STATUS    0x140

/*
 * Video Capture
 */
#define RCAR_GEN3_VIN0_BASE              0xE6EF0000
#define RCAR_GEN3_VIN1_BASE              0xE6EF1000
#define RCAR_GEN3_VIN2_BASE              0xE6EF2000
#define RCAR_GEN3_VIN3_BASE              0xE6EF3000
#define RCAR_GEN3_VIN4_BASE              0xE6EF4000
#define RCAR_GEN3_VIN5_BASE              0xE6EF5000
#define RCAR_GEN3_VIN6_BASE              0xE6EF6000
#define RCAR_GEN3_VIN7_BASE              0xE6EF7000
/* V3H only */
#define RCAR_GEN3_VIN8_BASE              0xE6EF8000
#define RCAR_GEN3_VIN9_BASE              0xE6EF9000
#define RCAR_GEN3_VIN10_BASE             0xE6EFA000
#define RCAR_GEN3_VIN11_BASE             0xE6EFB000
#define RCAR_GEN3_VIN12_BASE             0xE6EFC000
#define RCAR_GEN3_VIN13_BASE             0xE6EFD000
#define RCAR_GEN3_VIN14_BASE             0xE6EFE000
#define RCAR_GEN3_VIN15_BASE             0xE6EFF000

#define RCAR_GEN3_VIN_SIZE               0x1000

 #define RCAR_GEN3_VIN_MC                0x0000
 #define RCAR_GEN3_VIN_MS                0x0004
 #define RCAR_GEN3_VIN_FC                0x0008
 #define RCAR_GEN3_VIN_SLPRC             0x000C
 #define RCAR_GEN3_VIN_ELPRC             0x0010
 #define RCAR_GEN3_VIN_SPPRC             0x0014
 #define RCAR_GEN3_VIN_EPPRC             0x0018
 #define RCAR_GEN3_VIN_CSI_IFMD          0x0020
 #define RCAR_GEN3_VIN_IS                0x002C
 #define RCAR_GEN3_VIN_LC                0x003C
 #define RCAR_GEN3_VIN_IE                0x0040
 #define RCAR_GEN3_VIN_INTS              0x0044
 #define RCAR_GEN3_VIN_SI                0x0048
 #define RCAR_GEN3_VIN_DMR               0x0058
 #define RCAR_GEN3_VIN_DMR2              0x005C
 #define RCAR_GEN3_VIN_UVAOF             0x0060
 #define RCAR_GEN3_VIN_UDS_CTRL          0x0080
 #define RCAR_GEN3_VIN_UDS_SCALE         0x0084
 #define RCAR_GEN3_VIN_UDS_PASS_BW       0x0090
 #define RCAR_GEN3_VIN_UDS_IPC           0x0098
 #define RCAR_GEN3_VIN_UDS_CLIPSIZE      0x00A4
 #define RCAR_GEN3_VIN_LUTP              0x0100
 #define RCAR_GEN3_VIN_LUTD              0x0104
 #define RCAR_GEN3_VIN_MB(x)             0x0030 + 0x04*x
 #define RCAR_GEN3_VIN_CSCC(x)           0x0064 + 0x04*x
 #define RCAR_GEN3_VIN_YCCR(x)           0x0228 + 0x04*x
 #define RCAR_GEN3_VIN_CBCCR(x)          0x0234 + 0x04*x
 #define RCAR_GEN3_VIN_CRCCR(x)          0x0240 + 0x04*x
 #define RCAR_GEN3_VIN_CSCE(x)           0x0300 + 0x04*x

/*
 * Camera Serial Interface 2
 */
#define RCAR_GEN3_CSI40_BASE             0xFEAA0000
#define RCAR_GEN3_CSI41_BASE             0xFEAB0000
#define RCAR_GEN3_CSI20_BASE             0xFEA80000
#define RCAR_GEN3_CSI21_BASE             0xFEA90000
#define RCAR_GEN3_CSI2_SIZE              0x10000

#define RCAR_GEN3_CSI2_TREF              0x00
#define RCAR_GEN3_CSI2_SRST              0x04
#define RCAR_GEN3_CSI2_PHYCNT            0x08
#define RCAR_GEN3_CSI2_CHKSUM            0x0C
#define RCAR_GEN3_CSI2_VCDT              0x10
#define RCAR_GEN3_CSI2_VCDT2             0x14
#define RCAR_GEN3_CSI2_FRDT              0x18
#define RCAR_GEN3_CSI2_FLD               0x1C
#define RCAR_GEN3_CSI2_ASTBY             0x20
#define RCAR_GEN3_CSI2_LNGDT0            0x28
#define RCAR_GEN3_CSI2_LNGDT1            0x2C
#define RCAR_GEN3_CSI2_INTEN             0x30
#define RCAR_GEN3_CSI2_INTCLOSE          0x34
#define RCAR_GEN3_CSI2_INTSTATE          0x38
#define RCAR_GEN3_CSI2_INTERRSTATE       0x3C
#define RCAR_GEN3_CSI2_SHPDAT            0x40
#define RCAR_GEN3_CSI2_SHPCNT            0x44
#define RCAR_GEN3_CSI2_LINKCNT           0x48
#define RCAR_GEN3_CSI2_LSWAP             0x4C
#define RCAR_GEN3_CSI2_PHTC              0x58
#define RCAR_GEN3_CSI2_PHYPLL            0x68
#define RCAR_GEN3_CSI2_PHEERM            0x74
#define RCAR_GEN3_CSI2_PHCLM             0x78
#define RCAR_GEN3_CSI2_PHDLM             0x7C

/*
 * CAN
 */
#define RCAR_GEN3_CAN0_BASE              0xE6C30000
#define RCAR_GEN3_CAN1_BASE              0xE6C38000
#define RCAR_GEN3_CAN_SIZE               0x600
#define RCAR_GEN3_CAN_REG_OFFSET         0x400

/*
 * CANFD
 */
#define RCAR_GEN3_CANFD_BASE             0xE66C0000
#define RCAR_GEN3_CANFD_SIZE             0xFFFF

/*
 * L2 Cache
 */
#define RCAR_GEN3_L2CPL310_BASE          0xF0100000

/*
 * LBSC
 */
#define RCAR_GEN3_LBSC_BASE              0xEE220000

 #define RCAR_GEN3_LBSC_CS0CTRL          0x200
 #define RCAR_GEN3_LBSC_CS1CTRL          0x204
 #define RCAR_GEN3_LBSC_CSWCR0           0x230
 #define RCAR_GEN3_LBSC_CSWCR1           0x234
 #define RCAR_GEN3_LBSC_CSPWCR0          0x280
 #define RCAR_GEN3_LBSC_CSPWCR1          0x284
 #define RCAR_GEN3_LBSC_EXWTSYNC         0x2A0
 #define RCAR_GEN3_LBSC_CS1GDST          0x2C0

/*
 * Timer Unit (TMU)
 */
#define RCAR_GEN3_TMU_BASE               0xE61E0004
#define RCAR_GEN3_TMU0_BASE              (RCAR_GEN3_TMU_BASE + 0x4)
#define RCAR_GEN3_TMU1_BASE              (RCAR_GEN3_TMU_BASE + 0x10)

 #define RCAR_GEN3_TMU_TSTR0             0x0
  #define RCAR_GEN3_TMU_START0           (1<<0)
  #define RCAR_GEN3_TMU_START1           (1<<1)
 #define RCAR_GEN3_TMU_TCOR              0x0
 #define RCAR_GEN3_TMU_TCNT              0x4
 #define RCAR_GEN3_TMU_TCR               0x8
  #define RCAR_GEN3_TMU_UNIE             (1 << 5)
  #define RCAR_GEN3_TMU_UNF              (1 << 8)

/*
 * Product Register (PRR)
 */
#define RCAR_GEN3_PRODUCT_REGISTER       0xFFF00044
#define RCAR_GEN3_PRODUCT_ID(x)          ((x & 0x7F00) >> 8)
 #define PRODUCT_ID_RCAR_H3              0x4F
 #define PRODUCT_ID_RCAR_M3W             0x52
 #define PRODUCT_ID_RCAR_M3N             0x55
 #define PRODUCT_ID_RCAR_V3M             0x54
 #define PRODUCT_ID_RCAR_V3H             0x56
 #define PRODUCT_ID_RCAR_D3              0x58
 #define PRODUCT_ID_RCAR_E3              0x57
#define RCAR_GEN3_PRODUCT_REV(x)         (x & 0xFF)
 #define PRODUCT_H3_REV_1_0              0x00
 #define PRODUCT_H3_REV_1_1              0x01
 #define PRODUCT_H3_REV_2_0              0x10
 #define PRODUCT_H3_REV_3_0              0x20
 #define PRODUCT_M3_REV_1_0              0x00
 #define PRODUCT_M3_REV_1_1              0x10
 #define PRODUCT_M3_REV_3_0              0x20
 #define PRODUCT_M3N_REV_1_0             0x00
 #define PRODUCT_M3N_REV_1_1             0x01
 #define PRODUCT_V3M_REV_1_0             0x00
 #define PRODUCT_V3M_REV_2_0             0x10
 #define PRODUCT_V3H_REV_1_0             0x00
 #define PRODUCT_V3H_REV_1_1             0x01
 #define PRODUCT_D3_REV_1_0              0x00
 #define PRODUCT_E3_REV_1_0              0x00
#define RCAR_GEN3_PRODUCT_CA57EN(x)      ((x >> 27) & 0xF)
#define RCAR_GEN3_PRODUCT_CA53EN(x)      ((x >> 22) & 0xF)

/* MFIS */
#define RCAR_GEN3_MFIS_BASE              (0xE6260000)
/* MFIS registers */
#define RCAR_GEN3_MFIS_WPCNTR            (0x900)   /* Write Protection Control Register*/
#define RCAR_GEN3_MFIS_WPCNTR_CODE       (0xACCE << 16)
#define RCAR_GEN3_MFIS_WPCNTR_WPD        (0x1 << 0)

/***************************
 * Interrupt ID
 ************************* */
#define RCAR_GEN3_INTCSYS_IRQ0           (  0 + 32)
#define RCAR_GEN3_INTCSYS_IRQ1           (  1 + 32)
#define RCAR_GEN3_INTCSYS_IRQ2           (  2 + 32)
#define RCAR_GEN3_INTCSYS_IRQ3           (  3 + 32)
#define RCAR_GEN3_INTCSYS_GPIO0          (  4 + 32)
#define RCAR_GEN3_INTCSYS_GPIO1          (  5 + 32)
#define RCAR_GEN3_INTCSYS_GPIO2          (  6 + 32)
#define RCAR_GEN3_INTCSYS_GPIO3          (  7 + 32)
#define RCAR_GEN3_INTCSYS_GPIO4          (  8 + 32)
#define RCAR_GEN3_INTCSYS_GPIO5          (  9 + 32)
#define RCAR_GEN3_INTCSYS_GPIO6          ( 10 + 32)
#define RCAR_GEN3_INTCSYS_GPIO7          ( 11 + 32)
#define RCAR_GEN3_INTCSYS_SCIF4          ( 16 + 32)
#define RCAR_GEN3_INTCSYS_SCIF5          ( 17 + 32)
#define RCAR_GEN3_INTCSYS_IRQ4           ( 18 + 32)
#define RCAR_GEN3_INTCSYS_I2C4           ( 19 + 32)
#define RCAR_GEN3_INTCSYS_I2C5           ( 20 + 32)
#define RCAR_GEN3_INTCSYS_I2C6           ( 21 + 32)
#define RCAR_GEN3_INTCSYS_DBSC4          ( 22 + 32)
#define RCAR_GEN3_INTCSYS_SCIF3          ( 23 + 32)
#define RCAR_GEN3_INTCSYS_ISP0_CH0       ( 25 + 32)
#define RCAR_GEN3_INTCSYS_ISP0_CH1       ( 26 + 32)
#define RCAR_GEN3_INTCSYS_ISP1_CH0       ( 27 + 32)
#define RCAR_GEN3_INTCSYS_CANFD_CH       ( 29 + 32)
#define RCAR_GEN3_INTCSYS_CANFD_GL       ( 30 + 32)
#define RCAR_GEN3_INTCSYS_RPC            ( 38 + 32)
#define RCAR_GEN3_INTCSYS_RAVB0          ( 39 + 32)
#define RCAR_GEN3_INTCSYS_RAVB1          ( 40 + 32)
#define RCAR_GEN3_INTCSYS_RAVB2          ( 41 + 32)
#define RCAR_GEN3_INTCSYS_RAVB3          ( 42 + 32)
#define RCAR_GEN3_INTCSYS_RAVB4          ( 43 + 32)
#define RCAR_GEN3_INTCSYS_RAVB5          ( 44 + 32)
#define RCAR_GEN3_INTCSYS_RAVB6          ( 45 + 32)
#define RCAR_GEN3_INTCSYS_RAVB7          ( 46 + 32)
#define RCAR_GEN3_INTCSYS_RAVB8          ( 47 + 32)
#define RCAR_GEN3_INTCSYS_RAVB9          ( 48 + 32)
#define RCAR_GEN3_INTCSYS_RAVB10         ( 49 + 32)
#define RCAR_GEN3_INTCSYS_RAVB11         ( 50 + 32)
#define RCAR_GEN3_INTCSYS_RAVB12         ( 51 + 32)
#define RCAR_GEN3_INTCSYS_RAVB13         ( 52 + 32)
#define RCAR_GEN3_INTCSYS_RAVB14         ( 53 + 32)
#define RCAR_GEN3_INTCSYS_RAVB15         ( 54 + 32)
#define RCAR_GEN3_INTCSYS_RAVB16         ( 55 + 32)
#define RCAR_GEN3_INTCSYS_RAVB17         ( 56 + 32)
#define RCAR_GEN3_INTCSYS_RAVB18         ( 57 + 32)
#define RCAR_GEN3_INTCSYS_RAVB19         ( 58 + 32)
#define RCAR_GEN3_INTCSYS_RAVB20         ( 59 + 32)
#define RCAR_GEN3_INTCSYS_RAVB21         ( 60 + 32)
#define RCAR_GEN3_INTCSYS_RAVB22         ( 61 + 32)
#define RCAR_GEN3_INTCSYS_RAVB23         ( 62 + 32)
#define RCAR_GEN3_INTCSYS_RAVB24         ( 63 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL0       ( 67 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL1       ( 68 + 32)
#define RCAR_GEN3_INTCSYS_THERMAL2       ( 69 + 32)
#define RCAR_GEN3_INTCSYS_USB3_1         ( 98 + 32)
#define RCAR_GEN3_INTCSYS_USB3_0         (102 + 32)
#define RCAR_GEN3_INTCSYS_SATA0          (105 + 32)
#define RCAR_GEN3_INTCSYS_EHCI0          (108 + 32)
#define RCAR_GEN3_INTCSYS_EHCI1          (112 + 32)
#define RCAR_GEN3_INTCSYS_EHCI2          (113 + 32)
#define RCAR_GEN3_INTCSYS_PCIE0          (116 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF2         (144 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF3         (145 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF4         (146 + 32)
#define RCAR_GEN3_INTCSYS_PCIE1          (148 + 32)
#define RCAR_GEN3_INTCSYS_SCIF0          (152 + 32)
#define RCAR_GEN3_INTCSYS_SCIF1          (153 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF0         (154 + 32)
#define RCAR_GEN3_INTCSYS_HSCIF1         (155 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF0         (156 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF1         (157 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF2         (158 + 32)
#define RCAR_GEN3_INTCSYS_MSIOF3         (159 + 32)
#define RCAR_GEN3_INTCSYS_IRQ5           (161 + 32)
#define RCAR_GEN3_INTCSYS_SCIF2          (164 + 32)
#define RCAR_GEN3_INTCSYS_SDHI0          (165 + 32)
#define RCAR_GEN3_INTCSYS_SDHI1          (166 + 32)
#define RCAR_GEN3_INTCSYS_SDHI2          (167 + 32)
#define RCAR_GEN3_INTCSYS_SDHI3          (168 + 32)
#define RCAR_GEN3_INTCSYS_VIN7           (171 + 32)
#define RCAR_GEN3_INTCSYS_IIC            (173 + 32)
#define RCAR_GEN3_INTCSYS_VIN4           (174 + 32)
#define RCAR_GEN3_INTCSYS_VIN5           (175 + 32)
#define RCAR_GEN3_INTCSYS_VIN6           (176 + 32)
#define RCAR_GEN3_INTCSYS_CAN0           (186 + 32)
#define RCAR_GEN3_INTCSYS_CAN1           (187 + 32)
#define RCAR_GEN3_INTCSYS_VIN0           (188 + 32)
#define RCAR_GEN3_INTCSYS_VIN1           (189 + 32)
#define RCAR_GEN3_INTCSYS_VIN2           (190 + 32)
#define RCAR_GEN3_INTCSYS_VIN3           (191 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_0     (200 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_1     (201 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_2     (202 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_3     (203 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_4     (204 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_5     (205 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_6     (206 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_7     (207 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_8     (208 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_9     (209 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_10    (210 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_11    (211 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_12    (212 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_13    (213 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_14    (214 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC0_15    (215 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_0     (216 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_1     (217 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_2     (218 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_3     (219 + 32)
#define RCAR_GEN3_INTCSYS_VIN8           (268 + 32)
#define RCAR_GEN3_INTCSYS_VIN9           (269 + 32)
#define RCAR_GEN3_INTCSYS_I2C2           (286 + 32)
#define RCAR_GEN3_INTCSYS_I2C0           (287 + 32)
#define RCAR_GEN3_INTCSYS_I2C1           (288 + 32)
#define RCAR_GEN3_INTCSYS_VIN10          (289 + 32)
#define RCAR_GEN3_INTCSYS_I2C3           (290 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI0       (291 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI1       (292 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI2       (293 + 32)
#define RCAR_GEN3_INTCSYS_DMASDHI3       (294 + 32)
#define RCAR_GEN3_INTCSYS_VIN11          (296 + 32)
#define RCAR_GEN3_INTCSYS_VIN12          (298 + 32)
#define RCAR_GEN3_INTCSYS_VIN13          (299 + 32)
#define RCAR_GEN3_INTCSYS_VIN14          (301 + 32)
#define RCAR_GEN3_INTCSYS_VIN15          (302 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_4     (308 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_5     (309 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_6     (310 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_7     (311 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_8     (312 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_9     (313 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_10    (314 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_11    (315 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_12    (316 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_13    (317 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_14    (318 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC1_15    (319 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_0   (320 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_1   (321 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_2   (322 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_3   (323 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_4   (324 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_5   (325 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_6   (326 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_7   (327 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_8   (328 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_9   (329 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_10  (330 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_11  (331 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_12  (332 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_13  (333 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_14  (334 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC0_15  (335 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_0   (336 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_1   (337 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_2   (338 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_3   (339 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_4   (340 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_5   (341 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_6   (342 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_7   (343 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_8   (344 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_9   (345 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_10  (346 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_11  (347 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_12  (348 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_13  (349 + 32)
#define RCAR_GEN3_INTCSYS_SCU0           (352 + 32)
#define RCAR_GEN3_INTCSYS_SCU1           (353 + 32)
#define RCAR_GEN3_INTCSYS_SCU2           (354 + 32)
#define RCAR_GEN3_INTCSYS_SCU3           (355 + 32)
#define RCAR_GEN3_INTCSYS_SCU4           (356 + 32)
#define RCAR_GEN3_INTCSYS_SCU5           (357 + 32)
#define RCAR_GEN3_INTCSYS_SCU6           (358 + 32)
#define RCAR_GEN3_INTCSYS_SCU7           (359 + 32)
#define RCAR_GEN3_INTCSYS_SCU8           (360 + 32)
#define RCAR_GEN3_INTCSYS_SCU9           (361 + 32)
#define RCAR_GEN3_INTCSYS_MLM0           (362 + 32)
#define RCAR_GEN3_INTCSYS_MLM1           (363 + 32)
#define RCAR_GEN3_INTCSYS_MLM2           (364 + 32)
#define RCAR_GEN3_INTCSYS_MLM3           (365 + 32)
#define RCAR_GEN3_INTCSYS_MLM4           (366 + 32)
#define RCAR_GEN3_INTCSYS_MLM5           (367 + 32)
#define RCAR_GEN3_INTCSYS_MLM6           (368 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_14  (382 + 32)
#define RCAR_GEN3_INTCSYS_AUDIODMAC1_15  (383 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_15    (397 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_0     (417 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_1     (418 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_2     (419 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_3     (420 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_4     (421 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_5     (422 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_6     (423 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_7     (424 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_8     (425 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_9     (426 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_10    (427 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_11    (428 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_12    (429 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_13    (430 + 32)
#define RCAR_GEN3_INTCSYS_SYSDMAC2_14    (431 + 32)

#endif /* __AARCH64_RCAR_GEN3_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/r-car-gen3.h $ $Rev: 912222 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/r-car-gen3.h $ $Rev: 912222 $" )
#endif
#endif
