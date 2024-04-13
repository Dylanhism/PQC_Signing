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

#include "externs.h"

int tto(TTYDEV *ttydev, int action, int arg1) {
    TTYBUF *bup = &ttydev->obuf;
    DEV_MINIUART *dev = (DEV_MINIUART *) ttydev;
    uintptr_t base = dev->base;
    unsigned reg = 0;
    unsigned char c;

    switch (action) {
        case TTO_STTY:
            ser_stty(dev);
            return 0;

        case TTO_CTRL:
            if (arg1 & _SERCTL_BRK_CHG) {
                reg = in32(base + AUX_MU_LCR_REG);
                if (arg1 &_SERCTL_BRK)
                    reg |= LCR_BRK;
                else
                    reg &= ~LCR_BRK;
                out32(base + AUX_MU_LCR_REG, reg);
            }
            return 0;

        case TTO_LINESTATUS:
            /* miniuart controller doesn't support DTR, DSR, DCD signal */
            return in32(base + AUX_MU_STAT_REG);

        case TTO_DATA:
            break;

        default:
            return 0;
    }

    /*
     * If the OSW_PAGED_OVERRIDE flag is set then allow
     * transmit of character even if output is suspended via
     * the OSW_PAGED flag. This flag implies that the next
     * character in the obuf is a software flow control
     * charater (STOP/START).
     * Note: tx_inject sets it up so that the contol
     *       character is at the start (tail) of the buffer.
     *
     */
    if ((dev->tty.flags & (OHW_PAGED | OSW_PAGED))
            && !(dev->tty.xflags & OSW_PAGED_OVERRIDE)) {
        return 0;
    }

    while (bup->cnt > 0) {
        /*
        * Get the next character to print from the output buffer
        */
        dev_lock(&dev->tty);
        c = tto_getchar(&dev->tty);
        dev_unlock(&dev->tty);

        dev->tty.un.s.tx_tmr = 3;   /* Timeout 3 */

        /* Wait until Tx fifo has empyt slot */
        while ((in32(base + AUX_MU_STAT_REG) & STAT_TXFULL)) {
            ;
        }
        out32(base + AUX_MU_IO_REG, c);
        /* Clear the OSW_PAGED_OVERRIDE flag as we only want
         * one character to be transmitted in this case.
         */
        if (dev->tty.xflags & OSW_PAGED_OVERRIDE) {
            atomic_clr(&dev->tty.xflags, OSW_PAGED_OVERRIDE);
            break;
        }
    }
    if (!(dev->tty.flags & (OHW_PAGED|OSW_PAGED)) && bup->cnt) {
        reg = in32(base + AUX_MU_IER_REG);
        out32(base + AUX_MU_IER_REG, reg | IER_TXEN);
    }

    /* Check client lists for notify conditions */
    return (tto_checkclients(&dev->tty));
}

void ser_stty(DEV_MINIUART *dev) {
    uintptr_t base = dev->base;
    unsigned cntl, div, lcr, reg;
    uint32_t timeout;

    dev->cntl = cntl = in32(base + AUX_MU_CNTL_REG);
    dev->div = div = in32(base + AUX_MU_BAUD_REG);
    dev->lcr = lcr = in32(base + AUX_MU_LCR_REG);

    if (dev->tty.c_cflag & OHFLOW) {
        cntl &= ~CNTL_CTSEN;
        cntl |= CNTL_CTSEN;
    }

    if (dev->tty.c_cflag & IHFLOW) {
        cntl &= ~CNTL_RTSEN;
        cntl |= CNTL_RTSEN;
    }

    div &= ~0xFFFF;
    div |= (((dev->clk / (dev->tty.baud * 8)) - 1) & 0xFFFF);

    switch (dev->tty.c_cflag & CSIZE) {
        case CS8:
            lcr &= ~0x1;
            lcr |= LCR_DSIZE8;
            break;
        case CS7:
            lcr &= ~0x1;
            lcr |= LCR_DSIZE7;
            break;
    }

    cntl |= (CNTL_TXEN | CNTL_RXEN);

    if (dev->init_done && (dev->cntl == cntl) && (dev->lcr == lcr) && (dev->div == div)) {
       return;
    }

    dev->div = div;
    dev->cntl = cntl;
    dev->lcr = lcr;

    /*
     * Wait for Tx FIFO and shift register empty if the UART is enabled
     */
    timeout = 100000;
    if ((in32(base + AUX_MU_CNTL_REG) & CNTL_TXEN)) {
        while (!(in32(base + AUX_MU_STAT_REG) & STAT_TXDONE) && timeout--);
    }

    if (!timeout) {
        MINIUART_SLOG_WARNING("Timeout waiting for Tx done");
    }

    /* Disable Receiver and Transmitter */
    reg = in32(base + AUX_MU_CNTL_REG);
    reg &= ~(CNTL_TXEN | CNTL_RXEN);
    out32(base + AUX_MU_CNTL_REG, reg);

    /* Disable the interrupt */
    reg = in32(base + AUX_MU_IER_REG);
    out32(base + AUX_MU_IER_REG, reg & ~(IER_TXEN | IER_RXEN));

    /* Set bauderate */
    out32(base + AUX_MU_BAUD_REG, dev->div);

    /* Set line control */
    out32(base + AUX_MU_LCR_REG, dev->lcr);

    /* Set auto CTS/RTS and activate Tx and Rx */
    out32(base + AUX_MU_CNTL_REG, dev->cntl);

    /* Clear FIFOs */
    out32(base + AUX_MU_IIR_REG, IIR_CLRFIFO);

    /* Enable Rx interrupt */
    reg = in32(base + AUX_MU_IER_REG);
    out32(base + AUX_MU_IER_REG, reg | IER_RXEN);

    MINIUART_SLOG_DEBUG("cntl=0x%08x, baud=0x%08x, lcr=0x%08x, iir=0x%08x",
                in32(base + AUX_MU_CNTL_REG),
                in32(base + AUX_MU_BAUD_REG),
                in32(base + AUX_MU_LCR_REG),
                in32(base + AUX_MU_IIR_REG));
}

int drain_check(TTYDEV *ttydev, uintptr_t *count) {
    TTYBUF *bup = &ttydev->obuf;
    DEV_MINIUART *dev = (DEV_MINIUART *) ttydev;

    // if the device has DRAINED, return 1
    if ((bup->cnt == 0) && (in32(dev->base + AUX_MU_LSR_REG) & LSR_TXIDLE)) {
        return 1;
    }

    // if the device has not DRAINED, set a timer based on 50ms counts
    // wait for the time it takes for one character to be transmitted
    // out the shift register.  We do this dynamically since the
    // baud rate can change.
    if (count != NULL) {
        *count = (ttydev->baud == 0) ?
                    0 : ((IO_CHAR_DEFAULT_BITSIZE * MINIUART_FIFO_SIZE) / ttydev->baud) + 1;
    }
    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serminiuart/tto.c $ $Rev: 919648 $")
#endif
