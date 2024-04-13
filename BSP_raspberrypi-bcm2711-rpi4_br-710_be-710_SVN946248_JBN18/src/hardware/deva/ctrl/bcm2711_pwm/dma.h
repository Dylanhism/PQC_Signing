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

#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>

/* DMA, DMALITE AND DMA4 channels are much the same, except:
 * - DMA control blocks are the same size but laid out differently;
 * - DMA TI flags are different and some moved to the SRCI/DESTI registers;
 * - the remapping of physical addresses is different;
 * - DMALITE doesn't support stride functionality;
 * - DMALITE is limited to 64k transfer size.
 */

/* dma1_cb_t and dma4_cb_t are the same size */
#define DMA_CB_SIZE sizeof (dma1_cb_t)

/* Definitions for DMA channels 0-10 (DMA and DMALITE channels) */

/* DMA/DMALITE CS Register - Control and Status */
#define DMA1_CS_RESET                         (1 << 31) /* (w1sc) dma channel reset */
#define DMA1_CS_ABORT                         (1 << 30) /* (w1sc) abort dma transfer */
#define DMA1_CS_DISDEBUG                      (1 << 29) /* (rw) disable debug pause */
#define DMA1_CS_WAIT_FOR_WRITES               (1 << 28) /* (rw) wait for oustanding writes */
#define DMA1_CS_PANIC_PRIORITY(n)             ((n) << 20) /* (rw) axi panic priority level */
#define DMA1_CS_PANIC_PRIORITY_MSK            DMA1_CS_PANIC_PRIORITY(0xf)
#define DMA1_CS_PRIORITY(n)                   ((n) << 16) /* (rw) axi priority level */
#define DMA1_CS_PRIORITY_MSK                  DMA1_CS_PRIORITY(0xf)
#define DMA1_CS_ERROR                         (1 <<  8) /* (ro) dma error */
#define DMA1_CS_WAITING_FOR_WRITES            (1 <<  6) /* (ro) waiting for last write */
#define DMA1_CS_WAITING_FOR_DREQ              (1 <<  5) /* (ro) dma paused by dreq */
#define DMA1_CS_PAUSED                        (1 <<  4) /* (ro) dma paused */
#define DMA1_CS_DREQ                          (1 <<  3) /* (ro) dreq state */
#define DMA1_CS_INT                           (1 <<  2) /* (w1c) interrupt status */
#define DMA1_CS_END                           (1 <<  1) /* (w1c) dma end flag */
#define DMA1_CS_ACTIVE                        (1 <<  0) /* (rw) activate dma channel */

/* DMA/DMALITE TI Register - Transfer Information */
#define DMA1_TI_NO_WIDE_BURSTS                (1 << 26) /* (rw) don't do wide writes as 2-beat burst */
#define DMA1_TI_WAITS(n)                      ((n) << 21) /* (rw) add wait cycles */
#define DMA1_TI_WAITS_MSK                     DMA1_TI_WAITS(0x1f)
#define DMA1_TI_PERMAP(n)                     ((n) << 16) /* (rw) dreq pacing */
#define DMA1_TI_PERMAP_MSK                    DMA1_TI_PERMAP(0x1f)
#define DMA1_TI_BURST(n)                      ((n) << 12) /* (rw) burst transfer length in words */
#define DMA1_TI_BURST_MSK                     DMA1_TI_BURST(0xf)
#define DMA1_TI_SRC_IGNORE                    (1 << 11) /* (rw) ignore reads */
#define DMA1_TI_SRC_DREQ                      (1 << 10) /* (rw) dreq paces reads */
#define DMA1_TI_SRC_WIDTH                     (1 <<  9) /* (rw) 128-bit source read flag */
#define DMA1_TI_SRC_INC                       (1 <<  8) /* (rw) source address increment */
#define DMA1_TI_DST_IGNORE                    (1 <<  7) /* (rw) ignore writes */
#define DMA1_TI_DST_DREQ                      (1 <<  6) /* (rw) dreq paces writes */
#define DMA1_TI_DST_WIDTH                     (1 <<  5) /* (rw) 128-bit destination write flag */
#define DMA1_TI_DST_INC                       (1 <<  4) /* (rw) destination address increment */
#define DMA1_TI_WAIT_RESP                     (1 <<  3) /* (rw) wait for write responses */
#define DMA1_TI_2DMODE                        (1 <<  1) /* (rw) 2D mode */
#define DMA1_TI_INTEN                         (1 <<  0) /* (rw) interrupt enable for current cb */

/* DMA/DMALITE TXFR_LEN Register - Transfer Length */
#define DMA1_LEN_YLENGTH                  (16383 << 16) /* (rw) 2D mode y transfer length */
#define DMA1_LEN_XLENGTH                  (65535 <<  0) /* (rw) 2D mode x transfer length */

/* DMA/DMALITE STRIDE Register - 2D-mode Stride */
#define DMA1_STRIDE_D_STRIDE              (65535 << 16) /* (rw) 2D mode destination stride */
#define DMA1_STRIDE_S_STRIDE              (65535 <<  0) /* (rw) 2D mode source stride */

/* DMA/DMALITE DEBUG Register */
#define DMA1_DEBUG_LITE                       (1 << 28) /* (ro) dma lite flag */
#define DMA1_DEBUG_VERSION                    (7 << 25) /* (ro) dma version */
#define DMA1_DEBUG_DMA_STATE                (511 << 16) /* (ro) dma state machine state */
#define DMA1_DEBUG_DMA_ID                    (255 << 8) /* (ro) dma axi id */
#define DMA1_DEBUG_OUTSTANDING_WRITES        (15 <<  4) /* (ro) dma outstanding write counter */
#define DMA1_DEBUG_READ_ERROR                 (1 <<  2) /* (rw) slave read response error */
#define DMA1_DEBUG_FIFO_ERROR                 (1 <<  1) /* (rw) fifo error */
#define DMA1_DEBUG_READ_LAST_NOT_SET_ERROR    (1 <<  0) /* (rw) read last not set error */

/* DMA Channel Register Map for DMA/DMALITE Channels (0-10) */
typedef struct dma1_reg {
    uint32_t cs;            /* 0x00 CS register - control and status */
    uint32_t conblk_ad;     /* 0x04 CONBLK_AD register - control block address */
    uint32_t ti;            /* 0x08 TI register - transfer information */
    uint32_t source_ad;     /* 0x0c SOURCE_AD register - source address */
    uint32_t dest_ad;       /* 0x10 DEST_AD register - destination address */
    uint32_t txfr_len;      /* 0x14 TXFR_LEN register - transfer length */
    uint32_t stride;        /* 0x18 STRIDE register - 2D-mode stride */
    uint32_t nextconbk;     /* 0x1c next control block */
    uint32_t debug;         /* 0x20 debug */
    uint32_t _x24[55];
} dma1_reg_t;

/* DMA Channel Control Block for DMA/DMALITE Channels (0-10) */
typedef struct dma1_cb {
    uint32_t ti;            /* 0x00 Transfer Information */
    uint32_t source_ad;     /* 0x04 Source Address */
    uint32_t dest_ad;       /* 0x08 Destination Address */
    uint32_t txfr_len;      /* 0x0c Transfer Length */
    uint32_t stride;        /* 0x10 2D Mode Stride */
    uint32_t nextconbk;     /* 0x14 Next Control Block Address */
    uint32_t _x18;          /* 0x18 Reserved (set to zero) */
    uint32_t _x1c;          /* 0x1c Reserved (set to zero) */
} dma1_cb_t;

/* Definitions for DMA channels 11-14 (DMA4 channels) */

/* DMA4 CS Register - Control and Status */
#define DMA4_CS_HALT                          (1 << 31) /* (w1sc) dma halt */
#define DMA4_CS_ABORT                         (1 << 30) /* (w1sc) abort dma transfer */
#define DMA4_CS_DISDEBUG                      (1 << 29) /* (rw) disable debug pause */
#define DMA4_CS_WAIT_FOR_WRITES               (1 << 28) /* (rw) wait for oustanding writes */
#define DMA4_CS_OUTSTANDING_TRANS             (1 << 25) /* (ro) transfers oustanding */
#define DMA4_CS_BUSY                          (1 << 24) /* (ro) dma busy */
#define DMA4_CS_PANIC_QOS(n)                ((n) << 20) /* (rw) AXI Panic QOS Level */
#define DMA4_CS_PANIC_QOS_MSK     DMA4_CS_PANIC_QOS(15)
#define DMA4_CS_QOS(n)                      ((n) << 16) /* (rw) AXI QOS Level */
#define DMA4_CS_QOS_MSK                 DMA4_CS_QOS(15)
#define DMA4_CS_ERROR                         (1 << 10) /* (ro) dma error */
#define DMA4_CS_WAITING_FOR_WRITES            (1 <<  7) /* (ro) waiting for last write */
#define DMA4_CS_WAITING_FOR_DREQ              (1 <<  6) /* (ro) dma paused by dreq */
#define DMA4_CS_WR_PAUSED                     (1 <<  5) /* (ro) dma paused */
#define DMA4_CS_RD_PAUSED                     (1 <<  4) /* (ro) dma paused */
#define DMA4_CS_DREQ                          (1 <<  3) /* (ro) dreq state */
#define DMA4_CS_INT                           (1 <<  2) /* (w1c) interrupt status */
#define DMA4_CS_END                           (1 <<  1) /* (w1c) dma end flag */
#define DMA4_CS_ACTIVE                        (1 <<  0) /* (rw) activate dma channel */

/* DMA4 DEBUG Register - Debug */
#define DMA4_DEBUG_VERSION(n)               ((n) << 28) /* (ro) DMA Version */
#define DMA4_DEBUG_VERSION_MSK   DMA4_DEBUG_VERSION(15)
#define DMA4_DEBUG_ID(n)                    ((n) << 24) /* (ro) dma axi id */
#define DMA4_DEBUG_ID_MSK             DMA4_DEBUG_ID(15)
#define DMA4_DEBUG_RESET                      (1 << 23) /* (w1sc) dma reset */
#define DMA4_DEBUG_W_STATE(n)               ((n) << 18) /* (ro) Write State Machine State */
#define DMA4_DEBUG_W_STATE_MSK   DMA4_DEBUG_W_STATE(15)
    /* Returns the value of the DMA4 engine's write state machine.
     * 0 = W_IDLE, 1 = W_PRELOAD, 2 = W_CALC, 3 = W_WRITE4K, 4 = W_READFIFO_EMPTY, 5 = W_WAIT_OUSTANDING
     */
#define DMA4_DEBUG_R_STATE(n)               ((n) << 14) /* (ro) Read State Machine State */
#define DMA4_DEBUG_R_STATE_MSK   DMA4_DEBUG_R_STATE(15)
    /* Returns the value of the DMA4 engine's read state machine.
     * 0 = R_IDLE, 1 = R_WAIT_CB_DATA, 2 = R_CALC, 3 = R_READ4K, 4 = R_READING, 5 = R_READFIFO_FULL, 6 = R_WAIT_WRITE_COMPLETE
     */
#define DMA4_DEBUG_DISABLE_CLK_GATE           (1 << 11) /* (rw) disable clock gating logic */
#define DMA4_DEBUG_ABORT_ON_ERROR             (1 << 10) /* (rw) abort on error */
#define DMA4_DEBUG_HALT_ON_ERROR              (1 <<  9) /* (rw) halt on error */
#define DMA4_DEBUG_INT_ON_ERROR               (1 <<  8) /* (rw) interrupt on error */
#define DMA4_DEBUG_READ_CB_ERROR              (1 <<  3) /* (rc) error reading cb */
#define DMA4_DEBUG_READ_ERROR                 (1 <<  2) /* (rc) error during read */
#define DMA4_DEBUG_FIFO_ERROR                 (1 <<  1) /* (rc) fifo error */
#define DMA4_DEBUG_WRITE_ERROR                (1 <<  0) /* (rc) error during write */

/* DMA4 TI Register - Transfer Information */
#define DMA4_TI_D_WAITS(n)                  ((n) << 24) /* (rw) Write Wait Cycles */
#define DMA4_TI_D_WAITS_MSK        DMA4_TI_D_WAITS(255)
    /* This slows down the DMA throughput by setting the number of dummy cycles before each AXI Write operation is started.
     * A value of 0 means that no wait cycles are to be added. Waits are counted down when the DMA4 wants to do a wait and
     * the bus is available and the writes aren't paused because of DREQS or some other reason.
     */
#define DMA4_TI_S_WAITS(n)                  ((n) << 16)     /* (rw) Read Wait Cycles */
#define DMA4_TI_S_WAITS_MSK        DMA4_TI_S_WAITS(255)
    /* This slows down the DMA throughput by setting the number of dummy cycles burnt before each DMA AXI read operation is started.
     * A value of 0 means that no wait cycles are to be added. Waits are counted down when the DMA4 wants to do a read and
     * the bus is available and the reads aren't paused because of DREQS or some other reason.
     */
#define DMA4_TI_D_DREQ                        (1 << 15) /* (rw) dreq paces writes */
#define DMA4_TI_S_DREQ                        (1 << 14) /* (rw) dreq paces reads */
#define DMA4_TI_PERMAP(n)                    ((n) << 9) /* (rw) Peripheral Mapping */
#define DMA4_TI_PERMAP_MSK           DMA4_TI_PERMAP(31)
#define DMA4_TI_WAIT_RD_RESP                  (1 <<  3) /* (rw) wait for read responses */
#define DMA4_TI_WAIT_WR_RESP                  (1 <<  2) /* (rw) wait for write responses */
#define DMA4_TI_2DMODE                        (1 <<  1) /* (rw) 2D mode */
#define DMA4_TI_INTEN                         (1 <<  0) /* (rw) interrupt enable for current cb */

/* DMA4 SRCI Register - Source Address Word 2 */
#define DMA4_SRCI_STRIDE(n)                 ((n) << 16) /* (rw) Source Stride */
#define DMA4_SRCI_STRIDE_MSK    DMA4_SRCI_STRIDE(65535)
#define DMA4_SRCI_IGNORE                      (1 << 15) /* (rw) ignore reads */
#define DMA4_SRCI_WIDTH(n)                  ((n) << 13) /* (rw) Source Transfer Width */
#define DMA4_SRCI_WIDTH_MSK          DMA4_SRCI_WIDTH(3)
    /* The DMA4 will perform all AXI source reads with this AXI transfer width. Data will be fetched in bursts of this width
     * and assembled into the correct data size inside the DMA4. On the  BCM2711 the width cannot be set larger than 128 bits.
     * 3 = 256, 2 = 128, 1 = 64, 0 = 32
     */
#define DMA4_SRCI_INC                         (1 << 12) /* (rw) increment adddress */
#define DMA4_SRCI_BURST(n)                   ((n) << 8) /* (rw) Burst Transfer Length */
#define DMA4_SRCI_BURST_MSK         DMA4_SRCI_BURST(15)
    /* Indicates the maximum burst length of the source reads. The DMA4 will attempt to transfer data as bursts of this number
     * of words unless it will cause a 4k crossing or there isn't enough data required.
     * A value of zero will produce a single beat transfer.
     */
#define DMA4_SRCI_ADDR(n)                    ((n) << 0) /* (rw) High Bits of the Source Address [40:32] */
#define DMA4_SRCI_ADDR_MSK          DMA4_SRCI_ADDR(255)
    /* The source address is split over 2 registers, and together they give a 40 bit address */

/* DMA4 DESTI Register - Destination Address Word 2 */
#define DMA4_DESTI_STRIDE(n)                 ((n) << 16) /* (rw) Destination Stride */
#define DMA4_DESTI_STRIDE_MSK   DMA4_DESTI_STRIDE(65535)
    /* This is only used in 2D transfer mode. In a 2D transfer the DMA4 will perform Y transfers each of X bytes.
     * At the end of each X row, the destination stride is added to the destination address and this is used as
     * the start address of the destination for the next X row.
     * The destination stride is a signed (2 s complement) byte increment so negative values are allowed.
     */
#define DMA4_DESTI_IGNORE                      (1 << 15) /* (rw) ignore writes */
#define DMA4_DESTI_WIDTH(n)                  ((n) << 13) /* (rw) Destination Transfer Width */
#define DMA4_DESTI_WIDTH_MSK         DMA4_DESTI_WIDTH(3)
    /* The DMA4 will perform all AXI destination writes with this  AXI transfer width.  Data will be written in bursts of this width.
     * On the BCM2711 the width cannot be set larger than 128 bits.
     * 3 = 256, 2 = 128, 1 = 64, 0 = 32
     */
#define DMA4_DESTI_INC                         (1 << 12) /* (rw) increment adddress */
#define DMA4_DESTI_BURST(n)                   ((n) << 8) /* (rw) Burst Transfer Length */
#define DMA4_DESTI_BURST_MSK         DMA4_DESTI_BURST(15)
    /* Indicates the maximum burst length of the destination writes. The DMA4 will attempt to transfer data as bursts of this number
     * of words unless it will cause a 4k crossing or there isn't enough data required.
     * A value of zero will produce a single transfer.
     */
#define DMA4_DESTI_ADDR(n)                    ((n) << 0) /* (rw) High Bits of the Destination Address [40:32] */
#define DMA4_DESTI_ADDR_MSK          DMA4_DESTI_ADDR(255)
    /* The destination address is split over 2 registers, and together they give a 40 bit address */

/* DMA4 LEN Register - Transfer Length */
#define DMA4_LEN_YLENGTH(n)                   ((n)<<16) /* (rw) 2D mode y transfer length */
#define DMA4_LEN_YLENGTH_MSK    DMA4_LEN_YLENGTH(16383)
#define DMA4_LEN_XLENGTH(n)                    ((n)<<0) /* (rw) 2D mode x transfer length */
#define DMA4_LEN_XLENGTH_MSK    DMA4_LEN_XLENGTH(65535)

/* DMA4 DEBUG2 Register - More Debug */
#define DMA4_DEBUG2_OUTSTANDING_READS(n)    ((n) << 16)     /* (ro) Outstanding Read Words Count */
#define DMA4_DEBUG2_OUTSTANDING_READS_MSK   DMA4_DEBUG2_OUTSTANDING_READS(511)
    /* This keeps count of the number of read words that have been requested and the number that have actually been returned.
     * This should be zero at the end of every transfer
     */
/* 15:09 # Reserved - Write as 0, read as don't care */
#define DMA4_DEBUG2_OUTSTANDING_WRITES(n)   ((n) << 0)      /* (ro) Outstanding Write Response Count */
#define DMA4_DEBUG2_OUTSTANDING_WRITES_MSK  DMA4_DEBUG2_OUTSTANDING_WRITES(511)
    /* This keeps count of the number of write requests that have been generated and the number of responses that have been returned.
     * This should be zero at the end of every transfer
     */

/* DMA Channel Register Map for DMA4 Channels (11-14) */
typedef struct dma4_reg {
    uint32_t cs;            /* 0x00 CS register - control and status */
    uint32_t conblk_ad;     /* 0x04 CONBLK_AD register - control block address [bits 36:5] */
    uint32_t _x08;
    uint32_t debug;         /* 0x0c DEBUG register - debug */
    uint32_t ti;            /* 0x10 TI register - transfer information */
    uint32_t src;           /* 0x14 SRC register - source address word1 */
    uint32_t srci;          /* 0x18 SRCI register - source address word2 */
    uint32_t dest;          /* 0x1c DEST register - destination address word1 */
    uint32_t desti;         /* 0x20 DESTI register - destination address word2 */
    uint32_t len;           /* 0x24 LEN2 register - transfer length */
    uint32_t next_cb;       /* 0x28 NEXT_CB register - next control block */
    uint32_t debug2;        /* 0x2c DEBUG2 register - more debug */
    uint32_t _x30[52];
} dma4_reg_t;

/* DMA Channel Control Block for DMA4 Channels (11-14) */
typedef struct dma4_cb {
    uint32_t ti;            /* 0x00 Transfer Information */
    uint32_t src;           /* 0x04 Source Address Low */
    uint32_t srci;          /* 0x08 Source Address High */
    uint32_t dest;          /* 0x0c Destination Address Low */
    uint32_t desti;         /* 0x10 Destination Address High */
    uint32_t len;           /* 0x14 Transfer Length */
    uint32_t next_cb;       /* 0x18 Next Control Block Address */
    uint32_t _x1c;          /* 0x1c Reserved (set to zero) */
} dma4_cb_t;

/* Peripheral Data Request Signals */
#define DREQ_NONE           0   /* no DREQ is required */
#define DREQ_DSI0           1   /* DSI0 / PWM1 (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_PWM1           1   /* DSI0 / PWM1 (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_PCMTX          2   /* PCM TX */
#define DREQ_PCMRX          3   /* PCM RX */
#define DREQ_SMI            4   /* SMI */
#define DREQ_PWM0           5   /* PWM0 */
#define DREQ_SPI0TX         6   /* SPI0 TX */
#define DREQ_SPI0RX         7   /* SPI0 RX */
#define DREQ_BSCSLAVETX     8   /* BSC/SPI Slave TX */
#define DREQ_SPISLAVETX     8   /* BSC/SPI Slave TX */
#define DREQ_BSCSLAVERX     9   /* BSC/SPI Slave RX */
#define DREQ_SPISLAVERX     9   /* BSC/SPI Slave RX */
#define DREQ_HDMI0          10  /* HDMI0 */
#define DREQ_EMMC           11  /* e.MMC */
#define DREQ_UART0TX        12  /* UART0 TX */
#define DREQ_SDHOST         13  /* SD HOST */
#define DREQ_UART0RX        14  /* UART0 RX */
#define DREQ_DSI1           15  /* DSI1 */
#define DREQ_SPI1TX         16  /* SPI1 TX */
#define DREQ_HDMI1          17  /* HDMI1 */
#define DREQ_SPI1RX         18  /* SPI1 RX */
#define DREQ_SPI4TX         19  /* UART3 TX / SPI4 TX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_UART3TX        19  /* UART3 TX / SPI4 TX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_SPI4RX         20  /* UART3 RX / SPI4 RX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_UART3RX        20  /* UART3 RX / SPI4 RX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_SPI5TX         21  /* UART5 TX / SPI5 TX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_UART5TX        21  /* UART5 TX / SPI5 TX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_SPI5RX         22  /* UART5 RX / SPI5 RX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_UART5RX        22  /* UART5 RX / SPI5 RX (see DMA_CNTRL_MUX bits in PACTL_CS) */
#define DREQ_SPI6TX         23  /* SPI6 TX */
#define DREQ_SCALERFIFO0    24  /* Scaler FIFO 0 & SMI */
#define DREQ_SCALERSMI0     24  /* Scaler FIFO 0 & SMI */
#define DREQ_SCALERFIFO1    25  /* Scaler FIFO 1 & SMI */
#define DREQ_SCALERSMI1     25  /* Scaler FIFO 1 & SMI */
#define DREQ_SCALERFIFO2    26  /* Scaler FIFO 2 & SMI */
#define DREQ_SCALERSMI2     26  /* Scaler FIFO 2 & SMI */
#define DREQ_SPI6RX         27  /* SPI6 RX */
#define DREQ_UART2TX        28  /* UART2 TX */
#define DREQ_UART2RX        29  /* UART2 RX */
#define DREQ_UART4TX        30  /* UART4 TX */
#define DREQ_UART4RX        31  /* UART4 RX */

/* DMA Channel Ids */
#define DMA_0               0
#define DMA_1               1
#define DMA_2               2
#define DMA_3               3
#define DMA_4               4
#define DMA_5               5
#define DMA_6               6
#define DMALITE_7           7
#define DMALITE_8           8
#define DMALITE_9           9
#define DMALITE_10          10
#define DMA4_11             11
#define DMA4_12             12
#define DMA4_13             13
#define DMA4_14             14

#define DMA_CHANNEL_MIN     DMA_0
#define DMA_CHANNEL_MAX     DMA4_14

#define DMA_INT             0x1
#define DMA_ERROR           0x2

struct dma_funcs {
    void (*dma_start)(void *hw, uint32_t cbphys);
    void (*dma_abort)(void *hw);
    int  (*dma_ack)(void *hw);
    void (*dma_setcb)(void *cb, uint32_t ti, uint32_t src, uint32_t dst, uint32_t len, uint32_t nextcb);
    unsigned (*dma_len)(void *hw);
};

struct dma_config {
    uint32_t base;
    uint32_t irq;
    const struct dma_funcs *fn;
    uint32_t bits;
};

int dma_getcfg(unsigned chan, const struct dma_config **cfgp);
void dma_dropcfg(const struct dma_config *cfg);

#endif /* __DMA_H__ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pwm/dma.h $ $Rev: 934572 $")
#endif
