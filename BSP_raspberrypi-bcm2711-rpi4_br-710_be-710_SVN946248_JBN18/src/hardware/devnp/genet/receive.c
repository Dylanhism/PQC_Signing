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

#include "genet.h"
#include    "bpfilter.h"
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

void genet_add_pkt_to_rx_desc(Genet *genet, struct mbuf *m, Desc *const rxdesc)
{
    off64_t phys;

    rxdesc->mb = m;
    phys = pool_phys(m->m_data, m->m_ext.ext_page);

    CACHE_INVAL(&genet->cachectl, m->m_data, phys, m->m_ext.ext_size);

    // Now assign the newly allocated buffer
    out32(rxdesc->desc + GENET_DMA_DESC_ADDR_LSB, phys);
    out32(rxdesc->desc + GENET_DMA_DESC_ADDR_MSB, phys >> 32);
}

static unsigned rx_producer_index(Genet *genet, struct ifnet *ifp) {
    uint32_t r_pidx = genet_reg_read(genet, GENET_RDMA_PRODUCER_INDEX);
    unsigned discards = r_pidx >> 16; // upper 16-bits are running discard count

    if (genet->drops != discards) {
        ifp->if_iqdrops += discards - genet->drops;
        if (discards >= 0xf000) {
            discards = 0;

            // discards saturate at 0xffff; clear upper 16-bits
            genet_reg_write(genet, GENET_RDMA_PRODUCER_INDEX, discards);
        }
        genet->drops = discards;
    }

    return (uint16_t) r_pidx;
}

int rx_process_interrupt(void *arg, struct nw_work_thread *wtp)
{
    Genet           *genet = arg;
    struct ifnet    *ifp = &genet->sc_ec.ec_if;
    uint32_t        dma_status;
    uint16_t        r_cidx;

    r_cidx = genet_reg_read(genet, GENET_RDMA_CONSUMER_INDEX);
    while (r_cidx != rx_producer_index(genet, ifp)) {
        Desc *const rxdesc = &genet->rx_d[r_cidx % GENET_RING_SIZE_MAX];
        struct mbuf *m, *m_toStack;
        uint32_t desc_cntrl;
        uint8_t *dptr;
        off64_t phys;
        unsigned len;

        desc_cntrl = in32(rxdesc->desc + GENET_DMA_DESC_CNTRL);

        if ((desc_cntrl & (GENET_DMA_FIRST_PKT | GENET_DMA_LAST_PKT))
                 != (GENET_DMA_FIRST_PKT | GENET_DMA_LAST_PKT)) {
            GENET_ERROR("Skipping fragmented packet %x", desc_cntrl);
            ifp->if_ierrors++;
            goto next;
        }

        dma_status = desc_cntrl & 0xFFFF;
        if (dma_status & (GENET_DMA_RX_CRC_ERR    | GENET_DMA_RX_LENGTH_ERR
                         | GENET_DMA_RX_FRAME_ERR | GENET_DMA_RX_OVERRUN_ERR
                         | GENET_DMA_RX_RXERR)) {
            GENET_ERROR("RX packet status %x", dma_status);
            ifp->if_ierrors++;
            goto next;
        }

        len = desc_cntrl >> GENET_DMA_BUFLEN_SHIFT;

        m_toStack = rxdesc->mb;
        m_toStack->m_pkthdr.len = m_toStack->m_len  = len;
        m_toStack->m_pkthdr.rcvif = ifp; // ip_input() needs this

        phys = pool_phys(m_toStack->m_data, m_toStack->m_ext.ext_page);
        CACHE_INVAL(&genet->cachectl, m_toStack->m_data, phys, len);

        genet->stats.octets_rxed_ok += len;
        genet->stats.rxed_ok++;
        ifp->if_ipackets++;

        dptr = mtod (m_toStack, uint8_t *);
        if (dptr[0] & 1) {
            if (IS_BROADCAST (dptr))
                genet->stats.rxed_broadcast++;
            else
                genet->stats.rxed_multicast++;
        }

#if NBPFILTER > 0
        /* Pass this up to any BPF listeners. */
        if (ifp->if_bpf) {
            bpf_mtap(ifp->if_bpf, m_toStack);
        }
#endif

        /* Pass this up to the io-pkt stack. */
        ifp->if_input(ifp, m_toStack);

        /* Get the new buf to replace the current descriptor */
        m = m_getcl_wtp(M_DONTWAIT, MT_DATA, M_PKTHDR, wtp);

        if (!m) {
            GENET_ERROR("RX mbuf allocation failed");
            genet->stats.rx_failed_allocs++;
            ifp->if_ierrors++;
            break;
        }

        genet_add_pkt_to_rx_desc(genet, m, rxdesc);

next: // done the packet processing and update the hadware
        //Increment RDMA consumer index
        genet_reg_write(genet, GENET_RDMA_CONSUMER_INDEX, ++r_cidx); // r_cidx updated only by rx-interrupt
    }

    return 1;
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
