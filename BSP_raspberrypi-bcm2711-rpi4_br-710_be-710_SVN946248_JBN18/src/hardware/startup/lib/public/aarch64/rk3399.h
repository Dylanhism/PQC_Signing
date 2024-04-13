/*
 * $QNXLicenseC:
 * Copyright 2019 QNX Software Systems.
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
 * Rockchip RK3399
 */

#ifndef __AARCH64_RK3399_H_INCLUDED
#define __AARCH64_RK3399_H_INCLUDED

#define RK3399_A72_NUM_CORES                      2
#define RK3399_A53_NUM_CORES                      4
#define RK3399_NUM_CORES                          (RK3399_A72_NUM_CORES + RK3399_A53_NUM_CORES)

/* Set the bits and the write mask */
#define SET_BITS_WM(_shift, _mask, _val)         ( ((_val) << (_shift)) | ((_mask) << ((_shift) + 16)) )

#define RK3399_WDT0_BASE                          0xFF848000
#define RK3399_WDT1_BASE                          0xFF840000
#define RK3399_WDT2_BASE                          0xFF380000
 #define RK3399_WDT_SIZE                          0x100
 #define WDT_CR                                   0x0
 #define WDT_TORR                                 0x4
 #define WDT_CCVR                                 0x8
 #define WDT_CRR                                  0xC
    #define WDT_CRR_RESTART_VALUE                 0x76
 #define WDT_STAT                                 0x10
 #define WDT_EOI                                  0x14

/* CRU */
#define RK3399_CRU_BASE                           0xFF760000
 #define CRU_PLL_CON(pll, idx)                    (CRU_LPLL_CON0    + ((idx) * 4) + (pll) * 0x20)
 #define CRU_CLKSEL_CON(idx)                      (CRU_CLKSEL_CON0  + ((idx) * 4))
 #define CRU_CLKGATE_CON(idx)                     (CRU_CLKGATE_CON0 + ((idx) * 4))
 #define CRU_SOFTRST_CON(idx)                     (CRU_SOFTRST_CON0 + ((idx) * 4))

 #define CRU_LPLL_CON0                            0x0000
 #define CRU_LPLL_CON1                            0x0004
 #define CRU_LPLL_CON2                            0x0008
 #define CRU_LPLL_CON3                            0x000C
 #define CRU_LPLL_CON4                            0x0010
 #define CRU_LPLL_CON5                            0x0014
 #define CRU_BPLL_CON0                            0x0020
 #define CRU_BPLL_CON1                            0x0024
 #define CRU_BPLL_CON2                            0x0028
 #define CRU_BPLL_CON3                            0x002C
 #define CRU_BPLL_CON4                            0x0030
 #define CRU_BPLL_CON5                            0x0034
 #define CRU_DPLL_CON0                            0x0040
 #define CRU_DPLL_CON1                            0x0044
 #define CRU_DPLL_CON2                            0x0048
 #define CRU_DPLL_CON3                            0x004C
 #define CRU_DPLL_CON4                            0x0050
 #define CRU_DPLL_CON5                            0x0054
 #define CRU_CPLL_CON0                            0x0060
 #define CRU_CPLL_CON1                            0x0064
 #define CRU_CPLL_CON2                            0x0068
 #define CRU_CPLL_CON3                            0x006C
 #define CRU_CPLL_CON4                            0x0070
 #define CRU_CPLL_CON5                            0x0074
 #define CRU_GPLL_CON0                            0x0080
 #define CRU_GPLL_CON1                            0x0084
 #define CRU_GPLL_CON2                            0x0088
 #define CRU_GPLL_CON3                            0x008C
 #define CRU_GPLL_CON4                            0x0090
 #define CRU_GPLL_CON5                            0x0094
 #define CRU_NPLL_CON0                            0x00A0
 #define CRU_NPLL_CON1                            0x00A4
 #define CRU_NPLL_CON2                            0x00A8
 #define CRU_NPLL_CON3                            0x00AC
 #define CRU_NPLL_CON4                            0x00B0
 #define CRU_NPLL_CON5                            0x00B4
 #define CRU_VPLL_CON0                            0x00C0
 #define CRU_VPLL_CON1                            0x00C4
 #define CRU_VPLL_CON2                            0x00C8
 #define CRU_VPLL_CON3                            0x00CC
 #define CRU_VPLL_CON4                            0x00D0
 #define CRU_VPLL_CON5                            0x00D4
    #define CRU_PLL_CON0_FBDIV_MASK               0x3ff
    #define CRU_PLL_CON1_REFDIV_MASK              0x03f
    #define CRU_PLL_CON1_POSTDIV1_MASK            0x007
    #define CRU_PLL_CON1_POSTDIV1_SHIFT           0x008
    #define CRU_PLL_CON1_POSTDIV2_MASK            0x007
    #define CRU_PLL_CON1_POSTDIV2_SHIFT           0x00c
    #define CRU_PLL_CON2_FRACDIV_MASK             0x00FFFFFF
    #define CRU_PLL_CON2_FRACDIV_SHIFT            0x0
    #define CRU_PLL_CON2_PLL_LOCKED               (1 << 31)
    #define PLL_CON3_PWRDN                        (1 << 0)
    #define PLL_CON3_BYPASS                       (1 << 1)
    #define PLL_CON3_DACPD                        (1 << 2)
    #define PLL_CON3_DSMPD                        (1 << 3)
    #define PLL_CON3_FOUTPOSTDIVPD                (1 << 4)
    #define PLL_CON3_FOUTVCOPD                    (1 << 5)
    #define PLL_CON3_FOUT4PHASEPD                 (1 << 6)
    #define PLL_CON3_MODE_SHIFT                   0x8
    #define PLL_CON3_MODE_MASK                    0x3
    #define PLL_CON3_MODE_SLOW                    0x0
    #define PLL_CON3_MODE_NORMAL                  0x1
    #define PLL_CON3_MODE_DEEP_SLOW               0x2
 #define CRU_CLKSEL_CON0                          0x0100
 #define CRU_CLKSEL_CON1                          0x0104
 #define CRU_CLKSEL_CON2                          0x0108
 #define CRU_CLKSEL_CON3                          0x010C
 #define CRU_CLKSEL_CON4                          0x0110
 #define CRU_CLKSEL_CON5                          0x0114
 #define CRU_CLKSEL_CON6                          0x0118
 #define CRU_CLKSEL_CON7                          0x011C
 #define CRU_CLKSEL_CON8                          0x0120
 #define CRU_CLKSEL_CON9                          0x0124
 #define CRU_CLKSEL_CON10                         0x0128
 #define CRU_CLKSEL_CON11                         0x012C
 #define CRU_CLKSEL_CON12                         0x0130
 #define CRU_CLKSEL_CON13                         0x0134
 #define CRU_CLKSEL_CON14                         0x0138
 #define CRU_CLKSEL_CON15                         0x013C
 #define CRU_CLKSEL_CON16                         0x0140
  /* CRU_CLKSEL_CON15, CON16, CON22 */
  #define CLKSEL_SDMMC_DIV(_div)                  SET_BITS_WM(0, 0x7F, (_div))
  #define CLKSEL_SDMMC_PLLSEL(_clk)               SET_BITS_WM(8, 0x7 , (_clk))
 #define CRU_CLKSEL_CON17                         0x0144
 #define CRU_CLKSEL_CON18                         0x0148
 #define CRU_CLKSEL_CON19                         0x014C
 #define CRU_CLKSEL_CON20                         0x0150
 #define CRU_CLKSEL_CON21                         0x0154
 #define CRU_CLKSEL_CON22                         0x0158
 #define CRU_CLKSEL_CON23                         0x015C
 #define CRU_CLKSEL_CON24                         0x0160
 #define CRU_CLKSEL_CON25                         0x0164
 #define CRU_CLKSEL_CON26                         0x0168
 #define CRU_CLKSEL_CON27                         0x016C
 #define CRU_CLKSEL_CON28                         0x0170
 #define CRU_CLKSEL_CON29                         0x0174
 #define CRU_CLKSEL_CON30                         0x0178
 #define CRU_CLKSEL_CON31                         0x017C
 #define CRU_CLKSEL_CON32                         0x0180
 #define CRU_CLKSEL_CON33                         0x0184
 #define CRU_CLKSEL_CON34                         0x0188
 #define CRU_CLKSEL_CON35                         0x018C
 #define CRU_CLKSEL_CON36                         0x0190
 #define CRU_CLKSEL_CON38                         0x0198
 #define CRU_CLKSEL_CON39                         0x019C
 #define CRU_CLKSEL_CON40                         0x01A0
 #define CRU_CLKSEL_CON41                         0x01A4
 #define CRU_CLKSEL_CON42                         0x01A8
 #define CRU_CLKSEL_CON43                         0x01AC
 #define CRU_CLKSEL_CON44                         0x01B0
 #define CRU_CLKSEL_CON45                         0x01B4
 #define CRU_CLKSEL_CON46                         0x01B8
 #define CRU_CLKSEL_CON47                         0x01BC
 #define CRU_CLKSEL_CON48                         0x01C0
 #define CRU_CLKSEL_CON49                         0x01C4
 #define CRU_CLKSEL_CON50                         0x01C8
 #define CRU_CLKSEL_CON51                         0x01CC
 #define CRU_CLKSEL_CON52                         0x01D0
 #define CRU_CLKSEL_CON53                         0x01D4
 #define CRU_CLKSEL_CON54                         0x01D8
 #define CRU_CLKSEL_CON55                         0x01DC
 #define CRU_CLKSEL_CON56                         0x01E0
 #define CRU_CLKSEL_CON57                         0x01E4
 #define CRU_CLKSEL_CON58                         0x01E8
 #define CRU_CLKSEL_CON59                         0x01EC
 #define CRU_CLKSEL_CON60                         0x01F0
 #define CRU_CLKSEL_CON61                         0x01F4
 #define CRU_CLKSEL_CON62                         0x01F8
 #define CRU_CLKSEL_CON63                         0x01FC
 #define CRU_CLKSEL_CON64                         0x0200
 #define CRU_CLKSEL_CON65                         0x0204
 #define CRU_CLKSEL_CON96                         0x0280
 #define CRU_CLKSEL_CON97                         0x0284
 #define CRU_CLKSEL_CON98                         0x0288
 #define CRU_CLKSEL_CON99                         0x028C
 #define CRU_CLKSEL_CON100                        0x0290
 #define CRU_CLKSEL_CON101                        0x0294
 #define CRU_CLKSEL_CON102                        0x0298
 #define CRU_CLKSEL_CON103                        0x029C
 #define CRU_CLKSEL_CON105                        0x02A4
 #define CRU_CLKSEL_CON106                        0x02A8
 #define CRU_CLKSEL_CON107                        0x02AC
    #define CRU_CLKSEL_PLL_SEL_SHIFT              0x6
    #define CRU_CLKSEL_PLL_SEL_MASK               0x3
    #define CRU_CLKSEL_DIV_CON_SHIFT              0x0
    #define CRU_CLKSEL_DIV_CON_MASK               0x1F
 #define CRU_CLKGATE_CON0                         0x0300
 #define CRU_CLKGATE_CON1                         0x0304
 #define CRU_CLKGATE_CON2                         0x0308
 #define CRU_CLKGATE_CON3                         0x030C
 #define CRU_CLKGATE_CON4                         0x0310
 #define CRU_CLKGATE_CON5                         0x0314
 #define CRU_CLKGATE_CON6                         0x0318
 #define CRU_CLKGATE_CON7                         0x031C
 #define CRU_CLKGATE_CON8                         0x0320
 #define CRU_CLKGATE_CON9                         0x0324
 #define CRU_CLKGATE_CON10                        0x0328
 #define CRU_CLKGATE_CON11                        0x032C
 #define CRU_CLKGATE_CON12                        0x0330
 #define CRU_CLKGATE_CON13                        0x0334
 #define CRU_CLKGATE_CON14                        0x0338
 #define CRU_CLKGATE_CON15                        0x033C
 #define CRU_CLKGATE_CON16                        0x0340
 #define CRU_CLKGATE_CON17                        0x0344
 #define CRU_CLKGATE_CON18                        0x0348
 #define CRU_CLKGATE_CON19                        0x034C
 #define CRU_CLKGATE_CON20                        0x0350
 #define CRU_CLKGATE_CON21                        0x0354
 #define CRU_CLKGATE_CON22                        0x0358
 #define CRU_CLKGATE_CON23                        0x035C
 #define CRU_CLKGATE_CON24                        0x0360
 #define CRU_CLKGATE_CON25                        0x0364
 #define CRU_CLKGATE_CON26                        0x0368
 #define CRU_CLKGATE_CON27                        0x036C
 #define CRU_CLKGATE_CON28                        0x0370
 #define CRU_CLKGATE_CON29                        0x0374
 #define CRU_CLKGATE_CON30                        0x0378
 #define CRU_CLKGATE_CON31                        0x037C
 #define CRU_CLKGATE_CON32                        0x0380
 #define CRU_CLKGATE_CON33                        0x0384
 #define CRU_CLKGATE_CON34                        0x0388
 #define CRU_SOFTRST_CON0                         0x0400
 #define CRU_SOFTRST_CON1                         0x0404
 #define CRU_SOFTRST_CON2                         0x0408
 #define CRU_SOFTRST_CON3                         0x040C
 #define CRU_SOFTRST_CON4                         0x0410
 #define CRU_SOFTRST_CON5                         0x0414
 #define CRU_SOFTRST_CON6                         0x0418
 #define CRU_SOFTRST_CON7                         0x041C
 #define CRU_SOFTRST_CON8                         0x0420
 #define CRU_SOFTRST_CON9                         0x0424
 #define CRU_SOFTRST_CON10                        0x0428
 #define CRU_SOFTRST_CON11                        0x042C
 #define CRU_SOFTRST_CON12                        0x0430
 #define CRU_SOFTRST_CON13                        0x0434
 #define CRU_SOFTRST_CON14                        0x0438
 #define CRU_SOFTRST_CON15                        0x043C
 #define CRU_SOFTRST_CON16                        0x0440
 #define CRU_SOFTRST_CON17                        0x0444
 #define CRU_SOFTRST_CON18                        0x0448
 #define CRU_SOFTRST_CON19                        0x044C
 #define CRU_SOFTRST_CON20                        0x0450
 #define CRU_GLB_SRST_FST                         0x0500
    #define CRU_GLB_SRST_FST_VALUE                0xFDB9
 #define CRU_GLB_SRST_SND                         0x0504
    #define CRU_GLB_SRST_SND_VALUE                0xECA8
 #define CRU_GLB_CNT_TH                           0x0508
 #define CRU_MISC_CON                             0x050C
 #define CRU_GLB_RST_CON                          0x0510
 #define CRU_GLB_RST_ST                           0x0514
 #define CRU_SDMMC_CON0                           0x0580
 #define CRU_SDMMC_CON1                           0x0584
 #define CRU_SDIO0_CON0                           0x0588
 #define CRU_SDIO0_CON1                           0x058C

/* PMUCRU */
#define RK3399_PMUCRU_BASE                        0xFF750000
 #define PMUCRU_PPLL_CON0                         0x0000
 #define PMUCRU_PPLL_CON1                         0x0004
 #define PMUCRU_PPLL_CON2                         0x0008
 #define PMUCRU_PPLL_CON3                         0x000C
 #define PMUCRU_PPLL_CON4                         0x0010
 #define PMUCRU_PPLL_CON5                         0x0014
 #define PMUCRU_CLKSEL_CON0                       0x0080
 #define PMUCRU_CLKSEL_CON1                       0x0084
 #define PMUCRU_CLKSEL_CON2                       0x0088
 #define PMUCRU_CLKSEL_CON3                       0x008C
 #define PMUCRU_CLKSEL_CON4                       0x0090
 #define PMUCRU_CLKSEL_CON5                       0x0094
 #define PMUCRU_CLKFRAC_CON0                      0x0098
 #define PMUCRU_CLKFRAC_CON1                      0x009C
 #define PMUCRU_CLKGATE_CON0                      0x0100
 #define PMUCRU_CLKGATE_CON1                      0x0104
 #define PMUCRU_CLKGATE_CON2                      0x0108
 #define PMUCRU_SOFTRST_CON0                      0x0110
 #define PMUCRU_SOFTRST_CON1                      0x0114
 #define PMUCRU_RSTNHOLD_CON0                     0x0120
 #define PMUCRU_RSTNHOLD_CON1                     0x0124
 #define PMUCRU_GATEDIS_CON0                      0x0130

/* GRF */
#define RK3399_GRF_BASE                           0xFF770000
 #define GRF_USB3_PERF_CON0                       0x02000
 #define GRF_USB3_PERF_CON1                       0x02004
 #define GRF_USB3_PERF_CON2                       0x02008
 #define GRF_USB3_PERF_RD_MAX_LATENCY_NUM         0x0200C
 #define GRF_USB3_PERF_RD_LATENCY_SAMP_NUM        0x02010
 #define GRF_USB3_PERF_RD_LATENCY_ACC_NUM         0x02014
 #define GRF_USB3_PERF_RD_AXI_TOTAL_BYTE          0x02018
 #define GRF_USB3_PERF_WR_AXI_TOTAL_BYTE          0x0201C
 #define GRF_USB3_PERF_WORKING_CNT                0x02020
 #define GRF_USB3OTG0_CON0                        0x02430
 #define GRF_USB3OTG0_CON1                        0x02434
 #define GRF_USB3OTG1_CON0                        0x02440
 #define GRF_USB3OTG1_CON1                        0x02444
 #define GRF_USB3OTG0_STATUS_LAT0                 0x02450
 #define GRF_USB3OTG0_STATUS_LAT1                 0x02454
 #define GRF_USB3OTG0_STATUS_CB                   0x02458
 #define GRF_USB3OTG1_STATUS_LAT0                 0x02460
 #define GRF_USB3OTG1_STATUS_LAT1                 0x02464
 #define GRF_USB3OTG1_STATUS_CB                   0x02468
 #define GRF_PCIE_PERF_CON0                       0x04000
 #define GRF_PCIE_PERF_CON1                       0x04004
 #define GRF_PCIE_PERF_CON2                       0x04008
 #define GRF_PCIE_PERF_RD_MAX_LATENCY_NUM         0x0400C
 #define GRF_PCIE_PERF_RD_LATENCY_SAMP_NUM        0x04010
 #define GRF_PCIE_PERF_RD_LATENCY_ACC_NUM         0x04014
 #define GRF_PCIE_PERF_RD_AXI_TOTAL_BYTE          0x04018
 #define GRF_PCIE_PERF_WR_AXI_TOTAL_BYTE          0x0401C
 #define GRF_PCIE_PERF_WORKING_CNT                0x04020
 #define GRF_USB20_HOST0_CON0                     0x04100
 #define GRF_USB20_HOST0_CON1                     0x04104
 #define GRF_USB20_HOST1_CON0                     0x04110
 #define GRF_USB20_HOST1_CON1                     0x04114
 #define GRF_HSIC_CON0                            0x04120
 #define GRF_HSIC_CON1                            0x04124
 #define GRF_GRF_USBHOST0_STATUS                  0x04140
 #define GRF_GRF_USBHOST1_STATUS                  0x04144
 #define GRF_GRF_HSIC_STATUS                      0x04148
 #define GRF_HSICPHY_CON0                         0x04470
 #define GRF_USBPHY0_CTRL0                        0x04480
 #define GRF_USBPHY0_CTRL1                        0x04484
 #define GRF_USBPHY0_CTRL2                        0x04488
 #define GRF_USBPHY0_CTRL3                        0x0448C
 #define GRF_USBPHY0_CTRL4                        0x04490
 #define GRF_USBPHY0_CTRL5                        0x04494
 #define GRF_USBPHY0_CTRL6                        0x04498
 #define GRF_USBPHY0_CTRL7                        0x0449C
 #define GRF_USBPHY0_CTRL8                        0x044A0
 #define GRF_USBPHY0_CTRL9                        0x044A4
 #define GRF_USBPHY0_CTRL10                       0x044A8
 #define GRF_USBPHY0_CTRL11                       0x044AC
 #define GRF_USBPHY0_CTRL12                       0x044B0
 #define GRF_USBPHY0_CTRL13                       0x044B4
 #define GRF_USBPHY0_CTRL14                       0x044B8
 #define GRF_USBPHY0_CTRL15                       0x044BC
 #define GRF_USBPHY0_CTRL16                       0x044C0
 #define GRF_USBPHY0_CTRL17                       0x044C4
 #define GRF_USBPHY0_CTRL18                       0x044C8
 #define GRF_USBPHY0_CTRL19                       0x044CC
 #define GRF_USBPHY0_CTRL20                       0x044D0
 #define GRF_USBPHY0_CTRL21                       0x044D4
 #define GRF_USBPHY0_CTRL22                       0x044D8
 #define GRF_USBPHY0_CTRL23                       0x044DC
 #define GRF_USBPHY0_CTRL24                       0x044E0
 #define GRF_USBPHY0_CTRL25                       0x044E4
 #define GRF_USBPHY1_CTRL0                        0x04500
 #define GRF_USBPHY1_CTRL1                        0x04504
 #define GRF_USBPHY1_CTRL2                        0x04508
 #define GRF_USBPHY1_CTRL3                        0x0450C
 #define GRF_USBPHY1_CTRL4                        0x04510
 #define GRF_USBPHY1_CTRL5                        0x04514
 #define GRF_USBPHY1_CTRL6                        0x04518
 #define GRF_USBPHY1_CTRL7                        0x0451C
 #define GRF_USBPHY1_CTRL8                        0x04520
 #define GRF_USBPHY1_CTRL9                        0x04524
 #define GRF_USBPHY1_CTRL10                       0x04528
 #define GRF_USBPHY1_CTRL11                       0x0452C
 #define GRF_USBPHY1_CTRL12                       0x04530
 #define GRF_USBPHY1_CTRL13                       0x04534
 #define GRF_USBPHY1_CTRL14                       0x04538
 #define GRF_USBPHY1_CTRL15                       0x0453C
 #define GRF_USBPHY1_CTRL16                       0x04540
 #define GRF_USBPHY1_CTRL17                       0x04544
 #define GRF_USBPHY1_CTRL18                       0x04548
 #define GRF_USBPHY1_CTRL19                       0x0454C
 #define GRF_USBPHY1_CTRL20                       0x04550
 #define GRF_USBPHY1_CTRL21                       0x04554
 #define GRF_USBPHY1_CTRL22                       0x04558
 #define GRF_USBPHY1_CTRL23                       0x0455C
 #define GRF_USBPHY1_CTRL24                       0x04560
 #define GRF_USBPHY1_CTRL25                       0x04564
 #define GRF_HDCP22_PERF_CON0                     0x06000
 #define GRF_HDCP22_PERF_CON1                     0x06004
 #define GRF_HDCP22_PERF_CON2                     0x06008
 #define GRF_HDCP22_PERF_RD_MAX_LATENCY_NUM       0x0600C
 #define GRF_HDCP22_PERF_RD_LATENCY_SAMP_NUM      0x06010
 #define GRF_HDCP22_PERF_RD_LATENCY_ACC_NUM       0x06014
 #define GRF_HDCP22_PERF_RD_AXI_TOTAL_BYTE        0x06018
 #define GRF_HDCP22_PERF_WR_AXI_TOTAL_BYTE        0x0601C
 #define GRF_HDCP22_PERF_WORKING_CNT              0x06020
 #define GRF_SOC_CON9                             0x06224
 #define GRF_SOC_CON20                            0x06250
 #define GRF_SOC_CON21                            0x06254
 #define GRF_SOC_CON22                            0x06258
 #define GRF_SOC_CON23                            0x0625C
 #define GRF_SOC_CON24                            0x06260
 #define GRF_SOC_CON25                            0x06264
 #define GRF_SOC_CON26                            0x06268
 #define GRF_GPU_PERF_CON0                        0x08000
 #define GRF_GPU_PERF_CON1                        0x08004
 #define GRF_GPU_PERF_CON2                        0x08008
 #define GRF_GPU_PERF_RD_MAX_LATENCY_NUM          0x0800C
 #define GRF_GPU_PERF_RD_LATENCY_SAMP_NUM         0x08010
 #define GRF_GPU_PERF_RD_LATENCY_ACC_NUM          0x08014
 #define GRF_GPU_PERF_RD_AXI_TOTAL_BYTE           0x08018
 #define GRF_GPU_PERF_WR_AXI_TOTAL_BYTE           0x0801C
 #define GRF_GPU_PERF_WORKING_CNT                 0x08020
 #define GRF_CPU_CON0                             0x0A000
 #define GRF_CPU_CON1                             0x0A004
 #define GRF_CPU_CON2                             0x0A008
 #define GRF_CPU_CON3                             0x0A00C
 #define GRF_CPU_STATUS0                          0x0A080
 #define GRF_CPU_STATUS1                          0x0A084
 #define GRF_CPU_STATUS2                          0x0A088
 #define GRF_CPU_STATUS3                          0x0A08C
 #define GRF_CPU_STATUS4                          0x0A090
 #define GRF_CPU_STATUS5                          0x0A094
 #define GRF_A53_PERF_CON0                        0x0A100
 #define GRF_A53_PERF_CON1                        0x0A104
 #define GRF_A53_PERF_CON2                        0x0A108
 #define GRF_A53_PERF_CON3                        0x0A10C
 #define GRF_A53_PERF_RD_MON_ST                   0x0A110
 #define GRF_A53_PERF_RD_MON_END                  0x0A114
 #define GRF_A53_PERF_WR_MON_ST                   0x0A118
 #define GRF_A53_PERF_WR_MON_END                  0x0A11C
 #define GRF_A53_PERF_RD_MAX_LATENCY_NUM          0x0A120
 #define GRF_A53_PERF_RD_LATENCY_SAMP_NUM         0x0A124
 #define GRF_A53_PERF_RD_LATENCY_ACC_NUM          0x0A128
 #define GRF_A53_PERF_RD_AXI_TOTAL_BYTE           0x0A12C
 #define GRF_A53_PERF_WR_AXI_TOTAL_BYTE           0x0A130
 #define GRF_A53_PERF_WORKING_CNT                 0x0A134
 #define GRF_A53_PERF_INT_STATUS                  0x0A138
 #define GRF_A72_PERF_CON0                        0x0A200
 #define GRF_A72_PERF_CON1                        0x0A204
 #define GRF_A72_PERF_CON2                        0x0A208
 #define GRF_A72_PERF_CON3                        0x0A20C
 #define GRF_A72_PERF_RD_MON_ST                   0x0A210
 #define GRF_A72_PERF_RD_MON_END                  0x0A214
 #define GRF_A72_PERF_WR_MON_ST                   0x0A218
 #define GRF_A72_PERF_WR_MON_END                  0x0A21C
 #define GRF_A72_PERF_RD_MAX_LATENCY_NUM          0x0A220
 #define GRF_A72_PERF_RD_LATENCY_SAMP_NUM         0x0A224
 #define GRF_A72_PERF_RD_LATENCY_ACC_NUM          0x0A228
 #define GRF_A72_PERF_RD_AXI_TOTAL_BYTE           0x0A22C
 #define GRF_A72_PERF_WR_AXI_TOTAL_BYTE           0x0A230
 #define GRF_A72_PERF_WORKING_CNT                 0x0A234
 #define GRF_A72_PERF_INT_STATUS                  0x0A238
 #define GRF_GMAC_PERF_CON0                       0x0C000
 #define GRF_GMAC_PERF_CON1                       0x0C004
 #define GRF_GMAC_PERF_CON2                       0x0C008
 #define GRF_GMAC_PERF_RD_MAX_LATENCY_NUM         0x0C00C
 #define GRF_GMAC_PERF_RD_LATENCY_SAMP_NUM        0x0C010
 #define GRF_GMAC_PERF_RD_LATENCY_ACC_NUM         0x0C014
 #define GRF_GMAC_PERF_RD_AXI_TOTAL_BYTE          0x0C018
 #define GRF_GMAC_PERF_WR_AXI_TOTAL_BYTE          0x0C01C
 #define GRF_GMAC_PERF_WORKING_CNT                0x0C020
 #define GRF_SOC_CON5                             0x0C214
  #define CON5_RMII_CLK_SEL_2_5MHZ                SET_BITS_WM(3, 0x1, 0)
  #define CON5_RMII_CLK_SEL_25MHZ                 SET_BITS_WM(3, 0x1, 1)
  #define CON5_GMAC_CLK_SEL_125MHZ                SET_BITS_WM(4, 0x3, 0)
  #define CON5_GMAC_CLK_SEL_25MHZ                 SET_BITS_WM(4, 0x3, 3)
  #define CON5_GMAC_CLK_SEL_2_5MHZ                SET_BITS_WM(4, 0x3, 2)
  #define CON5_RMII_MODE_MII                      SET_BITS_WM(6, 0x1, 0)
  #define CON5_RMII_MODE_RMII                     SET_BITS_WM(6, 0x1, 1)
  #define CON5_GMAC_SPEED_10MBPS                  SET_BITS_WM(7, 0x1, 0)
  #define CON5_GMAC_SPEED_100MBPS                 SET_BITS_WM(7, 0x1, 1)
  #define CON5_GMAC_FLOWCTRL_ON                   SET_BITS_WM(8, 0x1, 0)
  #define CON5_GMAC_FLOWCTRL_OFF                  SET_BITS_WM(8, 0x1, 1)
  #define CON5_GMAC_PHY_SEL_RMII                  SET_BITS_WM(9, 0x7, 4)
  #define CON5_GMAC_PHY_SEL_RGMII                 SET_BITS_WM(9, 0x7, 1)
 #define GRF_SOC_CON6                             0x0C218
 #define GRF_GPIO2A_IOMUX                         0x0E000
 #define GRF_GPIO2B_IOMUX                         0x0E004
 #define GRF_GPIO2C_IOMUX                         0x0E008
 #define GRF_GPIO2D_IOMUX                         0x0E00C
 #define GRF_GPIO3A_IOMUX                         0x0E010
 #define GRF_GPIO3B_IOMUX                         0x0E014
 #define GRF_GPIO3C_IOMUX                         0x0E018
 #define GRF_GPIO3D_IOMUX                         0x0E01C
 #define GRF_GPIO4A_IOMUX                         0x0E020
 #define GRF_GPIO4B_IOMUX                         0x0E024
 #define GRF_GPIO4C_IOMUX                         0x0E028
 #define GRF_GPIO4D_IOMUX                         0x0E02C
 #define GRF_GPIO2A_P                             0x0E040
 #define GRF_GPIO2B_P                             0x0E044
 #define GRF_GPIO2C_P                             0x0E048
 #define GRF_GPIO2D_P                             0x0E04C
 #define GRF_GPIO3A_P                             0x0E050
 #define GRF_GPIO3B_P                             0x0E054
 #define GRF_GPIO3C_P                             0x0E058
 #define GRF_GPIO3D_P                             0x0E05C
 #define GRF_GPIO4A_P                             0x0E060
 #define GRF_GPIO4B_P                             0x0E064
 #define GRF_GPIO4C_P                             0x0E068
 #define GRF_GPIO4D_P                             0x0E06C
 #define GRF_GPIO2A_SR                            0x0E080
 #define GRF_GPIO2B_SR                            0x0E084
 #define GRF_GPIO2C_SR                            0x0E088
 #define GRF_GPIO2D_SR                            0x0E08C
 #define GRF_GPIO3D_SR                            0x0E09C
 #define GRF_GPIO4A_SR                            0x0E0A0
 #define GRF_GPIO4B_SR                            0x0E0A4
 #define GRF_GPIO4C_SR                            0x0E0A8
 #define GRF_GPIO4D_SR                            0x0E0AC
 #define GRF_GPIO2A_SMT                           0x0E0C0
 #define GRF_GPIO2B_SMT                           0x0E0C4
 #define GRF_GPIO2C_SMT                           0x0E0C8
 #define GRF_GPIO2D_SMT                           0x0E0CC
 #define GRF_GPIO3A_SMT                           0x0E0D0
 #define GRF_GPIO3B_SMT                           0x0E0D4
 #define GRF_GPIO3C_SMT                           0x0E0D8
 #define GRF_GPIO3D_SMT                           0x0E0DC
 #define GRF_GPIO4A_SMT                           0x0E0E0
 #define GRF_GPIO4B_SMT                           0x0E0E4
 #define GRF_GPIO4C_SMT                           0x0E0E8
 #define GRF_GPIO4D_SMT                           0x0E0EC
 #define GRF_GPIO2A_E                             0x0E100
 #define GRF_GPIO2B_E                             0x0E104
 #define GRF_GPIO2C_E                             0x0E108
 #define GRF_GPIO2D_E                             0x0E10C
 #define GRF_GPIO3A_E01                           0x0E110
 #define GRF_GPIO3A_E2                            0x0E114
 #define GRF_GPIO3B_E01                           0x0E118
 #define GRF_GPIO3B_E2                            0x0E11C
 #define GRF_GPIO3C_E01                           0x0E120
 #define GRF_GPIO3C_E2                            0x0E124
 #define GRF_GPIO3D_E                             0x0E128
 #define GRF_GPIO4A_E                             0x0E12C
 #define GRF_GPIO4B_E01                           0x0E130
 #define GRF_GPIO4B_E2                            0x0E134
 #define GRF_GPIO4C_E                             0x0E138
 #define GRF_GPIO4D_E                             0x0E13C
 #define GRF_GPIO2C_HE                            0x0E188
 #define GRF_GPIO2D_HE                            0x0E18C
 #define GRF_SOC_CON0                             0x0E200
 #define GRF_SOC_CON1                             0x0E204
 #define GRF_SOC_CON2                             0x0E208
 #define GRF_SOC_CON3                             0x0E20C
 #define GRF_SOC_CON4                             0x0E210
 #define GRF_SOC_CON_5_PCIE                       0x0E214
 #define GRF_SOC_CON7                             0x0E21C
 #define GRF_SOC_CON8                             0x0E220
 #define GRF_SOC_CON_9_PCIE                       0x0E224
 #define GRF_SOC_STATUS0                          0x0E2A0
 #define GRF_SOC_STATUS1                          0x0E2A4
 #define GRF_SOC_STATUS2                          0x0E2A8
 #define GRF_SOC_STATUS3                          0x0E2AC
 #define GRF_SOC_STATUS4                          0x0E2B0
 #define GRF_SOC_STATUS5                          0x0E2B4
 #define GRF_DDRC0_CON0                           0x0E380
 #define GRF_DDRC0_CON1                           0x0E384
 #define GRF_DDRC1_CON0                           0x0E388
 #define GRF_DDRC1_CON1                           0x0E38C
 #define GRF_SIG_DETECT_CON0                      0x0E3C0
 #define GRF_SIG_DETECT_CON1                      0x0E3C8
 #define GRF_SIG_DETECT_CLR                       0x0E3D0
 #define GRF_SIG_DETECT_STATUS                    0x0E3E0
 #define GRF_USB20_PHY0_CON0                      0x0E450
 #define GRF_USB20_PHY0_CON1                      0x0E454
 #define GRF_USB20_PHY0_CON2                      0x0E458
 #define GRF_USB20_PHY0_CON3                      0x0E45C
 #define GRF_USB20_PHY1_CON0                      0x0E460
 #define GRF_USB20_PHY1_CON1                      0x0E464
 #define GRF_USB20_PHY1_CON2                      0x0E468
 #define GRF_USB20_PHY1_CON3                      0x0E46C
 #define GRF_USB3PHY0_CON0                        0x0E580
 #define GRF_USB3PHY0_CON1                        0x0E584
 #define GRF_USB3PHY0_CON2                        0x0E588
 #define GRF_USB3PHY1_CON0                        0x0E58C
 #define GRF_USB3PHY1_CON1                        0x0E590
 #define GRF_USB3PHY1_CON2                        0x0E594
 #define GRF_USB3PHY_STATUS0                      0x0E5C0
 #define GRF_USB3PHY_STATUS1                      0x0E5C4
 #define GRF_DLL_CON0                             0x0E600
 #define GRF_DLL_CON1                             0x0E604
 #define GRF_DLL_CON2                             0x0E608
 #define GRF_DLL_CON3                             0x0E60C
 #define GRF_DLL_CON4                             0x0E610
 #define GRF_DLL_CON5                             0x0E614
 #define GRF_DLL_STATUS0                          0x0E620
 #define GRF_DLL_STATUS1                          0x0E624
 #define GRF_DLL_STATUS2                          0x0E628
 #define GRF_DLL_STATUS3                          0x0E62C
 #define GRF_DLL_STATUS4                          0x0E630
 #define GRF_IO_VSEL                              0x0E640
 #define GRF_SARADC_TESTBIT                       0x0E644
 #define GRF_TSADC_TESTBIT_L                      0x0E648
 #define GRF_TSADC_TESTBIT_H                      0x0E64C
 #define GRF_CHIP_ID_ADDR                         0x0E800
 #define GRF_FAST_BOOT_ADDR                       0x0E880
 #define GRF_EMMCCORE_CON0                        0x0F000
 #define GRF_EMMCCORE_CON1                        0x0F004
 #define GRF_EMMCCORE_CON2                        0x0F008
 #define GRF_EMMCCORE_CON3                        0x0F00C
 #define GRF_EMMCCORE_CON4                        0x0F010
 #define GRF_EMMCCORE_CON5                        0x0F014
 #define GRF_EMMCCORE_CON6                        0x0F018
 #define GRF_EMMCCORE_CON7                        0x0F01C
 #define GRF_EMMCCORE_CON8                        0x0F020
 #define GRF_EMMCCORE_CON9                        0x0F024
 #define GRF_EMMCCORE_CON10                       0x0F028
 #define GRF_EMMCCORE_CON11                       0x0F02C
 #define GRF_EMMCCORE_STATUS0                     0x0F040
 #define GRF_EMMCCORE_STATUS1                     0x0F044
 #define GRF_EMMCCORE_STATUS2                     0x0F048
 #define GRF_EMMCCORE_STATUS3                     0x0F04C
 #define GRF_EMMCPHY_CON0                         0x0F780
 #define GRF_EMMCPHY_CON1                         0x0F784
 #define GRF_EMMCPHY_CON2                         0x0F788
 #define GRF_EMMCPHY_CON3                         0x0F78C
 #define GRF_EMMCPHY_CON4                         0x0F790
 #define GRF_EMMCPHY_CON5                         0x0F794
 #define GRF_EMMCPHY_CON6                         0x0F798
 #define GRF_EMMCPHY_STATUS                       0x0F7A0

/* PMUGRF */
#define RK3399_PMUGRF_BASE                        0xFF320000
 #define PMUGRF_GPIO0A_IOMUX                      0x00000
 #define PMUGRF_GPIO0B_IOMUX                      0x00004
 #define PMUGRF_GPIO1A_IOMUX                      0x00010
 #define PMUGRF_GPIO1B_IOMUX                      0x00014
 #define PMUGRF_GPIO1C_IOMUX                      0x00018
 #define PMUGRF_GPIO1D_IOMUX                      0x0001C
 #define PMUGRF_GPIO0A_P                          0x00040
 #define PMUGRF_GPIO0B_P                          0x00044
 #define PMUGRF_GPIO1A_P                          0x00050
 #define PMUGRF_GPIO1B_P                          0x00054
 #define PMUGRF_GPIO1C_P                          0x00058
 #define PMUGRF_GPIO1D_P                          0x0005C
 #define PMUGRF_GPIO0A_E                          0x00080
 #define PMUGRF_GPIO0B_E                          0x00088
 #define PMUGRF_GPIO1A_E                          0x000A0
 #define PMUGRF_GPIO1B_E                          0x000A8
 #define PMUGRF_GPIO1C_E                          0x000B0
 #define PMUGRF_GPIO1D_E                          0x000B8
 #define PMUGRF_GPIO0L_SR                         0x00100
 #define PMUGRF_GPIO1L_SR                         0x00108
 #define PMUGRF_GPIO1H_SR                         0x0010C
 #define PMUGRF_GPIO0A_SMT                        0x00120
 #define PMUGRF_GPIO0B_SMT                        0x00124
 #define PMUGRF_GPIO1A_SMT                        0x00130
 #define PMUGRF_GPIO1B_SMT                        0x00134
 #define PMUGRF_GPIO1C_SMT                        0x00138
 #define PMUGRF_GPIO1D_SMT                        0x0013C
 #define PMUGRF_GPIO0L_HE                         0x00160
 #define PMUGRF_SOC_CON0                          0x00180
 #define PMUGRF_SOC_CON10                         0x001A8
 #define PMUGRF_SOC_CON11                         0x001AC
 #define PMUGRF_PMUPVTM_CON0                      0x00240
 #define PMUGRF_PMUPVTM_CON1                      0x00244
 #define PMUGRF_PMUPVTM_STATUS0                   0x00248
 #define PMUGRF_PMUPVTM_STATUS1                   0x0024C
 #define PMUGRF_OSC_E                             0x00250
 #define PMUGRF_OS_REG0                           0x00300
 #define PMUGRF_OS_REG1                           0x00304
 #define PMUGRF_OS_REG2                           0x00308
 #define PMUGRF_OS_REG3                           0x0030C

/* PMU */
#define RK3399_PMU_BASE                           0xFF310000
 #define PMU_WAKEUP_CFG0                          0X0000
 #define PMU_WAKEUP_CFG1                          0X0004
 #define PMU_WAKEUP_CFG2                          0X0008
 #define PMU_WAKEUP_CFG3                          0X000C
 #define PMU_WAKEUP_CFG4                          0X0010
 #define PMU_PWRDN_CON                            0X0014
 #define PMU_PWRDN_ST                             0X0018
 #define PMU_PLL_CON                              0X001C
 #define PMU_PWRMODE_CON                          0X0020
 #define PMU_SFT_CON                              0X0024
 #define PMU_INT_CON                              0X0028
 #define PMU_INT_ST                               0X002C
 #define PMU_GPIO0_POS_INT_CON                    0X0030
 #define PMU_GPIO0_NEG_INT_CON                    0X0034
 #define PMU_GPIO1_POS_INT_CON                    0X0038
 #define PMU_GPIO1_NEG_INT_CON                    0X003C
 #define PMU_GPIO0_POS_INT_ST                     0X0040
 #define PMU_GPIO0_NEG_INT_ST                     0X0044
 #define PMU_GPIO1_POS_INT_ST                     0X0048
 #define PMU_GPIO1_NEG_INT_ST                     0X004C
 #define PMU_PWRDN_INTEN                          0X0050
 #define PMU_PWRDN_STATUS                         0X0054
 #define PMU_WAKEUP_STATUS                        0X0058
 #define PMU_BUS_CLR                              0X005C
 #define PMU_BUS_IDLE_REQ                         0X0060
 #define PMU_BUS_IDLE_ST                          0X0064
 #define PMU_BUS_IDLE_ACK                         0X0068
 #define PMU_CCI500_CON                           0X006C
 #define PMU_ADB400_CON                           0X0070
 #define PMU_ADB400_ST                            0X0074
 #define PMU_POWER_ST                             0X0078
 #define PMU_CORE_PWR_ST                          0X007C
 #define PMU_OSC_CNT                              0X0080
 #define PMU_PLLLOCK_CNT                          0X0084
 #define PMU_PLLRST_CNT                           0X0088
 #define PMU_STABLE_CNT                           0X008C
 #define PMU_DDRIO_PWRON_CNT                      0X0090
 #define PMU_WAKEUP_RST_CLR_CNT                   0X0094
 #define PMU_DDR_SREF_ST                          0X0098
 #define PMU_SCU_L_PWRDN_CNT                      0X009C
 #define PMU_SCU_L_PWRUP_CNT                      0X00A0
 #define PMU_SCU_B_PWRDN_CNT                      0X00A4
 #define PMU_SCU_B_PWRUP_CNT                      0X00A8
 #define PMU_GPU_PWRDN_CNT                        0X00AC
 #define PMU_GPU_PWRUP_CNT                        0X00B0
 #define PMU_CENTER_PWRDN_CNT                     0X00B4
 #define PMU_CENTER_PWRUP_CNT                     0X00B8
 #define PMU_TIMEOUT_CNT                          0X00BC
 #define PMU_CPU0APM_CON                          0X00C0
 #define PMU_CPU1APM_CON                          0X00C4
 #define PMU_CPU2APM_CON                          0X00C8
 #define PMU_CPU3APM_CON                          0X00CC
 #define PMU_CPU0BPM_CON                          0X00D0
 #define PMU_CPU1BPM_CON                          0X00D4
 #define PMU_NOC_AUTO_ENA                         0X00D8
 #define PMU_PWRDN_CON1                           0X00DC
 #define PMU_SYS_REG0                             0X00F0
 #define PMU_SYS_REG1                             0X00F4
 #define PMU_SYS_REG2                             0X00F8
 #define PMU_SYS_REG3                             0X00FC


/* GPIO */
#define RK3399_GPIO0_BASE                         0xFF720000
#define RK3399_GPIO1_BASE                         0xFF730000
#define RK3399_GPIO2_BASE                         0xFF780000
#define RK3399_GPIO3_BASE                         0xFF788000
#define RK3399_GPIO4_BASE                         0xFF790000
#define RK3399_GPIO_PORTS_NUM                     5
 #define GPIO_SWPORTA_DR                          0X0000
 #define GPIO_SWPORTA_DDR                         0X0004
 #define GPIO_INTEN                               0X0030
 #define GPIO_INTMASK                             0X0034
 #define GPIO_INTTYPE_LEVEL                       0X0038
 #define GPIO_INT_POLARITY                        0X003C
 #define GPIO_INT_STATUS                          0X0040
 #define GPIO_INT_RAWSTATUS                       0X0044
 #define GPIO_DEBOUNCE                            0X0048
 #define GPIO_PORTA_EOI                           0X004C
 #define GPIO_EXT_PORTA                           0X0050
 #define GPIO_LS_SYNC                             0X0060

/* USB 2.0 */
#define RK3399_USB20_HOST0                        0xFE380000
#define RK3399_USB20_HOST1                        0xFE3C0000

/* USB 3.0 */
#define RK3399_USB30_OTG0                         0xFE800000
#define RK3399_USB30_OTG1                         0xFE900000
#define RK3399_USB30_TYPEC_PHY0                   0xFF7C0000
#define RK3399_USB30_TYPEC_PHY1                   0xFF800000

/* I2C */
#define RK3399_I2C0_BASE                          0xFF3C0000
#define RK3399_I2C1_BASE                          0xFF110000
#define RK3399_I2C2_BASE                          0xFF120000
#define RK3399_I2C3_BASE                          0xFF130000
#define RK3399_I2C4_BASE                          0xFF3D0000
#define RK3399_I2C5_BASE                          0xFF140000
#define RK3399_I2C6_BASE                          0xFF150000
#define RK3399_I2C7_BASE                          0xFF160000
#define RK3399_I2C8_BASE                          0xFF3E0000
 #define RK3399_I2C_SIZE                          0x300

 /* SPI */
 #define RK3399_SPI0_BASE                          0xFF1C0000
 #define RK3399_SPI1_BASE                          0xFF1D0000
 #define RK3399_SPI2_BASE                          0xFF1E0000
 #define RK3399_SPI3_BASE                          0xFF350000
 #define RK3399_SPI4_BASE                          0xFF1F0000
 #define RK3399_SPI5_BASE                          0xFF200000
  #define RK3399_SPI_SIZE                          0x1000

/* DMAC */
#define RK3399_DMAC0_BASE                         0xFF6D0000
#define RK3399_DMAC1_BASE                         0xFF6E0000
 #define RK3399_DMAC_SIZE                         0x1000

/***************************
 * DMAC HW requests
 ************************* */
#define RK_HWI_DMA_REQ_ID_SHIFT                  0
#define RK_HWI_DMA_REQ_ID_MASK                   0xFF
#define RK_HWI_DMA_IDX_SHIFT                     8
#define RK_HWI_DMA_IDX_MASK                      0xFF

#define RK_HWI_DMA(_dma_idx, _hw_req_id)         \
            ( ((_hw_req_id) & RK_HWI_DMA_REQ_ID_MASK) << RK_HWI_DMA_REQ_ID_SHIFT) | \
            ( ((_dma_idx) & RK_HWI_DMA_IDX_MASK) << RK_HWI_DMA_IDX_SHIFT)

#define RK_HWI_DMA_TO_REQ_ID(_dma_ch)            \
            ( ((_dma_ch) >> RK_HWI_DMA_REQ_ID_SHIFT) & RK_HWI_DMA_REQ_ID_MASK)

#define RK_HWI_DMA_TO_DMAC_IDX(_dma_ch)          \
            ( ((_dma_ch) >> RK_HWI_DMA_IDX_SHIFT) & RK_HWI_DMA_IDX_MASK)

/* DMAC0 */
#define RK3399_HWI_DMA_I2S0_TX                    RK_HWI_DMA(0, 0)
#define RK3399_HWI_DMA_I2S0_RX                    RK_HWI_DMA(0, 1)
#define RK3399_HWI_DMA_I2S1_TX                    RK_HWI_DMA(0, 2)
#define RK3399_HWI_DMA_I2S1_RX                    RK_HWI_DMA(0, 3)
#define RK3399_HWI_DMA_I2S2_TX                    RK_HWI_DMA(0, 4)
#define RK3399_HWI_DMA_I2S2_RX                    RK_HWI_DMA(0, 5)
#define RK3399_HWI_DMA_PWM                        RK_HWI_DMA(0, 6)
#define RK3399_HWI_DMA_SPDIF_TX                   RK_HWI_DMA(0, 7)
#define RK3399_HWI_DMA_SPI5_TX                    RK_HWI_DMA(0, 8)
#define RK3399_HWI_DMA_SPI5_RX                    RK_HWI_DMA(0, 9)

/* DMAC1 */
#define RK3399_HWI_DMA_UART0_TX                   RK_HWI_DMA(1, 0)
#define RK3399_HWI_DMA_UART0_RX                   RK_HWI_DMA(1, 1)
#define RK3399_HWI_DMA_UART1_TX                   RK_HWI_DMA(1, 2)
#define RK3399_HWI_DMA_UART1_RX                   RK_HWI_DMA(1, 3)
#define RK3399_HWI_DMA_UART2_TX                   RK_HWI_DMA(1, 4)
#define RK3399_HWI_DMA_UART2_RX                   RK_HWI_DMA(1, 5)
#define RK3399_HWI_DMA_UART3_TX                   RK_HWI_DMA(1, 6)
#define RK3399_HWI_DMA_UART3_RX                   RK_HWI_DMA(1, 7)
#define RK3399_HWI_DMA_UART4_TX                   RK_HWI_DMA(1, 8)
#define RK3399_HWI_DMA_UART4_RX                   RK_HWI_DMA(1, 9)
#define RK3399_HWI_DMA_SPI0_TX                    RK_HWI_DMA(1, 10)
#define RK3399_HWI_DMA_SPI0_RX                    RK_HWI_DMA(1, 11)
#define RK3399_HWI_DMA_SPI1_TX                    RK_HWI_DMA(1, 12)
#define RK3399_HWI_DMA_SPI1_RX                    RK_HWI_DMA(1, 13)
#define RK3399_HWI_DMA_SPI2_TX                    RK_HWI_DMA(1, 14)
#define RK3399_HWI_DMA_SPI2_RX                    RK_HWI_DMA(1, 15)
#define RK3399_HWI_DMA_SPI3_TX                    RK_HWI_DMA(1, 16)
#define RK3399_HWI_DMA_SPI3_RX                    RK_HWI_DMA(1, 17)
#define RK3399_HWI_DMA_SPI4_TX                    RK_HWI_DMA(1, 18)
#define RK3399_HWI_DMA_SPI4_RX                    RK_HWI_DMA(1, 19)

/***************************
 * Interrupt ID
 ************************* */
#define RK3399_INTR_CRYPTO0                       32
#define RK3399_INTR_DCF_DONE                      33
#define RK3399_INTR_DCF_ERROR                     34
#define RK3399_INTR_DDRC0                         35
#define RK3399_INTR_DDRC1                         36
#define RK3399_INTR_DMAC0_PERILP_IRQ_ABORT        37
#define RK3399_INTR_DMAC0_PERILP                  38
#define RK3399_INTR_DMAC1_PERILP_IRQ_ABORT        39
#define RK3399_INTR_DMAC1_PERILP                  40
#define RK3399_INTR_DP                            41
#define RK3399_INTR_EDP                           42
#define RK3399_INTR_EMMCCORE                      43
#define RK3399_INTR_GMAC_PMT                      45
#define RK3399_INTR_GPIO0                         46
#define RK3399_INTR_GPIO1                         47
#define RK3399_INTR_GPIO2                         48
#define RK3399_INTR_GPIO3                         49
#define RK3399_INTR_GPIO4                         50
#define RK3399_INTR_GPU_IRQGPU                    51
#define RK3399_INTR_GPU_IRQJOB                    52
#define RK3399_INTR_GPU_IRQMMU                    53
#define RK3399_INTR_HDCP22                        54
#define RK3399_INTR_HDMI                          55
#define RK3399_INTR_HDMI_WAKEUP                   56
#define RK3399_INTR_HOST0_ARB                     57
#define RK3399_INTR_HOST0_EHCI                    58
#define RK3399_INTR_HOST0_LINESTATE               59
#define RK3399_INTR_HOST0_OHCI                    60
#define RK3399_INTR_HOST1_ARB                     61
#define RK3399_INTR_HOST1_EHCI                    62
#define RK3399_INTR_HOST1_LINESTATE               63
#define RK3399_INTR_HOST1_OHCI                    64
#define RK3399_INTR_HSIC                          65
#define RK3399_INTR_I2C3                          66
#define RK3399_INTR_I2C2                          67
#define RK3399_INTR_I2C7                          68
#define RK3399_INTR_I2C6                          69
#define RK3399_INTR_I2C5                          70
#define RK3399_INTR_I2S0                          71
#define RK3399_INTR_I2S1                          72
#define RK3399_INTR_I2S2                          73
#define RK3399_INTR_IEP                           74
#define RK3399_INTR_ISP0                          75
#define RK3399_INTR_ISP1                          76
#define RK3399_INTR_MIPI_DSI_HOST0                77
#define RK3399_INTR_MIPI_DSI_HOST1                78
#define RK3399_INTR_ERRIRQ_CCI                    79
#define RK3399_INTR_NOC                           80
#define RK3399_INTR_PCIE_SYS                      81
#define RK3399_INTR_PCIE_LEGACY                   82
#define RK3399_INTR_PCIE_CLIENT                   83
#define RK3399_INTR_SPI2                          84
#define RK3399_INTR_SPI1                          85
#define RK3399_INTR_PMU                           86
#define RK3399_INTR_RGA                           87
#define RK3399_INTR_I2C4                          88
#define RK3399_INTR_I2C0                          89
#define RK3399_INTR_I2C8                          90
#define RK3399_INTR_I2C1                          91
#define RK3399_INTR_SPI3                          92
#define RK3399_INTR_PWM                           93
#define RK3399_INTR_SARADC                        94
#define RK3399_INTR_SD_DETECTN                    95
#define RK3399_INTR_SDIO                          96
#define RK3399_INTR_SDMMC                         97
#define RK3399_INTR_SPDIF                         98
#define RK3399_INTR_SPI4                          99
#define RK3399_INTR_SPI0                          100
#define RK3399_INTR_STIMER_INTR0                  101
#define RK3399_INTR_STIMER_INTR1                  102
#define RK3399_INTR_STIMER_INTR2                  103
#define RK3399_INTR_STIMER_INTR3                  104
#define RK3399_INTR_STIMER_INTR4                  105
#define RK3399_INTR_STIMER_INTR5                  106
#define RK3399_INTR_STIMER_INTR6                  107
#define RK3399_INTR_STIMER_INTR7                  108
#define RK3399_INTR_STIMER_INTR8                  109
#define RK3399_INTR_STIMER_INTR9                  110
#define RK3399_INTR_STIMER_INTR10                 111
#define RK3399_INTR_STIMER_INTR11                 112
#define RK3399_INTR_TIMER_INTR0                   113
#define RK3399_INTR_TIMER_INTR1                   114
#define RK3399_INTR_TIMER_INTR2                   115
#define RK3399_INTR_TIMER_INTR3                   116
#define RK3399_INTR_TIMER_INTR4                   117
#define RK3399_INTR_TIMER_INTR5                   118
#define RK3399_INTR_TIMER_INTR6                   119
#define RK3399_INTR_TIMER_INTR7                   120
#define RK3399_INTR_TIMER_INTR8                   121
#define RK3399_INTR_TIMER_INTR9                   122
#define RK3399_INTR_TIMER_INTR10                  123
#define RK3399_INTR_TIMER_INTR11                  124
#define RK3399_INTR_PERF_INT_A53                  125
#define RK3399_INTR_PERF_INT_A72                  126
#define RK3399_INTR_PMUTIMER_INT0                 127
#define RK3399_INTR_PMUTIMER_INT1                 128
#define RK3399_INTR_TSADC                         129
#define RK3399_INTR_UART1                         130
#define RK3399_INTR_UART0                         131
#define RK3399_INTR_UART2                         132
#define RK3399_INTR_UART3                         133
#define RK3399_INTR_UART4                         134
#define RK3399_INTR_USB3OTG0_BVALID               135
#define RK3399_INTR_USB3OTG0_ID                   136
#define RK3399_INTR_USB3OTG0                      137
#define RK3399_INTR_USB3OTG0_LINESTATE            138
#define RK3399_INTR_USB3OTG0_RXDET                139
#define RK3399_INTR_USB3OTG1_BVALID               140
#define RK3399_INTR_USB3OTG1_ID                   141
#define RK3399_INTR_USB3OTG1                      142
#define RK3399_INTR_USB3OTG1_LINESTATE            143
#define RK3399_INTR_USB3OTG1_RXDET                144
#define RK3399_INTR_VCODEC_DEC                    145
#define RK3399_INTR_VCODEC_ENC                    146
#define RK3399_INTR_VCODEC_MMU                    147
#define RK3399_INTR_VDU_DEC                       148
#define RK3399_INTR_VDU_MMU                       149
#define RK3399_INTR_VOPBIG                        150
#define RK3399_INTR_VOPLIT                        151
#define RK3399_INTR_WDT0                          152
#define RK3399_INTR_WDT1                          153
#define RK3399_INTR_WDT2                          154
#define RK3399_INTR_USB3OTG0_PME_GENERATION       155
#define RK3399_INTR_USB3OTG0_HOST_LEGACY_SMI      156
#define RK3399_INTR_USB3OTG0_HOST_SYS_ERR         157
#define RK3399_INTR_USB3OTG1_PME_GENERATION       158
#define RK3399_INTR_USB3OTG1_HOST_LEGACY_SMI      159
#define RK3399_INTR_USB3OTG1_HOST_SYS_ERR         160
#define RK3399_INTR_VOPBIG_IRQ_DDR                161
#define RK3399_INTR_VOPLIT_IRQ_DDR                162
#define RK3399_INTR_DDR_MON                       163
#define RK3399_INTR_SPI5                          164
#define RK3399_INTR_TCPD_INT0                     165
#define RK3399_INTR_TCPD_INT1                     166
#define RK3399_INTR_CRYPTO1                       167
#define RK3399_INTR_GASKET                        168
#define RK3399_INTR_PCIE_RC_MODE_ELEC_IDLE        169
#define RK3399_INTR_MAILBOX1_INT0                 172
#define RK3399_INTR_MAILBOX1_INT1                 173
#define RK3399_INTR_MAILBOX1_INT2                 174
#define RK3399_INTR_MAILBOX1_INT3                 175
#define RK3399_INTR_MAILBOX0_INT0                 176
#define RK3399_INTR_MAILBOX0_INT1                 177
#define RK3399_INTR_MAILBOX0_INT2                 178
#define RK3399_INTR_MAILBOX0_INT3                 179
#define RK3399_INTR_EXTERRIRQ_PD_CORE_L           180
#define RK3399_INTR_EXTERRIRQ_PD_CORE_B           181


#endif /* __AARCH64_RK3399_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/rk3399.h $ $Rev: 932044 $" )
#endif
