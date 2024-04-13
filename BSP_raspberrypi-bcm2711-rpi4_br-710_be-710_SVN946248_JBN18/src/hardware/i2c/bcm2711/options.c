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

#include "proto.h"

int bcm2711_options(bcm2711_dev_t *dev, int argc, char *argv[])
{
    int	c;
    int	prev_optind;
    int	done = 0;

    /* Initialization */
    dev->intr = BCM2711_I2C0_IRQ;
    dev->iid = -1;
    dev->physbase = 0;
    dev->regbase = (uintptr_t)MAP_FAILED;
    dev->reglen = BCM2711_I2C_REGLEN;
    dev->restart = 0;
    dev->slave_addr = 0;
    dev->clk = -1;
    dev->verbose = 0;
#ifdef GPIO_CONFIG
    dev->gpio = 255;
#endif /* GPIO_CONFIG */
    while (!done) {
        prev_optind = optind;
        c = getopt(argc, argv, "i:p:s:c:g:v");
        switch (c) {
            case 'p':
                dev->physbase = strtoul(optarg, &optarg, 0);
                break;
            case 's':
                dev->slave_addr = strtoul(optarg, &optarg, 0);
                break;
            case 'c':
                dev->clk = strtoul(optarg, &optarg, 0);
                break;
            case 'i':
                dev->intr = (int)strtoul(optarg, &optarg, 0);
                break;
#ifdef GPIO_CONFIG
            case 'g':
                dev->gpio = (uint8_t)strtoul(optarg, &optarg, 0);
                break;
#endif /* GPIO_CONFIG */
            case 'v':
                dev->verbose++;
                break;
            case '?':
                if (optopt == '-') {
                    ++optind;
                    break;
                }
                return -1;

            case -1:
                if (prev_optind < optind) /* -- */
                    return -1;

                if (argv[optind] == NULL) {
                    done = 1;
                    break;
                }
                if (*argv[optind] != '-') {
                    ++optind;
                    break;
                }
                return -1;

            case ':':
            default:
                return -1;
        }
    }

    if (dev->clk == 0)
    {
        dev->clk = 5000; /*default to 100KHz*/
    }

    if ((dev->physbase != BCM2711_I2C0_BASE)  ||
        (dev->physbase != BCM2711_I2C1_BASE) ||
        (dev->physbase != BCM2711_I2C3_BASE) ||
        (dev->physbase != BCM2711_I2C4_BASE) ||
        (dev->physbase != BCM2711_I2C5_BASE) ||
        (dev->physbase != BCM2711_I2C6_BASE)) {
        bcm2711_i2c_slogf(dev, _SLOG_ERROR,"%s: I2C phy address: 0x%x",
                                                        __FUNCTION__, dev->physbase);
    }
    else if (dev->physbase == BCM2711_I2C2_BASE) {
        bcm2711_i2c_slogf(dev, _SLOG_ERROR,"%s: Port2 is not supported as Master port", __FUNCTION__);
        return -1;
    }
    else {
        bcm2711_i2c_slogf(dev, _SLOG_ERROR,"%s: Invalid I2C master base address", __FUNCTION__);
        return -1;
    }

    return 0;;
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/i2c/bcm2711/options.c $ $Rev: 919272 $")
#endif
