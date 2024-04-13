/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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

#ifndef IMX_ECSPI_H_
#define IMX_ECSPI_H_

/* IMX_ECSPI_RXDATA - Receive Data Register (ECSPIx_RXDATA) */
#define IMX_ECSPI_RXDATA                        0x00

/* IMX_ECSPI_TXDATA - Transmit Data Register (ECSPIx_TXDATA) */
#define IMX_ECSPI_TXDATA                        0x04

/* IMX_ECSPI_CONREG - Control Register (ECSPIx_CONREG) */
#define IMX_ECSPI_CONREG                        0x08

/* IMX_ECSPI_CONREG bit-fields */
/* BURST_LENGTH - Burst Length */
#define IMX_ECSPI_CONREG_BURST_LENGTH_SHIFT     20
#define IMX_ECSPI_CONREG_BURST_LENGTH_MASK      0xFFF00000

/* CHANNEL_SELECT - SPI Channel select */
#define IMX_ECSPI_CONREG_CHANNEL_SELECT_SHIFT   18
#define IMX_ECSPI_CONREG_CHANNEL_SELECT_MASK    0xC0000

/* DRCTL - SPI Data Ready Control */
#define IMX_ECSPI_CONREG_DRCTL_SHIFT            16
#define IMX_ECSPI_CONREG_DRCTL_MASK             0x30000
//
#define IMX_ECSPI_CONREG_DRCTL_BV_EDGE          1
#define IMX_ECSPI_CONREG_DRCTL_BV_LEVEL         2

/* PRE_DIVIDER - SPI Pre Divider */
#define IMX_ECSPI_CONREG_PRE_DIVIDER_SHIFT      12
#define IMX_ECSPI_CONREG_PRE_DIVIDER_MASK       0xF000

/* POST_DIVIDER - SPI Post Divider */
#define IMX_ECSPI_CONREG_POST_DIVIDER_SHIFT     8
#define IMX_ECSPI_CONREG_POST_DIVIDER_MASK      0x0F00

/* CHANNEL_MODE */
#define IMX_ECSPI_CONREG_CHANNEL_MODE_SHIFT     4
#define IMX_ECSPI_CONREG_CHANNEL_MODE_MASK      0xF0
//
#define IMX_ECSPI_CONREG_CHANNEL_MODE_BV_MASTER 1
#define IMX_ECSPI_CONREG_CHANNEL_MODE_BV_SLAVE  0

/* SMC - Start Mode Control */
#define IMX_ECSPI_CONTREG_SMC_SHIFT             3
#define IMX_ECSPI_CONTREG_SMC_MASK              0x8

/* XCH - SPI Exchange Bit */
#define IMX_ECSPI_CONREG_XCH_SHIFT              2
#define IMX_ECSPI_CONREG_XCH_MASK               0x4

/* HT - Hardware trigger */
#define IMX_ECSPI_CONTREG_HT_SHIFT              1
#define IMX_ECSPI_CONTREG_HT_MASK               0x2

/* EN - SPI Block Enable Control */
#define IMX_ECSPI_CONEG_EN_SHIFT                0
#define IMX_ECSPI_CONREG_EN_MASK                0x1

/* IMX_ECSPI_CONFIGREG - Config Register (ECSPIx_CONFIGREG) */
#define IMX_ECSPI_CONFIGREG                     0x0C

/* IMX_ECSPI_CONFIGREG bit-fields */
/* HT_LENGTH - Message length in HT Mode */
#define IMX_ECSPI_CONFIGREG_HT_LENGTH_SHIFT     24
#define IMX_ECSPI_CONFIGREG_HT_LENGTH_MASK      0x1F000000

/* SCLK_CTL - Inactive state of SCLK per SPI channel */
#define IMX_ECSPI_CONFIGREG_SCLK_CTL_SHIFT      20
#define IMX_ECSPI_CONFIGREG_SCLK_CTL_MASK       0xF00000

/* DATA_CTL - Inactive state of the data line per SPI channel */
#define IMX_ECSPI_CONFIGREG_DATA_CTL_SHIFT      16
#define IMX_ECSPI_CONFIGREG_DATA_CTL_MASK       0xF0000

/* SS_POL DATA_CTL - Polarity of the SPI SS */
#define IMX_ECSPI_CONFIGREG_SS_POL_SHIFT        12
#define IMX_ECSPI_CONFIGREG_SS_POL_MASK         0xF000

/* SS_CTL - SPI SS Wave form select */
#define IMX_ECSPI_CONFIGREG_SS_CTL_SHIFT        8
#define IMX_ECSPI_CONFIGREG_SS_CTL_MASK         0xF00

/* CLK_POL - SPI Clock Polarity Control*/
#define IMX_ECSPI_CONFIGREG_CLK_POL_SHIFT       4
#define IMX_ECSPI_CONFIGREG_CLK_POL_MASK        0xF0

/* CLK_PHA - SPI Clock/Data Phase Control */
#define IMX_ECSPI_CONFIGREG_CLK_PHA_SHIFT       0
#define IMX_ECSPI_CONFIGREG_CLK_PHA_MASK        0xF

/* IMX_ECSPI_INTREG - Interrupt Control Register (ECSPIx_INTREG) */
#define IMX_ECSPI_INTREG                        0x10

/* IMX_ECSPI_INTREG bit-fields */
/* TCEN - Transfer Completed Interrupt enable */
#define IMX_ECSPI_INTREG_TCEN_SHIFT             7
#define IMX_ECSPI_INTREG_TCEN_MASK              0x80

/* ROEN - Rx FIFO Overflow Interrupt enable */
#define IMX_CSPI_INTREG_ROEN_SHIFT              6
#define IMX_CSPI_INTREG_ROEN_MASK               0x40

/* RFEN - Rx FIFO Full Interrupt enable */
#define IMX_ECSPI_INTREG_RFEN_SHIFT             5
#define IMX_ECSPI_INTREG_RFEN_MASK              0x20

/* RDREN - Rx FIFO Data Request Interrupt enable */
#define IMX_ECSPI_INTREG_RDREN_SHIFT            4
#define IMX_ECSPI_INTREG_RDREN_MASK             0x10

/* RREN - Rx FIFO Ready Interrupt enable */
#define IMX_ECSPI_INTREG_RREN_SHIFT             3
#define IMX_ECSPI_INTREG_RREN_MASK              0x8

/* TFEN - Tx FIFO Full Interrupt enable */
#define IMX_ECSPI_INTREG_TFEN_SHIFT             2
#define IMX_ECSPI_INTREG_TFEN_MASK              0x4

/* TDREN - Tx FIFO Data Request Interrupt enable */
#define IMX_ECSPI_INTREG_TDREN_SHIFT            1
#define IMX_ECSPI_INTREG_TDREN_MASK             0x2

/* TEEN - Tx FIFO Empty Interrupt enable */
#define IMX_ECSPI_INTREG_TEEN_SHIFT             0
#define IMX_ECSPI_INTREG_TEEN_MASK              0x1

/* IMX_ECSPI_DMAREG - DMA Control Register (ECSPIx_DMAREG) */
#define IMX_ECSPI_DMAREG                        0x14

/* IMX_ECSPI_STATREG - Status Register (ECSPIx_STATREG) */
#define IMX_ECSPI_STATREG                       0x18

/* IMX_ECSPI_STATREG bit-fields */
/* TC - Transfer Completed (w1c) */
#define IMX_ECSPI_STATREG_TC_SHIFT              7
#define IMX_ECSPI_STATREG_TC_MASK               0x80

/* RO - Rx FIFO Overflow (w1c) */
#define IMX_ECSPI_STATREG_RO_SHIFT              6
#define IMX_ECSPI_STATREG_RO_MASK               0x40

/* RF - Rx FIFO Full */
#define IMX_ECSPI_STATREG_RF_SHIFT              5
#define IMX_ECSPI_STATREG_RF_MASK               0x20

/* RDR - Rx FIFO Data Request */
#define IMX_ECSPI_STATREG_RDR_SHIFT             4
#define IMX_ECSPI_STATREG_RDR_MASK              0x10

/* RR - Rx FIFO Ready */
#define IMX_ECSPI_STATREG_RR_SHIFT              3
#define IMX_ECSPI_STATREG_RR_MASK               0x8

/* TF - Tx FIFO Full */
#define IMX_ECSPI_STATREG_TF_SHIFT              2
#define IMX_ECSPI_STATREG_TF_MASK               0x4

/* TDR - Tx FIFO Data Request */
#define IMX_ECSPI_STATREG_TDR_SHIFT             1
#define IMX_ECSPI_STATREG_TDR_MASK              0x2

/* TE - Tx FIFO Empty */
#define IMX_ECSPI_STATREG_TE_SHIFT              0
#define IMX_ECSPI_STATREG_TE_MASK               0x1

/* IMX_ECSPI_PERIODREG - Sample Period Control Register (ECSPIx_PERIODREG) */
#define IMX_ECSPI_PERIODREG                     0x1C

/* IMX_ECSPI_PERIODREG bit-fields */
/* CSD_CTL - Chip Select Delay Control bits */
#define IMX_ECSPI_PERIODREG_CSD_CTL_SHIFT       16
#define IMX_ECSPI_PERIODREG_CSD_CTL_MASK        0x3F0000

/* CSRC - Clock Source Control */
#define IMX_ECSPI_PERIODREG_CSRC_SHIFT          15
#define IMX_ECSPI_PERIODREG_CSRC_MASK           0x8000

/* SAMPLE_PERIOD - Sample Period Control */
#define IMX_ECSPI_PERIODREG_SAMPLE_PERIOD_SHIFT 0
#define IMX_ECSPI_PERIODREG_SAMPLE_PERIOD_MASK  0x7FFF

/* IMX_ECSPI_TESTREG - Test Control Register (ECSPIx_TESTREG) */
#define IMX_ECSPI_TESTREG                       0x20

/* IMX_ECSPI_TESTREG bit-fields */
/* LBC - Loop Back Control */
#define IMX_ECSPI_TESTREG_LBC_SHIFT             31
#define IMX_ECSPI_TESTREG_LBC_MASK              0x80000000

/* RXCNT - Rx FIFO Counter */
#define IMX_ECSPI_TESTREG_RXCNT_SHIFT           8
#define IMX_ECSPI_TESTREG_RXCNT_MASK            0x7F00

/* TXCNT - Tx FIFO Counter */
#define IMX_ECSPI_TESTREG_TXCNT_SHIFT           0
#define IMX_ECSPI_TESTREG_TXCNT_MASK            0x007F

/* IMX_ECSPI_MSGDATA - Message Data Register (ECSPIx_MSGDATA) */
#define IMX_ECSPI_MSGDATA                       0x40

#endif /* IMX_ECSPI_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_ecspi.h $ $Rev: 869196 $")
#endif
