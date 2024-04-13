/*
* $QNXLicenseC:
* Copyright 2017, 2020 QNX Software Systems.
* Copyright 2017, Renesas Electronics Corporation
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
#include <unistd.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <internal.h>
#include <sys/syspage.h>
#include <inttypes.h>

#ifdef SDIO_HC_RCAR_SDMMC
#include <rcar_gen3.h>
#include <aarch64/r-car-gen3.h>
#define RCAR_SDMMC_SCC_DEBUG

const uint8_t m3w_v13_hs400_calib_code[2][CALIB_TABLE_MAX] = {
    { 3,  3,  3,  3,  3,  3,  3,  4,  4,  5,  6,  7,  8,  9, 10, 15, 16, 16, 16, 16, 16, 16, 17, 18, 18, 19, 20, 21, 22, 23, 24, 25 },
    { 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,  7,  8, 11, 12, 17, 18, 18, 18, 18, 18, 18, 18, 19, 20, 21, 22, 23, 25, 25 }
};

const uint8_t m3n_hs400_calib_code[2][CALIB_TABLE_MAX] =  {
    { 1,  2,  6,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 15, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 29, 30, 31 },
    { 2,  3,  4,  4,  5,  6,  7,  9, 10, 11, 12, 13, 14, 15, 16, 17, 17, 17, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30, 31, 31, 31, 31 }
};

const uint8_t e3_mmc_hs400_calib_code[CALIB_TABLE_MAX] =
    { 0,  0,  0,  1,  2,  3,  3,  4,  4,  4,  5,  5,  6,  8,  9, 10, 11, 12, 13, 15, 16, 17, 17, 18, 18, 19, 20, 22, 24, 25, 26, 26 };

extern const uint8_t sdio_tbp_4bit[];
extern const uint8_t sdio_tbp_8bit[];

static int rcar_sdmmc_reset(sdio_hc_t *hc);

static int rcar_sdmmc_dma_setup(sdio_hc_t *hc, sdio_cmd_t *cmd);
static void rcar_sdmmc_dma_start( sdio_hc_t *hc, sdio_cmd_t *cmd);
static void rcar_sdmmc_dma_cmplt( sdio_hc_t *hc, sdio_cmd_t *cmd, int error);
static void rcar_sdmmc_hs400_adjust_enable(sdio_hc_t *hc);
static void rcar_sdmmc_hs400_adjust_disable(sdio_hc_t *hc);
static void rcar_sdmmc_scc_hs400_reset(sdio_hc_t *hc);
static int rcar_sdmmc_scc_error_check(sdio_hc_t *hc, uint32_t info2);
static void rcar_sdmmc_disable_scc(sdio_hc_t *hc);
static void rcar_sdmmc_scc_reset(sdio_hc_t *hc);

static int rcar_sdmmc_intr_event(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc;
    sdio_cmd_t      *cmd;
    uint32_t        mask1, mask2;
    uint32_t        stat1, stat2, dmasta;
    int             cs = CS_CMD_INPROG;

    sdmmc  = (rcar_sdmmc_t *)hc->cs_hdl;

    mask1 = ~sdmmc_read(sdmmc->vbase, MMC_SD_INFO1_MASK);
    mask2 = ~sdmmc_read(sdmmc->vbase, MMC_SD_INFO2_MASK);
    stat1 =  sdmmc_read(sdmmc->vbase, MMC_SD_INFO1) & mask1;
    stat2 =  sdmmc_read(sdmmc->vbase, MMC_SD_INFO2) & mask2;
    dmasta =  sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1);

    /* Clear interrupt status */
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO1, ~stat1);
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO2, ~stat2 | SDH_INFO2_BMSK);

    /*
     * Card insertion and card removal events
     */
    if (stat1 & (SDH_INFO1_INST | SDH_INFO1_RMVL)) {
        sdio_hc_event(hc, HC_EV_CD);
    }

    /* no command ? */
    if ((cmd = hc->wspc.cmd) == NULL)
        return (EOK);

    /* Start DMA ? */
    if ((stat1 & SDH_INFO1_RE) && !((stat2 & SDH_INFO2_CRCE && cmd->flags & SCF_RSP_CRC) ||
                                     stat2 & SDH_INFO2_CMDE ||
                                     stat2 & SDH_INFO2_ENDE ||
                                     stat2 & SDH_INFO2_RTO)) {
        if (cmd->flags & SCF_CTYPE_ADTC) {
            rcar_sdmmc_dma_start(hc, cmd);
        }
    }

    /* Check of errors */
    if (stat2 & (SDH_INFO2_ALL_ERR)) {
        /* Description of bit7 of SD_INFO2 register is saying:
         * If the data timeout (ERR3) is set but the response timeout (ERR6) is not set after the Erase command has been issued,
         * the end of the Erase sequence (DAT0 = 1) is confirmed by polling DAT0 */
        if ((cmd->opcode == MMC_ERASE) && ((stat2 & (SDH_INFO2_DTO | SDH_INFO2_RTO)) == SDH_INFO2_DTO)) {
            int timeout = 10*1000*1000; /* wait up to 10s for DAT0=1, the timeout value can depend on eMMC device */
            while (timeout-- > 0) {
                if(sdmmc_read(sdmmc->vbase, MMC_SD_INFO2) & SDH_INFO2_DAT0) {
                    cs = CS_CMD_CMP;
                    stat2 &= ~SDH_INFO2_DTO;
                    break;
                }
                nanospin_ns(1000);
            }
        }

        if (cs == CS_CMD_INPROG) {
            uint16_t    ests1, ests2;

            /* DMA error processing? */
            if ((sdmmc->flags & OF_DMA_ACTIVE) && (cmd->flags & SCF_CTYPE_ADTC)) {
                uint32_t      dma_info1, dma_info2;

                dma_info1 = sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1);
                dma_info2 = sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO2);

                if(dma_info2 & (DM_INFO2_DTRAN_ERR0 | DM_INFO2_DTRAN_ERR1)) {
                    sdio_slogf( _SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1, "%s: ERROR in SDHI%d, CMD %d, DMA info1 0x%X, DMA info2 0x%X",
                                        __func__, sdmmc->chan_idx, cmd->opcode, dma_info1, dma_info2 );

                    rcar_sdmmc_dma_cmplt(hc, cmd, 1);
                }
            }

            ests1 = sdmmc_read(sdmmc->vbase, MMC_SD_ERR_STS1);
            ests2 = sdmmc_read(sdmmc->vbase, MMC_SD_ERR_STS2);

            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
                            "%s: ERROR in SDHI%d, CMD %d, INFO 0x%x-0x%x, STS 0x%x-0x%x",
                            __func__, sdmmc->chan_idx, cmd->opcode, stat1, stat2,
                            sdmmc_read(sdmmc->vbase, MMC_SD_ERR_STS1), sdmmc_read(sdmmc->vbase, MMC_SD_ERR_STS2));

            if (stat2 & SDH_INFO2_DTO)          cs = CS_DATA_TO_ERR;
            if (ests1 & (1 << 11))              cs = CS_DATA_CRC_ERR;
            if (ests1 & (1 << 10))              cs = CS_DATA_CRC_ERR;
            if (ests1 & ((1 << 8) | (1 << 9)))  cs = CS_CMD_CRC_ERR;
            if (ests1 & (1 << 5))               cs = CS_CMD_END_ERR;
            if (ests1 & (1 << 4))               cs = CS_DATA_END_ERR;
            if (ests1 & ((1 << 1) | (1 << 0)))  cs = CS_CMD_IDX_ERR;
            if (ests2 & (1 << 5))               cs = CS_DATA_CRC_ERR;
            if (ests2 & ((1 << 5) | (1 << 4)))  cs = CS_DATA_TO_ERR;
            if (ests2 & ((1 << 1) | (1 << 0)))  cs = CS_CMD_TO_ERR;
            if (!cs)                            cs = CS_CMD_CMP_ERR;
        }
    } else {
        /* End of command */
        if (stat1 & SDH_INFO1_RE) {
            if (!(cmd->flags & SCF_CTYPE_ADTC)){
                cs = CS_CMD_CMP;

                if ((cmd->flags & SCF_RSP_136)) {
                    uint32_t    *resp = &cmd->rsp[0];
                        resp[0] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP76);
                        resp[1] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP54);
                        resp[2] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP32);
                        resp[3] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP10);

                        resp[0] = (resp[0] << 8) | (resp[1] >> 24);
                        resp[1] = (resp[1] << 8) | (resp[2] >> 24);
                        resp[2] = (resp[2] << 8) | (resp[3] >> 24);
                        resp[3] = (resp[3] << 8);
                } else if ((cmd->flags & SCF_RSP_PRESENT)) {
                    cmd->rsp[0] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP10);
                }
            }
        } else if (stat1 & SDH_INFO1_AE) {
            /* End of data transfer */
            cmd->rsp[0] = sdmmc_read(sdmmc->vbase, MMC_SD_RSP10);
            sdmmc->flags |= OF_DATA_DONE;
            if((sdmmc->flags & (OF_DATA_DONE | OF_DMA_DONE))==(OF_DATA_DONE | OF_DMA_DONE))
                cs = CS_CMD_CMP;
        }
    }

    if (sdmmc->flags & OF_DMA_ACTIVE) {
        if (dmasta & ( (cmd->flags & SCF_DIR_IN) ? DM_INFO1_DTRAN_END1(sdmmc->dma_tranend1) : DM_INFO1_DTRAN_END0)) {
            /* Clear DMA status */
            sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1, 0);

            sdmmc->flags |= OF_DMA_DONE;
            if((sdmmc->flags & (OF_DATA_DONE | OF_DMA_DONE))==(OF_DATA_DONE | OF_DMA_DONE)) {
                cs = CS_CMD_CMP;
            }
        }
    }

    if (stat1 & SDH_INFO1_RMVL) {
        cs = CS_CARD_REMOVED;
    }

    if (cs != CS_CMD_INPROG) {
		/* Command operation completion: Disable interrupts */
        sdmmc_write(sdmmc->vbase, MMC_SD_INFO1_MASK, ~(SDH_INFO1_RMVL | SDH_INFO1_INST));
        sdmmc_write(sdmmc->vbase, MMC_SD_INFO2_MASK, sdmmc_read(sdmmc->vbase, MMC_SD_INFO2_MASK) | SDH_INFO2_ALL_ERR);

        if ((sdmmc->flags & OF_DMA_ACTIVE) && (cmd->flags & SCF_CTYPE_ADTC)) {
            rcar_sdmmc_dma_cmplt(hc, cmd, cs != CS_CMD_CMP);
        }

        /*
         * R-Car3 WA for OPC #109: adjust hs400 offset for R-Car M3N, M3W1.3, E3
         * See the following URL for WA code: http://community.qnx.com/sf/go/projects.renesas_drivers/frs.diam_tyaa.r_car_m3n_emmc_driver_release_wi
         */
        if (sdmmc->need_adjust_hs400 && cmd->opcode == MMC_SEND_STATUS) {
            if (cmd->rsp[0] & CDS_READY_FOR_DATA) {
                rcar_sdmmc_hs400_adjust_enable(hc);
            }
        }

        /* Check SCC status */
        if (rcar_sdmmc_scc_error_check(hc, stat2)) {
            rcar_sdmmc_scc_reset(hc);

            /* This is to perform retune */
            sdio_hc_event(hc, HC_EV_TUNE);
        }

        sdio_cmd_cmplt(hc, cmd, cs);
    }

    return (EOK);
}

static int rcar_sdmmc_dma_setup(sdio_hc_t *hc, sdio_cmd_t *cmd)
{
    rcar_sdmmc_t    *sdmmc;
    sdio_sge_t      *sgp;
    int              sgc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    sgc = cmd->sgc;
    sgp = cmd->sgl;

    if (!(cmd->flags & SCF_DATA_PHYS)) {
        sdio_vtop_sg(sgp, sdmmc->sgl, sgc, cmd->mhdl);
        sgp = sdmmc->sgl;
    }

    /* Enable read/write by DMA */
    sdmmc_write(sdmmc->vbase, MMC_CC_EXT_MODE, BUF_ACC_DMAWEN | CC_EXT_MODE_MSK);

    /* Set the address mode */
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_DTRAN_MODE, ((cmd->flags & SCF_DIR_IN) ? CH_NUM_UPSTREAM : CH_NUM_DOWNSTREAM)
                        | BUS_WID_64BIT | INCREMENT_ADDRESS);

    /* Set the SDMA address */
    if( (sgp->sg_address & 0x7F) ) {
        /* The DMA has an 128 byte alignment requirement */
        sdmmc_write(sdmmc->vbase, MMC_CC_EXT_MODE, CC_EXT_MODE_MSK);

        return( EINVAL );
    }

    sdmmc_write(sdmmc->vbase, MMC_DM_CM_DTRAN_ADDR, sgp->sg_address);

    sdmmc->flags = 0;

    return (EOK);
}

static void rcar_sdmmc_dma_start( sdio_hc_t *hc, sdio_cmd_t *cmd )
{
    rcar_sdmmc_t *sdmmc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if(sdmmc->flags & OF_DMA_ACTIVE)
        return;

    /* Clear DMA status */
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1, 0);
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO2, 0);

    sdmmc->flags = OF_DMA_ACTIVE;

    /* Enable DMA interrupt */
    if (cmd->flags & SCF_DIR_IN)
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1_MASK, sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1_MASK) & ~DM_INFO1_DTRAN_END1(sdmmc->dma_tranend1));
    else
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1_MASK, sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1_MASK) & ~DM_INFO1_DTRAN_END0);

    /* Start DMA */
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_DTRAN_CTRL, DM_START);
}

static void rcar_sdmmc_dma_cmplt( sdio_hc_t *hc, sdio_cmd_t *cmd, int error)
{
    rcar_sdmmc_t  *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    /* Disable DMA interrupt */
    if (cmd->flags & SCF_DIR_IN) {
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1_MASK, sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1_MASK) | DM_INFO1_DTRAN_END1(sdmmc->dma_tranend1));
    } else {
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1_MASK, sdmmc_read(sdmmc->vbase, MMC_DM_CM_INFO1_MASK) | DM_INFO1_DTRAN_END0);
    }

    /* Reset DMA channels if error occurs */
    if(error) {
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_RST, DM_RST_REVBITS_MSK & ~(DM_RST_DTRANRST1 | DM_RST_DTRANRST0));
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_RST, DM_RST_REVBITS_MSK |  (DM_RST_DTRANRST1 | DM_RST_DTRANRST0));
    }

    /* Clear DMA Status */
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1, 0);
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO2, 0);

    /* The SD_BUF read/write DMA transfer is disabled */
    sdmmc_write(sdmmc->vbase, MMC_CC_EXT_MODE, CC_EXT_MODE_MSK);

    sdmmc->flags &= ~OF_DMA_ACTIVE;
}

static int rcar_sdmmc_event(sdio_hc_t *hc, sdio_event_t *ev)
{
    rcar_sdmmc_t    *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    int             status = CS_CMD_INPROG;

    switch (ev->code) {
        case HC_EV_INTR:
            status = rcar_sdmmc_intr_event(hc);
            InterruptUnmask(sdmmc->irq, hc->hc_iid);
            break;
        default:
            break;
    }

    return (status);
}

static int rcar_sdmmc_check_idle(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc;
    uint32_t        status;
    int             i;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    for (i = 0; i < SDHI_TMOUT; i++) {
        status =  sdmmc_read(sdmmc->vbase, MMC_SD_INFO2);
        if (!(status & SDH_INFO2_CBSY) && (status & SDH_INFO2_SCLKDIVEN))
            break;
        nanospin_ns(1000);
    }

    if (i >= SDHI_TMOUT) {
        return (EBUSY);
    }

    return EOK;
}

static int rcar_sdmmc_xfer_setup(sdio_hc_t *hc, sdio_cmd_t *cmd)
{
    rcar_sdmmc_t    *sdmmc;
    int             status = EOK;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (rcar_sdmmc_check_idle(hc)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, BUSY status is present in SD_INFO2 register, unable to make a transfer setup", __func__, sdmmc->chan_idx);
        return (EAGAIN);
    }

    sdio_sg_start(hc, cmd->sgl, cmd->sgc);

    if((cmd->blksz < 2) || (cmd->blksz > 512)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: Block size (%d) is not in range of 2 to 512", __func__, cmd->blksz);
        return (EINVAL);
    }

    /* Set block size */
    sdmmc_write(sdmmc->vbase, MMC_SD_SIZE, cmd->blksz);

    /* Set block count (multi-block transfers) */
    if (cmd->blks > 1) {
        sdmmc_write(sdmmc->vbase, MMC_SD_SECCNT, cmd->blks);
    }

    sdmmc->cmd = cmd;

    if (cmd->sgc && (hc->caps & HC_CAP_DMA)) {
        if ((status = rcar_sdmmc_dma_setup(hc, cmd)) == EOK) {
        }
    }

    return (status);
}

static int rcar_sdmmc_cmd(sdio_hc_t *hc, sdio_cmd_t *cmd)
{
    rcar_sdmmc_t  *sdmmc;
    int           status;
    uint32_t      command;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (rcar_sdmmc_check_idle(hc)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, BUSY status is present in SD_INFO2 register, unable to issue CMD %d", __func__, sdmmc->chan_idx, cmd->opcode);
        return (EAGAIN);
    }

    /* Clear Status */
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO1, sdmmc_read(sdmmc->vbase, MMC_SD_INFO1) & ~(SDH_INFO1_AE | SDH_INFO1_RE));
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO2, SDH_INFO2_BMSK);

    command = cmd->opcode;
    switch (cmd->flags & (0x1F << 4)) {
        case SCF_RSP_NONE:
            command |= SDH_CMD_NORSP;
            break;
        case SCF_RSP_R1:
            command |= SDH_CMD_RSPR1;
            break;
        case SCF_RSP_R1B:
            command |= SDH_CMD_RSPR1B;
            break;
        case SCF_RSP_R2:
            command |= SDH_CMD_RSPR2;
            break;
        case SCF_RSP_R3:
            command |= SDH_CMD_RSPR3;
            break;
    }

    sdmmc->flags = 0;

    if (cmd->flags & SCF_CTYPE_ADTC) {
        command |= SDH_CMD_ADTC;
        if (cmd->flags & SCF_DIR_IN)
            command |= SDH_CMD_DAT_READ;
        if (cmd->flags & SCF_MULTIBLK) {
              sdmmc_write(sdmmc->vbase, MMC_SD_STOP, SDH_STOP_SEC);
            command |= SDH_CMD_DAT_MULTI;
            if (!(hc->caps & HC_CAP_ACMD12))
                command |= SDH_CMD_NOAC12;
        } else
            sdmmc_write(sdmmc->vbase, MMC_SD_STOP, 0);

        if ((status = rcar_sdmmc_xfer_setup(hc, cmd)) != EOK)
            return (status);

        /* card insertion/removal are always enabled */
        sdmmc_write(sdmmc->vbase, MMC_SD_INFO1_MASK, ~(SDH_INFO1_AE | SDH_INFO1_RE | SDH_INFO1_RMVL | SDH_INFO1_INST));
    } else {
        sdmmc_write(sdmmc->vbase, MMC_SD_INFO1_MASK, ~(SDH_INFO1_RE | SDH_INFO1_RMVL | SDH_INFO1_INST));
    }

    sdmmc_write(sdmmc->vbase, MMC_SD_INFO2_MASK, ~SDH_INFO2_ALL_ERR);

    sdmmc_write(sdmmc->vbase, MMC_SD_ARG, cmd->arg);

    sdmmc_write(sdmmc->vbase, MMC_SD_CMD, command);

    return (EOK);
}

static int rcar_sdmmc_abort(sdio_hc_t *hc, sdio_cmd_t *cmd)
{
    return (EOK);
}

static int rcar_sdmmc_pwr(sdio_hc_t *hc, int vdd)
{
    rcar_sdmmc_t *sdmmc;
    int timeout = 1000;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if(vdd == 0) {
        sdmmc_write(sdmmc->vbase, MMC_SOFT_RST, sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) & ~SOFT_RST_OFF);
        delay(1);
        sdmmc_write(sdmmc->vbase, MMC_SOFT_RST, sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) | SOFT_RST_OFF);
        /* Check reset release state */
        while((!(sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) & SOFT_RST_OFF)) && (timeout-- > 0)) {
            nanospin_ns(1000);
        }

        /* Reset DMA channels */
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_RST, DM_RST_REVBITS_MSK & ~(DM_RST_DTRANRST1 | DM_RST_DTRANRST0));
        sdmmc_write(sdmmc->vbase, MMC_DM_CM_RST, DM_RST_REVBITS_MSK |  (DM_RST_DTRANRST1 | DM_RST_DTRANRST0));

        /* Stop clock */
        sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & ~SDH_CLKCTRL_SCLKEN);

        /* Disable SCC */
        sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, ~MMC_SCC_CKSEL_DTSEL & sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
        sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, ~MMC_SCC_DTCNTL_TAPEN & sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));
        sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN & sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
        sdmmc_write(sdmmc->vbase, MMC_SCC_DT2FF, sdmmc->tap);

        if (hc->flags & HC_FLAG_DEV_MMC) {
            rcar_sdmmc_scc_hs400_reset(hc);
        }

        /* Start clock */
        sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);

        sdmmc->tuning_status = RCAR_TUNING_NONE;
    }

    hc->vdd = vdd;

    return (EOK);
}

static int rcar_sdmmc_bus_mode(sdio_hc_t *hc, int bus_mode)
{
    hc->bus_mode = bus_mode;

    return (EOK);
}

static int rcar_sdmmc_bus_width(sdio_hc_t *hc, int width)
{
    rcar_sdmmc_t *sdmmc;
    uint32_t    hctl;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

     if (rcar_sdmmc_check_idle(hc)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, BUSY status is present in SD_INFO2 register, unable to set bus width", __func__, sdmmc->chan_idx);
        return (EAGAIN);
    }

    hctl = sdmmc_read(sdmmc->vbase, MMC_SD_OPTION) & ~(SDH_OPTION_WIDTH_1 | SDH_OPTION_WIDTH_8);

    if (width == 8) {
        hctl |=  (SDH_OPTION_WIDTH_8);
    }
    else if (width == 1) {
        hctl |=  (SDH_OPTION_WIDTH_1);
    }

    sdmmc_write(sdmmc->vbase, MMC_SD_OPTION, hctl);

    hc->bus_width = width;

    return (EOK);
}

static uint8_t clock_div(int hclk, int *clock, int timing)
{
    uint32_t clk;
    int      new_clock;

    if (*clock >= hclk) {
        /*  The output clock is the 1/2 frequency mode */
        if (timing == TIMING_HS400) {
            clk = 0;
        } else {
            clk = 0xFF;
        }
    }
    else {
        for (new_clock = hclk/512, clk = 0x80; *clock >= (new_clock * 2); clk >>= 1)
            new_clock <<= 1;

        *clock = new_clock;
    }

    return (uint8_t)clk;
}

static int rcar_sdmmc_clk(sdio_hc_t *hc, int clk)
{
    rcar_sdmmc_t  *sdmmc;
    uint32_t      clkctl;
    int           clock;
    uint32_t      clkrate_val;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (rcar_sdmmc_check_idle(hc)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, BUSY status is present in SD_INFO2 register, unable to change clock", __func__, sdmmc->chan_idx);
        return (EAGAIN);
    }

    /*
     * In R-Car Gen3 SDHI, internal SCC modules uses SDnH clock.
     * It is controlled by SDnCKCR.STPnHCK bit in CPG.
     * When SD clock is less than High Speed, SDnH clock is stopped.
     * And SDnH clock is supplied with 100MHz or more in Clock divider
     * table of CPG in R-Car Gen3.
     * It is the recommended setting of H/W.
     */
    if ( (hc->timing == TIMING_SDR104 || hc->timing == TIMING_HS200 || hc->timing == TIMING_HS400) &&
        (clk < sdmmc->scc_min_clk) )
        clk = sdmmc->scc_min_clk;

    /* Stop clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & ~SDH_CLKCTRL_SCLKEN);

    /* Set CPG SDCKCR register:
     * - For the hosts using 4TAP, we need to change SDCKCR register from 1 to 4 in HS400 mode based on HW manual.
     * - For E3, we found out write corruption may occur sometime if we don't lower down more clock in setting device to HS400 mode, see below comment.
     * - It doesn't need for other hosts. */
    if ((hc->flags & HC_FLAG_DEV_MMC) && (hc->caps & HC_CAP_HS400)) {
        if (hc->timing == TIMING_HS400) {
            /* Change SDH clock from 800MHz to 400MHz for the hosts using 4TAP */
            clkrate_val = sdmmc->sdckcr_val_for_hs400;
        } else if ((hc->timing == TIMING_LS) && (sdmmc->tuning_status == RCAR_TUNING_DONE)) {
            /* FIXME : Set to this value to switch eMMC chip to HS400 mode,
             * if not this setting, the write corruption may occur sometime on E3 */
            clkrate_val = 5;
        } else {
            /* By default: SDH clock = 800MHz, SD clock = 200MHz */
            clkrate_val = 1;
        }
        if (sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
            if (in32(sdmmc->vcpg_base + sdmmc->clkrate_offset) != clkrate_val) {
                out32(sdmmc->vcpg_base + RCAR_GEN3_CPG_CPGWPR, ~clkrate_val);
                out32(sdmmc->vcpg_base + sdmmc->clkrate_offset, clkrate_val);
            }
        }
    }

    if (clk > hc->clk_max) {
        clk = hc->clk_max;
    }

    clock = clk;

	clkctl  = sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL);
    clkctl &= 0xFFFFFF00;
    clkctl |= clock_div(sdmmc->pclk, &clock, hc->timing);

    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, clkctl);

     /* Start clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);

    hc->clk = sdmmc->busclk = clock;

    return (EOK);
}

static int rcar_sdmmc_signal_voltage(sdio_hc_t *hc, int signal_voltage)
{
    return (EOK);
}

static int rcar_sdmmc_timing(sdio_hc_t *hc, int timing)
{
    rcar_sdmmc_t  *sdmmc;
    uint32_t      new_tap;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    hc->timing = timing;

    rcar_sdmmc_clk(hc, hc->clk);

    if (rcar_sdmmc_check_idle(hc)) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, BUSY status is present in SD_INFO2 register, unable to change timing", __func__, sdmmc->chan_idx);
        return (EAGAIN);
    }

    /* Stop clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & ~SDH_CLKCTRL_SCLKEN);

    if (timing == TIMING_HS400 ) {
        /* Set HS400 mode */
        sdmmc_write(sdmmc->vbase, MMC_SDIF_MODE, sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE) | SDIF_MODE_HS400);

        sdmmc_write(sdmmc->vbase, MMC_SCC_DT2FF, sdmmc->tap_hs400);

        if(sdmmc->hs400_manual_correction) {
            /* Disable SCC sampling clock position correction */
            sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN &
                sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
        }

        sdmmc_write(sdmmc->vbase, MMC_SCC_TMPPORT2,  sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2) |
                MMC_SCC_TMPPORT2_HS400OSEL | MMC_SCC_TMPPORT2_HS400EN);

        sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, MMC_SCC_DTCNTL_TAPEN |
                sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));

        /* Avoid bad TAP */
        if (sdmmc->hs400_bad_tap & (1 << sdmmc->tap_set)) {
            new_tap = (sdmmc->tap_set + sdmmc->tap_num + 1) % sdmmc->tap_num;

            if (sdmmc->hs400_bad_tap & (1 << new_tap)) {
                new_tap = (sdmmc->tap_set + sdmmc->tap_num - 1) % sdmmc->tap_num;
            }

            if (sdmmc->hs400_bad_tap & (1 << new_tap)) {
                sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1,
                    "%s: Three consecutive bad tap is prohibited, TAPSET cannot change from %d to %d", __func__, sdmmc->tap_set, new_tap);
                new_tap = sdmmc->tap_set;
            }

            sdmmc->tap_set = new_tap;
            sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, sdmmc->tap_set);
        }

        /* Replace the tuning result of 8TAP with 4TAP */
        if (sdmmc->hs400_use_4tap) {
            sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, sdmmc->tap_set / 2);
        }

        /*  The output clock is the 1/2 frequency mode and enable clock */
        sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL) | MMC_SCC_CKSEL_DTSEL);

        /* Apply HS400 SW work around */
        if (sdmmc->adjust_hs400_enable) {
            /* Set this flag to execute adjust hs400 offset after setting to HS400 mode and after CMD13 */
            sdmmc->need_adjust_hs400 = 1;
        }
    } else {
        rcar_sdmmc_disable_scc(hc);

        /* Reset HS400 mode */
        if (hc->flags & HC_FLAG_DEV_MMC) {
            rcar_sdmmc_scc_hs400_reset(hc);
        }
    }

    /* Start clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);

#ifdef RCAR_SDMMC_SCC_DEBUG
    if (timing == TIMING_HS400 ) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   *************   HS400 Tuning   ********    ");
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TAPSET                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TAPSET));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DTCNTL                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_RVSCNTL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SD_CLK_CTRL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DT2FF                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DT2FF));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SDIF_MODE                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT2                 = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2));
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_CKSEL                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
        if (sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   RCAR_CPG_SDnCKCR                 = 0x%08X :", sdmmc_read(sdmmc->vcpg_base, sdmmc->clkrate_offset));
        }
    }
#endif
    return (EOK);
}

static int rcar_sdmmc_select_tuning(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;
    int      tapcnt;    /* Count 'OK' TAP */
    int      tapset;    /* Tap position */
    int      tapstart;  /* Start position of OK' TAP */
    int      tapend;    /* End position of OK' TAP */
    int      ntap;      /* Number of 'OK' TAP */
    int      matchcnt;  /* Count of matching data */
    int      i;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    /* Clear adjust hs400 flag */
    sdmmc->need_adjust_hs400 = 0;
    sdmmc->tuning_status = RCAR_TUNING_DONE;

    /* Clear SCC_RVSREQ */
    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSREQ, 0);

    /* If TAP is 'NG', force its repeated display index to 'NG' */
    for (i = 0; i < sdmmc->tap_num * 2; i++) {
        if (!sdmmc->taps[i]) {
            sdmmc->taps[i % sdmmc->tap_num] = 0;
            sdmmc->taps[(i % sdmmc->tap_num) + sdmmc->tap_num] = 0;
        }
        if (!sdmmc->smpcmp[i]) {
            sdmmc->smpcmp[i % sdmmc->tap_num] = 0;
            sdmmc->smpcmp[(i % sdmmc->tap_num) + sdmmc->tap_num] = 0;
        }
    }

    /* Find largest range of TAPs are 'OK'. The sampling clock position = (start position + end position) / 2 */
    tapset   = -1;
    tapcnt   = 0;
    ntap     = 0;
    tapstart = 0;
    tapend   = 0;
    for (i = 0; i < sdmmc->tap_num * 2; i++) {
        if (sdmmc->taps[i]) {
            ntap++;
        } else {
            if (ntap > tapcnt) {
                tapstart = i - ntap;
                tapend   = i - 1;
                tapcnt   = ntap;
            }
            ntap = 0;
        }
    }

    if (ntap > tapcnt) {
        tapstart = i - ntap;
        tapend   = i - 1;
        tapcnt   = ntap;
    }

    if((tapcnt < sdmmc->tap_num * 2) && (tapcnt >= RCAR_SDHI_MAX_TAP)) {
        tapset = ((tapstart + tapend) / 2) % sdmmc->tap_num;
    }
    else if (tapcnt == sdmmc->tap_num * 2) {
        /* If all TAP is 'OK', the sampling clock position is selected by
         * identifying the change point of data */
        matchcnt = 0;
        ntap     = 0;
        tapstart = 0;
        tapend   = 0;
        for (i = 0; i < sdmmc->tap_num * 2; i++) {
            if (sdmmc->smpcmp[i]) {
                ntap++;
            } else {
                if (ntap > matchcnt) {
                    tapstart = i - ntap;
                    tapend   = i - 1;
                    matchcnt = ntap;
                }
                ntap = 0;
            }
        }
        if (ntap > matchcnt) {
            tapstart = i - ntap;
            tapend   = i - 1;
            matchcnt = ntap;
        }
        if (matchcnt)
            tapset = ((tapstart + tapend) / 2) % sdmmc->tap_num;
    }

    /* Tuning is failed */
    if (tapset == -1) {
      return (EIO);
    }

    /* Set TAPSET */
    sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, tapset);

    /* Save for HS400 case */
    sdmmc->tap_set = tapset;

    /* Enable auto-retuning */
    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, MMC_SCC_RVSCNTL_RVSEN | sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));

#ifdef RCAR_SDMMC_SCC_DEBUG
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   ************* Select HS200 Tuning *********  ");
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TAPSET                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TAPSET));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DTCNTL                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_RVSCNTL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SD_CLK_CTRL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DT2FF                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DT2FF));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_CKSEL                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SDIF_MODE                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT2                 = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2));
    if (sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   RCAR_CPG_SDnCKCR                 = 0x%08X :", sdmmc_read(sdmmc->vcpg_base, sdmmc->clkrate_offset));
    }
#endif

    return (EOK);
}

static void rcar_sdmmc_scc_reset(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    /* Stop clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & ~SDH_CLKCTRL_SCLKEN);

    /* Reset SCC */
    sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, ~MMC_SCC_CKSEL_DTSEL & sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, ~MMC_SCC_DTCNTL_TAPEN & sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));

    if (hc->flags & HC_FLAG_DEV_MMC)
        rcar_sdmmc_scc_hs400_reset(hc);

    /* Start clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);

    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN & sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN & sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));

    sdmmc->tuning_status = RCAR_TUNING_NONE;
}

static void rcar_sdmmc_disable_scc(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (hc->timing == TIMING_SDR104 ||
        hc->timing == TIMING_HS200 ||
        hc->timing == TIMING_HS400)
        return;

    sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, ~MMC_SCC_CKSEL_DTSEL & sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, ~MMC_SCC_DTCNTL_TAPEN & sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));
}


static int rcar_sdmmc_tune(sdio_hc_t *hc, int op)
{
    rcar_sdmmc_t    *sdmmc;
    struct sdio_cmd *cmd;
    sdio_sge_t      sge;
    uint8_t         *td;
    const uint8_t   *sdio_tbp;
    int             tlen;
    int             status;
    int             i;
    int             tapnum;

    /* return if not HS200 or SDR104 that requires tuning */
    if ((hc->timing != TIMING_SDR104) && (hc->timing != TIMING_HS200)) {
        return (EOK);
    }

    sdmmc   = (rcar_sdmmc_t *)hc->cs_hdl;

    tapnum  = 8;
    if (hc->bus_width == 8) {
        tlen = 128;
        sdio_tbp = sdio_tbp_8bit;
    }
    else {
        tlen = 64;
        sdio_tbp = sdio_tbp_4bit;
    }

    /* Buffer of tuning command */
    if ((cmd = sdio_alloc_cmd()) == NULL) {
        return (ENOMEM);
    }

    /* Buffer for reading tuning data */
    if ((td = (uint8_t *)sdio_alloc(tlen)) == NULL) {
        sdio_free_cmd(cmd);
        return (ENOMEM);
    }

    sdmmc->tuning_status = RCAR_TUNING_DOING;

    memset(sdmmc->taps, 0, RCAR_SDHI_TUNING_RETRIES);   // Initialize all tap  to 'NG'
    memset(sdmmc->smpcmp, 0, RCAR_SDHI_TUNING_RETRIES); // Initialize all smpcmp to 'NG'

    /* Clear SD flag status */
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO1, 0);
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO2, SDH_INFO2_BMSK);

    /* Stop clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) & ~SDH_CLKCTRL_SCLKEN);

    /* Initialize SCC */
    sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, MMC_SCC_DTCNTL_TAPEN | (tapnum << 16));
    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN & sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_DT2FF, sdmmc->tap);
    sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, MMC_SCC_CKSEL_DTSEL | sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));

    /* Start clock */
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL) | SDH_CLKCTRL_SCLKEN);

#ifdef RCAR_SDMMC_SCC_DEBUG
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   *************  HS200 tuning *********    ");
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TAPSET                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TAPSET));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DTCNTL                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_RVSCNTL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SD_CLK_CTRL                  = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SD_CLK_CTRL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_DT2FF                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_DT2FF));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_CKSEL                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SDIF_MODE                    = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT2                 = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2));
    if (sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   RCAR_CPG_SDnCKCR                 = 0x%08X :", sdmmc_read(sdmmc->vcpg_base, sdmmc->clkrate_offset));
    }
#endif

    sdmmc->tap_num = (sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL) >> 16) & 0xFF;

    /* Issue CMD19/CMD21, 2 times for each tap */
    for (i = 0; i < sdmmc->tap_num * 2; i++) {
        /* Clear tuning data buffer to avoid comparing old data after each unsuccessful transfer */
        memset(td, 0, tlen);
        memset(cmd, 0, sizeof( struct sdio_cmd));

        /* Change the sampling clock position */
        sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, i % sdmmc->tap_num );

        /* Setup tuning command */
        sdio_setup_cmd(cmd, SCF_CTYPE_ADTC | SCF_RSP_R1, op, 0);
        sge.sg_count = tlen; sge.sg_address = (paddr_t)td;
        sdio_setup_cmd_io(cmd, SCF_DIR_IN, 1, tlen, &sge, 1, NULL);

        /* Execute and wait for tuning command completed */
        sdio_issue_cmd(&hc->device, cmd, RCAR_SDHI_TUNING_TIMEOUT);

        /* Tuning command is completed successfully, compare result data with tuning block pattern.
         * It is to determine the sampling clock position */
        if ((cmd->status == CS_CMD_CMP)) {
            if (!(memcmp(td, sdio_tbp, tlen))) {
                sdmmc->taps[i] = 1; //This TAP is 'OK'
            }
        }  else { /* Fix tuning error on some hardware platforms */
            /* A delay of 3.75ms (=150ms/40) based on the specification that the device should complete 40 commands in 150msec */
            delay(4);
        }

        if (!sdmmc_read(sdmmc->vbase, MMC_SCC_SMPCMP)) {
            sdmmc->smpcmp[i] = 1; //smpcmp of this TAP is 'OK'
        }
    }

    status = rcar_sdmmc_select_tuning(hc);

    sdio_free(td, tlen);
    sdio_free_cmd(cmd);

    if (status != EOK) {
        rcar_sdmmc_scc_reset(hc);
    }

    return (status);
}

static uint32_t sdmmc_tmpport_read(uintptr_t base, uint32_t addr)
{
    /* Read mode */
    sdmmc_write(base, MMC_SCC_TMPPORT5, MMC_SCC_TMPPORT5_DLL_RW_SEL_R |
               (MMC_SCC_TMPPORT5_DLL_ADR_MASK & addr));

    /* Access start and stop */
    sdmmc_write(base, MMC_SCC_TMPPORT4, MMC_SCC_TMPPORT4_DLL_ACC_START);
    sdmmc_write(base, MMC_SCC_TMPPORT4, 0);

    /* Read and return value */
    return sdmmc_read(base, MMC_SCC_TMPPORT7);
}

static void sdmmc_tmpport_write(uintptr_t base, uint32_t addr, uint32_t val)
{
    /* Write mode */
    sdmmc_write(base, MMC_SCC_TMPPORT5, MMC_SCC_TMPPORT5_DLL_RW_SEL_W |
               (MMC_SCC_TMPPORT5_DLL_ADR_MASK & addr));

    /* Write value */
    sdmmc_write(base, MMC_SCC_TMPPORT6, val);

    /* Access start and stop */
    sdmmc_write(base, MMC_SCC_TMPPORT4, MMC_SCC_TMPPORT4_DLL_ACC_START);
    sdmmc_write(base, MMC_SCC_TMPPORT4, 0);
}

static void rcar_sdmmc_hs400_adjust_enable(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc;
    uint32_t        calib_code;

    sdmmc   = (rcar_sdmmc_t *)hc->cs_hdl;

    if(!sdmmc->adjust_hs400_calib_table)
        return;

    /* Release WriteProtect of DLL built-in register:
     *  Instruct Write: Target register :MD(H’00)
     *  Set WriteProtect release code(H’A500_0000)
     */
    sdmmc_tmpport_write(sdmmc->vbase, 0x00, MMC_SCC_TMPPORT_DISABLE_WP_CODE);

    /* Reading the calibration code value:
     *  Instruction of Read: Target register :DQSRMONH0(H’26)
     */
    calib_code = sdmmc_tmpport_read(sdmmc->vbase, 0x26);
    calib_code &= MMC_SCC_TMPPORT_CALIB_CODE_MASK;
    calib_code = sdmmc->adjust_hs400_calib_table[calib_code];

    /* Adjust internal DS signal:
     *  Instruct Write: Target register :DQSRSETH0(H’22)
     *  Set DQSRSETH0.DQS00RSET(m) and write calibration code
     *  Set DelayLine Offset value(n) to TMPPORT3
     */
    sdmmc_tmpport_write(sdmmc->vbase, 0x22, MMC_SCC_TMPPORT_MANUAL_MODE | calib_code);
    sdmmc_write(sdmmc->vbase, MMC_SCC_TMPPORT3, sdmmc->adjust_hs400_offset);

#ifdef RCAR_SDMMC_SCC_DEBUG
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   ************* HS400 Adjust Enable ********    ");
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT3                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT3));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT4                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT4));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT6                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT6));
    sdio_slogf(_SLOGC_SDIODI, _SLOG_INFO, hc->cfg.verbosity, 1, "   MMC_SCC_TMPPORT7                   = 0x%08X :", sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT7));
#endif

    sdmmc->need_adjust_hs400_done = 1;

    /* Clear adjust HS400 mode flag */
    sdmmc->need_adjust_hs400 = 0;
}

static void rcar_sdmmc_hs400_adjust_disable(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc;

    sdmmc   = (rcar_sdmmc_t *)hc->cs_hdl;

    /* Release WriteProtect of DLL built-in register:
     *  Instruct Write: Target register :MD(H’00)
     *  Set WriteProtect release code(H’A500_0000)
     */
    sdmmc_tmpport_write(sdmmc->vbase, 0x00, MMC_SCC_TMPPORT_DISABLE_WP_CODE);

    /* Releases the adjustment of the internal DS signal
     *  Disabled Manual Calibration W(addr=0x22, 0)
     *  Clear offset value to TMPPORT3
     */
    sdmmc_tmpport_write(sdmmc->vbase, 0x22, 0);
    sdmmc_write(sdmmc->vbase, MMC_SCC_TMPPORT3, 0);

    sdmmc->need_adjust_hs400_done = 0;
}

static void rcar_sdmmc_scc_hs400_reset(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    /* Reset HS4000 mode */
    sdmmc_write(sdmmc->vbase, MMC_SDIF_MODE, sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE) & ~SDIF_MODE_HS400);
    sdmmc_write(sdmmc->vbase, MMC_SCC_DT2FF, sdmmc->tap);
    sdmmc_write(sdmmc->vbase, MMC_SCC_TMPPORT2,  sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2) &
            ~(MMC_SCC_TMPPORT2_HS400OSEL | MMC_SCC_TMPPORT2_HS400EN));

    if (sdmmc->need_adjust_hs400_done == 1) {
        rcar_sdmmc_hs400_adjust_disable(hc);
    }
}

static int rcar_sdmmc_scc_manual_correction(sdio_hc_t *hc)
{
    rcar_sdmmc_t  *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    uint32_t      rvsreq;
    uint32_t      smpcmp;
    uint32_t      new_tap = sdmmc->tap_set;
    uint32_t      err_tap = sdmmc->tap_set;

    rvsreq = sdmmc_read(sdmmc->vbase, MMC_SCC_RVSREQ);

    /* No error */
    if(!rvsreq)
        return (EOK);

    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSREQ, 0);

    /* Change TAP position according to correction status */
    if (sdmmc->hs400_ignore_dat_correction && hc->timing == TIMING_HS400) {
        /*
         * Correction Error Status contains CMD and DAT signal status.
         * In HS400, DAT signal based on DS signal, not CLK.
         * Therefore, use only CMD status.
         */
        smpcmp = sdmmc_read(sdmmc->vbase, MMC_SCC_SMPCMP) & MMC_SCC_SMPCMP_CMD_ERR;

        switch (smpcmp) {
        case 0:
            return EOK;   /* No error in CMD signal */
        case MMC_SCC_SMPCMP_CMD_REQUP:
            new_tap = (sdmmc->tap_set + sdmmc->tap_num + 1) % sdmmc->tap_num;
            err_tap = (sdmmc->tap_set + sdmmc->tap_num - 1) % sdmmc->tap_num;
            break;
        case MMC_SCC_SMPCMP_CMD_REQDOWN:
            new_tap = (sdmmc->tap_set + sdmmc->tap_num - 1) % sdmmc->tap_num;
            err_tap = (sdmmc->tap_set + sdmmc->tap_num + 1) % sdmmc->tap_num;
            break;
        default:
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
                "%s: ERROR in SCC, SCC_RVSREQ 0x%08X, SCC_SMPCMP 0x%08X, Need retune",
                __func__, rvsreq, smpcmp);
            return -(EIO);   /* Need re-tune */
        }

        if (sdmmc->hs400_bad_tap & (1 << new_tap)) {
            /* New tap is bad tap (cannot change).
               Compare  with HS200 tunning result, if smpcmp[err_tap] is OK, perform retune */

            if (sdmmc->smpcmp[err_tap])
                return -(EIO);   /* Need re-tune */

            return (EOK);   /* cannot change */
        }

        sdmmc->tap_set = new_tap;
    } else {
        if (rvsreq & MMC_SCC_RVSREQ_RVSERR) {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
                "%s: ERROR in SCC, SCC_RVSREQ 0x%08X, Need retune", __func__, rvsreq);
            return -(EIO);    /* Need re-tune */
        } else if (rvsreq & MMC_SCC_RVSREQ_REQTAPUP) {
            sdmmc->tap_set = (sdmmc->tap_set + sdmmc->tap_num + 1) % sdmmc->tap_num;
        } else if (rvsreq & MMC_SCC_RVSREQ_REQTAPDWN) {
            sdmmc->tap_set = (sdmmc->tap_set + sdmmc->tap_num - 1) % sdmmc->tap_num;
        } else {
            return (EOK);
        }
    }

    /* Set TAP position */
    if (sdmmc->hs400_use_4tap) {
        sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, sdmmc->tap_set / 2);
    } else {
        sdmmc_write(sdmmc->vbase, MMC_SCC_TAPSET, sdmmc->tap_set);
    }

    return (EOK);
}

static int rcar_sdmmc_scc_auto_correction(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    uint32_t     rvsreq;

    /* Check SCC error */
    if ( (rvsreq = sdmmc_read(sdmmc->vbase, MMC_SCC_RVSREQ)) & MMC_SCC_RVSREQ_RVSERR) {

        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SCC, SCC_RVSREQ 0x%08X, Need retune", __func__, rvsreq);

        /* Clear SCC error */
        sdmmc_write(sdmmc->vbase, MMC_SCC_RVSREQ, 0);

        return -(EIO); /* Need re-tune */
    }

    return (EOK); /* No error */
}

static int rcar_sdmmc_scc_error_check(sdio_hc_t *hc, uint32_t info2)
{
    rcar_sdmmc_t *sdmmc;

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (!(hc->timing == TIMING_SDR104) &&
        !(hc->timing == TIMING_HS200)  &&
        !(hc->timing == TIMING_HS400   && !sdmmc->hs400_use_4tap)) {
        return (EOK);
    }

    if (sdmmc->tuning_status == RCAR_TUNING_DOING) {
        return (EOK);
    }

    if (info2 & (SDH_INFO2_CMDE | SDH_INFO2_CRCE | SDH_INFO2_ENDE | SDH_INFO2_DTO | SDH_INFO2_RTO)) {
        /* Clear SCC error */
        sdmmc_write(sdmmc->vbase, MMC_SCC_RVSREQ, 0);

        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: ERROR in SDHI%d, SD_INFO2 0x%08X , Need retune", __func__, sdmmc->chan_idx, info2);

        /*
         * Follow section 70.7.2 Sampling Clock Position Correction after Tuning
         * of document ver.0027, we need re-tuning for these errors
         */
        return -(EIO); /* Need re-tune */
    }

    if (sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL) & MMC_SCC_RVSCNTL_RVSEN) {
        /* Automatic correction */
        return (rcar_sdmmc_scc_auto_correction(hc));
    } else {
        /* Manual correction */
        return (rcar_sdmmc_scc_manual_correction(hc));
    }
}

static int rcar_sdmmc_cd(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;
    int         cstate, pstate;

    sdmmc  = (rcar_sdmmc_t *)hc->cs_hdl;
    cstate = CD_RMV;
    pstate = sdmmc_read(sdmmc->vbase, MMC_SD_INFO1);

    hc->caps |= HC_CAP_CD_INTR;

    if ((pstate & SDH_INFO1_CD)) {
        cstate  |= CD_INS;
        if (!(pstate & SDH_INFO1_WP))
            cstate |= CD_WP;
    }

    return (cstate);
}

static int rcar_sdmmc_dma_init(sdio_hc_t *hc)
{
    sdio_hc_cfg_t *cfg;

    cfg   = &hc->cfg;

    cfg->sg_max = 1;

    return (EOK);
}

int rcar_sdmmc_dinit(sdio_hc_t *hc)
{
    rcar_sdmmc_t *sdmmc;

    if (!hc || !hc->cs_hdl) {
        return (EOK);
    }

    sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;

    if (sdmmc->vbase) {
        if (hc->hc_iid != -1) {
            InterruptDetach(hc->hc_iid);
        }

        munmap_device_io(sdmmc->vbase, sdmmc->basesize);

        if (sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
            munmap_device_io(sdmmc->vcpg_base, 0x1000);
        }
    }

    free(sdmmc);
    hc->cs_hdl = NULL;

    return (EOK);
}


static int rcar_sdmmc_reset(sdio_hc_t *hc)
{
    rcar_sdmmc_t    *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    int timeout = 1000;

    /* Apply a soft reset */
    sdmmc_write(sdmmc->vbase, MMC_SOFT_RST, sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) & ~SOFT_RST_OFF);
    sdmmc_write(sdmmc->vbase, MMC_SOFT_RST, sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) | SOFT_RST_OFF);

    /* Check reset release state */
    while((!(sdmmc_read(sdmmc->vbase, MMC_SOFT_RST) & SOFT_RST_OFF)) && (timeout-- > 0)) {
        nanospin_ns(1000);
    }

    sdmmc_write(sdmmc->vbase, MMC_SD_INFO1_MASK, SDH_INFO1_MASK_ALL);
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO2_MASK, SDH_INFO2_MASK_ALL);
    sdmmc_write(sdmmc->vbase, MMC_HOST_MODE, 0x00000000);    // SD_BUF access width = 64-bit
    sdmmc_write(sdmmc->vbase, MMC_SD_OPTION, 0x0000C0EE);    // Bus width = 1bit, timeout=MAX
    sdmmc_write(sdmmc->vbase, MMC_SD_CLK_CTRL, 0x00000080);  // Automatic Control=Disable, Clock Output=Disable

    /* Mask all DMA interrupts */
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO1_MASK, DM_INFO1_ALL_MSK);
    sdmmc_write(sdmmc->vbase, MMC_DM_CM_INFO2_MASK, DM_INFO2_ALL_MSK);

    /* Reset SCC */
    sdmmc_write(sdmmc->vbase, MMC_SCC_CKSEL, ~MMC_SCC_CKSEL_DTSEL & sdmmc_read(sdmmc->vbase, MMC_SCC_CKSEL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_RVSCNTL, ~MMC_SCC_RVSCNTL_RVSEN & sdmmc_read(sdmmc->vbase, MMC_SCC_RVSCNTL));
    sdmmc_write(sdmmc->vbase, MMC_SCC_DTCNTL, ~MMC_SCC_DTCNTL_TAPEN & sdmmc_read(sdmmc->vbase, MMC_SCC_DTCNTL));

    /* Reset HS400 mode */
    sdmmc_write(sdmmc->vbase, MMC_SDIF_MODE, sdmmc_read(sdmmc->vbase, MMC_SDIF_MODE) & ~SDIF_MODE_HS400);
    sdmmc_write(sdmmc->vbase, MMC_SCC_TMPPORT2,  sdmmc_read(sdmmc->vbase, MMC_SCC_TMPPORT2) &
                ~(MMC_SCC_TMPPORT2_HS400OSEL | MMC_SCC_TMPPORT2_HS400EN));

    if(sdmmc->adjust_hs400_enable) {
        rcar_sdmmc_hs400_adjust_disable(hc);
    }

    sdmmc->tuning_status = RCAR_TUNING_NONE;

    sdmmc->busclk = 400 * 1000;      // 400KHz clock for ident

    /* configure clock */
    rcar_sdmmc_clk(hc, sdmmc->busclk);

    return (EOK);
}

static sdio_hc_entry_t rcar_sdmmc_entry = {
    16,
    rcar_sdmmc_dinit, NULL,
    rcar_sdmmc_cmd, rcar_sdmmc_abort,
    rcar_sdmmc_event, rcar_sdmmc_cd, rcar_sdmmc_pwr,
    rcar_sdmmc_clk, rcar_sdmmc_bus_mode,
    rcar_sdmmc_bus_width, rcar_sdmmc_timing,
    rcar_sdmmc_signal_voltage, NULL, NULL, rcar_sdmmc_tune, NULL
};

static void rcar_sdmmc_parse_hs400_support(sdio_hc_t *hc)
{
    rcar_sdmmc_t  *sdmmc = (rcar_sdmmc_t *)hc->cs_hdl;
    sdio_hc_cfg_t *cfg = &hc->cfg;
    uintptr_t     prr_reg;
    uint32_t      prr_val;

    sdmmc->tap                      = 0x00000300;
    sdmmc->tap_hs400                = 0x00000300;
    sdmmc->hs400_use_4tap           = 0;
    sdmmc->dma_tranend1             = 20;
    sdmmc->adjust_hs400_calib_table = NULL;
    sdmmc->sdckcr_val_for_hs400     = 1;
    sdmmc->adjust_hs400_enable      = 0;
    sdmmc->adjust_hs400_offset      = MMC_SCC_TMPPORT3_OFFSET_0;

    hc->caps |= HC_CAP_HS400;

    /* R-CarGen3 SCC use manual sampling clock position correction in HS400 (Following eMMC_restriction_WA_Ver5.4) */
    sdmmc->hs400_manual_correction = 1;

    /* R-CarGen3 SCC doesn't use DAT signal correction error status in HS400 (Following eMMC_restriction_WA_Ver5.4) */
    sdmmc->hs400_ignore_dat_correction = 1;

    if ((prr_reg = mmap_device_io(4, RCAR_GEN3_PRODUCT_REGISTER)) != (uintptr_t)MAP_FAILED) {
        prr_val = in32(prr_reg);
        switch RCAR_GEN3_PRODUCT_ID(prr_val) {
            case PRODUCT_ID_RCAR_V3M:
            case PRODUCT_ID_RCAR_V3H:
            case PRODUCT_ID_RCAR_D3:
                /* D3 & V3M doesn't support HS400 speed.
                   V3H HS400 mode is a specification limitation (Following R-Car Gen3 W/A of eMMC restriction #109). */
                hc->caps &= ~HC_CAP_HS400;
                break;
            case PRODUCT_ID_RCAR_H3:
                switch RCAR_GEN3_PRODUCT_REV(prr_val) {
                    case PRODUCT_H3_REV_1_0:
                    case PRODUCT_H3_REV_1_1:
                        sdmmc->dma_tranend1 = 17;
                        /* H3 1.0 and 1.1 HS400 mode are a specification limitation (Following R-Car Gen3 W/A of eMMC restriction #109) */
                        hc->caps &= ~HC_CAP_HS400;
                        break;
                    case PRODUCT_H3_REV_2_0:
                        /* H3 2.0 uses 4TAP in HS400 mode */
                        sdmmc->hs400_use_4tap = 1;
                        /* H3 2.0 doesn't use TAP 2, 3, 6, 7 in HS400 (Following eMMC_restriction_WA_Ver5.4) */
                        sdmmc->hs400_bad_tap = (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
                        /* 4tap use 0x0100 for DT2FF */
                        sdmmc->tap_hs400 = 0x00000100;
                        /* H3 2.0 needs to change SDH clock from 800Mhz to 400MHz in HS400 mode */
                        sdmmc->sdckcr_val_for_hs400 = 4;
                        break;
                    case PRODUCT_H3_REV_3_0: // ES3.0
                        /* H3 3.0 doesn't use TAP 2, 3, 6, 7 in HS400 (Following eMMC_restriction_WA_Ver5.4) */
                        sdmmc->hs400_bad_tap = (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
                        break;
                    default:
                        break;
                }
                break;
            case PRODUCT_ID_RCAR_M3W:
                switch RCAR_GEN3_PRODUCT_REV(prr_val) {
                    case PRODUCT_M3_REV_1_0: // ES1.0
                        sdmmc->dma_tranend1 = 17;
                    case PRODUCT_M3_REV_1_1: // ES1.1 & ES1.2
                        /* M3 1.0 and 1.1 and 1.2 HS400 mode are a specification limitation (Following R-Car Gen3 W/A of eMMC restriction #109) */
                        hc->caps &= ~HC_CAP_HS400;
                        break;
//TODO: Keep M3W 1.3 disabled until we can test HS400
#if 0
                    case PRODUCT_M3_REV_1_3: // ES1.3
                        /* M3 1.3 uses 4TAP in HS400 mode */
                        sdmmc->hs400_use_4tap = 1;
                        /* M3 1.3 doesn't use TAP 1, 3 in HS400 mode (Following eMMC_restriction_WA_Ver5.4) */
                        sdmmc->hs400_bad_tap = (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7); //(TAP=2,TAP=3)/2=(TAP=1) and (TAP=6,TAP=7)/2=(TAP=3)
                        /* 4tap use 0x0100 for DT2FF */
                        sdmmc->tap_hs400 = 0x00000100;
                        /* M3 1.3 needs to change SDH clock from 800Mhz to 400MHz in HS400 mode */
                        sdmmc->sdckcr_val_for_hs400 = 4;
                        /* M3 1.3 needs to apply Manual Calibration Adjustment in HS400 mode (Following R-Car Gen3 W/A of eMMC restriction #109) */
                        sdmmc->adjust_hs400_enable = 1;
                        if (RCAR_GEN3_SDHI2_BASE == sdmmc->pbase) { // MMC0
                            sdmmc->adjust_hs400_calib_table = m3w_v13_hs400_calib_code[0];
                        } else if (RCAR_GEN3_SDHI3_BASE == sdmmc->pbase) { // MMC1
                            sdmmc->adjust_hs400_calib_table = m3w_v13_hs400_calib_code[1];
                        }
                        break;
#endif
                    case PRODUCT_M3_REV_3_0: // ES3.0
                        /* M3 3.0 doesn't use TAP 1, 3, 5, 7 in HS400 mode (Following eMMC_restriction_WA_Ver5.4) */
                        sdmmc->hs400_bad_tap = (1 << 1) | (1 << 3) | (1 << 5) | (1 << 7);
                        break;
                    default:
                        break;
                }
                break;
            case PRODUCT_ID_RCAR_M3N:
                /* M3N doesn't use TAP 2, 3, 6, 7 in HS400 (Following eMMC_restriction_WA_Ver5.4) */
                sdmmc->hs400_bad_tap = (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
                /* M3N needs to apply Manual Calibration Adjustment in HS400 mode (Following R-Car Gen3 W/A of eMMC restriction #109) */
                sdmmc->adjust_hs400_enable = 1;
                if ( RCAR_GEN3_SDHI2_BASE == sdmmc->pbase ) {  // MMC0
                    sdmmc->adjust_hs400_calib_table = m3n_hs400_calib_code[0];
                } else if ( RCAR_GEN3_SDHI3_BASE == sdmmc->pbase ) { // MMC1
                    sdmmc->adjust_hs400_calib_table = m3n_hs400_calib_code[1];
                }

                break;
            case PRODUCT_ID_RCAR_E3:
                /* There is no inhibit TAP for  R-Car E3. */
                sdmmc->hs400_bad_tap = 0;
                /* E3 SCC can use Auto sampling clock position correction in HS400 (Following eMMC_restriction_WA_Ver5.4) */
                //sdmmc->hs400_manual_correction = 0;
                /* E3 needs to apply Manual Calibration Adjustment in HS400 mode (Following R-Car Gen3 W/A of eMMC restriction #109) */
                sdmmc->adjust_hs400_enable = 1;
                if (RCAR_GEN3_SDHI3_BASE == sdmmc->pbase) {// MMC1
                    sdmmc->adjust_hs400_calib_table = e3_mmc_hs400_calib_code;
                }
                break;
            default:
                break;
        }

        munmap_device_io(prr_reg, 4);
    }

    sdmmc->vcpg_base = (uintptr_t)MAP_FAILED;
    if ( (hc->flags & HC_FLAG_DEV_MMC) && (hc->caps & HC_CAP_HS400) ) {

        if (cfg->idx == 0) {
            sdmmc->clkrate_offset = RCAR_GEN3_CPG_SD0CKCR;
        } else if (cfg->idx == 1) {
            sdmmc->clkrate_offset = RCAR_GEN3_CPG_SD1CKCR;
        } else if (cfg->idx == 2) {
            sdmmc->clkrate_offset = RCAR_GEN3_CPG_SD2CKCR;
        } else if (cfg->idx == 3) {
            sdmmc->clkrate_offset = RCAR_GEN3_CPG_SD3CKCR;
        }
        sdmmc->vcpg_base = mmap_device_io(0x1000, RCAR_GEN3_CPG_BASE);

        /* May slay and restart driver so should always start driver with initial clock rate setting = 1 */
        if(sdmmc->vcpg_base != (uintptr_t)MAP_FAILED) {
            out32(sdmmc->vcpg_base + RCAR_GEN3_CPG_CPGWPR, ~1);
            out32(sdmmc->vcpg_base + sdmmc->clkrate_offset, 1);
        }
        else {
            sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
                "%s: Unable to mmap_device_io (CPG base 0x%x) and unable change clock rate (error: %s)", __func__, RCAR_GEN3_CPG_BASE, strerror(errno));
        }

    }
}

int rcar_sdmmc_init(sdio_hc_t *hc)
{
    sdio_hc_cfg_t   *cfg;
    rcar_sdmmc_t    *sdmmc;
    struct sigevent event;

    hc->hc_iid  = -1;
    cfg         = &hc->cfg;

    memcpy(&hc->entry, &rcar_sdmmc_entry, sizeof(sdio_hc_entry_t));

    if ((sdmmc = hc->cs_hdl = calloc(1, sizeof(rcar_sdmmc_t))) == NULL) {
        return (ENOMEM);
    }

    if (cfg->base_addrs > 0 && cfg->irqs > 0) {
        sdmmc->pbase = cfg->base_addr[0];
        sdmmc->basesize = cfg->base_addr_size[0];
        sdmmc->irq   = cfg->irq[0];
        sdmmc->chan_idx = cfg->idx;
    } else {
        rcar_sdmmc_dinit(hc);
        return (ENODEV);
    }

    if ((sdmmc->vbase = (uintptr_t)mmap_device_io(sdmmc->basesize, sdmmc->pbase)) == (uintptr_t)MAP_FAILED) {
        sdio_slogf(_SLOGC_SDIODI, _SLOG_ERROR, hc->cfg.verbosity, 1,
            "%s: Unable to mmap_device_io (SDHI base 0x%lx) %s", __func__, sdmmc->pbase, strerror(errno));
        rcar_sdmmc_dinit(hc);
        return (errno);
    }

    if (cfg->clk) {
        sdmmc->pclk = cfg->clk;
    } else {
        sdmmc->pclk = 200 * 1000 * 1000;
    }

    /*
     * In R-Car Gen3 SDHI, internal SCC modules uses SDnH clock.
     * It is controlled by SDnCKCR.STPnHCK bit in CPG.
     * When SD clock is less than High Speed, SDnH clock is stopped.
     * And SDnH clock is supplied with 100MHz or more in Clock divider
     * table of CPG in R-Car Gen3.
     * It is the recommended setting of H/W.
     */
    sdmmc->scc_min_clk = 100 * 1000 * 1000;

    hc->clk_max = sdmmc->pclk;

    hc->caps |= HC_CAP_BSY | HC_CAP_BW4 | HC_CAP_BW8;
    hc->caps |= HC_CAP_ACMD12;
    hc->caps |= HC_CAP_DMA;
    hc->caps |= HC_CAP_HS | HC_CAP_HS200 | HC_CAP_SDR50 | HC_CAP_SDR104;

    rcar_sdmmc_parse_hs400_support(hc);

    hc->caps &= cfg->caps;      /* reconcile command line options */

    if (rcar_sdmmc_dma_init(hc) != EOK) {
        rcar_sdmmc_dinit(hc);
        return (ENODEV);
    }

    rcar_sdmmc_reset(hc);

    /* we don't want this interrupt at the driver startup */
    while (sdmmc_read(sdmmc->vbase, MMC_SD_INFO1) & SDH_INFO1_INST) {
        sdmmc_write(sdmmc->vbase, MMC_SD_INFO1, ~(SDH_INFO1_INST | SDH_INFO1_RMVL));
    }

	/* Only enable the card insertion and removal interrupts */
    sdmmc_write(sdmmc->vbase, MMC_SD_INFO1_MASK,  sdmmc_read(sdmmc->vbase, MMC_SD_INFO1_MASK) & ~(SDH_INFO1_INST | SDH_INFO1_RMVL));

    SIGEV_PULSE_INIT(&event, hc->hc_coid, SDIO_PRIORITY, HC_EV_INTR, NULL);
    if ((hc->hc_iid = InterruptAttachEvent(sdmmc->irq, &event, _NTO_INTR_FLAGS_TRK_MSK)) == -1) {
        rcar_sdmmc_dinit(hc);
        return (errno);
    }

    return (EOK);
}
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/sdmmc/sdiodi/hc/rcar_gen3.c $ $Rev: 944140 $")
#endif
