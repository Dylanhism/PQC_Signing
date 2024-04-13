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

/*
 * add two defines to fix error when start: spi-master -u0 -d bcm2711 base=0xfe204000,opmode=1
 *   unknown symbol: _lib_slog2f referenced from spi-bcm2711.so
 *   unknown symbol: _spi_buffer_handle referenced from spi-bcm2711.so
 */
int (*_lib_slog2f)( slog2_buffer_t buffer, uint16_t code, uint8_t severity, const char* format, ... );
slog2_buffer_t _spi_buffer_handle = 0;

static int bcm2711spi_options(bcm2711_spi_t *spi, char *optstring);
static void *bcm2711spi_init(void *hdl, char *options);
static void bcm2711spi_dinit(void *hdl);
static int bcm2711spi_drvinfo(void *hdl, spi_drvinfo_t *info);
static int bcm2711spi_devinfo(void *hdl, uint32_t device, spi_devinfo_t *info);
static int bcm2711spi_setcfg(void *hdl, uint16_t device, spi_cfg_t *cfg);
static void *bcm2711spi_xfer(void *hdl, uint32_t device, uint8_t *buf, int *len);
static int bcm2711spi_dmaxfer(void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len);

enum opt_index {BASE, CLOCK, RPANIC, RDREQ, TPANIC, TDREQ, DMA_RXBASE, DMA_TXBASE, OPERATION,END};
static char *bcm2711spi_opts[] = {
    [BASE] = "base",                /* Base address for this SPI controller */
    [CLOCK] = "clock",              /* SPI source clock frequency */
    [RPANIC] = "rpanic",            /* Read panic threshold */
    [RDREQ] = "rdreq",              /* DMA read request threshold */
    [TPANIC] = "tpanic",            /* Write panic threshold */
    [TDREQ] = "tdreq",              /* DMA write request threshold */
    [DMA_RXBASE] = "dmarxbase",     /* defines DMA RX channel base address */
    [DMA_TXBASE] = "dmatxbase",     /* defines DMA RX channel base address */
    [OPERATION] = "opmode",         /* Software operation Mode */
    [END] = NULL
};

spi_funcs_t spi_drv_entry = {
    sizeof(spi_funcs_t),
    bcm2711spi_init,    /* init() */
    bcm2711spi_dinit,   /* fini() */
    bcm2711spi_drvinfo, /* drvinfo() */
    bcm2711spi_devinfo, /* devinfo() */
    bcm2711spi_setcfg,  /* setcfg() */
    bcm2711spi_xfer,    /* xfer() */
    bcm2711spi_dmaxfer  /* dma_xfer() */
};

/*
 * The devices listed below are taken from BC2711 spec
 *
 */
static spi_devinfo_t devlist[NUM_OF_SPI_DEVS] = {
    {
        0x00,           // Device ID, for SS0
        "SPI-DEV0",     // Description
        {
          8,            // data length 8bit, CPHA = 0, CPOL = 0
          5000000	    // clock rate: 5MHz
        },
    },
    {
        0x01,           // Device ID, for SS1
        "SPI-DEV1",     // Description
        {
          8,            // data length 8bit, CPHA = 0, CPOL = 0
          5000000       // clock rate: 5MHz
        }
    }
};

static uint32_t devctrl[NUM_OF_SPI_DEVS] = {0,0};

static int bcm2711spi_dreq(bcm2711_spi_t *dev)
{
    int ret = 0;
    switch(dev->pbase)
    {
        case BCM2711_SPI0_BASE:
        {
            dev->dmarx_dreq = DREQ_SPI0RX;
            dev->dmatx_dreq = DREQ_SPI0TX;
            break;
        }

        case BCM2711_SPI4_BASE:
        {
            dev->dmarx_dreq = DREQ_SPI4RX;
            dev->dmatx_dreq = DREQ_SPI4TX;
            break;
        }

        case BCM2711_SPI5_BASE:
        {
            dev->dmarx_dreq = DREQ_SPI5RX;
            dev->dmatx_dreq = DREQ_SPI5TX;
            break;
        }

        case BCM2711_SPI6_BASE:
        {
            dev->dmarx_dreq = DREQ_SPI6RX;
            dev->dmatx_dreq = DREQ_SPI6TX;
            break;
        }

        default:
        {
            ret = -1;
            break;
        }
    }

    return ret;
}

static int bcm2711spi_options(bcm2711_spi_t *spi, char *optstring)
{
    int	opt, rc = 0;
    char *options, *freeptr, *c, *value;
    int	val;

    if (optstring == NULL)
        return 0;

    freeptr = options = strdup(optstring);
    while (options && *options != '\0') {
        c = options;
        if ((opt = getsubopt(&options, bcm2711spi_opts, &value)) == -1)
            goto error;

        switch (opt) {
            case BASE:
                spi->pbase = strtoull(value, 0, 0);
                continue;
            case CLOCK:
                spi->clock = strtoul(value, 0, 0);
                continue;
            case RPANIC:
                val = strtoul(value, 0, 0);
                if (val > 0 && val < 64) {
                    spi->rpanic_threshold = val;
                } else {
                    spi_error("spi-bcm2711: Invalid rpanic value %d", val);
                }
                continue;
            case RDREQ:
                val = strtoul(value, 0, 0);
                if (val > 0 && val < 64) {
                    spi->rdreq_threshold = val;
                } else {
                    spi_error("spi-bcm2711: Invalid rdreq value %d", val);
                }
                continue;
            case TPANIC:
                val = strtoul(value, 0, 0);
                if (val > 0 && val < 64) {
                    spi->tpanic_threshold = val;
                } else {
                    spi_error("spi-bcm2711:Invalid tpanic value %d", val);
                }
                continue;
            case TDREQ:
                val = strtoul(value, 0, 0);
                if (val > 0 && val < 64) {
                    spi->tdreq_threshold = val;
                } else {
                    spi_error("spi-bcm2711:Invalid tdreq value %d", val);
                }
                continue;
            case DMA_RXBASE:
                val = strtoul(value, NULL, 0);
                switch (val)
                {
                    case BCM2711_DMA0_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA0_BASE;
                        spi->dma_rx_irq = BCM2711_DMA0_IRQ;
                        break;
                    case BCM2711_DMA1_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA1_BASE;
                        spi->dma_rx_irq = BCM2711_DMA1_IRQ;
                        break;
                    case BCM2711_DMA2_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA2_BASE;
                        spi->dma_rx_irq = BCM2711_DMA2_IRQ;
                        break;
                    case BCM2711_DMA3_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA3_BASE;
                        spi->dma_rx_irq = BCM2711_DMA3_IRQ;
                        break;
                    case BCM2711_DMA4_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA4_BASE;
                        spi->dma_rx_irq = BCM2711_DMA4_IRQ;
                        break;
                    case BCM2711_DMA5_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA5_BASE;
                        spi->dma_rx_irq = BCM2711_DMA5_IRQ;
                        break;
                    case BCM2711_DMA6_BASE:
                        spi->dma_rx_pbase = BCM2711_DMA6_BASE;
                        spi->dma_rx_irq = BCM2711_DMA6_IRQ;
                        break;
                    default:
                        spi_error("Unknown DMA channel base address");
                        break;
                }
                continue;
            case DMA_TXBASE:
                val = strtoul(value, NULL, 0);
                switch (val)
                {
                    case BCM2711_DMA0_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA0_BASE;
                        spi->dma_tx_irq = BCM2711_DMA0_IRQ;
                        break;
                    case BCM2711_DMA1_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA1_BASE;
                        spi->dma_tx_irq = BCM2711_DMA1_IRQ;
                        break;
                    case BCM2711_DMA2_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA2_BASE;
                        spi->dma_tx_irq = BCM2711_DMA2_IRQ;
                        break;
                    case BCM2711_DMA3_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA3_BASE;
                        spi->dma_tx_irq = BCM2711_DMA3_IRQ;
                        break;
                    case BCM2711_DMA4_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA4_BASE;
                        spi->dma_tx_irq = BCM2711_DMA4_IRQ;
                        break;
                    case BCM2711_DMA5_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA5_BASE;
                        spi->dma_tx_irq = BCM2711_DMA5_IRQ;
                        break;
                    case BCM2711_DMA6_BASE:
                        spi->dma_tx_pbase = BCM2711_DMA6_BASE;
                        spi->dma_tx_irq = BCM2711_DMA6_IRQ;
                        break;
                    default:
                        spi_error("Unknown DMA channel base address");
                        break;
                }
                continue;
            case OPERATION:
                val = strtoul(value, 0, 0);
                switch((Op_Mode_e)val)
                {
                    case INTERRUPT:
                        spi->opmode = (Op_Mode_e)val;
                        break;
                    case DMA:
                        spi->opmode = (Op_Mode_e)val;
                        devlist[0].cfg.mode = (devlist[0].cfg.mode & ~SPI_MODE_CHAR_LEN_MASK) | 32;
                        devlist[1].cfg.mode = (devlist[1].cfg.mode & ~SPI_MODE_CHAR_LEN_MASK) | 32;
                        break;
                    case POLLING:
                    default:
                        spi_warn("spi-bcm2711: mode of operation is out of range, use default IRQ");
                        break;
                }
                continue;
        }
error:
        spi_error("spi-bcm2711: unknown option %s", c);
        rc = -1;
    }

    free(freeptr);

    return rc;
}

static void *bcm2711spi_init(void *hdl, char *options)
{
    bcm2711_spi_t *dev;
    uintptr_t base;
    int	i;

    dev = calloc(1, sizeof(*dev));
    if (dev == NULL)
        return NULL;

    /* Set defaults */
    dev->pbase = BCM2711_SPI0_BASE;
    dev->irq_spi = BCM2711_SPI0_IRQ;
    dev->clock = BCM2711_SPI_INPUT_CLOCK;
    dev->opmode = INTERRUPT;
    dev->dma_rx_pbase = BCM2711_DMA4_BASE;
    dev->dma_tx_pbase = BCM2711_DMA5_BASE;
    dev->dma_rx_irq = BCM2711_DMA4_IRQ;
    dev->dma_tx_irq = BCM2711_DMA5_IRQ;
    dev->dmarx_cb = NULL;
    dev->dmatx_cb = NULL;
    dev->rpanic_threshold = BCM2711_SPI_DEFAULT_RPANIC_TH;
    dev->tpanic_threshold = BCM2711_SPI_DEFAULT_TPANIC_TH;
    dev->rdreq_threshold = BCM2711_SPI_DEFAULT_RDREQ_TH;
    dev->tdreq_threshold = BCM2711_SPI_DEFAULT_TDREQ_TH;

    if (bcm2711spi_options(dev, options))
        goto fail0;

    /*
     * Map in SPI registers
     */
    if ((base = mmap_device_io(BCM2711_SPI_REGLEN, dev->pbase)) == (uintptr_t)MAP_FAILED)
    {
        spi_error("Failed to map SPI registers!");
        goto fail0;
    }

    dev->vbase = base;

    /* Reset the SPI interface
     * Clear both FIFOs
     */
    out32((base + BCM2711_SPI_CS), 0x0);
    out32((base + BCM2711_SPI_CS), in32((base + BCM2711_SPI_CS)) | ((SPI_CS_CLEAR_CLRTXFIFO | SPI_CS_CLEAR_CLRRXFIFO) << SPI_CS_CLEAR));

    /*
     * Initial device configuration with defaults
     */
    for (i = 0; i < NUM_OF_SPI_DEVS; i++)
    {
        bcm2711_cfg(dev, &devlist[i].cfg, &devctrl[i]);

        // Add Chip Select
        devctrl[i] |= i;
    }

    spi_info("SPI opmode: %d",dev->opmode);
    /*
     * Setup operation mode
     */
    switch(dev->opmode)
    {
        case INTERRUPT:
        default:
        {
            /* Attach SPI interrupt */
            if (bcm2711_attach_intr(dev))
            {
                goto fail1;
            }
            break;
        }

        case DMA:
        {
            if (bcm2711spi_dreq(dev))
            {
                goto fail1;
            }

            /* Init DMA controllers */
            if (bcm2711_init_dma(dev))
            {
                goto fail1;
            }

            /* Attach DMA interrupts */
            if (bcm2711_dma_attach_intr(dev))
            {
                goto fail1;
            }

            break;
        }
    }

    dev->spi.hdl = hdl;
    return dev;

fail1:
    munmap_device_io(dev->vbase, BCM2711_SPI_REGLEN);
fail0:
    free(dev);

    return NULL;
}

static void bcm2711spi_dinit(void *hdl)
{
    bcm2711_spi_t *dev = hdl;

    if (dev->opmode == DMA)
    {
        /*
         * Disable and unmap DMA related buffers and resources
         */
        InterruptDetach(dev->iid_dma);
        munmap_device_io(dev->dma_rx_vbase, BCM2711_DMA_REGLEN);
        munmap_device_io(dev->dma_tx_vbase, BCM2711_DMA_REGLEN);
        munmap(dev->mmapdmarx_cb, 2*sizeof(dmacb_t));
        ConnectDetach(dev->dma_coid);
        ChannelDestroy(dev->chid);
    }
    else
    {
        /*
         * unmap the register, detach the interrupt
         */
        InterruptDetach(dev->iid);
        ConnectDetach(dev->coid);
        ChannelDestroy(dev->chid);
    }

    /*
     * Disable and unmap SPI
     */
    out32((dev->vbase + BCM2711_SPI_CS), 0);
    munmap_device_io(dev->vbase, BCM2711_SPI_REGLEN);

    free(hdl);
}

static int bcm2711spi_drvinfo(void *hdl, spi_drvinfo_t *info)
{
    info->version = (SPI_VERSION_MAJOR << SPI_VERMAJOR_SHIFT) | (SPI_VERSION_MINOR << SPI_VERMINOR_SHIFT) | (SPI_REVISION << SPI_VERREV_SHIFT);
    strcpy(info->name, "BCM2711 SPI");
    info->feature = 0;
    return (EOK);
}

static int bcm2711spi_devinfo(void *hdl, uint32_t device, spi_devinfo_t *info)
{
    int	dev = device & SPI_DEV_ID_MASK;

    if (dev < NUM_OF_SPI_DEVS)
    {
        /*
        * Info of this device
        */
        memcpy(info, &devlist[dev], sizeof(*info));
    }
    else
    {
        return (EINVAL);
    }

    return (EOK);
}

static int bcm2711spi_setcfg(void *hdl, uint16_t device, spi_cfg_t *cfg)
{
    uint32_t control = 0;
    uint16_t id;

    id = device & SPI_DEV_ID_MASK;
    if (id > (NUM_OF_SPI_DEVS - 1)) {
        return (EINVAL);
    }

    memcpy(&(devlist[id].cfg), cfg, sizeof(spi_cfg_t));

    if (-1 == bcm2711_cfg(hdl, &(devlist[id].cfg), &control)) {
        return (EINVAL);
    }

    // Add Chip Select
    control |= id;

    devctrl[id] = control;

    return (EOK);
}

static void *bcm2711spi_xfer(void *hdl, uint32_t device, uint8_t *buf, int *len)
{
    bcm2711_spi_t *dev = hdl;
    uintptr_t base = dev->vbase;
    uint32_t id;
    uint16_t cdiv = 0;
    uint32_t    val;

    // Verify id
    id = device & SPI_DEV_ID_MASK;
    if (id > (NUM_OF_SPI_DEVS -1))
    {
        *len = -1;
        return buf;
    }

    // Capture full transaction length requested by client
    dev->xlen = *len;

    // Capture data size
    dev->dlen = ((devlist[id].cfg.mode & SPI_MODE_CHAR_LEN_MASK) >> 3);

    // Initialize receive and transmit counters
    dev->rlen = 0;
    dev->tlen = 0;

    // Copy client buffer address
    dev->pbuf = buf;

    // Estimate transfer time in us... The calculated dtime is only used for
    // the timeout, so it doesn't have to be that accurate. At higher clock
    // rates, a calculated dtime of 0 would mess-up the timeout calculation, so
    // round up to 1 us
    dev->dtime = (dev->xlen * 1000 * 1000) / devlist[id].cfg.clock_rate;
    if (dev->dtime == 0)
    {
        dev->dtime = 1;
    }

    /* Reset the SPI interface
     * Clear both FIFOs
     */
    out32((base + BCM2711_SPI_CS), 0x0);
    out32((base + BCM2711_SPI_CS), in32((base + BCM2711_SPI_CS)) | ((SPI_CS_CLEAR_CLRTXFIFO | SPI_CS_CLEAR_CLRRXFIFO) << SPI_CS_CLEAR));

    // Set SPI_CLK
    val = dev->clock / devlist[id].cfg.clock_rate;
    val = (val >= BCM2711_SPI_MAX_CDIV) ? 0 : val;
    cdiv = (uint16_t)(val);
    out32((base + BCM2711_SPI_CLK), (uint32_t)cdiv);

    /*
     * Start exchange. This will immediately trigger a DONE (TX
     * empty) interrupt, upon which we will fill the TX FIFO with the
     * first TX bytes. Pre-filling the TX FIFO here to avoid the
     * interrupt doesn't work:-(
     */
    out32((base + BCM2711_SPI_CS), ((SPI_CS_TA_ACTIVE << SPI_CS_TA) | (SPI_CS_INTD_INT << SPI_CS_INTD) | (SPI_CS_INTR_INT << SPI_CS_INTR) | devctrl[id]));

    /*
     * Wait for exchange to finish
     */
    if (bcm2711spi_wait(dev, dev->xlen))
    {
        spi_error("spi-bcm2711: XFER Timeout!!!");
        dev->rlen = -1;
    }

    // Disable interrupt and set TA to 0
    out32((base + BCM2711_SPI_CS), (in32(base + BCM2711_SPI_CS) & ~(SPI_CS_TA_ACTIVE << SPI_CS_TA) & ~(SPI_CS_INTD_INT << SPI_CS_INTD) & ~(SPI_CS_INTR_INT << SPI_CS_INTR)));

    *len = dev->rlen;

    return buf;
}

static int bcm2711spi_dmaxfer(void *hdl, uint32_t device, spi_dma_paddr_t *paddr, int len)
{
    bcm2711_spi_t *dev = hdl;
    uintptr_t   base = dev->vbase;
    uint32_t    id, val;
    uint32_t    spiCtrl = 0;
    uint16_t    cdiv = 0;
    int         retrycnt = 1000000;

    // Verify id
    id = device & SPI_DEV_ID_MASK;
    if (id > (NUM_OF_SPI_DEVS -1))
    {
        return -1;
    }

    // Capture full transaction length requested by client
    dev->xlen = len;

    // Capture data size
    dev->dlen = ((devlist[id].cfg.mode & SPI_MODE_CHAR_LEN_MASK) >> 3);

    // DMA only support 32bit transfer
    if ( (dev->dlen != 4) || ((dev->xlen % dev->dlen) != 0) ) {
        spi_error("%s: Error check aligtment dlen=%d, xlen=%d",__func__, dev->dlen, dev->xlen);
        return -1;
    }

    // Estimate transfer time in us... The calculated dtime is only used for
    // the timeout, so it doesn't have to be that accurate. At higher clock
    // rates, a calculated dtime of 0 would mess-up the timeout calculation, so
    // round up to 1 us
    dev->dtime = (dev->xlen * 1000 * 1000) / devlist[id].cfg.clock_rate;
    if (dev->dtime == 0)
    {
        dev->dtime = 1;
    }

    /* Reset the SPI interface
     * Clear both FIFOs
     */
    out32((base + BCM2711_SPI_CS), 0x0);
    out32((base + BCM2711_SPI_CS), in32((base + BCM2711_SPI_CS)) | ((SPI_CS_CLEAR_CLRTXFIFO | SPI_CS_CLEAR_CLRRXFIFO) << SPI_CS_CLEAR));

    // Set SPI_CLK
    val = dev->clock / devlist[id].cfg.clock_rate;
    val = (val >= BCM2711_SPI_MAX_CDIV) ? 0 : val;
    cdiv = (uint16_t)(val);
    out32((base + BCM2711_SPI_CLK), (uint32_t)cdiv);

    // Set SPI DLEN
    out32((base + BCM2711_SPI_DLEN), dev->xlen);

    // Set SPI DC
    val = (dev->rpanic_threshold << SPI_DC_RPANIC) | (dev->rdreq_threshold << SPI_DC_RDREQ) |
          (dev->tpanic_threshold << SPI_DC_TPANIC) | (dev->tdreq_threshold << SPI_DC_TDREQ);
    out32(base + BCM2711_SPI_DC, val);
    spi_debug("rpanic=%d, tpanic=%d, rdreq=%d, tdreq=%d", dev->rpanic_threshold,
                                                         dev->tpanic_threshold,
                                                         dev->rdreq_threshold,
                                                         dev->tdreq_threshold);

    // Setup dma channels
    bcm2711_setup_dma(dev, id, paddr, spiCtrl, dev->xlen);

    // Start transaction by activating both DMA channels
    bcm2711_start_dma(dev);

    // Set SPI full control word
    spiCtrl = devctrl[id];

    // Enable DMA transaction on SPI
    spiCtrl |= (SPI_CS_DMAEN_DMAEN << SPI_CS_DMAEN) | (SPI_CS_TA_ACTIVE << SPI_CS_TA) | (SPI_CS_ADCS_AUTO << SPI_CS_ADCS);
    out32((base + BCM2711_SPI_CS),spiCtrl);

    /*
     * Wait for transaction to finish
     */
    if (bcm2711spi_wait(dev, dev->xlen))
    {
        spi_error("spi-bcm2711: DMAXFER Timeout!!!");
        dev->xlen = -1;
    }
    else
    {
        /* Waiting until Rx FIFO is empty even we have received the DMA event */
        while ((in32(base + BCM2711_SPI_CS) & (1 << SPI_CS_RXD)) && retrycnt--);
        if (retrycnt < 0) {
            spi_error("%s: Timeout waiting for Rx FIFO empty!", __func__);
        }

    }

    out32((base + BCM2711_SPI_CS), (in32(base + BCM2711_SPI_CS) & ~(SPI_CS_TA_ACTIVE << SPI_CS_TA)));
    return dev->xlen;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/bcm2711spi.c $ $Rev: 928235 $")
#endif
