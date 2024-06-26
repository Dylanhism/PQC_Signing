/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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


#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <hw/inout.h>
#include <sys/mman.h>

#include <internal.h>

#ifdef SDIO_HC_SDHCI_DWC

//#define SDHCI_DWC_DEBUG

#include <sdhci_dwc.h>

static int sdhci_pwr( sdio_hc_t *hc, int vdd );
static int sdhci_drv_type( sdio_hc_t *hc, int type );
static int sdhci_bus_mode( sdio_hc_t *hc, int bus_mode );
static int sdhci_bus_width( sdio_hc_t *hc, int width );
static int sdhci_dwc_clk( sdio_hc_t *hc, int clk );
static int sdhci_dwc_timing( sdio_hc_t *hc, int timing );
static int sdhci_signal_voltage( sdio_hc_t *hc, int signal_voltage );
static int sdhci_cd( sdio_hc_t *hc );
static int sdhci_cmd( sdio_hc_t *hc, sdio_cmd_t *cmd );
static int sdhci_abort( sdio_hc_t *hc, sdio_cmd_t *cmd );
static int sdhci_event( sdio_hc_t *hc, sdio_event_t *ev );
static int sdhci_tune( sdio_hc_t *hc, int op );

static sdio_hc_entry_t sdhci_dwc_hc_entry = { 16,
                                        sdhci_dwc_dinit, NULL,
                                        sdhci_cmd, sdhci_abort,
                                        sdhci_event, sdhci_cd, sdhci_pwr,
                                        sdhci_dwc_clk, sdhci_bus_mode,
                                        sdhci_bus_width, sdhci_dwc_timing,
                                        sdhci_signal_voltage, sdhci_drv_type,
                                        NULL, sdhci_tune, NULL
                                        };


static int sdhci_dwc_enable_clk(sdio_hc_t *hc, uint16_t clk);

#ifdef SDHCI_DWC_DEBUG
static int sdhci_reg_dump( sdio_hc_t *hc, const char *func, int line )
{
    sdhci_hc_t  *sdhc;
    uintptr_t       base;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;

    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: line %d", func, line );
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: HCTL_VER %x, CAP %x, CAP2 %x, MCCAP %x",
        __FUNCTION__,
        sdhci_in16( base + SDHCI_HCTL_VERSION ),
        sdhci_in32( base + SDHCI_CAP ),
        sdhci_in32( base + SDHCI_CAP2 ),
        sdhci_in32( base + SDHCI_MCCAP ) );

    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: SDMA_ARG2 %x, ADMA %x, BLK %x, ARG %x, CMD %x, RSP10 %x, RSP32 %x, RSP54 %x,  RSP76 %x",
        __FUNCTION__,
        sdhci_in32( base + SDHCI_SDMA_ARG2 ),
        sdhci_in32( base + SDHCI_ADMA_ADDRL ),
        sdhci_in32( base + SDHCI_BLK ),
        sdhci_in32( base + SDHCI_ARG ),
        sdhci_in32( base + SDHCI_CMD ),
        sdhci_in32( base + SDHCI_RESP0 ),
        sdhci_in32( base + SDHCI_RESP1 ),
        sdhci_in32( base + SDHCI_RESP2 ),
        sdhci_in32( base + SDHCI_RESP3 ) );

    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: PSTATE %x, HCTL %x, HCTL2 %x, SYSCTL %x, STAT %x, IE %x, ISE %x, AC12 %x, ADMAES %x ADMASAL %x",
        __FUNCTION__,
        sdhci_in32( base + SDHCI_PSTATE ),
        sdhci_in32( base + SDHCI_HCTL ),
        sdhci_in16( base + SDHCI_HCTL2 ),
        sdhci_in32( base + SDHCI_SYSCTL ),
        sdhci_in32( base + SDHCI_IS ),
        sdhci_in32( base + SDHCI_IE ),
        sdhci_in32( base + SDHCI_ISE ),
        sdhci_in32( base + SDHCI_AC12 ),
        sdhci_in32( base + SDHCI_ADMA_ES ),
        sdhci_in32( base + SDHCI_ADMA_ADDRL ) );

    return( EOK );
}
#endif

static int sdhci_waitmask( sdio_hc_t *hc, uint32_t reg, uint32_t mask, uint32_t val, uint32_t usec )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        cnt;
    int             stat;
    int             rval;
    uint32_t        iter;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    stat    = ETIMEDOUT;
    rval    = 0;

        // fast poll for 1ms - 1us intervals
    for( cnt = min( usec, 1000 ); cnt; cnt-- ) {
        if( ( ( rval = sdhci_in32( base + reg ) ) & mask ) == val ) {
            stat = EOK;
            break;
        }
        nanospin_ns( 1000L );
    }

    if( usec > 1000 && stat ) {
        iter    = usec / 1000L;
        for( cnt = iter; cnt; cnt-- ) {
            if( ( ( rval = sdhci_in32( base + reg ) ) & mask ) == val ) {
                stat = EOK;
                break;
            }
            delay( 1 );
        }
    }

#ifdef SDHCI_DWC_DEBUG
    if( !cnt ) {
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: reg %x, mask %x, val %x, rval %x", __FUNCTION__, reg, mask, val, sdhci_in32( base + reg ) );
    }
#endif

    return( stat );
}

static void dwc_phy_pad_config( sdio_hc_t *hc )
{
    sdhci_hc_t  *sdhc;
    uintptr_t   base;

    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    if( hc->flags & HC_FLAG_DEV_MMC ) {
        sdhci_out16( base + DWC_MSHC_CMDPAD_CNFG, DWC_MSHC_PHY_PAD_EMMC_DAT );
        sdhci_out16( base + DWC_MSHC_DATPAD_CNFG, DWC_MSHC_PHY_PAD_EMMC_DAT );
        sdhci_out16( base + DWC_MSHC_CLKPAD_CNFG, DWC_MSHC_PHY_PAD_EMMC_CLK );
        sdhci_out16( base + DWC_MSHC_STBPAD_CNFG, DWC_MSHC_PHY_PAD_EMMC_STB );
        sdhci_out16( base + DWC_MSHC_RSTNPAD_CNFG, DWC_MSHC_PHY_PAD_EMMC_DAT );
    } else {
        sdhci_out16( base + DWC_MSHC_CMDPAD_CNFG, DWC_MSHC_PHY_PAD_SD_DAT );
        sdhci_out16( base + DWC_MSHC_DATPAD_CNFG, DWC_MSHC_PHY_PAD_SD_DAT );
        sdhci_out16( base + DWC_MSHC_CLKPAD_CNFG, DWC_MSHC_PHY_PAD_SD_CLK );
        sdhci_out16( base + DWC_MSHC_STBPAD_CNFG, DWC_MSHC_PHY_PAD_SD_STB );
        sdhci_out16( base + DWC_MSHC_RSTNPAD_CNFG, DWC_MSHC_PHY_PAD_SD_DAT );
    }
}

static void dwc_phy_delay_config( sdio_hc_t *hc )
{
    sdhci_hc_t  *sdhc;
    uintptr_t   base;

    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    sdhci_out8( base + DWC_MSHC_COMMDL_CNFG, 1 );
    sdhci_out8( base + DWC_MSHC_SDCLKDL_CNFG, 0 );
    sdhci_out8( base + DWC_MSHC_SMPLDL_CNFG, 8 );
    sdhci_out8( base + DWC_MSHC_ATDL_CNFG, 8 );
}

static int dwc_phy_init( sdio_hc_t *hc )
{
    sdhci_hc_t  *sdhc;
    uintptr_t   base;

    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    sdhci_out32( base + DWC_MSHC_PHY_CNFG, 0 );

    /* Disable the clock */
    sdhci_out16( base + SDHCI_SYSCTL, 0 );

    dwc_phy_pad_config( hc );
    dwc_phy_delay_config( hc );

    /* Wait max 150 ms */
    if( sdhci_waitmask( hc, DWC_MSHC_PHY_CNFG, PHY_PWRGOOD, PHY_PWRGOOD, 150000 ) != EOK ) {
        return( EIO );
    }

    sdhci_out32( base + DWC_MSHC_PHY_CNFG, PAD_SN_DEFAULT | PAD_SP_DEFAULT );

    /* de-assert the phy */
    sdhci_out32( base + DWC_MSHC_PHY_CNFG, PAD_SN_DEFAULT | PAD_SP_DEFAULT | PHY_RSTN );

    /* Enable the clock */
    sdhci_dwc_enable_clk( hc, 0 );

    return( EOK );
}

static int dwc_phy_dll_config( sdio_hc_t *hc )
{
    uint16_t    clk_ctrl;
    uint32_t    reg;
    sdhci_hc_t  *sdhc;
    uintptr_t   base;

    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    sdhci_out8( base + DWC_MSHC_DLL_CTRL, 0 );

    /* Disable the card clock */
    clk_ctrl = sdhci_in16( base + SDHCI_SYSCTL );
    clk_ctrl &= ~( SDHCI_SYSCTL_CEN );
    sdhci_out16( base + SDHCI_SYSCTL, clk_ctrl );

    sdhci_out8( base + DWC_MSHC_DLL_CNFG1, 0x20 );
    sdhci_out8( base + DWC_MSHC_DLL_CNFG2, 0 );
    sdhci_out8( base + DWC_MSHC_DLLDL_CNFG, 0x60 );
    sdhci_out8( base + DWC_MSHC_DLL_OFFSET, 0 );
    sdhci_out16( base + DWC_MSHC_DLLLBT_CNFG, 0 );

    /* Enable the clock */
    clk_ctrl |= SDHCI_SYSCTL_CEN;
    sdhci_out16( base + SDHCI_SYSCTL, clk_ctrl );

    sdhci_out8( base + DWC_MSHC_DLL_CTRL, 1 );
    /* Wait max 150 ms */
    if( sdhci_waitmask( hc, DWC_MSHC_DLLLBT_CNFG, LOCK_STS, LOCK_STS, 150000 ) != EOK ) {
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  lock wait timedout", __FUNCTION__ );
        return( EIO );
    }

    reg = sdhci_in32( base + DWC_MSHC_DLLLBT_CNFG );

    return (reg & ERROR_STS) ? EIO : EOK;
}

static int dwc_reset( sdio_hc_t *hc, uint32_t rst )
{
    sdhci_hc_t  *sdhc;
    sdhci_dwc_t *dwchc;
    uintptr_t   base;
    uint32_t    off;
    uint16_t    val;
#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  rst %x", __FUNCTION__, rst );
#endif

    dwchc = (sdhci_dwc_t *)hc->cs_hdl;
    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    // enable V4 mode
    if( sdhc->flags & SF_V4_MODE ) {
        sdhci_out16( base + SDHCI_HCTL2,
                sdhci_in16(base + SDHCI_HCTL2) | SDHCI_HCTL2_V4_MODE );
    }

    // set card type
    off = sdhci_in16( base + SDHCI_VENDOR_BASE_REG ) & 0xFFF;
    val = sdhci_in16( base + off + SDHCI_VENDER_EMMC_CTRL_REG );
    if( hc->flags & HC_FLAG_DEV_MMC ) {
        val |= SDHCI_IS_EMMC_CARD_MASK;
    } else {
        val &= ~SDHCI_IS_EMMC_CARD_MASK;
    }
    sdhci_out16( base + off + SDHCI_VENDER_EMMC_CTRL_REG, val );

    if( !( hc->caps & ( HC_CAP_SV_3_0V | HC_CAP_SV_3_3V ) ) ) {
        // Enable 1.8v signal
        sdhci_out16( base + SDHCI_HCTL2,
        sdhci_in16( base + SDHCI_HCTL2 ) | SDHCI_HCTL2_SIG_1_8V );
    }

    if( dwchc->phy ) {
        return dwc_phy_init( hc );
    } else {
        return EOK;
    }
}

static int sdhci_dwc_reset( sdio_hc_t *hc, uint32_t rst )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        sctl;
    int             status;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  rst %x", __FUNCTION__, rst );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;

    sctl    = sdhci_in32( base + SDHCI_SYSCTL );

        // wait up to 100 ms for reset to complete
    sdhci_out32( base + SDHCI_SYSCTL, sctl | rst | ( SDHCI_SYSCTL_DTO_MAX ) );    // maximum data timeout
    status = sdhci_waitmask( hc, SDHCI_SYSCTL, rst, 0, 100000 );

    if( status == EOK && rst == SDHCI_SYSCTL_SRA ) {
        status = dwc_reset( hc, rst );
    }

    return( status );
}

static int sdhci_pio_xfer( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    int             len;
    int             msk;
    int             blksz;
    uint8_t         *addr;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    msk     = ( cmd->flags & SCF_DIR_IN ) ? SDHCI_PSTATE_BRE : SDHCI_PSTATE_BWE;

    while( ( sdhci_in32( base + SDHCI_PSTATE ) & msk ) ) {
        blksz   = cmd->blksz;
        while( blksz ) {
            if( sdio_sg_nxt( hc, &addr, &len, blksz ) ) {
                break;
            }
            blksz   -= len;
            len     /= 4;
            if( ( cmd->flags & SCF_DIR_IN ) ) {
                sdhci_in32s( addr, len, base + SDHCI_DATA );
            }
            else {
                sdhci_out32s( addr, len, base + SDHCI_DATA );
            }
        }
    }

    return( EOK );
}

static int sdhci_intr_event( sdio_hc_t *hc )
{
    sdhci_hc_t      *sdhc;
    sdio_cmd_t      *cmd;
    uint32_t        sts;
    int             cs;
    int             idx;
    uint32_t        rsp;
    uint8_t         rbyte;
    uintptr_t       base;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    cs  = CS_CMD_INPROG;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  is 0x%x, ie 0x%x", __FUNCTION__, sdhci_in32( base + SDHCI_IS ), sdhci_in32( base + SDHCI_IE ) );
#endif

#if !defined(__ARM__) && !defined(__aarch64__)
    if( !( sdhci_in16( base + SDHCI_SLOT_IS ) & ( 1 << hc->slot ) ) ) {
        return( EOK );
    }
#endif

    sts = sdhci_in32( base + SDHCI_IS );

    if( !sts ) {                    // shared interrupt
        return( EOK );
    }

    sts &= sdhci_in32( base + SDHCI_IE ) | SDHCI_INTR_ERRI;
    sdhci_out32( base + SDHCI_IS, sts );

    if( ( sts & ( SDHCI_INTR_CINS | SDHCI_INTR_CREM ) ) ) {
        sdio_hc_event( hc, HC_EV_CD );
    }

    if( ( sts & ( SDCHI_INTR_RETUNE ) ) ) {
        sdio_hc_event( hc, HC_EV_TUNE );
    }

    if( ( cmd = hc->wspc.cmd ) == NULL ) {
        return( EOK );
    }

    if( ( sts & SDHCI_INTR_ERRI ) ) {           // Check of errors
#ifdef SDHCI_DWC_DEBUG
        sdhci_reg_dump( hc, __FUNCTION__, __LINE__ );
#else
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  is 0x%x", __FUNCTION__, sts );
#endif
        if( sts & SDHCI_INTR_DTO )      cs = CS_DATA_TO_ERR;
        if( sts & SDHCI_INTR_DCRC )     cs = CS_DATA_CRC_ERR;
        if( sts & SDHCI_INTR_DEB )      cs = CS_DATA_END_ERR;
        if( sts & SDHCI_INTR_CTO )      cs = CS_CMD_TO_ERR;
        if( sts & SDHCI_INTR_CCRC )     cs = CS_CMD_CRC_ERR;
        if( sts & SDHCI_INTR_CEB )      cs = CS_CMD_END_ERR;
        if( sts & SDHCI_INTR_CIE )      cs = CS_CMD_IDX_ERR;
        if( sts & SDHCI_INTR_ADMAE )    cs = CS_DATA_TO_ERR;
        if( sts & SDHCI_INTR_ADMAE ) {
            sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  ADMA 0x%x :%x :%x", __FUNCTION__,
                            sdhci_in32( base + SDHCI_ADMA_ES ), sdhci_in32( base + SDHCI_ADMA_ADDRL ), sdhci_in32(base + SDHCI_ADMA_ADDRH ) );
        }
        if( sts & SDHCI_INTR_ACE ) {
            sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s:  is 0x%x, ac12 0x%x", __FUNCTION__, sdhci_in32( base + SDHCI_IS ), sdhci_in32( base + SDHCI_AC12 ) );
            cs = CS_DATA_TO_ERR;
        }
        if( !cs )                       cs = CS_CMD_CMP_ERR;
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRD );
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRC );
    }
    else {
        if( ( sts & SDHCI_INTR_CC ) ) {
            if( ( cmd->flags & SCF_RSP_136 ) ) {
                    // CRC is not included in the response reg, left
                    // shift 8 bits to match the 128 CID/CSD structure
                for( idx = 0, rbyte = 0; idx < 4; idx++ ) {
                    rsp             = sdhci_in32( base + SDHCI_RESP0 + idx * 4 );
                    cmd->rsp[3 - idx] = (rsp << 8) + rbyte;
                    rbyte           = rsp >> 24;
                }
            }
            else if( ( cmd->flags & SCF_RSP_PRESENT ) ) {
                cmd->rsp[0] = sdhci_in32( base + SDHCI_RESP0 );
            }
            cs = CS_CMD_CMP;
        }

        if( ( sts & SDHCI_INTR_TC ) ) {
            cs = CS_CMD_CMP;
            cmd->rsp[0] = sdhci_in32( base + SDHCI_RESP0 );
        }
        else if( ( sts & SDHCI_INTR_DMA ) ) {   // restart on dma boundary
            sdhci_out32( base + SDHCI_SDMA_ARG2, sdhci_in32( base + SDHCI_SDMA_ARG2 ) );
        }

        if( ( sts & ( SDHCI_INTR_BWR | SDHCI_INTR_BRR ) ) ) {
            if( cmd->opcode == SD_SEND_TUNING_BLOCK ||
                    cmd->opcode == MMC_SEND_TUNING_BLOCK ) {
                cs = CS_CMD_CMP;
            }
            else {
                cs = sdhci_pio_xfer( hc, cmd );
            }
        }
    }

    if( ( sts & SDHCI_INTR_CREM ) ) {
        cs = CS_CARD_REMOVED;
    }

    if( cs != CS_CMD_INPROG ) {
        sdio_cmd_cmplt( hc, cmd, cs );
    }

    return( EOK );
}

static int sdhci_event( sdio_hc_t *hc, sdio_event_t *ev )
{
    int     status;

    switch( ev->code ) {
        case HC_EV_INTR:
            status = sdhci_intr_event( hc );
            InterruptUnmask( hc->cfg.irq[0], hc->hc_iid );
            break;

        default:
#ifdef BS_EVENT
            status = bs_event( hc, ev );
#else
            status = ENOTSUP;
#endif
            break;
    }

    return( status );
}

static int sdhci_adma_setup( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    sdhci_hc_t          *sdhc;
    sdhci_adma64_t      *adma;
    sdio_sge_t          *sgp;
    int                 sgc;
    int                 sgi;
    int                 acnt;
    int                 alen;
    int                 sg_count;
    paddr64_t           paddr;
    int                 desc_sz;
    sdio_hc_cfg_t       *cfg = &hc->cfg;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    adma    = (sdhci_adma64_t *)sdhc->adma;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: ", __FUNCTION__ );
#endif

    sgc = cmd->sgc;
    sgp = cmd->sgl;

    if( !( cmd->flags & SCF_DATA_PHYS ) ) {
        sdio_vtop_sg( sgp, sdhc->sgl, sgc, cmd->mhdl );
        sgp = sdhc->sgl;
    }

    if( sdhc->flags & SF_USE_ADMA64 ) {
        desc_sz = SDHCI_ADMA2_64_DESC_SZ( sdhc );
    } else {
        desc_sz = sizeof( sdhci_adma32_t );
    }

    for( sgi = 0, acnt = 0; sgi < sgc; sgi++, sgp++ ) {
        paddr       = sgp->sg_address + cfg->bmstr_xlat;
        sg_count    = sgp->sg_count;
        while( sg_count ) {
            alen        = min( sg_count, SDHCI_ADMA2_MAX_XFER );
            adma->attr  = SDHCI_ADMA2_VALID | SDHCI_ADMA2_TRAN;
            adma->addr_lo   = paddr;
            adma->len   = alen;
            if( ( sdhc->flags & SF_USE_ADMA64 ) ) {
                adma->addr_hi = paddr >> 32;
            }
            sg_count    -= alen;
            paddr       += alen;
            adma = (sdhci_adma64_t *)( (uintptr_t)adma + desc_sz );
            if( ++acnt > ADMA_DESC_MAX ) {
                return( ENOTSUP );
            }
        }
    }

    adma = (sdhci_adma64_t *)( (uintptr_t)adma - desc_sz );
    adma->attr |= SDHCI_ADMA2_END;

    __cpu_membarrier();

    sdhci_out32( sdhc->base + SDHCI_ADMA_ADDRL, (uint32_t)( sdhc->admap + cfg->bmstr_xlat ) );
    if( ( sdhc->flags & SF_USE_ADMA64 ) ) {
        sdhci_out32( sdhc->base + SDHCI_ADMA_ADDRH, (uint32_t)( ( sdhc->admap + cfg->bmstr_xlat ) >> 32 ) );
    }

    return( EOK );
}

static int sdhci_sdma_setup( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    sdhci_hc_t      *sdhc;
    sdio_sge_t      *sgp;
    int             sgc;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: ", __FUNCTION__ );
#endif

    sgc = cmd->sgc;
    sgp = cmd->sgl;

    if( !( cmd->flags & SCF_DATA_PHYS ) ) {
        sdio_vtop_sg( sgp, sdhc->sgl, sgc, cmd->mhdl );
        sgp = sdhc->sgl;
    }

    sdhci_out32( sdhc->base + SDHCI_SDMA_ARG2, sgp->sg_address );

    return( EOK );
}

static int sdhci_xfer_setup( sdio_hc_t *hc, sdio_cmd_t *cmd,
                                uint32_t *command, uint32_t *imask )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        hctl;
    int             status;

    sdhc        = (sdhci_hc_t *)hc->cs_hdl;
    status      = EOK;
    base        = sdhc->base;
    hctl        = sdhci_in32( base + SDHCI_HCTL ) & ~SDHCI_HCTL_DMA_MSK;
    *command    |= ( cmd->flags & SCF_DIR_IN ) ? ( SDHCI_CMD_DP | SDHCI_CMD_DDIR ) : SDHCI_CMD_DP;
    *imask      |= SDHCI_INTR_TC | SDHCI_INTR_DTO | SDHCI_INTR_DCRC | SDHCI_INTR_DEB;

    status = sdio_sg_start( hc, cmd->sgl, cmd->sgc );

    if( cmd->sgc && ( hc->caps & HC_CAP_DMA ) ) {
        if( ( sdhc->flags & SF_USE_ADMA ) ) {
            if( ( status = sdhci_adma_setup( hc, cmd ) ) == EOK ) {
                hctl    |= SDHCI_HCTL_ADMA32;
                if( sdhc->flags & SF_USE_ADMA64 ) {
                    if( sdhc->flags & SF_V4_MODE ) {
                        sdhci_out16( base + SDHCI_HCTL2,
                                sdhci_in16( base + SDHCI_HCTL2 ) | SDHCI_HCTL2_64BIT_ADDR );
                    } else {
                        hctl    |= SDHCI_HCTL_ADMA64;
                    }
                }
                *command    |= SDHCI_CMD_DE;
            }
        }
        else {
            if( ( status = sdhci_sdma_setup( hc, cmd ) ) == EOK ) {
                hctl        |= SDHCI_HCTL_SDMA;
                *command    |= SDHCI_CMD_DE;
            }
        }
    }

    if( status || !( hc->caps & HC_CAP_DMA ) ) {    // use PIO
        if( ( cmd->flags & SCF_DATA_PHYS ) ) {
            return( status );
        }
        status = EOK;
        *imask |= ( cmd->flags & SCF_DIR_IN ) ? SDHCI_INTR_BRR : SDHCI_INTR_BWR;
    }

    if( cmd->blks > 1 ) {
        *command |= SDHCI_CMD_MBS | SDHCI_CMD_BCE;
        if( ( hc->caps & HC_CAP_ACMD23 ) && ( cmd->flags & SCF_SBC ) ) {
            *command |= SDHCI_CMD_ACMD23;
            sdhci_out32( base + SDHCI_SDMA_ARG2, cmd->blks );
        }
        else if( ( hc->caps & HC_CAP_ACMD12 ) ) {
            *command |= SDHCI_CMD_ACMD12;
        }
    }

    sdhci_out32( base + SDHCI_HCTL, hctl );
    sdhci_out32( base + SDHCI_BLK, cmd->blksz | SDHCI_BLK_SDMA_BOUNDARY_512K |
                ( cmd->blks << SDHCI_BLK_BLKCNT_SHIFT ) );

    return( status );
}

static int sdhci_cmd( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        pmask;
    uint32_t        imask;
    int             status;
    uint32_t        command;

    sdhc            = (sdhci_hc_t *)hc->cs_hdl;
    base            = sdhc->base;

    imask   = SDHCI_INTR_DFLTS;

#ifndef BS_GPIO_INS_RMV
    if( !( hc->caps & HC_CAP_SLOT_TYPE_EMBEDDED ) ) {
        imask   |= SDHCI_INTR_CINS | SDHCI_INTR_CREM;
    }
#endif

    pmask   = SDHCI_PSTATE_CMDI;
    command = cmd->opcode << 24;

    sdhci_out32( base + SDHCI_IS, SDHCI_INTR_CLR_MSK ); // Clear Status

    if( cmd->opcode == MMC_STOP_TRANSMISSION ) {
        command |= SDHCI_CMD_TYPE_CMD12;
    }

    if( ( cmd->flags & SCF_DATA_MSK ) ) {
        pmask |= SDHCI_PSTATE_DATI;
        if( ( status = sdhci_xfer_setup( hc, cmd, &command, &imask ) ) != EOK ) {
            return( status );
        }
    }
    else {
        imask |= SDHCI_INTR_CC;                     // Enable command complete intr
    }

    if( ( cmd->flags & SCF_RSP_PRESENT ) ) {
        if( ( cmd->flags & SCF_RSP_136 ) ) {
            command |= SDHCI_CMD_RSP_TYPE_136;
        }
        else if( ( cmd->flags & SCF_RSP_BUSY ) ) {  // Response with busy check
            command |= SDHCI_CMD_RSP_TYPE_48b;
        }
        else {
            command |= SDHCI_CMD_RSP_TYPE_48;
        }

        if( ( cmd->flags & SCF_RSP_OPCODE ) ) {     // Index check
            command |= SDHCI_CMD_CICE;
        }

        if( ( cmd->flags & SCF_RSP_CRC ) ) {        // CRC check
            command |= SDHCI_CMD_CCCE;
        }
    }

    if( ( status = sdhci_waitmask( hc, SDHCI_PSTATE, pmask, 0, 100000 ) ) ) {
        return( status );
    }

    sdhci_out32( base + SDHCI_IE,  imask );
    sdhci_out32( base + SDHCI_ARG, cmd->arg );
    sdhci_out32( base + SDHCI_CMD, command );

    return( EOK );
}

static int sdhci_abort( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    return( EOK );
}

static int sdhci_pwr( sdio_hc_t *hc, int vdd )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        hctl;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: vdd 0x%x", __FUNCTION__, vdd );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;

    // disable interrupts
    sdhci_out32( base + SDHCI_IE, 0 );

    if( !hc->vdd || !vdd ) {            // reset core
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRC );
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRD );
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRA );
        sdhci_out32( base + SDHCI_ISE, SDHCI_INTR_ALL );
    }

    if( vdd ) {
        hctl = sdhci_in32( base + SDHCI_HCTL ) & ~SDHCI_HCTL_SDVS_MSK;
        switch( vdd ) {
            case OCR_VDD_17_195:
                hctl |= SDHCI_HCTL_SDVS1V8; break;

            case OCR_VDD_30_31:
            case OCR_VDD_29_30:
                hctl |= SDHCI_HCTL_SDVS3V0; break;

            case OCR_VDD_33_34:
            case OCR_VDD_32_33:
                hctl |= SDHCI_HCTL_SDVS3V3; break;

            default:
                return( EINVAL );
        }

        sdhci_out32( base + SDHCI_HCTL, hctl );
        sdhci_out32( base + SDHCI_HCTL, hctl | SDHCI_HCTL_SDBP );
        sdhci_waitmask( hc, SDHCI_HCTL, SDHCI_HCTL_SDBP, SDHCI_HCTL_SDBP, 10000 );
    }
    else {
        sdhci_out32( base + SDHCI_HCTL, 0 );
    }

#ifndef BS_GPIO_INS_RMV
    if( !( hc->caps & HC_CAP_SLOT_TYPE_EMBEDDED ) ) {
        sdhci_out32( base + SDHCI_IE, SDHCI_INTR_CINS | SDHCI_INTR_CREM );
    }
#endif

    hc->vdd = vdd;

    return( EOK );
}

static int sdhci_bus_mode( sdio_hc_t *hc, int bus_mode )
{
#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: bus_mode %d", __FUNCTION__, bus_mode );
#endif

    hc->bus_mode = bus_mode;

    return( EOK );
}

static int sdhci_bus_width( sdio_hc_t *hc, int width )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        hctl;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: width %d", __FUNCTION__, width );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    hctl    = sdhci_in8( base + SDHCI_HCTL ) & ~SDHCI_HCTL_DTW4;
    hctl    &= ~( hc->version ? SDHCI_HCTL_BW8 : SDHCI_HCTL_MMC8 );

    switch( width ) {
        case BUS_WIDTH_8:
            hctl |= ( hc->version ? SDHCI_HCTL_BW8 : SDHCI_HCTL_MMC8 );
            break;

        case BUS_WIDTH_4:
            hctl |= SDHCI_HCTL_DTW4;
            break;

        case BUS_WIDTH_1:
        default:
            break;
    }

    sdhci_out8( base + SDHCI_HCTL, hctl );

    hc->bus_width = width;

    return( EOK );
}

static int sdhci_dwc_enable_clk( sdio_hc_t *hc, uint16_t clk )
{
    sdhci_hc_t  *sdhc;
    uintptr_t   base;

    sdhc = (sdhci_hc_t *)hc->cs_hdl;
    base = sdhc->base;

    clk |= SDHCI_SYSCTL_ICE;
    sdhci_out16( base + SDHCI_SYSCTL, clk );

    if( sdhci_waitmask( hc, SDHCI_SYSCTL, SDHCI_SYSCTL_ICS, SDHCI_SYSCTL_ICS, 150 * 1000 ) ) {
        return( ETIMEDOUT );
    }

    if( ( hc->version >= SDHCI_SPEC_VER_410 && ( sdhc->flags & SF_V4_MODE ) ) ) {
        clk  = sdhci_in16( base + SDHCI_SYSCTL );
        clk |= SDHCI_SYSCTL_PLLEN;
        clk &= ~SDHCI_SYSCTL_ICS;
        sdhci_out16( base + SDHCI_SYSCTL, clk );

        if( sdhci_waitmask( hc, SDHCI_SYSCTL, SDHCI_SYSCTL_ICS, SDHCI_SYSCTL_ICS, 150 * 1000 ) ) {
            return( ETIMEDOUT );
        }
    }

    sdhci_out16( base + SDHCI_SYSCTL,
        sdhci_in16( base + SDHCI_SYSCTL ) | SDHCI_SYSCTL_CEN );

    return( EOK );
}

// dwc function
static int sdhci_dwc_clk( sdio_hc_t *hc, int clk )
{
    sdhci_dwc_t *dwchc;
    sdhci_hc_t  *sdhc;
    uintptr_t   base;
    uint32_t    sctl;
    uint32_t    clkd = 0;
    int         div = 1;
    uint32_t    clk_mul = 1;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: %d", __FUNCTION__, clk );
#endif

    dwchc = (sdhci_dwc_t *)hc->cs_hdl;
    sdhc  = (sdhci_hc_t *)hc->cs_hdl;
    base  = sdhc->base;

    sctl  = sdhci_in32( base + SDHCI_SYSCTL );

    // Stop clock
    sctl &= ~(SDHCI_SYSCTL_ICE | SDHCI_SYSCTL_CEN | SDHCI_SYSCTL_DTO_MSK |
                SDHCI_SYSCTL_CGS_PROG | SDHCI_SYSCTL_SDCLKFS_MSK);
    sctl |= SDHCI_SYSCTL_DTO_MAX | SDHCI_SYSCTL_SRC | SDHCI_SYSCTL_SRD;
    sdhci_out32( base + SDHCI_SYSCTL, sctl );

    if( hc->version >= SDHCI_SPEC_VER_3 ) {
        if( sdhc->clk_mul ) {
            if( !( sdhci_in16( base + SDHCI_HCTL2 ) & SDHCI_HCTL2_PRESET_VAL ) ) {
                for( clkd = 1; clkd < 1024; clkd++ ) {
                    if( ( ( hc->clk_max * sdhc->clk_mul ) / clkd ) <= clk ) {
                        break;
                    }
                }
                sctl |= SDHCI_SYSCTL_CGS_PROG;
                clk_mul = sdhc->clk_mul;
                div = clkd;
                clkd--;
            }
        }
        else {
            // 10 bit Divided Clock Mode
            for( clkd = 2; clkd < SDHCI_CLK_MAX_DIV_SPEC_VER_3; clkd += 2 ) {
                if( ( hc->clk_max / clkd ) <= clk ) {
                    break;
                }
            }
            if( hc->clk_max <= clk ) {
                clkd = 1;
            }
            div = clkd;
            clkd >>= 1;
        }
    }
    else {
        // 8 bit Divided Clock Mode
        for( clkd = 1; clkd < SDHCI_CLK_MAX_DIV_SPEC_VER_2; clkd *= 2 ) {
            if( ( hc->clk_max / clkd ) <= clk ) {
                break;
            }
        }
        div = clkd;
        clkd >>= 1;
    }

    // Need platform specific clock function
    if( dwchc->set_clk != NULL ) {
        dwchc->set_clk( hc, clk, div );
    }

    if( hc->timing == TIMING_HS400 && clk > 100000000 ) {
        if( dwchc->phy ) {
            dwc_phy_dll_config( hc );
        }
    }

    // Enable clock
    sctl |= SDHCI_SYSCTL_CLK( clkd );
    sdhci_out32( base + SDHCI_SYSCTL, sctl );
    sdhci_dwc_enable_clk( hc, 0 );

    hc->clk = (hc->clk_max * clk_mul) / div;

    delay(1);

    return( EOK );
}

static int sdhci_dwc_timing( sdio_hc_t *hc, int timing )
{
    sdhci_hc_t      *sdhc;
    sdhci_dwc_t     *dwchc;
    uintptr_t       base;
    uint32_t        hctl;
    uint16_t        hctl2;
    uint32_t        ddr_mode = 0;
    int             status = EOK;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: timing %d", __FUNCTION__, timing );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    dwchc   = (sdhci_dwc_t *)hc->cs_hdl;
    base    = sdhc->base;
    hctl    = sdhci_in32( base + SDHCI_HCTL ) & ~SDHCI_HCTL_HSE;
    hctl2   = 0;

    if( hc->version >= SDHCI_SPEC_VER_3 ) {
        hctl2   = sdhci_in16( base + SDHCI_HCTL2 ) & ~SDHCI_HCTL2_MODE_MSK;
    }

    switch( timing ) {
        case TIMING_HS400:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_HS400;
            ddr_mode = 1;
            break;

        case TIMING_HS200:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_HS200;
            break;

        case TIMING_SDR104:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_SDR104;
            break;

        case TIMING_SDR50:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_SDR50;
            break;

        case TIMING_SDR25:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_SDR25;
            break;

        case TIMING_SDR12:
            hctl |= SDHCI_HCTL_HSE;
            break;

        case TIMING_DDR50:
            hctl |= SDHCI_HCTL_HSE;
            hctl2 |= SDHCI_HCTL2_MODE_DDR50;
            ddr_mode = 1;
            break;

        case TIMING_HS:
            hctl |= SDHCI_HCTL_HSE;
            break;

        case TIMING_LS:
            break;

        default:
            break;
    }

    if( hc->version >= SDHCI_SPEC_VER_3 ) {
            // check for preset value enable and disble clock
        if( ( hctl2 & SDHCI_HCTL2_PRESET_VAL ) ) {
            sdhci_out32( base + SDHCI_SYSCTL, sdhci_in32( base + SDHCI_SYSCTL ) & ~SDHCI_SYSCTL_CEN );
        }

        sdhci_out32( base + SDHCI_HCTL, hctl );
        sdhci_out16( base + SDHCI_HCTL2, hctl2 );

        if( ( hctl2 & SDHCI_HCTL2_PRESET_VAL ) ) {
            hc->entry.clk( hc, hc->clk );
        }
    }
    else {
        sdhci_out32( base + SDHCI_HCTL, hctl );
    }

    if( dwchc->scr_base != 0 ) {
        out32( dwchc->scr_base, ( in32( dwchc->scr_base ) & ~( 1 << 0 ) ) | ( ddr_mode << 0 ) );
    }

    if( status == EOK ) {
        hc->timing = timing;
    }

    return( status );
}

static int sdhci_signal_voltage( sdio_hc_t *hc, int signal_voltage )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        sctl;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: signal_voltage %d", __FUNCTION__, signal_voltage );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;

    if( hc->version < SDHCI_SPEC_VER_3 ) {
        return( EOK );
    }

    if( !( hc->caps & ( HC_CAP_SV_3_0V | HC_CAP_SV_3_3V ) ) && signal_voltage == SIGNAL_VOLTAGE_3_3 ) {
        signal_voltage = SIGNAL_VOLTAGE_1_8;
    }

    if( hc->signal_voltage == signal_voltage ) {
        return( EOK );
    }

    if( signal_voltage == SIGNAL_VOLTAGE_3_3 ) {
        sdhci_out16( base + SDHCI_HCTL2, sdhci_in16( base + SDHCI_HCTL2 ) & ~SDHCI_HCTL2_SIG_1_8V );
        delay( 5 );     // wait for 3.3V regulator to stabilize
        if( ( sdhci_in16( base + SDHCI_HCTL2 ) & SDHCI_HCTL2_SIG_1_8V ) ) {
            sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: switch 3.3V failure", __FUNCTION__ );
            return( EIO );
        }
    }
    else if( signal_voltage == SIGNAL_VOLTAGE_1_8 ) {
            // disable clock
        sctl = sdhci_in32( base + SDHCI_SYSCTL );
        sdhci_out32( base + SDHCI_SYSCTL, sctl & ~SDHCI_SYSCTL_CEN );

            // Check whether DAT[3:0] is 0000
        if( !( sdhci_in32( base + SDHCI_PSTATE ) & SDHCI_PSTATE_DLSL_MSK ) || (hc->flags & HC_FLAG_DEV_MMC) ) {
            sdhci_out16( base + SDHCI_HCTL2, sdhci_in16( base + SDHCI_HCTL2 ) | SDHCI_HCTL2_SIG_1_8V );

            delay( 5 );

            if( ( sdhci_in16( base + SDHCI_HCTL2 ) & SDHCI_HCTL2_SIG_1_8V ) ) {
                sdhci_out32( base + SDHCI_SYSCTL, sctl );   // re-enable clock

                delay( 1 );

                    // If DAT[3:0] level is 1111b, then the switch succeeded
                if( ( ( sdhci_in32( base + SDHCI_PSTATE ) & SDHCI_PSTATE_DLSL_MSK ) == SDHCI_PSTATE_DLSL_MSK ) || (hc->flags & HC_FLAG_DEV_MMC) ) {
                    hc->signal_voltage = signal_voltage;
                    return( EOK );
                }
            }
        }

            // voltage switch failed, cycle power
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: switch 1.8V failure", __FUNCTION__ );

        sdhci_out32( base + SDHCI_HCTL, sdhci_in32( base + SDHCI_HCTL ) & ~SDHCI_HCTL_SDBP );
        delay( 1 );
        sdhci_out32( base + SDHCI_HCTL, sdhci_in32( base + SDHCI_HCTL ) | SDHCI_HCTL_SDBP );
        return( EIO );
    }

    hc->signal_voltage = signal_voltage;

    return( EOK );
}

static int sdhci_drv_type( sdio_hc_t *hc, int type )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint16_t        hctl2;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: type %d", __FUNCTION__, type );
#endif

    if( hc->version < SDHCI_SPEC_VER_3 ) {
        return( EOK );
    }

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    hctl2   = sdhci_in16( base + SDHCI_HCTL2 ) & ~SDHCI_HCTL2_DRV_TYPE_MSK;

    if( !( hctl2 & SDHCI_HCTL2_PRESET_VAL ) ) {
        switch( type ) {
            case DRV_TYPE_A:
                hctl2 |= SDHCI_HCTL2_DRV_TYPE_A; break;
            case DRV_TYPE_C:
                hctl2 |= SDHCI_HCTL2_DRV_TYPE_C; break;
            case DRV_TYPE_D:
                hctl2 |= SDHCI_HCTL2_DRV_TYPE_D; break;
            case DRV_TYPE_B:
            default:
                hctl2 |= SDHCI_HCTL2_DRV_TYPE_B; break;
                break;
        }

        sdhci_out16( base + SDHCI_HCTL2, hctl2 );
    }

    hc->drv_type = type;

    return( EOK );
}

static int sdhci_tune( sdio_hc_t *hc, int op )
{
    sdhci_hc_t          *sdhc;
    struct sdio_cmd     *cmd;
    uintptr_t           base;
    uint16_t            hctl2;
    int                 tlc;
    uint8_t             *rbuf;
    int                 tlen;
    int                 status;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: ", __FUNCTION__ );
#endif

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;
    tlen    = hc->bus_width == BUS_WIDTH_8 ? 128 : 64;
    rbuf    = alloca(tlen);

    if( hc->version < SDHCI_SPEC_VER_3 ) {
        return( EOK );
    }

    hctl2 = sdhci_in16( base + SDHCI_HCTL2 );

        // return if not HS200 or SDR104, and not SDR50 that requires tuning
    if( ( SDHCI_HCTL2_MODE( hctl2 ) != SDHCI_HCTL2_MODE_SDR104 ) &&
            ( SDHCI_HCTL2_MODE( hctl2 ) != SDHCI_HCTL2_MODE_HS200 ) &&
            ( ( SDHCI_HCTL2_MODE( hctl2 ) == SDHCI_HCTL2_MODE_SDR50 ) &&
            !( sdhc->flags & SF_TUNE_SDR50 ) ) ) {
        return( EOK );
    }

    if( ( cmd = sdio_alloc_cmd( ) ) == NULL ) {
        return( ENOMEM );
    }

    hctl2 |= SDHCI_HCTL2_EXEC_TUNING;

    sdhci_out16( base + SDHCI_HCTL2, hctl2 );

    for( tlc = SDHCI_TUNING_RETRIES; tlc; tlc-- ) {
        cmd->status = CS_CMD_INPROG;
        sdio_setup_cmd( cmd, SCF_CTYPE_ADTC | SCF_RSP_R1, op, 0 );
        sdio_setup_cmd_io( cmd, SCF_DIR_IN, 1, tlen, NULL, 0, NULL );

        if( ( status = sdio_issue_cmd( &hc->device, cmd, SDHCI_TUNING_TIMEOUT ) ) ) {
            break;
        }

        sdhci_in32s( rbuf, tlen >> 2, base + SDHCI_DATA );

        if( !( ( hctl2 = sdhci_in16( base + SDHCI_HCTL2 ) ) & SDHCI_HCTL2_EXEC_TUNING ) ) {
            break;
        }
    }

    if( status || ( hctl2 & SDHCI_HCTL2_EXEC_TUNING ) ) {
        hctl2 &= ~( SDHCI_HCTL2_TUNED_CLK | SDHCI_HCTL2_EXEC_TUNING );
        sdhci_out16( base + SDHCI_HCTL2, hctl2 );
        status = EIO;
    }

    sdio_free_cmd( cmd );

    return( status );
}

static int sdhci_cd( sdio_hc_t *hc )
{
    sdhci_hc_t      *sdhc;
    uintptr_t       base;
    uint32_t        pstate;
    int             cstate;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    base    = sdhc->base;

    if( ( hc->caps & HC_CAP_SLOT_TYPE_EMBEDDED ) ) {
        return( CD_INS );
    }

    cstate  = CD_RMV;
    pstate  = sdhci_in32( base + SDHCI_PSTATE );

    if( ( pstate & SDHCI_PSTATE_CI ) ) {
        if( sdhci_waitmask( hc, SDHCI_PSTATE, SDHCI_CARD_STABLE, SDHCI_CARD_STABLE, 100000 ) == EOK ) {
            cstate  |= CD_INS;
            if( !( pstate & SDHCI_PSTATE_WP ) ) {
                cstate  |= CD_WP;
            }
        }
    }

    return( cstate );
}

// dwc function
int sdhci_dwc_dinit( sdio_hc_t *hc )
{
    sdhci_hc_t  *sdhc;

    sdhc    = (sdhci_hc_t *)hc->cs_hdl;

    if( !sdhc ) {
        return EOK;
    }

    if( sdhc->base ) {
        sdhci_pwr( hc, 0 );
        sdhci_dwc_reset( hc, SDHCI_SYSCTL_SRA );
        if( hc->hc_iid != -1 ) {
            InterruptDetach( hc->hc_iid );
        }
        munmap_device_memory( (void *)sdhc->base, hc->cfg.base_addr_size[0] );
    }

    if( sdhc->adma ) {
        size_t  desc_sz;
        if( sdhc->flags & SF_USE_ADMA64 ) {
            desc_sz = SDHCI_ADMA2_64_DESC_SZ( sdhc );
        } else {
            desc_sz = sizeof( sdhci_adma32_t );
        }
        sdio_free( sdhc->adma, desc_sz * ADMA_DESC_MAX );
    }

    free( sdhc );

    return( EOK );
}

static int sdhci_dwc_hs_args( sdio_hc_t *hc )
{
    sdio_hc_cfg_t   *cfg;
    sdhci_hc_t      *sdhc;
    sdhci_dwc_t     *dwchc;
    int             opt;
    int             status = EOK;
    char            *value, *options;
    static char     *opts[] = {
#define DIS_64BIT   0
        "noadma64",
#define HS400ES     1
        "hs400es",
#define HS400       2
        "hs400",
#define HS200       3
        "hs200",
#define NOPHY       4
        "nophy",
        NULL
    };

    cfg     = &hc->cfg;
    options = cfg->hsoptions;
    sdhc    = (sdhci_hc_t *)hc->cs_hdl;
    dwchc   = (sdhci_dwc_t *)hc->cs_hdl;

    while( options && *options != '\0' ) {
        if( ( opt = sdio_hc_getsubopt( &options, opts, &value ) ) == -1 ) {
            sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 0, "%s: invalid HS options %s", __func__, options );
            continue;
        }

        switch (opt) {
            case DIS_64BIT:
                sdhc->flags |= SF_DIS_64BIT;
                break;
            case HS400ES:
                hc->caps    |= HC_CAP_HS400ES;
                break;
            case HS400:
                hc->caps    |= HC_CAP_HS400;
                break;
            case HS200:
                hc->caps    |= HC_CAP_HS200;
                break;
            case NOPHY:
                dwchc->phy = 0;
                break;
        }
    }

    return ( status );
}

// dwc function
int sdhci_dwc_init( sdio_hc_t *hc )
{
    sdio_hc_cfg_t       *cfg;
    sdhci_hc_t          *sdhc;
    sdhci_dwc_t         *dwchc;
    uint32_t            cap;
    uint32_t            cap2;
    uint32_t            mccap;
    uint32_t            cur;
    uintptr_t           base;
    struct sigevent     event;
    uint8_t             desc_sz;

#ifdef SDHCI_DWC_DEBUG
    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: ", __FUNCTION__ );
#endif

    hc->hc_iid          = -1;
    cfg                 = &hc->cfg;

    memcpy( &hc->entry, &sdhci_dwc_hc_entry, sizeof( sdio_hc_entry_t ) );

    if( !cfg->base_addr[0] ) {
        return( ENODEV );
    }

    if( !cfg->base_addr_size[0] ) {
        cfg->base_addr_size[0] = SDHCI_SIZE;
    }

    if( ( sdhc = hc->cs_hdl = calloc( 1, sizeof( sdhci_dwc_t ) ) ) == NULL ) {
        return( ENOMEM );
    }

    if( ( base = sdhc->base = (uintptr_t)mmap_device_memory( NULL, cfg->base_addr_size[0],
            PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED,
            cfg->base_addr[0] ) ) == (uintptr_t)MAP_FAILED ) {
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: DWC SDHCI base mmap_device_memory (0x%" PRIx64 "x) %s", __FUNCTION__, cfg->base_addr[0], strerror( errno ) );
        sdhci_dwc_dinit( hc );
        return( errno );
    }

    // Enable PHY by default
    dwchc = (sdhci_dwc_t *)hc->cs_hdl;
    dwchc->phy = 1;

    // Parse host specific options
    sdhci_dwc_hs_args( hc );

    if( !hc->version ) {
        hc->version = sdhci_in16( base + SDHCI_HCTL_VERSION ) & SDHCI_SPEC_VER_MSK;
    }

    cap         = sdhci_in32( base + SDHCI_CAP );
    cap2        = hc->version >= SDHCI_SPEC_VER_3 ? sdhci_in32( base + SDHCI_CAP2 ) : 0;
    mccap       = sdhci_in32( base + SDHCI_MCCAP );

    if (cfg->clk) {
        hc->clk_max = cfg->clk;
    } else {
        hc->clk_max = SDHCI_CAP_BASE_CLK( cap, hc->version );
    }

    hc->clk_min = hc->clk_max / SDHCI_CLK_MAX_DIV_SPEC_VER_2;

    if( hc->version >= SDHCI_SPEC_VER_3 ) {
        hc->clk_min = hc->clk_max / SDHCI_CLK_MAX_DIV_SPEC_VER_3;
        if (cfg->clk == 0) {
            sdhc->clk_mul = SDHCI_CAP_CLK_MULT( cap2 );
            if( sdhc->clk_mul ) {
                hc->clk_min = ( hc->clk_max * sdhc->clk_mul ) / 1024;
                hc->clk_max = hc->clk_max * sdhc->clk_mul;
            }
        }
    }

    hc->caps    |= HC_CAP_BSY | HC_CAP_BW4 | HC_CAP_BW8;
    hc->caps    |= HC_CAP_ACMD12 | HC_CAP_200MA | HC_CAP_DRV_TYPE_B;

    if( !( hc->caps & HC_CAP_SLOT_TYPE_EMBEDDED ) ) {
        hc->caps    |= HC_CAP_CD_INTR;
    }

    if( ( cap & SDHCI_CAP_HS ) ) {
        hc->caps    |= HC_CAP_HS;
    }

    if( ( cap & SDHCI_CAP_DMA ) ) {
        hc->caps    |= HC_CAP_DMA;
    }

    if( ( cap2 & SDHCI_CAP_SDR104 ) ) {
        hc->caps    |= HC_CAP_SDR104;
    }

    if( ( cap2 & SDHCI_CAP_SDR50 ) ) {
        hc->caps    |= HC_CAP_SDR50 | HC_CAP_SDR25 | HC_CAP_SDR12;
    }

    if( ( cap2 & SDHCI_CAP_DDR50 ) ) {
        hc->caps    |= HC_CAP_DDR50;
    }

    if( ( cap2 & SDHCI_CAP_TUNE_SDR50 ) ) {
        sdhc->flags |= SF_TUNE_SDR50;
    }

    if( ( cap2 & SDHCI_CAP_DRIVE_TYPE_A ) ) {
        hc->caps    |= HC_CAP_DRV_TYPE_A;
    }

    if( ( cap2 & SDHCI_CAP_DRIVE_TYPE_C ) ) {
        hc->caps    |= HC_CAP_DRV_TYPE_C;
    }

    if( ( cap2 & SDHCI_CAP_DRIVE_TYPE_D ) ) {
        hc->caps    |= HC_CAP_DRV_TYPE_D;
    }

    if( ( cap & SDHCI_CAP_S18 ) ) {
        hc->ocr         = OCR_VDD_17_195;
        cur             = SDHCI_MCCAP_180( mccap ) * SDHCI_MCCAP_MULT;

        if( cur >= 150 ) {
            hc->caps    |= HC_CAP_XPC_3_0V;
        }

        if( cur >= 800 ) {
            hc->caps    |= HC_CAP_800MA;
        } else if( cur >= 600 ) {
            hc->caps    |= HC_CAP_600MA;
        } else if( cur >= 400 ) {
            hc->caps    |= HC_CAP_400MA;
        } else {
            hc->caps    |= HC_CAP_200MA;
        }
    }

    if( ( cap & SDHCI_CAP_S30 ) ) {
        hc->ocr         = OCR_VDD_30_31 | OCR_VDD_29_30;
        if( ( SDHCI_MCCAP_300( mccap ) * SDHCI_MCCAP_MULT ) > 150 ) {
            hc->caps    |= HC_CAP_XPC_3_0V;
        }
    }

    if( ( cap & SDHCI_CAP_S33 ) ) {
        hc->ocr         = OCR_VDD_32_33 | OCR_VDD_33_34;
        if( ( SDHCI_MCCAP_330( mccap ) * SDHCI_MCCAP_MULT ) > 150 ) {
            hc->caps    |= HC_CAP_XPC_3_3V;
        }
    }

    if( SDHCI_CAP_TIMER_COUNT( cap2 ) ) {
        hc->tuning_count = 1 << ( SDHCI_CAP_TIMER_COUNT( cap2 ) - 1 );
    }

    sdhc->tuning_mode = SDHCI_CAP_TUNING_MODE( cap2 );

    hc->caps        |= HC_CAP_SV_3_3V;
    if( hc->version >= SDHCI_SPEC_VER_3 ) {
        hc->caps    |= HC_CAP_SV_1_8V;
    }

    if( ( cap & SDHCI_CAP_DMA ) ) {
        if( hc->version >= SDHCI_SPEC_VER_2 ) {
            desc_sz     = sizeof( sdhci_adma32_t );
            if( hc->version >= SDHCI_SPEC_VER_3 ) {
                if( hc->version >= SDHCI_SPEC_VER_4 ) {
                    sdhc->flags |= SF_V4_MODE;
                }
                if( ( cap & SDHCI_CAP_64BIT_ADDR ) && !( sdhc->flags & SF_DIS_64BIT ) ) {
                    sdhc->flags |= SF_USE_ADMA64;
                    desc_sz     = SDHCI_ADMA2_64_DESC_SZ( sdhc );
                }
                hc->caps    |= HC_CAP_ACMD23;
            }
            hc->cfg.sg_max  = ADMA_DESC_MAX;
            if( ( sdhc->adma = sdio_alloc( desc_sz * ADMA_DESC_MAX ) ) == NULL) {
                sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: ADMA mmap %s", __FUNCTION__, strerror( errno ) );
                sdhci_dwc_dinit( hc );
                return( errno );
            }
            sdhc->flags     |= SF_USE_ADMA;
            sdhc->admap     = sdio_vtop( sdhc->adma );
        }
        else {
            hc->cfg.sg_max  = 1;
            sdhc->flags     |= SF_USE_SDMA;
        }
    }

    hc->caps    &= cfg->caps;       // reconcile command line options

    SIGEV_PULSE_INIT( &event, hc->hc_coid, SDIO_PRIORITY, HC_EV_INTR, NULL );
    if( ( hc->hc_iid = InterruptAttachEvent( cfg->irq[0], &event, _NTO_INTR_FLAGS_TRK_MSK ) ) == -1 ) {
        sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, 1, 1, "%s: InterrruptAttachEvent (irq 0x%x) - %s", __FUNCTION__, cfg->irq[0], strerror( errno ) );
        sdhci_dwc_dinit( hc );
        return( errno );
    }

    sdhci_pwr( hc, 0 );
    return( EOK );
}

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/sdmmc/sdiodi/hc/sdhci_dwc.c $ $Rev: 945010 $")
#endif
