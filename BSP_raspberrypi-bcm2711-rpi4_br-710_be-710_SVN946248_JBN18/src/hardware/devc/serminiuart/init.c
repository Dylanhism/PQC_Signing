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
#include <sys/mman.h>
#include <sys/trace.h>

/*
 * Specify parameters for default devices.
 */
void *
query_default_device(TTYINIT_MINIUART *dip, void *link)
{
    return NULL;
}

void
create_device(TTYINIT_MINIUART *dip, unsigned unit)
{
    DEV_MINIUART	*dev;

    /*
     * Get a device entry and the input/output buffers for it.
     */
    dev = calloc(1, sizeof(*dev));

    if (dev == NULL) {
        MINIUART_SLOG_ERROR("Unable to allocate device entry: %s", strerror(errno));
        goto fail;
    }

    /*
     * Get buffers.
     */
    dev->tty.ibuf.head = dev->tty.ibuf.tail = dev->tty.ibuf.buff = (unsigned char *) malloc(dev->tty.ibuf.size = dip->tty.isize);
    if (dev->tty.ibuf.buff == NULL) {
        MINIUART_SLOG_ERROR("Failed to allocate input buffer");
        goto fail1;
    }

    dev->tty.obuf.head = dev->tty.obuf.tail = dev->tty.obuf.buff = (unsigned char *) malloc(dev->tty.obuf.size = dip->tty.osize);
    if (dev->tty.obuf.buff == NULL) {
        MINIUART_SLOG_ERROR("Failed to allocate output buffer");
        goto fail2;
    }

    dev->tty.cbuf.head = dev->tty.cbuf.tail = dev->tty.cbuf.buff = (unsigned char *) malloc(dev->tty.cbuf.size = dip->tty.csize);
    if (dev->tty.cbuf.buff == NULL) {
        MINIUART_SLOG_ERROR("Failed to allocate canonical buffer");
        goto fail3;
    }

    dev->tty.highwater = dev->tty.ibuf.size - (dev->tty.ibuf.size < 128 ? dev->tty.ibuf.size/4 : 100);

    strcpy(dev->tty.name, dip->tty.name);

    dev->tty.baud = dip->tty.baud;
    dev->intr = dip->tty.intr;
    dev->clk  = dip->tty.clk;
    dev->div  = dip->tty.div;

    dev->tty.flags = EDIT_INSERT | LOSES_TX_INTR;
    dev->tty.c_cflag = dip->tty.c_cflag;
    dev->tty.c_iflag = dip->tty.c_iflag;
    dev->tty.c_lflag = dip->tty.c_lflag;
    dev->tty.c_oflag = dip->tty.c_oflag;
    dev->tty.verbose = dip->tty.verbose;
    dev->isr         = dip->isr;
    /*
     * Map device registers
     */
    dev->base = mmap_device_io (MINIUART_SIZE, dip->tty.port);
    if (dev->base == (uintptr_t)MAP_FAILED) {
        MINIUART_SLOG_ERROR("mmap_device_io failed");
        goto fail4;
    }

    /*
     * Initialize termios cc codes to an ANSI terminal.
     */
    ttc(TTC_INIT_CC, &dev->tty, 0);

    /*
     * Initialize the device's name.
     * Assume that the basename is set in device name.  This will attach
     * to the path assigned by the unit number/minor number combination
     */
    unit = SET_NAME_NUMBER(unit) | NUMBER_DEV_FROM_USER;
    ttc(TTC_INIT_TTYNAME, &dev->tty, unit);

    /*
     * Only setup IRQ handler for non-pcmcia devices.
     * Pcmcia devices will have this done later when card is inserted.
     */
    if (dip->tty.port != 0 && dip->tty.intr != _NTO_INTR_SPARE) {
        ser_attach_intr(dev);
        ser_stty(dev);
    }

    /*
     * Attach the resource manager
     */
    ttc(TTC_INIT_ATTACH, &dev->tty, 0);
    atomic_set(&dev->init_done, 0x1);
    return ;

fail4:
    free(dev->tty.cbuf.buff);
fail3:
    free(dev->tty.obuf.buff);
fail2:
    free(dev->tty.ibuf.buff);
fail1:
    free (dev);
fail:
    exit(1);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serminiuart/init.c $ $Rev: 919648 $")
#endif
