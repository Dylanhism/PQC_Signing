/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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
 * NXP S32G SOCs:
 * S32G - 2x2 (2 cores per cluster) Cortex-A53 based SOC
 */

#ifndef	__AARCH64_S32G_H_INCLUDED
#define	__AARCH64_S32G_H_INCLUDED


/*
 * System Memory Map
 */

/*
 * LINFlexD (used for LIN and UART/serial communication)
 */
#define S32G_LINFLEXD0_BASE					0x401C8000
#define S32G_LINFLEXD0_IRQ					114
#define S32G_LINFLEXD1_BASE					0x401CC000
#define S32G_LINFLEXD1_IRQ					115

#define S32G_LINFLEXD0_SIZE					0x3000

/* Clock Generation Module */
#define S32G_MC_CGM0_BASE					0x40030000
#define S32G_MC_CGM1_BASE					0x40034000
#define S32G_MC_CGM5_BASE					0x40068000

/* MC_CGM registers definitions */
/* MC_CGM_MUX_n_CSS */
#define S32G_MC_CGM_MUXn_CSS(cgm_addr, mux) (((cgm_addr) + 0x304 + (mux) * 0x40))
#define S32G_MC_CGM_MUXn_CSS_SELCTL_MASK    (0x3F000000)
#define S32G_MC_CGM_MUXn_CSS_SELCTL_OFFSET  (24)
#define S32G_MC_CGM_MUXn_CSS_SELCTL_SIZE    (6)
#define S32G_MC_CGM_MUXn_CSS_CLK_SW         (1 << 2)

#define S32G_MC_CGM0_MUX12_DC0					0x608
#define S32G_MC_CGM0_MUX12_DC0_DE_OFFSET			31
#define S32G_MC_CGM0_MUX12_DC0_DE_MASK				(1 << S32G_MC_CGM0_MUX12_DC0_DE_OFFSET) // Divider enable bit
#define S32G_MC_CGM0_MUX12_DC0_DIV_OFFSET			16
#define S32G_MC_CGM0_MUX12_DC0_DIV_MASK				(0x4 << S32G_MC_CGM0_MUX12_DC0_DIV_OFFSET)

#define S32G_MC_CGM0_PCS_SDUR					0x703
#define S32G_MC_CGM0_PCS_DIVC1					0x704
#define S32G_MC_CGM0_PCS_DIVE1					0x708
#define S32G_MC_CGM0_PCS_DIVS1					0x70C
#define S32G_MC_CGM0_PCS_DIVC2					0x710
#define S32G_MC_CGM0_PCS_DIVE2					0x714
#define S32G_MC_CGM0_PCS_DIVS2					0x718
#define S32G_MC_CGM0_DIV_UPD_TYPE				0x7D4
#define S32G_MC_CGM0_DIV_UPD_TRIG				0x7D8
#define S32G_MC_CGM0_DIV_UPD_STAT				0x7DC
#define S32G_MC_CGM0_SC_SS					0x7E4
#define S32G_MC_CGM0_SC_DC0					0x7E8
#define S32G_MC_CGM0_SC_DC1					0x7EC
#define S32G_MC_CGM0_SC_DC2					0x7F0
#define S32G_MC_CGM0_ACn_SC(n)					(0x800+(n*0x20))	// select current clock source
#define S32G_MC_CGM0_ACn_SS(n)					(0x804+(n*0x20))	// read current clock source
#define S32G_MC_CGM0_ACn_DC0(n)					(0x808+(n*0x20))	// divider reg 0
	#define S32G_MC_CGM0_ACn_DE				(0x1 << 31)	// Divider Enable
	#define S32G_MC_CGM0_AC0_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC0_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC1_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC1_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC2_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC2_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC3_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC3_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC4_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC4_DIV_MASK			(0xf << 16)
	#define S32G_MC_CGM0_AC5_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC5_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC5_DIV1_OFFSET			16
	#define S32G_MC_CGM0_AC5_DIV1_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC6_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC6_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC7_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC7_DIV1_MASK			(0x1f << 16)
	#define S32G_MC_CGM0_AC8_DIV_OFFSET			16
	#define S32G_MC_CGM0_AC8_DIV_MASK			(0x3 << 16)
	#define S32G_MC_CGM0_AC8_DIV1_OFFSET			16
	#define S32G_MC_CGM0_AC8_DIV1_MASK			(0xf << 16)
	// ...

	#define S32G_MC_CGM0_ACn_DC1(n)				(0x80C+(n*0x20))	// divider reg 1 (not available for all aux clocks!)

	#define S32G_MC_CGM1_MUX0_CSS                   0x304
	#define S32G_MC_CGM1_MUX0_CSS_SELSTAT_MASK      (0x0F000000)
	#define S32G_MC_CGM1_MUX0_CSS_SELSTAT_OFFSET    (24)

/* Mode Entry Module - controls chip mode and mode transition sequences */
#define S32G_MC_ME_BASE						0x40088000
#define S32G_MC_ME_SIZE						0x1000
	#define S32G_MC_ME_CTL					0x0
	#define S32G_MC_ME_MODE					0x4
	#define S32G_MC_ME_UPD					0x8
	#define S32G_MC_ME_STAT					0xC
	#define S32G_MC_ME_COREID				0x10

/* PLL Digital Interface (PLLDIG) */
#define S32G_PLLDIG_CORE_PLL_BASE            0x40038000
#define S32G_PLLDIG_PERIPH_PLL_BASE          0x4003C000
#define S32G_PLLDIG_DDR_PLL_BASE             0x40044000

    /* PLLDIG PLL Control Register (PLLDIG_PLLCR) */
    #define S32G_PLLDIG_PLLCR                0x00
    #define S32G_PLLDIG_PLLCR_PLLPD          (1 << 31)

    /* PLLDIG PLL Status Register (PLLDIG_PLLSR) */
    #define S32G_PLLDIG_PLLSR                0x04
    #define S32G_PLLDIG_PLLSR_LOCK           (1 << 2)

    /* PLLDIG PLL Divider Register (PLLDIG_PLLDV) */
    #define S32G_PLLDIG_PLLDV                0x08
    #define S32G_PLLDIG_PLLDV_MFI_MASK       (0x000000FF)
    #define S32G_PLLDIG_PLLDV_RDIV_MASK      (0x00007000)
    #define S32G_PLLDIG_PLLDV_RDIV_OFFSET    (12)

    /* PLLDIG PLL Frequency Modulation Register (PLLDIG_PLLFM) */
    #define S32G_PLLDIG_PLLFM                0x0C
    #define S32G_PLLDIG_PLLFM_SSCGBYP        (1 << 30)

    /* PLLDIG PLL Fractional Divide Register (PLLDIG_PLLFD) */
    #define S32G_PLLDIG_PLLFD                0x10
    #define S32G_PLLDIG_PLLFD_MFN_MASK       (0x00007FFF)
    #define S32G_PLLDIG_PLLFD_SDMEN          (1 << 30)

    /* PLL Clock Mux (PLLCLKMUX) */
    #define S32G_PLLDIG_PLLCLKMUX                     0x20
    #define S32G_PLLDIG_PLLCLKMUX_REFCLKSEL_MASK      (0x3)
    #define S32G_PLLDIG_PLLCLKMUX_REFCLKSEL_SIZE      (2)
    #define S32G_PLLDIG_PLLCLKMUX_REFCLKSEL_OFFSET    (0)

    #define S32G_PLLDIG_PLLCLKMUX_REFCLKSEL_SET_FIRC  (0x0)
    #define S32G_PLLDIG_PLLCLKMUX_REFCLKSEL_SET_XOSC  (0x1)

    /* PLL Output Divider (PLLODIV0 - PLLODIV7) */
    #define S32G_PLLDIG_PLLODIV(n)               (0x80 + n * 0x4)
    #define S32G_PLLDIG_PLLODIV_DIV_MASK         (0x00FF0000)
    #define S32G_PLLDIG_PLLODIV_DIV_OFFSET       (16)
    #define S32G_PLLDIG_PLLODIV_DE               (1 << 31)

/* Clock source mapping on MC_CGM clock selectors. */
/* Clock source / Clock selector index: 23.3.1 */
#define S32G_MC_CGM_MUXn_CSC_SEL_FIRC                         0
#define S32G_MC_CGM_MUXn_CSC_SEL_SIRC                         1
#define S32G_MC_CGM_MUXn_CSC_SEL_FXOSC                        2
#define S32G_MC_CGM_MUXn_CSC_SEL_SXOSC                        3
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI0                 4
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI1                 5
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI2                 6
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI3                 7
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI4                 8
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI5                 9
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI6                 10
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_PHI7                 11
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS1                 12
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS2                 13
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS3                 14
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS4                 15
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS5                 16
#define S32G_MC_CGM_MUXn_CSC_SEL_ARM_PLL_DFS6                 17
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI0              18
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI1              19
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI2              20
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI3              21
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI4              22
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI5              23
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI6              24
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_PHI7              25
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS1              26
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS2              27
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS3              28
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS4              29
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS5              30
#define S32G_MC_CGM_MUXn_CSC_SEL_PERIPH_PLL_DFS6              31
#define S32G_MC_CGM_MUXn_CSC_SEL_ACCEL_PLL_PHI0               32
#define S32G_MC_CGM_MUXn_CSC_SEL_ACCEL_PLL_PHI1               33
#define S32G_MC_CGM_MUXn_CSC_SEL_FTM0_EXT_REF                 34
#define S32G_MC_CGM_MUXn_CSC_SEL_FTM1_EXT_REF                 35
#define S32G_MC_CGM_MUXn_CSC_SEL_DDR_PLL_PHI0                 36
#define S32G_MC_CGM_MUXn_CSC_SEL_GMAC_0_TX_CLK                37
#define S32G_MC_CGM_MUXn_CSC_SEL_GMAC_0_RX_CLK                38
#define S32G_MC_CGM_MUXn_CSC_SEL_GMAC_0_REF_CLK               39
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_0_LANE_0_TX_CLK       40
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_0_LANE_0_CDR_CLK      41
#define S32G_MC_CGM_MUXn_CSC_SEL_GMAC_0_TS_CLK                44
#define S32G_MC_CGM_MUXn_CSC_SEL_GMAC_0_REF_DIV_CLK           45
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_0_LANE_1_TX_CLK       46
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_0_LANE_1_CDR_CLK      47
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_0_EXT_TX_CLK         48
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_0_EXT_RX_CLK         49
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_0_EXT_REF_CLK        50
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_1_EXT_TX_CLK         51
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_1_EXT_RX_CLK         52
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_1_EXT_REF_CLK        53
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_2_EXT_TX_CLK         54
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_2_EXT_RX_CLK         55
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_2_EXT_REF_CLK        56
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_1_LANE_0_TX_CLK       57
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_1_LANE_0_TX_CLK_R45   59
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_1_LANE_0_CDR_CLK      58
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_0_REF_DIV_CLK        59
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_1_REF_DIV_CLK        60
#define S32G_MC_CGM_MUXn_CSC_SEL_PFE_MAC_2_REF_DIV_CLK        61
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_1_LANE_1_TX_CLK       62
#define S32G_MC_CGM_MUXn_CSC_SEL_SERDES_1_LANE_1_CDR_CLK      63

/* DFS Divide Register Portn (DFS_DVPORTn) */
#define S32G_CORE_DFS_BASE         0x40054000
#define S32G_PERIPH_DFS_BASE       0x40058000

#define DFS_DVPORTn(dfs, n)        ((dfs) + (0x0000001C + ((n) * sizeof(uint32_t))))
#define DFS_DVPORTn_MFI_MASK       (0x0000FF00)
#define DFS_DVPORTn_MFN_MASK       (0x000000FF)
#define DFS_DVPORTn_MFI_OFFSET     (8)
#define DFS_DVPORTn_MFN_OFFSET     (0)

/*
 * GMAC Controller
 */
#define S32G_ENET_BASE						0x4033C000
#define S32G_ENET_IRQ						89

#define S32G_ENET_SIZE						0x2000

/*
 * SWT
 */

#define S32G_SWT0_BASE 0x40100000
#define S32G_SWT1_BASE 0x40104000
#define S32G_SWT2_BASE 0x40108000
#define S32G_SWT3_BASE 0x4010C000
#define S32G_SWT4_BASE 0x40200000
#define S32G_SWT5_BASE 0x40204000
#define S32G_SWT6_BASE 0x40208000

#define S32G_SWT_SIZE	0x1000

#define SWT_CR		0x00	/* SWT Control Register */
    #define SWT_CR_FIXED_SS	(0 << 9)	/* -> Fixed Service Sequence */
    #define SWT_CR_KEYED_SS	(1 << 9)	/* -> Keyed Service Sequence */
    #define SWT_CR_RIA		(1 << 8)	/* -> Reset on Invalid Access */
    #define SWT_CR_WND		(1 << 7)	/* -> Window Mode */
    #define SWT_CR_ITR		(1 << 6)	/* -> Interrupt then reset */
    #define SWT_CR_HLK		(1 << 5)	/* -> Hard Lock */
    #define SWT_CR_SLK		(1 << 4)	/* -> Soft Lock */
    #define SWT_CR_STP		(1 << 2)	/* -> Stop Mode Control */
    #define SWT_CR_FRZ		(1 << 1)	/* -> Debug Mode Control */
    #define SWT_CR_WEN		(1 << 0)	/* -> Watchdog Enable */
#define SWT_IR		0x04	/* SWT Interrupt Register */
#define SWT_TO		0x08	/* SWT Timeout Register */
#define SWT_SR		0x10	/* SWT Service Register */
    #define WDT_SEQ1		0xA602	        /* -> service sequence 1 */
    #define WDT_SEQ2		0xB480	        /* -> service sequence 2 */
#define SWT_CO		0x14	/* SWT Counter Output Register */
#define SWT_SK		0x18	/* SWT Service Key Register */
#define SWT_RRR		0x1C	/* SWT Reset Request Register */
    #define SWT_RRR_RRF		(1 << 0)	/* -> Reset Request Flag */

/*
 * SIUL2
 */
#define S32G_SIUL2_0_BASE		0x4009C000
#define S32G_SIUL2_1_BASE		0x44010000

#define SIUL2_MIDR1			0x04
#define SIUL2_MIDR2			0x08
#define SIUL2_DISR0			0x10
#define SIUL2_DIRER0			0x18
#define SIUL2_DIRSR0			0x20
#define SIUL2_IREER0			0x28
#define SIUL2_IFEER0			0x30
#define SIUL2_IFER0			0x38
#define SIUL2_IFMCRn(i)			(0x40 + 4 * (i))
#define SIUL2_IFCPR			0xC0

/* SIUL2_0: MSCRs 0~101 and 512~595 */

/* MSCR0 - MSCR101 (i:0~101) */
#define SIUL2_0_MSCRn(i)		(0x240 + 4 * (i))
/* IMCR0 - IMCR83 (i: 512~595) */
#define SIUL2_0_IMCRn(i)		(0xA40 + 4 * (i - 512))
/* GPDO0 - GPDO101 (i:0~101) */
#define SIUL2_0_GPDOn(i)		(0x1300 + 4 * (i))
/* GPDI0 - GPDI101  (i:0~101) */
#define SIUL2_0_GPDIn(i)		(0x1500 + 4 * (i))
/* PGPDO0 - PGPDO5 (i:0~5)*/
#define SIUL2_0_PGPDOn(i)		(0x1700 + 2 * (i))
#define SIUL2_0_PGPDO6			(0x170E)
/* PGPDI0 - PGPDI5 (i:0~5)*/
#define SIUL2_0_PGPDIn(i)		(0x1740 + 2 * (i))
#define SIUL2_0_PGPDI6			(0x174E)
/* MPGPDO0 - MPGPDO5 (i:0~5)*/
#define SIUL2_0_MPGPDOn(i)		(0x1780 + 4 * (i))
#define SIUL2_0_MPGPDO6			(0x1798)

/* SIUL2_1: MSCRs 112~190 and 631~1007 */

/* MSCR112 - MSCR190 (i:112~190) */
#define SIUL2_1_MSCRn(i)		(0x400 + 4 * (i - 112))
/* IMCR119 - IMCR495 (i:631~1007) */
#define SIUL2_1_IMCRn(i)		(0xC1C + 4 * (i - 631))
/* GPDO112 - GPDO190 (i:112~190) */
#define SIUL2_1_GPDOn(i)		(0x1370 + 1 * (i - 112))
/* GPDI112 - GPDI190 (i:112~190) */
#define SIUL2_1_GPDIn(i)		(0x1570 + 1 * (i - 112))

#define SIUL2_1_PGPDO7			(0x170C)
#define SIUL2_1_PGPDO9			(0x1710)
#define SIUL2_1_PGPDO11			(0x1714)
#define SIUL2_1_PGPDO10			(0x1716)

#define SIUL2_1_PGPDI7			(0x174C)
#define SIUL2_1_PGPDI9			(0x1750)
#define SIUL2_1_PGPDI11			(0x1754)
#define SIUL2_1_PGPDI10			(0x1756)

#define SIUL2_1_MPGPDO7			(0x179C)
#define SIUL2_1_MPGPDO9			(0x17A4)
#define SIUL2_1_MPGPDO10		(0x17A8)
#define SIUL2_1_MPGPDO11		(0x17AC)

/* GPIO */
/* 163 GPIOs in output mode, we assume the GPIO number is in range */
#define SIUL2_GPDO_for_GPIO(i)		(((i) & (~0x3))>>2)
#define SIUL2_GPDO_PDO_off_for_GPIO(i)	(((i) & (0x3))
#define SIUL2_PDOn(i)			(SIUL2_GPDOn(SIUL2_GPDO_for_GPIO(i) + \
						SIUL2_GPDO_PDO_off_for_GPIO(i))
#define SIUL2_GPIO_VALUE0		(0x00)
#define SIUL2_GPIO_VALUE1		(0x01)

/* SIUL2_MSCR masks */
#define SIUL2_MSCR_OBE			(1 << 21)
#define SIUL2_MSCR_ODE			(1 << 20)
#define SIUL2_MSCR_IBE			(1 << 19)

#define SIUL2_MSCR_SRE_208MHZ		(0 << 14)
#define SIUL2_MSCR_SRE_150MHZ		(4 << 14)
#define SIUL2_MSCR_SRE_100MHZ		(5 << 14)
#define SIUL2_MSCR_SRE_50MHZ		(6 << 14)
#define SIUL2_MSCR_SRE_25MHZ		(7 << 14)

#define SIUL2_MSCR_PUE			(1 << 13)

#define SIUL2_MSCR_PUS_DOWN		(0 << 12)
#define SIUL2_MSCR_PUS_UP		(1 << 12)

#define SIUL2_MSCR_RCVR			(1 << 10)
#define SIUL2_MSCR_SMC			(1 << 5)

#define SIUL2_MSCR_SSS_0		(0x0)
#define SIUL2_MSCR_SSS_1		(0x1)
#define SIUL2_MSCR_SSS_2		(0x2)
#define SIUL2_MSCR_SSS_3		(0x3)
#define SIUL2_MSCR_SSS_4		(0x4)
#define SIUL2_MSCR_SSS_5		(0x5)
#define SIUL2_MSCR_SSS_6		(0x6)
#define SIUL2_MSCR_SSS_7		(0x7)
#define SIUL2_MSCR_SSS_MASK		(0x7)

/*
 * EDMA3
 */

#define S32G_EDMA0_BASE			0x40144000
#define S32G_EDMA0TX0_IRQ		40	/*EDMA tx channel 0~15 interrupt */
#define S32G_EDMA0TX1_IRQ		41	/*EDMA tx channel 16~31 interrupt */
#define S32G_EDMA0ERR_IRQ		42

#define S32G_EDMA1_BASE			0x40244000
#define S32G_EDMA1TX0_IRQ		43	/*EDMA tx channel 0~15 interrupt */
#define S32G_EDMA1TX1_IRQ		44	/*EDMA tx channel 16~31 interrupt */
#define S32G_EDMA1ERR_IRQ		45

#define S32G_EDMA_NUM_CH		32

#define S32G_DMAMUX0_BASE		0x4012C000
#define S32G_DMAMUX1_BASE		0x40130000
#define S32G_DMAMUX2_BASE		0x4022C000
#define S32G_DMAMUX3_BASE		0x40230000
#define S32G_DMAMUX_NUM_SLOTS		32


/*
 * CAN
 */
#define S32G_CAN0_PORT			0x401B4000
#define S32G_CAN0_MEM			0x401B4080
#define S32G_CAN0_IRQ			69

#define S32G_CAN1_PORT			0x401BE000
#define S32G_CAN1_MEM			0x401BE080
#define S32G_CAN1_IRQ			73

#define S32G_CAN2_PORT			0x402A8000
#define S32G_CAN2_MEM			0x402A8080
#define S32G_CAN2_IRQ			77

#define S32G_CAN3_PORT			0x402b2000
#define S32G_CAN3_MEM			0x402b2080
#define S32G_CAN3_IRQ			81

#define S32G_CAN_SIZE			0xA000

/*
 * QuadSPI
 */
#define S32G_QSPI_BASE			0x40134000
#define S32G_QSPI_IRQ			64
#define S32G_QSPI_SIZE			0x1000

/*
 * DSPI
 */
#define S32G_SPI0_BASE			0x401D4000
#define S32G_SPI0_IRQ			117
#define S32G_SPI1_BASE			0x401D8000
#define S32G_SPI1_IRQ			118
#define S32G_SPI2_BASE			0x401DC000
#define S32G_SPI2_IRQ			119
#define S32G_SPI3_BASE			0x402C8000
#define S32G_SPI3_IRQ			120
#define S32G_SPI4_BASE			0x402CC000
#define S32G_SPI4_IRQ			121
#define S32G_SPI5_BASE			0x402D0000
#define S32G_SPI5_IRQ			122

#define S32G_SPI_SIZE			0x140

/*
 * I2C Controllers
 */
#define S32G_I2C0_BASE			0x401E4000
#define S32G_I2C0_IRQ			124
#define S32G_I2C1_BASE			0x402E8000
#define S32G_I2C1_IRQ			125
#define S32G_I2C2_BASE			0x401EC000
#define S32G_I2C2_IRQ			126
#define S32G_I2C3_BASE			0x402D8000
#define S32G_I2C3_IRQ			127
#define S32G_I2C4_BASE			0x402DC000
#define S32G_I2C4_IRQ			128

#define S32G_I2C_SIZE			0x1000

/*
 * RTC
 */
#define S32G_RTC_BASE			0x40060000
#define S32G_RTC_IRQ			153
#define S32G_RTC_SIZE			0x100

#define S32G_RTCSUPV			0x0
#define S32G_RTCC			0x4
#define S32G_RTCS			0x8
#define S32G_RTCCNT			0xC
#define S32G_APIVAL			0x10
#define S32G_RTCVAL			0x14


#endif	/* __AARCH64_S32G_H_INCLUDED */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/s32g.h $ $Rev: 921266 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/s32g.h $ $Rev: 921266 $" )
#endif
#endif
