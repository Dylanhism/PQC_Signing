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


// convert physical addresses to legacy physical for dma
static uint32_t dma_legacy(uintptr_t phys)
{
    if ((phys & 0xfc000000) == 0xfc000000)
        return phys & ~(0x8 << 28); // legacy peripheral
    return phys | (0xc << 28); // legacy sdram
}

int bcm2711_start_dma(bcm2711_spi_t *dev)
{
    // Reset DMA channels
    out32((dev->dma_tx_vbase + BCM2711_DMA_CS), in32(dev->dma_tx_vbase + BCM2711_DMA_CS) | (DMA_CS_RESET | DMA_CS_INT | DMA_CS_END));
    out32((dev->dma_rx_vbase + BCM2711_DMA_CS), in32(dev->dma_rx_vbase + BCM2711_DMA_CS) | (DMA_CS_RESET | DMA_CS_INT | DMA_CS_END));

    usleep(100000);

    // Set CBs addresses to their corresponding DMA channel
    out32((dev->dma_tx_vbase + BCM2711_DMA_CBADRS), dma_legacy((uintptr_t)(dev->pdmatx_cb)));
    out32((dev->dma_rx_vbase + BCM2711_DMA_CBADRS), dma_legacy((uintptr_t)(dev->pdmarx_cb)));

    // Activate DMA channels
    out32((dev->dma_tx_vbase + BCM2711_DMA_CS), DMA_CS_ACTIVE);
    out32((dev->dma_rx_vbase + BCM2711_DMA_CS), DMA_CS_ACTIVE);

    return 0;
}

int bcm2711_setup_dma(bcm2711_spi_t *dev, int device, spi_dma_paddr_t *paddr, uint32_t spi_init_ctrl, int len)
{
    spi_dma_paddr_t *bufaddr = paddr;

    dev->dmarx_cb = (dmacb_t *)dev->mmapdmarx_cb;
    dev->dmatx_cb = (dmacb_t *)dev->mmapdmatx_cb;

    /******************************
     * Setup Tx DMA Control Block *
     ******************************/
    dev->dmatx_cb->src       = dma_legacy((uintptr_t)bufaddr->wpaddr);
    dev->dmatx_cb->dst       = dma_legacy(dev->pbase + BCM2711_SPI_FIFO);
    dev->dmatx_cb->len       = (uint32_t)len;
    dev->dmatx_cb->ti        = (uint32_t)( DMA_TI_WAIT_RESP | DMA_TI_SRC_INC | DMA_TI_DST_DREQ | (dev->dmatx_dreq << 16) );
    dev->dmatx_cb->nextcb    = 0;

    /******************************
     * Setup Rx DMA Control Block *
     ******************************/
    dev->dmarx_cb->src       = dma_legacy(dev->pbase + BCM2711_SPI_FIFO);
    dev->dmarx_cb->dst       = dma_legacy((uintptr_t)bufaddr->rpaddr);
    dev->dmarx_cb->len       = (uint32_t)len;
    dev->dmarx_cb->ti        = (uint32_t)( DMA_TI_WAIT_RESP | DMA_TI_DST_INC | DMA_TI_SRC_DREQ | DMA_TI_INTEN | (dev->dmarx_dreq << 16) );
    dev->dmarx_cb->nextcb    = 0;

    spi_debug("tx_cb: src %#8X ,dst %#8X ,len %d ,ti %#8X ,nextcb %#8X",
                dev->dmatx_cb->src,dev->dmatx_cb->dst,
                dev->dmatx_cb->len,dev->dmatx_cb->ti,
                dev->dmatx_cb->nextcb);


    spi_debug("rx_cb: src %#8X ,dst %#8X ,len %d ,ti %#8X ,nextcb %#8X",
                dev->dmarx_cb->src,dev->dmarx_cb->dst,
                dev->dmarx_cb->len,dev->dmarx_cb->ti,
                dev->dmarx_cb->nextcb);
    return 0;

}

static int32_t bcm2711_dma_attach(bcm2711_spi_t *dev)
{
    /* Map in RX DMA registers */
    dev->dma_rx_vbase = mmap_device_io(BCM2711_DMA_REGLEN, dev->dma_rx_pbase);
    if (dev->dma_rx_vbase == (uintptr_t) MAP_FAILED)
    {
        spi_error("Unable to mmap DMA Rx registers(%s)", strerror (errno));
        return (-1);
    }

    /* Map in TX DMA registers */
    dev->dma_tx_vbase = mmap_device_io(BCM2711_DMA_REGLEN, dev->dma_tx_pbase);
    if (dev->dma_tx_vbase == (uintptr_t) MAP_FAILED)
    {
        munmap_device_io(dev->dma_rx_vbase, BCM2711_DMA_REGLEN);
        spi_error("Unable to mmap DMA Tx registers(%s)", strerror (errno));
        return (-1);
    }

    return 0;
}

int bcm2711_init_dma(bcm2711_spi_t *dev)
{
    off_t offset2,offset3;

    /*
     * Open a typed memory object within the below1G ram region
     */
    dev->dma_fd = dev->dma_fd ?: posix_typed_mem_open("sysram&below1G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
    if (dev->dma_fd < 0)
    {
        spi_error("Faile to open typed memory from below1G");
        return (-1);
    }

    /*
     * Create TX and RX DMA control block within the below1G ram region
     * and get its physical address for Tx/Rx DMA engine to use
     */
    dev->mmapdmarx_cb = mmap(0, 2*sizeof(dmacb_t), PROT_NOCACHE|PROT_READ|PROT_WRITE, MAP_SHARED, dev->dma_fd, 0);
    dev->mmapdmatx_cb = dev->mmapdmarx_cb + sizeof(dmacb_t);

    if (mem_offset(dev->mmapdmarx_cb, NOFD, 1, &offset2, 0) != EOK)
    {
        munmap(dev->mmapdmarx_cb, 2*sizeof(dmacb_t));
        close(dev->dma_fd);
        spi_error("Error to get mem_offset for Rx descriptor");
        return (-1);
    }

    dev->pdmarx_cb = (uint32_t)offset2;

    if (mem_offset(dev->mmapdmatx_cb, NOFD, 1, &offset3, 0) != EOK)
    {
        munmap(dev->mmapdmarx_cb, 2*sizeof(dmacb_t));
        close(dev->dma_fd);
        spi_error("Error to get mem_offset for Tx descriptor");
        return (-1);
    }

    dev->pdmatx_cb = (uint32_t)offset3;

    /*
     * Map in DMA registers and resources
     */
    if(bcm2711_dma_attach(dev))
    {
        munmap(dev->mmapdmarx_cb, sizeof(dmacb_t));
        munmap(dev->mmapdmatx_cb, sizeof(dmacb_t));
        close(dev->dma_fd);
        spi_error("Could not attach DMA");
        return (-1);
    }

    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/dma.c $ $Rev: 928235 $")
#endif
