/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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


#include <internal.h>

#ifdef SDIO_HC_SDHCI_DWC
#include <sdhci.h>

#ifdef  SDHCI_HCTL2_MODE_HS400
#undef  SDHCI_HCTL2_MODE_HS400
#endif
#define SDHCI_HCTL2_MODE_HS400      (0x7 << 0)

#ifdef  SDHCI_TUNING_RETRIES
#undef  SDHCI_TUNING_RETRIES
#endif
#define SDHCI_TUNING_RETRIES        140

// Vendor specific register
#define SDHCI_VENDOR_BASE_REG       0xE8

#define SDHCI_VENDER_EMMC_CTRL_REG  0x2C
 #define    SDHCI_IS_EMMC_CARD_MASK     (1 << 0)

#define SDHCI_VENDOR_AT_CTRL_REG    0x40
 #define    SDHCI_TUNE_SWIN_TH_VAL_MASK 0xFF
 #define    SDHCI_TUNE_SWIN_TH_VAL_LSB  24
 #define    SDHCI_TUNE_CLK_STOP_EN_MASK (1 << 16)

// DWC PHY registers
#define DWC_MSHC_PHY_REGS           0x300
#define DWC_MSHC_PHY_CNFG           (DWC_MSHC_PHY_REGS + 0x0)
 #define    PAD_SN_LSB                  20
 #define    PAD_SN_MASK                 0xF
 #define    PAD_SN_DEFAULT              ((0x8 & PAD_SN_MASK) << PAD_SN_LSB)
 #define    PAD_SP_LSB                  16
 #define    PAD_SP_MASK                 0xF
 #define    PAD_SP_DEFAULT              ((0x9 & PAD_SP_MASK) << PAD_SP_LSB)
 #define    PHY_PWRGOOD                 (1 << 1)
 #define    PHY_RSTN                    (1 << 0)

#define DWC_MSHC_CMDPAD_CNFG        (DWC_MSHC_PHY_REGS + 0x4)
#define DWC_MSHC_DATPAD_CNFG        (DWC_MSHC_PHY_REGS + 0x6)
#define DWC_MSHC_CLKPAD_CNFG        (DWC_MSHC_PHY_REGS + 0x8)
#define DWC_MSHC_STBPAD_CNFG        (DWC_MSHC_PHY_REGS + 0xA)
#define DWC_MSHC_RSTNPAD_CNFG       (DWC_MSHC_PHY_REGS + 0xC)
 #define    TXSLEW_N_LSB                9
 #define    TXSLEW_N_MASK               0xF
 #define    TXSLEW_P_LSB                5
 #define    TXSLEW_P_MASK               0xF
 #define    WEAKPULL_EN_LSB             3
 #define    WEAKPULL_EN_MASK            0x3
 #define    RXSEL_LSB                   0
 #define    RXSEL_MASK                  0x3

#define DWC_MSHC_COMMDL_CNFG        (DWC_MSHC_PHY_REGS + 0x1C)
#define DWC_MSHC_SDCLKDL_CNFG       (DWC_MSHC_PHY_REGS + 0x1D)
#define DWC_MSHC_SDCLKDL_DC         (DWC_MSHC_PHY_REGS + 0x1E)
#define DWC_MSHC_SMPLDL_CNFG        (DWC_MSHC_PHY_REGS + 0x20)
#define DWC_MSHC_ATDL_CNFG          (DWC_MSHC_PHY_REGS + 0x21)

#define DWC_MSHC_DLL_CTRL           (DWC_MSHC_PHY_REGS + 0x24)
#define DWC_MSHC_DLL_CNFG1          (DWC_MSHC_PHY_REGS + 0x25)
#define DWC_MSHC_DLL_CNFG2          (DWC_MSHC_PHY_REGS + 0x26)
#define DWC_MSHC_DLLDL_CNFG         (DWC_MSHC_PHY_REGS + 0x28)
#define DWC_MSHC_DLL_OFFSET         (DWC_MSHC_PHY_REGS + 0x29)
#define DWC_MSHC_DLLLBT_CNFG        (DWC_MSHC_PHY_REGS + 0x2C)
#define DWC_MSHC_DLL_STATUS         (DWC_MSHC_PHY_REGS + 0x2E)
 #define    ERROR_STS                   (1 << 17)
 #define    LOCK_STS                    (1 << 16)

#define DWC_MSHC_PHY_PAD_SD_CLK     \
    ((1 << TXSLEW_N_LSB) | (3 << TXSLEW_P_LSB) | (0 << WEAKPULL_EN_LSB) | (1 << RXSEL_LSB))
#define DWC_MSHC_PHY_PAD_SD_DAT     \
    ((1 << TXSLEW_N_LSB) | (3 << TXSLEW_P_LSB) | (1 << WEAKPULL_EN_LSB) | (1 << RXSEL_LSB))
#define DWC_MSHC_PHY_PAD_SD_STB     \
    ((1 << TXSLEW_N_LSB) | (3 << TXSLEW_P_LSB) | (2 << WEAKPULL_EN_LSB) | (1 << RXSEL_LSB))

#define DWC_MSHC_PHY_PAD_EMMC_CLK   \
    ((2 << TXSLEW_N_LSB) | (2 << TXSLEW_P_LSB) | (0 << WEAKPULL_EN_LSB) | (0 << RXSEL_LSB))
#define DWC_MSHC_PHY_PAD_EMMC_DAT   \
    ((2 << TXSLEW_N_LSB) | (2 << TXSLEW_P_LSB) | (1 << WEAKPULL_EN_LSB) | (1 << RXSEL_LSB))
#define DWC_MSHC_PHY_PAD_EMMC_STB   \
    ((2 << TXSLEW_N_LSB) | (2 << TXSLEW_P_LSB) | (2 << WEAKPULL_EN_LSB) | (1 << RXSEL_LSB))


typedef struct _sdhci_dwc {
    sdhci_hc_t  sdhci;
    uintptr_t   scr_base;
    int         (*set_clk)(struct _sdio_hc *, int, int);
    int         phy; //0: DWC_MSHC_LS_NO_PHY_MODE is set, skip PHY config; 1: DWC_MSHC_LS_NO_PHY_MODE is not set, do PHY config
} sdhci_dwc_t;

extern int sdhci_dwc_dinit( sdio_hc_t *hc );
extern int sdhci_dwc_init( sdio_hc_t *hc );

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/sdmmc/sdiodi/hc/sdhci_dwc.h $ $Rev: 945010 $")
#endif
