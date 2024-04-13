/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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
 * Xilinx Z7000 processor with Dual ARMv7 Cortex-A9 core
 */

#ifndef    __ARM_XZYNQ_H_INCLUDED
#define    __ARM_XZYNQ_H_INCLUDED

#include <arm/xzynq_irq.h>

/* -------------------------------------------------------------------------
 * Devices
 * -------------------------------------------------------------------------
 */
#define XZYNQ_HWI_UART                        "uart"
#define XZYNQ_HWI_WDT                         "wdt"
#define XZYNQ_HWI_DMA                         "dma"
#define XZYNQ_HWI_ENET                        "gem"
#define XZYNQ_HWI_ENET_MDIO                   "gem,mdio"
#define XZYNQ_HWI_PSS_REF_CLK                 "pss_ref_clk"

/* -------------------------------------------------------------------------
 * SDRAM
 * -------------------------------------------------------------------------
 */
#define XZYNQ_SDRAM_64_BIT_BASE                 0x00100000

/* -------------------------------------
 * EMAC
 * -------------------------------------
 */

#define XZYNQ_EMAC0_BASE           0xE000B000
#define XZYNQ_EMAC1_BASE           0xE000C000
#define XZYNQ_EMAC_SIZE            0x1000

/* Register offset definitions. Unless otherwise noted, register access is
 * 32 bit. Names are self explained here.
 */

#define XZYNQ_EMACPS_NWCTRL_OFFSET        0x00000000    /**< Network Control reg */
#define XZYNQ_EMACPS_NWCFG_OFFSET         0x00000004    /**< Network Config reg */
#define XZYNQ_EMACPS_NWSR_OFFSET          0x00000008    /**< Network Status reg */

#define XZYNQ_EMACPS_DMACR_OFFSET         0x00000010    /**< DMA Control reg */
#define XZYNQ_EMACPS_TXSR_OFFSET          0x00000014    /**< TX Status reg */
#define XZYNQ_EMACPS_RXQBASE_OFFSET       0x00000018    /**< RX Q Base address reg */
#define XZYNQ_EMACPS_TXQBASE_OFFSET       0x0000001C    /**< TX Q Base address reg */
#define XZYNQ_EMACPS_RXSR_OFFSET          0x00000020    /**< RX Status reg */

#define XZYNQ_EMACPS_ISR_OFFSET           0x00000024    /**< Interrupt Status reg */
#define XZYNQ_EMACPS_IER_OFFSET           0x00000028    /**< Interrupt Enable reg */
#define XZYNQ_EMACPS_IDR_OFFSET           0x0000002C    /**< Interrupt Disable reg */
#define XZYNQ_EMACPS_IMR_OFFSET           0x00000030    /**< Interrupt Mask reg */

#define XZYNQ_EMACPS_PHYMNTNC_OFFSET      0x00000034    /**< Phy Maintaince reg */
#define XZYNQ_EMACPS_RXPAUSE_OFFSET       0x00000038    /**< RX Pause Time reg */
#define XZYNQ_EMACPS_TXPAUSE_OFFSET       0x0000003C    /**< TX Pause Time reg */

#define XZYNQ_EMACPS_HASHL_OFFSET         0x00000080    /**< Hash Low address reg */
#define XZYNQ_EMACPS_HASHH_OFFSET         0x00000084    /**< Hash High address reg */

#define XZYNQ_EMACPS_LADDR1L_OFFSET       0x00000088    /**< Specific1 addr low reg */
#define XZYNQ_EMACPS_LADDR1H_OFFSET       0x0000008C    /**< Specific1 addr high reg */
#define XZYNQ_EMACPS_LADDR2L_OFFSET       0x00000090    /**< Specific2 addr low reg */
#define XZYNQ_EMACPS_LADDR2H_OFFSET       0x00000094    /**< Specific2 addr high reg */
#define XZYNQ_EMACPS_LADDR3L_OFFSET       0x00000098    /**< Specific3 addr low reg */
#define XZYNQ_EMACPS_LADDR3H_OFFSET       0x0000009C    /**< Specific3 addr high reg */
#define XZYNQ_EMACPS_LADDR4L_OFFSET       0x000000A0    /**< Specific4 addr low reg */
#define XZYNQ_EMACPS_LADDR4H_OFFSET       0x000000A4    /**< Specific4 addr high reg */

#define XZYNQ_EMACPS_MATCH1_OFFSET        0x000000A8    /**< Type ID1 Match reg */
#define XZYNQ_EMACPS_MATCH2_OFFSET        0x000000AC    /**< Type ID2 Match reg */
#define XZYNQ_EMACPS_MATCH3_OFFSET        0x000000B0    /**< Type ID3 Match reg */
#define XZYNQ_EMACPS_MATCH4_OFFSET        0x000000B4    /**< Type ID4 Match reg */

#define XZYNQ_EMACPS_STRETCH_OFFSET       0x000000BC    /**< IPG Stretch reg */

#define XZYNQ_EMACPS_OCTTXL_OFFSET        0x00000100    /**< Octects transmitted Low
                                                       reg */
#define XZYNQ_EMACPS_OCTTXH_OFFSET        0x00000104    /**< Octects transmitted High
                                                       reg */

#define XZYNQ_EMACPS_TXCNT_OFFSET         0x00000108    /**< Error-free Frmaes
                                                       transmitted counter */
#define XZYNQ_EMACPS_TXBCCNT_OFFSET       0x0000010C    /**< Error-free Broadcast
                                                       Frames counter*/
#define XZYNQ_EMACPS_TXMCCNT_OFFSET       0x00000110    /**< Error-free Multicast
                                                       Frame counter */
#define XZYNQ_EMACPS_TXPAUSECNT_OFFSET    0x00000114    /**< Pause Frames Transmitted
                                                       Counter */
#define XZYNQ_EMACPS_TX64CNT_OFFSET       0x00000118    /**< Error-free 64 byte Frames
                                                       Transmitted counter */
#define XZYNQ_EMACPS_TX65CNT_OFFSET       0x0000011C    /**< Error-free 65-127 byte
                                                       Frames Transmitted
                                                       counter */
#define XZYNQ_EMACPS_TX128CNT_OFFSET      0x00000120    /**< Error-free 128-255 byte
                                                       Frames Transmitted
                                                       counter*/
#define XZYNQ_EMACPS_TX256CNT_OFFSET      0x00000124    /**< Error-free 256-511 byte
                                                       Frames transmitted
                                                       counter */
#define XZYNQ_EMACPS_TX512CNT_OFFSET      0x00000128    /**< Error-free 512-1023 byte
                                                       Frames transmitted
                                                       counter */
#define XZYNQ_EMACPS_TX1024CNT_OFFSET     0x0000012C    /**< Error-free 1024-1518 byte
                                                       Frames transmitted
                                                       counter */
#define XZYNQ_EMACPS_TX1519CNT_OFFSET     0x00000130    /**< Error-free larger than
                                                       1519 byte Frames
                                                       transmitted counter */
#define XZYNQ_EMACPS_TXURUNCNT_OFFSET     0x00000134    /**< TX under run error
                                                       counter */

#define XZYNQ_EMACPS_SNGLCOLLCNT_OFFSET   0x00000138    /**< Single Collision Frame
                                                       Counter */
#define XZYNQ_EMACPS_MULTICOLLCNT_OFFSET  0x0000013C    /**< Multiple Collision Frame
                                                       Counter */
#define XZYNQ_EMACPS_EXCESSCOLLCNT_OFFSET 0x00000140    /**< Excessive Collision Frame
                                                       Counter */
#define XZYNQ_EMACPS_LATECOLLCNT_OFFSET   0x00000144    /**< Late Collision Frame
                                                       Counter */
#define XZYNQ_EMACPS_TXDEFERCNT_OFFSET    0x00000148    /**< Deferred Transmission
                                                       Frame Counter */
#define XZYNQ_EMACPS_TXCSENSECNT_OFFSET   0x0000014C    /**< Transmit Carrier Sense
                                                       Error Counter */

#define XZYNQ_EMACPS_OCTRXL_OFFSET        0x00000150    /**< Octects Received register
                                                       Low */
#define XZYNQ_EMACPS_OCTRXH_OFFSET        0x00000154    /**< Octects Received register
                                                       High */

#define XZYNQ_EMACPS_RXCNT_OFFSET         0x00000158    /**< Error-free Frames
                                                       Received Counter */
#define XZYNQ_EMACPS_RXBROADCNT_OFFSET    0x0000015C    /**< Error-free Broadcast
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RXMULTICNT_OFFSET    0x00000160    /**< Error-free Multicast
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RXPAUSECNT_OFFSET    0x00000164    /**< Pause Frames
                                                       Received Counter */
#define XZYNQ_EMACPS_RX64CNT_OFFSET       0x00000168    /**< Error-free 64 byte Frames
                                                       Received Counter */
#define XZYNQ_EMACPS_RX65CNT_OFFSET       0x0000016C    /**< Error-free 65-127 byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RX128CNT_OFFSET      0x00000170    /**< Error-free 128-255 byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RX256CNT_OFFSET      0x00000174    /**< Error-free 256-512 byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RX512CNT_OFFSET      0x00000178    /**< Error-free 512-1023 byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RX1024CNT_OFFSET     0x0000017C    /**< Error-free 1024-1518 byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RX1519CNT_OFFSET     0x00000180    /**< Error-free 1519-max byte
                                                       Frames Received Counter */
#define XZYNQ_EMACPS_RXUNDRCNT_OFFSET     0x00000184    /**< Undersize Frames Received
                                                       Counter */
#define XZYNQ_EMACPS_RXOVRCNT_OFFSET      0x00000188    /**< Oversize Frames Received
                                                       Counter */
#define XZYNQ_EMACPS_RXJABCNT_OFFSET      0x0000018C    /**< Jabbers Received
                                                       Counter */
#define XZYNQ_EMACPS_RXFCSCNT_OFFSET      0x00000190    /**< Frame Check Sequence
                                                       Error Counter */
#define XZYNQ_EMACPS_RXLENGTHCNT_OFFSET   0x00000194    /**< length Field Error
                                                       Counter */
#define XZYNQ_EMACPS_RXSYMBCNT_OFFSET     0x00000198    /**< Symbol Error Counter */
#define XZYNQ_EMACPS_RXALIGNCNT_OFFSET    0x0000019C    /**< alignment Error Counter */
#define XZYNQ_EMACPS_RXRESERRCNT_OFFSET   0x000001A0    /**< Receive Resource Error
                                                       Counter */
#define XZYNQ_EMACPS_RXORCNT_OFFSET       0x000001A4    /**< Receive Overrun Counter */
#define XZYNQ_EMACPS_RXIPCCNT_OFFSET      0x000001A8    /**< IP header Checksum Error
                                                       Counter */
#define XZYNQ_EMACPS_RXTCPCCNT_OFFSET     0x000001AC    /**< TCP Checksum Error
                                                       Counter */
#define XZYNQ_EMACPS_RXUDPCCNT_OFFSET     0x000001B0    /**< UDP Checksum Error
                                                       Counter */
#define XZYNQ_EMACPS_LAST_OFFSET          0x000001B4    /**< Last statistic counter
                                                       offset, for clearing */

#define XZYNQ_EMACPS_1588_SEC_OFFSET      0x000001D0    /**< 1588 second counter */
#define XZYNQ_EMACPS_1588_NANOSEC_OFFSET  0x000001D4    /**< 1588 nanosecond counter */
#define XZYNQ_EMACPS_1588_ADJ_OFFSET      0x000001D8    /**< 1588 nanosecond
                                                       adjustment counter */
#define XZYNQ_EMACPS_1588_INC_OFFSET      0x000001DC    /**< 1588 nanosecond
                                                       increment counter */
#define XZYNQ_EMACPS_PTP_TXSEC_OFFSET     0x000001E0    /**< 1588 PTP transmit second
                                                       counter */
#define XZYNQ_EMACPS_PTP_TXNANOSEC_OFFSET 0x000001E4    /**< 1588 PTP transmit
                                                       nanosecond counter */
#define XZYNQ_EMACPS_PTP_RXSEC_OFFSET     0x000001E8    /**< 1588 PTP receive second
                                                       counter */
#define XZYNQ_EMACPS_PTP_RXNANOSEC_OFFSET 0x000001EC    /**< 1588 PTP receive
                                                       nanosecond counter */
#define XZYNQ_EMACPS_PTPP_TXSEC_OFFSET    0x000001F0    /**< 1588 PTP peer transmit
                                                       second counter */
#define XZYNQ_EMACPS_PTPP_TXNANOSEC_OFFSET 0x000001F4   /**< 1588 PTP peer transmit
                                                       nanosecond counter */
#define XZYNQ_EMACPS_PTPP_RXSEC_OFFSET    0x000001F8    /**< 1588 PTP peer receive
                                                       second counter */
#define XZYNQ_EMACPS_PTPP_RXNANOSEC_OFFSET 0x000001FC   /**< 1588 PTP peer receive
                                                       nanosecond counter */
#define XZYNQ_EMACPS_RXQBASE_UPPER_OFFSET  0x000004D4

#define XZYNQ_EMACPS_TXQBASE_UPPER_OFFSET  0x000004C8

#define XZYNQ_EMACPS_TX_Q1PTR_OFFSET       0x00000440   /**< Hold base address of
                                                       second transmit queue */
#define XZYNQ_EMACPS_RX_Q1PTR_OFFSET       0x00000480   /**< Hold base address of
                                                       second transmit queue */
#define XZYNQ_EMACPS_JUMBO_MAXLEN_OFFSET   0x00000048   /**< Maximum jumbo frame
                                                       size register */

/* Define some bit positions for registers. */

/** @name network control register bit definitions
 * @{
 */
#define XZYNQ_EMACPS_NWCTRL_ZEROPAUSETX_MASK 0x00000800 /**< Transmit zero quantum
                                                       pause frame */
#define XZYNQ_EMACPS_NWCTRL_PAUSETX_MASK     0x00000800 /**< Transmit pause frame */
#define XZYNQ_EMACPS_NWCTRL_HALTTX_MASK      0x00000400 /**< Halt transmission
                                                       after current frame */
#define XZYNQ_EMACPS_NWCTRL_STARTTX_MASK     0x00000200 /**< Start tx (tx_go) */

#define XZYNQ_EMACPS_NWCTRL_STATWEN_MASK     0x00000080 /**< Enable writing to
                                                       stat counters */
#define XZYNQ_EMACPS_NWCTRL_STATINC_MASK     0x00000040 /**< Increment statistic
                                                       registers */
#define XZYNQ_EMACPS_NWCTRL_STATCLR_MASK     0x00000020 /**< Clear statistic
                                                       registers */
#define XZYNQ_EMACPS_NWCTRL_MDEN_MASK        0x00000010 /**< Enable MDIO port */
#define XZYNQ_EMACPS_NWCTRL_TXEN_MASK        0x00000008 /**< Enable transmit */
#define XZYNQ_EMACPS_NWCTRL_RXEN_MASK        0x00000004 /**< Enable receive */
#define XZYNQ_EMACPS_NWCTRL_LOOPEN_MASK      0x00000002 /**< local loopback */
/*@}*/

/** @name network configuration register bit definitions
 * @{
 */
#define XZYNQ_EMACPS_NWCFG_BADPREAMBEN_MASK 0x20000000  /**< disable rejection of
                                                       non-standard preamble */
#define XZYNQ_EMACPS_NWCFG_IPDSTRETCH_MASK  0x10000000  /**< enable transmit IPG */
#define XZYNQ_EMACPS_NWCFG_SGMII_MODE_EN_MASK 0x08000000 /**< Use SGMII-compatible auto-neg */
#define XZYNQ_EMACPS_NWCFG_FCSIGNORE_MASK   0x04000000  /**< disable rejection of
                                                       FCS error */
#define XZYNQ_EMACPS_NWCFG_HDRXEN_MASK      0x02000000  /**< RX half duplex */
#define XZYNQ_EMACPS_NWCFG_RXCHKSUMEN_MASK  0x01000000  /**< enable RX checksum
                                                       offload */

#define XZYNQ_EMACPS_NWCFG_PAUSECOPYDI_MASK 0x00800000  /**< Do not copy pause
                                                       Frames to memory */
#define XZYNQ_EMACPS_NWCFG_DATABUS64_MASK   0x00200000  /**< Data bus width 64 */
#define XZYNQ_EMACPS_NWCFG_MDCCLKDIV(v)     ((v & 0x7) << XZYNQ_EMACPS_NWCFG_MDC_SHIFT_MASK)
     #define XZYNQ_EMACPS_NWCFG_MDCCLKDIV_8  0x0
     #define XZYNQ_EMACPS_NWCFG_MDCCLKDIV_16 0x1
     #define XZYNQ_EMACPS_NWCFG_MDCCLKDIV_32 0x2
     #define XZYNQ_EMACPS_NWCFG_MDCCLKDIV_48 0x3
#define XZYNQ_EMACPS_NWCFG_MDC_SHIFT_MASK   18          /**< shift bits for MDC */
#define XZYNQ_EMACPS_NWCFG_MDCCLKDIV_MASK   0x001C0000  /**< MDC Mask PCLK divisor */
#define XZYNQ_EMACPS_NWCFG_FCSREM_MASK      0x00020000  /**< Discard FCS from
                                                       received frames */
#define XZYNQ_EMACPS_NWCFG_LENGTHERRDSCRD_MASK 0x00010000
/**< RX length error discard */
#define XZYNQ_EMACPS_NWCFG_RXOFFS_MASK      0x0000C000  /**< RX buffer offset */
#define XZYNQ_EMACPS_NWCFG_PAUSEEN_MASK     0x00002000  /**< Enable pause RX */
#define XZYNQ_EMACPS_NWCFG_RETRYTESTEN_MASK 0x00001000  /**< Retry test */
#define XZYNQ_EMACPS_NWCFG_PCS_SELECT       0x00000800  /**< Enables TBI (instead of MII/GMII) */
#define XZYNQ_EMACPS_NWCFG_1000_MASK        0x00000400  /**< 1000 Mbps */
#define XZYNQ_EMACPS_NWCFG_EXTADDRMATCHEN_MASK 0x00000200
/**< External address match enable */
#define XZYNQ_EMACPS_NWCFG_1536RXEN_MASK    0x00000100  /**< Enable 1536 byte
                                                       frames reception */
#define XZYNQ_EMACPS_NWCFG_UCASTHASHEN_MASK 0x00000080  /**< Receive unicast hash
                                                       frames */
#define XZYNQ_EMACPS_NWCFG_MCASTHASHEN_MASK 0x00000040  /**< Receive multicast hash
                                                       frames */
#define XZYNQ_EMACPS_NWCFG_BCASTDI_MASK     0x00000020  /**< Do not receive
                                                       broadcast frames */
#define XZYNQ_EMACPS_NWCFG_COPYALLEN_MASK   0x00000010  /**< Copy all frames */
#define XZYNQ_EMACPS_NWCFG_JUMBO_MASK       0x00000008  /**< Jumbo frames */
#define XZYNQ_EMACPS_NWCFG_NVLANDISC_MASK   0x00000004  /**< Receive only VLAN
                                                       frames */
#define XZYNQ_EMACPS_NWCFG_FDEN_MASK        0x00000002  /**< full duplex */
#define XZYNQ_EMACPS_NWCFG_100_MASK         0x00000001  /**< 100 Mbps */
/*@}*/

/** @name network status register bit definitaions
 * @{
 */
#define XZYNQ_EMACPS_NWSR_MDIOIDLE_MASK     0x00000004  /**< PHY management idle */
#define XZYNQ_EMACPS_NWSR_MDIO_MASK         0x00000002  /**< Status of mdio_in */
/*@}*/

/** @name MAC address register word 1 mask
 * @{
 */
#define XZYNQ_EMACPS_LADDR_MACH_MASK        0x0000FFFF /**< Address bits[47:32]
                                                      bit[31:0] are in BOTTOM */
/*@}*/

/** @name DMA control register bit definitions
 * @{
 */
#define XZYNQ_EMACPS_DMACR_RXBUF_MASK      0x00FF0000   /**< Mask bit for RX buffer size */
#define XZYNQ_EMACPS_DMACR_RXBUF_SHIFT     16           /**< Shift bit for RX buffer size */
#define XZYNQ_EMACPS_DMACR_TCPCKSUM_MASK   0x00000800   /**< enable/disable TX checksum offload */
#define XZYNQ_EMACPS_DMACR_TXSIZE_MASK     0x00000400   /**< TX buffer memory size */
#define XZYNQ_EMACPS_DMACR_RXSIZE_MASK     0x00000300   /**< RX buffer memory size */
#define XZYNQ_EMACPS_DMACR_ENDIAN_MASK     0x00000080   /**< endian configuration */
#define XZYNQ_EMACPS_DMACR_BLENGTH_MASK    0x0000001F   /**< buffer burst length */
#define XZYNQ_EMACPS_DMACR_BW64_MASK       0x40000000   /**< use 64-bit descriptors */
/*@}*/

/** @name transmit status register bit definitions
 * @{
 */
#define XZYNQ_EMACPS_TXSR_HRESPNOK_MASK    0x00000100   /**< Transmit hresp not OK */
#define XZYNQ_EMACPS_TXSR_URUN_MASK        0x00000040   /**< Transmit underrun */
#define XZYNQ_EMACPS_TXSR_TXCOMPL_MASK     0x00000020   /**< Transmit completed OK */
#define XZYNQ_EMACPS_TXSR_BUFEXH_MASK      0x00000010   /**< Transmit buffs exhausted mid frame */
#define XZYNQ_EMACPS_TXSR_TXGO_MASK        0x00000008   /**< Status of go flag */
#define XZYNQ_EMACPS_TXSR_RXOVR_MASK       0x00000004   /**< Retry limit exceeded */
#define XZYNQ_EMACPS_TXSR_FRAMERX_MASK     0x00000002   /**< Collision tx frame */
#define XZYNQ_EMACPS_TXSR_USEDREAD_MASK    0x00000001   /**< TX buffer used bit set */

#define XZYNQ_EMACPS_TXSR_ERROR_MASK      (XZYNQ_EMACPS_TXSR_HRESPNOK_MASK | \
                       XZYNQ_EMACPS_TXSR_URUN_MASK | \
                       XZYNQ_EMACPS_TXSR_BUFEXH_MASK | \
                       XZYNQ_EMACPS_TXSR_RXOVR_MASK | \
                       XZYNQ_EMACPS_TXSR_FRAMERX_MASK | \
                       XZYNQ_EMACPS_TXSR_USEDREAD_MASK)
/*@}*/

/**
 * @name receive status register bit definitions
 * @{
 */
#define XZYNQ_EMACPS_RXSR_HRESPNOK_MASK    0x00000008   /**< Receive hresp not OK */
#define XZYNQ_EMACPS_RXSR_RXOVR_MASK       0x00000004   /**< Receive overrun */
#define XZYNQ_EMACPS_RXSR_FRAMERX_MASK     0x00000002   /**< Frame received OK */
#define XZYNQ_EMACPS_RXSR_BUFFNA_MASK      0x00000001   /**< RX buffer used bit set */

#define XZYNQ_EMACPS_RXSR_ERROR_MASK      (XZYNQ_EMACPS_RXSR_HRESPNOK_MASK | \
                       XZYNQ_EMACPS_RXSR_RXOVR_MASK | \
                       XZYNQ_EMACPS_RXSR_BUFFNA_MASK)
/*@}*/

/**
 * @name interrupts bit definitions
 * Bits definitions are same in XZYNQ_EMACPS_ISR_OFFSET,
 * XZYNQ_EMACPS_IER_OFFSET, XZYNQ_EMACPS_IDR_OFFSET, and XZYNQ_EMACPS_IMR_OFFSET
 * @{
 */
#define XZYNQ_EMACPS_IXR_PTPPSTX_MASK    0x02000000     /**< PTP Psync transmitted */
#define XZYNQ_EMACPS_IXR_PTPPDRTX_MASK   0x01000000     /**< PTP Pdelay_req transmitted */
#define XZYNQ_EMACPS_IXR_PTPSTX_MASK     0x00800000     /**< PTP Sync transmitted */
#define XZYNQ_EMACPS_IXR_PTPDRTX_MASK    0x00400000     /**< PTP Delay_req transmitted */
#define XZYNQ_EMACPS_IXR_PTPPSRX_MASK    0x00200000     /**< PTP Psync received */
#define XZYNQ_EMACPS_IXR_PTPPDRRX_MASK   0x00100000     /**< PTP Pdelay_req received */
#define XZYNQ_EMACPS_IXR_PTPSRX_MASK     0x00080000     /**< PTP Sync received */
#define XZYNQ_EMACPS_IXR_PTPDRRX_MASK    0x00040000     /**< PTP Delay_req received */
#define XZYNQ_EMACPS_IXR_PAUSETX_MASK    0x00004000     /**< Pause frame transmitted */
#define XZYNQ_EMACPS_IXR_PAUSEZERO_MASK  0x00002000     /**< Pause time has reached zero */
#define XZYNQ_EMACPS_IXR_PAUSENZERO_MASK 0x00001000     /**< Pause frame received */
#define XZYNQ_EMACPS_IXR_HRESPNOK_MASK   0x00000800     /**< hresp not ok */
#define XZYNQ_EMACPS_IXR_RXOVR_MASK      0x00000400     /**< Receive overrun occurred */
#define XZYNQ_EMACPS_IXR_TXCOMPL_MASK    0x00000080     /**< Frame transmitted ok */
#define XZYNQ_EMACPS_IXR_TXEXH_MASK      0x00000040     /**< Transmit err occurred or no buffers*/
#define XZYNQ_EMACPS_IXR_RETRY_MASK      0x00000020     /**< Retry limit exceeded */
#define XZYNQ_EMACPS_IXR_URUN_MASK       0x00000010     /**< Transmit underrun */
#define XZYNQ_EMACPS_IXR_TXUSED_MASK     0x00000008     /**< Tx buffer used bit read */
#define XZYNQ_EMACPS_IXR_RXUSED_MASK     0x00000004     /**< Rx buffer used bit read */
#define XZYNQ_EMACPS_IXR_FRAMERX_MASK    0x00000002     /**< Frame received ok */
#define XZYNQ_EMACPS_IXR_MGMNT_MASK      0x00000001     /**< PHY management complete */
#define XZYNQ_EMACPS_IXR_ALL_MASK        0xFFFFFFFF     /**< Everything! */

#define XZYNQ_EMACPS_IXR_TX_ERR_MASK    (XZYNQ_EMACPS_IXR_TXEXH_MASK |   \
                     XZYNQ_EMACPS_IXR_RETRY_MASK |           \
                     XZYNQ_EMACPS_IXR_URUN_MASK  |           \
                     XZYNQ_EMACPS_IXR_TXUSED_MASK)

#define XZYNQ_EMACPS_IXR_RX_ERR_MASK    (XZYNQ_EMACPS_IXR_HRESPNOK_MASK |   \
                     XZYNQ_EMACPS_IXR_RXUSED_MASK |          \
                     XZYNQ_EMACPS_IXR_RXOVR_MASK)

/*@}*/

/** @name PHY Maintenance bit definitions
 * @{
 */
#define XZYNQ_EMACPS_PHYMNTNC_OP_MASK    0x40020000     /**< operation mask bits */
#define XZYNQ_EMACPS_PHYMNTNC_OP_R_MASK  0x20000000     /**< read operation */
#define XZYNQ_EMACPS_PHYMNTNC_OP_W_MASK  0x10000000     /**< write operation */
#define XZYNQ_EMACPS_PHYMNTNC_ADDR_MASK  0x0F800000     /**< Address bits */
#define XZYNQ_EMACPS_PHYMNTNC_REG_MASK   0x007C0000     /**< register bits */
#define XZYNQ_EMACPS_PHYMNTNC_DATA_MASK  0x00000FFF     /**< data bits */
#define XZYNQ_EMACPS_PHYMNTNC_PHYAD_SHIFT_MASK   23     /**< Shift bits for PHYAD */
#define XZYNQ_EMACPS_PHYMNTNC_PHREG_SHIFT_MASK   18     /**< Shift bits for PHREG */
/*@}*/

/* Transmit buffer descriptor status words offset
 * @{
 */
#define XZYNQ_EMACPS_BD_ADDR_OFFSET      0x00000000 /**< word 0/addr of BDs */
#define XZYNQ_EMACPS_BD_STAT_OFFSET      0x00000004 /**< word 1/status of BDs */
#define XZYNQ_EMACPS_BD_ADDR_HIGH_OFFSET 0x00000008 /**< word 2/high part of BDs addr*/
#define XZYNQ_EMACPS_BD_RESERVED         0x0000000C /**< word 3/reserved */

/*
 * @}
 */

/* Transmit buffer descriptor status words bit positions.
 * Transmit buffer descriptor consists of two 32-bit registers,
 * the first - word0 contains a 32-bit address pointing to the location of
 * the transmit data.
 * The following register - word1, consists of various information to control
 * the emac transmit process.  After transmit, this is updated with status
 * information, whether the frame was transmitted OK or why it had failed.
 * @{
 */
#define XZYNQ_EMACPS_TXBUF_USED_MASK  0x80000000        /**< Used bit. */
#define XZYNQ_EMACPS_TXBUF_WRAP_MASK  0x40000000        /**< Wrap bit, last descriptor */
#define XZYNQ_EMACPS_TXBUF_RETRY_MASK 0x20000000        /**< Retry limit exceeded */
#define XZYNQ_EMACPS_TXBUF_URUN_MASK  0x10000000        /**< Transmit underrun occurred */
#define XZYNQ_EMACPS_TXBUF_EXH_MASK   0x08000000        /**< Buffers exhausted */
#define XZYNQ_EMACPS_TXBUF_TCP_MASK   0x04000000        /**< Late collision. */
#define XZYNQ_EMACPS_TXBUF_NOCRC_MASK 0x00010000        /**< No CRC */
#define XZYNQ_EMACPS_TXBUF_LAST_MASK  0x00008000        /**< Last buffer */
#define XZYNQ_EMACPS_TXBUF_LEN_MASK   0x00003FFF        /**< Mask for length field */
/*
 * @}
 */

/* Receive buffer descriptor status words bit positions.
 * Receive buffer descriptor consists of two 32-bit registers,
 * the first - word0 contains a 32-bit word aligned address pointing to the
 * address of the buffer. The lower two bits make up the wrap bit indicating
 * the last descriptor and the ownership bit to indicate it has been used by
 * the emac.
 * The following register - word1, contains status information regarding why
 * the frame was received (the filter match condition) as well as other
 * useful info.
 * @{
 */
#define XZYNQ_EMACPS_RXBUF_BCAST_MASK     0x80000000    /**< Broadcast frame */
#define XZYNQ_EMACPS_RXBUF_MULTIHASH_MASK 0x40000000    /**< Multicast hashed frame */
#define XZYNQ_EMACPS_RXBUF_UNIHASH_MASK   0x20000000    /**< Unicast hashed frame */
#define XZYNQ_EMACPS_RXBUF_EXH_MASK       0x08000000    /**< buffer exhausted */
#define XZYNQ_EMACPS_RXBUF_AMATCH_MASK    0x06000000    /**< Specific address
                                                       matched */
#define XZYNQ_EMACPS_RXBUF_IDFOUND_MASK   0x01000000    /**< Type ID matched */
#define XZYNQ_EMACPS_RXBUF_IDMATCH_MASK   0x00C00000    /**< ID matched mask */
#define XZYNQ_EMACPS_RXBUF_VLAN_MASK      0x00200000    /**< VLAN tagged */
#define XZYNQ_EMACPS_RXBUF_PRI_MASK       0x00100000    /**< Priority tagged */
#define XZYNQ_EMACPS_RXBUF_VPRI_MASK      0x000E0000    /**< Vlan priority */
#define XZYNQ_EMACPS_RXBUF_CFI_MASK       0x00010000    /**< CFI frame */
#define XZYNQ_EMACPS_RXBUF_EOF_MASK       0x00008000    /**< End of frame. */
#define XZYNQ_EMACPS_RXBUF_SOF_MASK       0x00004000    /**< Start of frame. */
#define XZYNQ_EMACPS_RXBUF_LEN_MASK       0x00003FFF    /**< Mask for length field */

#define XZYNQ_EMACPS_RXBUF_WRAP_MASK      0x00000002    /**< Wrap bit, last BD */
#define XZYNQ_EMACPS_RXBUF_NEW_MASK       0x00000001    /**< Used bit.. */
#define XZYNQ_EMACPS_RXBUF_ADD_MASK       0xFFFFFFFC    /**< Mask for address */

/* -------------------------------------
 * DMA
 * -------------------------------------
 */

#define XZYNQ_DMA_NS_BASE_ADDR        0xF8004000
#define XZYNQ_DMA_S_BASE_ADDR         0xF8003000
#define XZYNQ_DMA_REG_SIZE            0x1000

/* -------------------------------------
 * I2C
 * -------------------------------------
 */

#define XZYNQ_XIICPS1_BASE_ADDR        0xE0004000
#define XZYNQ_XIICPS2_BASE_ADDR        0xE0005000
#define XZYNQ_XIICPS_REG_SIZE          0x2C

/* -------------------------------------
 * GPIO
 * -------------------------------------
 */

#define XZYNQ_GPIO_BASE        0xE000A000
#define XZYNQ_GPIO_SIZE        0x300

/** @name Register offsets for the GPIO. Each register is 32 bits.
 *  @{
 */
#define XZYNQ_GPIOPS_DATA_LSW_OFFSET    0x000  /* Mask and Data Register LSW, WO */
#define XZYNQ_GPIOPS_DATA_MSW_OFFSET    0x004  /* Mask and Data Register MSW, WO */
#define XZYNQ_GPIOPS_DATA_OFFSET        0x040  /* Data Register, RW */
#define XZYNQ_GPIOPS_DIRM_OFFSET        0x204  /* Direction Mode Register, RW */
#define XZYNQ_GPIOPS_OUTEN_OFFSET       0x208  /* Output Enable Register, RW */
#define XZYNQ_GPIOPS_INTMASK_OFFSET     0x20C  /* Interrupt Mask Register, RO */
#define XZYNQ_GPIOPS_INTEN_OFFSET       0x210  /* Interrupt Enable Register, WO */
#define XZYNQ_GPIOPS_INTDIS_OFFSET      0x214  /* Interrupt Disable Register, WO*/
#define XZYNQ_GPIOPS_INTSTS_OFFSET      0x218  /* Interrupt Status Register, RO */
#define XZYNQ_GPIOPS_INTTYPE_OFFSET     0x21C  /* Interrupt Type Register, RW */
#define XZYNQ_GPIOPS_INTPOL_OFFSET      0x220  /* Interrupt Polarity Register, RW */
#define XZYNQ_GPIOPS_INTANY_OFFSET      0x224  /* Interrupt On Any Register, RW */
/* @} */


/* -------------------------------------------------------------------------
 * ARM / SMP
 * -------------------------------------------------------------------------
 */
#define XZYNQ_CPU1_START_ADDRESS    0xFFFFFFF0
#define XZYNQ_PL310_BASE            0xF8F02000

/* -------------------------------------------------------------------------
 * Clocks
 * -------------------------------------------------------------------------
 */
#define XZYNQ_PERIPH_CLOCK    50000000

/* -------------------------------------------------------------------------
 * Timers
 * -------------------------------------------------------------------------
 */
/* -------------------------------------------------------------------------
 * CPU Private timers
 * -------------------------------------------------------------------------
 */

#define XZYNQ_PRIVATE_TIMER_BASEADDR    0xF8F00600
#define XZYNQ_PRIVATE_TIMER_SIZE        0x10

/* Timer Load register */
#define XZYNQ_PRIVATE_TIMER_LOAD        0x00

/* Timer Counter register */
#define XZYNQ_PRIVATE_TIMER_COUNTER     0x04

/* Timer Control register */
#define XZYNQ_PRIVATE_TIMER_CTRL                   0x08
#define XZYNQ_PRIVATE_TIMER_CTRL_EN                (1 << 0)
#define XZYNQ_PRIVATE_TIMER_CTRL_AUTO_RELOAD       (1 << 1)
#define XZYNQ_PRIVATE_TIMER_CTRL_IRQ               (1 << 2)
#define XZYNQ_PRIVATE_TIMER_CTRL_PRESCALER_MSK     (0xFF << 8)

/* Timer Interrupt Status register */
#define XZYNQ_PRIVATE_TIMER_INT_STS                0x0C

/* -------------------------------------------------------------------------
 * MPCORE
 * -------------------------------------------------------------------------
 */
#define XZYNQ_MPCORE_BASE                   0xF8F00000
#define XZYNQ_MPCORE_LEN                    0x00002000
#define XZYNQ_MPCORE_FILTER_START_ADDR      0x00000040
#define XZYNQ_MPCORE_FILTER_END_ADDR        0x00000044
#define XZYNQ_MPCORE_FILTER_ADDR_MSK        0xFFF00000


/* -------------------------------------
 * Global timer
 * -------------------------------------
 */

#define XZYNQ_GLOBAL_TIMER_COUNTER_BASE                0xF8F00200
/* Size of the register area (used for mapping) */
#define XZYNQ_GLOBAL_TIMER_COUNTER_BASE_MAP_SIZE       0x610

#define XZYNQ_GLOBAL_TIMER_COUNTER_REG0                0x00
#define XZYNQ_GLOBAL_TIMER_COUNTER_REG1                0x04
#define XZYNQ_GLOBAL_TIMER_CONTROL_REG                 0x08
#define XZYNQ_GLOBAL_TIMER_INT_STS                     0x0C
#define XZYNQ_GLOBAL_TIMER_CMP_REG0                    0x10
#define XZYNQ_GLOBAL_TIMER_CMP_REG1                    0x14

#define XZYNQ_GLOBAL_TIMER_CONTROL_TIMER_EN            (1 << 0)
#define XZYNQ_GLOBAL_TIMER_CONTROL_CMP_EN              (1 << 1)
#define XZYNQ_GLOBAL_TIMER_CONTROL_IRQ                 (1 << 2)
#define XZYNQ_GLOBAL_TIMER_CONTROL_AE                  (1 << 3)
#define XZYNQ_GLOBAL_TIMER_CONTROL_PRESCALER_MSK       (0xFF << 8)

/* -------------------------------------
 * TTC timers
 * -------------------------------------
 */

#define XZYNQ_TTC0_BASE(timer)        (unsigned int)(0xF8001000 + 4 * timer)
#define XZYNQ_TTC1_BASE(timer)        (unsigned int)(0xF8002000 + 4 * timer)
#define XZYNQ_TTC_MAP_SIZE            0x84

/* Address used in the timer callout
 * Make sure it is consistent with the C code (cannot use the C definition
 * in assembly code).
 */
#define XZYNQ_TTC0_BASE_CALLOUT       0xF8001000


#define XZYNQ_TTC_CLOCK_CTRL          0x00
#define XZYNQ_TTC_COUNTER_CTRL        0x0C
#define XZYNQ_TTC_COUNTER_VAL         0x18
#define XZYNQ_TTC_INTR_VAL            0x24
#define XZYNQ_TTC_MATCH_1             0x30
#define XZYNQ_TTC_MATCH_2             0x3C
#define XZYNQ_TTC_MATCH_3             0x48
#define XZYNQ_TTC_ISR                 0x54
#define XZYNQ_TTC_IER                 0x60

#define XZYNQ_TTC_COUNTER_CTRL_DIS        (1 << 0)
#define XZYNQ_TTC_COUNTER_CTRL_INT        (1 << 1)
#define XZYNQ_TTC_COUNTER_CTRL_DEC        (1 << 2)
#define XZYNQ_TTC_COUNTER_CTRL_MATCH      (1 << 3)
#define XZYNQ_TTC_COUNTER_CTRL_RST        (1 << 4)
#define XZYNQ_TTC_COUNTER_CTRL_EN_WAVE    (1 << 5)
#define XZYNQ_TTC_COUNTER_CTRL_POL_WAVE   (1 << 6)

#define XZYNQ_TTC_CLOCK_CTRL_PS_EN        (1 << 0)

#define XZYNQ_TTC_IER_IXR_INTERVAL        (1 << 0)
#define XZYNQ_TTC_IER_IXR_MATCH_0         (1 << 1)
#define XZYNQ_TTC_IER_IXR_MATCH_1         (1 << 2)
#define XZYNQ_TTC_IER_IXR_MATCH_2         (1 << 3)
#define XZYNQ_TTC_IER_IXR_CNT_OVR         (1 << 4)



/* -------------------------------------------------------------------------
 * System level control registers (SLCR)
 * -------------------------------------------------------------------------
 */

#define XZYNQ_SLCR_BASE                    0xF8000000
#define XZYNQ_SLCR_LEN                     0x00001000
#define XZYNQ_SLCR_LOCK_OFFSET             0x00000004
#define XZYNQ_SLCR_LOCK_KEY                0x0000767B
#define XZYNQ_SLCR_UNLOCK_OFFSET           0x00000008
#define XZYNQ_SLCR_UNLOCK_KEY              0x0000DF0D
#define XZYNQ_SLCR_ARM_PLL_CTRL_REG        0x00000100
#define XZYNQ_SLCR_DDR_PLL_CTRL_REG        0x00000104
#define XZYNQ_SLCR_IO_PLL_CTRL_REG         0x00000108
#define XZYNQ_SLCR_ARM_CLK_CTRL_REG        0x00000120
#define XZYNQ_SLCR_APER_CLK_CTRL_REG       0x0000012C
#define XZYNQ_SLCR_GEM0_RCLK_CTRL_REG      0x00000138
#define XZYNQ_SLCR_GEM0_CLK_CTRL_REG       0x00000140
#define XZYNQ_SLCR_GEM1_RCLK_CTRL_REG      0x0000013C
#define XZYNQ_SLCR_GEM1_CLK_CTRL_REG       0x00000144
#define XZYNQ_SLCR_LQSPI_CLK_CTRL_REG      0x0000014C
#define XZYNQ_SLCR_SDIO_CLK_CTRL_REG       0x00000150
#define XZYNQ_SLCR_UART_CLK_CTRL_REG       0x00000154
#define XZYNQ_SLCR_SPI_CLK_CTRL_REG        0x00000158
#define XZYNQ_SLCR_CAN_CLK_CTRL_REG        0x0000015C
#define XZYNQ_SLCR_CAN_MIOCLK_CTRL_REG     0x00000160
#define XZYNQ_SLCR_CLK_621_TRUE_REG        0x000001C4
#define XZYNQ_SLCR_CAN_RST_CTRL_REG        0x00000220
#define XZYNQ_SLCR_FPGA_RST_CTRL_REG       0x00000240
#define XZYNQ_SLCR_LVL_SHFTR_EN_REG        0x00000900
#define XZYNQ_SLCR_OCM_CFG_REG             0x00000910

#define XZYNQ_SLCR_ARM_PLL_CTRL_PLL_FDIV_MASK          (0x7F << 12)
#define XZYNQ_SLCR_ARM_PLL_CTRL_PLL_FDIV_OFFSET        (12)

#define XZYNQ_SLCR_DDR_PLL_CTRL_PLL_FDIV_MASK          (0x7F << 12)
#define XZYNQ_SLCR_DDR_PLL_CTRL_PLL_FDIV_OFFSET        (12)

#define XZYNQ_SLCR_IO_PLL_CTRL_PLL_FDIV_MASK           (0x7F << 12)
#define XZYNQ_SLCR_IO_PLL_CTRL_PLL_FDIV_OFFSET         (12)

/* ARM CTRL register */
#define XZYNQ_SLCR_ARM_CLK_CTRL_SRCSEL_MASK            (0x3 << 4)
#define XZYNQ_SLCR_ARM_CLK_CTRL_SRCSEL_CPU_PLL1        (0x0 << 4)
#define XZYNQ_SLCR_ARM_CLK_CTRL_SRCSEL_CPU_PLL2        (0x1 << 4)
#define XZYNQ_SLCR_ARM_CLK_CTRL_SRCSEL_DDR_PLL         (0x2 << 4)
#define XZYNQ_SLCR_ARM_CLK_CTRL_SRCSEL_IO_PLL          (0x3 << 4)
#define XZYNQ_SLCR_ARM_CLK_CTRL_DIVISOR_OFFSET         (8)
#define XZYNQ_SLCR_ARM_CLK_CTRL_DIVISOR_MASK           (0x3F << 8)

/* Peripheral CTRL register */
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_SRCSEL_MASK         (0x3 << 4)
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_SRCSEL_DDR_PLL      (0x3 << 4)
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_SRCSEL_ARM_PLL      (0x2 << 4)
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_SRCSEL_IO_PLL       (0x0 << 4)
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_DIVISOR_OFFSET      (8)
#define XZYNQ_SLCR_PERIPH_CLK_CTRL_DIVISOR_MASK        (0x3F << 8)

/* CAN CTRL register */
#define XZYNQ_SLCR_CAN_CLK_CTRL_DIVISOR0_OFFSET        (8)
#define XZYNQ_SLCR_CAN_CLK_CTRL_DIVISOR1_OFFSET        (20)
#define XZYNQ_SLCR_CAN_CLK_CTRL_DIVISOR0_MASK          (0x3F << 8)
#define XZYNQ_SLCR_CAN_CLK_CTRL_DIVISOR1_MASK          (0x3F << 20)

#define XZYNQ_SLCR_CLK_CTRL_CLKACT_CAN0_OFFSET      (0)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_CAN1_OFFSET      (1)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_GEM0_OFFSET      (0)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_GEM1_OFFSET      (0)
#define XZYNQ_SLCR_RCLK_CTRL_CLKACT_GEM0_OFFSET     (0)
#define XZYNQ_SLCR_RCLK_CTRL_CLKACT_GEM1_OFFSET     (0)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_LQSPI_OFFSET     (0)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_SPI0_OFFSET      (0)
#define XZYNQ_SLCR_CLK_CTRL_CLKACT_SPI1_OFFSET      (1)

#define XZYNQ_SLCR_APER_CLK_CTRL_DMA     (1<<0)
#define XZYNQ_SLCR_APER_CLK_CTRL_USB0    (1<<2)
#define XZYNQ_SLCR_APER_CLK_CTRL_USB1    (1<<3)
#define XZYNQ_SLCR_APER_CLK_CTRL_GEM0    (1<<6)
#define XZYNQ_SLCR_APER_CLK_CTRL_GEM1    (1<<7)
#define XZYNQ_SLCR_APER_CLK_CTRL_SDI0    (1<<10)
#define XZYNQ_SLCR_APER_CLK_CTRL_SDI1    (1<<11)
#define XZYNQ_SLCR_APER_CLK_CTRL_SPI0    (1<<14)
#define XZYNQ_SLCR_APER_CLK_CTRL_SPI1    (1<<15)
#define XZYNQ_SLCR_APER_CLK_CTRL_CAN0    (1<<16)
#define XZYNQ_SLCR_APER_CLK_CTRL_CAN1    (1<<17)
#define XZYNQ_SLCR_APER_CLK_CTRL_I2C0    (1<<18)
#define XZYNQ_SLCR_APER_CLK_CTRL_I2C1    (1<<19)
#define XZYNQ_SLCR_APER_CLK_CTRL_UART0   (1<<20)
#define XZYNQ_SLCR_APER_CLK_CTRL_UART1   (1<<21)
#define XZYNQ_SLCR_APER_CLK_CTRL_GPIO    (1<<22)

#define XZYNQ_SLCR_PER_DIV_SHIFT        (8)
#define XZYNQ_SLCR_PER_DIV_MASK         (0x00003F00)
#define XZYNQ_SLCR_PER_DIV1_SHIFT       (20)
#define XZYNQ_SLCR_PER_DIV1_MASK        (0x03F00000)
#define XZYNQ_SLCR_PER_SRC_SHIFT        (4)
#define XZYNQ_SLCR_PER_SRC_IOPLL        (0)
#define XZYNQ_SLCR_PER_SRC_ARMPLL       (2)
#define XZYNQ_SLCR_PER_SRC_DDRPLL       (3)
#define XZYNQ_SLCR_PER_SRC_MASK         (0x00000030)
#define XZYNQ_SLCR_PER_CLK_ACT0         (0x00000001)
#define XZYNQ_SLCR_PER_CLK_ACT1         (0x00000002)

#define XZYNQ_SLCR_CAN0_RST_MASK        (1)
#define XZYNQ_SLCR_CAN1_RST_MASK        (2)

/* -------------------------------------------------------------------------
 * SDIO ports
 * -------------------------------------------------------------------------
 */
#define XZYNQ_SDIO0_BASEADDR        0xE0100000
#define XZYNQ_SDIO1_BASEADDR        0xE0101000

/* -------------------------------------------------------------------------
 * Serial ports
 * -------------------------------------------------------------------------
 */

/* UART base addresses */
#define    XZYNQ_XUARTPS_UART0_BASE        0xE0000000
#define    XZYNQ_XUARTPS_UART1_BASE        0xE0001000

#define    XZYNQ_XUARTPS_UART_SIZE         0x1000

/* UART registers */
#define XZYNQ_XUARTPS_CR_REG          0x00 /* UART Control register */
#define XZYNQ_XUARTPS_MR_REG          0x04 /* UART Mode register */
#define XZYNQ_XUARTPS_IER_REG         0x08 /* Interrupt Enable register */
#define XZYNQ_XUARTPS_IDR_REG         0x0C /* Interrupt Disable register */
#define XZYNQ_XUARTPS_IMR_REG         0x10 /* Interrupt Mask register */
#define XZYNQ_XUARTPS_ISR_REG         0x14 /* Channel Int Status register*/
#define XZYNQ_XUARTPS_BAUDGEN_REG     0x18 /* Baud rate divider register */
#define XZYNQ_XUARTPS_RXTOUT_REG      0x1C /* Receiver timeout register */
#define XZYNQ_XUARTPS_RXWM_REG        0x20 /* Receiver FIFO trigger level */
#define XZYNQ_XUARTPS_MODEMCR_REG     0x24 /* Modem control register */
#define XZYNQ_XUARTPS_MODEMSR_REG     0x28 /* Modem status register */
#define XZYNQ_XUARTPS_SR_REG          0x2C /* Channel status register */
#define XZYNQ_XUARTPS_FIFO_REG        0x30 /* Transmit and Receive FIFO */
#define XZYNQ_XUARTPS_BAUDDIV_REG     0x34 /* Baud Rate Count Register */
#define XZYNQ_XUARTPS_FLOWDEL_REG     0x38 /* Flow Control delay register */
#define XZYNQ_XUARTPS_TXWM_REG        0x44 /* Transmitter timeout register */

/* UART Control register */
#define XZYNQ_XUARTPS_CR_STOPBRK     0x00000100  /* Stop transmission of break */
#define XZYNQ_XUARTPS_CR_STARTBRK    0x00000080  /* Set break */
#define XZYNQ_XUARTPS_CR_TORST       0x00000040  /* RX timeout counter restart */
#define XZYNQ_XUARTPS_CR_TX_DIS      0x00000020  /* TX disabled. */
#define XZYNQ_XUARTPS_CR_TX_EN       0x00000010  /* TX enabled */
#define XZYNQ_XUARTPS_CR_RX_DIS      0x00000008  /* RX disabled. */
#define XZYNQ_XUARTPS_CR_RX_EN       0x00000004  /* RX enabled */
#define XZYNQ_XUARTPS_CR_EN_DIS_MASK 0x0000003C  /* Enable/disable Mask */
#define XZYNQ_XUARTPS_CR_TXRST       0x00000002  /* TX logic reset */
#define XZYNQ_XUARTPS_CR_RXRST       0x00000001  /* RX logic reset */

/* UART Mode register */
#define XZYNQ_XUARTPS_MR_CCLK             0x00000400  /* Input clock selection */
#define XZYNQ_XUARTPS_MR_CHMODE_R_LOOP    0x00000300  /* Remote loopback mode */
#define XZYNQ_XUARTPS_MR_CHMODE_L_LOOP    0x00000200  /* Local loopback mode */
#define XZYNQ_XUARTPS_MR_CHMODE_ECHO      0x00000100  /* Auto echo mode */
#define XZYNQ_XUARTPS_MR_CHMODE_NORM      0x00000000  /* Normal mode */
#define XZYNQ_XUARTPS_MR_CHMODE_SHIFT              8  /* Mode shift */
#define XZYNQ_XUARTPS_MR_CHMODE_MASK      0x00000300  /* Mode mask */
#define XZYNQ_XUARTPS_MR_STOPMODE_2_BIT   0x00000080  /* 2 stop bits */
#define XZYNQ_XUARTPS_MR_STOPMODE_1_5_BIT 0x00000040  /* 1.5 stop bits */
#define XZYNQ_XUARTPS_MR_STOPMODE_1_BIT   0x00000000  /* 1 stop bit */
#define XZYNQ_XUARTPS_MR_STOPMODE_SHIFT            6  /* Stop bits setting shift */
#define XZYNQ_XUARTPS_MR_STOPMODE_MASK    0x000000A0  /* Stop bits setting mask */
#define XZYNQ_XUARTPS_MR_PARITY_NONE      0x00000020  /* No parity mode */
#define XZYNQ_XUARTPS_MR_PARITY_MARK      0x00000018  /* Mark parity mode */
#define XZYNQ_XUARTPS_MR_PARITY_SPACE     0x00000010  /* Space parity mode */
#define XZYNQ_XUARTPS_MR_PARITY_ODD       0x00000008  /* Odd parity mode */
#define XZYNQ_XUARTPS_MR_PARITY_EVEN      0x00000000  /* Even parity mode */
#define XZYNQ_XUARTPS_MR_PARITY_SHIFT              3  /* Parity setting shift */
#define XZYNQ_XUARTPS_MR_PARITY_MASK      0x00000038  /* Parity mask */
#define XZYNQ_XUARTPS_MR_CHARLEN_6_BIT    0x00000006  /* 6 bits data */
#define XZYNQ_XUARTPS_MR_CHARLEN_7_BIT    0x00000004  /* 7 bits data */
#define XZYNQ_XUARTPS_MR_CHARLEN_8_BIT    0x00000000  /* 8 bits data */
#define XZYNQ_XUARTPS_MR_CHARLEN_SHIFT             1  /* data Length setting shift */
#define XZYNQ_XUARTPS_MR_CHARLEN_MASK     0x00000006  /* Data length mask. */
#define XZYNQ_XUARTPS_MR_CLKSEL           0x00000001  /* Input clock selection */

/* Interrupt registers */
#define XZYNQ_XUARTPS_IXR_TXOVR   0x00001000    /* TX Overrun error interrupt */
#define XZYNQ_XUARTPS_IXR_TXNFULL 0x00000800    /* TX FIFO near full interrupt */
#define XZYNQ_XUARTPS_IXR_TRIG    0x00000400    /* TX FIFO trigger interrupt */
#define XZYNQ_XUARTPS_IXR_DMS     0x00000200    /* Modem status change interrupt */
#define XZYNQ_XUARTPS_IXR_TOUT    0x00000100    /* Timeout error interrupt */
#define XZYNQ_XUARTPS_IXR_PARITY  0x00000080    /* Parity error interrupt */
#define XZYNQ_XUARTPS_IXR_FRAMING 0x00000040    /* Framing error interrupt */
#define XZYNQ_XUARTPS_IXR_RXOVR   0x00000020    /* RX Overrun error interrupt */
#define XZYNQ_XUARTPS_IXR_TXFULL  0x00000010    /* TX FIFO full interrupt. */
#define XZYNQ_XUARTPS_IXR_TXEMPTY 0x00000008    /* TX FIFO empty interrupt. */
#define XZYNQ_XUARTPS_IXR_RXFULL  0x00000004    /* RX FIFO full interrupt. */
#define XZYNQ_XUARTPS_IXR_RXEMPTY 0x00000002    /* RX FIFO empty interrupt. */
#define XZYNQ_XUARTPS_IXR_RTRIG   0x00000001    /* RX FIFO trigger interrupt. */
#define XZYNQ_XUARTPS_IXR_MASK    0x00001FFF    /* Valid bit mask */

/* Baud rate divider register */
#define XZYNQ_XUARTPS_BAUDGEN_DISABLE 0x00000000    /* Disable clock */
#define XZYNQ_XUARTPS_BAUDGEN_MASK    0x0000FFFF    /* Valid bits mask */

/* Receiver timeout register */
#define XZYNQ_XUARTPS_RXTOUT_DISABLE    0x00000000  /* Disable time out */
#define XZYNQ_XUARTPS_RXTOUT_MASK       0x000000FF  /* Valid bits mask */

/* Receiver FIFO trigger level */
#define XZYNQ_XUARTPS_RXWM_DISABLE 0x00000000  /* Disable RX trigger interrupt */
#define XZYNQ_XUARTPS_RXWM_MASK    0x0000001F  /* Valid bits mask */

/* Modem control register */
#define XZYNQ_XUARTPS_MODEMCR_FCM 0x00000010     /* Flow control mode */
#define XZYNQ_XUARTPS_MODEMCR_RTS 0x00000002     /* Request to send */
#define XZYNQ_XUARTPS_MODEMCR_DTR 0x00000001     /* Data terminal ready */

/* Modem status register */
#define XZYNQ_XUARTPS_MODEMSR_FCMS  0x00000100  /* Flow control mode (FCMS) */
#define XZYNQ_XUARTPS_MODEMSR_DCD   0x00000080  /* Complement of DCD input */
#define XZYNQ_XUARTPS_MODEMSR_RI    0x00000040  /* Complement of RI input */
#define XZYNQ_XUARTPS_MODEMSR_DSR   0x00000020  /* Complement of DSR input */
#define XZYNQ_XUARTPS_MODEMSR_CTS   0x00000010  /* Complement of CTS input */
#define XZYNQ_XUARTPS_MODEMSR_DCDX  0x00000008  /* Delta DCD indicator */
#define XZYNQ_XUARTPS_MODEMSR_RIX   0x00000004  /* Change of RI */
#define XZYNQ_XUARTPS_MODEMSR_DSRX  0x00000002  /* Change of DSR */
#define XZYNQ_XUARTPS_MODEMSR_CTSX  0x00000001  /* Change of CTS */

/* Channel status register */
#define XZYNQ_XUARTPS_SR_FLOWDEL  0x00001000     /* RX FIFO fill over flow delay */
#define XZYNQ_XUARTPS_SR_TACTIVE  0x00000800     /* TX active */
#define XZYNQ_XUARTPS_SR_RACTIVE  0x00000400     /* RX active */
#define XZYNQ_XUARTPS_SR_DMS      0x00000200     /* Delta modem status change */
#define XZYNQ_XUARTPS_SR_TOUT     0x00000100     /* RX timeout */
#define XZYNQ_XUARTPS_SR_PARITY   0x00000080     /* RX parity error */
#define XZYNQ_XUARTPS_SR_FRAME    0x00000040     /* RX frame error */
#define XZYNQ_XUARTPS_SR_OVER     0x00000020     /* RX overflow error */
#define XZYNQ_XUARTPS_SR_TXFULL   0x00000010     /* TX FIFO full */
#define XZYNQ_XUARTPS_SR_TXEMPTY  0x00000008     /* TX FIFO empty */
#define XZYNQ_XUARTPS_SR_RXFULL   0x00000004     /* RX FIFO full */
#define XZYNQ_XUARTPS_SR_RXEMPTY  0x00000002     /* RX FIFO empty */
#define XZYNQ_XUARTPS_SR_RXOVR    0x00000001     /* RX FIFO fill over trigger */

/* Flow Control delay register */
#define XZYNQ_XUARTPS_FLOWDEL_MASK    XZYNQ_XUARTPS_RXWM_MASK    /* Valid bit mask */

/* -------------------------------------------------------------------------
 * CAN
 * -------------------------------------------------------------------------
 */
#define XZYNQ_CAN0_BASEADDR            0xE0008000
#define XZYNQ_CAN1_BASEADDR            0xE0009000

/* -------------------------------------------------------------------------
 * Watchdog
 * -------------------------------------------------------------------------
 */
#define XZYNQ_WDT_BASEADDR            0xF8005000
#define XZYNQ_WDT_SIZE                0x10
#define XZYNQ_WDT_CLOCK_FREQ          133000000

#define XZYNQ_WDT_ZMR_OFFSET        0x0 /**< Zero Mode Register */
#define XZYNQ_WDT_CCR_OFFSET        0x4 /**< Counter Control Register */
#define XZYNQ_WDT_RESTART_OFFSET    0x8 /**< Restart Register */
#define XZYNQ_WDT_SR_OFFSET         0xC /**< Status Register */

/* Register MODE */
#define XZYNQ_WDT_ZMR_WDEN_MASK     0x00000001    /**< enable the WDT */
#define XZYNQ_WDT_ZMR_RSTEN_MASK    0x00000002    /**< enable the reset output */
#define XZYNQ_WDT_ZMR_IRQEN_MASK    0x00000004    /**< enable the IRQ output */

#define XZYNQ_WDT_ZMR_RSTLN_MASK    0x00000070    /**< set length of reset pulse */
#define XZYNQ_WDT_ZMR_RSTLN_SHIFT   4             /**< shift for reset pulse */

#define XZYNQ_WDT_ZMR_IRQLN_MASK    0x00000180    /**< set length of interrupt pulse */
#define XZYNQ_WDT_ZMR_IRQLN_SHIFT   7             /**< shift for interrupt pulse */

#define XZYNQ_WDT_ZMR_ZKEY_MASK     0x00FFF000    /**< mask for writing access key */
#define XZYNQ_WDT_ZMR_ZKEY          0xABC         /**< access key, 0xABC */
#define XZYNQ_WDT_ZMR_ZKEY_SHIFT    12
#define XZYNQ_WDT_ZMR_ZKEY_VAL      (XZYNQ_WDT_ZMR_ZKEY<<XZYNQ_WDT_ZMR_ZKEY_SHIFT)

/* Register CONTROL */
#define XZYNQ_WDT_CCR_CLKSEL_MASK    0x00000003    /**< counter clock prescale */

#define XZYNQ_WDT_CCR_PSCALE_0008    0x00000000    /**< divide clock by 8 */
#define XZYNQ_WDT_CCR_PSCALE_0064    0x00000001    /**< divide clock by 64 */
#define XZYNQ_WDT_CCR_PSCALE_0512    0x00000002    /**< divide clock by 512 */
#define XZYNQ_WDT_CCR_PSCALE_4096    0x00000003    /**< divide clock by 4096 */

#define XZYNQ_WDT_CCR_CRV_MASK       0x00003FFC    /**< counter reset value */
#define XZYNQ_WDT_CCR_CRV_SHIFT      2             /**< shift for writing value */

#define XZYNQ_WDT_CCR_CKEY_MASK      0x03FFC000    /**< mask for writing access key */
#define XZYNQ_WDT_CCR_CKEY           0x00000248    /**< access key, 0x248 */
#define XZYNQ_WDT_CCR_CKEY_SHIFT     14
#define XZYNQ_WDT_CCR_CKEY_VAL       (XZYNQ_WDT_CCR_CKEY<<XZYNQ_WDT_CCR_CKEY_SHIFT)

/* Register RESTART */
#define XZYNQ_WDT_RESTART_KEY_VAL    0x00001999    /**< valid key */

/* Register STATUS */
#define XZYNQ_WDT_SR_WDZ_MASK        0x00000001    /**< time out occured */

/* -------------------------------------------------------------------------
 * Device Config
 * -------------------------------------------------------------------------
 */
#define XZYNQ_DEVCFG_BASEADDR            0xF8007000
#define XZYNQ_DEVCFG_SIZE                0x1000

/* Register offsets */
#define XZYNQ_DEVCFG_CTRL_OFFSET         0x00          /* Control Register */
#define XZYNQ_DEVCFG_LOCK_OFFSET         0x04          /* Lock Register. Used to lock changes in the control reg */
#define XZYNQ_DEVCFG_UNLOCK_OFFSET       0x34          /* Unlock Register. Write the unlock key to access the devcfg interface */

#define XZYNQ_DEVCFG_UNLOCK_KEY          0x757BDF0D    /*Write to Unlock Register to use the devcfg interface */

/* Return Codes */
#define XZYNQ_SUCCESS    0
#define XZYNQ_FAILURE    1

/* -------------------------------------------------------------------------
 * FPGA
 * Part of the Device Config Interface. All Offsets are based on the devcfg
 * base address.
 * -------------------------------------------------------------------------
 */

/* Register offsets */
#define XZYNQ_FPGA_CTRL_OFFSET            0x00 /*Control Register*/
#define XZYNQ_FPGA_LOCK_OFFSET            0x04 /* Lock Register */
#define XZYNQ_FPGA_CFG_OFFSET             0x08 /* Configuration Register */
#define XZYNQ_FPGA_INT_STS_OFFSET         0x0C /* Interrupt Status Register */
#define XZYNQ_FPGA_INT_MASK_OFFSET        0x10 /* Interrupt Mask Register */
#define XZYNQ_FPGA_STATUS_OFFSET          0x14 /* Status Register */
#define XZYNQ_FPGA_DMA_SRC_ADDR_OFFSET    0x18 /* DMA Source Address Register */
#define XZYNQ_FPGA_DMA_DEST_ADDR_OFFSET   0x1C /* DMA Destination Address Reg */
#define XZYNQ_FPGA_DMA_SRC_LEN_OFFSET     0x20 /* DMA Source Transfer Length */
#define XZYNQ_FPGA_DMA_DEST_LEN_OFFSET    0x24 /* DMA Destination Transfer */
#define XZYNQ_FPGA_ROM_SHADOW_OFFSET      0x28 /* DMA ROM Shadow Register */
#define XZYNQ_FPGA_MULTIBOOT_ADDR_OFFSET  0x2C /* Multi BootAddress Pointer */
#define XZYNQ_FPGA_SW_ID_OFFSET           0x30 /* Software ID Register */
#define XZYNQ_FPGA_UNLOCK_OFFSET          0x34 /* Unlock Register */
#define XZYNQ_FPGA_MCTRL_OFFSET           0x80 /* Miscellaneous Control Reg */

/* Control Masks */
#define XZYNQ_FPGA_CTRL_FORCE_RST_MASK         0x80000000 /* Force  into secure Reset */
#define XZYNQ_FPGA_CTRL_PCFG_PROG_B_MASK       0x40000000 /* Program signal to reset FPGA */
#define XZYNQ_FPGA_CTRL_PCAP_PR_MASK           0x08000000 /* Enable PCAP for PR */
#define XZYNQ_FPGA_CTRL_PCAP_MODE_MASK         0x04000000 /* Enable PCAP */
#define XZYNQ_FPGA_CTRL_PCAP_RATE_EN_MASK      0x02000000 /* Enable PCAP send data to FPGA every 4 PCAP cycles */
#define XZYNQ_FPGA_CTRL_MULTIBOOT_EN_MASK      0x01000000 /* Multiboot Enable */
#define XZYNQ_FPGA_CTRL_JTAG_CHAIN_DIS_MASK    0x00800000 /* JTAG Chain Disable */
#define XZYNQ_FPGA_CTRL_USER_MODE_MASK         0x00008000 /* User Mode Mask */
#define XZYNQ_FPGA_CTRL_PCFG_AES_EN_MASK       0x00000E00 /* AES Enable Mask */
#define XZYNQ_FPGA_CTRL_SEU_EN_MASK            0x00000100 /* SEU Enable Mask */
#define XZYNQ_FPGA_CTRL_SEC_EN_MASK            0x00000080 /* Secure/Non Secure status mask */
#define XZYNQ_FPGA_CTRL_SPNIDEN_MASK           0x00000040 /* Secure Non Invasive debug enable */
#define XZYNQ_FPGA_CTRL_SPIDEN_MASK            0x00000020 /* Secure Invasive Debug Enable */
#define XZYNQ_FPGA_CTRL_NIDEN_MASK             0x00000010 /* Non-Invasive Debug Enable */
#define XZYNQ_FPGA_CTRL_DBGEN_MASK             0x00000008 /* Invasive Debug Enable */
#define XZYNQ_FPGA_CTRL_DAP_EN_MASK            0x00000007 /* DAP Enable Mask */

/* Lock Masks */
#define XZYNQ_FPGA_LOCK_AES_EN_MASK     0x00000008 /* Lock AES_EN update */
#define XZYNQ_FPGA_LOCK_SEU_MASK        0x00000004 /* Lock SEU_En update */
#define XZYNQ_FPGA_LOCK_SEC_MASK        0x00000002 /* Lock SEC_EN and USER_MODE */
#define XZYNQ_FPGA_LOCK_DBG_MASK        0x00000001 /* This bit locks security config including: DAP_En, DBGEN, NIDEN, SPNIEN */

/* Config Masks */
#define XZYNQ_FPGA_CFG_RFIFO_TH_MASK           0x00000C00 /* Read FIFO Threshold Mask */
#define XZYNQ_FPGA_CFG_WFIFO_TH_MASK           0x00000300 /* Write FIFO Threshold Mask */
#define XZYNQ_FPGA_CFG_RCLK_EDGE_MASK          0x00000080 /* Read data active clock edge */
#define XZYNQ_FPGA_CFG_WCLK_EDGE_MASK          0x00000040 /* Write data active clock edge */
#define XZYNQ_FPGA_CFG_DISABLE_SRC_INC_MASK    0x00000020 /* Disable Source address increment mask */
#define XZYNQ_FPGA_CFG_DISABLE_DST_INC_MASK    0x00000010 /* Disable Destination address increment mask */
#define XZYNQ_FPGA_CFG_RD_ISSU_MASK            0x00000008 /* AXI read issuing capability */
#define XZYNQ_FPGA_CFG_WR_ISSU_MASK            0x00000004 /* AXI write issuing capability */
#define XZYNQ_FPGA_CFG_RDLEN_MASK              0x00000002 /* AXI Read burst length  Mask */
#define XZYNQ_FPGA_CFG_WRLEN_MASK              0x00000001 /* AXI write burst length  Mask */

/* Status Masks */
#define XZYNQ_FPGA_STATUS_DMA_CMD_Q_F_MASK            0x80000000 /* DMA command queue full */
#define XZYNQ_FPGA_STATUS_DMA_CMD_Q_E_MASK            0x40000000 /* DMA command queue empty */
#define XZYNQ_FPGA_STATUS_DMA_DONE_CNT_MASK           0x30000000 /* Number of completed DMA transfers */
#define XZYNQ_FPGA_STATUS_RX_FIFO_LVL_MASK            0x01F000000 /* Rx FIFO level */
#define XZYNQ_FPGA_STATUS_TX_FIFO_LVL_MASK            0x0007F000  /* Tx FIFO level */
#define XZYNQ_FPGA_STATUS_SECURE_RST_MASK             0x00000080  /* Secure Reset POR Status */
#define XZYNQ_FPGA_STATUS_ILLEGAL_APB_ACCESS_MASK     0x00000040 /* Illegal APB access */
#define XZYNQ_FPGA_STATUS_PCFG_INIT_MASK              0x00000010 /* FPGA Init Status */
#define XZYNQ_FPGA_STATUS_EFUSE_SW_RESERVE_MASK       0x00000008 /* Efuse Reserved Status */
#define XZYNQ_FPGA_STATUS_EFUSE_SEC_EN_MASK           0x00000004 /* Efuse Security Enable Status */
#define XZYNQ_FPGA_STATUS_EFUSE_JTAG_DIS_MASK         0x00000002 /* EFuse JTAG Disable status */
#define XZYNQ_FPGA_STATUS_SECURE_DIS_MASK             0x00000001 /* Security Disable Status */

#define XZYNQ_FPGA_DMA_LEN_MASK        0x7FFFFFF /* Length Mask */

/* Miscellaneous Control Masks */
#define XZYNQ_FPGA_MCTRL_PCAP_LPBK_MASK      0x00000010 /* PCAP loopback mask */
#define XZYNQ_FPGA_MCTRL_RFIFO_FLUSH_MASK    0x00000002 /* Flush Rx FIFO */
#define XZYNQ_FPGA_MCTRL_WFIFO_FLUSH_MASK    0x00000001 /* Flush Tx FIFO */

/*Interrupt Masks */
#define XZYNQ_FPGA_IXR_AXI_WTO_MASK         0x00800000 /**< AXI Write Address or Data or response timeout */
#define XZYNQ_FPGA_IXR_AXI_WERR_MASK        0x00400000 /**< AXI Write response error */
#define XZYNQ_FPGA_IXR_AXI_RTO_MASK         0x00200000 /**< AXI Read Address or response timeout */
#define XZYNQ_FPGA_IXR_AXI_RERR_MASK        0x00100000 /**< AXI Read response error */
#define XZYNQ_FPGA_IXR_RX_FIFO_OV_MASK      0x00040000 /**< Rx FIFO Overflow */
#define XZYNQ_FPGA_IXR_WR_FIFO_LVL_MASK     0x00020000 /**< Tx FIFO less than threshold */
#define XZYNQ_FPGA_IXR_RD_FIFO_LVL_MASK     0x00010000 /**< Rx FIFO greater than threshold */
#define XZYNQ_FPGA_IXR_DMA_CMD_ERR_MASK     0x00008000 /**< Illegal DMA command */
#define XZYNQ_FPGA_IXR_DMA_Q_OV_MASK        0x00004000 /**< DMA command queue overflow */
#define XZYNQ_FPGA_IXR_DMA_DONE_MASK        0x00002000 /**< DMA Command Done */
#define XZYNQ_FPGA_IXR_D_P_DONE_MASK        0x00001000 /**< DMA and PCAP transfers Done */
#define XZYNQ_FPGA_IXR_P2D_LEN_ERR_MASK     0x00000800 /**< PCAP to DMA transfer length error */
#define XZYNQ_FPGA_IXR_PCFG_HMAC_ERR_MASK   0x00000040 /**< HMAC error mask */
#define XZYNQ_FPGA_IXR_PCFG_SEU_ERR_MASK    0x00000020 /**< SEU Error mask */
#define XZYNQ_FPGA_IXR_PCFG_POR_B_MASK      0x00000010 /**< FPGA POR mask */
#define XZYNQ_FPGA_IXR_PCFG_CFG_RST_MASK    0x00000008 /**< FPGA Reset mask */
#define XZYNQ_FPGA_IXR_PCFG_DONE_MASK       0x00000004 /**< Done Signal  Mask */
#define XZYNQ_FPGA_IXR_PCFG_INIT_PE_MASK    0x00000002 /**< Detect Positive edge of Init Signal */
#define XZYNQ_FPGA_IXR_PCFG_INIT_NE_MASK    0x00000001 /**< Detect Negative edge of Init Signal */
#define XZYNQ_FPGA_IXR_ERROR_FLAGS_MASK     (XZYNQ_FPGA_IXR_AXI_WTO_MASK | \
                        XZYNQ_FPGA_IXR_AXI_WERR_MASK | \
                        XZYNQ_FPGA_IXR_AXI_RTO_MASK |  \
                        XZYNQ_FPGA_IXR_AXI_RERR_MASK | \
                        XZYNQ_FPGA_IXR_RX_FIFO_OV_MASK | \
                        XZYNQ_FPGA_IXR_DMA_CMD_ERR_MASK |\
                        XZYNQ_FPGA_IXR_DMA_Q_OV_MASK |   \
                        XZYNQ_FPGA_IXR_P2D_LEN_ERR_MASK |\
                        XZYNQ_FPGA_IXR_PCFG_HMAC_ERR_MASK)
#define XZYNQ_FPGA_IXR_ALL_MASK            0x00F7F8EF


#define XZYNQ_FPGA_DMA_INVALID_ADDRESS     0xFFFFFFFF

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/arm/xzynq.h $ $Rev: 922478 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/arm/xzynq.h $ $Rev: 922478 $" )
#endif
#endif
