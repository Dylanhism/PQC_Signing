/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2018 NXP
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

#ifndef IMX_GPMI_H_
#define IMX_GPMI_H_

#include <stdint.h>

/* General Purpose Media Interface (GPMI) */

/* Register CTRL0 Control Register 0 */
#define IMX_GPMI_CTRL0                                      0x0
#define IMX_GPMI_CTRL0_SET                                  0x4
#define IMX_GPMI_CTRL0_CLR                                  0x8
#define IMX_GPMI_CTRL0_TOG                                  0xC
/* Field SFTRST */
#define IMX_GPMI_CTRL0_SFTRST_SHIFT                         31
#define IMX_GPMI_CTRL0_SFTRST_MASK                          0x80000000
#define IMX_GPMI_CTRL0_SFTRST(v)                            (((v) << 31) & IMX_GPMI_CTRL0_SFTRST_MASK)
#define IMX_GPMI_CTRL0_SFTRST_BV_RUN                        0x0
#define IMX_GPMI_CTRL0_SFTRST_BV_RESET                      0x1
/* Field CLKGATE */
#define IMX_GPMI_CTRL0_CLKGATE_SHIFT                        30
#define IMX_GPMI_CTRL0_CLKGATE_MASK                         0x40000000
#define IMX_GPMI_CTRL0_CLKGATE(v)                           (((v) << 30) & IMX_GPMI_CTRL0_CLKGATE_MASK)
#define IMX_GPMI_CTRL0_CLKGATE_BV_RUN                       0x0
#define IMX_GPMI_CTRL0_CLKGATE_BV_NO_CLKS                   0x1
/* Field RUN */
#define IMX_GPMI_CTRL0_RUN_SHIFT                            29
#define IMX_GPMI_CTRL0_RUN_MASK                             0x20000000
#define IMX_GPMI_CTRL0_RUN(v)                               (((v) << 29) & IMX_GPMI_CTRL0_RUN_MASK)
#define IMX_GPMI_CTRL0_RUN_BV_IDLE                          0x0
#define IMX_GPMI_CTRL0_RUN_BV_BUSY                          0x1
/* Field DEV_IRQ_EN */
#define IMX_GPMI_CTRL0_DEV_IRQ_EN_SHIFT                     28
#define IMX_GPMI_CTRL0_DEV_IRQ_EN_MASK                      0x10000000
#define IMX_GPMI_CTRL0_DEV_IRQ_EN(v)                        (((v) << 28) & IMX_GPMI_CTRL0_DEV_IRQ_EN_MASK)
/* Field LOCK_CS */
#define IMX_GPMI_CTRL0_LOCK_CS_SHIFT                        27
#define IMX_GPMI_CTRL0_LOCK_CS_MASK                         0x8000000
#define IMX_GPMI_CTRL0_LOCK_CS(v)                           (((v) << 27) & IMX_GPMI_CTRL0_LOCK_CS_MASK)
#define IMX_GPMI_CTRL0_LOCK_CS_BV_DISABLED                  0x0
#define IMX_GPMI_CTRL0_LOCK_CS_BV_ENABLED                   0x1
/* Field UDMA */
#define IMX_GPMI_CTRL0_UDMA_SHIFT                           26
#define IMX_GPMI_CTRL0_UDMA_MASK                            0x4000000
#define IMX_GPMI_CTRL0_UDMA(v)                              (((v) << 26) & IMX_GPMI_CTRL0_UDMA_MASK)
#define IMX_GPMI_CTRL0_UDMA_BV_DISABLED                     0x0
#define IMX_GPMI_CTRL0_UDMA_BV_ENABLED                      0x1
/* Field COMMAND_MODE */
#define IMX_GPMI_CTRL0_COMMAND_MODE_SHIFT                   24
#define IMX_GPMI_CTRL0_COMMAND_MODE_MASK                    0x3000000
#define IMX_GPMI_CTRL0_COMMAND_MODE(v)                      (((v) << 24) & IMX_GPMI_CTRL0_COMMAND_MODE_MASK)
#define IMX_GPMI_CTRL0_COMMAND_MODE_BV_WRITE                0x0
#define IMX_GPMI_CTRL0_COMMAND_MODE_BV_READ                 0x1
#define IMX_GPMI_CTRL0_COMMAND_MODE_BV_READ_AND_COMPARE     0x2
#define IMX_GPMI_CTRL0_COMMAND_MODE_BV_WAIT_FOR_READY       0x3
/* Field WORD_LENGTH */
#define IMX_GPMI_CTRL0_WORD_LENGTH_SHIFT                    23
#define IMX_GPMI_CTRL0_WORD_LENGTH_MASK                     0x800000
#define IMX_GPMI_CTRL0_WORD_LENGTH(v)                       (((v) << 23) & IMX_GPMI_CTRL0_WORD_LENGTH_MASK)
#define IMX_GPMI_CTRL0_WORD_LENGTH_BV_16_BIT                0x0
#define IMX_GPMI_CTRL0_WORD_LENGTH_BV_8_BIT                 0x1
/* Field CS */
#define IMX_GPMI_CTRL0_CS_SHIFT                             20
#define IMX_GPMI_CTRL0_CS_MASK                              0x700000
#define IMX_GPMI_CTRL0_CS(v)                                (((v) << 20) & IMX_GPMI_CTRL0_CS_MASK)
/* Field ADDRESS */
#define IMX_GPMI_CTRL0_ADDRESS_SHIFT                        17
#define IMX_GPMI_CTRL0_ADDRESS_MASK                         0xE0000
#define IMX_GPMI_CTRL0_ADDRESS(v)                           (((v) << 17) & IMX_GPMI_CTRL0_ADDRESS_MASK)
#define IMX_GPMI_CTRL0_ADDRESS_BV_NAND_DATA                 0x0
#define IMX_GPMI_CTRL0_ADDRESS_BV_NAND_CLE                  0x1
#define IMX_GPMI_CTRL0_ADDRESS_BV_NAND_ALE                  0x2
/* Field ADDRESS_INCREMENT */
#define IMX_GPMI_CTRL0_ADDRESS_INCREMENT_SHIFT              16
#define IMX_GPMI_CTRL0_ADDRESS_INCREMENT_MASK               0x10000
#define IMX_GPMI_CTRL0_ADDRESS_INCREMENT(v)                 (((v) << 16) & IMX_GPMI_CTRL0_ADDRESS_INCREMENT_MASK)
#define IMX_GPMI_CTRL0_ADDRESS_INCREMENT_BV_DISABLED        0x0
#define IMX_GPMI_CTRL0_ADDRESS_INCREMENT_BV_ENABLED         0x1
/* Field XFER_COUNT */
#define IMX_GPMI_CTRL0_XFER_COUNT_SHIFT                     0
#define IMX_GPMI_CTRL0_XFER_COUNT_MASK                      0xFFFF
#define IMX_GPMI_CTRL0_XFER_COUNT(v)                        (((v) << 0) & IMX_GPMI_CTRL0_XFER_COUNT_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned XFER_COUNT:16;
        unsigned ADDRESS_INCREMENT:1;
        unsigned ADDRESS:3;
        unsigned CS:3;
        unsigned WORD_LENGTH:1;
        unsigned COMMAND_MODE:2;
        unsigned UDMA:1;
        unsigned LOCK_CS:1;
        unsigned DEV_IRQ_EN:1;
        unsigned RUN:1;
        unsigned CLKGATE:1;
        unsigned SFTRST:1;
    } B;
} imx_gpmi_ctrl0_t;



/* Register COMPARE Compare Register */
#define IMX_GPMI_COMPARE                                    0x10
/* Field MASK */
#define IMX_GPMI_COMPARE_MASK_SHIFT                         16
#define IMX_GPMI_COMPARE_MASK_MASK                          0xFFFF0000
#define IMX_GPMI_COMPARE_MASK(v)                            (((v) << 16) & IMX_GPMI_COMPARE_MASK_MASK)
/* Field REFERENCE */
#define IMX_GPMI_COMPARE_REFERENCE_SHIFT                    0
#define IMX_GPMI_COMPARE_REFERENCE_MASK                     0xFFFF
#define IMX_GPMI_COMPARE_REFERENCE(v)                       (((v) << 0) & IMX_GPMI_COMPARE_REFERENCE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned REFERENCE:16;
        unsigned MASK:16;
    } B;
} imx_gpmi_compare_t;



/* Register ECCCTRL Integrated ECC Control Register */
#define IMX_GPMI_ECCCTRL                                    0x20
#define IMX_GPMI_ECCCTRL_SET                                0x24
#define IMX_GPMI_ECCCTRL_CLR                                0x28
#define IMX_GPMI_ECCCTRL_TOG                                0x2C
/* Field HANDLE */
#define IMX_GPMI_ECCCTRL_HANDLE_SHIFT                       16
#define IMX_GPMI_ECCCTRL_HANDLE_MASK                        0xFFFF0000
#define IMX_GPMI_ECCCTRL_HANDLE(v)                          (((v) << 16) & IMX_GPMI_ECCCTRL_HANDLE_MASK)
/* Field ECC_CMD */
#define IMX_GPMI_ECCCTRL_ECC_CMD_SHIFT                      13
#define IMX_GPMI_ECCCTRL_ECC_CMD_MASK                       0x6000
#define IMX_GPMI_ECCCTRL_ECC_CMD(v)                         (((v) << 13) & IMX_GPMI_ECCCTRL_ECC_CMD_MASK)
#define IMX_GPMI_ECCCTRL_ECC_CMD_BV_DECODE                  0x0
#define IMX_GPMI_ECCCTRL_ECC_CMD_BV_ENCODE                  0x1
#define IMX_GPMI_ECCCTRL_ECC_CMD_BV_RESERVE2                0x2
#define IMX_GPMI_ECCCTRL_ECC_CMD_BV_RESERVE3                0x3
/* Field ENABLE_ECC */
#define IMX_GPMI_ECCCTRL_ENABLE_ECC_SHIFT                   12
#define IMX_GPMI_ECCCTRL_ENABLE_ECC_MASK                    0x1000
#define IMX_GPMI_ECCCTRL_ENABLE_ECC(v)                      (((v) << 12) & IMX_GPMI_ECCCTRL_ENABLE_ECC_MASK)
#define IMX_GPMI_ECCCTRL_ENABLE_ECC_BV_ENABLE               0x1
#define IMX_GPMI_ECCCTRL_ENABLE_ECC_BV_DISABLE              0x0
/* Field BUFFER_MASK */
#define IMX_GPMI_ECCCTRL_BUFFER_MASK_SHIFT                  0
#define IMX_GPMI_ECCCTRL_BUFFER_MASK_MASK                   0x1FF
#define IMX_GPMI_ECCCTRL_BUFFER_MASK(v)                     (((v) << 0) & IMX_GPMI_ECCCTRL_BUFFER_MASK_MASK)
#define IMX_GPMI_ECCCTRL_BUFFER_MASK_BV_BCH_AUXONLY         0x100
#define IMX_GPMI_ECCCTRL_BUFFER_MASK_BV_BCH_PAGE            0x1FF
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned BUFFER_MASK:9;
        unsigned RSVD1:3;
        unsigned ENABLE_ECC:1;
        unsigned ECC_CMD:2;
        unsigned RSVD2:1;
        unsigned HANDLE:16;
    } B;
} imx_gpmi_eccctrl_t;



/* Register ECCCOUNT ECC Transfer Count Register */
#define IMX_GPMI_ECCCOUNT                                   0x30
/* Field COUNT */
#define IMX_GPMI_ECCCOUNT_COUNT_SHIFT                       0
#define IMX_GPMI_ECCCOUNT_COUNT_MASK                        0xFFFF
#define IMX_GPMI_ECCCOUNT_COUNT(v)                          (((v) << 0) & IMX_GPMI_ECCCOUNT_COUNT_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned COUNT:16;
        unsigned RSVD2:16;
    } B;
} imx_gpmi_ecccount_t;



/* Register PAYLOAD Payload Address Register */
#define IMX_GPMI_PAYLOAD                                    0x40
/* Field ADDRESS */
#define IMX_GPMI_PAYLOAD_ADDRESS_SHIFT                      2
#define IMX_GPMI_PAYLOAD_ADDRESS_MASK                       0xFFFFFFFC
#define IMX_GPMI_PAYLOAD_ADDRESS(v)                         (((v) << 2) & IMX_GPMI_PAYLOAD_ADDRESS_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned RSVD0:2;
        unsigned ADDRESS:30;
    } B;
} imx_gpmi_payload_t;



/* Register AUXILIARY Auxiliary Address Register */
#define IMX_GPMI_AUXILIARY                                  0x50
/* Field ADDRESS */
#define IMX_GPMI_AUXILIARY_ADDRESS_SHIFT                    2
#define IMX_GPMI_AUXILIARY_ADDRESS_MASK                     0xFFFFFFFC
#define IMX_GPMI_AUXILIARY_ADDRESS(v)                       (((v) << 2) & IMX_GPMI_AUXILIARY_ADDRESS_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned RSVD0:2;
        unsigned ADDRESS:30;
    } B;
} imx_gpmi_auxiliary_t;

/* Register CTRL1 Control Register 1 */
#define IMX_GPMI_CTRL1                                      0x60
#define IMX_GPMI_CTRL1_SET                                  0x64
#define IMX_GPMI_CTRL1_CLR                                  0x68
#define IMX_GPMI_CTRL1_TOG                                  0x6C
/* DEV_CLK_STOP */
#define IMX_GPMI_CTRL1_DEV_CLK_STOP_SHIFT                   31
#define IMX_GPMI_CTRL1_DEV_CLK_STOP_MASK                    0x80000000
#define IMX_GPMI_CTRL1_DEV_CLK_STOP(v)                      ((((uint32_t) v) << 31) & IMX_GPMI_CTRL1_DEV_CLK_STOP_MASK)
/* SSYNC_CLK_STOP */
#define IMX_GPMI_CTRL1_SSYNC_CLK_STOP_SHIFT                 30
#define IMX_GPMI_CTRL1_SSYNC_CLK_STOP_MASK                  0x40000000
#define IMX_GPMI_CTRL1_SSYNC_CLK_STOP(v)                    (((v) << 30) & IMX_GPMI_CTRL1_SSYNC_CLK_STOP_MASK)
/* WRITE_CLK_STOP */
#define IMX_GPMI_CTRL1_WRITE_CLK_STOP_SHIFT                 29
#define IMX_GPMI_CTRL1_WRITE_CLK_STOP_MASK                  0x20000000
#define IMX_GPMI_CTRL1_WRITE_CLK_STOP(v)                    (((v) << 29) & IMX_GPMI_CTRL1_WRITE_CLK_STOP_MASK)
/* TOGGLE_MODE */
#define IMX_GPMI_CTRL1_TOGGLE_MODE_SHIFT                    28
#define IMX_GPMI_CTRL1_TOGGLE_MODE_MASK                     0x10000000
#define IMX_GPMI_CTRL1_TOGGLE_MODE(v)                       (((v) << 28) & IMX_GPMI_CTRL1_TOGGLE_MODE_MASK)
/* GPMI_CLK_DIV2_EN */
#define IMX_GPMI_CTRL1_GPMI_CLK_DIV2_EN_SHIFT               27
#define IMX_GPMI_CTRL1_GPMI_CLK_DIV2_EN_MASK                0x8000000
#define IMX_GPMI_CTRL1_GPMI_CLK_DIV2_EN(v)                  (((v) << 27) & IMX_GPMI_CTRL1_GPMI_CLK_DIV2_EN_MASK)
/* UPDATE_CS */
#define IMX_GPMI_CTRL1_UPDATE_CS_SHIFT                      26
#define IMX_GPMI_CTRL1_UPDATE_CS_MASK                       0x4000000
#define IMX_GPMI_CTRL1_UPDATE_CS(v)                         (((v) << 26) & IMX_GPMI_CTRL1_UPDATE_CS_MASK)
/* SSYNCMODE */
#define IMX_GPMI_CTRL1_SSYNCMODE_SHIFT                      25
#define IMX_GPMI_CTRL1_SSYNCMODE_MASK                       0x2000000
#define IMX_GPMI_CTRL1_SSYNCMODE(v)                         (((v) << 25) & IMX_GPMI_CTRL1_SSYNCMODE_MASK)
#define IMX_GPMI_CTRL1_SSYNCMODE_BV_ASYNC                   0x0
#define IMX_GPMI_CTRL1_SSYNCMODE_BV_SSYNC                   0x1
/* DECOUPLE_CS */
#define IMX_GPMI_CTRL1_DECOUPLE_CS_SHIFT                    24
#define IMX_GPMI_CTRL1_DECOUPLE_CS_MASK                     0x1000000
#define IMX_GPMI_CTRL1_DECOUPLE_CS(v)                       (((v) << 24) & IMX_GPMI_CTRL1_DECOUPLE_CS_MASK)
/* WRN_DLY_SEL */
#define IMX_GPMI_CTRL1_WRN_DLY_SEL_SHIFT                    22
#define IMX_GPMI_CTRL1_WRN_DLY_SEL_MASK                     0xC00000
#define IMX_GPMI_CTRL1_WRN_DLY_SEL(v)                       (((v) << 22) & IMX_GPMI_CTRL1_WRN_DLY_SEL_MASK)
/* TIMEOUT_IRQ_EN */
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ_EN_SHIFT                 20
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ_EN_MASK                  0x100000
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ_EN(v)                    (((v) << 20) & IMX_GPMI_CTRL1_TIMEOUT_IRQ_EN_MASK)
/* GANGED_RDYBUSY */
#define IMX_GPMI_CTRL1_GANGED_RDYBUSY_SHIFT                 19
#define IMX_GPMI_CTRL1_GANGED_RDYBUSY_MASK                  0x80000
#define IMX_GPMI_CTRL1_GANGED_RDYBUSY(v)                    (((v) << 19) & IMX_GPMI_CTRL1_GANGED_RDYBUSY_MASK)
/* BCH_MODE */
#define IMX_GPMI_CTRL1_BCH_MODE_SHIFT                       18
#define IMX_GPMI_CTRL1_BCH_MODE_MASK                        0x40000
#define IMX_GPMI_CTRL1_BCH_MODE(v)                          (((v) << 18) & IMX_GPMI_CTRL1_BCH_MODE_MASK)
/* DLL_ENABLE */
#define IMX_GPMI_CTRL1_DLL_ENABLE_SHIFT                     17
#define IMX_GPMI_CTRL1_DLL_ENABLE_MASK                      0x20000
#define IMX_GPMI_CTRL1_DLL_ENABLE(v)                        (((v) << 17) & IMX_GPMI_CTRL1_DLL_ENABLE_MASK)
/* HALF_PERIOD */
#define IMX_GPMI_CTRL1_HALF_PERIOD_SHIFT                    16
#define IMX_GPMI_CTRL1_HALF_PERIOD_MASK                     0x10000
#define IMX_GPMI_CTRL1_HALF_PERIOD(v)                       (((v) << 16) & IMX_GPMI_CTRL1_HALF_PERIOD_MASK)
/* RDN_DELAY */
#define IMX_GPMI_CTRL1_RDN_DELAY_SHIFT                      12
#define IMX_GPMI_CTRL1_RDN_DELAY_MASK                       0xF000
#define IMX_GPMI_CTRL1_RDN_DELAY(v)                         (((v) << 12) & IMX_GPMI_CTRL1_RDN_DELAY_MASK)
/* DMA2ECC_MODE */
#define IMX_GPMI_CTRL1_DMA2ECC_MODE_SHIFT                   11
#define IMX_GPMI_CTRL1_DMA2ECC_MODE_MASK                    0x800
#define IMX_GPMI_CTRL1_DMA2ECC_MODE(v)                      (((v) << 11) & IMX_GPMI_CTRL1_DMA2ECC_MODE_MASK)
/* DEV_IRQ */
#define IMX_GPMI_CTRL1_DEV_IRQ_SHIFT                        10
#define IMX_GPMI_CTRL1_DEV_IRQ_MASK                         0x400
#define IMX_GPMI_CTRL1_DEV_IRQ(v)                           (((v) << 10) & IMX_GPMI_CTRL1_DEV_IRQ_MASK)
/* TIMEOUT_IRQ */
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ_SHIFT                    9
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ_MASK                     0x200
#define IMX_GPMI_CTRL1_TIMEOUT_IRQ(v)                       (((v) << 9) & IMX_GPMI_CTRL1_TIMEOUT_IRQ_MASK)
/* BURST_EN */
#define IMX_GPMI_CTRL1_BURST_EN_SHIFT                       8
#define IMX_GPMI_CTRL1_BURST_EN_MASK                        0x100
#define IMX_GPMI_CTRL1_BURST_EN(v)                          (((v) << 8) & IMX_GPMI_CTRL1_BURST_EN_MASK)
/* ABORT_WAIT_REQUEST */
#define IMX_GPMI_CTRL1_ABORT_WAIT_REQUEST_SHIFT             7
#define IMX_GPMI_CTRL1_ABORT_WAIT_REQUEST_MASK              0x80
#define IMX_GPMI_CTRL1_ABORT_WAIT_REQUEST(v)                (((v) << 7) & IMX_GPMI_CTRL1_ABORT_WAIT_REQUEST_MASK)
/* ABORT_WAIT_FOR_READY_CHANNEL */
#define IMX_GPMI_CTRL1_ABORT_WAIT_FOR_READY_CHANNEL_SHIFT   4
#define IMX_GPMI_CTRL1_ABORT_WAIT_FOR_READY_CHANNEL_MASK    0x70
#define IMX_GPMI_CTRL1_ABORT_WAIT_FOR_READY_CHANNEL(v)      (((v) << 4) & IMX_GPMI_CTRL1_ABORT_WAIT_FOR_READY_CHANNEL_MASK)
/* DEV_RESET */
#define IMX_GPMI_CTRL1_DEV_RESET_SHIFT                      3
#define IMX_GPMI_CTRL1_DEV_RESET_MASK                       0x8
#define IMX_GPMI_CTRL1_DEV_RESET(v)                         (((v) << 3) & IMX_GPMI_CTRL1_DEV_RESET_MASK)
#define IMX_GPMI_CTRL1_DEV_RESET_BV_ENABLED                 0x0
#define IMX_GPMI_CTRL1_DEV_RESET_BV_DISABLED                0x1
/* ATA_IRQRDY_POLARITY */
#define IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY_SHIFT            2
#define IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY_MASK             0x4
#define IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY(v)               (((v) << 2) & IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY_MASK)
#define IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY_BV_ACTIVELOW     0x0
#define IMX_GPMI_CTRL1_ATA_IRQRDY_POLARITY_BV_ACTIVEHIGH    0x1
/* Field CAMERA_MODE */
#define IMX_GPMI_CTRL1_CAMERA_MODE_SHIFT                    1
#define IMX_GPMI_CTRL1_CAMERA_MODE_MASK                     0x2
#define IMX_GPMI_CTRL1_CAMERA_MODE(v)                       (((v) << 1) & IMX_GPMI_CTRL1_CAMERA_MODE_MASK)
/* Field GPMI_MODE */
#define IMX_GPMI_CTRL1_GPMI_MODE_SHIFT                      0
#define IMX_GPMI_CTRL1_GPMI_MODE_MASK                       0x1
#define IMX_GPMI_CTRL1_GPMI_MODE(v)                         (((v) << 0) & IMX_GPMI_CTRL1_GPMI_MODE_MASK)
#define IMX_GPMI_CTRL1_GPMI_MODE_BV_NAND                    0x0
#define IMX_GPMI_CTRL1_GPMI_MODE_BV_ATA                     0x1
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned GPMI_MODE:1;
        unsigned CAMERA_MODE:1;
        unsigned ATA_IRQRDY_POLARITY:1;
        unsigned DEV_RESET:1;
        unsigned ABORT_WAIT_FOR_READY_CHANNEL:3;
        unsigned ABORT_WAIT_REQUEST:1;
        unsigned BURST_EN:1;
        unsigned TIMEOUT_IRQ:1;
        unsigned DEV_IRQ:1;
        unsigned DMA2ECC_MODE:1;
        unsigned RDN_DELAY:4;
        unsigned HALF_PERIOD:1;
        unsigned DLL_ENABLE:1;
        unsigned BCH_MODE:1;
        unsigned GANGED_RDYBUSY:1;
        unsigned TIMEOUT_IRQ_EN:1;
        unsigned RSVD1:1;
        unsigned WRN_DLY_SEL:2;
        unsigned DECOUPLE_CS:1;
        unsigned SSYNCMODE:1;
        unsigned UPDATE_CS:1;
        unsigned GPMI_CLK_DIV2_EN:1;
        unsigned TOGGLE_MODE:1;
        unsigned WRITE_CLK_STOP:1;
        unsigned SSYNC_CLK_STOP:1;
        unsigned DEV_CLK_STOP:1;
    } B;
} imx_gpmi_ctrl1_t;



/* Register GPMI_TIMING0 Timing Register 0 */
 #define IMX_GPMI_TIMING0                                   0x70
/* Field ADDRESS_SETUP */
#define IMX_GPMI_TIMING0_ADDRESS_SETUP_SHIFT                16
#define IMX_GPMI_TIMING0_ADDRESS_SETUP_MASK                 0xFF0000
#define IMX_GPMI_TIMING0_ADDRESS_SETUP(v)                   (((v) << 16) & IMX_GPMI_TIMING0_ADDRESS_SETUP_MASK)
/* Field DATA_HOLD */
#define IMX_GPMI_TIMING0_DATA_HOLD_SHIFT                    8
#define IMX_GPMI_TIMING0_DATA_HOLD_MASK                     0xFF00
#define IMX_GPMI_TIMING0_DATA_HOLD(v)                       (((v) << 8) & IMX_GPMI_TIMING0_DATA_HOLD_MASK)
/* Field DATA_SETUP */
#define IMX_GPMI_TIMING0_DATA_SETUP_SHIFT                   0
#define IMX_GPMI_TIMING0_DATA_SETUP_MASK                    0xFF
#define IMX_GPMI_TIMING0_DATA_SETUP(v)                      (((v) << 0) & IMX_GPMI_TIMING0_DATA_SETUP_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATA_SETUP:8;
        unsigned DATA_HOLD:8;
        unsigned ADDRESS_SETUP:8;
        unsigned RSVD1:8;
    } B;
} imx_gpmi_timing0_t;



/* Register TIMING1 Timing Register 1 */
#define IMX_GPMI_TIMING1                                    0x80
/* Field DEVICE_BUSY_TIMEOUT */
#define IMX_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT_SHIFT          16
#define IMX_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT_MASK           0xFFFF0000
#define IMX_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT(v)             ((((uint32_t) v) << 16) & IMX_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT_MASK)
/* Register type  */
typedef union {
    uint32_t U;
    struct {
        unsigned RSVD1:16;
        unsigned DEVICE_BUSY_TIMEOUT:16;
    } B;
} imx_gpmi_timing1_t;




/* Register TIMING2 Timing Register 2 */
#define IMX_GPMI_TIMING2                                    0x90
/* READ_LATENCY */
#define IMX_GPMI_TIMING2_READ_LATENCY_SHIFT                 24
#define IMX_GPMI_TIMING2_READ_LATENCY_MASK                  0x7000000
#define IMX_GPMI_TIMING2_READ_LATENCY(v)                    (((v) << 24) & IMX_GPMI_TIMING2_READ_LATENCY_MASK)
/* CE_DELAY */
#define IMX_GPMI_TIMING2_CE_DELAY_SHIFT                     16
#define IMX_GPMI_TIMING2_CE_DELAY_MASK                      0x1F0000
#define IMX_GPMI_TIMING2_CE_DELAY(v)                        (((v) << 16) & IMX_GPMI_TIMING2_CE_DELAY_MASK)
/* PREAMBLE_DELAY */
#define IMX_GPMI_TIMING2_PREAMBLE_DELAY_SHIFT               12
#define IMX_GPMI_TIMING2_PREAMBLE_DELAY_MASK                0xF000
#define IMX_GPMI_TIMING2_PREAMBLE_DELAY(v)                  (((v) << 12) & IMX_GPMI_TIMING2_PREAMBLE_DELAY_MASK)
/* POSTAMBLE_DELAY */
#define IMX_GPMI_TIMING2_POSTAMBLE_DELAY_SHIFT              8
#define IMX_GPMI_TIMING2_POSTAMBLE_DELAY_MASK               0xF00
#define IMX_GPMI_TIMING2_POSTAMBLE_DELAY(v)                 (((v) << 8) & IMX_GPMI_TIMING2_POSTAMBLE_DELAY_MASK)
/* CMDADD_PAUSE */
#define IMX_GPMI_TIMING2_CMDADD_PAUSE_SHIFT                 4
#define IMX_GPMI_TIMING2_CMDADD_PAUSE_MASK                  0xF0
#define IMX_GPMI_TIMING2_CMDADD_PAUSE(v)                    (((v) << 4) & IMX_GPMI_TIMING2_CMDADD_PAUSE_MASK)
/* DATA_PAUSE */
#define IMX_GPMI_TIMING2_DATA_PAUSE_SHIFT                   0
#define IMX_GPMI_TIMING2_DATA_PAUSE_MASK                    0xF
#define IMX_GPMI_TIMING2_DATA_PAUSE(v)                      (((v) << 0) & IMX_GPMI_TIMING2_DATA_PAUSE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATA_PAUSE:4;
        unsigned CMDADD_PAUSE:4;
        unsigned POSTAMBLE_DELAY:4;
        unsigned PREAMBLE_DELAY:4;
        unsigned CE_DELAY:5;
        unsigned RSVD0:3;
        unsigned READ_LATENCY:3;
        unsigned RSVD1:5;
    } B;
} imx_gpmi_timing2_t;



/* Register DATA Data Transfer Register */
#define IMX_GPMI_DATA                                       0xA0
/* Field DATA */
#define IMX_GPMI_DATA_DATA_SHIFT                            0
#define IMX_GPMI_DATA_DATA_MASK                             0xFFFFFFFF
#define IMX_GPMI_DATA_DATA(v)                               ((uint32_t) v)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATA:32;
    } B;
} imx_gpmi_data_t;



/* Register STAT Status Register */
#define IMX_GPMI_STAT                                       0xB0
/* Field READY_BUSY */
#define IMX_GPMI_STAT_READY_BUSY_SHIFT                      24
#define IMX_GPMI_STAT_READY_BUSY_MASK                       0xFF000000
#define IMX_GPMI_STAT_READY_BUSY(v)                         ((((uint32_t) v) << 24) & IMX_GPMI_STAT_READY_BUSY_MASK)
/* RDY_TIMEOUT */
#define IMX_GPMI_STAT_RDY_TIMEOUT_SHIFT                     16
#define IMX_GPMI_STAT_RDY_TIMEOUT_MASK                      0xFF0000
#define IMX_GPMI_STAT_RDY_TIMEOUT(v)                        (((v) << 16) & IMX_GPMI_STAT_RDY_TIMEOUT_MASK)
/* DEV7_ERROR */
#define IMX_GPMI_STAT_DEV7_ERROR_SHIFT                      15
#define IMX_GPMI_STAT_DEV7_ERROR_MASK                       0x8000
#define IMX_GPMI_STAT_DEV7_ERROR(v)                         (((v) << 15) & IMX_GPMI_STAT_DEV7_ERROR_MASK)
/* DEV6_ERROR */
#define IMX_GPMI_STAT_DEV6_ERROR_SHIFT                      14
#define IMX_GPMI_STAT_DEV6_ERROR_MASK                       0x4000
#define IMX_GPMI_STAT_DEV6_ERROR(v)                         (((v) << 14) & IMX_GPMI_STAT_DEV6_ERROR_MASK)
/* DEV5_ERROR */
#define IMX_GPMI_STAT_DEV5_ERROR_SHIFT                      13
#define IMX_GPMI_STAT_DEV5_ERROR_MASK                       0x2000
#define IMX_GPMI_STAT_DEV5_ERROR(v)                         (((v) << 13) & IMX_GPMI_STAT_DEV5_ERROR_MASK)
/* DEV4_ERROR */
#define IMX_GPMI_STAT_DEV4_ERROR_SHIFT                      12
#define IMX_GPMI_STAT_DEV4_ERROR_MASK                       0x1000
#define IMX_GPMI_STAT_DEV4_ERROR(v)                         (((v) << 12) & IMX_GPMI_STAT_DEV4_ERROR_MASK)
/* DEV3_ERROR */
#define IMX_GPMI_STAT_DEV3_ERROR_SHIFT                      11
#define IMX_GPMI_STAT_DEV3_ERROR_MASK                       0x800
#define IMX_GPMI_STAT_DEV3_ERROR(v)                         (((v) << 11) & IMX_GPMI_STAT_DEV3_ERROR_MASK)
/* DEV2_ERROR */
#define IMX_GPMI_STAT_DEV2_ERROR_SHIFT                      10
#define IMX_GPMI_STAT_DEV2_ERROR_MASK                       0x400
#define IMX_GPMI_STAT_DEV2_ERROR(v)                         (((v) << 10) & IMX_GPMI_STAT_DEV2_ERROR_MASK)
/* DEV1_ERROR */
#define IMX_GPMI_STAT_DEV1_ERROR_SHIFT                      9
#define IMX_GPMI_STAT_DEV1_ERROR_MASK                       0x200
#define IMX_GPMI_STAT_DEV1_ERROR(v)                         (((v) << 9) & IMX_GPMI_STAT_DEV1_ERROR_MASK)
/* Field DEV0_ERROR */
#define IMX_GPMI_STAT_DEV0_ERROR_SHIFT                      8
#define IMX_GPMI_STAT_DEV0_ERROR_MASK                       0x100
#define IMX_GPMI_STAT_DEV0_ERROR(v)                         (((v) << 8) & IMX_GPMI_STAT_DEV0_ERROR_MASK)
/* Field ATA_IRQ */
#define IMX_GPMI_STAT_ATA_IRQ_SHIFT                         4
#define IMX_GPMI_STAT_ATA_IRQ_MASK                          0x10
#define IMX_GPMI_STAT_ATA_IRQ(v)                            (((v) << 4) & IMX_GPMI_STAT_ATA_IRQ_MASK)
/* Field INVALID_BUFFER_MASK */
#define IMX_GPMI_STAT_INVALID_BUFFER_MASK_SHIFT             3
#define IMX_GPMI_STAT_INVALID_BUFFER_MASK_MASK              0x8
#define IMX_GPMI_STAT_INVALID_BUFFER_MASK(v)                (((v) << 3) & IMX_GPMI_STAT_INVALID_BUFFER_MASK_MASK)
/* Field FIFO_EMPTY */
#define IMX_GPMI_STAT_FIFO_EMPTY_SHIFT                      2
#define IMX_GPMI_STAT_FIFO_EMPTY_MASK                       0x4
#define IMX_GPMI_STAT_FIFO_EMPTY(v)                         (((v) << 2) & IMX_GPMI_STAT_FIFO_EMPTY_MASK)
#define IMX_GPMI_STAT_FIFO_EMPTY_BV_NOT_EMPTY               0x0
#define IMX_GPMI_STAT_FIFO_EMPTY_BV_EMPTY                   0x1
/* Field FIFO_FULL */
#define IMX_GPMI_STAT_FIFO_FULL_SHIFT                       1
#define IMX_GPMI_STAT_FIFO_FULL_MASK                        0x2
#define IMX_GPMI_STAT_FIFO_FULL(v)                          (((v) << 1) & IMX_GPMI_STAT_FIFO_FULL_MASK)
#define IMX_GPMI_STAT_FIFO_FULL_BV_NOT_FULL                 0x0
#define IMX_GPMI_STAT_FIFO_FULL_BV_FULL                     0x1
/* Field PRESENT */
#define IMX_GPMI_STAT_PRESENT_SHIFT                         0
#define IMX_GPMI_STAT_PRESENT_MASK                          0x1
#define IMX_GPMI_STAT_PRESENT(v)                            (((v) << 0) & IMX_GPMI_STAT_PRESENT_MASK)
#define IMX_GPMI_STAT_PRESENT_BV_UNAVAILABLE                0x0
#define IMX_GPMI_STAT_PRESENT_BV_AVAILABLE                  0x1
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned PRESENT:1;
        unsigned FIFO_FULL:1;
        unsigned FIFO_EMPTY:1;
        unsigned INVALID_BUFFER_MASK:1;
        unsigned ATA_IRQ:1;
        unsigned RSVD1:3;
        unsigned DEV0_ERROR:1;
        unsigned DEV1_ERROR:1;
        unsigned DEV2_ERROR:1;
        unsigned DEV3_ERROR:1;
        unsigned DEV4_ERROR:1;
        unsigned DEV5_ERROR:1;
        unsigned DEV6_ERROR:1;
        unsigned DEV7_ERROR:1;
        unsigned RDY_TIMEOUT:8;
        unsigned READY_BUSY:8;
    } B;
} imx_gpmi_stat_t;



/* Register READ_DDR_DLL_CTRL Double Rate Read DLL Control Register */
#define IMX_GPMI_READ_DDR_DLL_CTRL                          0x100
/* Field REF_UPDATE_INT */
#define IMX_GPMI_READ_DDR_DLL_CTRL_REF_UPDATE_INT_SHIFT     28
#define IMX_GPMI_READ_DDR_DLL_CTRL_REF_UPDATE_INT_MASK      0xF0000000
#define IMX_GPMI_READ_DDR_DLL_CTRL_REF_UPDATE_INT(v)        ((((uint32_t) v) << 28) & IMX_GPMI_READ_DDR_DLL_CTRL_REF_UPDATE_INT_MASK)
/* Field SLV_UPDATE_INT */
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_UPDATE_INT_SHIFT     20
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_UPDATE_INT_MASK      0xFF00000
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_UPDATE_INT(v)        (((v) << 20) & IMX_GPMI_READ_DDR_DLL_CTRL_SLV_UPDATE_INT_MASK)
/* Field SLV_OVERRIDE_VAL */
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_SHIFT   10
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_MASK    0x3FC00
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_VAL(v)      (((v) << 10) & IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_MASK)
/* Field SLV_OVERRIDE */
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_SHIFT       9
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_MASK        0x200
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE(v)          (((v) << 9) & IMX_GPMI_READ_DDR_DLL_CTRL_SLV_OVERRIDE_MASK)
/* Field REFCLK_ON */
#define IMX_GPMI_READ_DDR_DLL_CTRL_REFCLK_ON_SHIFT          8
#define IMX_GPMI_READ_DDR_DLL_CTRL_REFCLK_ON_MASK           0x100
#define IMX_GPMI_READ_DDR_DLL_CTRL_REFCLK_ON(v)             (((v) << 8) & IMX_GPMI_READ_DDR_DLL_CTRL_REFCLK_ON_MASK)
/* Field GATE_UPDATE */
#define IMX_GPMI_READ_DDR_DLL_CTRL_GATE_UPDATE_SHIFT        7
#define IMX_GPMI_READ_DDR_DLL_CTRL_GATE_UPDATE_MASK         0x80
#define IMX_GPMI_READ_DDR_DLL_CTRL_GATE_UPDATE(v)           (((v) << 7) & IMX_GPMI_READ_DDR_DLL_CTRL_GATE_UPDATE_MASK)
/* Field SLV_DLY_TARGET */
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_DLY_TARGET_SHIFT     3
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_DLY_TARGET_MASK      0x78
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_DLY_TARGET(v)        (((v) << 3) & IMX_GPMI_READ_DDR_DLL_CTRL_SLV_DLY_TARGET_MASK)
/* Field SLV_FORCE_UPD */
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_FORCE_UPD_SHIFT      2
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_FORCE_UPD_MASK       0x4
#define IMX_GPMI_READ_DDR_DLL_CTRL_SLV_FORCE_UPD(v)         (((v) << 2) & IMX_GPMI_READ_DDR_DLL_CTRL_SLV_FORCE_UPD_MASK)
/* Field RESET */
#define IMX_GPMI_READ_DDR_DLL_CTRL_RESET_SHIFT              1
#define IMX_GPMI_READ_DDR_DLL_CTRL_RESET_MASK               0x2
#define IMX_GPMI_READ_DDR_DLL_CTRL_RESET(v)                 (((v) << 1) & IMX_GPMI_READ_DDR_DLL_CTRL_RESET_MASK)
/* Field ENABLE */
#define IMX_GPMI_READ_DDR_DLL_CTRL_ENABLE_SHIFT             0
#define IMX_GPMI_READ_DDR_DLL_CTRL_ENABLE_MASK              0x1
#define IMX_GPMI_READ_DDR_DLL_CTRL_ENABLE(v)                (((v) << 0) & IMX_GPMI_READ_DDR_DLL_CTRL_ENABLE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ENABLE:1;
        unsigned RESET:1;
        unsigned SLV_FORCE_UPD:1;
        unsigned SLV_DLY_TARGET:4;
        unsigned GATE_UPDATE:1;
        unsigned REFCLK_ON:1;
        unsigned SLV_OVERRIDE:1;
        unsigned SLV_OVERRIDE_VAL:8;
        unsigned RSVD1:2;
        unsigned SLV_UPDATE_INT:8;
        unsigned REF_UPDATE_INT:4;
    } B;
} imx_gpmi_read_ddr_dll_ctrl_t;



/* WRITE_DDR_DLL_CTRL Double Rate Write DLL Control Register */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL                         0x110
/* REF_UPDATE_INT */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REF_UPDATE_INT_SHIFT    28
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REF_UPDATE_INT_MASK     0xF0000000
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REF_UPDATE_INT(v)       ((((uint32_t) v) << 28) & IMX_GPMI_WRITE_DDR_DLL_CTRL_REF_UPDATE_INT_MASK)
/* SLV_UPDATE_INT */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_UPDATE_INT_SHIFT    20
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_UPDATE_INT_MASK     0xFF00000
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_UPDATE_INT(v)           (((v) << 20) & IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_UPDATE_INT_MASK)
/* SLV_OVERRIDE_VAL */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_SHIFT  10
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_MASK   0x3FC00
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_VAL(v)     (((v) << 10) & IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_VAL_MASK)
/* SLV_OVERRIDE */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_SHIFT      9
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_MASK       0x200
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE(v)         (((v) << 9) & IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_OVERRIDE_MASK)
/* REFCLK_ON */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REFCLK_ON_SHIFT         8
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REFCLK_ON_MASK          0x100
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_REFCLK_ON(v)            (((v) << 8) & IMX_GPMI_WRITE_DDR_DLL_CTRL_REFCLK_ON_MASK)
/* GATE_UPDATE */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_GATE_UPDATE_SHIFT       7
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_GATE_UPDATE_MASK        0x80
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_GATE_UPDATE(v)          (((v) << 7) & IMX_GPMI_WRITE_DDR_DLL_CTRL_GATE_UPDATE_MASK)
/* SLV_DLY_TARGET */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_DLY_TARGET_SHIFT    3
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_DLY_TARGET_MASK     0x78
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_DLY_TARGET(v)       (((v) << 3) & IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_DLY_TARGET_MASK)
/* SLV_FORCE_UPD */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_FORCE_UPD_SHIFT     2
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_FORCE_UPD_MASK      0x4
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_FORCE_UPD(v)        (((v) << 2) & IMX_GPMI_WRITE_DDR_DLL_CTRL_SLV_FORCE_UPD_MASK)
/* RESET */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_RESET_SHIFT             1
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_RESET_MASK              0x2
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_RESET(v)                (((v) << 1) & IMX_GPMI_WRITE_DDR_DLL_CTRL_RESET_MASK)
/* ENABLE */
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_ENABLE_SHIFT            0
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_ENABLE_MASK             0x1
#define IMX_GPMI_WRITE_DDR_DLL_CTRL_ENABLE(v)               (((v) << 0) & IMX_GPMI_WRITE_DDR_DLL_CTRL_ENABLE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ENABLE:1;
        unsigned RESET:1;
        unsigned SLV_FORCE_UPD:1;
        unsigned SLV_DLY_TARGET:4;
        unsigned GATE_UPDATE:1;
        unsigned REFCLK_ON:1;
        unsigned SLV_OVERRIDE:1;
        unsigned SLV_OVERRIDE_VAL:8;
        unsigned RSVD1:2;
        unsigned SLV_UPDATE_INT:8;
        unsigned REF_UPDATE_INT:4;
    } B;
} imx_gpmi_write_ddr_dll_ctrl_t;



/* READ_DDR_DLL_STS Double Rate Read DLL Status Register */
 #define IMX_GPMI_READ_DDR_DLL_STS_ADDR                     0x120
/* Field REF_SEL */
#define IMX_GPMI_READ_DDR_DLL_STS_REF_SEL_SHIFT             17
#define IMX_GPMI_READ_DDR_DLL_STS_REF_SEL_MASK              0x1FE0000
#define IMX_GPMI_READ_DDR_DLL_STS_REF_SEL(v)                (((v) << 17) & IMX_GPMI_READ_DDR_DLL_STS_REF_SEL_MASK)
/* Field REF_LOCK */
#define IMX_GPMI_READ_DDR_DLL_STS_REF_LOCK_SHIFT            16
#define IMX_GPMI_READ_DDR_DLL_STS_REF_LOCK_MASK             0x10000
#define IMX_GPMI_READ_DDR_DLL_STS_REF_LOCK(v)               (((v) << 16) & IMX_GPMI_READ_DDR_DLL_STS_REF_LOCK_MASK)
/* Field SLV_SEL */
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_SEL_SHIFT             1
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_SEL_MASK              0x1FE
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_SEL(v)                (((v) << 1) & IMX_GPMI_READ_DDR_DLL_STS_SLV_SEL_MASK)
/* Field SLV_LOCK */
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_LOCK_SHIFT            0
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_LOCK_MASK             0x1
#define IMX_GPMI_READ_DDR_DLL_STS_SLV_LOCK(v)               (((v) << 0) & IMX_GPMI_READ_DDR_DLL_STS_SLV_LOCK_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned SLV_LOCK:1;
        unsigned SLV_SEL:8;
        unsigned RSVD0:7;
        unsigned REF_LOCK:1;
        unsigned REF_SEL:8;
        unsigned RSVD1:7;
    } B;
} imx_gpmi_read_ddr_dll_sts_t;



/* Register WRITE_DDR_DLL_STS Double Rate Write DLL Status Register */
#define IMX_GPMI_WRITE_DDR_DLL_STS                          0x130
/* Field REF_SEL */
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_SEL_SHIFT            17
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_SEL_MASK             0x1FE0000
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_SEL(v)               (((v) << 17) & IMX_GPMI_WRITE_DDR_DLL_STS_REF_SEL_MASK)
/* Field REF_LOCK */
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_LOCK_SHIFT           16
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_LOCK_MASK            0x10000
#define IMX_GPMI_WRITE_DDR_DLL_STS_REF_LOCK(v)              (((v) << 16) & IMX_GPMI_WRITE_DDR_DLL_STS_REF_LOCK_MASK)
/* Field SLV_SEL */
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_SEL_SHIFT            1
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_SEL_MASK             0x1FE
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_SEL(v)               ((v) << 1) & IMX_GPMI_WRITE_DDR_DLL_STS_SLV_SEL_MASK)
/* Field SLV_LOCK */
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_LOCK_SHIFT           0
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_LOCK_MASK            0x1
#define IMX_GPMI_WRITE_DDR_DLL_STS_SLV_LOCK(v)              (((v) << 0) & IMX_GPMI_WRITE_DDR_DLL_STS_SLV_LOCK_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned SLV_LOCK:1;
        unsigned SLV_SEL:8;
        unsigned RSVD0:7;
        unsigned REF_LOCK:1;
        unsigned REF_SEL:8;
        unsigned RSVD1:7;
    } B;
} imx_gpmi_write_ddr_dll_sts_t;

#endif /* IMX_GPMI_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_gpmi.h $ $Rev: 869196 $")
#endif
