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

#ifndef IMX_UART_H_
#define IMX_UART_H_

/* UART registers, offset from base address */
#define    IMX_UART_URXD                   0x00    /* Receiver Register */
#define    IMX_UART_UTXD                   0x40    /* Transmitter Register */
#define    IMX_UART_UCR1                   0x80    /* Control Register 1 */
#define    IMX_UART_UCR2                   0x84    /* Control Register 2 */
#define    IMX_UART_UCR3                   0x88    /* Control Register 3 */
#define    IMX_UART_UCR4                   0x8C    /* Control Register 4 */
#define    IMX_UART_UFCR                   0x90    /* FIFO Control Register */
#define    IMX_UART_USR1                   0x94    /* Status Register 1 */
#define    IMX_UART_USR2                   0x98    /* Status Register 2 */
#define    IMX_UART_UESC                   0x9C    /* Escape Character Register */
#define    IMX_UART_UTIM                   0xA0    /* Escape Timer Register */
#define    IMX_UART_UBIR                   0xA4    /* BRM Incremental Register */
#define    IMX_UART_UBMR                   0xA8    /* BRM Modulator Register */
#define    IMX_UART_UBRC                   0xAC    /* Baud Rate Count Register */
#define    IMX_UART_ONEMS                  0xB0    /* contain the value of the UART internal frequency divided by 1000*/
#define    IMX_UART_UTS                    0xB4    /* Test Register */
#define IMX_UART_UMCR                      0xB8    /* Mode Control Register */

/*
 * Receiver Register bits
 */
#define IMX_UART_URXD_CHARRDY          (1<<15)     /* Character Ready */
#define IMX_UART_URXD_ERR              (1<<14)     /* Error Detect */
#define IMX_UART_URXD_OVERRUN          (1<<13)     /* Receiver Overrun */
#define IMX_UART_URXD_FRMERR           (1<<12)     /* Frame Error */
#define IMX_UART_URXD_BRK              (1<<11)     /* BREAK detect */
#define IMX_UART_URXD_PRERR            (1<<10)     /* Parity Error */

/*
 * Control Register 1 bits
 */
#define IMX_UART_UCR1_ADEN             (1<<15)     /* Automatic Baud Rate Detection Interrupt Enable */
#define IMX_UART_UCR1_ADBR             (1<<14)     /* Automatic Detection of Baud Rate */
#define IMX_UART_UCR1_TRDYEN           (1<<13)     /* Transmitter Ready Interrupt Enable */
#define IMX_UART_UCR1_IDEN             (1<<12)     /* Idle Condition Detected Interrupt */
#define IMX_UART_UCR1_ICD_MASK         (3<<10)     /* Idle Condition Detect Mask */
#define IMX_UART_UCR1_RRDYEN           (1<<9)      /* Receiver Ready Interrupt Enable */
#define IMX_UART_UCR1_RXDMAEN          (1<<8)      /* Receive Ready DMA Enable */
#define IMX_UART_UCR1_IREN             (1<<7)      /* Infrared Interface Enable */
#define IMX_UART_UCR1_TXMPTYEN         (1<<6)      /* Transmitter Empty Interrupt Enable */
#define IMX_UART_UCR1_RTSDEN           (1<<5)      /* RTS Delta Interrupt Enable */
#define IMX_UART_UCR1_SNDBRK           (1<<4)      /* Send BREAK */
#define IMX_UART_UCR1_TXDMAEN          (1<<3)      /* Transmitter Ready DMA Enable */
#define IMX_UART_UCR1_ATDMAEN          (1<<2)      /* Ageing DMA timer enable */
#define IMX_UART_UCR1_DOZE             (1<<1)      /* UART DOZE State Control */
#define IMX_UART_UCR1_UARTEN           (1<<0)      /* UART Enable */

/*
 * Control Register 2 bits
 */
#define IMX_UART_UCR2_ESCI             (1<<15)     /* Escape Sequence Interrupt Enable */
#define IMX_UART_UCR2_IRTS             (1<<14)     /* Ignore UART RTS pin */
#define IMX_UART_UCR2_CTSC             (1<<13)     /* UART CTS pin Control */
#define IMX_UART_UCR2_CTS              (1<<12)     /* Clear To Send */
#define IMX_UART_UCR2_ESCEN            (1<<11)     /* Escape Enable */
#define IMX_UART_UCR2_RTEC_MASK        (3<<9)      /* Request to Send Edge Control Mask */
#define IMX_UART_UCR2_PREN             (1<<8)      /* Parity Enable */
#define IMX_UART_UCR2_PROE             (1<<7)      /* Parity Odd/Even */
#define IMX_UART_UCR2_STPB             (1<<6)      /* Stop Bit */
#define IMX_UART_UCR2_WS               (1<<5)      /* Word Size */
#define IMX_UART_UCR2_RTSEN            (1<<4)      /* Request to Send Interrupt Enable */
#define IMX_UART_UCR2_ATEN             (1<<3)      /* Ageing Timer Interrupt Enable */
#define IMX_UART_UCR2_TXEN             (1<<2)      /* Transmitter Enable */
#define IMX_UART_UCR2_RXEN             (1<<1)      /* Receiver Enable */
#define IMX_UART_UCR2_SRST             (1<<0)      /* Software Reset */

/*
 * Control Register 3 bits
 */
#define IMX_UART_UCR3_DPEC_MASK        (3<<14)     /* DTR Interrupt Edge Control */
#define IMX_UART_UCR3_DTREN            (1<<13)     /* Data Terminal Ready Interrupt Enable */
#define IMX_UART_UCR3_PARERREN         (1<<12)     /* Parity Error Interrupt Enable */
#define IMX_UART_UCR3_FRAERREN         (1<<11)     /* Frame Error Interrupt Enable */
#define IMX_UART_UCR3_DSR              (1<<10)     /* Data Set Ready */
#define IMX_UART_UCR3_DCD              (1<<9)      /* Data Carrier Detect */
#define IMX_UART_UCR3_RI               (1<<8)      /* Ring Indicator */
#define IMX_UART_UCR3_RXDSEN           (1<<6)      /* Receive Status Interrupt Enable */
#define IMX_UART_UCR3_AIRINTEN         (1<<5)      /* Asynchronous IR WAKE Interrupt Enable */
#define IMX_UART_UCR3_AWAKEN           (1<<4)      /* Asynchronous WAKE Interrupt Enable */
#define IMX_UART_UCR3_REF25            (1<<3)      /* Reference Frequency 25 MHz */
#define IMX_UART_UCR3_REF20            (1<<2)      /* Reference Frequency 30 MHz */
#define IMX_UART_UCR3_INVT             (1<<1)      /* Inverted Infrared Transmission */
#define IMX_UART_UCR3_BPEN             (1<<0)      /* Preset Registers Enable */

/*
 * Control Register 4 bits
 */
#define IMX_UART_UCR4_CTSTL_MASK       (0x3F<<10)  /* CTS Trigger Level (0-32)*/
#define IMX_UART_UCR4_INVR             (1<<9)      /* Inverted Infrared Reception */
#define IMX_UART_UCR4_ENIRI            (1<<8)      /* Serial Infrared Interrupt Enable */
#define IMX_UART_UCR4_WKEN             (1<<7)      /* WAKE Interrupt Enable */
#define IMX_UART_UCR4_REF16            (1<<6)      /* Reference Frequency 16 MHz */
#define IMX_UART_UCR4_IRSC             (1<<5)      /* IR Special Case */
#define IMX_UART_UCR4_TCEN             (1<<3)      /* Transmit Complete Interrupt Enable */
#define IMX_UART_UCR4_BKEN             (1<<2)      /* BREAK Condition Detected Interrupt Enable */
#define IMX_UART_UCR4_OREN             (1<<1)      /* Receive Overrun Interrupt Enable */
#define IMX_UART_UCR4_DREN             (1<<0)      /* Receive Data Ready Interrupt Enable */

/*
 * FIFO Control Register bits
 */
#define IMX_UART_UFCR_RFDIV_2          (4<<7)      /* Reference freq divider (div 2) */
#define IMX_UART_UFCR_RFDIV_7          (6<<7)      /* Reference freq divider (div 7) */
#define IMX_UART_UFCR_RFDIV_SHIFT      7
#define IMX_UART_UFCR_DCEDTE           (1<<6)
#define IMX_UART_UFCR_RXTL_SHIFT       0
#define IMX_UART_UFCR_TXTL_SHIFT       10

//#define MX1_UFCR_TXTL_MASK           (0x3F<<10)  /* Transmitter Trigger Level Mask*/
//#define MX1_UFCR_TXTL                (1<<10)     /* Transmitter Trigger Level */
//#define MX1_UFCR_RFDIV_MASK          (0x7<<7)    /* Reference Frequency Divider Mask */
//#define MX1_UFCR_RFDIV               (1<<7)      /* Reference Frequency Divider */
//#define MX1_UFCR_DCEDTE              (1<<6)      /* DCE/DTE mode select */
#define IMX_UART_UFCR_RXTL_MASK        (0x3F<<0)   /* Receive Trigger Level Mask */
//#define MX1_UFCR_RXTL                (1<<0)      /* Receive Trigger Level (0-32) */

/*
 * Status Register 1 bits
 */
#define IMX_UART_USR1_PARITYERR        (1<<15)     /* Parity Error Interrupt Flag */
#define IMX_UART_USR1_RTSS             (1<<14)     /* RTS Pin Status */
#define IMX_UART_USR1_TRDY             (1<<13)     /* Transmitter Ready Interrupt/DMA Flag */
#define IMX_UART_USR1_RTSD             (1<<12)     /* RTS Delta */
#define IMX_UART_USR1_ESCF             (1<<11)     /* Escape Sequence Interrupt Flag */
#define IMX_UART_USR1_FRAMERR          (1<<10)     /* Frame Error Interrupt Flag */
#define IMX_UART_USR1_RRDY             (1<<9)      /* Receiver Ready Interrupt/DMA Flag */
#define IMX_UART_USR1_AGTIM            (1<<8)      /* Ageing Timer Interrupt Flag */
#define IMX_UART_USR1_RXDS             (1<<6)      /* Receiver IDLE Interrupt Flag */
#define IMX_UART_USR1_AIRINT           (1<<5)      /* Asynchronous IR WAKE Interrupt Flag */
#define IMX_UART_USR1_AWAKE            (1<<4)      /* Asynchronous WAKE Interrupt Flag */

/*
 * Status Register 2 bits
 */
#define IMX_UART_USR2_ADET             (1<<15)     /* Automatic Baud Rate Detect Complete */
#define IMX_UART_USR2_TXFE             (1<<14)     /* Transmit Buffer FIFO Empty */
#define IMX_UART_USR2_DTRF             (1<<13)     /* DTR Edge Triggered Interrupt Flag */
#define IMX_UART_USR2_IDLE             (1<<12)     /* IDLE Condition */
#define IMX_UART_USR2_IRINT            (1<<8)      /* Serial Infrared Interrupt Flag */
#define IMX_UART_USR2_WAKE             (1<<7)      /* WAKE */
#define IMX_UART_USR2_RTSF             (1<<4)      /* RTS Edge Triggered Interrupt Flag */
#define IMX_UART_USR2_TXDC             (1<<3)      /* Transmitter Complete */
#define IMX_UART_USR2_BRCD             (1<<2)      /* BREAK Condition Detected */
#define IMX_UART_USR2_ORE              (1<<1)      /* Overrun Error */
#define IMX_UART_USR2_RDR              (1<<0)      /* Receive Data Ready */

/*
 * BRM Incremental register bits
 */
#define IMX_UART_UBIR_INC_AUTO         0x000F

/*
 * Test register bits
 */
#define IMX_UART_UTS_FRCPERR           (1<<13)
#define IMX_UART_UTS_LOOP              (1<<12)
#define IMX_UART_UTS_DBGEN             (1<<11)
#define IMX_UART_UTS_LOOPIR            (1<<10)
#define IMX_UART_UTS_RXDBG             (1<<9)
#define IMX_UART_UTS_TXEMPTY           (1<<6)
#define IMX_UART_UTS_RXEMPTY           (1<<5)
#define IMX_UART_UTS_TXFULL            (1<<4)
#define IMX_UART_UTS_RXFULL            (1<<3)
#define IMX_UART_UTS_SOFTRST           (1<<0)


#endif /* __ARM_IMX_UART_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_uart.h $ $Rev: 869196 $")
#endif
