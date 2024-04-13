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

#include <startup.h>
#include "bcm2711_startup.h"

#define AUX_MU_IO_REG   0x40    // Mini Uart I/O Data
#define AUX_MU_IER_REG  0x44    // Mini Uart Interrupt Enable
#define AUX_MU_IIR_REG  0x48    // Mini Uart Interrupt Identify
#define AUX_MU_LCR_REG  0x4c    // Mini Uart Line Control
#define AUX_MU_MCR_REG  0x50    // Mini Uart Modem Control
#define AUX_MU_LSR_REG  0x54    // Mini Uart Line Status
#define AUX_MU_MSR_REG  0x58    // Mini Uart Modem Status
#define AUX_MU_SCRATCH  0x5c    // Mini Uart Scratch
#define AUX_MU_CNTL_REG 0x60    // Mini Uart Extra Control
#define AUX_MU_STAT_REG 0x64    // Mini Uart Extra Status
#define AUX_MU_BAUD_REG 0x68    // Mini Uart Baudrate

/*
 * Syntax: base^shift.baud
 * Where:
 *
 *  base  = physical address (0xfe215000)
 *  shift = not used by driver code (typically set to 0)
 *  baud  = baud rate
 *  clock = clock frequency (typically 48000000, but may be different)
 */
static void
parse_line(unsigned channel, const char *line, unsigned *baud)
{
    /*
     * Get device base address and register stride
     */
    if (*line != '.' && *line != '\0') {
        dbg_device[channel].base = strtopaddr(line, (char **) &line, 16);
        if (*line == '^')
            dbg_device[channel].shift = strtoul(line + 1, (char **) &line, 0);
    }

    /*
     * Get baud rate
     */
    if (*line == '.')
        ++line;
    if (*line != '.' && *line != '\0')
        *baud = strtoul(line, (char **) &line, 0);
}

void
init_miniuart(unsigned channel, const char *init, const char *defaults)
{
    unsigned baud;
    parse_line(channel, defaults, &baud);
    parse_line(channel, init, &baud);
}

void
put_miniuart(int c)
{
    paddr_t base = dbg_device[0].base;

    while (!(in32(base + AUX_MU_LSR_REG) & 0x20));
    out32(base + AUX_MU_IO_REG, c);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/hw_ser_miniuart.c $ $Rev: 928235 $")
#endif
