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

#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

typedef enum gpio_fsel {
    FSEL_INPUT  = 0,
    FSEL_OUTPUT = 1,
    FSEL_FUN0   = 4,
    FSEL_FUN1   = 5,
    FSEL_FUN2   = 6,
    FSEL_FUN3   = 7,
    FSEL_FUN4   = 3,
    FSEL_FUN5   = 2,
} gpio_fsel_t;

typedef enum gpio_pull {
    PULL_NONE   = 0,
    PULL_UP     = 1,
    PULL_DOWN   = 2,
} gpio_pull_t;

/*- gpio -*/

void gpio_fsel(volatile uint32_t *base, unsigned pin, gpio_fsel_t fun);
void gpio_pull(volatile uint32_t *base, unsigned pin, gpio_pull_t pull);
void gpio_fsel_pull(volatile uint32_t *base, unsigned pin, gpio_fsel_t fun, gpio_pull_t pull);

#endif /* __GPIO_H__ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pwm/gpio.h $ $Rev: 923583 $")
#endif
