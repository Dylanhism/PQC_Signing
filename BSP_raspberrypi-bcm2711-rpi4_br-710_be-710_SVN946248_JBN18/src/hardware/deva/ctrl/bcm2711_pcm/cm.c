/*
 * $QNXLicenseC:
 * Copyright 2020 QNX Software Systems.
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

#include "cm.h"
#include <aarch64/bcm2711.h>
#include <audio_driver.h>

/**- Clock Manager -**/

void cm_config(unsigned unit, unsigned freq, unsigned clk, unsigned src, unsigned mash) {
    // fixed-point scaled clock divisor with 12 bit (bit width of DIVF bit field) fractional part
    const uint32_t scl = ((uint64_t) clk << 12) / freq;
    // key bit field of CM clock CTL register
    const uint32_t cm_key = CM_CTL_PASSWD(0x5a);

    uint32_t ctl;
    uint32_t div;
    volatile cm_clk_reg_t *cm;

    ado_debug(DB_LVL_DRIVER, "clk %u freq %u scl %u", clk, freq, scl);

    cm = ado_device_mmap(NULL, BCM2711_CM_BASE, BCM2711_CM_SIZE);

    ctl = cm[unit].ctl;
    div = cm[unit].div;

    ado_debug(DB_LVL_DRIVER, "mash %u busy %u ena %u src %u, div int %u frac %u",
        (ctl & CM_CTL_MASH_MSK) >> 9, (ctl & CM_CTL_BUSY) >> 7, (ctl & CM_CTL_ENAB) >> 4,
        (ctl & CM_CTL_SRC_MSK) >> 0, (div & CM_DIV_INT_MSK) >> 12, (div & CM_DIV_FRAC_MSK) >> 0);

    if (div != scl || (ctl & CM_CTL_MASH_MSK) != CM_CTL_MASH(mash)) {
        ctl &= ~CM_CTL_ENAB;
        cm[unit].ctl = cm_key | ctl;
        while (cm[unit].ctl & CM_CTL_BUSY);
        cm[unit].div = cm_key | scl;
        ctl = CM_CTL_ENAB | (ctl & ~(CM_CTL_MASH_MSK | CM_CTL_SRC_MSK)) | CM_CTL_MASH(mash) | CM_CTL_SRC(src);
        cm[unit].ctl = cm_key | ctl;
        ctl = cm[unit].ctl;
        div = cm[unit].div;
        ado_debug(DB_LVL_DRIVER, "mash %u busy %u ena %u src %u, div int %u frac %u",
            (ctl & CM_CTL_MASH_MSK) >> 9, (ctl & CM_CTL_BUSY) >> 7, (ctl & CM_CTL_ENAB) >> 4,
            (ctl & CM_CTL_SRC_MSK) >> 0, (div & CM_DIV_INT_MSK) >> 12, (div & CM_DIV_FRAC_MSK) >> 0);
    }

    ado_device_munmap(NULL, (void*) cm, BCM2711_CM_SIZE);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/cm.c $ $Rev: 927538 $")
#endif
