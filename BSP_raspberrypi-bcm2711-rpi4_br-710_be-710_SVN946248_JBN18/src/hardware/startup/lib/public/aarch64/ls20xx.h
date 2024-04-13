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
 * NXP LS20XX SOCs:
 */

#ifndef __AARCH64_LS20XX_H_INCLUDED
#define __AARCH64_LS20XX_H_INCLUDED

/*
 * System Memory Map
 */

#define LS20XX_DRAM_BASE        0x80000000

/*
 * Device configuration and pin control (DCFG)
 */
#define LS20XX_DCFG_BASE        0x01E00000
#define LS20XX_DCFG_SIZE        0x1000
    #define LS20XX_DCFG_PORSR1         0x00
    #define LS20XX_DCFG_SVR            0xA4
        #define DCFG_SVR_REV_MASK           0xFF
        #define DCFG_SVR_MAJOR_REV_MASK     0xF0
        #define DCFG_SVR_MAJOR_REV_1        0x10
        #define DCFG_SVR_MAJOR_REV(x)       ((x & DCFG_SVR_MAJOR_REV_MASK) >> 4)
        #define DCFG_SVR_MINOR_REV_MASK     0x0F
        #define DCFG_SVR_MINOR_REV_0        0x00
        #define DCFG_SVR_MINOR_REV_1        0x01
        #define DCFG_SVR_MINOR_REV(x)       (x & DCFG_SVR_MINOR_REV_MASK)
    #define LS20XX_DCFG_RCW            0x100

/*
 * CCSR Reset address map
 */
#define LS20XX_RST_BASE         0x01E60000
#define LS20XX_RST_SIZE         0x100
    #define LS20XX_RST_RSTCR       0x00
        #define RESET_REQ           (1 << 1)

/*
 * DUART1
 */
#define LS20XX_DUART1_BASE      0x021C0000
#define LS20XX_DUART_SIZE       0x100
#define LS20XX_UART1_BASE       0x021C0500
#define LS20XX_UART2_BASE       0x021C0600

/*
 * Enhanced secured digital host controller (eSHDC)
 */
#define LS20XX_eSDHC_BASE       0x02140000
#define LS20XX_eSDHC_SIZE       0x10000

/*
 * Serial peripheral interface (SPI)
 */
#define LS20XX_SPI_BASE         0x02100000
#define LS20XX_SPI_SIZE         0x10000

/*
 * I2C controllers
 */
#define	LS20XX_I2C1_BASE        0x02000000
#define	LS20XX_I2C2_BASE        0x02010000
#define	LS20XX_I2C3_BASE        0x02020000
#define	LS20XX_I2C4_BASE        0x02030000
#define LS20XX_I2C_SIZE         0x10000

/*
 * IRQ assignments
 */
#define LS20XX_IRQ_TMU_ALARM           55  /* Thermal monitor unit alarm */
#define LS20XX_IRQ_TMU_CALARM          56  /* Thermal monitor unit critical alarm */
#define LS20XX_IRQ_QSPI                57
#define LS20XX_IRQ_DSPI                58
#define LS20XX_IRQ_EPUPM               59  /* Performance monitor */
#define LS20XX_IRQ_eSDHC1              60
#define LS20XX_IRQ_SEC_MON_SECURITY    61
#define LS20XX_IRQ_SEC_MON_GENERAL     62
#define LS20XX_IRQ_MBEE                63
#define LS20XX_IRQ_DUART1              64
#define LS20XX_IRQ_DUART2              65
#define LS20XX_IRQ_I2C1_2              66
#define LS20XX_IRQ_I2C3_4              67
#define LS20XX_IRQ_GPIO1_2             68
#define LS20XX_IRQ_GPIO3_4             69
#define LS20XX_IRQ_EPU1                70
#define LS20XX_IRQ_EPU2                71
#define LS20XX_IRQ_EPU3                72
#define LS20XX_IRQ_EPU4                73
#define LS20XX_IRQ_QDMA                74
#define LS20XX_IRQ_WDOG1               91
#define LS20XX_IRQ_WDOG2               92
#define LS20XX_IRQ_eSDHC2              95
#define LS20XX_IRQ_USB3_1              112
#define LS20XX_IRQ_USB3_2              113

#endif /* __AARCH64_LS20XX_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/ls20xx.h $ $Rev: 877503 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/ls20xx.h $ $Rev: 877503 $" )
#endif
#endif
