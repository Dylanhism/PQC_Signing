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

#ifndef IMX_BCH_H_
#define IMX_BCH_H_

#include <stdint.h>

/* Correcting ECC Accelerator (BCH) */

/* Register CTRL - Hardware BCH ECC Accelerator Control Register */
#define IMX_BCH_CTRL                                    0x0
#define IMX_BCH_CTRL_SET                                0x4
#define IMX_BCH_CTRL_CLR                                0x8
/* Field SFTRST */
#define IMX_BCH_CTRL_SFTRST_SHIFT                       31
#define IMX_BCH_CTRL_SFTRST_MASK                        0x80000000
#define IMX_BCH_CTRL_SFTRST(v)                          (((uint32_t) v) << 31) & IMX_BCH_CTRL_SFTRST_MASK)
#define IMX_BCH_CTRL_SFTRST_BV_RUN                      0x0
#define IMX_BCH_CTRL_SFTRST_BV_RESET                    0x1
/* Field CLKGATE */
#define IMX_BCH_CTRL_CLKGATE_SHIFT                      30
#define IMX_BCH_CTRL_CLKGATE_MASK                       0x40000000
#define IMX_BCH_CTRL_CLKGATE(v)                         (((v) << 30) & IMX_BCH_CTRL_CLKGATE_MASK)
#define IMX_BCH_CTRL_CLKGATE_BV_RUN                     0x0
#define IMX_BCH_CTRL_CLKGATE_BV_NO_CLKS                 0x1
/* Field DEBUGSYNDROME */
#define IMX_BCH_CTRL_DEBUGSYNDROME_SHIFT                22
#define IMX_BCH_CTRL_DEBUGSYNDROME_MASK                 0x400000
#define IMX_BCH_CTRL_DEBUGSYNDROME(v)                   (((v) << 22) & IMX_BCH_CTRL_DEBUGSYNDROME_MASK)
/* Field M2M_LAYOUT */
#define IMX_BCH_CTRL_M2M_LAYOUT_SHIFT                   18
#define IMX_BCH_CTRL_M2M_LAYOUT_MASK                    0xC0000
#define IMX_BCH_CTRL_M2M_LAYOUT(v)                      (((v) << 18) & IMX_BCH_CTRL_M2M_LAYOUT_MASK)
/* Field M2M_ENCODE */
#define IMX_BCH_CTRL_M2M_ENCODE_SHIFT                   17
#define IMX_BCH_CTRL_M2M_ENCODE_MASK                    0x20000
#define IMX_BCH_CTRL_M2M_ENCODE(v)                      (((v) << 17) & IMX_BCH_CTRL_M2M_ENCODE_MASK)
/* Field M2M_ENABLE */
#define IMX_BCH_CTRL_M2M_ENABLE_SHIFT                   16
#define IMX_BCH_CTRL_M2M_ENABLE_MASK                    0x10000
#define IMX_BCH_CTRL_M2M_ENABLE(v)                      (((v) << 16) & IMX_BCH_CTRL_M2M_ENABLE_MASK)
/* Field DEBUG_STALL_IRQ_EN */
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ_EN_SHIFT           10
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ_EN_MASK            0x400
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ_EN(v)              (((v) << 10) & IMX_BCH_CTRL_DEBUG_STALL_IRQ_EN_MASK)
/* Field COMPLETE_IRQ_EN */
#define IMX_BCH_CTRL_COMPLETE_IRQ_EN_SHIFT              8
#define IMX_BCH_CTRL_COMPLETE_IRQ_EN_MASK               0x100
#define IMX_BCH_CTRL_COMPLETE_IRQ_EN(v)                 (((v) << 8) & IMX_BCH_CTRL_COMPLETE_IRQ_EN_MASK)
/* Field ERROR_IRQ */
#define IMX_BCH_CTRL_ERROR_IRQ_SHIFT_MASK               3
#define IMX_BCH_CTRL_ERROR_IRQ_MASK                     0x8
#define IMX_BCH_CTRL_ERROR_IRQ(v_MASK)                  (((v) << 3) & IMX_BCH_CTRL_BM_ERROR_IRQ_MASK)
/* Field DEBUG_STALL_IRQ */
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ_SHIFT              2
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ_MASK               0x4
#define IMX_BCH_CTRL_DEBUG_STALL_IRQ(v)                 (((v) << 2) & IMX_BCH_CTRL_DEBUG_STALL_IRQ_MASK)
/* Field COMPLETE_IRQ */
#define IMX_BCH_CTRL_COMPLETE_IRQ_SHIFT                 0
#define IMX_BCH_CTRL_COMPLETE_IRQ_MASK                  0x1
#define IMX_BCH_CTRL_COMPLETE_IRQ(v)                    (((v) << 0) & IMX_BCH_CTRL_COMPLETE_IRQ_MASK)
/* Register types */
typedef union {
    uint32_t U;
    struct {
        unsigned COMPLETE_IRQ:1;
        unsigned RSVD0:1;
        unsigned DEBUG_STALL_IRQ:1;
        unsigned BM_ERROR_IRQ:1;
        unsigned RSVD1:4;
        unsigned COMPLETE_IRQ_EN:1;
        unsigned RSVD2:1;
        unsigned DEBUG_STALL_IRQ_EN:1;
        unsigned RSVD3:5;
        unsigned M2M_ENABLE:1;
        unsigned M2M_ENCODE:1;
        unsigned M2M_LAYOUT:2;
        unsigned RSVD4:2;
        unsigned DEBUGSYNDROME:1;
        unsigned RSVD5:7;
        unsigned CLKGATE:1;
        unsigned SFTRST:1;
    } B;
} imx_bch_ctrl_t;



/* Register IMX_BCH_STATUS0 - Hardware ECC Accelerator Status Register 0 */
#define IMX_BCH_STATUS0                                 0x10
/* Field HANDLE */
#define IMX_BCH_STATUS0_HANDLE_SHIFT                    20
#define IMX_BCH_STATUS0_HANDLE_MASK                     0xFFF00000
#define IMX_BCH_STATUS0_HANDLE(v)                       ((((uint32_t) v) << 20) & IMX_BCH_STATUS0_HANDLE_MASK)
/* Field COMPLETED_CE */
#define IMX_BCH_STATUS0_COMPLETED_CE_SHIFT              16
#define IMX_BCH_STATUS0_COMPLETED_CE_MASK               0xF0000
#define IMX_BCH_STATUS0_COMPLETED_CE(v)                 (((v) << 16) & IMX_BCH_STATUS0_COMPLETED_CE_MASK)
/* Field STATUS_BLK0 */
#define IMX_BCH_STATUS0_STATUS_BLK0_SHIFT               8
#define IMX_BCH_STATUS0_STATUS_BLK0_MASK                0xFF00
#define IMX_BCH_STATUS0_STATUS_BLK0(v)                  (((v) << 8) & IMX_BCH_STATUS0_STATUS_BLK0_MASK)
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ZERO             0x0
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ERROR1           0x1
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ERROR2           0x2
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ERROR3           0x3
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ERROR4           0x4
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_UNCORRECTABLE    0xFE
#define IMX_BCH_STATUS0_STATUS_BLK0_BV_ERASED           0xFF
/* Field ALLONES */
#define IMX_BCH_STATUS0_ALLONES_SHIFT                   4
#define IMX_BCH_STATUS0_ALLONES_MASK                    0x10
#define IMX_BCH_STATUS0_ALLONES(v)                      (((v) << 4) & IMX_BCH_STATUS0_ALLONES_MASK)
/* Field CORRECTED */
#define IMX_BCH_STATUS0_CORRECTED_SHIFT                 3
#define IMX_BCH_STATUS0_CORRECTED_MASK                  0x8
#define IMX_BCH_STATUS0_CORRECTED(v)                    (((v) << 3) & IMX_BCH_STATUS0_CORRECTED_MASK)
/* Field UNCORRECTABLE */
#define IMX_BCH_STATUS0_UNCORRECTABLE_SHIFT             2
#define IMX_BCH_STATUS0_UNCORRECTABLE_MASK              0x4
#define IMX_BCH_STATUS0_UNCORRECTABLE(v)                (((v) << 2) & IMX_BCH_STATUS0_UNCORRECTABLE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned RSVD0:2;
        unsigned UNCORRECTABLE:1;
        unsigned CORRECTED:1;
        unsigned ALLONES:1;
        unsigned RSVD1:3;
        unsigned STATUS_BLK0:8;
        unsigned COMPLETED_CE:4;
        unsigned HANDLE:12;
    } B;
} imx_bch_status0_t;



/* Register MODE - Hardware ECC Accelerator Mode Register */
#define IMX_BCH_MODE                                    0x20
/* Field ERASE_THRESHOLD */
#define IMX_BCH_MODE_ERASE_THRESHOLD_SHIFT              0
#define IMX_BCH_MODE_ERASE_THRESHOLD_MASK               0xFF
#define IMX_BCH_MODE_ERASE_THRESHOLD(v)                 (((v) << 0) & IMX_BCH_MODE_ERASE_THRESHOLD_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ERASE_THRESHOLD:8;
        unsigned RSVD:24;
    } B;
} imx_bch_mode_t;



/* Register ENCODEPTR - Hardware BCH ECC Loop-back Encode Buffer Register */
#define IMX_BCH_ENCODEPTR                               0x30
/* Field ADDR */
#define IMX_BCH_ENCODEPTR_ADDR_SHIFT                    0
#define IMX_BCH_ENCODEPTR_ADDR_MASK                     0xFFFFFFFF
#define IMX_BCH_ENCODEPTR_ADDR(v)                       ((uint32_t) v)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ADDR:32;
    } B;
} imx_bch_encodeptr_t;



/* Register DATAPTR - Hardware BCH ECC Loop-back Data Buffer Register */
#define IMX_BCH_DATAPTR                                 0x40
/* Field ADDR */
#define IMX_BCH_DATAPTR_ADDR_SHIFT                      0
#define IMX_BCH_DATAPTR_ADDR_MASK                       0xFFFFFFFF
#define IMX_BCH_DATAPTR_ADDR(v)                         ((uint32_t) v)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ADDR:32;
    } B;
} imx_bch_dataptr_t;



/* Register METAPTR - Hardware BCH ECC Loop-back Meta-data Buffer Register */
#define IMX_BCH_METAPTR                                 0x50
/* Field ADDR */
#define IMX_BCH_METAPTR_ADDR_SHIFT                      0
#define IMX_BCH_METAPTR_ADDR_MASK                       0xFFFFFFFF
#define IMX_BCH_METAPTR_ADDR(v)                         ((uint32_t) v)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned ADDR:32;
    } B;
} imx_bch_metaptr_t;



/* Register LAYOUTSELECT - Hardware ECC Accelerator Layout Select Register  */
#define IMX_BCH_LAYOUTSELECT                            0x70
/* Field CS15_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS15_SELECT_SHIFT          30
#define IMX_BCH_LAYOUTSELECT_CS15_SELECT_MASK           0xC0000000
#define IMX_BCH_LAYOUTSELECT_CS15_SELECT(v)             ((((uint32_t) v) << 30) & IMX_BCH_LAYOUTSELECT_CS15_SELECT_MASK)
/* Field CS14_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS14_SELECT_SHIFT          28
#define IMX_BCH_LAYOUTSELECT_CS14_SELECT_MASK           0x30000000
#define IMX_BCH_LAYOUTSELECT_CS14_SELECT(v)             (((v) << 28) & IMX_BCH_LAYOUTSELECT_CS14_SELECT_MASK)
/* Field CS13_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS13_SELECT_SHIFT          26
#define IMX_BCH_LAYOUTSELECT_CS13_SELECT_MASK           0xC000000
#define IMX_BCH_LAYOUTSELECT_CS13_SELECT(v)             (((v) << 26) & IMX_BCH_LAYOUTSELECT_CS13_SELECT_MASK)
/* Field CS12_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS12_SELECT_SHIFT          24
#define IMX_BCH_LAYOUTSELECT_CS12_SELECT_MASK           0x3000000
#define IMX_BCH_LAYOUTSELECT_CS12_SELECT(v)             (((v) << 24) & IMX_BCH_LAYOUTSELECT_CS12_SELECT_MASK)
/* Field CS11_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS11_SELECT_SHIFT          22
#define IMX_BCH_LAYOUTSELECT_CS11_SELECT_MASK           0xC00000
#define IMX_BCH_LAYOUTSELECT_CS11_SELECT(v)             (((v) << 22) & IMX_BCH_LAYOUTSELECT_CS11_SELECT_MASK)
/* Field CS10_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS10_SELECT_SHIFT          20
#define IMX_BCH_LAYOUTSELECT_CS10_SELECT_MASK           0x300000
#define IMX_BCH_LAYOUTSELECT_CS10_SELECT(v)             (((v) << 20) & IMX_BCH_LAYOUTSELECT_CS10_SELECT_MASK)
/* Field CS9_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS9_SELECT_SHIFT           18
#define IMX_BCH_LAYOUTSELECT_CS9_SELECT_MASK            0xC0000
#define IMX_BCH_LAYOUTSELECT_CS9_SELECT(v)              (((v) << 18) & IMX_BCH_LAYOUTSELECT_CS9_SELECT_MASK)
/* Field CS8_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS8_SELECT_SHIFT           16
#define IMX_BCH_LAYOUTSELECT_CS8_SELECT_MASK            0x30000
#define IMX_BCH_LAYOUTSELECT_CS8_SELECT(v)              (((v) << 16) & IMX_BCH_LAYOUTSELECT_CS8_SELECT_MASK)
/* Field CS7_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS7_SELECT_SHIFT           14
#define IMX_BCH_LAYOUTSELECT_CS7_SELECT_MASK            0xC000
#define IMX_BCH_LAYOUTSELECT_CS7_SELECT(v)              (((v) << 14) & IMX_BCH_LAYOUTSELECT_CS7_SELECT_MASK)
/* Field CS6_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS6_SELECT_SHIFT           12
#define IMX_BCH_LAYOUTSELECT_CS6_SELECT_MASK            0x3000
#define IMX_BCH_LAYOUTSELECT_CS6_SELECT(v)              (((v) << 12) & IMX_BCH_LAYOUTSELECT_CS6_SELECT_MASK)
/* Field CS5_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS5_SELECT_SHIFT           10
#define IMX_BCH_LAYOUTSELECT_CS5_SELECT_MASK            0xC00
#define IMX_BCH_LAYOUTSELECT_CS5_SELECT(v)              (((v) << 10) & IMX_BCH_LAYOUTSELECT_CS5_SELECT_MASK)
/* Field CS4_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS4_SELECT_SHIFT           8
#define IMX_BCH_LAYOUTSELECT_CS4_SELECT_MASK            0x300
#define IMX_BCH_LAYOUTSELECT_CS4_SELECT(v)              (((v) << 8) & IMX_BCH_LAYOUTSELECT_CS4_SELECT_MASK)
/* Field CS3_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS3_SELECT_SHIFT           6
#define IMX_BCH_LAYOUTSELECT_CS3_SELECT_MASK            0xC0
#define IMX_BCH_LAYOUTSELECT_CS3_SELECT(v)              (((v) << 6) & IMX_BCH_LAYOUTSELECT_CS3_SELECT_MASK)
/* Field CS2_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS2_SELECT_SHIFT           4
#define IMX_BCH_LAYOUTSELECT_CS2_SELECT_MASK            0x30
#define IMX_BCH_LAYOUTSELECT_CS2_SELECT(v)              (((v) << 4) & IMX_BCH_LAYOUTSELECT_CS2_SELECT_MASK)
/* Field CS1_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS1_SELECT_SHIFT           2
#define IMX_BCH_LAYOUTSELECT_CS1_SELECT_MASK            0xC
#define IMX_BCH_LAYOUTSELECT_CS1_SELECT(v)              (((v) << 2) & IMX_BCH_LAYOUTSELECT_CS1_SELECT_MASK)
/* Field CS0_SELECT */
#define IMX_BCH_LAYOUTSELECT_CS0_SELECT_SHIFT           0
#define IMX_BCH_LAYOUTSELECT_CS0_SELECT_MASK            0x3
#define IMX_BCH_LAYOUTSELECT_CS0_SELECT(v)              (((v) << 0) & IMX_BCH_LAYOUTSELECT_CS0_SELECT_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned CS0_SELECT:2;
        unsigned CS1_SELECT:2;
        unsigned CS2_SELECT:2;
        unsigned CS3_SELECT:2;
        unsigned CS4_SELECT:2;
        unsigned CS5_SELECT:2;
        unsigned CS6_SELECT:2;
        unsigned CS7_SELECT:2;
        unsigned CS8_SELECT:2;
        unsigned CS9_SELECT:2;
        unsigned CS10_SELECT:2;
        unsigned CS11_SELECT:2;
        unsigned CS12_SELECT:2;
        unsigned CS13_SELECT:2;
        unsigned CS14_SELECT:2;
        unsigned CS15_SELECT:2;
    } B;
} imx_bch_layoutselect_t;



/* IMX_BCH_FLASH0LAYOUT0 - Hardware BCH ECC Flash 0 Layout 0 Register */
#define IMX_BCH_FLASH0LAYOUT0                           0x80
/* Field NBLOCKS */
#define IMX_BCH_FLASH0LAYOUT0_NBLOCKS_SHIFT             24
#define IMX_BCH_FLASH0LAYOUT0_NBLOCKS_MASK              0xFF000000
#define IMX_BCH_FLASH0LAYOUT0_NBLOCKS(v)                ((((uint32_t) v) << 24) & IMX_BCH_FLASH0LAYOUT0_NBLOCKS_MASK)
/* Field META_SIZE */
#define IMX_BCH_FLASH0LAYOUT0_META_SIZE_SHIFT           16
#define IMX_BCH_FLASH0LAYOUT0_META_SIZE_MASK            0xFF0000
#define IMX_BCH_FLASH0LAYOUT0_META_SIZE(v)              (((v) << 16) & IMX_BCH_FLASH0LAYOUT0_META_SIZE_MASK)
/* Field ECC0 */
#define IMX_BCH_FLASH0LAYOUT0_ECC0_SHIFT                11
#define IMX_BCH_FLASH0LAYOUT0_ECC0_MASK                 0xF800
#define IMX_BCH_FLASH0LAYOUT0_ECC0(v)                   (((v) << 11) & IMX_BCH_FLASH0LAYOUT0_ECC0_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH0LAYOUT0_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH0LAYOUT0_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH0LAYOUT0_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH0LAYOUT0_GF13_0_GF14_1_MASK)
/* Field DATA0_SIZE */
#define IMX_BCH_FLASH0LAYOUT0_DATA0_SIZE_SHIFT          0
#define IMX_BCH_FLASH0LAYOUT0_DATA0_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH0LAYOUT0_DATA0_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH0LAYOUT0_DATA0_SIZE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATA0_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECC0:5;
        unsigned META_SIZE:8;
        unsigned NBLOCKS:8;
    } B;
} imx_bch_flash0layout0_t;



/* Register FLASH0LAYOUT1 - Hardware BCH ECC Flash 0 Layout 1 Register */
#define IMX_BCH_FLASH0LAYOUT1                           0x90
/* Field PAGE_SIZE */
#define IMX_BCH_FLASH0LAYOUT1_PAGE_SIZE_SHIFT           16
#define IMX_BCH_FLASH0LAYOUT1_PAGE_SIZE_MASK            0xFFFF0000
#define IMX_BCH_FLASH0LAYOUT1_PAGE_SIZE(v)              ((((uint32_t) v) << 16) & IMX_BCH_FLASH0LAYOUT1_PAGE_SIZE_MASK)
/* Field ECCN */
#define IMX_BCH_FLASH0LAYOUT1_ECCN_SHIFT                11
#define IMX_BCH_FLASH0LAYOUT1_ECCN_MASK                 0xF800
#define IMX_BCH_FLASH0LAYOUT1_ECCN(v)                   (((v) << 11) & IMX_BCH_FLASH0LAYOUT1_ECCN_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH0LAYOUT1_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH0LAYOUT1_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH0LAYOUT1_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH0LAYOUT1_GF13_0_GF14_1_MASK)
/* Field DATAN_SIZE */
#define IMX_BCH_FLASH0LAYOUT1_DATAN_SIZE_SHIFT          0
#define IMX_BCH_FLASH0LAYOUT1_DATAN_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH0LAYOUT1_DATAN_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH0LAYOUT1_DATAN_SIZE_MASK)
typedef union {
    uint32_t U;
    struct {
        unsigned DATAN_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECCN:5;
        unsigned PAGE_SIZE:16;
    } B;
} imx_bch_flash0layout1_t;



/* Register FLASH1LAYOUT0 - Hardware BCH ECC Flash 1 Layout 0 Register */
#define IMX_BCH_FLASH1LAYOUT0                           0xA0
/* Field NBLOCKS */
#define IMX_BCH_FLASH1LAYOUT0_NBLOCKS_SHIFT             24
#define IMX_BCH_FLASH1LAYOUT0_NBLOCKS_MASK              0xFF000000
#define IMX_BCH_FLASH1LAYOUT0_NBLOCKS(v)                ((((uint32_t) v) << 24) & IMX_BCH_FLASH1LAYOUT0_NBLOCKS_MASK)
/* Field META_SIZE */
#define IMX_BCH_FLASH1LAYOUT0_META_SIZE_SHIFT           16
#define IMX_BCH_FLASH1LAYOUT0_META_SIZE_MASK            0xFF0000
#define IMX_BCH_FLASH1LAYOUT0_META_SIZE(v)              (((v) << 16) & IMX_BCH_FLASH1LAYOUT0_META_SIZE_MASK)
/* Field ECC0 */
#define IMX_BCH_FLASH1LAYOUT0_ECC0_SHIFT                11
#define IMX_BCH_FLASH1LAYOUT0_ECC0_MASK                 0xF800
#define IMX_BCH_FLASH1LAYOUT0_ECC0(v)                   (((v) << 11) & IMX_BCH_FLASH1LAYOUT0_ECC0_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH1LAYOUT0_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH1LAYOUT0_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH1LAYOUT0_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH1LAYOUT0_GF13_0_GF14_1_MASK)
/* Field DATA0_SIZE */
#define IMX_BCH_FLASH1LAYOUT0_DATA0_SIZE_SHIFT          0
#define IMX_BCH_FLASH1LAYOUT0_DATA0_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH1LAYOUT0_DATA0_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH1LAYOUT0_DATA0_SIZE_MASK)
typedef union {
    uint32_t U;
    struct {
        unsigned DATA0_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECC0:5;
        unsigned META_SIZE:8;
        unsigned NBLOCKS:8;
    } B;
} imx_bch_flash1layout0_t;



/* Register FLASH1LAYOUT1 - Hardware BCH ECC Flash 1 Layout 1 Register */
#define IMX_BCH_FLASH1LAYOUT1                           0xB0
/* Field PAGE_SIZE */
#define IMX_BCH_FLASH1LAYOUT1_PAGE_SIZE_SHIFT           16
#define IMX_BCH_FLASH1LAYOUT1_PAGE_SIZE_MASK            0xFFFF0000
#define IMX_BCH_FLASH1LAYOUT1_PAGE_SIZE(v)              ((((uint32_t) v) << 16) & IMX_BCH_FLASH1LAYOUT1_PAGE_SIZE_MASK)
/* Field ECCN */
#define IMX_BCH_FLASH1LAYOUT1_ECCN_SHIFT                11
#define IMX_BCH_FLASH1LAYOUT1_ECCN_MASK                 0xF800
#define IMX_BCH_FLASH1LAYOUT1_ECCN(v)                   (((v) << 11) & IMX_BCH_FLASH1LAYOUT1_ECCN_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH1LAYOUT1_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH1LAYOUT1_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH1LAYOUT1_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH1LAYOUT1_GF13_0_GF14_1_MASK)
/* Field DATAN_SIZE */
#define IMX_BCH_FLASH1LAYOUT1_DATAN_SIZE_SHIFT          0
#define IMX_BCH_FLASH1LAYOUT1_DATAN_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH1LAYOUT1_DATAN_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH1LAYOUT1_DATAN_SIZE_MASK)
typedef union {
    uint32_t U;
    struct {
        unsigned DATAN_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECCN:5;
        unsigned PAGE_SIZE:16;
    } B;
} imx_bch_flash1layout1_t;



/* Register FLASH2LAYOUT0 - Hardware BCH ECC Flash 2 Layout 0 Register */
#define IMX_BCH_FLASH2LAYOUT0                           0xC0
/* Field NBLOCKS */
#define IMX_BCH_FLASH2LAYOUT0_NBLOCKS_SHIFT             24
#define IMX_BCH_FLASH2LAYOUT0_NBLOCKS_MASK              0xFF000000
#define IMX_BCH_FLASH2LAYOUT0_NBLOCKS(v)                ((((uint32_t) v) << 24) & IMX_BCH_FLASH2LAYOUT0_NBLOCKS_MASK)
/* Field META_SIZE */
#define IMX_BCH_FLASH2LAYOUT0_META_SIZE_SHIFT           16
#define IMX_BCH_FLASH2LAYOUT0_META_SIZE_MASK            0xFF0000
#define IMX_BCH_FLASH2LAYOUT0_META_SIZE(v)              (((v) << 16) & IMX_BCH_FLASH2LAYOUT0_META_SIZE_MASK)
/* Field ECC0 */
#define IMX_BCH_FLASH2LAYOUT0_ECC0_SHIFT                11
#define IMX_BCH_FLASH2LAYOUT0_ECC0_MASK                 0xF800
#define IMX_BCH_FLASH2LAYOUT0_ECC0(v)                   (((v) << 11) & IMX_BCH_FLASH2LAYOUT0_ECC0_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH2LAYOUT0_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH2LAYOUT0_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH2LAYOUT0_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH2LAYOUT0_GF13_0_GF14_1_MASK)
/* Field DATA0_SIZE */
#define IMX_BCH_FLASH2LAYOUT0_DATA0_SIZE_SHIFT          0
#define IMX_BCH_FLASH2LAYOUT0_DATA0_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH2LAYOUT0_DATA0_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH2LAYOUT0_DATA0_SIZE_MASK)
typedef union {
    uint32_t U;
    struct {
        unsigned DATA0_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECC0:5;
        unsigned META_SIZE:8;
        unsigned NBLOCKS:8;
    } B;
} imx_bch_flash2layout0_t;



/* Register FLASH2LAYOUT1 - Hardware BCH ECC Flash 2 Layout 1 Register */
#define IMX_BCH_FLASH2LAYOUT1                           0xD0
/* Field PAGE_SIZE */
#define IMX_BCH_FLASH2LAYOUT1_PAGE_SIZE_SHIFT           16
#define IMX_BCH_FLASH2LAYOUT1_PAGE_SIZE_MASK            0xFFFF0000
#define IMX_BCH_FLASH2LAYOUT1_PAGE_SIZE(v)              ((((uint32_t) v) << 16) & IMX_BCH_FLASH2LAYOUT1_PAGE_SIZE_MASK)
/* Field ECCN */
#define IMX_BCH_FLASH2LAYOUT1_ECCN_SHIFT                11
#define IMX_BCH_FLASH2LAYOUT1_ECCN_MASK                 0xF800
#define IMX_BCH_FLASH2LAYOUT1_ECCN(v)                   (((v) << 11) & IMX_BCH_FLASH2LAYOUT1_ECCN_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH2LAYOUT1_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH2LAYOUT1_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH2LAYOUT1_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH2LAYOUT1_GF13_0_GF14_1_MASK)
/* Field DATAN_SIZE */
#define IMX_BCH_FLASH2LAYOUT1_DATAN_SIZE_SHIFT          0
#define IMX_BCH_FLASH2LAYOUT1_DATAN_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH2LAYOUT1_DATAN_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH2LAYOUT1_DATAN_SIZE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATAN_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECCN:5;
        unsigned PAGE_SIZE:16;
    } B;
} imx_bch_flash2layout1_t;



/* Register FLASH3LAYOUT0 - Hardware BCH ECC Flash 3 Layout 0 Register */
#define IMX_BCH_FLASH3LAYOUT0                           0xE0
/* Field NBLOCKS */
#define IMX_BCH_FLASH3LAYOUT0_NBLOCKS_SHIFT             24
#define IMX_BCH_FLASH3LAYOUT0_NBLOCKS_MASK              0xFF000000
#define IMX_BCH_FLASH3LAYOUT0_NBLOCKS(v)                ((((uint32_t) v) << 24) & IMX_BCH_FLASH3LAYOUT0_NBLOCKS_MASK)
/* Field META_SIZE */
#define IMX_BCH_FLASH3LAYOUT0_META_SIZE_SHIFT           16
#define IMX_BCH_FLASH3LAYOUT0_META_SIZE_MASK            0xFF0000
#define IMX_BCH_FLASH3LAYOUT0_META_SIZE(v)              (((v) << 16) & IMX_BCH_FLASH3LAYOUT0_META_SIZE_MASK)
/* Field ECC0 */
#define IMX_BCH_FLASH3LAYOUT0_ECC0_SHIFT                11
#define IMX_BCH_FLASH3LAYOUT0_ECC0_MASK                 0xF800
#define IMX_BCH_FLASH3LAYOUT0_ECC0(v)                   (((v) << 11) & IMX_BCH_FLASH3LAYOUT0_ECC0_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH3LAYOUT0_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH3LAYOUT0_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH3LAYOUT0_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH3LAYOUT0_GF13_0_GF14_1_MASK)
/* Field DATA0_SIZE */
#define IMX_BCH_FLASH3LAYOUT0_DATA0_SIZE_SHIFT          0
#define IMX_BCH_FLASH3LAYOUT0_DATA0_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH3LAYOUT0_DATA0_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH3LAYOUT0_DATA0_SIZE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATA0_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECC0:5;
        unsigned META_SIZE:8;
        unsigned NBLOCKS:8;
    } B;
} imx_bch_flash3layout0_t;



/* Register FLASH3LAYOUT1 - Hardware BCH ECC Flash 3 Layout 1 Register */
#define IMX_BCH_FLASH3LAYOUT1                           0xF0
/* Field PAGE_SIZE */
#define IMX_BCH_FLASH3LAYOUT1_PAGE_SIZE_SHIFT           16
#define IMX_BCH_FLASH3LAYOUT1_PAGE_SIZE_MASK            0xFFFF0000
#define IMX_BCH_FLASH3LAYOUT1_PAGE_SIZE(v)              ((((uint32_t) v) << 16) & IMX_BCH_FLASH3LAYOUT1_PAGE_SIZE_MASK)
/* Field ECCN */
#define IMX_BCH_FLASH3LAYOUT1_ECCN_SHIFT                11
#define IMX_BCH_FLASH3LAYOUT1_ECCN_MASK                 0xF800
#define IMX_BCH_FLASH3LAYOUT1_ECCN(v)                   (((v) << 11) & IMX_BCH_FLASH3LAYOUT1_ECCN_MASK)
/* Field GF13_0_GF14_1 */
#define IMX_BCH_FLASH3LAYOUT1_GF13_0_GF14_1_SHIFT       10
#define IMX_BCH_FLASH3LAYOUT1_GF13_0_GF14_1_MASK        0x400
#define IMX_BCH_FLASH3LAYOUT1_GF13_0_GF14_1(v)          (((v) << 10) & IMX_BCH_FLASH3LAYOUT1_GF13_0_GF14_1_MASK)
/* Field DATAN_SIZE */
#define IMX_BCH_FLASH3LAYOUT1_DATAN_SIZE_SHIFT          0
#define IMX_BCH_FLASH3LAYOUT1_DATAN_SIZE_MASK           0x3FF
#define IMX_BCH_FLASH3LAYOUT1_DATAN_SIZE(v)             (((v) << 0) & IMX_BCH_FLASH3LAYOUT1_DATAN_SIZE_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned DATAN_SIZE:10;
        unsigned GF13_0_GF14_1:1;
        unsigned ECCN:5;
        unsigned PAGE_SIZE:16;
    } B;
} imx_bch_flash3layout1_t;



/* Register BLOCKNAME - Block Name Register */
#define IMX_BCH_BLOCKNAME                               0x150
/* Field NAME */
#define IMX_BCH_BLOCKNAME_NAME_SHIFT                    0
#define IMX_BCH_BLOCKNAME_NAME_MASK                     0xFFFFFFFF
#define IMX_BCH_BLOCKNAME_NAME(v)                       ((uint32_t) v)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned NAME:32;
    } B;
} imx_bch_blockname_t;



/* Register VERSION - BCH Version Register */
#define IMX_BCH_VERSION                                 0x160
/* Field MAJOR */
#define IMX_BCH_VERSION_MAJOR_SHIFT                     24
#define IMX_BCH_VERSION_MAJOR_MASK                      0xFF000000
#define IMX_BCH_VERSION_MAJOR(v)                        ((((uint32_t) v) << 24) & IMX_BCH_VERSION_MAJOR_MASK)
/* Field MINOR */
#define IMX_BCH_VERSION_MINOR_SHIFT                     16
#define IMX_BCH_VERSION_MINOR_MASK                      0xFF0000
#define IMX_BCH_VERSION_MINOR(v)                        (((v) << 16) & IMX_BCH_VERSION_MINOR_MASK)
/* Field STEP */
#define IMX_BCH_VERSION_STEP_SHIFT                      0
#define IMX_BCH_VERSION_STEP_MASK                       0xFFFF
#define IMX_BCH_VERSION_STEP(v)                         (((v) << 0) & IMX_BCH_VERSION_STEP_MASK)
/* Register type */
typedef union {
    uint32_t U;
    struct {
        unsigned STEP:16;
        unsigned MINOR:8;
        unsigned MAJOR:8;
    } B;
} imx_bch_version_t;

#endif /* IMX_BCH_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_bch.h $ $Rev: 869196 $")
#endif
