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

#include "proto.h"

int bcm2711_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed)
{
    bcm2711_dev_t	*dev = hdl;
    unsigned int ckdiv = 0;

    dev->speed = speed;

    ckdiv = (BCM2711_CORE_FREQ / dev->speed);
    if (ckdiv > CLKTREG_BIT_MSK) {

        slogf(_SLOG_SETCODE(_SLOGC_TEST, 0) , _SLOG_ERROR,
                            "Invalid clkdiv %d, bus speed %d  \n", dev->clk, dev->speed);
        errno = EINVAL;
        return -1;
    }

    out32(dev->regbase + BCM2711_I2C_CLKDIVREG_OFF, ckdiv);
    dev->clk = ckdiv;

    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/i2c/bcm2711/bus_speed.c $ $Rev: 919272 $")
#endif
