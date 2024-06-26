#
# Copyright 2020, QNX Software Systems.
#
# Licensed under the Apache License, Version 2.0 (the "License"). You
# may not reproduce, modify or distribute this software except in
# compliance with the License. You may obtain a copy of the License
# at: http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTIES OF ANY KIND, either express or implied.
#
# This file may contain contributions from others, either as
# contributors under the License or as licensors under other terms.
# Please review this entire file for other proprietary rights or license
# notices, as well as the QNX Development Suite License Guide at
# http://licensing.qnx.com/license-guide/ for other information.
#

/*
 * BCM2711 system reboot
 */

#include "callout.ah"

/*
 * -----------------------------------------------------------------------
 * Routine to patch callout code
 *
 * Patcher routine takes the following arguments:
 *	x0 - syspage paddr
 *	x1 - vaddr of callout
 *	x2 - offset from start of syspage to start of callout routine
 *	x3 - offset from start of syspage to rw storage
 *	x4 - patch data
 *	x5 - callout_rtn
 */

rw_size_reboot: .word 8

patch_reboot:
	sub		sp, sp, #16
	stp		x19, x30, [sp]

	add		x2, x0, x2		// address of callout routine
	and		x3, x3, #0xffff
	add		x19, x0, x3		// address of rw data

	// Patch callout with RW offset in w3
	CALLOUT_PATCH_RW	x2, w3, w6, w7

	// Map the register space and store in rw area
	mov		x0, #0x1000
	mov		x1, #0xfe100000
	bl		callout_io_map
	str		x0, [x19]

	ldp		x19, x30, [sp]
	add		sp, sp, #16
	ret

/*
 * On entry:
 * x0 - _syspage_ptr
 */

#define RSTC	0x1c
#define WDOG	0x24
#define PASSWORD	0x5a000000
#define RESET_CMD	0x20

CALLOUT_START(reboot_bcm2711, rw_size_reboot, patch_reboot)
	mov		w7, #0xabcd			// offset of rw data (patched)
	add		x7, x7, x0			// address of rwdata
	ldr		x5, [x7]			// register base

	/*
	 * set up the watch dog timer to something really small
	 * and let it trigger the reboot
	 */
	mov		w1, #PASSWORD

	mov		w0, #10				// 10 ticks
	orr		w0, w0, w1
	str		w0, [x5, #WDOG]

	ldr		w0, [x5, #RSTC]
	bic		w0, w0, #0x30
	orr		w0, w0, w1
	orr		w0, w0, #RESET_CMD
	str		w0, [x5, #RSTC]

0:	b		0b					// wait for reboot
CALLOUT_END(reboot_bcm2711)

#ifdef __QNXNTO__
#ifdef __USESRCVERSION
.section .ident, "SM",%progbits,1;
.asciz "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/callout_reboot_bcm2711.S $ $Rev: 919977 $";
.previous
#endif
#endif
