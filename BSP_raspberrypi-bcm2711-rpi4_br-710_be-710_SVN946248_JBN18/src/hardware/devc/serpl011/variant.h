/*
 * $QNXLicenseC:
 * Copyright 2008-2019, QNX Software Systems.
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
#ifndef FIFOSIZE
#define FIFOSIZE	32
#endif

#ifndef write_pl011
#define	write_pl011(__dev,__reg,__val)	write_port(__dev,__reg,__val)
#endif

#ifndef read_pl011
#define	read_pl011(__dev,__reg)	read_port(__dev,__reg)
#endif



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serpl011/variant.h $ $Rev: 932262 $")
#endif
