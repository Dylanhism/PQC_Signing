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

#include "genet.h"

static void genet_MDI_MonitorPhy (void *arg)
{
    Genet* genet;
    genet = arg;

    nic_config_t* cfg;
    cfg = &genet->cfg;

    /*
     * We will probe the PHY if:
     *   probe_phy is set in genet_attach()
     *   or the link is considered down
     */
    if (genet->probe_phy || cfg->media_rate <= 0 || cfg->flags
            & NIC_FLAG_LINK_DOWN) {

        /*
         * Directly call drvr lib to probe PHY link state which in turn
         * will call genet_mdi_callback() above if link state changes
         */
        MDI_MonitorPhy(genet->mdi);
    }

    // restart timer to call us again in 2 seconds
    callout_msec(&genet->mii_callout, 2 * 1000, genet_MDI_MonitorPhy, genet);
}

/*
 * This is a callback, made by the bsd media code.  We passed
 * a pointer to this function during the ifmedia_init() call
 * in bsd_mii_initmedia().  This function is called when
 * someone makes an ioctl into us, we call into the generic
 * ifmedia source, and it make this callback to actually
 * force the speed and duplex, just as if the user had
 * set the cmd line options
 */
int bsd_mii_mediachange (struct ifnet *ifp)
{
    Genet *genet        = ifp->if_softc;
    struct ifmedia *ifm = &genet->bsd_mii.mii_media;
    nic_config_t *cfg   = &genet->cfg;
    int user_duplex     = ifm->ifm_media & IFM_FDX ? 1 : 0;
    int user_media      = ifm->ifm_media & IFM_TMASK;
    int auto_neg;

    if (!(ifp->if_flags & IFF_UP)) {
    GENET_WARNING("%s: genet interface isn't up, ioctl ignored", __FUNCTION__);
    return EOK;
    }

    if (!(ifm->ifm_media & IFM_ETHER)) {
        return EOK;
    }

    /* Media is changing so link will be down until autoneg completes */
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    switch (user_media) {
        case IFM_NONE:      /* disable media */
            cfg->media_rate = 0;
            cfg->duplex = 0;

            /* Special case, shut down the PHY and bail out */
            // callout_stop(&genet->mii_callout);
            MDI_DisableMonitor(genet->mdi);
            MDI_PowerdownPhy(genet->mdi, cfg->phy_addr);
            cfg->flags |= NIC_FLAG_LINK_DOWN;
            genet->force_link = 0;
            if_link_state_change(ifp, LINK_STATE_DOWN);
            return EOK;
            break;

        case IFM_AUTO:      /* auto-select media */
            cfg->media_rate = -1;
            cfg->duplex = -1;
            genet->force_link = -1;
            MDI_GetMediaCapable(genet->mdi, cfg->phy_addr, &auto_neg);
            genet->force_link = auto_neg;
            break;

        case IFM_10_T:      /* force 10baseT */
            cfg->media_rate = 10 * 1000L;
            cfg->duplex = user_duplex;
            genet->force_link = (user_duplex > 0) ? MDI_10bTFD : MDI_10bT;
            break;

        case IFM_100_TX:    /* force 100baseTX */
            cfg->media_rate = 100 * 1000L;
            cfg->duplex = user_duplex;
            genet->force_link = (user_duplex > 0) ? MDI_100bTFD : MDI_100bT;
            break;

        case IFM_1000_T:    /* force 1000baseT */
            cfg->media_rate = 1000 * 1000L;
            cfg->duplex = user_duplex;
            genet->force_link = (user_duplex > 0) ? MDI_1000bTFD : MDI_1000bT;
            break;

        default:
            // should never happen
            GENET_WARNING("network interface - unknown media:0x%X\n", user_media);
            return EOK;
            break;
    }

    MDI_PowerupPhy(genet->mdi, cfg->phy_addr);
    MDI_EnableMonitor(genet->mdi, 0);
    MDI_SetAdvert(genet->mdi, cfg->phy_addr, genet->force_link);
    MDI_AutoNegotiate(genet->mdi, cfg->phy_addr, NoWait);

    callout_msec(&genet->mii_callout, 3 * 1000, genet_MDI_MonitorPhy, genet);

    return EOK;
}

/*
 * This is a callback, made by the bsd media code.  We passed
 * a pointer to this function during the ifmedia_init() call
 * in bsd_mii_initmedia()
 */
static void bsd_mii_mediastatus (struct ifnet *ifp, struct ifmediareq *ifmr)
{
    Genet   *genet = ifp->if_softc;
    nic_config_t *cfg = &genet->cfg;

    genet->bsd_mii.mii_media_active = IFM_ETHER;
    genet->bsd_mii.mii_media_status = IFM_AVALID;

    if (cfg->flags & NIC_FLAG_LINK_DOWN) {
        genet->bsd_mii.mii_media_active |= IFM_NONE;
    } else {
        genet->bsd_mii.mii_media_status |= IFM_ACTIVE;

        switch (cfg->media_rate) {
            case 10 * 1000L:
                genet->bsd_mii.mii_media_active |= (cfg->duplex > 0) ? (IFM_10_T | IFM_FDX) : IFM_10_T;
                break;

            case 100 * 1000L:
                genet->bsd_mii.mii_media_active |= (cfg->duplex > 0) ? (IFM_100_TX | IFM_FDX) : IFM_100_TX;
                break;

            case 1000 * 1000L:
                genet->bsd_mii.mii_media_active |= (cfg->duplex > 0) ? (IFM_1000_T | IFM_FDX) : IFM_1000_T;
                break;

            case 0:
                genet->bsd_mii.mii_media_active |= IFM_NONE;
                break;

            /* Fallthrough */
            default:
                GENET_WARNING("unknown media:%X, forcing none.\n", cfg->media_rate);
        }
    }

    /* Stuff parameter values with hoked-up bsd values */
    ifmr->ifm_status = genet->bsd_mii.mii_media_status;
    ifmr->ifm_active = genet->bsd_mii.mii_media_active;
}

/*
 * This is called from genet_attach() in genet.c to hook up
 * to the bsd media structure.
 *
 * NOTE: Must call bsd_mii_finimedia() to free resources.
 */
void bsd_mii_initmedia(Genet *genet)
{
    nic_config_t *cfg = &genet->cfg;
    struct ifmedia    *ifm = &genet->bsd_mii.mii_media;
    genet->bsd_mii.mii_ifp = &genet->sc_ec.ec_if;

    ifmedia_init(&genet->bsd_mii.mii_media, IFM_IMASK, bsd_mii_mediachange,
                                                       bsd_mii_mediastatus);

    /* As per sample Raspberry PI4 genet driver we do
     * NOT support flow control on all the media types
     */

    /* ifconfig genet0 none  */
    ifmedia_add(ifm, IFM_ETHER|IFM_NONE, 0, NULL);

    /* ifconfig genet0 auto */
    ifmedia_add(ifm, IFM_ETHER|IFM_AUTO, 0, NULL);

    /* ifconfig genet0 10baseT (half duplex) */
    ifmedia_add(ifm, IFM_ETHER|IFM_10_T, 0, NULL);

    /* ifconfig genet0 10baseT mediaopt full-duplex */
    ifmedia_add(ifm, IFM_ETHER|IFM_10_T|IFM_FDX, 0, NULL);

    /* ifconfig genet0 100baseTX (half duplex) */
    ifmedia_add(ifm, IFM_ETHER|IFM_100_TX, 0, NULL);

    /* ifconfig genet0 100baseTX mediaopt full-duplex (full duplex) */
    ifmedia_add(ifm, IFM_ETHER|IFM_100_TX|IFM_FDX, 0, NULL);

    /* ifconfig genet0 1000baseT mediaopt full-duplex (full duplex) */
    ifmedia_add(ifm, IFM_ETHER|IFM_1000_T|IFM_FDX, 0, NULL);

    ifm->ifm_media = IFM_ETHER;

    switch (cfg->media_rate) {
        case -1:
            ifm->ifm_media = IFM_ETHER | IFM_AUTO;
            break;

        case 10*1000:
            ifm->ifm_media = IFM_ETHER | IFM_10_T;
            break;

        case 100*1000:
            ifm->ifm_media = IFM_ETHER | IFM_100_TX;
            break;

        case 1000*1000:
            ifm->ifm_media = IFM_ETHER | IFM_1000_T;
            break;

        case 0:
            ifm->ifm_media = IFM_ETHER | IFM_NONE;
            break;

        /* Fallthrough */
        default:
            GENET_WARNING("unknown initial media forcing none.");
    }

    ifmedia_set(ifm, ifm->ifm_media);

}

/*
 * Free any memory associated with the bsd mii.
 * ifmedia_add() allocates memory and must be freed by
 * ifmedia_delete_instance().
 */
void bsd_mii_finimedia(Genet *genet)
{
    ifmedia_delete_instance(&genet->bsd_mii.mii_media, IFM_INST_ANY);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
