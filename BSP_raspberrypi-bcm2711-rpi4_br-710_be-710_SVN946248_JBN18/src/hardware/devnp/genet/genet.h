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

#ifndef GENET_H_
#define GENET_H_
#include <sys/io-pkt.h>
#include <net/if_ether.h>
/*MDI related header files*/
#include <netdrvr/mdi.h>
#include <netdrvr/nicsupport.h>
#include <hw/nicinfo.h>
#include <sys/callout.h>
/*cache header files*/
#include <sys/cache.h>
#include <hw/inout.h>

/*bsd media related header files*/
#include <sys/device.h>
#include <net/if_media.h>
#include <dev/mii/miivar.h>
#include <sys/syslog.h>
#include <sys/slogcodes.h>

#include "genet_reg.h"

#define GENET_ERROR(fmt, ...)   \
    do { slogf(_SLOGC_NETWORK, _SLOG_ERROR,"genet:%s():%d: "fmt, \
            __func__, __LINE__, ##__VA_ARGS__); } while(0)
#define GENET_WARNING(fmt, ...) \
    do { slogf(_SLOGC_NETWORK, _SLOG_WARNING,"genet:%s():%d: "fmt, \
            __func__, __LINE__, ##__VA_ARGS__); } while(0)

#define GENET_INFO(fmt, ...)    \
    do { slogf(_SLOGC_NETWORK, _SLOG_INFO,"genet:%s():%d: "fmt, \
            __func__, __LINE__, ##__VA_ARGS__); } while(0)

#define GENET_DEBUG(fmt, ...)   \
    do { slogf(_SLOGC_NETWORK, _SLOG_DEBUG1,"genet:%s():%d: "fmt, \
            __func__, __LINE__, ##__VA_ARGS__); } while(0)

#define IS_BROADCAST(dptr) \
        ((dptr)[0] == 0xff && (dptr)[1] == 0xff && \
        (dptr)[2] == 0xff && (dptr)[3] == 0xff && \
        (dptr)[4] == 0xff && (dptr)[5] == 0xff)

#define GENET_DEFAULT_PRIO      21

typedef struct Desc_t
{
    uintptr_t desc;       // 0:CONTROL/STATUS, 1: PHYSICAL_ADDRESS_LO, 2: PHYSICAL_ADDRESS_HI
    struct mbuf *mb;
}Desc;

typedef struct Genet_t
{
    /* Do not change the order of first three element */
    struct device       sc_dev; /* common device */
    struct ethercom     sc_ec;  /* common ethernet */
    nic_config_t        cfg;    /* nic information */

    /* whatever else you need follows */
    nic_stats_t         stats;
    struct cache_ctrl   cachectl;
    struct callout      mii_callout;
    struct mii_data     bsd_mii;

    struct _iopkt_self  *iopkt;
    struct _iopkt_inter intr_rx;
    struct _iopkt_inter intr_tx;
    struct _iopkt_inter intr_link;
    mdi_t               *mdi;

    /* RX, TX descriptors (256)*/
    Desc                tx_d[GENET_RING_SIZE_MAX];
    Desc                rx_d[GENET_RING_SIZE_MAX];

    /* Register base address */
    uintptr_t           genet_base;
    uintptr_t           mdio_base;

    volatile uint16_t   free_idx; // last freed tx slot
    unsigned            drops;
    uint32_t            irq0_status;
    uint32_t            link_status;
    int                 iid_isr0;
    int                 iid_isr1;
    int                 probe_phy;
    int                 force_link;
    void                *sc_sdhook;
}Genet;

/* Function proto types */
int genet_attach(struct device *, struct device *, void *);
int genet_detach(struct device *, int);
int genet_entry(void *dll_hdl, struct _iopkt_self *iopkt, char *options);
int genet_init(struct ifnet *);
void genet_stop(struct ifnet *, int);
void genet_start(struct ifnet *);
void genet_shutdown(void *);
int genet_ioctl(struct ifnet *, unsigned long, caddr_t);
void genet_add_pkt_to_rx_desc(Genet *genet, struct mbuf *m, Desc *const rxdesc);
uint32_t genet_reg_read(Genet *, uint32_t );
void genet_reg_write(Genet *, uint32_t , uint32_t );

const struct sigevent * genet_isr0(void *, int);
#ifdef GENET_ISR_1
const struct sigevent * genet_isr1(void *, int);
#endif
int rx_process_interrupt(void *, struct nw_work_thread *);
int tx_process_interrupt(void *, struct nw_work_thread *);
int link_process_interrupt(void *, struct nw_work_thread *);

int genet_ext_phy_init(struct ifnet *, Genet *);
int bsd_mii_mediachange(struct ifnet *);
void bsd_mii_initmedia(Genet *);
void bsd_mii_finimedia(Genet *);
void genet_mdi_finiphy(Genet *);

#endif /* GENET_H_ */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
