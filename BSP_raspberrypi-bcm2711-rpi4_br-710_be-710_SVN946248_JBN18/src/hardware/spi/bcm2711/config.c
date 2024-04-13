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

int bcm2711_cfg(void *hdl, spi_cfg_t *cfg, uint32_t *clientctrl)
{
    bcm2711_spi_t *dev = hdl;
    uint32_t divisor = 0;

    if (cfg == NULL)
        return -1;

    switch((cfg->mode & SPI_MODE_CHAR_LEN_MASK))
    {
        case 8:
            if (dev->opmode != INTERRUPT) {
                spi_error("%s: Only 8bit dlen is supported in interrupt mode", __func__);
                return -1;
            }
            break;
        case 32:
            if (dev->opmode != DMA) {
                spi_error("%s: Only 32bit dlen is supported in dma mode", __func__);
                return -1;
            }
            break;
        case 16:
        default:
            spi_error("%s: %d bit data width is not supported by this controller",
                                __func__, cfg->mode & SPI_MODE_CHAR_LEN_MASK);
            return -1;
    }

    /* Gather default CS and clock polarity related configuration
     * The SPI module is supplied with a 500MHz reference clock.
     */
    if (cfg->mode & SPI_MODE_CSPOL_HIGH)
    {
        *clientctrl |= (SPI_CS_CSPOL_HIGH << SPI_CS_CSPOL);	/* CS Polarity: CS active high */
    }

    if (cfg->mode & SPI_MODE_CKPOL_HIGH)
    {
        *clientctrl |= (SPI_CS_CPOL_HIGH << SPI_CS_CPOL);	/* Clock Polarity: Rest state of clock high */
    }

    if (cfg->mode & SPI_MODE_CKPHASE_HALF)
    {
        *clientctrl |= (SPI_CS_CPHA_BEGIN << SPI_CS_CPHA);	/* Clock Phase: SCLK beginning of data bit */
    }

    /* Calculate the SPI target operational speed.
     * The SPI module is supplied with a 500MHz reference clock.
     */
    divisor = (dev->clock + (cfg->clock_rate / 2)) / cfg->clock_rate;
    spi_debug("%s:target_sclk=%d, dvif=%f, div=%d", __func__,
                    cfg->clock_rate, (float)dev->clock / cfg->clock_rate, divisor);

    if (divisor < BCM2711_SPI_MIN_CDIV) {
        divisor = BCM2711_SPI_MIN_CDIV;
    }
    else if (divisor >= BCM2711_SPI_MAX_CDIV) {
        divisor = BCM2711_SPI_MAX_CDIV;
    }
    else
    {
        /* Divisor must be the multiple of 2. TRM suggests to round down
         * the odded number, but it seems we can get more closed rate
         * by rounding up
         */
        //divisor = divisor - (divisor % 2);
        divisor = divisor + (divisor % 2);
    }
    /* updated the best rate */
    cfg->clock_rate = dev->clock / divisor;

    spi_debug("%s: sclk=%d, divisor=%d", __func__, cfg->clock_rate, divisor);
    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/config.c $ $Rev: 919018 $")
#endif
