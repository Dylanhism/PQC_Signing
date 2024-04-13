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

#include "bcm2711spi.h"

int bcm2711spi_wait(bcm2711_spi_t *dev, int len)
{
    struct _pulse pulse;

    while (1)
    {
        if (len)
        {
            uint64_t to = dev->dtime;
            to *= (len * 50); /* 50 times for time out */
            TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, NULL, &to, NULL);
        }

        if (MsgReceivePulse(dev->chid, &pulse, sizeof(pulse), NULL) == -1)
        {
            return -1;
        }

        switch (pulse.code)
        {
            case BCM2711_SPI_EVENT:
            {
                return 0;
            }

            case BCM2711_SPI_DMA_EVENT:
            {
                spi_debug("%s: DMA Event!", __func__);
                InterruptUnmask(dev->dma_rx_irq, dev->iid_dma);
                return 0;
            }

            default:
                return -1;
        }
    }

    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/wait.c $ $Rev: 919018 $")
#endif
