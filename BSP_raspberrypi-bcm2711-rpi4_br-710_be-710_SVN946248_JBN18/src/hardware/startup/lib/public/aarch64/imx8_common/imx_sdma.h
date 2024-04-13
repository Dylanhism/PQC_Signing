/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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

#ifndef IMX_SDMA_H_
#define IMX_SDMA_H_

/*
 * SDMA Base Address
 */
#define IMX_SDMA_BASE                  0x5A2A0000
#define IMX_SDMA_IRQ                   250
#define IMX_SDMA_SIZE                  0x10000

/* SDMA Registers, offset from base address */
#define IMX_SDMA_MC0PTR                0x00    /* AP (MCU) Channel 0 Pointer */
#define IMX_SDMA_INTR                  0x04    /* Channel Interrupts */
#define IMX_SDMA_STOP_STAT             0x08    /* Channel Stop / Channel Status */
#define IMX_SDMA_HSTART                0x0C    /* Channel Start */
#define IMX_SDMA_EVTOVR                0x10    /* Channel Event Override */
#define IMX_SDMA_DSPOVR                0x14    /* Channel DSP (BP) Override */
#define IMX_SDMA_HOSTOVR               0x18    /* Channel AP Override */
#define IMX_SDMA_EVTPEND               0x1C    /* Channel Event Pending */
#define IMX_SDMA_RESET                 0x24    /* Reset Register */
#define IMX_SDMA_EVTERR                0x28    /* DMA Request Error Register */
#define IMX_SDMA_INTRMASK              0x2C    /* Channel AP Interrupt Mask */
#define IMX_SDMA_PSW                   0x30    /* Schedule Status */
#define IMX_SDMA_EVTERRDBG             0x34    /* DMA Request Error Register */
#define IMX_SDMA_CONFIG                0x38    /* Configuration Register */
#define IMX_SDMA_ONCE_ENB              0x40    /* OnCE Enable */
#define IMX_SDMA_ONCE_DATA             0x44    /* OnCE Data Register */
#define IMX_SDMA_ONCE_INSTR            0x48    /* OnCE Instruction Register */
#define IMX_SDMA_ONCE_STAT             0x4C    /* OnCE Status Register */
#define IMX_SDMA_ONCE_CMD              0x50    /* OnCE Command Register */
#define IMX_SDMA_EVT_MIRROT            0x54    /* DMA Request */
#define IMX_SDMA_ILLINSTADDR           0x58    /* Illegal Instruction Trap Address */
#define IMX_SDMA_CHN0ADDR              0x5C    /* Channel 0 Boot Address */
#define IMX_SDMA_XTRIG_CONF1           0x70    /* Cross-Trigger Events Configuration Register 1 */
#define IMX_SDMA_XTRIG_CONF2           0x74    /* Cross-Trigger Events Configuration Register 2 */
#define IMX_SDMA_CHNPRI(n)             (0x100 + ((n)<<2))   /* Channel Priority n = 0 to 31 */
#define IMX_SDMA_CHNENBL(n)            (0x200 + ((n)<<2))   /* Channel Enable n = 0 to 47 */


#endif /* IMX_SDMA_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_sdma.h $ $Rev: 869196 $")
#endif
