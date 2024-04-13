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

#ifndef IMX_WDOG_H_
#define IMX_WDOG_H_

/*
 * Watchdog Timer Registers, offset from base address
 */
#define IMX_WDOG_WCR                   0x00
#define IMX_WDOG_WSR                   0x02
#define IMX_WDOG_WRSR                  0x04
#define IMX_WDOG_WICR                  0x06
#define IMX_WDOG_WMCR                  0x08


/* WDOG_WCR bit fields */
#define IMX_WDOG_WCR_WT_MASK           (0xff << 8)
#define IMX_WDOG_WCR_WDW_MASK          (0x01 << 7)
#define IMX_WDOG_WCR_SRE_MASK          (0x01 << 6)
#define IMX_WDOG_WCR_WDA_MASK          (0x01 << 5)
#define IMX_WDOG_WCR_SRS_MASK          (0x01 << 4)
#define IMX_WDOG_WCR_WDT_MASK          (0x01 << 3)
#define IMX_WDOG_WCR_WDE_MASK          (0x01 << 2)
#define IMX_WDOG_WCR_WDBG_MASK         (0x01 << 1)
#define IMX_WDOG_WCR_WDZST_MASK        (0x01 << 0)

#endif /* IMX_WDOG_H_ */
#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_wdog.h $ $Rev: 869196 $")
#endif
