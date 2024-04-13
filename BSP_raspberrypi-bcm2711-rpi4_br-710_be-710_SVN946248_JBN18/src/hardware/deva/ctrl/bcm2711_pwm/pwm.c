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

/*-
 *  bcm2711 sound using PWM duty-cycle for PCM sample data and PWM period for PCM sample width
 *
 * - PWM fed through DMA to 64-entry 32-bit FIFO (with interleaved channels),
 *   meaning 4-byte wide interleaved samples are required; given that io-audio
 *   doesn't support unsigned formats other than u8 on the hw side (see
 *   ado_pcm_pick_best_format), this driver will support only the S32 format
 *
 * - use reconstitute callback to convert from signed 32-bit to unsigned PWM duty-cycle
 *   (PWM RNG1 and RNG2 specify the sample-width)
 *
 * - configure PWM clock based on sample-rate and sample-width
 *   (e.g. supporting 10-bit samples at 96kHz requires clock: 96000 * 2^10 == 98304000)
 */


#include "pwm.h"
#include "cm.h"
#include "gpio.h"
#include "dma.h"

#include <aarch64/bcm2711.h>
#include <drvr/hwinfo.h>

#define PWM_CLOCK_FREQ      750*1000*1000l
#define PWM_CLOCK_SRC       6      /* PLLD */
#define PWM_CHANNELS        2      /* 2 channel */
/* sample precision: log2(PWM_CLOCK_FREQ / SAMPLE_RATE)
 * Note: Sample precisions over 10bits are unstable (can lead to static or PWM stream halt),
 *       so we max out at 10.
 */
#define PWM_MAX_PREC        10

/**
 * Helper function used to determine the system clock and sample precision based
 * on configured sample rate and source clock frequency and then configure the
 * clock manager accordingly
 * @param  hwc  hardware context structure.
 * @return None
 */
static void pwm_clock_config(HW_CONTEXT_T *hwc) {
    uint32_t sysclk;
    if (!hwc->sample_rate) {
        return;
    }
    /* Determine the sample_prec and sysclk values, such that we get the highest
     * sample_prec value for which sysclk is smaller or equal half of PWM_CLOCK_FREQ
     * (this is required since sysclk must be generated starting from PWM_CLOCK_FREQ)
     * Master_Clk_Freq = (Sample_Rate * 2^n_bits) = (Sample_Rate << n_bits)
     */
    hwc->sample_prec = PWM_MAX_PREC;
    while ((sysclk = (hwc->sample_rate << hwc->sample_prec)) > PWM_CLOCK_FREQ/2) {
        hwc->sample_prec--;
    }

    ado_debug(DB_LVL_DRIVER, "rate %u, prec %u, sysclk %lu",
        hwc->sample_rate, hwc->sample_prec, sysclk);

    cm_config(BCM2711_CM_PWM, sysclk, PWM_CLOCK_FREQ, PWM_CLOCK_SRC, hwc->clk_mash);

    /* Update sample range based on clock config */
    hwc->pwm->rng1 = (1 << hwc->sample_prec); /* channel 1 range */
    hwc->pwm->rng2 = (1 << hwc->sample_prec); /* channel 2 range */
}

/**
 * Helper function used to disable the PWM module used by this driver instance
 * @param  hwc  hardware context structure.
 * @return None
 */
static void pwm_disable(HW_CONTEXT_T *hwc) {
    int i = 0;

    hwc->dma.cfg->fn->dma_abort(hwc->dma.hw);
    while (!(hwc->pwm->sta & PWM_STA_EMPT) && (i < 100)) {
        nanospin_ns(5000); /* 5us */
        i++;
    }
    if (i >= 100) {
        ado_error_fmt("Timed out (%dus) waiting for FIFO to empty, ctl = 0x%x, sta =  0x%x",
                  i * 5, hwc->pwm->ctl, hwc->pwm->sta);
    } else {
        ado_debug(DB_LVL_DRIVER, "FIFO emptied in %dus, ctl = 0x%x sta = 0x%x",
                  i * 5, hwc->pwm->ctl, hwc->pwm->sta);
    }
    hwc->pwm->ctl &= ~(PWM_CTL_PWEN1 | PWM_CTL_PWEN2);
    hwc->pwm->sta |= (hwc->pwm->sta & PWM_STA_ALL_ERR);
}

/**
 * Helper function used to configure and enable the PWM module used by this driver instance
 * @param  hwc  hardware context structure.
 * @return EOK on success, EIO on error
 */
static int32_t pwm_enable(HW_CONTEXT_T *hwc) {
    int i = 0;
    const uint32_t transmit_active = (PWM_STA_STA1 | PWM_STA_STA2);

    /* Reset FIFO (ensure clean FIFO) */
    hwc->pwm->ctl |= PWM_CTL_CLRF;
    hwc->dma.cfg->fn->dma_start(hwc->dma.hw, hwc->dma.cb_phys);
    /* Wait for FIFO fill (prime before enable) */
    while ((hwc->pwm->sta & PWM_STA_EMPT) && (i < 200)) {
        nanospin_ns(5000); /* 5us */
        i++;
    }
    if (i >= 200) {
        ado_error_fmt("Timed out (%dus) waiting for FIFO to go non-empty, ctl = 0x%x, sta =  0x%x",
                  i * 5, hwc->pwm->ctl, hwc->pwm->sta);
    } else {
        ado_debug(DB_LVL_DRIVER, "FIFO data in %dus, ctl = 0x%x sta = 0x%x",
                  i * 5, hwc->pwm->ctl, hwc->pwm->sta);
    }

    /* Enable transmitter */
    hwc->pwm->ctl |= PWM_CTL_PWEN1 | PWM_CTL_PWEN2;

    /* The transmitter does not always start when we enable,
     * as a workaround we check the status register to verify
     * if the transmitter is active, if not then toggle the
     * enables to try again
     * Note: We need a delay to give the transmitter
     *       a chance to react to the enable
     */
    nanospin_ns(2000); /* 2us */

    i = 0;
    while (((hwc->pwm->sta & transmit_active) != transmit_active) && i < 5) {
        ado_error_fmt("Transmitter failed to start, toggling enable");
        hwc->pwm->ctl &= ~(PWM_CTL_PWEN1 | PWM_CTL_PWEN2);
        hwc->pwm->ctl |= PWM_CTL_PWEN1 | PWM_CTL_PWEN2;
        nanospin_ns(2000); /* 2us */
        i++;
    }
    if (i == 5) {
        ado_error_fmt("Failed to enable transmitter");
        pwm_disable(hwc);
        return EIO;
    }

    return EOK;
}

/**
 * Interrupt handler routine
 * @param  hwc  hardware context structure.
 * @param  irq  interrupt request number
 * @return None
 */
static void pwm_isr(HW_CONTEXT_T *hwc, int32_t irq) {
    uint32_t sta;
    PCM_SUBCHN_CONTEXT_T *pc = &hwc->sc;
    (void) irq; /* not currently used */

    ado_mutex_lock(&hwc->lock);

    if (hwc->dma.cfg->fn->dma_ack(hwc->dma.hw) == 0 || pc->active == 0) {
        /* dma_ack returns 0 if DMA channel not in use or there is no completed current DMA transfer */
        ado_mutex_unlock(&hwc->lock);
        return;
    }

    ado_mutex_unlock(&hwc->lock);

    dma_interrupt(pc->subchn); /* acknowledge each fragment */

    ado_mutex_lock(&hwc->lock);

    sta = hwc->pwm->sta;

    /* check for pwm error status */
    if (sta & PWM_STA_ALL_ERR) {
        ado_error_fmt("PWM error status 0x%x", sta & PWM_STA_ALL_ERR);
        hwc->pwm->sta |= (sta & PWM_STA_ALL_ERR);
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

    if (hwc->sc.subchn) {
        /* if a subchannel is already configured return zeroed capabilities */
        info->formats       = 0;
        info->rates         = 0;
        info->min_rate      = 0;
        info->max_rate      = 0;
        info->min_voices    = 0;
        info->max_voices    = 0;
        info->min_fragment_size = 0;
        info->max_fragment_size = 0;
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

    ado_mutex_lock(&hwc->lock);
    if (hwc->sc.subchn) {
        *why_failed = SND_PCM_PARAMS_NO_CHANNEL;
        ado_mutex_unlock(&hwc->lock);
        return EAGAIN;
    }

    if (config->format.rate != hwc->sample_rate) {
        hwc->sample_rate = config->format.rate;
        pwm_clock_config(hwc);
    }

    /* allocate dma buffer */
    config->dmabuf.flags = ADO_BUF_DMA_SAFE | ADO_BUF_CACHE;
    if (ado_pcm_buf_alloc(hwc->card, config, config->dmabuf.size, config->dmabuf.flags) == NULL) {
        ado_mutex_unlock(&hwc->lock);
        return errno;
    }

    /* allocate dma control-blocks for each fragment */
    hwc->dma.cb_phys = 0; /* Ensure phys address is 0 so we map anonymous memory */
    hwc->dma.cb = ado_mmap_phys(hwc->card, config->mode.block.frags_total * DMA_CB_SIZE,
        ADO_BUF_DMA_SAFE, &hwc->dma.cb_phys);
    if (!hwc->dma.cb) {
        ado_pcm_buf_free(hwc->card, config);
        ado_mutex_unlock(&hwc->lock);
        ado_error_fmt("ado_mmap_phys failure");
        return errno;
    }
    ado_debug(DB_LVL_DRIVER, "config rate %u dma %lx size %u frag_size %u cb %lx time %luus",
        config->format.rate, config->dmabuf.phys_addr, config->dmabuf.size, config->mode.block.frag_size,
        hwc->dma.cb_phys, 1000000l * config->mode.block.frag_size / (8 * hwc->sample_rate));

    /* configure addresses/length for dma cb for each fragment and point to the next fragment */
    for (int f = 0; f < config->mode.block.frags_total; f++) {
        const uint32_t off = f * DMA_CB_SIZE;
        uint32_t src, dst, nextcb;

        dst = hwc->pwmbase + offsetof(pwm_reg_t, fif1);
        src = config->dmabuf.phys_addr + f * config->mode.block.frag_size;
        nextcb = hwc->dma.cb_phys + ((f + 1) % config->mode.block.frags_total) * DMA_CB_SIZE;
        hwc->dma.cfg->fn->dma_setcb(hwc->dma.cb + off, hwc->dma.cb_ti, src, dst, config->mode.block.frag_size, nextcb);
    }

    /* Store subchannel and config */
    hwc->sc.subchn = subchn;
    hwc->sc.config = config;
    hwc->sc.buffer_pos = 0;

    ado_mutex_unlock(&hwc->lock);
    *ppc = &hwc->sc;

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
    ado_munmap_phys(hwc->card, (void *) hwc->dma.cb, config->mode.block.frags_total * DMA_CB_SIZE);
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
    pc->buffer_pos = 0;
    ado_mutex_unlock(&hwc->lock);

    return EOK;
}

/*
 * This function is a catchall for hardware with very strange format support.
 * It's used to reformat the data in the DMA buffer for the strange hardware
 * requirements.
 * @param hwc    hardware context structure.
 * @param pc     subchannel context pointer
 * @param dmaptr fragment memory pointer
 * @param size   fragment size
 * @return EOK.
 */
static int32_t hw_reconstitute(HW_CONTEXT_T *hwc, PCM_SUBCHN_CONTEXT_T *pc, int8_t *dmaptr, size_t size) {
    uint32_t *sample = (void *) dmaptr;
    const size_t nsamples = size / sizeof(*sample);

    for (size_t n = 0; n < nsamples; n++) {
        /* shift down high-order precision bits; given that the original format is signed,
         * toggle sign-bit, in order to convert to an unsigned sample_prec wide value,
         * which is required for PWM mode
         */
        sample[n] = (sample[n] >> (32 - hwc->sample_prec)) ^ (1 << (hwc->sample_prec - 1));
    }

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
    int status = EOK;
    ado_debug(DB_LVL_DRIVER, "cmd=%u", cmd);

    ado_mutex_lock(&hwc->lock);
    if (cmd == ADO_PCM_TRIGGER_GO) {
        if ((status = pwm_enable(hwc)) == EOK)
            pc->active = true;
    } else { /* STOP */
        pc->active = false;
        pwm_disable(hwc);
    }
    ado_mutex_unlock(&hwc->lock);

    return status;
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
        pos += config->mode.block.frag_size - hwc->dma.cfg->fn->dma_len(hwc->dma.hw);
        if (pos == pc->config->dmabuf.size) {
            pos = 0;
        }
    }
    ado_mutex_unlock(&hwc->lock);
    return pos;
}

/* driver options enum */
enum {
    OPT_PWM = 0,
    OPT_DMA,
    OPT_RATE,
    OPT_CLK_MASH,
    OPT_MSEN,
    OPT_DREQ,
    OPT_PANIC,
    OPT_NUM
};

/* driver options strings */
static char *const s_opts[] = {
    [OPT_PWM] = "pwm",           /* pwm controller instance */
    [OPT_DMA] = "dma",           /* dma channel */
    [OPT_RATE] = "rate",         /* min/max sample rate of audio in Hz */
    [OPT_CLK_MASH] = "clk_mash", /* clock manager mash value */
    [OPT_MSEN] = "msen",         /* msen enable/disable */
    [OPT_DREQ] = "dreq",         /* dma dreq threshold */
    [OPT_PANIC] = "panic",       /* dma panic threshold */
    NULL
};

/**
 * This function parses the driver options
 * @param hwc  hardware context structure.
 * @param args driver options passed as a string
 * @return Execution status
 */
static int arg_parse(HW_CONTEXT_T *hwc, char *args) {

    int dma_chan = 2;              /* default dma channel */

    while (*args) {
        char *str;
        long val;

        switch (getsubopt(&args, s_opts, &str)) {
        case OPT_PWM:
            val = strtoul(str, NULL, 0);
            if (val == 0 || val == BCM2711_PWM0_BASE) {
                hwc->pwm_idx = 0;
            } else if (val == 1 || val == BCM2711_PWM1_BASE) {
                hwc->pwm_idx = 1;
            } else {
                ado_error_fmt("Unsupported PWM module index or base address %d", val);
                return -1;
            }
            break;
        case OPT_DMA:
            val = strtoul(str, NULL, 0);
            dma_chan = val;
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
        case OPT_CLK_MASH:
            val = strtoul(str, NULL, 0);
            if (val > CM_MASH_MAX) {
                ado_error_fmt("Unsupported clk_mash value %ld", val);
                return -1;
            }
            hwc->clk_mash = val;
            break;
        case OPT_MSEN:
            val = strtoul(str, NULL, 0);
            if (val) {
                hwc->pwm_msen = PWM_CTL_MSEN1 | PWM_CTL_MSEN2;
            } else {
                hwc->pwm_msen = 0;
            }
            break;
        case OPT_DREQ:
            val = strtoul(str, NULL, 0);
            hwc->dma.thresh_dreq = val;
            break;
        case OPT_PANIC:
            val = strtoul(str, NULL, 0);
            hwc->dma.thresh_panic = val;
            break;
        }
    }

    volatile uint32_t *gpio = ado_device_mmap(NULL, BCM2711_GPIO_BASE, __PAGESIZE);

    if (hwc->pwm_idx == 0) {
        gpio_fsel_pull(gpio, 18, FSEL_FUN5, PULL_NONE); // pwm0_0
        gpio_fsel_pull(gpio, 19, FSEL_FUN5, PULL_NONE); // pwm0_1
        hwc->pwmbase = BCM2711_PWM0_BASE;
        hwc->dma.dreq = DREQ_PWM0;
    } else {
        /* default configuration */
        gpio_fsel_pull(gpio, 40, FSEL_FUN0, PULL_NONE); // pwm1_0
        gpio_fsel_pull(gpio, 41, FSEL_FUN0, PULL_NONE); // pwm1_1
        hwc->pwmbase = BCM2711_PWM1_BASE;
        hwc->dma.dreq = DREQ_PWM1;
    }

    ado_device_munmap(NULL, (void*) gpio, __PAGESIZE);

    if (dma_getcfg(dma_chan, &hwc->dma.cfg) < 0) {
        ado_error_fmt("Unavailable dma channel %d", dma_chan);
        return -1;
    } else {
        if (hwc->dma.cfg->bits < 32) {
            hwc->dma.cb_ti = DMA1_TI_INTEN | DMA1_TI_WAIT_RESP | DMA1_TI_SRC_INC | DMA1_TI_DST_DREQ | DMA1_TI_PERMAP(hwc->dma.dreq);
        } else {
            /* SRC_INC flag in srci register set based on TI_D_DREQ flag */
            hwc->dma.cb_ti = DMA4_TI_INTEN | DMA4_TI_WAIT_WR_RESP | DMA4_TI_D_DREQ | DMA4_TI_PERMAP(hwc->dma.dreq);
        }
    }

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
    /* channel order is right then left */
    static const snd_pcm_chmap_t chmap = {
        .pos = { SND_CHMAP_FR, SND_CHMAP_FL },
        .channels = PWM_CHANNELS,
    };

    HW_CONTEXT_T *hwc;
    char pcm_name[80];
    char pcm_id[64];

    if ((hwc = ado_calloc(1, sizeof *hwc)) == NULL) {
        ado_error_fmt("Failed to allocate memory for hw context");
        return -1;
    }

    hwc->sc.active = false;
    hwc->sc.buffer_pos = 0;
    hwc->clk_mash = 1;

    /* defaults */
    hwc->dma.thresh_dreq = 32;
    hwc->dma.thresh_panic = 4;
    hwc->pwm_msen = PWM_CTL_MSEN1 | PWM_CTL_MSEN2;
    hwc->pwm_idx = 1;
    hwc->sample_rate = 0;
    hwc->sample_rate_min = 44100;
    hwc->sample_rate_max = 48000;

    if (arg_parse(hwc, args) != EOK) {
        ado_error_fmt("Failed to parse driver options");
        ctrl_destroy(hwc);
        return -1;
    }

    hwc->pwm = ado_device_mmap(card, hwc->pwmbase, BCM2711_PWM_SIZE);
    if (!hwc->pwm) {
        ado_error_fmt("Failed to map PWM registers");
        ctrl_destroy(hwc);
        return -1;
    }
    /* Make sure PWM is disabled */
    hwc->pwm->ctl = 0;
    hwc->pwm->dmac = 0;

    hwc->dma.hw = ado_device_mmap(card, hwc->dma.cfg->base, BCM2711_DMA_SIZE);
    if (!hwc->dma.hw) {
        ado_error_fmt("Failed to map DMA registers");
        ctrl_destroy(hwc);
        return -1;
    }
    /* Initialize file descriptor */
    ado_card_set_shortname(card, "bcm2711 pwm");
    ado_card_set_longname(card, "bcm2711 pwm", hwc->pwmbase);

    /* Initialize pointer to HW context */
    *phwc = (void *) hwc;
    hwc->card = card;

    ado_mutex_init(&hwc->lock);

    if (ado_attach_interrupt(card, hwc->dma.cfg->irq, pwm_isr, hwc) != 0) {
        ado_error_fmt("Failed to attach interrupt");
        ctrl_destroy(hwc);
        return -1;
    }

    ado_debug(DB_LVL_DRIVER, "pwm %x dma %x dreq %x irq %x", hwc->pwmbase, hwc->dma.cfg->base, hwc->dma.dreq, hwc->dma.cfg->irq);

    hwc->sc.caps.chn_flags = SND_PCM_CHNINFO_BLOCK | SND_PCM_CHNINFO_INTERLEAVE |
        SND_PCM_CHNINFO_BLOCK_TRANSFER | SND_PCM_CHNINFO_MMAP | SND_PCM_CHNINFO_MMAP_VALID;

    hwc->sc.caps.formats = SND_PCM_FMT_S32;

    hwc->sc.caps.rates = ado_pcm_rates2flag(hwc->sample_rate_min, hwc->sample_rate_max);
    hwc->sc.caps.min_rate = hwc->sample_rate_min;
    hwc->sc.caps.max_rate = hwc->sample_rate_max;

    hwc->sc.caps.min_voices = PWM_CHANNELS;
    hwc->sc.caps.max_voices = PWM_CHANNELS;
    hwc->sc.caps.min_fragsize = 64;
    hwc->sc.caps.max_fragsize = 64 * 1024;
    /* Set max_frags so that all DMA control blocks fit into 4k of memory; one control block
     * has size DMA_CB_SIZE = 32, so we can have 4096/32 = 128 control blocks
     */
    hwc->sc.caps.max_frags = 128;
    hwc->sc.caps.chmap = &chmap;

    hwc->sc.funcs.capabilities = hw_capabilities;
    hwc->sc.funcs.aquire = hw_acquire;
    hwc->sc.funcs.release = hw_release;
    hwc->sc.funcs.prepare = hw_prepare;
    hwc->sc.funcs.trigger = hw_trigger;
    hwc->sc.funcs.position = hw_position;
    hwc->sc.funcs.reconstitute = hw_reconstitute;

    pwm_clock_config(hwc);

    /* Configure DMA */
    hwc->pwm->dmac = PWM_DMAC_ENAB | (hwc->dma.thresh_panic<<8) | hwc->dma.thresh_dreq;
    /* Configure PWM settings
     * Clear the FIFOs, set PWM mode, enable FIFO use
     */
    hwc->pwm->ctl = PWM_CTL_CLRF | PWM_CTL_USEF1 | PWM_CTL_USEF2 | hwc->pwm_msen;

    if (ado_mixer_create(card, "pwm", &hwc->mixer, hwc) != EOK) {
        ado_error_fmt("Failed to create mixer");
        ctrl_destroy(hwc);
        return -1;
    }

    snprintf(pcm_name, sizeof pcm_name, "pwm %x", hwc->pwmbase);
    snprintf(pcm_id, sizeof pcm_id, "pwm-%x", hwc->pwmbase);

    if (ado_pcm_create(card, pcm_name, 0, pcm_id,
            1, &hwc->sc.caps, &hwc->sc.funcs, 0, NULL, NULL, hwc->mixer, &hwc->pcm)) {
        ado_error_fmt("Failed to create pcm device (%s)", strerror(errno));
        ctrl_destroy(hwc);
        return -1;
    }

    return 0;
}

/**
 * Entry point called when card is unmounted.
 * @param hwc  hardware context structure.
 * @return EOK
 */
ado_ctrl_dll_destroy_t ctrl_destroy;
int32_t ctrl_destroy(HW_CONTEXT_T *hwc) {
    if (hwc->pwm) {
        /* Disable PWM interface */
        hwc->pwm->ctl = 0;
        hwc->pwm->dmac = 0;
        ado_device_munmap(hwc->card, (void *) hwc->pwm, BCM2711_PWM_SIZE);
    }
    if (hwc->dma.hw) {
        ado_device_munmap(hwc->card, hwc->dma.hw, BCM2711_DMA_SIZE);
    }
    ado_mutex_destroy(&hwc->lock);
    dma_dropcfg(hwc->dma.cfg);
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
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pwm/pwm.c $ $Rev: 934572 $")
#endif
