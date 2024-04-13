/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#ifndef __HWINFO_BCM2711_H_INCLUDED
#define __HWINFO_BCM2711_H_INCLUDED

#include <hw/sysinfo.h>

/*
 * =============================================================================
 *                                B U S E S
 * =============================================================================
*/

/*
 * =============================================================================
 *                              D E V I C E S
 * =============================================================================
*/

/* GENET Ethernet */
#define BCM2711_HWI_GENET         "genet"

#endif  /* __HWINFO_BCM2711_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/hw/hwinfo_bcm2711.h $ $Rev: 928388 $")
#endif
