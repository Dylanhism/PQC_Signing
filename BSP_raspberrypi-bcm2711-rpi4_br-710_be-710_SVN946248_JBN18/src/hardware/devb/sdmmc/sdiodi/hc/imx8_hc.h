/*
 * $QNXLicenseC:
 * Copyright 2013, QNX Software Systems.
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2017, 2019-2021 NXP
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

#ifndef IMX_HC_H_
#define IMX_HC_H_

#include <internal.h>

/**
 * Host controller interface
 *
 * @file       hc/imx8_hc.h
 * @addtogroup sdmmc_hc
 * @{
 */

/* A0 Workaround detection code enums - will be removed in production code. */
enum imx_chip_type_list {
    IMX_CHIP_TYPE_QUAD_MAX = 0x01,
    IMX_CHIP_TYPE_QUAD_X_PLUS = 0x02,
};
enum imx_chip_rev_list {
    IMX_CHIP_REV_A = 0x00,
    IMX_CHIP_REV_B = 0x01,
};

/** USDHC peripheral input clock */
#define IMX_USDHC_CLOCK_DEFAULT         198000000

#ifndef IMX_USDHC_SIZE
    #define IMX_USDHC_SIZE              0x10000
#endif

/** USDHC driver common defines */
#define IMX_USDHC_TUNING_RETRIES        40
#define IMX_USDHC_TUNING_TIMEOUT        150
#define IMX_USDHC_CLOCK_TIMEOUT         10000
#define IMX_USDHC_COMMAND_TIMEOUT       1000000  /* The card could take very long time to process the transfered data */
#define IMX_USDHC_TRANSFER_TIMEOUT      1000000

/** 32 bit ADMA descriptor definition */
typedef struct _imx_usdhc_adma32_t {
    uint16_t    attr;
    uint16_t    len;
    uint32_t    addr;
} imx_usdhc_adma32_t;

/** ADMA2 defines */
#define IMX_USDHC_ADMA2_MAX_XFER        (1024 * 60)
#define IMX_USDHC_ADMA2_VALID           (1 << 0)        /* Valid */
#define IMX_USDHC_ADMA2_END             (1 << 1)        /* End of descriptor, transfer complete interrupt will be generated */
#define IMX_USDHC_ADMA2_INT             (1 << 2)        /* Generate DMA interrupt, will not be used */
#define IMX_USDHC_ADMA2_NOP             (0 << 4)        /* No OP, go to the next descriptor */
#define IMX_USDHC_ADMA2_SET             (1 << 4)        /* No OP, go to the next descriptor */
#define IMX_USDHC_ADMA2_TRAN            (2 << 4)        /* Transfer data */
#define IMX_USDHC_ADMA2_LINK            (3 << 4)        /* Link to another descriptor */
#define ADMA_DESC_MAX                   256

/* Defines for flags field from 'Processor specific structure' */
#define SF_USE_SDMA     0x01
#define SF_USE_ADMA     0x02
#define SF_TUNE_SDR50   0x04
#define SF_SDMA_ACTIVE  0x10

/* Define for common initialization delay
 * Setting the lower delay will reduce driver startup
 * time. Side effects have been observed on certain SD
 * cards from the lower initialization delay,
 * so we recommend leaving the full delay in unless you
 * know you need a quicker startup. Ongoing (runtime) performance
 * is unaffected. */
#define FULL_DELAY 1
#ifdef FULL_DELAY
#define IMX_INIT_DELAY (delay(1))
#else
#define IMX_INIT_DELAY (nanospin_ns(100000L))
#endif

/** Processor specific structure */
typedef struct _imx_usdhcx_hc {
    void                *bshdl;
    uintptr_t           base;
    int                 fd;
    imx_usdhc_adma32_t* adma;
    uint32_t            admap;
    sdio_sge_t          sgl[ADMA_DESC_MAX];
    uint32_t            flags;
    int                 sdma_iid;       /**< SDMA interrupt id */
    uintptr_t           sdma_irq;
    uintptr_t           sdma_base;
    uint32_t            mix_ctrl;
    uint32_t            intmask;
    _Uint64t            usdhc_addr;     /**< Used to determine which controller it belongs to */
} imx_sdhcx_hc_t;

extern int imx_sdhcx_init(sdio_hc_t *hc);
extern int imx_sdhcx_dinit(sdio_hc_t *hc);

int imx_sdhcx_host_sdll(sdio_hc_t *hc, unsigned val);

/** @} */ /* End of sdmmc_hc */

#endif /* IMX_HC_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/sdmmc/sdiodi/hc/imx8_hc.h $ $Rev: 936554 $")
#endif
