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

static const struct callout_slot	timer_callouts[] = {
	{ CALLOUT_SLOT( timer_load, _armv8) },
	{ CALLOUT_SLOT( timer_value, _armv8) },
	{ CALLOUT_SLOT( timer_reload, _armv8) },
};

static unsigned
timer_start_v8gt(void) {
	return (unsigned)aa64_sr_rd64(cntpct_el0);
}

static unsigned
timer_diff_v8gt(unsigned const start) {
	return (unsigned)(aa64_sr_rd64(cntpct_el0) - (unsigned long)start);
}

void
init_qtime_v8gt(unsigned const vcnt_intr, unsigned const hvcnt_intr) {
	struct qtime_entry	*qtime;

	aarch64_hwi_add_ppi("vcnt", vcnt_intr);
	aarch64_hwi_add_ppi("hvcnt", hvcnt_intr);

	qtime = alloc_qtime();

	timer_start = timer_start_v8gt;
	timer_diff  = timer_diff_v8gt;

	/*
	 * Set up qtime timer rate/scale
	 */
	if (timer_freq == 0) {
		timer_freq = aa64_sr_rd32(cntfrq_el0);
	}
	invert_timer_freq(qtime, timer_freq);

	/*
	 * Each core has its own generic timer registers, so we force the
	 * system timer to cpu0's timer.
	 */
	qtime->flags |= QTIME_FLAG_TIMER_ON_CPU0;
	qtime->cycles_per_sec = timer_freq;
	qtime->intr = vcnt_intr;
	qtime->timer_load_max = 0x7fffffffffffffffull;

	/*
	 * Disable CNTV interrupt
	 */
	aa64_sr_wr32(cntv_ctl_el0, 0);

	add_callout_array(timer_callouts, sizeof(timer_callouts));
}



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/aarch64/init_qtime_v8gt.c $ $Rev: 912447 $")
#endif
