/*
 * $QNXLicenseC:
 * Copyright 2019, QNX Software Systems.
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

#ifndef __HWINFO_ROCKCHIP_H_INCLUDED
#define __HWINFO_ROCKCHIP_H_INCLUDED

#include <hw/sysinfo.h>

/*
 * =============================================================================
 *
 *                                B U S E S
 *
 * =============================================================================
*/
#define RK_HWI_I2C                        HWI_ITEM_BUS_I2C
#define RK_HWI_IIC                        "iic"

#define RK_HWI_SPI                        HWI_ITEM_BUS_SPI
#define RK_HWI_SPI_DEV                    "spi_dev"

#define RK_HWI_SDHI                       HWI_ITEM_BUS_SDIO

/*
 * =============================================================================
 *
 *                              D E V I C E S
 *
 * =============================================================================
*/
#define RK_HWI_UART                       "uart"

#define RK_HWI_SYSDMAC                    "sys"

#define RK_HWI_WDT                        "wdog"

#define RK_HWI_THERMAL                    "thermal"

#endif //__HWINFO_ROCKCHIP_H_INCLUDED

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/hw/hwinfo_rockchip.h $ $Rev: 932064 $")
#endif
