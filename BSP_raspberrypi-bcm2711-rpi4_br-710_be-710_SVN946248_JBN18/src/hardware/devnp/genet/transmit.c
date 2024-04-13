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
#include "bpfilter.h"
#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

int tx_process_interrupt(void *arg, struct nw_work_thread *wtp)
{
    Genet *genet = arg;
    uint16_t f_idx = genet->free_idx;

    // ignore upper 16-bits of c_idx
    while (f_idx != (uint16_t) genet_reg_read(genet, GENET_TDMA_CONSUMER_INDEX)) {
        Desc *const txdesc = &genet->tx_d[f_idx++ % GENET_RING_SIZE_MAX];
        m_free(txdesc->mb);
        genet->free_idx = f_idx; // free_idx updated only by tx-interrupt
    }

    return 1;
}

static struct mbuf *genet_defrag (struct mbuf *m)
{
    struct mbuf *m2;

    MGET(m2, M_DONTWAIT, MT_DATA);
    if (m2 == NULL)
        return NULL;

    M_COPY_PKTHDR(m2, m);

    MCLGET(m2, M_DONTWAIT);
    if ((m2->m_flags & M_EXT) == 0) {
        m_freem(m2);
        return NULL;
    }

    /* jumbo packets larger than cluster? */
    if ((size_t) m->m_pkthdr.len > m2->m_ext.ext_size) {
        m_freem(m2);
        return NULL;
    }

    m_copydata(m, 0, m->m_pkthdr.len, mtod(m2, caddr_t));
    m2->m_pkthdr.len = m2->m_len = m->m_pkthdr.len;

    m_freem(m);

    return m2;
}

static void genet_tx(Genet *genet, struct mbuf *m, unsigned parts)
{
    uint32_t      flags = GENET_DMA_QTAG | GENET_DMA_DO_CRC | GENET_DMA_FIRST_PKT;
    uint16_t     r_pidx = genet_reg_read(genet, GENET_TDMA_PRODUCER_INDEX);
    struct ifnet   *ifp = &genet->sc_ec.ec_if;
    uint8_t       *dptr = mtod(m, uint8_t *);

    if (dptr[0] & 1) {
        if (IS_BROADCAST(dptr))
            genet->stats.txed_broadcast++;
        else
            genet->stats.txed_multicast++;
    }

#if NBPFILTER > 0
    // Pass this up to any BPF listeners
    // Call bpf_mtap() beore we start freeing mbufs below
    if (ifp->if_bpf)
        bpf_mtap(ifp->if_bpf, m);
#endif

    // already checked that r_pidx + parts won't cross free_idx
    unsigned len = 0;
    for (struct mbuf *m2 = m; m2;) {
        len += m2->m_len;
        if (m2->m_len == 0) {
            m2 = m_free(m2);
            continue;
        }

        Desc *const txdesc = &genet->tx_d[r_pidx++ % GENET_RING_SIZE_MAX];

        txdesc->mb = m2;
        const off64_t phys = mbuf_phys(m2);

        CACHE_FLUSH(&genet->cachectl, m2->m_data, phys, m2->m_len);

        out32(txdesc->desc + GENET_DMA_DESC_ADDR_LSB, phys);
        out32(txdesc->desc + GENET_DMA_DESC_ADDR_MSB, phys >> 32);

        if (--parts != 0) {
            out32(txdesc->desc + GENET_DMA_DESC_CNTRL, m2->m_len << 16 | flags);
            flags &= ~GENET_DMA_FIRST_PKT; // SOP only on first segment
        } else {
            genet->stats.octets_txed_ok += len;
            genet->stats.txed_ok++;
            // adjust last length for minimum 60-byte frame
            // also adjust last length for appending crc
            len = len < 60 ? 60 - (len - (unsigned) m2->m_len) + 4 : (unsigned) m2->m_len + 4;
            flags |= GENET_DMA_LAST_PKT | GENET_DMA_APPEND_CRC;
            out32(txdesc->desc + GENET_DMA_DESC_CNTRL, len << 16 | flags);
        }
        m2 = m2->m_next;
    }

    genet_reg_write(genet, GENET_TDMA_PRODUCER_INDEX, r_pidx); // p_idx updated only by genet_tx
}

void genet_start(struct ifnet *ifp)
{
    struct nw_work_thread *wtp = WTP;
    Genet *genet = ifp->if_softc;
    struct mbuf *m, *m2;

    /* Transmit only if the link is up */
    if (!(ifp->if_flags_tx & IFF_RUNNING) || (genet->cfg.flags & NIC_FLAG_LINK_DOWN)) {
        NW_SIGUNLOCK_P(&ifp->if_snd_ex, genet->iopkt, wtp);
        return;
    }

    ifp->if_flags_tx |= IFF_OACTIVE;

    for (;;) {
        IFQ_POLL(&ifp->if_snd, m);
        if (m == NULL)
            break;

        // Count the non-zero-length mbufs
        //why are zero-length mbufs present?
        unsigned parts = 0;
        for (m2 = m; m2; m2 = m2->m_next) {
            if (m2->m_len)
                parts++;
        }

        // try brief delay before allocating new buffer and copying old buffers
        const uint16_t p_idx = genet_reg_read(genet, GENET_TDMA_PRODUCER_INDEX);
        unsigned tries = 10;

        for (;;) {
            uint16_t in_use = p_idx - genet->free_idx;
            uint16_t free = GENET_RING_SIZE_MAX - in_use;

            if (parts > free && tries--) {
                usleep(14); // assume 60k+ packets/second?
            } else {
                break;
            }
        }

        // commit to transmitting or dropping this frame
        IFQ_DEQUEUE(&ifp->if_snd, m);

        if (!tries) {
            // defrag if after retries there is a slot free
            uint16_t in_use = p_idx - genet->free_idx;
            uint16_t free = GENET_RING_SIZE_MAX - in_use;

            if (free && (m2 = genet_defrag(m))) {
                m_freem(m);
                m = m2;
                parts = 1;
            } else {
                genet->stats.tx_failed_allocs++;
                ifp->if_oerrors++; // dropped frame
                m_freem(m);
                continue;
           }
        }

        genet_tx(genet, m, parts);
        ifp->if_opackets++;  // for ifconfig -v
    }

    ifp->if_flags_tx &= ~IFF_OACTIVE;
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, genet->iopkt, wtp);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
