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


#include <io-pkt/iopkt_driver.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <netinet/ip.h>


#include <sys/io-pkt.h>
#include <sys/syspage.h>
#include <device_qnx.h>

#include <net/if_ether.h>

#include <net/if_media.h>
#include <net/netbyte.h>

#include <drvr/hwinfo.h>

#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/trace.h>

#include <net/ifdrvcom.h>
#include <sys/sockio.h>


#include "genet.h"

static void genet_cleanup(Genet *, int);
static int enable_interrupt(void *);

uint32_t genet_reg_read(Genet *genet, uint32_t reg)
{
    return in32(genet->genet_base + reg);
}

void genet_reg_write(Genet *genet, uint32_t reg, uint32_t val)
{
    out32(genet->genet_base + reg, val);
}

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(genet) = IOPKT_DRVR_ENTRY_SYM_INIT(genet_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms genet_syms[] = {
        {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(genet)},
        {NULL, NULL}
};
#endif

CFATTACH_DECL(genet,
    sizeof(Genet),
    NULL,
    genet_attach,
    genet_detach,
    NULL);

struct attach_args {
    char *options;
    struct _iopkt_self *iopkt;
};

static int genet_detect(void)
{
    int                   ret;
    struct ifnet          *ifp;
    struct drvcom_config  *dcon;

    /* Check if driver is already mounted by doing a "nicinfo" on each interface */
    dcon = (malloc)(sizeof(*dcon));
    if (dcon == NULL) {
        GENET_ERROR("malloc failed.\n");
        return ENOMEM;
    }

    IFNET_FOREACH(ifp) {
        dcon->dcom_cmd.ifdc_cmd = DRVCOM_CONFIG;
        dcon->dcom_cmd.ifdc_len = sizeof(dcon->dcom_config);
        ret = ifp->if_ioctl(ifp, SIOCGDRVCOM, (caddr_t)dcon);
        if ((ret == EOK) && !strcmp(ifp->if_xname,"genet0")) {
            GENET_ERROR( "Driver already loaded for %s", ifp->if_xname);
            (free)(dcon);
            return EBUSY;
        }
    }
    (free)(dcon);

    return EOK;
}

int genet_entry(void *dll_hdl,  struct _iopkt_self *iopkt, char *options)
{
    struct device      *dev;
    struct attach_args attach_args;
    int32_t            single = 1;

    /* Check if another driver already exists
     * - If 2nd time start the driver using io-pkt-v6-hc -d genet
     *   not detecting the already running genet0 interface
     * - If 2nd time start the driver using mount -Tio-pkt <path-to-so>
     *   it able to detect the running genet0 and exiting successfully
     */
    if(genet_detect() != EOK) {
        GENET_ERROR("Driver load failed.");
        return EBUSY;
    }

    /* Initialize arguments/parameters for genet_attach() */
    memset(&attach_args, 0x00, sizeof(attach_args));
    attach_args.iopkt = iopkt;

    /* currently not parsing anything from cmd line
     * place holder for next release
     */
    attach_args.options = options;

    dev = NULL; /* No Parent */
    if (dev_attach("genet", options, &genet_ca, &attach_args,
                &single, &dev, NULL) != EOK) {
        return ENODEV;
    }

    dev->dv_dll_hdl = dll_hdl;

    return EOK;
}

static int enable_interrupt(void *arg)
{
        return 1;
}

static int genet_get_board_mac_addr(uchar_t *mac)
{
    unsigned hwi_off = hwi_find_device("genet", 0);

    if (hwi_off != HWI_NULL_OFF) {

        /* Find MAC address */
        if (hwitag_find_nicaddr(hwi_off, NULL, mac) == -1) {
            GENET_WARNING("%s: Failed to find mac from hwi", __func__);
        }

    } else {
        /* No device in syspage */
        return ENODEV;
    }

    return EOK;
}

static void init_tx_ring(Genet *genet)
{
    genet_reg_write(genet, GENET_TDMA_START_ADDRESS, 0);
    genet_reg_write(genet, GENET_TDMA_START_ADDRESSH, 0);
    genet_reg_write(genet, GENET_TDMA_FLOW_PERIOD, 0);

    genet_reg_write(genet, GENET_TDMA_END_ADDRESS,
            ((GENET_RING_SIZE_MAX * GENET_DMA_DESC_WORDS) - 1) );
    genet_reg_write(genet, GENET_TDMA_END_ADDRESSH, 0);

    genet_reg_write(genet, GENET_TDMA_RING_BUF_SIZE,
            (GENET_RING_SIZE_MAX << GENET_TDMA_RING_BUF_SIZE_DESC_SHIFT) | GENET_BUFFER_SIZE);

    genet_reg_write(genet, GENET_TDMA_MBUF_DONE_INTR_THRESH, 2); // Interrupt for number of buffer??
    genet_reg_write(genet, GENET_TDMA_WRITE_POINTER, 0);
    genet_reg_write(genet, GENET_TDMA_READ_POINTER, 0);
    genet_reg_write(genet, GENET_TDMA_CONSUMER_INDEX, 0);
    genet_reg_write(genet, GENET_TDMA_PRODUCER_INDEX, 0);

    /* enable the current ring */
    genet_reg_write(genet, GENET_TDMA_RING_CFG, 1 << GENET_DEFAULT_RING );
    /* enable the ring for the DMA
       17b : 16th ring
    */
    genet_reg_write(genet, GENET_TDMA_CONTROL, 2 << GENET_DEFAULT_RING);
}

static void init_rx_ring(Genet *genet)
{

    genet_reg_write(genet, GENET_RDMA_START_ADDRESS, 0);
    genet_reg_write(genet, GENET_RDMA_START_ADDRESSH, 0);

    genet_reg_write(genet, GENET_RDMA_END_ADDRESS,
            (GENET_RING_SIZE_MAX * GENET_DMA_DESC_WORDS)-1);
    genet_reg_write(genet, GENET_RDMA_END_ADDRESSH, 0);

    genet_reg_write(genet, GENET_RDMA_RING_BUF_SIZE,
            (GENET_RING_SIZE_MAX << GENET_TDMA_RING_BUF_SIZE_DESC_SHIFT) | GENET_BUFFER_SIZE);

    genet_reg_write(genet, GENET_RDMA_MBUF_DONE_INTR_THRESH, 1); // Interrupt for number of buffer??
    //genet_reg_write(genet, GENET_RDMA_XON_XOFF_THRESH, (5 << GENET_RDMA_XON_XOFF_THR_SHIFT) | 10 << ?);
    genet_reg_write(genet, GENET_RDMA_WRITE_POINTER, 0);
    genet_reg_write(genet, GENET_RDMA_READ_POINTER, 0);
    genet_reg_write(genet, GENET_RDMA_PRODUCER_INDEX, 0);
    genet_reg_write(genet, GENET_RDMA_CONSUMER_INDEX, 0);

    /* enable the current ring */
    genet_reg_write(genet, GENET_RDMA_RING_CFG, 1 << GENET_DEFAULT_RING );
    /* enable the ring for the DMA
       17b : 16th ring
    */
    genet_reg_write(genet, GENET_RDMA_CONTROL, 2 << GENET_DEFAULT_RING);
}

static void genet_dma_init(Genet *genet)
{
    int i;

    for(i=0; i < GENET_RING_SIZE_MAX; i++)
    {
        genet->tx_d[i].desc = genet->genet_base + GENET_TDMA_REG_OFF + i * GENET_DMA_DESC_SIZE;
        genet->rx_d[i].desc = genet->genet_base + GENET_RDMA_REG_OFF + i * GENET_DMA_DESC_SIZE;
    }

    init_tx_ring(genet);

    init_rx_ring(genet);

    /* Enable the TX and RX DMA for already configured rings
     * no packets will be sent or received until the UMAC is started.
     */
    genet_reg_write(genet, GENET_TDMA_CONTROL,
            genet_reg_read(genet, GENET_TDMA_CONTROL) | GENET_TDMA_CONTROL_DMA_ENABLE);
    genet_reg_write(genet, GENET_RDMA_CONTROL,
            genet_reg_read(genet, GENET_RDMA_CONTROL) | GENET_RDMA_CONTROL_DMA_ENABLE);
}

static void genet_umac_reset(Genet *genet)
{
    int ret;

    /* reset the UMAC before configuring it */
    genet_reg_write(genet, GENET_SYS_RBUF_FLUSH_CNTRL, 1);
    genet_reg_write(genet, GENET_SYS_TBUF_FLUSH_CNTRL, 1);

    nanospin_ns(10 * 1000);

    genet_reg_write(genet, GENET_SYS_RBUF_FLUSH_CNTRL, 0);
    genet_reg_write(genet, GENET_SYS_TBUF_FLUSH_CNTRL, 0);

    /* Configure the PORT to as external GPHY connected*/
    genet_reg_write(genet, GENET_SYS_PORT_CNTRL, GENET_SYS_PORT_CNTRL_EXT_GPHY);

    /* Enable the RGMII mode */
    ret = genet_reg_read(genet, GENET_EXT_RGMII_OOB_CNTRL);
    genet_reg_write(genet, GENET_EXT_RGMII_OOB_CNTRL, ret
                            | GENET_EXT_RGMII_OOB_CNTRL_ID_MODE_DIS
                            | GENET_EXT_RGMII_OOB_CNTRL_RGMII_MODE_EN
                            | GENET_EXT_RGMII_OOB_CNTRL_LINK);
    genet_reg_write(genet, GENET_EXT_GPHY_CNTRL, 0);

    /* SW reset and loopback*/
    genet_reg_write(genet, GENET_UMAC_CMD, GENET_UMAC_CMD_SW_RESET
                            | GENET_UMAC_CMD_LCL_LOOP_EN);

    nanospin_ns(20 * 1000);
}

static void genet_hw_stop(Genet *genet)
{
    /* clear the DMA control for both RX and Tx */
    genet_reg_write(genet, GENET_RDMA_CONTROL, 0);
    genet_reg_write(genet, GENET_TDMA_CONTROL, 0);

    /* clear rx ring */
    genet_reg_write(genet, GENET_RDMA_START_ADDRESS, 0);
    genet_reg_write(genet, GENET_RDMA_START_ADDRESSH, 0);
    genet_reg_write(genet, GENET_RDMA_END_ADDRESS,0);
    genet_reg_write(genet, GENET_RDMA_END_ADDRESSH,0);
    genet_reg_write(genet, GENET_RDMA_RING_BUF_SIZE,0);
    genet_reg_write(genet, GENET_RDMA_MBUF_DONE_INTR_THRESH, 0);
    genet_reg_write(genet, GENET_RDMA_WRITE_POINTER, 0);
    genet_reg_write(genet, GENET_RDMA_READ_POINTER, 0);
    genet_reg_write(genet, GENET_RDMA_PRODUCER_INDEX, 0);
    genet_reg_write(genet, GENET_RDMA_CONSUMER_INDEX, 0);

    /* clear tx ring */
    genet_reg_write(genet, GENET_TDMA_START_ADDRESS, 0);
    genet_reg_write(genet, GENET_TDMA_START_ADDRESSH, 0);
    genet_reg_write(genet, GENET_TDMA_FLOW_PERIOD, 0);
    genet_reg_write(genet, GENET_TDMA_END_ADDRESS,0);
    genet_reg_write(genet, GENET_TDMA_END_ADDRESSH,0);
    genet_reg_write(genet, GENET_TDMA_RING_BUF_SIZE,0);
    genet_reg_write(genet, GENET_TDMA_MBUF_DONE_INTR_THRESH, 0);
    genet_reg_write(genet, GENET_TDMA_WRITE_POINTER, 0);
    genet_reg_write(genet, GENET_TDMA_READ_POINTER, 0);
    genet_reg_write(genet, GENET_TDMA_CONSUMER_INDEX, 0);
    genet_reg_write(genet, GENET_TDMA_PRODUCER_INDEX, 0);
}

static int genet_umac_init(Genet *genet)
{
    int ret;
    uint32_t int0_enable;

    /* in case u-boot uses ethernet with DMA */
    genet_hw_stop(genet);

    genet_umac_reset(genet);

    // Enable DMA
    /* Important :: RX :: SCB_BURST_SIZE */
    genet_reg_write(genet, GENET_RDMA_SCB_BURST_SIZE, 8);

    /* Important :: TX :: SCB_BURST_SIZE */
    genet_reg_write(genet, GENET_TDMA_SCB_BURST_SIZE, 8);

    /* Enable the interrupts */
    int0_enable = GENET_INTRL2_RX_DONE
              | GENET_INTRL2_TX_DONE
              | GENET_INTRL2_0_LINK_UP
              | GENET_INTRL2_0_LINK_DOWN;

    genet_reg_write(genet, GENET_INTRL2_0_CPU_CLEAR, ~0);
    genet_reg_write(genet, GENET_INTRL2_0_CPU_MASK_CLEAR, int0_enable);
    genet_reg_write(genet, GENET_INTRL2_1_CPU_CLEAR, ~0);
    genet_reg_write(genet, GENET_INTRL2_1_CPU_MASK_CLEAR, 0);

    /* Clear checksum offload and alignment */
    ret = genet_reg_read(genet, GENET_RBUF_CNTRL);
    genet_reg_write(genet, GENET_RBUF_CNTRL, ret & ~(GENET_RBUF_CNTRL_STATUS_64B_EN | GENET_RBUF_CNTRL_ALIGN_2B) );

    /* Enable RX checksum */
    ret = genet_reg_read(genet, GENET_RBUF_CHK_CNTRL);
    genet_reg_write(genet, GENET_RBUF_CHK_CNTRL, ret & (GENET_RBUF_CHK_CNTRL_RXCHK_EN) );

    genet_reg_write(genet, GENET_UMAC_FRM_LEN, 0x600);

    /* Reset counters */
    genet_reg_write(genet, GENET_UMAC_MIB_CNTRL, GENET_UMAC_MIB_CNTRL_RX_CNT_RST
                            | GENET_UMAC_MIB_CNTRL_RUNT_CNT_RST
                            | GENET_UMAC_MIB_CNTRL_TX_CNT_RST );

    /* Re enable the counters */
    genet_reg_write(genet, GENET_UMAC_MIB_CNTRL, 0);

    return 0;
}

static void genet_set_macaddr(Genet *genet)
{
    genet_reg_write(genet, GENET_UMAC_MAC0,
            (genet->cfg.current_address[0] << 24) | (genet->cfg.current_address[1] << 16) | (genet->cfg.current_address[2] << 8) | genet->cfg.current_address[3]);

    genet_reg_write(genet, GENET_UMAC_MAC1,
            (genet->cfg.current_address[4] << 8) | genet->cfg.current_address[5]);
}

static void genet_rx_reap(Genet *genet, int count)
{
    int i;
    for(i = 0; i < count; i++)
    {
        if( genet->rx_d[i].mb != NULL ){
            m_freem(genet->rx_d[i].mb);
        }
        /* Clear the DMA control, DMA physical MSB and LSB address*/
        out32(genet->rx_d[i].desc + GENET_DMA_DESC_CNTRL, 0);
        out32(genet->rx_d[i].desc + GENET_DMA_DESC_ADDR_LSB, 0);
        out32(genet->rx_d[i].desc + GENET_DMA_DESC_ADDR_MSB, 0);
    }
}

static int genet_setup_rx_descriptors(Genet *genet)
{
    int i;
    struct mbuf *m;

    for(i = 0; i < GENET_RING_SIZE_MAX; i++){
        m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m == NULL) {
            /*
             * No memory in system for mbuf
             * exiting the driver because Rx won't work
             * without mbuf allocated.
             */
            GENET_ERROR("No room for new mbuf..");
            if(i) {
                genet_rx_reap(genet, i-1);
            }
                    return -1;
                }
        genet_add_pkt_to_rx_desc(genet, m, &genet->rx_d[i]);
    }
    return 0;
}

int genet_attach(struct device *parent, struct device *self, void *aux)
{
    Genet *genet;
    struct ifnet *ifp;

    struct attach_args *attach_args;
    struct _iopkt_self *iopkt;
    unsigned char board_mac[6];
    uint32_t err;

    attach_args = aux;
    iopkt = attach_args->iopkt;
    genet = (Genet *)self;
    genet->iopkt = iopkt;
    ifp = &genet->sc_ec.ec_if;
    ifp->if_softc = genet;


    genet->iid_isr0 = -1;
    genet->iid_isr1 = -1;

    /* will be disabled when MDIO interrupt fixed */
    genet->probe_phy = 1;

    /* Seting interface name */
    strcpy(ifp->if_xname, genet->sc_dev.dv_xname);
    strcpy((char *) genet->cfg.uptype, "en");
    strcpy((char *) genet->cfg.device_description, "genet");

    /* Rx done interrupt */
    genet->intr_rx.func   = rx_process_interrupt;
    genet->intr_rx.enable = enable_interrupt;
    genet->intr_rx.arg    = genet;
    if ((err = interrupt_entry_init(&genet->intr_rx, 0, NULL,
                    IRUPT_PRIO_DEFAULT)) != EOK) {
        return err;
    }
    /* Tx done interrupt */
    genet->intr_tx.func = tx_process_interrupt;
    genet->intr_tx.enable = enable_interrupt;
    genet->intr_tx.arg = genet;
    if ((err = interrupt_entry_init (&genet->intr_tx, 0, NULL,
                    IRUPT_PRIO_DEFAULT)) != EOK) {
        return err;
    }
    /* MDIO interrupt */
    genet->intr_link.func   = link_process_interrupt;
    genet->intr_link.enable = enable_interrupt;
    genet->intr_link.arg    = genet;
    if ((err = interrupt_entry_init (&genet->intr_link, 0, NULL,
                    IRUPT_PRIO_DEFAULT)) != EOK) {
        return err;
    }

    /*HW support checksum and enabled using GENET_RBUF_CHK_CNTRL_RXCHK_EN*/
    ifp->if_capabilities_rx = IFCAP_CSUM_IPv4 | IFCAP_CSUM_TCPv4 | IFCAP_CSUM_UDPv4;
    ifp->if_capabilities_rx |= IFCAP_CSUM_TCPv6 | IFCAP_CSUM_UDPv6;

    /* as of now no details to this from Pi4 and BCM */
    // ifp->if_capabilities_tx = IFCAP_CSUM_IPv4 | IFCAP_CSUM_TCPv4 | IFCAP_CSUM_UDPv4;
    // genet->sc_ec.ec_capabilities |= ETHERCAP_JUMBO_MTU;

    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST;

    /* Set callouts */
    ifp->if_ioctl = genet_ioctl;
    ifp->if_start = genet_start;
    ifp->if_init = genet_init;
    ifp->if_stop = genet_stop;
    IFQ_SET_READY(&ifp->if_snd);

    /*GENET_CALL*/
    genet->free_idx = 0;
    genet->cachectl.fd = NOFD;

    if (cache_init(0, &genet->cachectl, NULL) == -1) {
        GENET_ERROR("cache_init failed..");
        return EINVAL;
        }

    if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
        GENET_ERROR("ThreadCtl failed..");
        return EINVAL;
        }

    genet->genet_base = (uintptr_t)mmap_device_memory(0,
            GENET_REG_SIZE,PROT_READ | PROT_WRITE | PROT_NOCACHE, 0, GENET_BASE_ADDR);
    if( genet->genet_base== (uintptr_t)MAP_FAILED){
        GENET_ERROR("mmap_device_io failed..");
        genet_cleanup(genet, 1);
        return EIO;
    }

    if((genet->sc_sdhook = shutdownhook_establish(genet_shutdown, genet)) == NULL ){
        GENET_ERROR("shutdownhook_establish failed.");
        genet_cleanup(genet, 3);
        return ENOMEM;
    }

    /* Ethernet stats we are interested in */
    genet->stats.un.estats.valid_stats = NIC_ETHER_STAT_INTERNAL_TX_ERRORS
            | NIC_ETHER_STAT_INTERNAL_RX_ERRORS
            | NIC_ETHER_STAT_NO_CARRIER
            | NIC_ETHER_STAT_XCOLL_ABORTED
            | NIC_ETHER_STAT_SINGLE_COLLISIONS
            | NIC_ETHER_STAT_MULTI_COLLISIONS
            | NIC_ETHER_STAT_LATE_COLLISIONS
            | NIC_ETHER_STAT_TX_DEFERRED
            | NIC_ETHER_STAT_ALIGN_ERRORS
            | NIC_ETHER_STAT_FCS_ERRORS
            | NIC_ETHER_STAT_JABBER_DETECTED
            | NIC_ETHER_STAT_OVERSIZED_PACKETS
            | NIC_ETHER_STAT_SHORT_PACKETS;

    /* Generic networking stats we are interested in */
    genet->stats.valid_stats = NIC_STAT_TX_FAILED_ALLOCS
            | NIC_STAT_RX_FAILED_ALLOCS | NIC_STAT_RXED_MULTICAST
            | NIC_STAT_RXED_BROADCAST | NIC_STAT_TXED_BROADCAST
            | NIC_STAT_TXED_MULTICAST;

    genet->cfg.priority = GENET_DEFAULT_PRIO; /* default priority */
    genet->cfg.lan = genet->sc_dev.dv_unit;
    genet->cfg.media_rate = -1;
    genet->cfg.duplex = -1;
    genet->force_link = -1;

    genet->cfg.flags |= NIC_FLAG_LINK_DOWN;

    genet->cfg.num_mem_windows = 1;
    genet->cfg.mem_window_base[0] = genet->genet_base;
    genet->cfg.mem_window_size[0] = GENET_REG_SIZE;

    /* Number of tx descriptors needs to be split up between interfaces (2)*/
    genet->cfg.mtu = (ETHER_MAX_LEN - ETHER_CRC_LEN);
    genet->cfg.mru = (ETHER_MAX_LEN - ETHER_CRC_LEN);
    genet->cfg.flags |= NIC_FLAG_MULTICAST;
    genet->cfg.mac_length = ETHER_ADDR_LEN;

    /* update the virtual address for mdio access register */
    genet->mdio_base = genet->genet_base + GENET_MDIO_OFFSET;

    /* Configure the UMAC */
    genet_umac_init(genet);

    /* Initialize the tx, rx rings and DMA */
    genet_dma_init(genet);

    /* Initialize the rx descriptors with mbuf */
    if (genet_setup_rx_descriptors(genet) < 0 ){
        genet_cleanup(genet, 1);
        return ENOMEM;
    }

    /* Fetch the board MAC address from syspage */
    err = genet_get_board_mac_addr(board_mac);
    if (err != EOK) {
        GENET_ERROR("%s: Failed to parse device details from syspage.", __func__);
        return err;
    }

    if (memcmp (genet->cfg.current_address, "\0\0\0\0\0\0", 6) == 0)  {
        memcpy(genet->cfg.current_address, board_mac, ETHER_ADDR_LEN);
    }

    /* PHY initialization */
    err = genet_ext_phy_init(ifp, genet);
    if(err != EOK){
        GENET_ERROR("Failed to initialize the phy..");
        genet_cleanup(genet, 2);
        return EINVAL;
    }

    genet->cfg.lan = genet->sc_dev.dv_unit;
        genet->cfg.media_rate = -1;
        genet->cfg.duplex = -1;

    bsd_mii_initmedia(genet);

    genet->cfg.flags |= NIC_FLAG_LINK_DOWN;

    strcpy(ifp->if_xname, genet->sc_dev.dv_xname);

    if_attach(ifp);

    /* Normal ethernet */
    ether_ifattach(ifp, genet->cfg.current_address);

    return EOK;
}

static void genet_set_multicast(Genet *genet)
{
    struct ethercom         *ec = &genet->sc_ec;
    struct ifnet            *ifp = &ec->ec_if;
    struct ether_multi      *enm;
    struct ether_multistep  step;

    ifp->if_flags &= ~IFF_ALLMULTI;

    ETHER_FIRST_MULTI(step, ec, enm);
    while (enm != NULL) {
                if (memcmp(enm->enm_addrlo, enm->enm_addrhi, ETHER_ADDR_LEN)) {
                        /*
                         * We must listen to a range of multicast addresses.
                         * For now, just accept all multicasts, rather than
                         * trying to filter out the range.
                         * At this time, the only use of address ranges is
                         * for IP multicast routing.
                         */
                        ifp->if_flags |= IFF_ALLMULTI;
            break;
                }

        ETHER_NEXT_MULTI(step, enm);
    }

    if ((ifp->if_flags & IFF_ALLMULTI) != 0) {
        if(genet->cfg.verbose)
            GENET_INFO("Enable multicast promiscuous.");
    } else {
        if(genet->cfg.verbose)
            GENET_INFO("Disable multicast promiscuous.");
    }
}

int genet_init(struct ifnet *ifp)
{
    Genet *genet;
    int ret;
    /*
     * - enable hardware.
     *   - look at ifp->if_capenable_[rx/tx]
     *   - enable promiscuous / multicast filter.
     * - attach to interrupt.
     */
    genet = ifp->if_softc;

    // Get mtu from stack for nicinfo
    genet->cfg.mtu = ifp->if_mtu;
    genet->cfg.mru = ifp->if_mtu;

    if (memcmp (genet->cfg.current_address, "\0\0\0\0\0\0", 6) != 0)  {
        /* Configure the MAC address to the UMAC */
        genet_set_macaddr(genet);
        GENET_DEBUG("MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                     genet->cfg.current_address[0], genet->cfg.current_address[1], genet->cfg.current_address[2],
                     genet->cfg.current_address[3], genet->cfg.current_address[4], genet->cfg.current_address[5]);
    }
    else {
        GENET_INFO("Invalid MAC address");
        return EINVAL;
    }

    /* Attach the interrupts */
    if(genet->iid_isr0 == -1) {
        GENET_DEBUG("Attaching interrupt(%d).", GENET_ENET_IRQ0);
        if ((ret = InterruptAttach_r(GENET_ENET_IRQ0, genet_isr0,
                   genet, sizeof(*genet), _NTO_INTR_FLAGS_TRK_MSK)) < 0) {

            GENET_ERROR("Attaching interrupt(%d) failed..", GENET_ENET_IRQ0);
            genet_cleanup(genet, 5);
            return -ret;
        }

        genet->iid_isr0 = ret;
    }

/*ISR_1(190) is for priority queues but we are not using those queues */
#ifdef GENET_ISR_1
    if(genet->iid_isr1 == -1) {
        GENET_DEBUG(" Attaching interrupt(%d).", GENET_ENET_IRQ1);
        if ((ret = InterruptAttach_r(GENET_ENET_IRQ1, genet_isr1,
                   genet, sizeof(*genet), _NTO_INTR_FLAGS_TRK_MSK )) < 0) {
            GENET_ERROR("Attaching interrupt(%d) failed..", GENET_ENET_IRQ1);
            genet_cleanup(genet, 5);
            return -ret;
        }

        genet->iid_isr1 = ret;
    }
#endif

    genet_set_multicast(genet);

    if ((ifp->if_flags & IFF_RUNNING) == 0) {

        NW_SIGLOCK(&ifp->if_snd_ex, genet->iopkt);
        ifp->if_flags_tx |= IFF_RUNNING;
        ifp->if_flags_tx &= ~IFF_OACTIVE;

        NW_SIGUNLOCK(&ifp->if_snd_ex, genet->iopkt);
        ifp->if_flags |= IFF_RUNNING;
        bsd_mii_mediachange(ifp);
    }

    return EOK;
}

void genet_stop(struct ifnet *ifp, int disable)
{
    Genet *genet;
    struct nw_work_thread *wtp;
    wtp = WTP;

    genet = ifp->if_softc;

    /* shut down mii probing */
    callout_stop(&genet->mii_callout);

    /* Stop monitoring phy */
    MDI_DisableMonitor(genet->mdi);

    genet->cfg.flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    /* Take the locks */
    NW_SIGLOCK_P(&ifp->if_snd_ex, genet->iopkt, wtp);

    /* Mark the interface as down */
    ifp->if_flags &= ~IFF_RUNNING;
    ifp->if_flags_tx &= ~(IFF_OACTIVE | IFF_RUNNING);

    /* Tx is clean, unlock ready for next time */
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, genet->iopkt, wtp);

    MDI_PowerdownPhy(genet->mdi, genet->cfg.phy_addr);
}

int genet_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data)
{
    Genet *genet;
    struct drvcom_config *dcfgp;
    struct drvcom_stats *dstp;
    struct ifdrv_com *ifdc;
    int error;

    genet = ifp->if_softc;
    error = 0;

    switch (cmd) {
        case SIOCGDRVCOM:
            ifdc = (struct ifdrv_com *) data;

            switch (ifdc->ifdc_cmd) {
                case DRVCOM_CONFIG:
                    dcfgp = (struct drvcom_config *) ifdc;

                    if (ifdc->ifdc_len != sizeof(nic_config_t)) {
                        error = EINVAL;
                        break;
                    }

                    memcpy(&dcfgp->dcom_config, &genet->cfg, sizeof(genet->cfg));
                    break;

                case DRVCOM_STATS:
                    dstp = (struct drvcom_stats *) ifdc;

                    if (ifdc->ifdc_len != sizeof(nic_stats_t)) {
                        error = EINVAL;
                        break;
                    }

                    memcpy(&dstp->dcom_stats, &genet->stats, sizeof(genet->stats));
                    break;

                default:
                    error = ENOTTY;
            }

            break;

        case SIOCSIFMEDIA:
        case SIOCGIFMEDIA:
        {
            struct ifreq *ifr = (struct ifreq *) data;
            error = ifmedia_ioctl(ifp, ifr, &genet->bsd_mii.mii_media, cmd);
            break;
        }

        default:
            error = ether_ioctl(ifp, cmd, data);
            if (error == ENETRESET) {
                /*
                 * Multicast list has changed; set the
                 * hardware filter accordingly.
                 */
                if ((ifp->if_flags & IFF_RUNNING) == 0) {
                    /*
                     * Interface is currently down: genet_init()
                     * will call genet_set_multicast() so
                     * nothing to do
                     */
                } else {
                    /*
                     * interface is up, recalculate and
                     * reprogram the hardware.
                     */
                    genet_set_multicast(genet);
                }
                error = 0;
            }
            break;
    }

    return error;
}

static void genet_cleanup(Genet *genet, int level)
{
    struct ifnet *ifp;
    ifp = &genet->sc_ec.ec_if;

    switch (level) {
        case 7:
            ether_ifdetach(ifp);
            if_detach(ifp);
        case 6:
            if(genet->iid_isr0 != -1) {
                InterruptDetach(genet->iid_isr0);
                genet->iid_isr0 = -1;
        }

#ifdef GENET_ISR_1
            if(genet->iid_isr1 != -1) {
                InterruptDetach(genet->iid_isr1);
                genet->iid_isr1 = -1;
            }
#endif

        case 5:
            interrupt_entry_remove(&genet->intr_rx, NULL);
            interrupt_entry_remove(&genet->intr_tx, NULL);
            interrupt_entry_remove(&genet->intr_link, NULL);

        case 4:
            bsd_mii_finimedia(genet);
            genet_mdi_finiphy(genet);

        case 3:
            shutdownhook_disestablish(genet->sc_sdhook);

        case 2:
            genet_rx_reap(genet, GENET_RING_SIZE_MAX);

        case 1:
            if(genet->genet_base) {
                munmap_device_memory((void *)genet->genet_base, GENET_REG_SIZE);
                genet->genet_base = (uintptr_t)NULL;
            }

            break;

        default:
            GENET_ERROR("invalid cleanup option:%d", level);
            break;
    }
}

int genet_detach(struct device *dev, int flags)
{
    Genet        *genet;
    struct ifnet *ifp;
    struct _iopkt_self *iopkt;

    genet = (struct Genet_t *)dev;
    ifp = &genet->sc_ec.ec_if;
    iopkt = genet->iopkt;
    (void)iopkt;

    cache_fini(&genet->cachectl);
    callout_stop(&genet->mii_callout);

    genet_stop(ifp, 1);
    genet_hw_stop(genet);
    genet_cleanup(genet, 7);

    return EOK;
}

void genet_shutdown(void *arg)
{
    Genet *genet;
    genet = arg;

    genet_hw_stop(genet);
    genet_cleanup(genet, 7);
    genet_stop(&genet->sc_ec.ec_if, 1);
}

#ifdef GENET_ISR_1
const struct sigevent *
genet_isr1(void *arg, int iid)
{
    Genet *genet;
    uint32_t status;
    genet = arg;

    status = (genet_intrl2_1_read(genet, BCM_GENET_0_INTRL2_CPU_STAT)
            & ~(genet_intrl2_1_read(genet, BCM_GENET_0_INTRL2_CPU_MASK_STATUS)) );

    /* clear interrupts */
    genet_intrl2_1_write(genet, BCM_GENET_0_INTRL2_CPU_CLEAR, status);

    return 0;
}
#endif

const struct sigevent *
genet_isr0(void *arg, int iid)
{
    Genet *genet;

    genet = arg;

    genet->irq0_status = ( genet_reg_read(genet, GENET_INTRL2_0_CPU_STAT)
            & ~ (genet_reg_read(genet, GENET_INTRL2_0_CPU_MASK_STATUS)) );

    if (genet->irq0_status & GENET_INTRL2_RX_DONE) {
        genet_reg_write(genet, GENET_INTRL2_0_CPU_CLEAR, genet->irq0_status & GENET_INTRL2_RX_DONE);
        return interrupt_queue(genet->iopkt, &genet->intr_rx);
    }

    if (genet->irq0_status & GENET_INTRL2_TX_DONE) {
        genet_reg_write(genet, GENET_INTRL2_0_CPU_CLEAR, genet->irq0_status & GENET_INTRL2_TX_DONE);
        return interrupt_queue(genet->iopkt, &genet->intr_tx);
    }

    if (genet->irq0_status & (GENET_INTRL2_0_LINK_UP | GENET_INTRL2_0_LINK_DOWN)){
        genet_reg_write(genet, GENET_INTRL2_0_CPU_CLEAR,
                genet->irq0_status & (GENET_INTRL2_0_LINK_UP | GENET_INTRL2_0_LINK_DOWN));
        genet->link_status = genet->irq0_status;
        return interrupt_queue(genet->iopkt, &genet->intr_link);
    }
    /* Clear the interrupt for unknown recipient*/
    genet_reg_write(genet, GENET_INTRL2_0_CPU_CLEAR, genet->irq0_status);
    TraceEvent(_NTO_TRACE_INSERTSUSEREVENT, 3, genet->irq0_status, genet->irq0_status);

    return 0;
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
