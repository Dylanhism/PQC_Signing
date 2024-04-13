/*
 * $QNXLicenseC:
 * Copyright 2007-2019, QNX Software Systems.
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
%C - Serial driver for PL011 UARTs

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
 -t number    Set receive FIFO trigger level ( 0 - 3; default 2 (1 / 2 full))
 -T number    Set number of characters to send to transmit FIFO
                                             ( 0 - 3; default 2 (1 / 2 full))
 -u unit      Set serial unit number (default 1)

#endif
*/
#include "externs.h"

unsigned
options(int argc, char *argv[])
{
	int				opt;
	int				numports = 0;
	void			*link;
	unsigned		unit;
	unsigned		rx_fifo = 2;	// 1 / 2 full, default
	unsigned		tx_fifo = 2;	// 1 / 2 full, default
	TTYINIT_PL011	devinit = {
		{	0,			// port
			1,			// port_shift
			0,			// intr
			38400,		// baud
			2048,		// isize
			2048,		// osize
			256,		// csize
			0,			// c_cflag
			0,			// c_iflag
			0,			// c_lflag
			0,			// c_oflag
			0,			// fifo
			24000000,	// clk
			16,			// div
			"/dev/ser"	// name
		},
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
		while ((opt = getopt(argc, argv, IO_CHAR_SERIAL_OPTIONS "c:t:T:u:")) != -1) {	
			switch (ttc(TTC_SET_OPTION, &devinit, opt)) {
				case 'c':
					devinit.tty.clk = strtoul(optarg, &optarg, 0);
					break;

				case 't':
					rx_fifo = strtoul(optarg, NULL, 0);
					if (rx_fifo > 4)
						rx_fifo = 2;
					break;

				case 'T':
					tx_fifo = strtoul(optarg, NULL, 0);
					if (tx_fifo > 4)
						tx_fifo = 2;

					break;

				case 'u':
					unit = strtoul(optarg, NULL, 0);
					break;
			}
		}

		devinit.tty.fifo = (rx_fifo << 3) | tx_fifo;

		/*
		 * Process ports and interrupts.
		 */
		while (optind < argc  &&  *(optarg = argv[optind]) != '-') {
			devinit.tty.port = strtoul(optarg, &optarg, 16);
			if (*optarg == '^') {
				devinit.tty.port_shift = strtoul(optarg + 1, &optarg, 0);
			}
			if (*optarg == ',')  {
				devinit.tty.intr = strtoul(optarg + 1, NULL, 0);
			}
			create_device(&devinit, unit++);
			++numports;
			++optind;
		}
	}

	if (numports == 0) {
		link = NULL;
		devinit.tty.fifo = (rx_fifo << 3) | tx_fifo;
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
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serpl011/options.c $ $Rev: 932262 $")
#endif
