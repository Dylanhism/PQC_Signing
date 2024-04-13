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


#define MINIUART_SLOG(severity, fmt, ...)    slogf(_SLOGC_CHAR_SER, severity, "(miniuart %s:%d) " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define MINIUART_SLOG_ERROR(fmt, ...)        MINIUART_SLOG(_SLOG_ERROR, fmt, ##__VA_ARGS__)
#define MINIUART_SLOG_WARNING(fmt, ...)      MINIUART_SLOG(_SLOG_WARNING, fmt, ##__VA_ARGS__)
#define MINIUART_SLOG_INFO(fmt, ...)         MINIUART_SLOG(_SLOG_INFO, fmt, ##__VA_ARGS__)
#define MINIUART_SLOG_DEBUG(fmt, ...)        MINIUART_SLOG(_SLOG_DEBUG1, fmt, ##__VA_ARGS__)



void create_device(TTYINIT_MINIUART *dip, unsigned unit);
void ser_stty(DEV_MINIUART *dev);
void ser_attach_intr(DEV_MINIUART *dev);
void ser_detach_intr(DEV_MINIUART *dev);
int rx_interrupt(DEV_MINIUART *dev);
int tx_interrupt(DEV_MINIUART *dev);
void *timer_thread (void *data);
void *tx_dma_thread (void *data);
void *query_default_device(TTYINIT_MINIUART *dip, void *link);
unsigned options(int argc, char *argv[]);

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devc/serminiuart/proto.h $ $Rev: 919648 $")
#endif
