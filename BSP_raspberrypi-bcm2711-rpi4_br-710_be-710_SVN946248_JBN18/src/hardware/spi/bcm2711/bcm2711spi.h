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

#ifndef _BCM2711SPI_H_
#define _BCM2711SPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <hw/inout.h>
#include <hw/spi-master.h>
#include <aarch64/bcm2711.h>
#include "spi_slog2.h"

#define NUM_OF_SPI_DEVS         2

/* Main SPI Registers Offsets */
#define BCM2711_SPI_CS          0x0
#define BCM2711_SPI_FIFO        0x4
#define BCM2711_SPI_CLK         0x8
#define BCM2711_SPI_DLEN        0xC
#define BCM2711_SPI_LTOH        0x10
#define BCM2711_SPI_DC          0x14

/* CS register definitions */
#define SPI_CS_CS           0
#define SPI_CS_CPHA         2
#define SPI_CS_CPOL         3
#define SPI_CS_CLEAR        4
#define SPI_CS_CSPOL        6
#define SPI_CS_TA           7
#define SPI_CS_DMAEN        8
#define SPI_CS_INTD         9
#define SPI_CS_INTR         10
#define SPI_CS_ADCS         11
#define SPI_CS_REN          12
#define SPI_CS_LEN          13
#define SPI_CS_LMONO        14
#define SPI_CS_TE_EN        15
#define SPI_CS_DONE         16
#define SPI_CS_RXD          17
#define SPI_CS_TXD          18
#define SPI_CS_RXR          19
#define SPI_CS_RXF          20
#define SPI_CS_CSPOL0       21
#define SPI_CS_CSPOL1       22
#define SPI_CS_CSPOL2       23
#define SPI_CS_DMA_LEN      24
#define SPI_CS_LEN_LONG     25

#define SPI_CS_CS0                   UINT32_C(0x0)
#define SPI_CS_CS1                   UINT32_C(0x1)
#define SPI_CS_CS2                   UINT32_C(0x2)
#define SPI_CS_CPHA_MIDDLE           UINT32_C(0x0)
#define SPI_CS_CPHA_BEGIN            UINT32_C(0x1)
#define SPI_CS_CPOL_LOW              UINT32_C(0x0)
#define SPI_CS_CPOL_HIGH             UINT32_C(0x1)
#define SPI_CS_CLEAR_NOACTION        UINT32_C(0x0)
#define SPI_CS_CLEAR_CLRTXFIFO       UINT32_C(0x1)
#define SPI_CS_CLEAR_CLRRXFIFO       UINT32_C(0x2)
#define SPI_CS_CSPOL_LOW             UINT32_C(0x0)
#define SPI_CS_CSPOL_HIGH            UINT32_C(0x1)
#define SPI_CS_TA_NOTACTIVE          UINT32_C(0x0)
#define SPI_CS_TA_ACTIVE             UINT32_C(0x1)
#define SPI_CS_DMAEN_DMADIS          UINT32_C(0x0)
#define SPI_CS_DMAEN_DMAEN           UINT32_C(0x1)
#define SPI_CS_INTD_NOINT            UINT32_C(0x0)
#define SPI_CS_INTD_INT              UINT32_C(0x1)
#define SPI_CS_INTR_NOINT            UINT32_C(0x0)
#define SPI_CS_INTR_INT              UINT32_C(0x1)
#define SPI_CS_ADCS_NOTAUTO          UINT32_C(0x0)
#define SPI_CS_ADCS_AUTO             UINT32_C(0x1)
#define SPI_CS_REN_WR                UINT32_C(0x0)
#define SPI_CS_REN_RD                UINT32_C(0x1)
#define SPI_CS_LEN_SPIMASTER         UINT32_C(0x0)
#define SPI_CS_LEN_LOSSIMASTER       UINT32_C(0x1)

#define SPI_CS_CSPOL0_LOW            UINT32_C(0x0)
#define SPI_CS_CSPOL0_HIGH           UINT32_C(0x1)
#define SPI_CS_CSPOL1_LOW            UINT32_C(0x0)
#define SPI_CS_CSPOL1_HIGH           UINT32_C(0x1)
#define SPI_CS_CSPOL2_LOW            UINT32_C(0x0)
#define SPI_CS_CSPOL2_HIGH           UINT32_C(0x1)
#define SPI_CS_DMA_LEN_DMADIS        UINT32_C(0x0)
#define SPI_CS_DMA_LEN_DMAEN         UINT32_C(0x1)
#define SPI_CS_LEN_LONG_BYTE         UINT32_C(0x0)
#define SPI_CS_LEN_LONG_WORD         UINT32_C(0x1)

#define SPI_CS_RXD_FIFOEMPTY         UINT8_C(0x0)
#define SPI_CS_RXD_FIFONOTEMPTY      UINT8_C(0x1)
#define SPI_CS_TXD_FIFOFULL          UINT8_C(0x0)
#define SPI_CS_TXD_FIFONOTFULL       UINT8_C(0x1)
#define SPI_CS_RXR_FIFONOTFULL       UINT8_C(0x0)
#define SPI_CS_RXR_FIFOFULL          UINT8_C(0x1)
#define SPI_CS_RXF_FIFONOTFULL       UINT8_C(0x0)
#define SPI_CS_RXF_FIFOFULL          UINT8_C(0x1)


/* CLK register definitions */
#define SPI_CLK_MASK        0x0000FFFF

/* DLEN register definitions */
#define SPI_DLEN_MASK       0x0000FFFF

/* LTOH register definitions */
#define SPI_LTOH_MASK       0x0000000F

/* DC register */
#define SPI_DC_TDREQ        0
#define SPI_DC_TPANIC       8
#define SPI_DC_RDREQ        16
#define SPI_DC_RPANIC       24

#define SPI_CS_CS_00        0x00
#define SPI_CS_CS_01        0x01
#define SPI_CS_CS_02        0x02

typedef enum
{
    STANDARD_MODE,
    BIDIRECTIONAL_MODE,
    LOSSI_MODE
}Mode_e;

typedef enum
{
    POLLING,
    INTERRUPT,
    DMA
}Op_Mode_e;

#define BCM2711_SPI_INPUT_CLOCK 500000000
#define BCM2711_SPI_MAX_CDIV     65536
#define BCM2711_SPI_MIN_CDIV     2

#define BCM2711_SPI_REGLEN              0x200
#define	BCM2711_SPI_EVENT               1
#define	BCM2711_SPI_DMA_EVENT           2
#define BCM2711_SPI_FIFOLEN             16
#define BCM2711_SPI_RD_LEN              12
#define	BCM2711_SPI_MAXDMALEN           ((1 * 1024) - 1)
#define BCM2711_SPI_DEFAULT_TPANIC_TH   16
#define BCM2711_SPI_DEFAULT_RPANIC_TH   48
#define BCM2711_SPI_DEFAULT_TDREQ_TH    32
#define BCM2711_SPI_DEFAULT_RDREQ_TH    32
#define BCM2711_DMA_REGLEN  BCM2711_DMA_SIZE

#define DMA_CS_RESET                            (1 << 31) // dma channel reset (w1sc)
#define DMA_CS_ABORT                            (1 << 30) // abort dma transfer (w1sc)
#define DMA_CS_DISDEBUG                         (1 << 29) // disable debug pause (rw)
#define DMA_CS_WAIT_FOR_WRITES                  (1 << 28) // wait for oustanding writes (rw)
#define DMA_CS_PANIC_PRIORITY                   (15 << 20) // axi panic priority level (rw)
#define DMA_CS_PRIORITY                         (15 << 16) // axi priority level (rw)
#define DMA_CS_ERROR                            (1 << 8) // dma error (ro)
#define DMA_CS_WAITING_FOR_WRITES               (1 << 6) // waiting for last write (ro)
#define DMA_CS_WAITING_FOR_DREQ                 (1 << 5) // dma paused by dreq (ro)
#define DMA_CS_PAUSED                           (1 << 4) // dma paused (ro)
#define DMA_CS_DREQ                             (1 << 3) // dreq state (ro)
#define DMA_CS_INT                              (1 << 2) // interrupt status (w1c)
#define DMA_CS_END                              (1 << 1) // dma end flag (w1c)
#define DMA_CS_ACTIVE                           (1 << 0) // activate dma channel (rw)

#define DMA_TI_NO_WIDE_BURSTS                   (1 << 26) // don't do wide writes as 2-beat burst (rw)
#define DMA_TI_WAITS                            (31 << 21) // add wait cycles (rw)
#define DMA_TI_PERMAP                           (31 << 16) // dreq pacing (rw)
#define DMA_TI_BURST_LENGTH                     (15 << 12) // burst transfer length in words (rw)
#define DMA_TI_SRC_IGNORE                       (1 << 11) // ignore reads (rw)
#define DMA_TI_SRC_DREQ                         (1 << 10) // dreq paces reads (rw)
#define DMA_TI_SRC_WIDTH                        (1 << 9) // 128-bit source read flag (rw)
#define DMA_TI_SRC_INC                          (1 << 8) // source address increment (rw)
#define DMA_TI_DST_IGNORE                       (1 << 7) // ignore writes (rw)
#define DMA_TI_DST_DREQ                         (1 << 6) // dreq paces writes (rw)
#define DMA_TI_DST_WIDTH                        (1 << 5) // 128-bit destination write flag (rw)
#define DMA_TI_DST_INC                          (1 << 4) // destination address increment (rw)
#define DMA_TI_WAIT_RESP                        (1 << 3) // wait for write responses (rw)
#define DMA_TI_2DMODE                           (1 << 1) // 2D mode (rw)
#define DMA_TI_INTEN                            (1 << 0) // interrupt enable for current cb (rw)

#define DMA_LEN_YLENGTH                         (16383 << 16) // 2D mode y transfer length (rw)
#define DMA_LEN_XLENGTH                         (65535 << 0) // 2D mode x transfer length (rw)

#define DMA_STRIDE_D_STRIDE                     (65535 << 16) // 2D mode destination stride (rw)
#define DMA_STRIDE_S_STRIDE                     (65535 << 0) // 2D mode source stride (rw)

#define DMA_DEBUG_LITE                          (1 << 28) // dma lite flag (ro)
#define DMA_DEBUG_VERSION                       (7 << 25) // dma version (ro)
#define DMA_DEBUG_DMA_STATE                     (511 << 16) // dma state machine state (ro)
#define DMA_DEBUG_DMA_ID                        (255 << 8) // dma axi id (ro)
#define DMA_DEBUG_OUTSTANDING_WRITES            (15 << 4) // dma outstanding write counter (ro)
#define DMA_DEBUG_READ_ERROR                    (1 << 2) // slave read response error (rw)
#define DMA_DEBUG_FIFO_ERROR                    (1 << 1) // fifo error (rw)
#define DMA_DEBUG_READ_LAST_NOT_SET_ERROR       (1 <<	 0) // read last not set error (rw)


// dma control-block for channels 0..6

typedef struct __attribute__ ((packed, aligned(4))) dmacb
{
        uint32_t ti;            // Transfer Information
        uint32_t src;           // Source Address
        uint32_t dst;           // Destination Address
        uint32_t len;           // Transfer Length
        uint32_t stride;        // 2D Mode Stride
        uint32_t nextcb;        // Next Control Block Address
        uint32_t _x18;          // Reserved (set to zero)
        uint32_t _x1c;          // Reserved (set to zero)
} dmacb_t;

// dma control-block for channels 7..10
typedef struct dmalitecb {
        uint32_t ti;            // Transfer Information
        uint32_t src;           // Source Address
        uint32_t dst;           // Destination Address
        uint32_t len;           // Transfer Length
        uint32_t _x14;          // Reserved (set to zero)
        uint32_t nextcb;        // Next Control Block Address
        uint32_t _x18;          // Reserved (set to zero)
        uint32_t _x1c;          // Reserved (set to zero)
} dmalitecb_t;

/* Main DMA Registers Offsets */
#define BCM2711_DMA_CS          0x0
#define BCM2711_DMA_CBADRS      0x4
#define BCM2711_DMA_TI          0x8
#define BCM2711_DMA_SRC         0xC
#define BCM2711_DMA_DST         0x10
#define BCM2711_DMA_TXFRLEN     0x14
#define BCM2711_DMA_STRIDE      0x18
#define BCM2711_DMA_NXTCB       0x1C
#define BCM2711_DMA_DEBUG       0x20

#define DREQ_NONE               0               // no DREQ is required
#define DREQ_DSI0               1               // DSI0 / PWM1 (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_PWM1               1               // DSI0 / PWM1 (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_PCMTX              2               // PCM TX
#define DREQ_PCMRX              3               // PCM RX
#define DREQ_SMI                4               // SMI
#define DREQ_PWM0               5               // PWM0
#define DREQ_SPI0TX             6               // SPI0 TX
#define DREQ_SPI0RX             7               // SPI0 RX
#define DREQ_BSCSLAVETX         8               // BSC/SPI Slave TX
#define DREQ_SPISLAVETX         8               // BSC/SPI Slave TX
#define DREQ_BSCSLAVERX         9               // BSC/SPI Slave RX
#define DREQ_SPISLAVERX         9               // BSC/SPI Slave RX
#define DREQ_HDMI0              10              // HDMI0
#define DREQ_EMMC               11              // e.MMC
#define DREQ_UART0TX            12              // UART0 TX
#define DREQ_SDHOST             13              // SD HOST
#define DREQ_UART0RX            14              // UART0 RX
#define DREQ_DSI1               15              // DSI1
#define DREQ_SPI1TX             16              // SPI1 TX
#define DREQ_HDMI1              17              // HDMI1
#define DREQ_SPI1RX             18              // SPI1 RX
#define DREQ_SPI4TX             19              // UART3 TX / SPI4 TX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_UART3TX            19              // UART3 TX / SPI4 TX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_SPI4RX             20              // UART3 RX / SPI4 RX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_UART3RX            20              // UART3 RX / SPI4 RX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_SPI5TX             21              // UART5 TX / SPI5 TX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_UART5TX            21              // UART5 TX / SPI5 TX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_SPI5RX             22              // UART5 RX / SPI5 RX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_UART5RX            22              // UART5 RX / SPI5 RX (see DMA_CNTRL_MUX bits in PACTL_CS)
#define DREQ_SPI6TX             23              // SPI6 TX
#define DREQ_SCALERFIFO0        24              // Scaler FIFO 0 & SMI
#define DREQ_SCALERSMI0         24              // Scaler FIFO 0 & SMI
#define DREQ_SCALERFIFO1        25              // Scaler FIFO 1 & SMI
#define DREQ_SCALERSMI1         25              // Scaler FIFO 1 & SMI
#define DREQ_SCALERFIFO2        26              // Scaler FIFO 2 & SMI
#define DREQ_SCALERSMI2         26              // Scaler FIFO 2 & SMI
#define DREQ_SPI6RX             27              // SPI6 RX
#define DREQ_UART2TX            28              // UART2 TX
#define DREQ_UART2RX            29              // UART2 RX
#define DREQ_UART4TX            30              // UART4 TX
#define DREQ_UART4RX            31              // UART4 RX

/* The structure which maintains the various parameters
 * of the SPI module.
 */
typedef struct {
    SPIDEV      spi; /* This has to be the first element */
    uint64_t	pbase;
    uint64_t	dma_rx_pbase;
    uint64_t	dma_tx_pbase;
    uintptr_t	vbase;
    uintptr_t	gpio_vbase;
    uintptr_t	dma_rx_vbase;
    uintptr_t	dma_tx_vbase;
    int         irq_spi;
    int         chid, coid;
    int         iid;
    int         dma_rx_irq;
    int         dma_tx_irq;
    int         dma_coid;
    void        *mmapdmarx_cb;
    void        *mmapdmatx_cb;
    dmacb_t     *dmarx_cb;
    dmacb_t     *dmatx_cb;
    uint32_t    pdmarx_cb;
    uint32_t    pdmatx_cb;
    uint32_t    dmarx_dreq;
    uint32_t    dmatx_dreq;
    int         iid_dma;
    uint32_t    bitrate;
    uint32_t    clock;
    uint8_t     *pbuf;
    int         xlen;  /* Full transaction length requested by client */
    int         tlen;  /* Transmit counter */
    int         rlen;  /* Receive counter */
    int         dlen;  /* Data size. 1 = 8bits/byte, 2 = 16bits/2 bytes, 4 = 32bits/4 bytes */
    int         dtime; /* usec per data, for time out use */
    uint32_t    rpanic_threshold;
    uint32_t    tpanic_threshold;
    uint32_t    rdreq_threshold;
    uint32_t    tdreq_threshold;
    Op_Mode_e  opmode;
    struct      sigevent spievent;
    int         dma_fd;
} bcm2711_spi_t;

int bcm2711_cfg(void *hdl, spi_cfg_t *cfg, uint32_t *clientctrl);
int bcm2711_attach_intr(bcm2711_spi_t *bcm2711);
int bcm2711spi_wait(bcm2711_spi_t *dev, int len);
int bcm2711_init_dma(bcm2711_spi_t *dev);
int bcm2711_dma_attach_intr(bcm2711_spi_t *bcm2711);
int bcm2711_setup_dma(bcm2711_spi_t *dev, int device, spi_dma_paddr_t *paddr, uint32_t spi_init_ctrl, int len);
int bcm2711_start_dma(bcm2711_spi_t *dev);

#endif /* _BCM2711SPI_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/bcm2711/bcm2711spi.h $ $Rev: 919018 $")
#endif
