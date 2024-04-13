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

#include <aarch64/bcm2711.h>
#include <sys/slogcodes.h>
#include <sys/slog.h>
#include <sys/rsrcdbmgr.h>
#include <errno.h>
#include "dma.h"

#include <audio_driver.h>
#define DBG_INFO(...)   ado_debug(DB_LVL_DRIVER, __VA_ARGS__)
#define DBG_ERROR(...)  ado_error_fmt(__VA_ARGS__)

#define DMA_ABORT_TIMEOUT 50

// convert physical addresses to legacy physical for dma
static uint32_t dma1_legacy(uintptr_t phys) {
    if ((phys & 0xfc000000) == 0xfc000000) {
        return phys & ~(0x8 << 28); // legacy peripheral
    }
    return phys | (0xc << 28); // legacy sdram
}

static void dma1_abort(void *hw) {
    volatile dma1_reg_t *dma = hw;
    uint32_t dma_cs = dma->cs;
    int i = 0;

    /* pause DMA in order to be able to safely set nextconbk to 0 */
    dma->cs &= ~DMA1_CS_ACTIVE;
    /* CS_ABORT aborts the current control block and loads the next control block;
     * set nextconbk to 0, to cause the entire DMA transfer to be aborted
     */
    dma->nextconbk = 0;
    /* abort */
    dma->cs |= DMA1_CS_ABORT;
    /* unpause the DMA transfer, so that we can abort */
    dma->cs |= DMA1_CS_ACTIVE;

    /* wait for the CS_ACTIVE bit to clear automatically as result of the abort */
    while ((dma->cs & DMA1_CS_ACTIVE) && (i < DMA_ABORT_TIMEOUT)) {
        nanospin_ns(1000);   /* 1us */
        i++;
    }
    if (i >= DMA_ABORT_TIMEOUT) {
        DBG_ERROR("Abort Timed out waiting for CS_ACTIVE to clear, DMA CS 0x%x -> 0x%x", dma_cs, dma->cs);
    } else {
        DBG_INFO("CS_ACTIVE cleared in %dus, DMA cs 0x%x -> 0x%x", i, dma_cs, dma->cs);
    }

    /* Force a channel reset to ensure DMA stops even if there is a problem with the abort */
    dma->cs |= DMA1_CS_RESET;
}

// Only three registers in each channel's register set are directly writable (CS, CB and DEBUG).
// The other registers (TI, SRC, DST, LEN, STRIDE & NEXTCB) are automatically loaded from a Control Block data structure in external memory.
static void dma1_start(void *hw, uint32_t cbphys) {
    volatile dma1_reg_t *dma = hw;
    uint32_t dma_cs = dma->cs;

    dma->cs |= DMA1_CS_RESET;
    dma->conblk_ad = dma1_legacy(cbphys);
    dma->cs |= DMA1_CS_ACTIVE;
    DBG_INFO("cs 0x%x->0x%x", dma_cs, dma->cs);
}

static int dma1_ack(void *hw) {
    volatile dma1_reg_t *dma = hw;
    uint32_t dma_cs = dma->cs;

    if (dma_cs & DMA1_CS_INT) {
        if (dma_cs & DMA1_CS_ERROR) {
            dma->cs |= DMA1_CS_INT | DMA1_CS_END | DMA1_CS_ERROR;
            return DMA_ERROR | DMA_INT;
        }
        dma->cs |= DMA1_CS_INT | DMA1_CS_END;
        return DMA_INT;
    }
    return 0;
}

static void dma1_setcb(void *hw, uint32_t ti, uint32_t src, uint32_t dst, uint32_t len, uint32_t nextcb) {
    dma1_cb_t *cb = hw;

    cb->source_ad = dma1_legacy(src);
    cb->dest_ad = dma1_legacy(dst);
    cb->txfr_len = len;
    cb->ti = ti;
    cb->nextconbk = nextcb ? dma1_legacy(nextcb) : nextcb;
}

static unsigned dma1_len(void *hw) {
    volatile dma1_reg_t *dma = hw;

    return dma->txfr_len;
}

// convert physical addresses to full 35-bit physical for dma
static uint64_t dma4_full(uintptr_t phys) {
    if ((phys & 0xfc000000) == 0xfc000000)
        return 0x400000000ll | (phys & ~(0x8 << 28));
    return phys;
}

static void dma4_abort(void *hw) {
    volatile dma4_reg_t *dma = hw;
    uint32_t dma_cs = dma->cs;
    int i = 0;

    /* pause DMA in order to be able to safely set nextconbk to 0 */
    dma->cs &= ~DMA4_CS_ACTIVE;
    /* CS_ABORT aborts the current control block and loads the next control block;
     * set nextconbk to 0, to cause the entire DMA transfer to be aborted
     */
    dma->next_cb = 0;
    /* abort */
    dma->cs |= DMA4_CS_ABORT;
    /* unpause the DMA transfer, so that we can abort */
    dma->cs |= DMA4_CS_ACTIVE;

    /* wait for the CS_ACTIVE bit to clear automatically as result of the abort */
    while ((dma->cs & DMA4_CS_ACTIVE) && (i < DMA_ABORT_TIMEOUT)) {
        nanospin_ns(1000);
        i++;
    }
    if (i >= DMA_ABORT_TIMEOUT) {
        DBG_ERROR("Abort Timed out waiting for CS_ACTIVE to clear, DMA CS 0x%x -> 0x%x", dma_cs, dma->cs);
    } else {
        DBG_INFO("CS_ACTIVE cleared in %dus, DMA cs 0x%x -> 0x%x", i, dma_cs, dma->cs);
    }

    /* Force a channel reset to ensure DMA stops even if there is a problem with the abort */
    dma->debug |= DMA4_DEBUG_RESET;
}

// Only three registers in each channel's register set are directly writable (CS, CB and DEBUG).
// The other registers (TI, SRC, DST, LEN, STRIDE & NEXTCB) are automatically loaded from a Control Block data structure in external memory.
static void dma4_start(void *hw, uint32_t cbphys) {
    volatile dma4_reg_t *dma = hw;

    dma->debug |= DMA4_DEBUG_RESET;
    dma->conblk_ad = dma4_full(cbphys) >> 5;
    dma->cs |= DMA4_CS_ACTIVE;
}

static int dma4_ack(void *hw) {
    volatile dma4_reg_t *dma = hw;
    uint32_t dma_cs = dma->cs;
    int ret = 0;

    if (dma_cs & (DMA4_CS_BUSY | DMA4_CS_INT)) {
        if (dma_cs & DMA4_CS_ERROR) {
            dma->debug |= DMA4_DEBUG_READ_CB_ERROR | DMA4_DEBUG_READ_ERROR |
                DMA4_DEBUG_WRITE_ERROR | DMA4_DEBUG_FIFO_ERROR;
            ret |= DMA_ERROR;
        }
        dma->cs |= DMA4_CS_INT | DMA4_CS_END;
        return ret | DMA_INT;
    }

    return 0;
}

static void dma4_setcb(void *hw, uint32_t ti, uint32_t src, uint32_t dst, uint32_t len, uint32_t nextcb) {
    dma4_cb_t *cb = hw;
    uint64_t addr;

    addr = dma4_full(src);
    cb->src = addr;
    cb->srci = addr >> 32;
    if (ti & DMA4_TI_D_DREQ) cb->srci |= DMA4_SRCI_INC; // inc src if dest dreq
    addr = dma4_full(dst);
    cb->dest = addr;
    cb->desti = addr >> 32;
    if (ti & DMA4_TI_S_DREQ) cb->desti |= DMA4_DESTI_INC; // inc dest if src dreq
    cb->len = len;
    cb->ti = ti;
    cb->next_cb = nextcb ? dma4_full(nextcb) >> 5: nextcb;
}

static unsigned dma4_len(void *hw) {
    volatile dma4_reg_t *dma = hw;

    return dma->len;
}

static const struct dma_funcs dma1_fn = {
    dma1_start, dma1_abort, dma1_ack, dma1_setcb, dma1_len
};

static const struct dma_funcs  dma4_fn = {
    dma4_start, dma4_abort, dma4_ack, dma4_setcb, dma4_len
};

static int rsrc_req(unsigned flags, unsigned chan) {
    static rsrc_alloc_t alloc[] = {
        { .name = "DMA", .start = 0, .end = 14, .flags = RSRCDBMGR_FLAG_NOREMOVE | RSRCDBMGR_DMA_CHANNEL },
        { .name = "PIN", .start = 0, .end = 40, .flags = RSRCDBMGR_FLAG_NOREMOVE | RSRCDBMGR_IO_PORT },
    };
    static char created;

    if (!created) {
        if (rsrcdbmgr_create(alloc, sizeof alloc / sizeof alloc[0]) == -1) {
            DBG_ERROR("rsrc 0x%x %s %d", flags, "create failure",  errno);
            return -1;
        }
        created = 1;
    }

    rsrc_request_t request[] = {
        { .length = 1, .start = chan, .end = chan, .flags = flags }
    };

    if (rsrcdbmgr_attach(request, sizeof request / sizeof request[0]) == -1) {
        DBG_ERROR("rsrc 0x%x %s %d", flags, "attach failure", errno);
        return -1;
    }

    DBG_INFO("rsrc 0x%x %s %ld", flags, "attached", request[0].start);
    return request[0].start;
}

static const struct dma_config dma_cfg[] = {
    { .base = BCM2711_DMA0_BASE,  .irq = BCM2711_DMA0_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA1_BASE,  .irq = BCM2711_DMA1_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA2_BASE,  .irq = BCM2711_DMA2_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA3_BASE,  .irq = BCM2711_DMA3_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA4_BASE,  .irq = BCM2711_DMA4_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA5_BASE,  .irq = BCM2711_DMA5_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA6_BASE,  .irq = BCM2711_DMA6_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA7_BASE,  .irq = BCM2711_DMA7_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA8_BASE,  .irq = BCM2711_DMA8_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA9_BASE,  .irq = BCM2711_DMA9_IRQ,  .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA10_BASE, .irq = BCM2711_DMA10_IRQ, .fn = &dma1_fn, .bits = 28 },
    { .base = BCM2711_DMA11_BASE, .irq = BCM2711_DMA11_IRQ, .fn = &dma4_fn, .bits = 40 },
    { .base = BCM2711_DMA12_BASE, .irq = BCM2711_DMA12_IRQ, .fn = &dma4_fn, .bits = 40 },
    { .base = BCM2711_DMA13_BASE, .irq = BCM2711_DMA13_IRQ, .fn = &dma4_fn, .bits = 40 },
    { .base = BCM2711_DMA14_BASE, .irq = BCM2711_DMA14_IRQ, .fn = &dma4_fn, .bits = 40 },
};

int dma_getcfg(unsigned chan, const struct dma_config **cfgp) {
    if (chan < sizeof dma_cfg / sizeof dma_cfg[0]) {
        if (rsrc_req(RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE, chan) < 0) {
            *cfgp = 0;
            return -1;
        }
        *cfgp = &dma_cfg[chan];
        return 0;
    }

    for (unsigned n = 0; n < sizeof dma_cfg / sizeof dma_cfg[0]; n++) {
        if (chan == dma_cfg[n].base) {
            if (rsrc_req(RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE, n) < 0) {
                *cfgp = 0;
                return -1;
            }
            *cfgp = &dma_cfg[n];
            return 0;
        }
    }

    return -1;
}

void dma_dropcfg(const struct dma_config *cfg) {
    unsigned chan = cfg - dma_cfg;
    rsrc_request_t request[] = {
        { .length = 1, .start = chan, .end = chan, .flags = RSRCDBMGR_DMA_CHANNEL | RSRCDBMGR_FLAG_RANGE }
    };

    if (rsrcdbmgr_detach(request, sizeof request / sizeof request[0]) == -1) {
        DBG_ERROR("rsrc detach failure for DMA chan %d - %d", chan, errno);
    }
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/dma.c $ $Rev: 934572 $")
#endif
