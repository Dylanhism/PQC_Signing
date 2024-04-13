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

unsigned
board_smp_num_cpu() {
    return 4;
}

void
board_smp_init(struct smp_entry *smp, unsigned num_cpus)
{
#ifndef	GICv3
    smp->send_ipi = (void *)&sendipi_gic_v2;
#else
    smp->send_ipi = (void *)&sendipi_gic_v3_sr;
#endif

    /* Need to turn on the IPI interrupt for mailbox 0 on all cores */
    uint32_t cpu;
    for (cpu = 0; cpu < num_cpus; ++cpu) {
        uint32_t mbic_addr = 0x40000050 + 0x4 * cpu;
        uint32_t mbic = in32(mbic_addr);
        mbic |= 0x1;
        out32(mbic_addr, mbic);
    }
}

int
board_smp_start(unsigned cpu, void (*start)(void))
{
    typedef void (*start_t)(void);
    volatile start_t *cpu_start = (start_t *)(0xd8L + cpu * 8);
    *cpu_start = start;

    // same assenbly works in 32 and 64 bit
    __asm__ __volatile__(
        "dsb sy\n"
        "sev\n"
        : : : "memory"
    );

    return 1;
}

unsigned
board_smp_adjust_num(unsigned cpu)
{
    return cpu;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/boards/bcm2711/board_smp.c $ $Rev: 919977 $")
#endif
