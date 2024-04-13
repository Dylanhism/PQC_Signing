
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

#include <assert.h>
#include "proto.h"

#define I2C_WAIT_TRANSFER_TMO_COUNT     10000

static i2c_status_t bcm2711_check_status(bcm2711_dev_t *dev)
{
    dev->status = in32(dev->regbase + BCM2711_I2C_STATREG_OFF) & STATREG_MASK;

    if (dev->status & STATREG_ERR) {
        bcm2711_i2c_slogf(dev, _SLOG_ERROR, "%s:  NACK",__FUNCTION__);
        return I2C_STATUS_NACK;
    }

    if (dev->status & STATREG_DONE) {
        dev->txrx = BCM2711_I2C_IDLE;
        return I2C_STATUS_DONE;
    }

    return I2C_STATUS_ERROR;
}

i2c_status_t bcm2711_wait_complete(bcm2711_dev_t *dev)
{
    uint64_t    ntime;
    int         interr = EOK;

    /* Wait for isr return*/
    ntime = (uint64_t) ( (uint64_t)(10000000)
            * (uint64_t)(10000000)/(uint64_t)dev->speed );

    TimerTimeout(CLOCK_MONOTONIC, _NTO_TIMEOUT_INTR, NULL, &ntime, NULL);
    interr = InterruptWait(0, NULL);
    if (interr == ETIMEDOUT) {
        bcm2711_i2c_slogf(dev, _SLOG_ERROR, "%s:  Timeout, total data %d sent data %d",
                                                __FUNCTION__, dev->tot_len, dev->cur_len);
        return I2C_STATUS_BUSY;
    }

    return bcm2711_check_status(dev);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/i2c/bcm2711/wait.c $ $Rev: 919272 $")
#endif
