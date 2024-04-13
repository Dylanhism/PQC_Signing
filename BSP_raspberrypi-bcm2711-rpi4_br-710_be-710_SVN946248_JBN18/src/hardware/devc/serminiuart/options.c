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



/*
#ifdef __USAGE
%C - Serial driver for Mini UART

%C [options] [port[,irq]] &
Options:
 -b number    Define initial baud rate (default 38400)
 -c clk       Set the input clock rate (default 24000000)
 -C number    Size of canonical input buffer (default 256)
 -e           Set options to "edit" mode
 -E           Set options to "raw" mode (default)
 -I number    Size of raw input buffer (default 2048)
 -f           Enable hardware flow control (default)
 -F           Disable hardware flow control
 -O number    Size of output buffer (default 2048)
 -s           Enable software flow control
 -S           Disable software flow control (default)
 -u unit      Set serial unit number (default 1)
 -i [0|1]     Set interrupt mode (0=event, 1=isr, default is isr mode)

#endif
*/
#include "externs.h"

unsigned
options(int argc, char *argv[])
{
    int             opt;
    int             numports = 0;
    void            *link;
    unsigned        unit;
    TTYINIT_MINIUART    devinit = {
        {	0,          // port
            0,          // port_shift
            0,          // intr
            115200,      // baud
            2048,       // isize
            2048,       // osize
            256,        // csize
            0,          // c_cflag
            0,          // c_iflag
            0,          // c_lflag
            0,          // c_oflag
            0,          //
            500000000,  // clk
            541,        // div
            "/dev/ser"  // name
        },
        1,              //isr
    };

    /*
     * Initialize the devinit to raw mode
     */
    ttc(TTC_INIT_RAW, &devinit, 0);

    unit = 1;

    while (optind < argc) {
        /*
         * Process dash options.
         */
        while ((opt = getopt(argc, argv, IO_CHAR_SERIAL_OPTIONS "c:i:u:")) != -1) {
            switch (ttc(TTC_SET_OPTION, &devinit, opt)) {
                case 'c':
                    devinit.tty.clk = strtoul(optarg, &optarg, 0);
                    break;
                case 'i':
                    devinit.isr = strtoul(optarg, NULL, 0);
                    break;
                case 'u':
                    unit = strtoul(optarg, NULL, 0);
                    break;
            }
        }

        /*
         * Process ports and interrupts.
         */
        while (optind < argc  &&  *(optarg = argv[optind]) != '-') {
            devinit.tty.port = strtoul(optarg, &optarg, 16);
            if (*optarg == ',')
                devinit.tty.intr = strtoul(optarg + 1, NULL, 0);
            create_device(&devinit, unit++);
            ++numports;
            ++optind;
        }
    }

    if (numports == 0) {
        link = NULL;
        while (1) {
            link = query_default_device(&devinit, link);
            if (link == NULL)
                break;
            create_device(&devinit, unit++);
            ++numports;
        }
    }

    return numports;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serminiuart/options.c $ $Rev: 925363 $")
#endif
