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

#ifndef IMX_QSPI_H_
#define IMX_QSPI_H_

#include <stdint.h>

/* Register MCR - Module Configuration Register */
#define IMX_QSPI_MCR                                0x0
/* Field DQS_PHASE_EN - This bit controls internal DQS output phase */
#define IMX_QSPI_MCR_DQS_PHASE_EN_SHIFT             26
#define IMX_QSPI_MCR_DQS_PHASE_EN_MASK              (0x1 << 26)
/* Field DQS_LOOPBACK_EN - Serial data strobe loop-back enable */
#define IMX_QSPI_MCR_DQS_LOOPBACK_EN_SHIFT          25
#define IMX_QSPI_MCR_DQS_LOOPBACK_EN_MASK           (0x1 << 25)
/* Field DQS_LOOPBACK_EN - Serial data strobe loop-back enable */
#define IMX_QSPI_MCR_DQS_LOOPBACK_FROM_PAD_SHIFT    24
#define IMX_QSPI_MCR_DQS_LOOPBACK_FROM_PAD_MASK     (0x1 << 24)
/* Field RESERVED - This reserved field must be always programmed to 0xF! */
#define IMX_QSPI_MCR_RESERVED_SHIFT                 16
#define IMX_QSPI_MCR_RESERVED_MASK                  (0xF << 16)
/* Field MDIS - Module Disable */
#define IMX_QSPI_MCR_MDIS_SHIFT                     14
#define IMX_QSPI_MCR_MDIS_MASK                      (0x1 << 14)
/* Field CLR_TXF - Clear TX FIFO/Buffer */
#define IMX_QSPI_MCR_CLR_TXF_SHIFT                  11
#define IMX_QSPI_MCR_CLR_TXF_MASK                   (0x1 << 11)
/* Field CLR_RXF - Clear RX FIFO/Buffer */
#define IMX_QSPI_MCR_CLR_RXF_SHIFT                  10
#define IMX_QSPI_MCR_CLR_RXF_MASK                   (0x1 << 10)
/* Field DDR_EN - SDR and DDR instructions are supported */
#define IMX_QSPI_MCR_DDR_EN_SHIFT                   7
#define IMX_QSPI_MCR_DDR_EN_MASK                    (0x1 << 7)
/* Field DQS_EN - DQS enable */
#define IMX_QSPI_MCR_DQS_EN_SHIFT                   6
#define IMX_QSPI_MCR_DQS_EN_MASK                    (0x1 << 6)
/* Field END_CFG - Endian configuration */
#define IMX_QSPI_MCR_END_CFG_SHIFT                  2
#define IMX_QSPI_MCR_END_CFG_MASK                   (0x3 << 2)
/* Field SWRSTHD - Software reset for QSPI AHB domain */
#define IMX_QSPI_MCR_SWRSTHD_SHIFT                  1
#define IMX_QSPI_MCR_SWRSTHD_MASK                   (0x1 << 1)
/* Field SWRSTSD - Software reset for Serial Flash domain */
#define IMX_QSPI_MCR_SWRSTSD_SHIFT                  0
#define IMX_QSPI_MCR_SWRSTSD_MASK                   (0x1 << 0)



/* Register IPCR - IP Configuration Register */
#define IMX_QSPI_IPCR                               0x08


/* IP Configuration Register */
/* Field SEQID - Points to a sequence in the Look-up-table. A write triggers a transaction. */
#define IMX_QSPI_IPCR_SEQID_SHIFT                   24
#define IMX_QSPI_IPCR_SEQID_MASK                    (0xF << 24)
/* Field PAR_EN - When set, a transaction to two serial flash devices is triggered in parallel mode. */
#define IMX_QSPI_IPCR_PAR_EN_SHIFT                  16
#define IMX_QSPI_IPCR_PAR_EN_MASK                   (0x1 << 16)
/* Field IDATSZ - IP data transfer size */
#define IMX_QSPI_IPCR_IDATSZ_SHIFT                  0
#define IMX_QSPI_IPCR_IDATSZ_MASK                   (0xFFFF << 0)

/* Register FLSHCR - Flash Configuration Register */
#define IMX_QSPI_FLSHCR                             0x0C
/* Field TDH - Serial flash Data In hold time */
#define IMX_QSPI_FLSHCR_TDH_SHIFT                   16
#define IMX_QSPI_FLSHCR_TDH_MASK                    (0x3 << 16)
/* Field TCSH - Serial flash CS hold time */
#define IMX_QSPI_FLSHCR_TCSH_SHIFT                  8
#define IMX_QSPI_FLSHCR_TCSH_MASK                   (0xF << 8)
/* Field TCSS - Serial flash CS setup time */
#define IMX_QSPI_FLSHCR_TCSS_SHIFT                  0
#define IMX_QSPI_FLSHCR_TCSS_MASK                   (0xF << 0)


/* Register BUF0CR - Buffer0 Configuration Register */
#define IMX_QSPI_BUF0CR                             0x10
/* Field HP_EN - High Priority Enable */
#define IMX_QSPI_BUF0CR_HP_EN_SHIFT                 31
#define IMX_QSPI_BUF0CR_HP_EN_MASK                  (0x1 << 31)
/* Field ADATSZ - AHB data transfer size */
#define IMX_QSPI_BUF0CR_ADATSZ_SHIFT                8
#define IMX_QSPI_BUF0CR_ADATSZ_MASK                 (0xFF << 8)
/* Field MSTRID - The ID of the AHB master */
#define IMX_QSPI_BUF0CR_MSTRID_SHIFT                0
#define IMX_QSPI_BUF0CR_MSTRID_MASK                 (0xF << 0)


/* Register BUF1CR - Buffer1 Configuration Register */
#define IMX_QSPI_BUF1CR                             0x14
/* Field ADATSZ - AHB data transfer size */
#define IMX_QSPI_BUF1CR_ADATSZ_SHIFT                8
#define IMX_QSPI_BUF1CR_ADATSZ_MASK                 (0xFF << 8)
/* Field MSTRID - The ID of the AHB master */
#define IMX_QSPI_BUF1CR_MSTRID_SHIFT                0
#define IMX_QSPI_BUF1CR_MSTRID_MASK                 (0xF << 0)



/* Register BUF2CR - Buffer2 Configuration Register */
#define IMX_QSPI_BUF2CR                             0x18
/* Field ADATSZ - AHB data transfer size */
#define IMX_QSPI_BUF2CR_ADATSZ_SHIFT                8
#define IMX_QSPI_BUF2CR_ADATSZ_MASK                 (0xFF << 8)
/* Field MSTRID - The ID of the AHB master */
#define IMX_QSPI_BUF2CR_MSTRID_SHIFT                0
#define IMX_QSPI_BUF2CR_MSTRID_MASK                 (0xF << 0)



/* Register BUF3CR - Buffer3 Configuration Register */
#define IMX_QSPI_BUF3CR                             0x1C
/* Field ALLMST - All master enable */
#define IMX_QSPI_BUF3CR_ALLMST_SHIFT                31
#define IMX_QSPI_BUF3CR_ALLMST_MASK                 (0x1 << 31)
/* Field ADATSZ - AHB data transfer size */
#define IMX_QSPI_BUF3CR_ADATSZ_SHIFT                8
#define IMX_QSPI_BUF3CR_ADATSZ_MASK                 (0xFF << 8)
/* Field MSTRID - The ID of the AHB master */
#define IMX_QSPI_BUF3CR_MSTRID_SHIFT                0
#define IMX_QSPI_BUF3CR_MSTRID_MASK                 (0xF << 0)



/* Register BFGENCR - Buffer Generic Configuration Register */
#define IMX_QSPI_BFGENCR                            0x20
/* Field PAR_EN - When set, a transaction to two serial flash devices is triggered in parallel mode. */
#define IMX_QSPI_BFGENCR_PAR_EN_SHIFT               16
#define IMX_QSPI_BFGENCR_PAR_EN_MASK                (0x1 << 16)
/* Field SEQID - Points to a sequence in the Look-up-table. A write triggers a transaction. */
#define IMX_QSPI_BFGENCR_SEQID_SHIFT                12
#define IMX_QSPI_BFGENCR_SEQID_MASK                 (0xF << 12)



/* Register BUF0IND - Buffer0 Top Index Register */
#define IMX_QSPI_BUF0IND                            0x30
/* Field TPINDX0 - Top index of buffer */
#define IMX_QSPI_BUF0IND_TPINDX0_SHIFT              3
#define IMX_QSPI_BUF0IND_TPINDX0_MASK               (0x1FFFFFFF << 3)



/* Register BUF1IND - Buffer1 Top Index Register */
#define IMX_QSPI_BUF1IND                            0x34
/* Field TPINDX1 - Top index of buffer */
#define IMX_QSPI_BUF1IND_TPINDX1_SHIFT              3
#define IMX_QSPI_BUF1IND_TPINDX1_MASK               (0x1FFFFFFF << 3)



/* Register BUF2IND - Buffer2 Top Index Register */
#define IMX_QSPI_BUF2IND                            0x38
/* Field TPINDX2 - Top index of buffer */
#define IMX_QSPI_BUF2IND_TPINDX2_SHIFT              3
#define IMX_QSPI_BUF2IND_TPINDX2_MASK               (0x1FFFFFFF << 3)



/* Register SFAR - Serial Flash Address Register */
#define IMX_QSPI_SFAR                               0x100
/* Field SFADR - Serial Flash Address */
#define IMX_QSPI_SFAR_SFADR_SHIFT                   0
#define IMX_QSPI_SFAR_SFADR_MASK                    (0xFFFFFFFF << 0)



/* Register SMPR - Sampling Register */
#define IMX_QSPI_SMPR                               0x108
/* Field SDRSMP - DDR Sampling point */
#define IMX_QSPI_SMPR_DDRSMP_SHIFT                  16
#define IMX_QSPI_SMPR_DDRSMP_MASK                   (0x7 << 16)
/* Field SDRSMP - SDR sampling point */
#define IMX_QSPI_SMPR_SDRSMP_SHIFT                  5
#define IMX_QSPI_SMPR_SDRSMP_MASK                   (0x3 << 5)



/* Register RBSR - RX Buffer Status Register */
#define IMX_QSPI_RBSR                               0x10C
/* Field RDCTR - Read Counter */
#define IMX_QSPI_RBSR_RDCTR_SHIFT                   16
#define IMX_QSPI_RBSR_RDCTR_MASK                    (0xFFFF << 16)
/* Field RDBFL - RX Buffer Fill Level (4 bytes) entries count */
#define IMX_QSPI_RBSR_RDBFL_SHIFT                   8
#define IMX_QSPI_RBSR_RDBFL_MASK                    (0x3F << 8)



/* Register RBCT - RX Buffer Control Register */
#define IMX_QSPI_RBCT                               0x110
/* Field RXBRD - RX Buffer Readout: 1: RBDR0 to RBDRn */
#define IMX_QSPI_RBCT_RXBRD_SHIFT                   8
#define IMX_QSPI_RBCT_RXBRD_MASK                    (0x1 << 8)
/* Field WMRK - RX Buffer Watermark */
#define IMX_QSPI_RBCT_WMRK_SHIFT                    0
#define IMX_QSPI_RBCT_WMRK_MASK                     (0x1F << 0)



/* Register TBSR - TX Buffer Status Register */
#define IMX_QSPI_TBSR                               0x150
/* Field TRCTR - Transmit Counter */
#define IMX_QSPI_TBSR_TRCTR_SHIFT                   16
#define IMX_QSPI_TBSR_TRCTR_MASK                    (0xFFFF << 16)
/* Field TRBFL - TX Buffer Fill Level */
#define IMX_QSPI_TBSR_TRBFL_SHIFT                   8
#define IMX_QSPI_TBSR_TRBFL_MASK                    (0x1F << 8)



/* Register TBDR - TX Buffer Data Register */
#define IMX_QSPI_TBDR                               0x154
/* Field TXDATA - TX Data */
#define IMX_QSPI_TBDR_TXDATA_SHIFT                  0
#define IMX_QSPI_TBDR_TXDATA_MASK                   (0xFFFFFFFF << 0)



/* Register SR - Status Register */
#define IMX_QSPI_SR                                 0x15C
/* Field DLPSMP - Data learning */
#define IMX_QSPI_SR_DLPSMP_SHIFT                    29
#define IMX_QSPI_SR_DLPSMP_MASK                     (0x7 << 29)
/* Field TXFULL - TX Buffer Full */
#define IMX_QSPI_SR_TXFULL_SHIFT                    27
#define IMX_QSPI_SR_TXFULL_MASK                     (0x1 << 27)
/* Field TXEDA - Tx Buffer Enough Data Available */
#define IMX_QSPI_SR_TXEDA_SHIFT                     24
#define IMX_QSPI_SR_TXEDA_MASK                      (0x1 << 24)
/* Field RXDMA - RX Buffer DMA */
#define IMX_QSPI_SR_RXDMA_SHIFT                     23
#define IMX_QSPI_SR_RXDMA_MASK                      (0x1 << 23)
/* Field RXFULL - RX Buffer Full */
#define IMX_QSPI_SR_RXFULL_SHIFT                    19
#define IMX_QSPI_SR_RXFULL_MASK                     (0x1 << 19)
/* Field RXWE - RX Buffer Watermark Exceeded */
#define IMX_QSPI_SR_RXWE_SHIFT                      16
#define IMX_QSPI_SR_RXWE_MASK                       (0x1 << 16)
/* Field AHB3FUL - AHB 3 Buffer Full */
#define IMX_QSPI_SR_AHB3FUL_SHIFT                   14
#define IMX_QSPI_SR_AHB3FUL_MASK                    (0x1 << 14)
/* Field AHB2FUL - AHB 2 Buffer Full */
#define IMX_QSPI_SR_AHB2FUL_SHIFT                   13
#define IMX_QSPI_SR_AHB2FUL_MASK                    (0x1 << 13)
/* Field AHB1FUL - AHB 1 Buffer Full */
#define IMX_QSPI_SR_AHB1FUL_SHIFT                   12
#define IMX_QSPI_SR_AHB1FUL_MASK                    (0x1 << 12)
/* Field AHB0FUL - AHB 0 Buffer Full */
#define IMX_QSPI_SR_AHB0FUL_SHIFT                   11
#define IMX_QSPI_SR_AHB0FUL_MASK                    (0x1 << 11)
/* Field AHB3NE - AHB 3 Buffer Not Empty */
#define IMX_QSPI_SR_AHB3NE_SHIFT                    10
#define IMX_QSPI_SR_AHB3NE_MASK                     (0x1 << 10)
/* Field AHB2NE - AHB 2 Buffer Not Empty */
#define IMX_QSPI_SR_AHB2NE_SHIFT                    9
#define IMX_QSPI_SR_AHB2NE_MASK                     (0x1 << 9)
/* Field AHB1NE - AHB 1 Buffer Not Empty */
#define IMX_QSPI_SR_AHB1NE_SHIFT                    8
#define IMX_QSPI_SR_AHB1NE_MASK                     (0x1 << 8)
/* Field AHB0NE - AHB 0 Buffer Not Empty */
#define IMX_QSPI_SR_AHB0NE_SHIFT                    7
#define IMX_QSPI_SR_AHB0NE_MASK                     (0x1 << 7)
/* Field AHBTRN - AHB Access Transaction pending */
#define IMX_QSPI_SR_AHBTRN_SHIFT                    6
#define IMX_QSPI_SR_AHBTRN_MASK                     (0x1 << 6)
/* Field AHBGNT - AHB Command priority Granted */
#define IMX_QSPI_SR_AHBGNT_SHIFT                    5
#define IMX_QSPI_SR_AHBGNT_MASK                     (0x1 << 5)
/* Field AHB_ACC - AHB Access */
#define IMX_QSPI_SR_AHB_ACC_SHIFT                   2
#define IMX_QSPI_SR_AHB_ACC_MASK                    (0x1 << 2)
/* Field IP_ACC - IP Access */
#define IMX_QSPI_SR_IP_ACC_SHIFT                    1
#define IMX_QSPI_SR_IP_ACC_MASK                     (0x1 << 1)
/* Field SR_BUSY - Module Busy */
#define IMX_QSPI_SR_BUSY_SHIFT                      0
#define IMX_QSPI_SR_BUSY_MASK                       (0x1 << 0)



/* Register FR - Flag Register */
#define IMX_QSPI_FR                                 0x160
/* Field DLPFF - Data learning Flag */
#define IMX_QSPI_FR_DLPFF_SHIFT                     31
#define IMX_QSPI_FR_DLPFF_MASK                      (0x1 << 31)
/* Field TBFF - TX Buffer Fill Flag */
#define IMX_QSPI_FR_TBFF_SHIFT                      27
#define IMX_QSPI_FR_TBFF_MASK                       (0x1 << 27)
/* Field TBUF - TX Buffer Underrun Flag */
#define IMX_QSPI_FR_TBUF_SHIFT                      26
#define IMX_QSPI_FR_TBUF_MASK                       (0x1 << 26)
/* Field ILLINE - Illegal Instruction Error Flag */
#define IMX_QSPI_FR_ILLINE_SHIFT                    23
#define IMX_QSPI_FR_ILLINE_MASK                     (0x1 << 23)
/* Field RBOF - RX Buffer Overflow Flag */
#define IMX_QSPI_FR_RBOF_SHIFT                      17
#define IMX_QSPI_FR_RBOF_MASK                       (0x1 << 17)
/* Field RBDF - RX Buffer Watermark Exceeded */
#define IMX_QSPI_FR_RBDF_SHIFT                      16
#define IMX_QSPI_FR_RBDF_MASK                       (0x1 << 16)
/* Field ABSEF - AHB Sequence Error Flag */
#define IMX_QSPI_FR_ABSEF_SHIFT                     15
#define IMX_QSPI_FR_ABSEF_MASK                      (0x1 << 15)
/* Field ABOF - AHB Buffer Overflow Flag */
#define IMX_QSPI_FR_ABOF_SHIFT                      12
#define IMX_QSPI_FR_ABOF_MASK                       (0x1 << 12)
/* Field IUEF - IP Command Usage Error Flag */
#define IMX_QSPI_FR_IUEF_SHIFT                      11
#define IMX_QSPI_FR_IUEF_MASK                       (0x1 << 11)
/* Field IPAEF - IP Command Trigger during AHB Access Error Flag */
#define IMX_QSPI_FR_IPAEF_SHIFT                     7
#define IMX_QSPI_FR_IPAEF_MASK                      (0x1 << 7)
/* Field IPIEF - IP Command Trigger could not be executed Error Flag */
#define IMX_QSPI_FR_IPIEF_SHIFT                     6
#define IMX_QSPI_FR_IPIEF_MASK                      (0x1 << 6)
/* Field IPGEF - IP Command Trigger during AHB Grant Error Flag */
#define IMX_QSPI_FR_IPGEF_SHIFT                     4
#define IMX_QSPI_FR_IPGEF_MASK                      (0x1 << 4)
/* Field TFF - IP Command Transaction Finished Flag */
#define IMX_QSPI_FR_TFF_SHIFT                       0
#define IMX_QSPI_FR_TFF_MASK                        (0x1 << 0)




/* Register RSER - Interrupt and DMA Request Select and Enable Register */
#define IMX_QSPI_RSER                               0x164
/* Field DLPFIE - Data learning Interrupt */
#define IMX_QSPI_RSER_DLPFIE_SHIFT                  31
#define IMX_QSPI_RSER_DLPFIE_MASK                   (0x1 << 31)
/* Field TBFIE - TX Buffer Fill Interrupt */
#define IMX_QSPI_RSER_TBFIE_SHIFT                   27
#define IMX_QSPI_RSER_TBFIE_MASK                    (0x1 << 27)
/* Field TBUIE - TX Buffer Underrun Interrupt */
#define IMX_QSPI_RSER_TBUIE_SHIFT                   26
#define IMX_QSPI_RSER_TBUIE_MASK                    (0x1 << 26)
/* Field ILLINIE - Illegal Instruction Error Interrupt */
#define IMX_QSPI_RSER_ILLINIE_SHIFT                 23
#define IMX_QSPI_RSER_ILLINIE_MASK                  (0x1 << 23)
/* Field RBDDE - RX Buffer Drain DMA Enable */
#define IMX_QSPI_RSER_RBDDE_SHIFT                   21
#define IMX_QSPI_RSER_RBDDE_MASK                    (0x1 << 21)
/* Field RBOIE - RX Buffer Overflow Interrupt */
#define IMX_QSPI_RSER_RBOIE_SHIFT                   17
#define IMX_QSPI_RSER_RBOIE_MASK                    (0x1 << 17)
/* Field RBDIE - RX Buffer Drain Interrupt */
#define IMX_QSPI_RSER_RBDIE_SHIFT                   16
#define IMX_QSPI_RSER_RBDIE_MASK                    (0x1 << 16)
/* Field ABSEIE - AHB Sequence Error Interrupt */
#define IMX_QSPI_RSER_ABSEIE_SHIFT                  15
#define IMX_QSPI_RSER_ABSEIE_MASK                   (0x1 << 15)
/* Field ABOIE - AHB Buffer Overflow Interrupt */
#define IMX_QSPI_RSER_ABOIE_SHIFT                   12
#define IMX_QSPI_RSER_ABOIE_MASK                    (0x1 << 12)
/* Field IUEIE - IP Command Usage Error Interrupt */
#define IMX_QSPI_RSER_IUEIE_SHIFT                   11
#define IMX_QSPI_RSER_IUEIE_MASK                    (0x1 << 11)
/* Field IPAEIE - IP Command Trigger during AHB Access Error Interrupt */
#define IMX_QSPI_RSER_IPAEIE_SHIFT                  7
#define IMX_QSPI_RSER_IPAEIE_MASK                   (0x1 << 7)
/* Field IPIEIE - IP Command Trigger could not be executed Error Interrupt */
#define IMX_QSPI_RSER_IPIEIE_SHIFT                  6
#define IMX_QSPI_RSER_IPIEIE_MASK                   (0x1 << 6)
/* Field IPGEIE - IP Command Trigger during AHB Grant Error Interrupt */
#define IMX_QSPI_RSER_IPGEIE_SHIFT                  4
#define IMX_QSPI_RSER_IPGEIE_MASK                   (0x1 << 4)
/* Field TFIE - IP Command Transaction Finished Interrupt */
#define IMX_QSPI_RSER_TFIE_SHIFT                    0
#define IMX_QSPI_RSER_TFIE_MASK                     (0x1 << 0)



/* Register SPNDST - Sequence Suspend Status Register */
#define IMX_QSPI_SPNDST                             0x168
/* Field DATLFT - Data left */
#define IMX_QSPI_SPNDST_DATLFT_SHIFT                9
#define IMX_QSPI_SPNDST_DATLFT_MASK                 (0x7F << 9)
/* Field SPDBUF - Suspended Buffer */
#define IMX_QSPI_SPNDST_SPDBUF_SHIFT                6
#define IMX_QSPI_SPNDST_SPDBUF_MASK                 (0x3 << 6)
/* Field SUSPND - Sequence is in suspended state */
#define IMX_QSPI_SPNDST_SUSPND_SHIFT                0
#define IMX_QSPI_SPNDST_SUSPND_MASK                 (0x1 << 0)



/* Register SPTRCLR - Sequence Pointer Clear Register */
#define IMX_QSPI_SPTRCLR                            0x16C
/* Field IPPTRC - IP Pointer Clear */
#define IMX_QSPI_SPTRCLR_IPPTRC_SHIFT               8
#define IMX_QSPI_SPTRCLR_IPPTRC_MASK                (0x1 << 8)
/* Field BFPTRC - Buffer Pointer Clear */
#define IMX_QSPI_SPTRCLR_BFPTRC_SHIFT               0
#define IMX_QSPI_SPTRCLR_BFPTRC_MASK                (0x1 << 0)



/* Register SFA1AD - Serial Flash A1Top Address Register */
#define IMX_QSPI_SFA1AD                             0x180
/* Field TPADA1 - Top address for Serial Flash A1 */
#define IMX_QSPI_SFA1AD_TPADA1_SHIFT                10
#define IMX_QSPI_SFA1AD_TPADA1_MASK                 (0x3FFFFF << 10)



/* Register SFA2AD - Serial Flash A2Top Address Register */
#define IMX_QSPI_SFA2AD                             0x184
/* Field TPADA2 - Top address for Serial Flash A2 */
#define IMX_QSPI_SFA1AD_TPADA2_SHIFT                10
#define IMX_QSPI_SFA1AD_TPADA2_MASK                 (0x3FFFFF << 10)



/* Register SFB1AD - Serial Flash B1Top Address Register */
#define IMX_QSPI_SFB1AD                             0x188
/* Field TPADB1 - Top address for Serial Flash B1 */
#define IMX_QSPI_SFB1AD_TPADB1_SHIFT                10
#define IMX_QSPI_SFB1AD_TPADB1_MASK                 (0x3FFFFF << 10)



/* Register SFB2AD - Serial Flash B2Top Address Register */
#define IMX_QSPI_SFB2AD                             0x18C
/* Field TPADB2 - Top address for Serial Flash B2 */
#define IMX_QSPI_SFB1AD_TPADB2_SHIFT                10
#define IMX_QSPI_SFB1AD_TPADB2_MASK                 (0x3FFFFF << 10)



/* Register RBDRn - RX Buffer Data Register n */
#define IMX_QSPI_RBDRn_NUM                          32
#define IMX_QSPI_RBDR0                              0x200
#define IMX_QSPI_RBDRn(index)                       (IMX_QSPI_RBDR0 + (index * 4))



/* Register LUTKEY - LUT Key Register */
#define IMX_QSPI_LUTKEY                             0x300
#define IMX_QSPI_LUTKEY_VAL                         0x5AF05AF0
/* Field KEY - The key is 0x5AF05AF0 */
#define IMX_QSPI_LUTKEY_KEY_SHIFT                   0
#define IMX_QSPI_LUTKEY_KEY_MASK                    (0xFFFFFFFF << 0)

/* Register LCKCR - LUT Lock Configuration Register */
#define IMX_QSPI_LCKCR                              0x304
/* Field UNLOCK - Unlocks the LUT */
#define IMX_QSPI_LCKCR_UNLOCK_SHIFT                 1
#define IMX_QSPI_LCKCR_UNLOCK_MASK                  (0x1 << 1)
/* Field LOCK - Locks the LUT */
#define IMX_QSPI_LCKCR_LOCK_SHIFT                   0
#define IMX_QSPI_LCKCR_LOCK_MASK                    (0x1 << 0)



/* Register LUTn - Look-up Table register n */
#define IMX_QSPI_LUTn_NUM                           64
#define IMX_QSPI_LUT0                               0x310
#define IMX_QSPI_LUTn(index)                        (IMX_QSPI_LUT0 + (index * 4))
/* Field OPCODE1 */
#define IMX_QSPI_LUT_INSTR1_SHIFT                   26
#define IMX_QSPI_LUT_INSTR1_MASK                    (0x3F << 26)
/* Field NUM_PADS1 */
#define IMX_QSPI_LUT_PAD1_SHIFT                     24
#define IMX_QSPI_LUT_PAD1_MASK                      (0x3 << 24)
/* Field OPERAND1 */
#define IMX_QSPI_LUT_OPRND1_SHIFT                   16
#define IMX_QSPI_LUT_OPRND1_MASK                    (0xFF << 16)
/* Field OPCODE0 */
#define IMX_QSPI_LUT_INSTR0_SHIFT                   10
#define IMX_QSPI_LUT_INSTR0_MASK                    (0x3F << 10)
/* Field NUM_PADS0 */
#define IMX_QSPI_LUT_PAD0_SHIFT                     8
#define IMX_QSPI_LUT_PAD0_MASK                      (0x3 << 8)
/* Field OPERAND0 */
#define IMX_QSPI_LUT_OPRND0_SHIFT                   0
#define IMX_QSPI_LUT_OPRND0_MASK                    (0xFF << 0)
/* PAD0/PAD1 bit values */
#define IMX_QSPI_LUT_PAD_1                          0x0        /* Communication on 1 pad only */
#define IMX_QSPI_LUT_PAD_2                          0x1        /* Communication on 2 pads */
#define IMX_QSPI_LUT_PAD_4                          0x2        /* Communication on 4 pads */
/* INST0/INSTR1 bit values */
#define IMX_QSPI_LUT_INSTR_CMD                      1           /* Command */
#define IMX_QSPI_LUT_INSTR_ADDR                     2           /* Instruction address */
#define IMX_QSPI_LUT_DUMMY                          3           /* Dummy operation */
#define IMX_QSPI_LUT_READ                           7           /* Read */
#define IMX_QSPI_LUT_WRITE                          8           /* Write */
#define IMX_QSPI_LUT_JMP_ON_CS                      9           /* Jump on cs */
#define IMX_QSPI_LUT_ADDR_DDR                       10          /* DDR address */
#define IMX_QSPI_LUT_READ_DDR                       14          /* DDR read */
#define IMX_QSPI_LUT_STOP                           0           /* Stop */
/** LUT entry */
typedef union {
    uint32_t U;
    struct {
        unsigned OPRND0: 8;
        unsigned PAD0: 2;
        unsigned INSTR0: 6;
        unsigned OPRND1: 8;
        unsigned PAD1: 2;
        unsigned INSTR1: 6;
    } B;
} imx_qspi_lutx_t;


#endif /* IMX_QSPI_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_qspi.h $ $Rev: 904597 $")
#endif
