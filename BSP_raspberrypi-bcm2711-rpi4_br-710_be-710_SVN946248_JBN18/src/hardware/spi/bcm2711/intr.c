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

static const struct sigevent *spi_intr(void *area, int id)
{
    bcm2711_spi_t *dev = area;
    uintptr_t base = dev->vbase;
    int i,expected;

    // Is DONE asserted ?
    if ((in32(base + BCM2711_SPI_CS) & (1 << SPI_CS_DONE)))
    {

        // If there's still data to send
        if (dev->tlen < dev->xlen)
        {
                for(i = 0; i < BCM2711_SPI_FIFOLEN; i += dev->dlen)
                {
                    if (dev->tlen >= dev->xlen)
                        break;

                    switch (dev->dlen)
                    {
                        case 1:
                            out32((base + BCM2711_SPI_FIFO), *(dev->pbuf + dev->tlen));
                        break;
                        case 2:
                            out32((base + BCM2711_SPI_FIFO), *(uint16_t *)(dev->pbuf + dev->tlen));
                        break;
                        default:
                            out32((base + BCM2711_SPI_FIFO), *(uint32_t *)(dev->pbuf + dev->tlen));
                        break;
                    }

                    dev->tlen += dev->dlen;
                }
        }
        else
        {
            // There's no more data to write, set TA to 0
            out32((base + BCM2711_SPI_CS), (in32(base + BCM2711_SPI_CS) & ~(SPI_CS_TA_ACTIVE << SPI_CS_TA)));

            // Read trailing data from FIFO until RXD = 0
            while ((in32(base + BCM2711_SPI_CS) & (SPI_CS_RXD_FIFONOTEMPTY << SPI_CS_RXD)))
            {
                    switch (dev->dlen)
                    {
                        case 1:
                            *(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                        case 2:
                            *(uint16_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                        default:
                            *(uint32_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                    }

                    dev->rlen += dev->dlen;
            }

            // Exchange done...
            return (&dev->spievent);
        }
     }

     // Is RXR asserted (RX FIFO full) ?
     if ((in32(base + BCM2711_SPI_CS) & (SPI_CS_RXR_FIFOFULL << SPI_CS_RXR)))
     {
            // Bytes available in the Rx FIFO, read 12 bytes
            for(i = 0; i < BCM2711_SPI_RD_LEN; i += dev->dlen)
            {
                    switch (dev->dlen)
                    {
                        case 1:
                            *(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                        case 2:
                            *(uint16_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                        default:
                            *(uint32_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                            break;
                    }

                    dev->rlen += dev->dlen;
            }

            // Send 12 bytes or what number of bytes remains to be sent
            expected = min(BCM2711_SPI_RD_LEN, dev->xlen - dev->tlen);
            if (expected > 0)
            {
                    for (i = 0; i < expected; i += dev->dlen)
                    {
                        switch(dev->dlen)
                        {
                            case 1:
                                out32((base + BCM2711_SPI_FIFO), *(dev->pbuf + dev->tlen));
                                break;
                            case 2:
                                out32((base + BCM2711_SPI_FIFO), *(uint16_t *)(dev->pbuf + dev->tlen));
                                break;
                            default:
                                out32((base + BCM2711_SPI_FIFO), *(uint32_t *)(dev->pbuf + dev->tlen));
                                break;
                        }

                        dev->tlen += dev->dlen;
                    }
            }
            else
            {
                // Read trailing data from FIFO until RXD = 0
                while ((in32(base + BCM2711_SPI_CS) & (SPI_CS_RXD_FIFONOTEMPTY << SPI_CS_RXD)))
                {
                     switch (dev->dlen)
                     {
                            case 1:
                                *(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                                break;
                            case 2:
                                *(uint16_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                                break;
                            default:
                                *(uint32_t *)(dev->pbuf + dev->rlen) = in32((base + BCM2711_SPI_FIFO));
                                break;
                     }

                     dev->rlen += dev->dlen;
                }

            }
     }

    return NULL;
}

int bcm2711_attach_intr(bcm2711_spi_t *bcm2711)
{
    if ((bcm2711->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
    {
        return -1;
    }

    if ((bcm2711->coid = ConnectAttach(0, 0, bcm2711->chid, _NTO_SIDE_CHANNEL, 0)) == -1)
    {
        goto fail0;
    }

    bcm2711->spievent.sigev_notify      = SIGEV_PULSE;
    bcm2711->spievent.sigev_coid        = bcm2711->coid;
    bcm2711->spievent.sigev_code        = BCM2711_SPI_EVENT;
    bcm2711->spievent.sigev_priority    = 1;

    /*
     * Attach SPI interrupt
     */
    bcm2711->iid = InterruptAttach(bcm2711->irq_spi, spi_intr, bcm2711, 0, _NTO_INTR_FLAGS_TRK_MSK);

    if (bcm2711->iid != -1)
    {
        return 0;
    }

    ConnectDetach(bcm2711->coid);
fail0:
    ChannelDestroy(bcm2711->chid);

    return -1;
}

int bcm2711_dma_attach_intr(bcm2711_spi_t *bcm2711)
{
    struct sigevent	event;

    if ((bcm2711->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
    {
        spi_error("%s: chid failed", __func__);
        return -1;
    }

    if ((bcm2711->dma_coid = ConnectAttach(0, 0, bcm2711->chid, _NTO_SIDE_CHANNEL, 0)) == -1)
    {
        spi_error("%s: DMA coid failed", __func__);
        goto fail0;
    }

    event.sigev_notify      = SIGEV_PULSE;
    event.sigev_coid        = bcm2711->dma_coid;
    event.sigev_code        = BCM2711_SPI_DMA_EVENT;
    event.sigev_priority    = 1;

    bcm2711->iid_dma = InterruptAttachEvent(bcm2711->dma_rx_irq, &event, _NTO_INTR_FLAGS_TRK_MSK);

    if (bcm2711->iid_dma != -1)
    {
        return 0;
    }

    ConnectDetach(bcm2711->dma_coid);
fail0:
    ChannelDestroy(bcm2711->chid);
    return -1;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/intr.c $ $Rev: 919018 $")
#endif
