/*
 * $QNXLicenseC:
* Copyright 2017, 2020 QNX Software Systems.
* Copyright 2017, Renesas Electronics Corporation
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

#ifndef _RCAR_MMCIF_H_INCLUDED
#define _RCAR_MMCIF_H_INCLUDED

#include <internal.h>
#include <hw/dma.h>

/*
 * SDMMCIF Memory-mapped registers
 */

#define MMC_SD_CMD                  0x0000  //  16/32/64
#define MMC_SD_PORTSEL              0x0008  //  16/32/64
#define MMC_SD_ARG                  0x0010  //  16/32/64
#define MMC_SD_ARG1                 0x0018  //  16/32/64
#define MMC_SD_STOP                 0x0020  //  16/32/64
#define MMC_SD_SECCNT               0x0028  //  16/32/64
#define MMC_SD_RSP10                0x0030  //  16/32/64
#define MMC_SD_RSP1                 0x0038  //  16/32/64
#define MMC_SD_RSP32                0x0040  //  16/32/64
#define MMC_SD_RSP3                 0x0048  //  16/32/64
#define MMC_SD_RSP54                0x0050  //  16/32/64
#define MMC_SD_RSP5                 0x0058  //  16/32/64
#define MMC_SD_RSP76                0x0060  //  16/32/64
#define MMC_SD_RSP7                 0x0068  //  16/32/64
#define MMC_SD_INFO1                0x0070  //  16/32/64
#define MMC_SD_INFO2                0x0078  //  16/32/64
#define MMC_SD_INFO1_MASK           0x0080  //  16/32/64
#define MMC_SD_INFO2_MASK           0x0088  //  16/32/64
#define MMC_SD_CLK_CTRL             0x0090  //  16/32/64
#define MMC_SD_SIZE                 0x0098  //  16/32/64
#define MMC_SD_OPTION               0x00A0  //  16/32/64
#define MMC_SD_ERR_STS1             0x00B0  //  16/32/64
#define MMC_SD_ERR_STS2             0x00B8  //  16/32/64
#define MMC_SD_BUF0                 0x00C0  //  16/32/64
#define MMC_SDIO_MODE               0x00D0  //  16/32/64
#define MMC_SDIO_INFO1              0x00D8  //  16/32/64
#define MMC_SDIO_INFO1_MASK         0x00E0  //  16/32/64
#define MMC_CC_EXT_MODE             0x0360  //  16/32/64
#define MMC_SOFT_RST                0x0380  //  16/32/64
#define MMC_VERSION                 0x0388  //  16/32/64
#define MMC_HOST_MODE               0x0390  //  16/32/64
#define MMC_SDIF_MODE               0x0398  //  16/32/64
#define MMC_SD_STATUS               0x03C8  //  16/32/64
#define MMC_DM_CM_SEQ_MODE          0x0808  //  16/32/64
#define MMC_DM_CM_DTRAN_MODE        0x0820  //  16/32/64
#define MMC_DM_CM_DTRAN_CTRL        0x0828  //  16/32/64
#define MMC_DM_CM_RST               0x0830  //  16/32/64
#define MMC_DM_CM_INFO1             0x0840  //  16/32/64
#define MMC_DM_CM_INFO1_MASK        0x0848  //  16/32/64
#define MMC_DM_CM_INFO2             0x0850  //  16/32/64
#define MMC_DM_CM_INFO2_MASK        0x0858  //  16/32/64
#define MMC_DM_CM_DTRAN_ADDR        0x0880  //  16/32/64
#define MMC_SCC_DTCNTL              0x1000  //  32
#define MMC_SCC_TAPSET              0x1008  //  32
#define MMC_SCC_DT2FF               0x1010  //  32
#define MMC_SCC_CKSEL               0x1018  //  32
#define MMC_SCC_RVSCNTL             0x1020  //  32
#define MMC_SCC_RVSREQ              0x1028  //  32
#define MMC_SCC_SMPCMP	            0x1030  //  32
#define MMC_SCC_TMPPORT2            0x1038  //  32
#define MMC_SCC_TMPPORT3            0x1050  //  32
#define MMC_SCC_TMPPORT4            0x1058  //  32
#define MMC_SCC_TMPPORT5            0x1060  //  32
#define MMC_SCC_TMPPORT6            0x1068  //  32
#define MMC_SCC_TMPPORT7            0x1070  //  32

// Command register bits
#define SDH_CMD_AC12            (0 << 14)   // CMD12 is automatically issued
#define SDH_CMD_NOAC12          (1 << 14)
#define SDH_CMD_DAT_MULTI       (1 << 13)   // multi block transfer
#define SDH_CMD_DAT_READ        (1 << 12)   // read
#define SDH_CMD_ADTC            (1 << 11)   // command with data
#define SDH_CMD_NORSP           (3 <<  8)   // no response
#define SDH_CMD_RSPR1           (4 <<  8)   // R1, R5, R6, R7
#define SDH_CMD_RSPR1B          (5 <<  8)   // R1b
#define SDH_CMD_RSPR2           (6 <<  8)   // R2
#define SDH_CMD_RSPR3           (7 <<  8)   // R3, R4
#define SDH_CMD_ACMD            (6 <<  8)   // ACMD

// Stop register
#define SDH_STOP_STP            (1 <<  0)
#define SDH_STOP_SEC            (1 <<  8)

// CLK_CTRL register
#define SDH_CLKCTRL_SCLKEN      (1 <<  8)
#define SDH_CLKCTRL_SDCLKOFFEN  (1 <<  9)

// INFO1
#define SDH_INFO1_HPIRES        (1 << 16)
#define SDH_INFO1_WP            (1 <<  7)   // write protect
#define SDH_INFO1_CD            (1 <<  5)   // card detection state
#define SDH_INFO1_INST          (1 <<  4)   // card insertion
#define SDH_INFO1_RMVL          (1 <<  3)   // card removal
#define SDH_INFO1_AE            (1 <<  2)   // access end
#define SDH_INFO1_RE            (1 <<  0)   // response end
#define SDH_INFO1_CMD           (SDH_INFO1_RE | SDH_INFO1_INST | SDH_INFO1_RMVL)
#define SDH_INFO1_DAT           (SDH_INFO1_AE | SDH_INFO1_INST | SDH_INFO1_RMVL)

// INFO2
#define SDH_INFO2_ILA           (1 << 15)   // illegal access error
#define SDH_INFO2_CBSY          (1 << 14)   // command response busy
#define SDH_INFO2_SCLKDIVEN     (1 << 13)   // SD bus busy
#define SDH_INFO2_BMSK          ((1 << 10)|(1 << 11)|(1 << 12))   // mask Bits (Follow document ver.0027)
#define SDH_INFO2_BWE           (1 <<  9)   // SD_BUF Write Enable
#define SDH_INFO2_BRE           (1 <<  8)   // SD_BUF Read Enable
#define SDH_INFO2_DAT0          (1 <<  7)
#define SDH_INFO2_RTO           (1 <<  6)   // Response Timeout
#define SDH_INFO2_BIRA          (1 <<  5)   // SD_BUF Illegal Read Access
#define SDH_INFO2_BIWA          (1 <<  4)   // SD_BUF Illegal Write Access
#define SDH_INFO2_DTO           (1 <<  3)   // Timeout (except response timeout)
#define SDH_INFO2_ENDE          (1 <<  2)   // END Error
#define SDH_INFO2_CRCE          (1 <<  1)   // CRC Error
#define SDH_INFO2_CMDE          (1 <<  0)   // CMD Error
#define SDH_INFO2_ALL_ERR       (SDH_INFO2_CMDE | SDH_INFO2_CRCE | SDH_INFO2_ENDE | SDH_INFO2_DTO |    \
                                                  SDH_INFO2_BIWA | SDH_INFO2_BIRA | SDH_INFO2_RTO)

#define SDH_INFO2_CMD_ERR       (SDH_INFO2_CMDE | SDH_INFO2_CRCE | SDH_INFO2_ENDE | SDH_INFO2_RTO)
#define SDH_INFO2_DAT_ERR       (SDH_INFO2_ENDE | SDH_INFO2_CRCE | SDH_INFO2_DTO | SDH_INFO2_BIWA | SDH_INFO2_BIRA)

// INFO1_MASK
#define SDH_INFO1_MASK_ALL      0x0001FFFF  // all mask (Follow document ver.0027)

// INFO2_MASK
#define SDH_INFO2_MASK_ALL      0x0000FFFF  // all mask (Follow document ver.0027)

// SD Card Access Control Option Register (SD_OPTION) bit defination
#define SDH_OPTION_DISABLE_TO   (1 << 8)
#define SDH_OPTION_EXTOP        (1 << 9)
#define SDH_OPTION_WIDTH_1      (1 << 15)   // Data Bus Width 1 bit
#define SDH_OPTION_WIDTH_8      (1 << 13)   // Data Bus Width 8 bit

/* SOFT_RST */
#define SOFT_RST_ON             (0 << 0)
#define SOFT_RST_OFF            (1 << 0)

/* CC_EXT_MODE */
#define BUF_ACC_DMAWEN          (1 << 1)
#define CC_EXT_MODE_MSK         (0 << 0)    // Follow document ver.0027

/* MMC_SDIF_MODE */
#define SDIF_MODE_HS400         (1 << 0)

/* DM_CM_DTRAN_MODE */
#define CH_NUM_DOWNSTREAM       (0 << 16)
#define CH_NUM_UPSTREAM         (1 << 16)
#define BUS_WID_64BIT           (3 << 4)
#define FIXED_ADDRESS           (0 << 0)
#define INCREMENT_ADDRESS       (1 << 0)

/* DM_CM_DTRAN_CTRL */
#define DM_START                (1 << 0)

/* MMC_DM_CM_INFO2 & MMC_DM_CM_INFO2_MASK */
#define DM_INFO2_ALL_MSK        0xFFFFFFFF
#define DM_INFO2_DTRAN_ERR0     (1 << 16)
#define DM_INFO2_DTRAN_ERR1     (1 << 17)

/* MMC_DM_CM_INFO1 & MMC_DM_CM_INFO1_MASK */
#define DM_INFO1_ALL_MSK        0xFFFFFFFF
#define DM_INFO1_DTRAN_END0     (1 << 16)
#define DM_INFO1_DTRAN_END1(x)  (1 << (x))

/* MMC_DM_CM_RST */
#define DM_RST_REVBITS_MSK       0xFFFFFFFF
#define DM_RST_DTRANRST0         (1 << 8)
#define DM_RST_DTRANRST1         (1 << 9)

/* MMC_SCC_DTCNTL register */
#define MMC_SCC_DTCNTL_TAPEN      (1 << 0)

/* MMC_SCC_CKSEL register */
#define MMC_SCC_CKSEL_DTSEL       (1 << 0)

/* MMC_SCC_RVSCNTL register */
#define MMC_SCC_RVSCNTL_RVSEN     (1 << 0)

/* MMC_SCC_RVSREQ register */
#define MMC_SCC_RVSREQ_RVSERR     (1 << 2)
#define MMC_SCC_RVSREQ_REQTAPUP   (1 << 1)
#define MMC_SCC_RVSREQ_REQTAPDWN  (1 << 0)
#define MMC_SCC_RVSREQ_REQTAPMSK  (3 << 0)

/* MMC_SCC_SMPCMP register */
#define MMC_SCC_SMPCMP_CMD_ERR      ((1 << 24) | (1 << 8))
#define MMC_SCC_SMPCMP_CMD_REQUP     (1 << 24)
#define MMC_SCC_SMPCMP_CMD_REQDOWN   (1 << 8)

/* MMC_SCC_TMPPORT2 register */
#define MMC_SCC_TMPPORT2_HS400OSEL    (1 << 4)
#define MMC_SCC_TMPPORT2_HS400EN      (1 << 31)

/* MMC_SCC_TMPPORT3 register */
#define MMC_SCC_TMPPORT3_OFFSET_0   3
#define MMC_SCC_TMPPORT3_OFFSET_1   2
#define MMC_SCC_TMPPORT3_OFFSET_2   1
#define MMC_SCC_TMPPORT3_OFFSET_3   0

/* MMC_SCC_TMPPORT4 register */
#define MMC_SCC_TMPPORT4_DLL_ACC_START  (1 << 0)

/* MMC_SCC_TMPPORT5 register */
#define MMC_SCC_TMPPORT5_DLL_RW_SEL_R   (1 << 8)
#define MMC_SCC_TMPPORT5_DLL_RW_SEL_W   (0 << 8)
#define MMC_SCC_TMPPORT5_DLL_ADR_MASK   0x3F

/* MMC_SCC register */
#define MMC_SCC_TMPPORT_DISABLE_WP_CODE 0xa5000000
#define MMC_SCC_TMPPORT_CALIB_CODE_MASK 0x1f
#define MMC_SCC_TMPPORT_MANUAL_MODE     (1 << 7)
#define CALIB_TABLE_MAX	(MMC_SCC_TMPPORT_CALIB_CODE_MASK + 1)

#define RCAR_SDHI_MAX_TAP           3

/*
* According to the tuning specs, Tuning process
* is normally shorter 40 executions of CMD19,
* and timeout value should be shorter than 150 ms
*/
#define RCAR_SDHI_TUNING_TIMEOUT    150
#define RCAR_SDHI_TUNING_RETRIES    40

#define SDHI_TMOUT                  1000

#define DMA_DESC_MAX                256


typedef struct _rcar_sdmmc_t {
    void            *bshdl;
    paddr_t         pbase;
    uint32_t        basesize;
    uint32_t        pclk;
    int             blksz;
    int             busclk;       // MMC bus clock
    int             scc_min_clk;  // SCC min clock
    uintptr_t       vbase;
    uint32_t        flags;
#define OF_DMA_ACTIVE     1
#define OF_DMA_DONE       2
#define OF_DATA_DONE      4
    int             chan_idx;
    /* SCC */
    uint32_t        tap;
    uint32_t        tap_set;
    uint32_t        tap_num;
    int             tuning_status;
#define RCAR_TUNING_NONE     0
#define RCAR_TUNING_DOING    1
#define RCAR_TUNING_DONE     2
    int             dma_tranend1;
    sdio_cmd_t     *cmd;
    int             irq;
    int             dma_iid;
    int             dma_irq;
    struct sigevent dma_ev;
    int             cs;
    sdio_sge_t      sgl[DMA_DESC_MAX];
    char            taps[RCAR_SDHI_TUNING_RETRIES];
    char            smpcmp[RCAR_SDHI_TUNING_RETRIES];

    /* For HS400 mode */
    uintptr_t       vcpg_base;
    int             clkrate_offset;
    int             hs400_use_4tap;
    uint32_t        sdckcr_val_for_hs400;
    uint32_t        tap_hs400;

    /* WA: adjust eMMC HS400 offset for R-Car M3N, E3 */
    int             adjust_hs400_enable;
    uint8_t         adjust_hs400_offset;
    int             need_adjust_hs400;
    int             need_adjust_hs400_done;
    const uint8_t  *adjust_hs400_calib_table;
    /* SCC HS400 correction of sampling clock position */
    int             hs400_ignore_dat_correction;
    int             hs400_bad_tap;
    int             hs400_manual_correction;
} rcar_sdmmc_t;

extern int rcar_sdmmc_init(sdio_hc_t *hc);
extern int rcar_sdmmc_dinit(sdio_hc_t *hc);

static inline uint32_t sdmmc_read(uintptr_t base, int reg)
{
    return in32(base + reg);
}

static inline int rcar_sdmmc_check_sclkdiven(uintptr_t base)
{
    int i;

    for (i = 0; i < SDHI_TMOUT; i++) {
        if (sdmmc_read(base, MMC_SD_INFO2) & SDH_INFO2_SCLKDIVEN)
            break;
        nanospin_ns(1000);
    }

    if (i >= SDHI_TMOUT)
        return (EBUSY);

    return EOK;
}

static inline void sdmmc_write(uintptr_t base, int reg, uint32_t val)
{
    if ((reg == MMC_SD_CLK_CTRL) && rcar_sdmmc_check_sclkdiven(base))
        return;

    switch (reg) {
        case MMC_SD_INFO1_MASK:
            val &= 0x0001FFFF;
            break;
        case MMC_SD_INFO1:
        case MMC_SD_INFO2:
        case MMC_SD_INFO2_MASK:
            val &= 0x0000FFFF;
            break;
        case MMC_SCC_TAPSET:
            val &= 0x000000FF;
            break;
    }

    out32(base + reg, val);
}

static inline void sdmmc_bitset(uintptr_t base, int reg, uint32_t val)
{
    out32(base + reg, val | in32(base + reg));
}

static inline void sdmmc_bitclr(uintptr_t base, int reg, uint32_t val)
{
    out32(base + reg, ~val & in32(base + reg));
}

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/sdmmc/sdiodi/hc/rcar_gen3.h $ $Rev: 944140 $")
#endif
