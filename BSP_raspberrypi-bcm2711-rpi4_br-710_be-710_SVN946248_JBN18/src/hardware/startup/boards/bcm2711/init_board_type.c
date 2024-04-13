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
#include "mbox.h"
#include "bcm2711_startup.h"

/*
 *  Please refer to https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
 *  New-style revision codes:
 *    NOQuuuWuFMMMCCCCPPPPTTTTTTTTRRRR in which:
 *      N         Overvoltage
 *      O         OTP Program
 *      Q         OTP Read
 *      uuu       Unused
 *      W         Warranty bit
 *      u         Unused
 *      F         New flag
 *      MMM       Memory size
 *      CCCC      Manufacturer
 *      PPPP      Processor
 *      TTTTTTTT  Type
 *      RRRR      Revision
 */
#define UNKNOWN_SOC     4
#define UNKNOWN_SIZE    5
#define	UNKNOWN_MFR     6
#define UNKNOWN_TYPE    21

const char *socs[] = {
    [0] = "BCM2835",
    [1] = "BCM2836",
    [2] = "BCM2837",
    [3] = "BCM2711",
    [4] = "Unknown",
};

static const unsigned memory[] = {
    [0] = 256,
    [1] = 512,
    [2] = 1024,
    [3] = 2048,
    [4] = 4096,
    [5] = 8192,
};

static const char *mfrs[] = {
    [0] = "Sony UK",
    [1] = "Egoman",
    [2] = "Embest",
    [3] = "Sony Japan",
    [4] = "Embest",
    [5] = "Stadium",
    [6] = "Unknown",
};

static const char *types[] = {
    [6]  = "RaspberryPiCM1",
    [8]  = "RaspberryPi3B",
    [9]  = "RaspberryPi0",
    [10] = "RaspberryPiCM3",
    [12] = "RaspberryPi0W",
    [13] = "RaspberryPi3B+",
    [14] = "RaspberryPi3A+",
    [16] = "RaspberryPiCM3+",
    [17] = "RaspberryPi4B",
    [18] = "Unknown",
    [19] = "RaspberryPi400",
    [20] = "RaspberryPiCM3+",
    [21] = "Unknown",
};

static unsigned mfr = UNKNOWN_MFR;
static unsigned soc = UNKNOWN_SOC;
static unsigned type = UNKNOWN_TYPE;
static unsigned memsize = UNKNOWN_SIZE;
static char serial[20];

void init_board_type(void)
{
    unsigned val;
    uint32_t revision = mbox_get_board_revision();

    val = (revision >> 20) & 7;
    memsize = (val > UNKNOWN_SIZE) ? UNKNOWN_SIZE : val;

    val = (revision >> 16) & 15;
    mfr = (val > UNKNOWN_MFR) ? UNKNOWN_MFR : val;

    val = (revision >> 12) & 15;
    soc = (val > UNKNOWN_SOC) ? UNKNOWN_SOC : val;

    val = (revision >> 4) & 255;
    type = (val > UNKNOWN_TYPE) ? UNKNOWN_TYPE : val;

    ksprintf(serial, "%L", mbox_get_board_serial());
}

const char *get_soc_name(void)
{
    return socs[soc];
}

const unsigned get_memory_size(void)
{
    return memory[memsize];
}

const char *get_mfr_name(void)
{
    return mfrs[mfr];
}

const char * get_board_name(void)
{
    return types[type];
}

const char *get_board_serial(void)
{
    return serial;
}
#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/init_board_type.c $ $Rev: 930301 $")
#endif
