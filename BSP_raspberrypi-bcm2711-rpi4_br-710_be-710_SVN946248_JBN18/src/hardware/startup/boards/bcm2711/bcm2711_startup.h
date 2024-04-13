/*
 * $QNXLicenseC:
 * Copyright 2020 QNX Software Systems.
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

#ifndef __BCM2711_BOARD_H__
#define __BCM2711_BOARD_H__

#define BCM2711_WDT_USE_DEFAULT (-1)

#define DRIVE_LOW   0
#define DRIVE_HIGH  1

#define PULL_NONE   0
#define PULL_UP     1
#define PULL_DOWN   2

#define FUNC_IP     0
#define FUNC_OP     1
#define FUNC_A0     4
#define FUNC_A1     5
#define FUNC_A2     6
#define FUNC_A3     7
#define FUNC_A4     3
#define FUNC_A5     2

typedef struct bcm2711_pinmux_t {
    uint8_t pin_num;
    uint8_t fsel;
    uint8_t pull;
    uint8_t gpio_lvl;
    uint8_t is_last;
} bcm2711_pinmux;

typedef struct bcm2711_config_t {
    bcm2711_pinmux *pinmux;
} bcm2711_config;


bcm2711_config *bcm2711_get_board_config(size_t *num_devs);
const unsigned get_memory_size(void);
const char *get_soc_name(void);
const char *get_mfr_name(void);
const char * get_board_name(void);
const char *get_board_serial(void);
void bcm2711_init_raminfo(void);
void init_board_type(void);
void bcm2711_wdt_enable(int timeout_value);
void init_board(void);
void init_miniuart(unsigned channel, const char *init, const char *defaults);
void put_miniuart(int c);

#endif /* __BCM2711_BOARD_H__ */
#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/bcm2711_startup.h $ $Rev: 930301 $")
#endif
