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
#include <hwinfo_private.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_bcm2711.h>
#include <aarch64/bcm2711.h>
#include "mbox.h"

void init_hwinfo()
{
    unsigned hwi_bus_internal = 0;
    /* Add ENET */
    {
        unsigned i, hwi_off;
        uint8_t mac[6];
        hwiattr_enet_t attr = HWIATTR_ENET_T_INITIALIZER;
        hwiattr_common_t common_attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_ENET_SET_NUM_IRQ(&attr, 1);

        uint32_t irqs[] = {
            BCM2711_ENET_IRQA,
            BCM2711_ENET_IRQB
        };

        HWIATTR_SET_NUM_IRQ(&common_attr, NUM_ELTS(irqs));

        /* Create DWC0 */
        HWIATTR_ENET_SET_LOCATION(&attr, BCM2711_ENET_BASE,
                BCM2711_ENET_SIZE, 0, hwi_find_as(BCM2711_ENET_BASE, 1));
        hwi_off = hwidev_add_enet(BCM2711_HWI_GENET, &attr, hwi_bus_internal);
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);

        /* Add IRQ number */
        for(i = 0; i < NUM_ELTS(irqs); i++) {
            hwitag_set_ivec(hwi_off, i, irqs[i]);
        }

        mbox_get_board_mac_address(mac);
        hwitag_add_nicaddr(hwi_off, mac, sizeof(mac));
    }

    /* add the WATCHDOG device */
    {
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        const struct hwi_inputclk clksrc_kick = {.clk = 500, .div = 1};
        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);
        HWIATTR_TIMER_SET_LOCATION(&attr, BCM2711_PM_BASE, BCM2711_PM_SIZE, 0, hwi_find_as(BCM2711_PM_BASE, 1));
        hwi_off = hwidev_add_timer("wdog", &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);

        hwi_off = hwidev_add("wdt,options", 0, HWI_NULL_OFF);
        hwitag_add_regname(hwi_off, "write_width", 32);
        hwitag_add_regname(hwi_off, "enable_width", 32);
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/rpi4/init_hwinfo.c $ $Rev: 919977 $")
#endif
