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

#include "startup.h"
#include <aarch64/bcm2711.h>

extern struct callout_rtn interrupt_id_bcm2711_msi;
extern struct callout_rtn interrupt_eoi_bcm2711_msi;
extern struct callout_rtn interrupt_mask_bcm2711_msi;
extern struct callout_rtn interrupt_unmask_bcm2711_msi;

static paddr_t bcm2711_msi_base = BCM2711_PCIE_BASE + 0x4000;

static const struct startup_intrinfo intrs[] =
{
    /* MSI interrupts (32) starting at 0x200 */
    {	.vector_base = 0x200,           // vector base (MSI 0x200 - 0x21F)
        .num_vectors = 32,              // 32 MSIs
        .cascade_vector = BCM2711_PCIE_MSI,
        .cpu_intr_base = 0,             // CPU vector base (MSI from 0 - 31)
        .cpu_intr_stride = 0,
        .flags = INTR_FLAG_MSI,
        .id = { INTR_GENFLAG_NOGLITCH, 0, &interrupt_id_bcm2711_msi },
        .eoi = { INTR_GENFLAG_LOAD_INTRMASK, 0, &interrupt_eoi_bcm2711_msi },
        .mask = &interrupt_mask_bcm2711_msi,
        .unmask = &interrupt_unmask_bcm2711_msi,
        .config = 0,
        .patch_data = &bcm2711_msi_base,
    },
};

void init_intrinfo()
{
    /*
     * Initialise GIC
     */
#ifndef	GICv3
#define	GICD_PADDR		0xff841000
#define	GICC_PADDR		0xff842000
    gic_v2_init(GICD_PADDR, GICC_PADDR);
#else
#define GIC_GICD_BASE	0x4c0040000     // CPU distributor BASE address
#define GIC_GICR_BASE	0x4c0041000     // CPU redistributor BASE address
#define GIC_GICC_BASE	0x4c0042000     // CPU interface, CBAR register value
    gic_v3_init(GIC_GICD_BASE, GIC_GICR_BASE, GIC_GICC_BASE, 0, 1);
#endif

    /*
     * Add interrupt callouts
     */
    add_interrupt_array(intrs, sizeof intrs);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/init_intrinfo.c $ $Rev: 919977 $")
#endif
