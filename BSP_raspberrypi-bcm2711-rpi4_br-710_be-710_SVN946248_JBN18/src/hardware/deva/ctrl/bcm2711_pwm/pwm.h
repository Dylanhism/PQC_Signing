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

#ifndef __PWM_H__
#define __PWM_H__

typedef struct pwm_subchn pwm_subchn_t;
typedef struct pwm_context pwm_context_t;

#define PCM_SUBCHN_CONTEXT_T pwm_subchn_t
#define MIXER_CONTEXT_T pwm_context_t
#define HW_CONTEXT_T pwm_context_t

#include "dma.h"
#include <audio_driver.h>

/* BCM2711 PWM register map */

/* PWM CTL register - PWM Control */
#define PWM_CTL_MSEN2   (1<<15)  /* Channel 2 M/S Enable (0: PWM algorithm is used; 1: M/S transmission is used) */
#define PWM_CTL_MSEN1   (1<<7)   /* Channel 1 M/S Enable (0: PWM algorithm is used; 1: M/S transmission is used) */
#define PWM_CTL_RSVD    (1<<14)  /* Reserved */
#define PWM_CTL_CLRF    (1<<6)   /* Clear Fifo (1: Clears FIFO; 0: Has no effect) */
#define PWM_CTL_USEF2   (1<<13)  /* Channel 2 Use Fifo (0: Data register is transmitted; 1: Fifo is used for transmission) */
#define PWM_CTL_USEF1   (1<<5)   /* Channel 1 Use Fifo (0: Data register is transmitted; 1: Fifo is used for transmission) */
#define PWM_CTL_POLA2   (1<<12)  /* Channel 2 Polarity (0: 0=low 1=high; 1: 1=low 0=high) */
#define PWM_CTL_POLA1   (1<<4)   /* Channel 1 Polarity (0: 0=low 1=high; 1: 1=low 0=high) */
#define PWM_CTL_SBIT2   (1<<11)  /* Channel 2 Silence Bit (Defines the state of the output when no transmission takes place) */
#define PWM_CTL_SBIT1   (1<<3)   /* Channel 1 Silence Bit (Defines the state of the output when no transmission takes place) */
#define PWM_CTL_RPTL2   (1<<10)  /* Channel 2 Repeat Last FIFO Data */
                            /* 0: Transmission interrupts when FIFO is empty */
                            /* 1: Last data in FIFO is transmitted repeatedly until FIFO is not empty */
#define PWM_CTL_RPTL1   (1<<2)   /* Channel 1 Repeat Last FIFO Data */
                            /* 0: Transmission interrupts when FIFO is empty */
                            /* 1: Last data in FIFO is transmitted repeatedly until FIFO is not empty */
#define PWM_CTL_MODE2   (1<<9)   /* Channel 2 Mode (0: PWM mode; 1: Serialiser mode) */
#define PWM_CTL_MODE1   (1<<1)   /* Channel 1 Mode (0: PWM mode; 1: Serialiser mode) */
#define PWM_CTL_PWEN2   (1<<8)   /* Channel 2 Enable (0: Channel is disabled; 1: Channel is enabled) */
#define PWM_CTL_PWEN1   (1<<0)   /* Channel 1 Enable (0: Channel is disabled; 1: Channel is enabled) */

/* PWM STA register - PWM Status */
#define PWM_STA_STA2    (1<<10)  /* Channel 2 State */
#define PWM_STA_STA1    (1<<9)   /* Channel 1 State */
#define PWM_STA_BERR    (1<<8)   /* Bus Error Flag */
#define PWM_STA_GAPO2   (1<<5)   /* Channel 2 Gap Occurred Flag */
#define PWM_STA_GAPO1   (1<<4)   /* Channel 1 Gap Occurred Flag */
#define PWM_STA_RERR1   (1<<3)   /* Fifo Read Error Flag */
#define PWM_STA_WERR1   (1<<2)   /* Fifo Write Error Flag */
#define PWM_STA_EMPT    (1<<1)   /* Fifo Empty Flag */
#define PWM_STA_FULL    (1<<0)   /* Fifo Full Flag */
#define PWM_STA_ALL_ERR (PWM_STA_BERR | PWM_STA_GAPO2 | PWM_STA_GAPO1 | PWM_STA_RERR1 | PWM_STA_WERR1)

/* PWM DMAC register - PWM DMA Configuration */
#define PWM_DMAC_ENAB   (1<<31)  /* DMA Enable */
#define PWM_DMAC_PANIC  (255<<8) /* DMA Threshold for PANIC signal */
#define PWM_DMAC_DREQ   (255<<0) /* DMA Threshold for DREQ signal */

/* PWM register map; there are two PWM modules, each with their own register map;
 * the base addresses (BCM2711_PWM0_BASE and BCM2711_PWM1_BASE) and
 * size (BCM2711_PWM_SIZE) of the PWM register maps are defined in bcm2711.h
 */
typedef struct pwm_reg {
    uint32_t ctl;               /* 0x00 CTL register - PWM Control */
    uint32_t sta;               /* 0x04 STA register - PWM Status */
    uint32_t dmac;              /* 0x08 DMAC register - PWM DMA Configuration */
    uint32_t _x0c;              /* 0x0c Reserved */
    uint32_t rng1;              /* 0x10 RNG1 register - PWM Channel 1 Range */
    uint32_t dat1;              /* 0x14 DAT1 register - PWM Channel 1 Data */
    uint32_t fif1;              /* 0x18 FIF1 register - PWM FIFO Input */
    uint32_t _x1c;              /* 0x1c Reserved */
    uint32_t rng2;              /* 0x20 RNG2 register - PWM Channel 2 Range */
    uint32_t dat2;              /* 0x24 DAT2 register - PWM Channel 2 Data */
} pwm_reg_t;

typedef struct dma_context {
    const struct dma_config *cfg;
    void *hw;                   /* dma hardware */
    void *cb;                   /* dma control-block */
    off64_t cb_phys;            /* dma control-block legacy physical address */
    uint32_t cb_ti;
    uint32_t dreq;
    uint32_t thresh_dreq;       /* DMA Threshold for DREQ signal */
    uint32_t thresh_panic;      /* DMA Threshold for PANIC signal */
} dma_context_t;

struct pwm_subchn {
    ado_pcm_subchn_t *subchn;   /* Pointer to subchannel structure */
    ado_pcm_config_t *config;   /* Data structure that describes the configuration of a PCM subchannel */
    ado_pcm_cap_t caps;         /* Data structure of capabilities of a PCM device */
    ado_pcm_hw_t funcs;         /* Data structure of callbacks for PCM devices */
    uint32_t buffer_pos;
    bool active;
};

struct pwm_context {
    pthread_mutex_t lock;
    ado_card_t *card;
    ado_mixer_t *mixer;
    pwm_subchn_t sc;
    ado_pcm_t *pcm;
    uint32_t pwm_idx;
    volatile pwm_reg_t *pwm;     /* PWM controller */
    uint32_t pwmbase;
    dma_context_t dma;
    uint32_t sample_rate_min;
    uint32_t sample_rate_max;
    uint32_t sample_rate;        /* sample rate in Hz */
    uint32_t sample_prec;
    uint32_t clk_mash;
    uint32_t pwm_msen;
};

#endif /* __PWM_H__ */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pwm/pwm.h $ $Rev: 924476 $")
#endif
