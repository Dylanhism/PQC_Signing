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

#ifndef __MBOX_H__
#define __MBOX_H__

#include <stddef.h>
#include <stdint.h>

#define	MBOX_CLK_EMMC   1
#define	MBOX_CLK_UART   2
#define	MBOX_CLK_ARM    3
#define	MBOX_CLK_CORE   4
#define	MBOX_CLK_V3D    5
#define	MBOX_CLK_H264   6
#define	MBOX_CLK_ISP    7
#define	MBOX_CLK_SDRAM  8
#define	MBOX_CLK_PIXEL  9
#define	MBOX_CLK_PWM    10
#define	MBOX_CLK_EMMC2  12

uint32_t mbox_get_board_revision(void);
void mbox_get_board_mac_address(uint8_t *address);
uint32_t mbox_get_clock_rate(uint32_t id);
uint32_t mbox_set_clock_rate(uint32_t id, uint32_t rate);
char *mbox_get_cmdline(void);
uint32_t mbox_get_voltage(uint32_t id);
int32_t  mbox_get_temperature(uint32_t id);
uint64_t mbox_get_board_serial(void);
uint32_t mbox_get_arm_memory(void);
uint32_t mbox_get_vc_memory(void);
uint32_t mbox_get_power_state(uint32_t id);
uint32_t mbox_set_power_state(uint32_t id, uint32_t state);
uint32_t mbox_get_expgpio(uint32_t gpio);
uint32_t mbox_set_expgpio(uint32_t gpio, uint32_t state);

#endif // __MBOX_H__

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/mbox.h $ $Rev: 928235 $")
#endif
