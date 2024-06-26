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

#ifdef DEFN
    #define EXT
    #define INIT1(a)    = { a }
#else
    #define EXT extern
    #define INIT1(a)
#endif

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <termios.h>
#include <devctl.h>
#include <sys/dcmd_chr.h>
#include <sys/iomsg.h>
#include <atomic.h>
#include <hw/inout.h>
#include <sys/io-char.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/rsrcdbmgr.h>
#include <sys/dispatch.h>

#include "mini_uart.h"

#define DEFAULT_PRIORITY	24

typedef struct dev_miniuart {
    TTYDEV              tty;
    struct dev_miniuart *next;
    uintptr_t           base;
    unsigned            intr;
    int                 iid;

    unsigned            clk;
    unsigned            div;
    unsigned            baud;
    unsigned            cntl;
    unsigned            lcr;
    unsigned            isr;
    volatile unsigned   init_done;

#if 0
    unsigned            lcr_h;
    unsigned            fifo;
    int                 fifosize;
    unsigned            ibrd;
    unsigned            fbrd;
    unsigned            cr;
    unsigned            imr;
    unsigned            loopback;
    unsigned            drt;
    unsigned            prio;
    unsigned            fifo_reg;
    unsigned            dma_enable;
    unsigned            dma_state;
    unsigned            dma_request_rx;
    unsigned            dma_request_tx;
    unsigned            dma_xfer_size;
    unsigned            chan_rx;
    unsigned            chan_tx;
#endif
} DEV_MINIUART;

typedef struct ttyinit_miniuart {
    TTYINIT             tty;
    unsigned            isr;
#if 0
    unsigned            drt;
    unsigned            prio;
    unsigned            loopback;
    unsigned            fifo_reg;
    unsigned            dma_enable;
    unsigned            dma_request_rx;
    unsigned            dma_request_tx;
    unsigned            dma_xfer_size;
    unsigned            chan_rx;
    unsigned            chan_tx;
#endif
} TTYINIT_MINIUART;

EXT TTYCTRL     ttyctrl;

#include "proto.h"

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serminiuart/externs.h $ $Rev: 919648 $")
#endif
