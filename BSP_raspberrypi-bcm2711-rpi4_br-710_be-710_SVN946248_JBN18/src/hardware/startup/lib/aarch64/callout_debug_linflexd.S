#
# Copyright 2014, QNX Software Systems. 
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
 * LINFlexD UART specific polled serial I/O
 *
 * This should be usable by any board that uses a LINFLexD UART.
 */

#include "callout.ah"
#include <hw/linflexd.h>


/*
 * Patch interrupt callouts to access rw data.
 * The first call will also map the uart.
 *
 * Patcher routine takes the following arguments:
 *	x0 - syspage paddr
 *	x1 - vaddr of callout
 *	x2 - offset from start of syspage to start of callout routine
 *	x3 - offset from start of syspage to rw storage
 *	x4 - patch data
 *	x5 - callout_rtn
 */

patch_debug:
	sub	sp, sp, #16
	stp	x19, x30, [sp]

	add	x19, x0, x2				// x19 = address of callout routine

	/*
	 * Map UART using patch_data parameter
	 */
	mov	x0, #0x1000
	ldr	x1, [x4]
	bl	callout_io_map

	/*
	 * Patch callout with mapped virtual address in x0
	 */
	CALLOUT_PATCH	x19, w6, w7

 	ldp	x19, x30, [sp]
	add	sp, sp, #16
	ret

/*
 * -----------------------------------------------------------------------
 * void	display_char_linflexd(struct sypage_entry *, char)
 *
 * x0: syspage pointer
 * x1: character
 * -----------------------------------------------------------------------
 */
CALLOUT_START(display_char_linflexd, 0, patch_debug)
	mov	x7, #0xabcd				// LINFLEXD base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

	mov     w2, #0x400

	// Write character to buffer's least significant byte
	and	w0, w1, #0xff
	strb	w0, [x7, #LINFLEXD_BDRL]

	// Wait for data transmission completed by looping until LINFLEXD_UARTSR_DTFTFF is set
0:
	subs   w2, w2, #1
	beq    1f

	ldr	w0, [x7, #LINFLEXD_UARTSR]
	tst	w0, #LINFLEXD_UARTSR_DTFTFF
	b.ne	0b

1:
	ret
CALLOUT_END(display_char_linflexd)


/*
 * -----------------------------------------------------------------------
 * char	poll_key_linflexd(struct sypage_entry *)
 *
 * x0: syspage pointer
 * -----------------------------------------------------------------------
 */
CALLOUT_START(poll_key_linflexd, 0, patch_debug)
	mov	x7, #0xabcd				// UART base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

	// Check if a reception of data has completed, if no data has been received then exit
	ldr	w1, [x7, #LINFLEXD_UARTSR]
	tst	w1, #LINFLEXD_UARTSR_DRFRFE

	b.ne 0f

	// read from MSB and store in w0
	ldrb	w0, [x7, #LINFLEXD_BDRM]
	ret

	// If no bytes are read we return -1 in w0
0:	mov		w0, #-1
	ret
CALLOUT_END(poll_key_linflexd)


/*
 * -----------------------------------------------------------------------
 * int break_detect(struct syspage_entry *)
 *
 * x0: syspage pointer
 *
 * Return 1 if break detected, 0 otherwise
 * -----------------------------------------------------------------------
 */
CALLOUT_START(break_detect_linflexd, 0, patch_debug)
	mov		x7, #0xabcd				// UART base address (patched)
	movk	x7, #0xabcd, lsl #16
	movk	x7, #0xabcd, lsl #32
	movk	x7, #0xabcd, lsl #48

	/*
	 * Not clear that anything is required for linflexd module to deal with break
	 */
	mov		w0, #0
	ret
CALLOUT_END(break_detect_linflexd)

#ifdef __QNXNTO__
#ifdef __USESRCVERSION
.section .ident, "SM",%progbits,1;
.asciz "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/aarch64/callout_debug_linflexd.S $ $Rev: 937002 $";
.previous
#endif
#endif
