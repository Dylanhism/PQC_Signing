/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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


#ifndef GENET_REG_H_
#define GENET_REG_H_

/*BCM GENET base address*/
#define GENET_BASE_ADDR             0xFD580000
#define GENET_REG_SIZE              0x6000

#define GENET_ENET_IRQ0             189
#define GENET_ENET_IRQ1             190

/* genet SYS reg and bits */
#define GENET_SYS_PORT_CNTRL            0x04
#define  GENET_SYS_PORT_CNTRL_EXT_GPHY  3
#define GENET_SYS_RBUF_FLUSH_CNTRL      0x08
#define GENET_SYS_TBUF_FLUSH_CNTRL      0x0C

/* genet EXT reg and bits */
#define GENET_EXT_RGMII_OOB_CNTRL       0x8C
#define GENET_EXT_RGMII_OOB_CNTRL_LINK              (1 << 4)
#define GENET_EXT_RGMII_OOB_CNTRL_RGMII_MODE_EN     (1 << 6)
#define GENET_EXT_RGMII_OOB_CNTRL_ID_MODE_DIS       (1 << 16)

#define GENET_EXT_GPHY_CNTRL            0x9C

/* genet L2 INTR 0 reg */
#define GENET_INTRL2_0_CPU_STAT         0x200
#define GENET_INTRL2_0_CPU_SET          0x204
#define GENET_INTRL2_0_CPU_CLEAR        0x208
#define GENET_INTRL2_0_CPU_MASK_STATUS  0x20C
#define GENET_INTRL2_0_CPU_MASK_SET     0x210
#define GENET_INTRL2_0_CPU_MASK_CLEAR   0x214

/* genet L2 INTR 1 reg */
#define GENET_INTRL2_1_CPU_STAT         0x240
#define GENET_INTRL2_1_CPU_SET          0x244
#define GENET_INTRL2_1_CPU_CLEAR        0x248
#define GENET_INTRL2_1_CPU_MASK_STATUS  0x24C
#define GENET_INTRL2_1_CPU_MASK_SET     0x250
#define GENET_INTRL2_1_CPU_MASK_CLEAR   0x254

/* genet INTR bits */
#define GENET_INTRL2_0_LINK_UP          (1 << 4)
#define GENET_INTRL2_0_LINK_DOWN        (1 << 5)
#define GENET_INTRL2_RX_DONE            (1 << 13)
#define GENET_INTRL2_TX_DONE            (1 << 16)

/* genet RBUF reg and bits */
#define GENET_RBUF_CNTRL                0x300
#define GENET_RBUF_CNTRL_STATUS_64B_EN  (1 << 0)
#define GENET_RBUF_CNTRL_ALIGN_2B       (1 << 1)

#define GENET_RBUF_CHK_CNTRL            0x314
#define GENET_RBUF_CHK_CNTRL_RXCHK_EN   (1 << 0)
#define GENET_RBUF_CHK_CNTRL_SKIP_FCS   (1 << 4)

/* genet TBUF reg and bits */
#define GENET_TBUF_CNTRL                0x600
#define GENET_TBUF_CNTRL_STATUS_64B_EN  (1 << 0)

/* genet UMAC and command */
#define GENET_UMAC_CMD                  0x808
#define GENET_UMAC_CMD_TX_ENA           (1 << 0)
#define GENET_UMAC_CMD_RX_ENA           (1 << 1)
#define GENET_UMAC_CMD_RX_PAUSE_IGNORE  (1 << 8)
#define GENET_UMAC_CMD_HD_ENA           (1 << 10)
#define GENET_UMAC_CMD_TX_PAUSE_IGNORE  (1 << 28)
#define GENET_UMAC_CMD_SW_RESET         (1 << 13)
#define GENET_UMAC_CMD_LCL_LOOP_EN      (1 << 15)
#define GENET_UMAC_CMD_ETH_SPEED_SHIFT  2

#define GENET_SPEED_UNKNOWN_MASK        (3 << GENET_UMAC_CMD_ETH_SPEED_SHIFT)
#define GENET_SPEED_1000_MASK           (2 << GENET_UMAC_CMD_ETH_SPEED_SHIFT)
#define GENET_SPEED_100_MASK            (1 << GENET_UMAC_CMD_ETH_SPEED_SHIFT)
#define GENET_SPEED_10_MASK             (0 << GENET_UMAC_CMD_ETH_SPEED_SHIFT)

#define GENET_UMAC_MAC0                 0x80C
#define GENET_UMAC_MAC1                 0x810
#define GENET_UMAC_FRM_LEN              0x814

/* genet MIB reg and bits */
#define GENET_UMAC_MIB_CNTRL            0xD80
#define GENET_UMAC_MIB_CNTRL_RX_CNT_RST     (1 << 0)
#define GENET_UMAC_MIB_CNTRL_RUNT_CNT_RST   (1 << 1)
#define GENET_UMAC_MIB_CNTRL_TX_CNT_RST     (1 << 2)

#define GENET_MDIO_OFFSET               0xE14

/* DMA misc */
#define GENET_DMA_DESC_WORDS        3
#define GENET_DMA_DESC_SIZE         (GENET_DMA_DESC_WORDS * sizeof (uint32_t))  //CNTRL, physical address LSB, and MSB

#define GENET_RDMA_REG_OFF          0x2000
#define GENET_TDMA_REG_OFF          0x4000


/*DMA_RING_CNTRL*/
#define GENET_DEFAULT_RING          16  // its a best effort queue(low priority)
#define GENET_RING_CNTRL_SIZE       64  //prod idx, cons idx, rd ptr, wr ptr, and etc,.
#define GENET_BUFFER_SIZE           2048    // This must match the cluster size
#define GENET_RING_SIZE_MAX         256 //256 for each TX and RX

#define DMA_RING_OFFSET             ( (GENET_RING_SIZE_MAX * GENET_DMA_DESC_SIZE) + \
                     (GENET_DEFAULT_RING * GENET_RING_CNTRL_SIZE) )

#define GENET_TDMA_RING_OFFSET      ( GENET_TDMA_REG_OFF + DMA_RING_OFFSET )
#define GENET_RDMA_RING_OFFSET      ( GENET_RDMA_REG_OFF + DMA_RING_OFFSET )

/* genet TDMA ring reg and bits */
#define GENET_TDMA_READ_POINTER         (GENET_TDMA_RING_OFFSET + 0x00)
#define GENET_TDMA_CONSUMER_INDEX       (GENET_TDMA_RING_OFFSET + 0x08)
#define GENET_TDMA_PRODUCER_INDEX       (GENET_TDMA_RING_OFFSET + 0x0c)
#define GENET_TDMA_RING_BUF_SIZE        (GENET_TDMA_RING_OFFSET + 0x10)
#define GENET_TDMA_RING_BUF_SIZE_DESC_SHIFT 16
#define GENET_TDMA_RING_BUF_SIZE_SIZE_SHIFT 0
#define GENET_TDMA_START_ADDRESS        (GENET_TDMA_RING_OFFSET + 0x14)
#define GENET_TDMA_START_ADDRESSH       (GENET_TDMA_RING_OFFSET + 0x18)
#define GENET_TDMA_END_ADDRESS          (GENET_TDMA_RING_OFFSET + 0x1c)
#define GENET_TDMA_END_ADDRESSH         (GENET_TDMA_RING_OFFSET + 0x20)
#define GENET_TDMA_MBUF_DONE_INTR_THRESH    (GENET_TDMA_RING_OFFSET + 0x24)
#define GENET_TDMA_FLOW_PERIOD          (GENET_TDMA_RING_OFFSET + 0x28)
#define GENET_TDMA_WRITE_POINTER        (GENET_TDMA_RING_OFFSET + 0x2c)

/* genet RDMA ring reg and bits */
#define GENET_RDMA_WRITE_POINTER        (GENET_RDMA_RING_OFFSET + 0x00)
#define GENET_RDMA_PRODUCER_INDEX       (GENET_RDMA_RING_OFFSET + 0x08)
#define GENET_RDMA_CONSUMER_INDEX       (GENET_RDMA_RING_OFFSET + 0x0c)
#define GENET_RDMA_RING_BUF_SIZE        (GENET_RDMA_RING_OFFSET + 0x10)
#define GENET_RDMA_RING_BUF_SIZE_DESC_SHIFT 16
#define GENET_RDMA_RING_BUF_SIZE_SIZE_SHIFT 0
#define GENET_RDMA_START_ADDRESS        (GENET_RDMA_RING_OFFSET + 0x14)
#define GENET_RDMA_START_ADDRESSH       (GENET_RDMA_RING_OFFSET + 0x18)
#define GENET_RDMA_END_ADDRESS          (GENET_RDMA_RING_OFFSET + 0x1c)
#define GENET_RDMA_END_ADDRESSH         (GENET_RDMA_RING_OFFSET + 0x20)
#define GENET_RDMA_MBUF_DONE_INTR_THRESH    (GENET_RDMA_RING_OFFSET + 0x24)
#define GENET_RDMA_XON_XOFF_THRESH      (GENET_RDMA_RING_OFFSET + 0x28)
#define GENET_RDMA_READ_POINTER         (GENET_RDMA_RING_OFFSET + 0x2c)

#define GENET_RDMA_XON_XOFF_THR_SHIFT   16

/* genet DMA ring configuration reg and bits */
#define GENET_TDMA_RING_CFG             (GENET_TDMA_RING_OFFSET + 0x40)
#define GENET_RDMA_RING_CFG             (GENET_RDMA_RING_OFFSET + 0x40)

/* genet DMA ring control reg and bits */
#define GENET_TDMA_CONTROL              (GENET_TDMA_RING_OFFSET + 0x44)
#define GENET_RDMA_CONTROL              (GENET_RDMA_RING_OFFSET + 0x44)
#define GENET_TDMA_CONTROL_DMA_ENABLE   (1 << 0)
#define GENET_RDMA_CONTROL_DMA_ENABLE   (1 << 0)

#define GENET_TDMA_SCB_BURST_SIZE       (GENET_TDMA_RING_OFFSET + 0x4c)
#define GENET_RDMA_SCB_BURST_SIZE       (GENET_RDMA_RING_OFFSET + 0x4c)

/* genet DMA DESC flags */
#define GENET_DMA_DESC_CNTRL            0x00
#define GENET_DMA_DESC_ADDR_LSB         0x04
#define GENET_DMA_DESC_ADDR_MSB         0x08


/* genet TDMA DESC flags */
#define GENET_DMA_DO_CRC                (1 << 5)
#define GENET_DMA_APPEND_CRC            (1 << 6)
#define GENET_DMA_FIRST_PKT             (1 << 13)
#define GENET_DMA_LAST_PKT              (1 << 14)
#define GENET_DMA_QTAG                  (0x3F << 7)
#define GENET_DMA_BUFLEN_SHIFT          16

/* genet TDMA DESC flags */
#define GENET_DMA_RX_OVERRUN_ERR        (1 << 0)
#define GENET_DMA_RX_CRC_ERR            (1 << 1)
#define GENET_DMA_RX_RXERR              (1 << 2)
#define GENET_DMA_RX_FRAME_ERR          (1 << 3)
#define GENET_DMA_RX_LENGTH_ERR         (1 << 4)
#define GENET_DMA_RX_MCAST              (1 << 5)
#define GENET_DMA_RX_BCAST              (1 << 6)

/* MDIO */
#define MDIO_MAX_PORT                   31
#define GENET_PHY_ADDR                  1     /*1st port of phy is connected with */

#define PHY_ADDR                        GENET_PHY_ADDR

/* MDIO commands */
#define MDIO_CMD_WRITE                  (1 << 26)
#define MDIO_CMD_READ                   (1 << 27)
#define MDIO_READ_FAIL                  (1 << 28)
#define MDIO_START_BUSY                 (1 << 29)
#define MDIO_PHY_ADR_SHIFT              21
#define MDIO_REG_ADDR_SHIFT             16

typedef enum MII_REG
{
   MII_REG_CONTROL                      = 0x00,
   MII_REG_STATUS                       = 0x01,
   MII_REG_PHY_ID_MSB                   = 0x02,
   MII_REG_PHY_ID_LSB                   = 0x03,
   MII_REG_AUTONEG_LINK_PARTNER_ABILITY = 0x05,
   MII_REG_1000_BASE_T_CONTROL          = 0x09,
   MII_REG_1000_BASE_T_STATUS           = 0x0a,
   MII_REG_EXPANSION                    = 0x15,
   MII_REG_EXPANSION_ACCESS             = 0x17,
   MII_REG_AUX_CONTROL_SHADOW           = 0x18,
   MII_REG_SHADOW                       = 0x1c,
} MII_REG;

typedef enum MII_SHADOW_REG
{
   MII_SHADOW_REG_LED_CONTROL           = 0x09,
   MII_SHADOW_REG_LEDS1                 = 0x0d,
} MII_SHADOW_REG;

typedef enum MII_EXPANSION_REG
{
   MII_EXPANSION_REG_PACKET_COUNTER     = 0x00,
   MII_EXPANSION_REG_LED                = 0x04,
} MII_EXPANSION_REG;

#endif /* GENET_REG_H_ */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
