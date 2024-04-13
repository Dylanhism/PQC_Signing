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

int genet_mdio_raw_write(Genet *, uint8_t phyid, uint8_t addr, uint16_t data);
void genet_mdi_write(void *hdl, uint8_t phyid, uint8_t addr, uint16_t data);
uint16_t genet_mdio_raw_read ( Genet *,uint8_t phyid, uint8_t addr);
uint16_t genet_mdi_read(void *hdl, uint8_t phyid, uint8_t addr);

/* Currently not getting MDIO interrupt */
int link_process_interrupt(void *arg, struct nw_work_thread *wtp)
{
    return 1;
}
/*****************************************************************************/
/* PHY write routine                                                         */
/*****************************************************************************/
int genet_mdio_raw_write(Genet *genet, uint8_t phyid, uint8_t addr, uint16_t data)
{
        uint32_t reg, val;
        int i;

        reg = MDIO_CMD_WRITE | (phyid << MDIO_PHY_ADR_SHIFT) | (addr << MDIO_REG_ADDR_SHIFT) | (data& 0xFFFF);

        reg |= MDIO_START_BUSY;

        out32(genet->mdio_base, reg);

        for(i=0; i<100; i++){
                val = in32(genet->mdio_base);
            if (! (val & MDIO_START_BUSY) )
                return 0;
        }

    GENET_ERROR("MDIO write failed..");
        return -1;
}
void genet_mdi_write(void *hdl, uint8_t phyid, uint8_t addr, uint16_t data)
{
    Genet       *genet;
    genet = (Genet *)hdl;
    genet_mdio_raw_write(genet, phyid, addr, data);
}
/*****************************************************************************/
/* PHY read routine                                                          */
/*****************************************************************************/
uint16_t genet_mdio_raw_read(Genet *genet, uint8_t phyid, uint8_t addr)
{

        uint32_t reg, val, i;

        reg = MDIO_CMD_READ | (phyid << MDIO_PHY_ADR_SHIFT) | (addr << MDIO_REG_ADDR_SHIFT);

        reg |= MDIO_START_BUSY;

        out32(genet->mdio_base, reg);

        val = in32(genet->mdio_base);

        if (val & MDIO_READ_FAIL){
        GENET_ERROR("MDIO read failed..");
        return (0xffff);
        }

        for(i=0; i<100; i++){ //30: To-Do convert this logic into sleep instead for
            val = in32(genet->mdio_base);
            if (! (val & MDIO_START_BUSY) ){
        return (uint16_t)val;
            }
        }
    GENET_ERROR("MDIO read failed.");
    return (0xffff);
}

uint16_t genet_mdi_read(void *hdl, uint8_t phyid, uint8_t addr)
{
    Genet       *genet;
    genet = (Genet *)hdl;
    return genet_mdio_raw_read(genet, phyid, addr);
}
#define MII_SHADOW_REG_WRITE_ENABLE_LSB   15
#define MII_SHADOW_REG_SEL_LSB            10
/*****************************************************************************/
/* PHY shadow register read routine                                          */
/*****************************************************************************/
static int genet_mdio_shadow_reg_read(Genet *genet, uint8_t shadow_reg, uint16_t *val)
{
    uint16_t reg;

    reg = (shadow_reg << MII_SHADOW_REG_SEL_LSB);
    if (genet_mdio_raw_write(genet, PHY_ADDR, MII_REG_SHADOW, reg) != 0)
        goto fail;

    reg = genet_mdio_raw_read(genet, PHY_ADDR, MII_REG_SHADOW);
    if(reg == 0xffff)
        goto fail;

    *val = (reg & 0x3ff); // bits [9:0] are the shadowed registers values

   return 0;
fail:
   return -1;
}

/*****************************************************************************/
/* PHY shadow register write routine                                         */
/*****************************************************************************/
static int genet_mdio_shadow_reg_write(Genet *genet, uint8_t shadow_reg, uint16_t val)
{
    uint16_t reg = 0
        | (1U << MII_SHADOW_REG_WRITE_ENABLE_LSB)
        | (shadow_reg << MII_SHADOW_REG_SEL_LSB)
        | (val & 0x3ff);

    if (genet_mdio_raw_write(genet, PHY_ADDR, MII_REG_SHADOW, reg) != 0)
       return -1;

    return 0;
}
/*****************************************************************************/
/* Callback when PHY link state changes                                      */
/*****************************************************************************/
static void genet_mdi_callback(void *handle, uint8_t phyaddr, uint8_t linkstate)
{
    Genet *genet      = (Genet *)handle;
    nic_config_t *cfg = &genet->cfg;
    struct ifnet *ifp = &genet->sc_ec.ec_if;
    int                 rc;
    int                 mode;
    uint16_t            val = 0;
    char                *s = 0;
    uint32_t            cmd = 0;

    /* Ensure correct PHY is being called */
    if (phyaddr != cfg->phy_addr) {
        return;
    }

    switch(linkstate) {
        case MDI_LINK_UP:
            if ((rc = MDI_GetActiveMedia(genet->mdi, cfg->phy_addr, &mode)) != MDI_LINK_UP) {
                GENET_ERROR("MDI_GetActiveMedia() failed: 0x%x..", rc);
                mode = 0;  // force default case below - all MDI_ macros are non-zero
            }

            switch (mode) {
                case MDI_10bTFD:
                    s = "10 BaseT Full Duplex";
                    cfg->media_rate = 10 * 1000L;
                    cfg->duplex = 1;
                    break;

                case MDI_10bT:
                    s = "10 BaseT Half Duplex";
                    cfg->duplex = 0;
                    cfg->media_rate = 10 * 1000L;
                    break;

                case MDI_100bTFD:
                    s = "100 BaseT Full Duplex";
                    cfg->duplex = 1;
                    cfg->media_rate = 100 * 1000L;
                    break;

                case MDI_100bT:
                    s = "100 BaseT Half Duplex";
                    cfg->duplex = 0;
                    cfg->media_rate = 100 * 1000L;
                    break;

                case MDI_100bT4:
                    s = "100 BaseT4";
                    cfg->duplex = 0;
                    cfg->media_rate = 100 * 1000L;
                    break;

                case MDI_1000bT:
                    s = "1000 BaseT Half Duplex";
                    cfg->duplex = 0;
                    cfg->media_rate = 1000 * 1000L;
                    break;

                case MDI_1000bTFD:
                    s = "1000 BaseT Full Duplex";
                    cfg->duplex = 1;
                    cfg->media_rate = 1000 * 1000L;
                    break;

                default:
                    s = "Unknown";
                    cfg->duplex = 0;
                    cfg->media_rate = 0L;
                    break;
            }

            cmd = 0 | GENET_UMAC_CMD_TX_PAUSE_IGNORE
                    | GENET_UMAC_CMD_RX_PAUSE_IGNORE
                    | (cfg->duplex ? GENET_UMAC_CMD_HD_ENA : 0)
                    | GENET_UMAC_CMD_RX_ENA
                    | GENET_UMAC_CMD_TX_ENA;

            if (cfg->media_rate == 1000 * 1000L) {
                cmd |= GENET_SPEED_1000_MASK;
            } else if (cfg->media_rate == 100 * 1000L) {
                cmd |= GENET_SPEED_100_MASK;
            } else if (cfg->media_rate == 10 * 1000L) {
                cmd |= GENET_SPEED_100_MASK;
            } else {
                cmd |= GENET_SPEED_UNKNOWN_MASK;
            }

            genet_reg_write(genet, GENET_UMAC_CMD, cmd);

            GENET_INFO("if=%s: Link up (%s) \n",genet->sc_dev.dv_xname, s);

            /* Setup complete - start the activity and link speed indicators*/
            genet_mdio_shadow_reg_read(genet, MII_SHADOW_REG_LED_CONTROL, &val);

            val |= (1 << 4); // Activity/link
            genet_mdio_shadow_reg_write(genet, MII_SHADOW_REG_LED_CONTROL, val);

            // LED1 bits[3:0] LINK SPEED[1] (0x0)
            // LED2 bits[7:4] Activity      (0x3)
            val = (0x3 << 0) | (0x0 << 0);
            genet_mdio_shadow_reg_write(genet, MII_SHADOW_REG_LEDS1, val);

            cfg->flags &= ~NIC_FLAG_LINK_DOWN;
            if_link_state_change(ifp, LINK_STATE_UP);

            break;

        case MDI_LINK_DOWN:
            cfg->media_rate = cfg->duplex = -1;
            cfg->flags |= NIC_FLAG_LINK_DOWN;

            GENET_INFO("if=%s: Link down ", genet->sc_dev.dv_xname);

            MDI_AutoNegotiate(genet->mdi, cfg->phy_addr, NoWait);

            if_link_state_change(ifp, LINK_STATE_DOWN);
            break;

        default:
            GENET_WARNING("device:%d Unknown link state 0x%X\n", cfg->device_index, linkstate);
            break;
    }
}

int genet_ext_phy_init(struct ifnet *ifp, Genet *genet)
{
    int rc;
    int phy_idx;
    nic_config_t    *cfg;

    cfg = &genet->cfg;

    rc = MDI_Register_Extended(genet, genet_mdi_write, genet_mdi_read,
            genet_mdi_callback, (mdi_t **)&genet->mdi, NULL, 0, 0);

    if (rc != MDI_SUCCESS) {
        genet->mdi = NULL;
        GENET_ERROR("Failed to register with MDI..");
        return (ENODEV);
    }

    for (phy_idx = 0; phy_idx < MDIO_MAX_PORT + 1; phy_idx++) {
        if (MDI_FindPhy(genet->mdi, phy_idx) == MDI_SUCCESS) {
            genet->cfg.phy_addr = phy_idx;
            break;
        }

        /* If PHY is not detected then exit */
        if (phy_idx == MDIO_MAX_PORT) {
            GENET_ERROR("PHY not found..");
            return -1;
        }
    }

    cfg->connector = NIC_CONNECTOR_MII;

    rc = MDI_InitPhy(genet->mdi, cfg->phy_addr);
    if (rc != MDI_SUCCESS) {
        GENET_ERROR("Failed to initialize the PHY:%d..",rc);
        return -1;
    }

    /*Reset the PHY*/
    MDI_ResetPhy(genet->mdi, phy_idx, WaitBusy);

    GENET_INFO("Detected PHY vendor OUI:0x%x.",genet->mdi->PhyData[cfg->phy_addr]->VendorOUI);

    /* Start off down */
    MDI_PowerdownPhy(genet->mdi, phy_idx);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);

    /* register callbacks with MII managment library */
    callout_init(&genet->mii_callout);

    return EOK;
}

/*****************************************************************************/
/* MII PHY De-register Interface                                             */
/*****************************************************************************/
void genet_mdi_finiphy(Genet *genet)
{
    MDI_DeRegister(&genet->mdi);
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif
