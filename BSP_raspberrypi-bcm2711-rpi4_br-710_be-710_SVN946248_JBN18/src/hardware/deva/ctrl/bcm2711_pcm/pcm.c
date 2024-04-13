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

/*
 * bcm2711 sound using PCM module
 *
 * PDM Input Mode and Gray Code Input Mode currently not supported
 */

#include "pcm.h"
#include "dma.h"
#include "cm.h"
#include "gpio.h"
#include "mixer.h"

#include <aarch64/bcm2711.h>
#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>

#define OSC_CLK_FREQ        54*1000*1000l
#define OSC_CLK_SRC         1
#define PLLD_CLK_FREQ       750*1000*1000l
#define PLLD_CLK_SRC        6

/**
 * Helper function used to determine the system clock and sample precision based
 * on configured sample rate and source clock frequency and then configure the
 * clock manager accordingly
 * @param  hwc  hardware context structure.
 * @return None
 */
static void bcm2711pcm_clock_config(HW_CONTEXT_T *hwc) {
    uint32_t sysclk;
    if (!hwc->sample_rate) {
        return;
    }

    sysclk = hwc->sample_rate * PCM_CHANNELS * ado_pcm_format_bit_width(hwc->sample_format);

    ado_debug(DB_LVL_DRIVER, "rate %u, format %u, sysclk %lu",
        hwc->sample_rate, hwc->sample_format, sysclk);

    cm_config(BCM2711_CM_PCM, sysclk, hwc->clk_freq, hwc->clk_src, hwc->clk_mash);
}

/**
 * Helper function used to configure the PCM module of the BCM2711
 * @param  hwc  hardware context structure.
 * @return None
 */
static void bcm2711pcm_config(HW_CONTEXT_T *hwc) {
    const unsigned sample_bit_width = ado_pcm_format_bit_width(hwc->sample_format);
    /* the channel width in bits is coded as two different bit fields:
     * the legacy channel width (chwid) bit field captures the entire channel width
     * minus 8, if the channel width is smaller than 24 bits (for compatibility with
     * older chip versions), and the extended channel width bit (chwex) is not set;
     * if the channel width is greater equal 24, the extended channel width bit
     * must be set, and the legacy channel width bit is to be set to channel width minus 24
     */
    const unsigned chwex = sample_bit_width >= 24 ? 1 : 0;
    const unsigned chwid = sample_bit_width - (16 * chwex) - 8;

    hwc->pcm_reg->cs_a = 0; /* Disable eveything */

    hwc->pcm_reg->mode_a =
        PCM_MODE_FLEN(sample_bit_width*PCM_CHANNELS - 1) | PCM_MODE_FSLEN(sample_bit_width);
    if (!hwc->clk_pol) {
        hwc->pcm_reg->mode_a |= PCM_MODE_CLKI;
    }
    if (!hwc->fs_pol) {
        hwc->pcm_reg->mode_a |= PCM_MODE_FSI;
    }
    if (hwc->clk_mode == CLK_MODE_SLAVE) {
        /* PCM CLK and FS are both inputs */
        hwc->pcm_reg->mode_a |= PCM_MODE_CLKM | PCM_MODE_FSM;
    }

    /* DREQ and PANIC thresholds */
    hwc->pcm_reg->dreq_a =
        PCM_DREQ_TX_PANIC(hwc->playback.dma.thresh_panic) |
        PCM_DREQ_TX_REQ(hwc->playback.dma.thresh_dreq) |
        PCM_DREQ_RX_PANIC(hwc->capture.dma.thresh_panic) |
        PCM_DREQ_RX_REQ(hwc->capture.dma.thresh_dreq);

    if (hwc->playback.configured) {
        /* if channel bit width is 16 bits, pack two 16-bit samples into
         * one 32 bit FIFO entry; otherwise use one FIFO entry per sample
         */
        if (sample_bit_width == 16) {
            hwc->pcm_reg->mode_a |= PCM_MODE_FTXP;
        }
        hwc->pcm_reg->txc_a =
            PCM_TXC_CH1EN | PCM_TXC_CH1WEX(chwex) | PCM_TXC_CH1WID(chwid) |
            PCM_TXC_CH2EN | PCM_TXC_CH2WEX(chwex) | PCM_TXC_CH2WID(chwid) |
            PCM_TXC_CH1POS(hwc->bit_delay) | PCM_TXC_CH2POS(hwc->bit_delay + sample_bit_width);

        /* Assert TXW bit while TX FIFO is less then 3/4 full (used for priming FIFO on enable) */
        hwc->pcm_reg->cs_a |= PCM_CS_TXTHR(2);
    }

    if (hwc->capture.configured) {
        /* if channel bit width is 16 bits, pack two 16-bit samples into
         * one 32 bit FIFO entry; otherwise use one FIFO entry per sample;
         * if channel bit width is 24 bits, extend the sign bit over the MSB
         */
        if (sample_bit_width == 16) {
            hwc->pcm_reg->mode_a |= PCM_MODE_FRXP;
        } else if (sample_bit_width == 24) {
            hwc->pcm_reg->cs_a |= PCM_CS_RXSEX;
        }
        hwc->pcm_reg->rxc_a =
            PCM_RXC_CH1EN | PCM_RXC_CH1WEX(chwex) | PCM_RXC_CH1WID(chwid) |
            PCM_RXC_CH2EN | PCM_RXC_CH2WEX(chwex) | PCM_RXC_CH2WID(chwid) |
            PCM_RXC_CH1POS(hwc->bit_delay) | PCM_RXC_CH2POS(hwc->bit_delay + sample_bit_width);

        /* Assert RXR bit while RX FIFO has any data in it (used to detect FIFO empty on disable) */
        hwc->pcm_reg->cs_a |= PCM_CS_RXTHR(0);
    }

    /* Enable DMA requestes
     * Note: DMAEN is common to both RX and TX so we assert
     *       it once during init.
     */
    hwc->pcm_reg->cs_a |= PCM_CS_DMAEN;
}

/**
 * Helper function used to disable and unconfigure the PCM module
 * @param  hwc  hardware context structure.
 * @param  pc     subchannel context pointer
 * @return None
 */
static inline void bcm2711pcm_unconfig(HW_CONTEXT_T *hwc) {
    ado_debug(DB_LVL_DRIVER, "");
    hwc->pcm_reg->cs_a &= ~(PCM_CS_TXON | PCM_CS_RXON | PCM_CS_EN | PCM_CS_DMAEN);
    hwc->pcm_reg->mode_a |= PCM_MODE_CLK_DIS;
}

/**
 * Helper function used to stop the TX or RX operation of the PCM module
 * and disable the PCM module if both directions are stopped
 * @param  hwc  hardware context structure.
 * @param  pc     subchannel context pointer
 * @return None
 */
static inline void bcm2711pcm_disable(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc) {

    ado_debug(DB_LVL_DRIVER, "%s subchn = 0x%p", pc == &hwc->playback ? "Playback" : "Capture", pc);

    if (pc == &hwc->playback) {
        int timeout = 200; /* 200 * 5us = 1ms */

        /* Stop DMA transfer */
        pc->dma.cfg->fn->dma_abort(pc->dma.hw);

        /* Wait for the transmitter to empty the FIFO before we
         * disable. This ensures we do  not lose any samples and
         * that the FIFO is clean for the next playback session
         */
        while ( !(hwc->pcm_reg->cs_a & PCM_CS_TXE) && timeout-- > 0)
            nanospin_ns(5000);  /* 5us */
        if (timeout <= 0) {
            ado_error_fmt("Failed to drain TX FIFO");
        } else {
            ado_debug(DB_LVL_DRIVER, "TX Drain completed in %dus", (200 - timeout) * 5);
        }

        /* Disable transmitter */
        hwc->pcm_reg->cs_a &= ~PCM_CS_TXON;
    } else {
        /* Stop DMA transfer */
        pc->dma.cfg->fn->dma_abort(pc->dma.hw);

        /* Disable Receiver
         * Note: DMA cannot successfully abort if RXON is cleared
         *       so we abort first
         */
        hwc->pcm_reg->cs_a &= ~PCM_CS_RXON;
    }

    if ((hwc->pcm_reg->cs_a & (PCM_CS_TXON | PCM_CS_RXON)) == 0) {
        /* If both transmitter and receiver are disabled then
         * disable the PCM Audio interface to conserve power
         */
        hwc->pcm_reg->cs_a &= ~(PCM_CS_EN);
    }
}

/**
 * Helper function used to enable the PCM module and start its TX or RX operation,
 * @param  hwc  hardware context structure.
 * @param  pc     subchannel context pointer
 * @return None
 */
static void bcm2711pcm_enable(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc)
{
    ado_debug(DB_LVL_DRIVER, "%s subchn = 0x%p", pc == &hwc->playback ? "Playback" : "Capture", pc);

    /* Ensure PCM Audio interface is enabled */
    hwc->pcm_reg->cs_a |= PCM_CS_EN;

    if (pc == &hwc->playback) {
        int timeout = 200; /* 200 * 5us = 1ms */

        /* Ensure the TXERR bit is cleared and reset/clear the TX FIFO */
        hwc->pcm_reg->cs_a |= PCM_CS_TXERR | PCM_CS_TXCLR;
        nanospin_ns(4000); /* Wait 2 PCM clocks for FIFOs to clear */

        /* Start DMA and wait for the TX FIFO to reach the TXTH level */
        pc->dma.cfg->fn->dma_start(pc->dma.hw, pc->dma.cb_phys);
        while ( (hwc->pcm_reg->cs_a & PCM_CS_TXW) && timeout-- > 0 )
        {
            nanospin_ns(5000); /* 5us */
        }
        if (timeout <= 0) {
            ado_error_fmt("Failed to prime TX FIFO");
        } else {
            ado_debug(DB_LVL_DRIVER, "TX FIFO primed in %dus", (200 - timeout) * 5);
        }

        /* Enable transmitter */
        hwc->pcm_reg->cs_a |= PCM_CS_TXON;
    } else {
        /* Ensure the RX error bit is cleared and reset/clear the RX FIFO */
        hwc->pcm_reg->cs_a |= PCM_CS_RXERR | PCM_CS_RXCLR;
        nanospin_ns(4000); /* Wait 2 PCM clocks for FIFO to clear */

        /* Start DMA transfers */
        pc->dma.cfg->fn->dma_start(pc->dma.hw, pc->dma.cb_phys);

        /* Enable receiver */
        hwc->pcm_reg->cs_a |= PCM_CS_RXON;
    }
}

/**
 * Helper function used to setup the DMA control blocks
 * @param  hwc  hardware context structure.
 * @param  pc  subchannel context pointer
 * @return None
 */
static inline void bcm2711pcm_dmacb_config(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc)
{
    /* configure addresses/length for dmacb for each fragment and point to the next fragment */
    for (int f = 0; f < pc->config->mode.block.frags_total; f++) {
        const uint32_t off = f * DMA_CB_SIZE;
        uint32_t src, dst, nextcb;

        if (pc == &hwc->playback) {
            dst = BCM2711_PCM_BASE + offsetof(bcm2711pcm_reg_t, fifo_a);

            src = pc->config->dmabuf.phys_addr + f * pc->config->mode.block.frag_size;
        } else {
            src = BCM2711_PCM_BASE + offsetof(bcm2711pcm_reg_t, fifo_a);
            dst = pc->config->dmabuf.phys_addr + f * pc->config->mode.block.frag_size;
        }
        nextcb = (f == pc->config->mode.block.frags_total - 1) ? pc->dma.cb_phys : pc->dma.cb_phys + (f + 1) * DMA_CB_SIZE;
        pc->dma.cfg->fn->dma_setcb(pc->dma.cb + off, pc->dma.cb_ti, src, dst, pc->config->mode.block.frag_size, nextcb);
    }
}

/**
 * Interrupt handler routine for the capture channel
 * @param  hwc  hardware context structure.
 * @param  irq  interrupt request number
 * @return None
 */
static void bcm2711pcm_rx_isr(HW_CONTEXT_T *hwc, int32_t irq) {
    PCM_SUBCHN_CONTEXT_T *pc = &hwc->capture;
    (void) irq;
    int ret;

    ado_mutex_lock(&hwc->lock);

    ret = pc->dma.cfg->fn->dma_ack(pc->dma.hw);
    if (ret == 0) {
        ado_mutex_unlock(&hwc->lock);
        ado_debug(DB_LVL_INTERRUPT, "Not our interrupt");
        return;
    }
    if (!pc->subchn || !pc->config || !pc->active) {
        ado_mutex_unlock(&hwc->lock);
        ado_debug(DB_LVL_INTERRUPT, "Unexpected interrupt");
        return;
    }

    if (ret & DMA_ERROR) {
        pc->dma_err_count++;
    }

    if (hwc->pcm_reg->cs_a & PCM_CS_RXERR) {
        if (!pc->fifo_err_count) {
            ado_error_fmt ("RXERR detected");
        }
        pc->fifo_err_count ++;

        /* If we detect a RX error then left-right channel synchronization may be lost as the number of samples in the
         * FIFO may no longer be a multiple of the frame size. To ensue we maintain channel synchronizaion (and prevent channel swap)
         * we must toggle the receiver off and on (which resets the FIFO)
         * Note: The manual also mentions RXSYNC==0 as an indication of RX FIFO out of sync, but it is found that RXSYNC
         *       is always 0, thus not a reliable indication of a problem.
         */
        bcm2711pcm_disable (hwc, pc);
        if (pc->active == true)
            bcm2711pcm_enable (hwc, pc);
    }

    pc->buffer_pos += pc->config->mode.block.frag_size;
    if (pc->buffer_pos == pc->config->dmabuf.size ) {
        pc->buffer_pos = 0;
    }

    ado_mutex_unlock(&hwc->lock);

    dma_interrupt(pc->subchn);
}

/**
 * Interrupt handler routine for the playback channel
 * @param  hwc  hardware context structure.
 * @param  irq  interrupt request number
 * @return None
 */
static void bcm2711pcm_tx_isr(HW_CONTEXT_T *hwc, int32_t irq) {
    PCM_SUBCHN_CONTEXT_T *pc = &hwc->playback;
    int ret;
    (void) irq;

    ado_mutex_lock(&hwc->lock);

    ret = pc->dma.cfg->fn->dma_ack(pc->dma.hw);

    ado_mutex_unlock(&hwc->lock);

    if (ret == 0) {
        ado_debug(DB_LVL_INTERRUPT, "Not our interrupt");
        return;
    }
    if (!pc->subchn || !pc->config || !pc->active) {
        ado_debug(DB_LVL_INTERRUPT, "Unexpected interrupt");
        return;
    }

    dma_interrupt(pc->subchn);

    ado_mutex_lock(&hwc->lock);

    if (ret & DMA_ERROR) {
        pc->dma_err_count++;
    }

    if (hwc->pcm_reg->cs_a & PCM_CS_TXERR) {
        if (!pc->fifo_err_count) {
            ado_error_fmt ("TXERR detected");
        }
        pc->fifo_err_count ++;

        /* If we detect a TX error then left-right channel synchronization may be lost as the number of samples in the
         * FIFO may no longer be a multiple of the frame size. To ensue we maintain channel synchronizaion (prevent channel swap)
         * we must toggle the transmitter off and on (which drains the FIFO)
         * Note: The manual also mentions TXSYNC==0 as an indication of TX FIFO out of sync, but it is found that TXSYNC
         *       is always 0, thus not a reliable indication of a problem.
         */
        bcm2711pcm_disable (hwc, pc);
        if (pc->active == true)
            bcm2711pcm_enable (hwc, pc);
    }

    pc->buffer_pos += pc->config->mode.block.frag_size;
    if (pc->buffer_pos == pc->config->dmabuf.size ) {
        pc->buffer_pos = 0;
    }

    ado_mutex_unlock(&hwc->lock);
}

/**- pcm -**/

/**
 * This function is used to return to the client the capabilities of the device
 * at this instant. When the device was created, its static capabilities were
 * passed in as an argument; however, if a number of sub-channels are already
 * running, the device may no longer have the ability to support those capabilities.
 * @param hwc  hardware context structure.
 * @param pcm  pointer to pcm device
 * @param info pointer to channel info struct, info->channel indicates whether the request is
 *             for the playback or capture channel of the pcm device
 * @return EOK
 */
static int32_t hw_capabilities(HW_CONTEXT_T *hwc, ado_pcm_t *pcm, snd_pcm_channel_info_t *info) {

    ado_pcm_subchn_t *subchn, *other_subchn;
    if (info->channel == ADO_PCM_CHANNEL_PLAYBACK) {
        subchn = hwc->playback.subchn;
        other_subchn = hwc->capture.subchn;
    } else {
        subchn = hwc->capture.subchn;
        other_subchn = hwc->playback.subchn;
    }
    if (subchn) {
        /* if a subchannel is already configured return zeroed capabilities */
        info->formats       = 0;
        info->rates         = 0;
        info->min_rate      = 0;
        info->max_rate      = 0;
        info->min_voices    = 0;
        info->max_voices    = 0;
        info->min_fragment_size = 0;
        info->max_fragment_size = 0;
    } else if (other_subchn) {
        /* if a subchannel for the other direction is configured, the rate is locked */
        info->rates         = ado_pcm_rate2flag(hwc->sample_rate);
        info->min_rate      = hwc->sample_rate;
        info->max_rate      = hwc->sample_rate;
    }

    return EOK;
}

/**
 * Called when a client attempts to open a PCM stream.
 * @param hwc        hardware context structure.
 * @param ppc        pointer to subchannel context pointer, that is assigned to subchannel context created
 *                   for passed subchannel in this function
 * @param config     PCM channel config pointer
 * @param subchn     pointer to subchannel being opened
 * @param why_failed pointer to unsigned value, assigned to failure reason if appicable
 * @return Execution status
 */
static int32_t hw_acquire(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T **ppc, ado_pcm_config_t *config, ado_pcm_subchn_t *subchn, uint32_t *why_failed) {
    ado_pcm_subchn_t *target_subchn, *other_subchn;

    ado_mutex_lock(&hwc->lock);
    if (ado_pcm_subchn_is_channel(subchn, hwc->pcm, ADO_PCM_CHANNEL_PLAYBACK)) {
        *ppc = &hwc->playback;
        target_subchn = hwc->playback.subchn;
        other_subchn = hwc->capture.subchn;
    } else {
        *ppc = &hwc->capture;
        target_subchn = hwc->capture.subchn;
        other_subchn = hwc->playback.subchn;
    }
    if (target_subchn) {
        ado_mutex_unlock(&hwc->lock);
        ado_error_fmt("no channel available");
        *why_failed = SND_PCM_PARAMS_NO_CHANNEL;
        *ppc = NULL;
        return EAGAIN;
    }
    /* The sample rate is shared between playback and capture, so if a subchannel is configured
     * for the other direction, the sample rate is locked to the sample rate of the subchannel
     * configured for the other direction
     */
    if (other_subchn && config->format.rate != hwc->sample_rate) {
        ado_mutex_unlock(&hwc->lock);
        ado_error_fmt("mismatch between requested rate and rate configured for other dir %u",
            config->format.rate, hwc->sample_rate);
        *why_failed = SND_PCM_PARAMS_BAD_RATE;
        *ppc = NULL;
        return EINVAL;
    }

    /* set the codec rate and clock config if sample rate changed */
    if (hwc->sample_rate != config->format.rate) {
        hwc->sample_rate = config->format.rate;
        codec_set_rate(hwc, config->format.rate);
        bcm2711pcm_clock_config(hwc);
    }

    /* allocate dma buffer */
    config->dmabuf.flags = ADO_BUF_DMA_SAFE | ADO_BUF_CACHE;
    if (ado_pcm_buf_alloc(hwc->card, config, config->dmabuf.size, config->dmabuf.flags) == NULL) {
        ado_mutex_unlock(&hwc->lock);
        *ppc = NULL;
        return errno;
    }

    /* allocate dma control-blocks for each fragment */
    (*ppc)->dma.cb_phys = 0; /* Ensure phys address is 0 so we map anonymous memory */
    (*ppc)->dma.cb = ado_mmap_phys(hwc->card, config->mode.block.frags_total * DMA_CB_SIZE,
        ADO_BUF_DMA_SAFE, &(*ppc)->dma.cb_phys);
    if (!(*ppc)->dma.cb) {
        ado_pcm_buf_free(hwc->card, config);
        ado_mutex_unlock(&hwc->lock);
        ado_error_fmt("ado_mmap_phys failure");
        *ppc = NULL;
        return errno;
    }

    ado_debug(DB_LVL_DRIVER, "config rate %u dma %lx size %u frag_size %u cb %lx time %luus",
        config->format.rate, config->dmabuf.phys_addr, config->dmabuf.size, config->mode.block.frag_size, (*ppc)->dma.cb_phys,
        1000000l * config->mode.block.frag_size / (2 * ado_pcm_format_byte_width(hwc->sample_format) * hwc->sample_rate));

    /* Store subchannel and config */
    (*ppc)->subchn = subchn;
    (*ppc)->config = config;
    (*ppc)->buffer_pos = 0;
    (*ppc)->active = false;

    bcm2711pcm_dmacb_config (hwc, *ppc);

    ado_mutex_unlock(&hwc->lock);

    return EOK;
}

/**
 * Called by upper layer when client closes its connection to the device.
 * @param hwc    hardware context structure.
 * @param pc     subchannel context pointer
 * @param config PCM channel config pointer
 * @return EOK
 */
static int32_t hw_release(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc, ado_pcm_config_t *config) {
    ado_mutex_lock(&hwc->lock);
    pc->subchn = NULL;
    ado_munmap_phys(hwc->card, pc->dma.cb, config->mode.block.frags_total * DMA_CB_SIZE);
    ado_pcm_buf_free(hwc->card, config);
    ado_mutex_unlock(&hwc->lock);

    return EOK;
}

/**
 * Called by upper layer to prepare hardware before it's started up.
 * @param hwc    hardware context structure.
 * @param pc     subchannel context pointer
 * @param config PCM channel config pointer
 * @return EOK
 */
static int32_t hw_prepare(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc, ado_pcm_config_t *config) {

    (void) config; /* not currently used */

    ado_debug(DB_LVL_DRIVER, "");

    ado_mutex_lock(&hwc->lock);
    pc->fifo_err_count = 0;
    pc->dma_err_count = 0;
    pc->buffer_pos = 0;
    ado_mutex_unlock(&hwc->lock);

    return EOK;
}

/**
 * This function is the hardware start and stop callback.
 * @param hwc    hardware context structure.
 * @param pc     subchannel context pointer
 * @param cmd    states the go or stop condition, and is one of ADO_PCM_TRIGGER_GO,
 *               ADO_PCM_TRIGGER_STOP
 * @return EOK
 */
static int32_t hw_trigger(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc, uint32_t cmd) {

    ado_debug(DB_LVL_DRIVER, "%s subchn = 0x%p - %s", pc == &hwc->playback ? "Playback" : "Capture", pc, cmd == ADO_PCM_TRIGGER_GO ? "Go" : "Stop");
    if (cmd == ADO_PCM_TRIGGER_GO) {

        ado_mutex_lock(&hwc->lock);
        bcm2711pcm_enable(hwc, pc);
        codec_on(hwc, pc == &hwc->playback ? ADO_PCM_CHANNEL_PLAYBACK : ADO_PCM_CHANNEL_CAPTURE);
        pc->active = true;
        ado_mutex_unlock(&hwc->lock);
    } else { /* STOP */
        ado_mutex_lock(&hwc->lock);
        pc->active = false;
        codec_off(hwc, pc == &hwc->playback ? ADO_PCM_CHANNEL_PLAYBACK : ADO_PCM_CHANNEL_CAPTURE);
        bcm2711pcm_disable(hwc, pc);
        ado_mutex_unlock(&hwc->lock);

        if (pc->dma_err_count || pc->fifo_err_count) {
            ado_debug(DB_LVL_DRIVER, "DMA errors = %u, FIFO errors = %u", pc->dma_err_count, pc->fifo_err_count);
        }
    }

    return EOK;
}

/**
 * This function must return where the DMA engine is within the DMA buffer.
 * It's used by the upper layer of the driver to return timing information
 * to the client.
 * @param hwc     hardware context structure.
 * @param pc      subchannel context pointer
 * @param config  PCM channel config pointer
 * @param hw_buffer_level assigned to hw buffer level, if applicable, otherwise assigned to 0
 * @return number of bytes played from the beginning of the DMA buffer.
 */
static uint32_t hw_position(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc, ado_pcm_config_t *config, uint32_t *hw_buffer_level) {
    uint32_t pos = 0;
    if (hw_buffer_level) {
        *hw_buffer_level = 0;
    }
    ado_mutex_lock(&hwc->lock);
    pos = pc->buffer_pos;
    if (pc->active) {
        pos += config->mode.block.frag_size - pc->dma.cfg->fn->dma_len(pc->dma.hw);
        if (pos == pc->config->dmabuf.size) {
            pos = 0;
        }
    }
    ado_mutex_unlock(&hwc->lock);
    return pos;
}

/* driver options enum */
enum {
    OPT_TX_VOICES,
    OPT_RX_VOICES,
    OPT_TX_DMA,
    OPT_RX_DMA,
    OPT_GPIO,
    OPT_CLK_SRC,
    OPT_CLK_MASH,
    OPT_CLK_MODE,
    OPT_RATE,
    OPT_BITS,
    OPT_BIT_DELAY,
    OPT_CLK_POL,
    OPT_FS_POL,
    OPT_TX_DREQ,
    OPT_RX_DREQ,
    OPT_TX_PANIC,
    OPT_RX_PANIC,
    OPT_NUM
};

/* driver options strings */
static char *const s_opts[] = {
    [OPT_TX_VOICES] = "tx_voices", /* number of tx voices, 0 if tx not configured */
    [OPT_RX_VOICES] = "rx_voices", /* number of rx voices, 0 if rx not configured */
    [OPT_TX_DMA] = "tx_dma",       /* tx dma channel */
    [OPT_RX_DMA] = "rx_dma",       /* rx dma channel */
    [OPT_GPIO] = "gpio",           /* GPIO pins used for the PCM/I2S interface (primary - 18-21 or secondary - 28-31) */
    [OPT_CLK_SRC] = "clk_src",     /* clock source (osc or plld) */
    [OPT_CLK_MASH] = "clk_mash",   /* clock manager mash value */
    [OPT_CLK_MODE] = "clk_mode",   /* clock mode (master or slave) */
    [OPT_RATE] = "rate",           /* min and max sample rate in Hz, separated by ':' */
    [OPT_BITS] = "bits",           /* sample format bits (16 or 32) */
    [OPT_BIT_DELAY] = "bit_delay", /* bit delay in bit clock cycles */
    [OPT_CLK_POL] = "clk_pol",     /* bit clock polarity, 1 for I2S specific polarity, 0 for PCM interface specific clock polarity */
    [OPT_FS_POL] = "fs_pol",       /* fsync (word select) polarity, 1 for I2S specific polarity, 0 for PCM interface specific clock polarity */
    [OPT_TX_DREQ] = "tx_dreq",     /* tx dma dreq threshold */
    [OPT_RX_DREQ] = "rx_dreq",     /* rx dma dreq threshold */
    [OPT_TX_PANIC] = "tx_panic",   /* tx dma panic threshold */
    [OPT_TX_PANIC] = "rx_panic",   /* rx dma panic threshold */
    NULL
};

/**
 * This function parses the driver options
 * @param hwc  hardware context structure.
 * @param args driver options passed as a string
 * @return Execution status
 */
static int arg_parse(HW_CONTEXT_T *hwc, char *args) {

    unsigned gpio_pin_base = 18;
    unsigned gpio_fun = FSEL_FUN0;

    while (*args) {
        char *str;
        long val;

        switch (getsubopt(&args, s_opts, &str)) {
        case OPT_TX_VOICES:
            val = strtoul(str, NULL, 0);
            if (val == PCM_CHANNELS) {
                hwc->playback.configured = true;
            } else if (val == 0) {
                hwc->playback.configured = false;
            } else {
                ado_error_fmt("Unsupported tx_voices value %ld", val);
                return -1;
            }
            break;
        case OPT_RX_VOICES:
            val = strtoul(str, NULL, 0);
            if (val == PCM_CHANNELS) {
                hwc->capture.configured = true;
            } else if (val == 0) {
                hwc->capture.configured = false;
            } else {
                ado_error_fmt("Unsupported rx_voices value %ld", val);
                return -1;
            }
            break;
        case OPT_TX_DMA:
            val = strtoul(str, NULL, 0);
            if (val > DMA_CHANNEL_MAX) {
                ado_error_fmt("Unsupported tx_dma value %ld", val);
                return -1;
            }
            hwc->playback.dma.chan = val;
            break;
        case OPT_RX_DMA:
            val = strtoul(str, NULL, 0);
            if (val > DMA_CHANNEL_MAX) {
                ado_error_fmt("Unsupported rx_dma value %ld", val);
                return -1;
            }
            hwc->capture.dma.chan = val;
            break;
        case OPT_GPIO:
            if (!strcmp(str, "primary")) {
                gpio_pin_base = 18;
                gpio_fun = FSEL_FUN0;
            } else if (!strcmp(str, "secondary")) {
                gpio_pin_base = 28;
                gpio_fun = FSEL_FUN2;
            } else {
                ado_error_fmt("Unsupported gpio value %s", str);
                return -1;
            }
            break;
        case OPT_CLK_SRC:
            if (!strcmp(str, "osc")) {
                hwc->clk_src = OSC_CLK_SRC;
                hwc->clk_freq = OSC_CLK_FREQ;
            } else if (!strcmp(str, "plld")) {
                hwc->clk_src = PLLD_CLK_SRC;
                hwc->clk_freq = PLLD_CLK_FREQ;
            } else {
                ado_error_fmt("Unsupported clk_src value %s", str);
                return -1;
            }
            break;
        case OPT_CLK_MASH:
            val = strtoul(str, NULL, 0);
            if (val > CM_MASH_MAX) {
                ado_error_fmt("Unsupported clk_mash value %ld", val);
                return -1;
            }
            hwc->clk_mash = val;
            break;
        case OPT_CLK_MODE:
            if (!strcmp(str, "master")) {
                hwc->clk_mode = CLK_MODE_MASTER;
            } else if (!strcmp(str, "slave")) {
                hwc->clk_mode = CLK_MODE_SLAVE;
            } else {
                ado_error_fmt("Unsupported clk_mode value %s", str);
                return -1;
            }
            break;
        case OPT_RATE:
        {
            char* strend;
            val = strtoul(str, &strend, 0);
            if (ado_pcm_rate2flag(val) == 0) {
                ado_error_fmt("Unsupported rate %d", val);
                return -1;
            }
            hwc->sample_rate_min = val;
            if (strend && *strend == ':') {
                strend++;
                val = strtoul(strend, NULL, 0);
                if (ado_pcm_rate2flag(val) == 0) {
                    ado_error_fmt("Unsupported rate %d", val);
                    return -1;
                }
            }
            hwc->sample_rate_max = val;
            if (hwc->sample_rate_min > hwc->sample_rate_max) {
                ado_error_fmt("Min rate %d higher than max rate %d", hwc->sample_rate_min, hwc->sample_rate_max);
                return -1;
            }
            break;
        }
        case OPT_BITS:
            val = strtoul(str, NULL, 0);
            if (val == 16) {
                hwc->sample_format = SND_PCM_SFMT_S16;
            } else if (val == 24) {
                hwc->sample_format = SND_PCM_SFMT_S24_4;
            } else if (val == 32) {
                hwc->sample_format = SND_PCM_SFMT_S32;
            } else {
                ado_error_fmt("Unsupported bits value %ld", val);
                return -1;
            }
            break;
        case OPT_BIT_DELAY:
            hwc->bit_delay = strtoul(str, NULL, 0);
            break;
        case OPT_CLK_POL:
            val = strtoul(str, NULL, 0);
            if (val > 1) {
                ado_error_fmt("Unsupported clk_pol value %ld", val);
                return -1;
            }
            hwc->clk_pol = val;
            break;
        case OPT_FS_POL:
            val = strtoul(str, NULL, 0);
            if (val > 1) {
                ado_error_fmt("Unsupported fs_pol value %ld", val);
                return -1;
            }
            hwc->fs_pol = val;
            break;
        case OPT_TX_DREQ:
            val = strtoul(str, NULL, 0);
            if (val > PCM_DREQ_REQ_MAX) {
                ado_error_fmt("Unsupported tx_dreq value %ld", val);
                return -1;
            }
            hwc->playback.dma.thresh_dreq = val;
            break;
        case OPT_RX_DREQ:
            val = strtoul(str, NULL, 0);
            if (val > PCM_DREQ_REQ_MAX) {
                ado_error_fmt("Unsupported rx_dreq value %ld", val);
                return -1;
            }
            hwc->capture.dma.thresh_dreq = val;
            break;
        case OPT_TX_PANIC:
            val = strtoul(str, NULL, 0);
            if (val > PCM_DREQ_PANIC_MAX) {
                ado_error_fmt("Unsupported tx_panic value %ld", val);
                return -1;
            }
            hwc->playback.dma.thresh_panic = val;
            break;
        case OPT_RX_PANIC:
            val = strtoul(str, NULL, 0);
            if (val > PCM_DREQ_PANIC_MAX) {
                ado_error_fmt("Unsupported rx_panic value %ld", val);
                return -1;
            }
            hwc->capture.dma.thresh_panic = val;
            break;
        default:
            ado_error_fmt("Unexpected option %s", str);
            break;
        }
    }

    if (hwc->playback.dma.chan == hwc->capture.dma.chan) {
        ado_error_fmt("tx_dma and rx_dma share the same value %u", hwc->playback.dma.chan);
        return -1;
    }

    void *gpio = ado_device_mmap(NULL, BCM2711_GPIO_BASE, BCM2711_GPIO_SIZE);

    gpio_fsel_pull(gpio, gpio_pin_base + 0, gpio_fun, PULL_NONE); /* bit clk pin */
    gpio_fsel_pull(gpio, gpio_pin_base + 1, gpio_fun, PULL_NONE); /* fs pin */
    gpio_fsel_pull(gpio, gpio_pin_base + 2, gpio_fun, PULL_NONE); /* din pin */
    gpio_fsel_pull(gpio, gpio_pin_base + 3, gpio_fun, PULL_NONE); /* dout pin */

    ado_device_munmap(NULL, gpio, BCM2711_GPIO_SIZE);

    return 0;
}

/**
 * Entry point of audio driver library.
 * Called when io-audio loads HW DLL.
 * @param phwc  Pointer to hardware context structure.
 * @param card  Pointer to an internal card structure.
 * @param args  Any command-line arguments.
 * @return Execution status.
 */
ado_ctrl_dll_init_t ctrl_init;
int32_t ctrl_init(HW_CONTEXT_T **phwc, ado_card_t *card, char *args) {
    PCM_SUBCHN_CONTEXT_T *pc;
    HW_CONTEXT_T *hwc;
    uint32_t codec_rates;
    uint32_t codec_formats;
    char pcm_name[80];
    char pcm_id[64];
    int status;

    if ((hwc = ado_calloc(1, sizeof *hwc)) == NULL) {
        ado_error_fmt("Failed to allocate memory for hw context");
        return -1;
    }

    /* defaults */
    hwc->clk_freq = OSC_CLK_FREQ;
    hwc->clk_src = OSC_CLK_SRC;
    hwc->clk_mode = CLK_MODE_MASTER;
    hwc->clk_mash = 1;
    hwc->bit_delay = 1; /* I2S uses a 1 bit delay */
    hwc->fs_pol = 0; /* I2S specific FS polarity */
    hwc->clk_pol = 0; /* I2S specific CLK polarity */

    hwc->sample_rate = 0;
    hwc->sample_format = SND_PCM_SFMT_S32;
    hwc->sample_rate_min = 8000;
    hwc->sample_rate_max = 192000;

    hwc->chmap = ado_pcm_get_default_chmap (PCM_CHANNELS);

    hwc->playback.configured = true;
    hwc->playback.dma.thresh_dreq = 48;
    hwc->playback.dma.thresh_panic = 16;
    hwc->playback.dma.chan = 2;
    hwc->playback.dma.dreq = DREQ_PCMTX;
    hwc->capture.configured = false;
    hwc->capture.dma.thresh_dreq = 32;
    hwc->capture.dma.thresh_panic = 48;
    hwc->capture.dma.chan = 3;
    hwc->capture.dma.dreq = DREQ_PCMRX;

    if (arg_parse(hwc, args) != EOK) {
        ado_error_fmt("Failed to parse driver options");
        ctrl_destroy(hwc);
        return -1;
    }

    /* PCM */
    hwc->pcm_reg = ado_device_mmap(card, BCM2711_PCM_BASE, BCM2711_PCM_SIZE);
    if (!hwc->pcm_reg) {
        ado_error_fmt("Failed to map registers");
        ctrl_destroy(hwc);
        return -1;
    }

    /* DMA-TX */
    if (hwc->playback.configured) {
        pc = &hwc->playback;
        if (dma_getcfg(pc->dma.chan, &pc->dma.cfg) < 0) {
            ado_error_fmt("Unavailable dma channel %u", pc->dma.chan);
            ctrl_destroy(hwc);
            return -1;
        } else if (pc->dma.cfg->bits < 32) {
            pc->dma.cb_ti = DMA1_TI_INTEN | DMA1_TI_WAIT_RESP | DMA1_TI_SRC_INC | DMA1_TI_DST_DREQ | DMA1_TI_PERMAP(pc->dma.dreq);
        } else {
            /* SRCI_INC flag in srci register set based on TI_D_DREQ flag */
            pc->dma.cb_ti = DMA4_TI_INTEN | DMA4_TI_WAIT_WR_RESP | DMA4_TI_D_DREQ | DMA4_TI_PERMAP(pc->dma.dreq);
        }
        pc->dma.hw = ado_device_mmap(card, pc->dma.cfg->base, BCM2711_DMA_SIZE);
        if (!pc->dma.hw) {
            ado_error_fmt("Failed to map registers");
            ctrl_destroy(hwc);
            return -1;
        }
        if (ado_attach_interrupt(card, pc->dma.cfg->irq, bcm2711pcm_tx_isr, hwc) != 0) {
            ado_error_fmt("Failed to attach interrupt");
            ctrl_destroy(hwc);
            return -1;
        }
    }

    /* DMA-RX */
    if (hwc->capture.configured) {
        pc = &hwc->capture;
        if (dma_getcfg(pc->dma.chan, &pc->dma.cfg) < 0) {
            ado_error_fmt("Unavailable dma channel %u", pc->dma.chan);
            ctrl_destroy(hwc);
            return -1;
        } else if (pc->dma.cfg->bits < 32) {
            pc->dma.cb_ti = DMA1_TI_INTEN | DMA1_TI_WAIT_RESP | DMA1_TI_DST_INC | DMA1_TI_SRC_DREQ | DMA1_TI_PERMAP(pc->dma.dreq);
        } else {
            /* DSTI_INC flag in srci register set based on TI_S_DREQ flag */
            pc->dma.cb_ti = DMA4_TI_INTEN | DMA4_TI_WAIT_RD_RESP | DMA4_TI_S_DREQ | DMA4_TI_PERMAP(pc->dma.dreq);
        }
        pc->dma.hw = ado_device_mmap(card, pc->dma.cfg->base, BCM2711_DMA_SIZE);
        if (!pc->dma.hw) {
            ado_error_fmt("Failed to map registers");
            ctrl_destroy(hwc);
            return -1;
        }
        if (ado_attach_interrupt(card, pc->dma.cfg->irq, bcm2711pcm_rx_isr, hwc) != 0) {
            ado_error_fmt("Failed to attach interrupt");
            ctrl_destroy(hwc);
            return -1;
        }
    }

    /* Initialize file descriptor */
    ado_card_set_shortname(card, "BCM2711 pcm");
    ado_card_set_longname(card, "BCM2711 pcm", BCM2711_PCM_BASE);

    /* Initialize pointer to HW context */
    *phwc = (void *) hwc;
    hwc->card = card;

    ado_mutex_init(&hwc->lock);

    if (hwc->clk_mode == CLK_MODE_MASTER) {
        bcm2711pcm_clock_config(hwc);
    }

    bcm2711pcm_config(hwc);

    hwc->playback.caps.chn_flags = SND_PCM_CHNINFO_BLOCK | SND_PCM_CHNINFO_INTERLEAVE |
        SND_PCM_CHNINFO_BLOCK_TRANSFER | SND_PCM_CHNINFO_MMAP | SND_PCM_CHNINFO_MMAP_VALID;
    if (hwc->sample_format == SND_PCM_SFMT_S16) {
        hwc->playback.caps.formats = SND_PCM_FMT_S16;
    } else if (hwc->sample_format == SND_PCM_SFMT_S24_4) {
        hwc->playback.caps.formats = SND_PCM_FMT_S24_4;
    } else {
        hwc->playback.caps.formats = SND_PCM_FMT_S32;
    }
    hwc->playback.caps.rates = ado_pcm_rates2flag(hwc->sample_rate_min, hwc->sample_rate_max);
    hwc->playback.caps.min_rate = hwc->sample_rate_min;
    hwc->playback.caps.max_rate = hwc->sample_rate_max;
    hwc->playback.caps.min_voices = PCM_CHANNELS;
    hwc->playback.caps.max_voices = PCM_CHANNELS;
    /* Set max_frags so that all DMA control blocks fit into 4k of memory; one control block
     * has size DMA_CB_SIZE = 32, so we can have 4096/32 = 128 control blocks; if both playback
     * and capture configured, this means a max of 64 frags per direction; otherwise we can have
     * 128 frags for the configured direction
     */
    if (hwc->playback.configured && hwc->capture.configured) {
        hwc->playback.caps.max_frags = 64;
    } else {
        hwc->playback.caps.max_frags = 128;
    }
    hwc->playback.caps.min_fragsize = 64;
    hwc->playback.caps.max_fragsize = 64 * 1024;
    hwc->playback.caps.chmap = ado_pcm_get_default_chmap(PCM_CHANNELS);

    hwc->playback.funcs.capabilities = hw_capabilities;
    hwc->playback.funcs.aquire = hw_acquire;
    hwc->playback.funcs.release = hw_release;
    hwc->playback.funcs.prepare = hw_prepare;
    hwc->playback.funcs.trigger = hw_trigger;
    hwc->playback.funcs.position = hw_position;

    /* Since capture capabilities and callback funcs are the same as the playback ones, copy them from playback */
    memcpy (&hwc->capture.caps, &hwc->playback.caps, sizeof(hwc->capture.caps));
    memcpy (&hwc->capture.funcs, &hwc->playback.funcs, sizeof(hwc->capture.funcs));

    status = build_codec_mixer(hwc->card, hwc);
    if (status != EOK) {
        ado_error_fmt ("Failed building the mixer (%d)", status);
        ctrl_destroy(hwc);
        return -1;
    }

    /* If the codec has codec_on/codec_off callbacks, invoke codec_off to mute the codec;
     * the trigger GO callback will invoke codec_on to unmute the codec and trigger STOP
     * will call codec_off to revert the codec to the mute state
     */
    codec_off(hwc, ADO_PCM_CHANNEL_PLAYBACK);
    codec_off(hwc, ADO_PCM_CHANNEL_CAPTURE);

    /* restrict rates and formats capabilities based on codec capabilities */
    codec_supported_rates(hwc, &codec_rates);
    codec_supported_formats(hwc, &codec_formats);
    hwc->playback.caps.formats &= codec_formats;
    hwc->capture.caps.formats &= codec_formats;
    hwc->playback.caps.rates &= codec_rates;
    hwc->capture.caps.rates &= codec_rates;
    if (hwc->playback.caps.formats == 0 || hwc->playback.caps.rates == 0) {
        ado_error_fmt("Configured format or rate not supported by codec - codec formats %u rates %u",
            codec_formats, codec_rates);
        ctrl_destroy(hwc);
        return -1;
    }

    snprintf(pcm_name, sizeof pcm_name, "pcm %x", BCM2711_PCM_BASE);
    snprintf(pcm_id, sizeof pcm_id, "pcm-%x", BCM2711_PCM_BASE);

    if (ado_pcm_create(card, pcm_name, 0, pcm_id,
        hwc->playback.configured ? 1 : 0, &hwc->playback.caps, &hwc->playback.funcs,
        hwc->capture.configured ? 1 : 0,  &hwc->capture.caps,  &hwc->capture.funcs,
        hwc->mixer,  &hwc->pcm)) {
        ado_error_fmt("Failed to create pcm devices (%s)", strerror(errno));
        ctrl_destroy(hwc);
        return -1;
    }

    codec_set_default_group(hwc, hwc->pcm, ADO_PCM_CHANNEL_PLAYBACK, 0);
    codec_set_default_group(hwc, hwc->pcm, ADO_PCM_CHANNEL_CAPTURE, 0);

    return 0;
}

/**
 * Entry point called when card is unmounted.
 * @param hwc  hardware context structure.
 * @return Execution status.
 */
ado_ctrl_dll_destroy_t ctrl_destroy;
int32_t ctrl_destroy(HW_CONTEXT_T *hwc) {
    bcm2711pcm_unconfig(hwc);
    if (hwc->playback.dma.hw) {
        ado_device_munmap(hwc->card, hwc->playback.dma.hw, BCM2711_DMA_SIZE);
    }
    if (hwc->capture.dma.hw) {
        ado_device_munmap(hwc->card, hwc->capture.dma.hw, BCM2711_DMA_SIZE);
    }
    if (hwc->pcm_reg) {
        ado_device_munmap(hwc->card, (void *) hwc->pcm_reg, BCM2711_PCM_SIZE);
    }
    ado_mutex_destroy(&hwc->lock);
    if (hwc->playback.configured) {
        dma_dropcfg(hwc->playback.dma.cfg);
    }
    if (hwc->capture.configured) {
        dma_dropcfg(hwc->capture.dma.cfg);
    }
    ado_free(hwc);
    return EOK;
}

/**
 * DLL Version callback
 * @param major  assigned to major version number
 * @param minor  assigned to minor version number
 * @param date   assigned to binary generation date
 * @return None.
 */
ado_dll_version_t ctrl_version;
void ctrl_version ( int *major, int *minor, const char **date )
{
    *major = ADO_MAJOR_VERSION;
    *minor = 1;
    *date = __DATE__;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/pcm.c $ $Rev: 934572 $")
#endif
