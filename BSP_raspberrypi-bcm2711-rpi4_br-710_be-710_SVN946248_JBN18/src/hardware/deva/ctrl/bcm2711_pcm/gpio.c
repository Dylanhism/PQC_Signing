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

#include "gpio.h"
#include <aarch64/bcm2711.h>

void gpio_fsel(volatile uint32_t *base, unsigned pin, gpio_fsel_t fun) {
    volatile uint32_t *gpfsel = base + BCM2711_GPIO_FSEL0/sizeof *base + pin / 10;
    unsigned group = 3 * (pin % 10); // 10 3-bit pins per register
    unsigned mask = 7 << group;

    *gpfsel = (*gpfsel & ~mask) | (fun << group);
}

void gpio_pull(volatile uint32_t *base, unsigned pin, gpio_pull_t pull) {
    volatile uint32_t *gppull = base + BCM2711_GPIO_PULL0/sizeof *base + pin / 16;
    unsigned group = 2 * (pin % 16); // 16 2-bit pins per register
    unsigned mask = 3 << group;

    *gppull = (*gppull & ~mask) | (pull << group);
}

void gpio_fsel_pull(volatile uint32_t *base, unsigned pin, gpio_fsel_t fun, gpio_pull_t pull) {
    gpio_fsel(base, pin, fun);
    gpio_pull(base, pin, pull);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/gpio.c $ $Rev: 924476 $")
#endif
