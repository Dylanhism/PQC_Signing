/*
 * $QNXLicenseC:
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 * Copyright 2018, QNX Software Systems.
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

struct wm8960_context;
#define MIXER_CONTEXT_T struct wm8960_context

#include <string.h>
#include <stdint.h>
#include <audio_driver.h>
#include <hw/i2c.h>
#include <mixer/i2s_codec_dll.h>
#include "wm8960.h"

/** One playback device, one capture device. */
static int32_t pcm_devices[1] = { 0 };

/** Speaker Range */
static struct snd_mixer_element_volume1_range spk_range[2] = {
    {0, WM8960_SPKOUT_VOL_MAX - WM8960_SPKOUT_VOL_MIN, -7300, 600, 100, {0}},
    {0, WM8960_SPKOUT_VOL_MAX - WM8960_SPKOUT_VOL_MIN, -7300, 600, 100, {0}},
};

/** Headphone Range */
static struct snd_mixer_element_volume1_range hp_range[2] = {
    {0, WM8960_HPOUT_VOL_MAX - WM8960_HPOUT_VOL_MIN, -7300, 600, 100, {0}},
    {0, WM8960_HPOUT_VOL_MAX - WM8960_HPOUT_VOL_MIN, -7300, 600, 100, {0}},
};

/** DAC Master Range */
static struct snd_mixer_element_volume1_range master_range[2] = {
    {0, WM8960_DAC_VOL_MAX - WM8960_DAC_VOL_MIN, -12700, 0, 100, {0}},
    {0, WM8960_DAC_VOL_MAX - WM8960_DAC_VOL_MIN, -12700, 0, 100, {0}},
};

/** ADC IGAIN Range */
static struct snd_mixer_element_volume1_range adc_range[2] = {
    {0, WM8960_ADC_VOL_MAX - WM8960_ADC_VOL_MIN, -9700, 3000, 100, {0}},
    {0, WM8960_ADC_VOL_MAX - WM8960_ADC_VOL_MIN, -9700, 3000, 100, {0}},
};

/** Analog Input Range */
static struct snd_mixer_element_volume1_range input_range[2] = {
    {0, WM8960_INPUT_VOL_MAX - WM8960_INPUT_VOL_MIN, -1725, 3000, 100, {0}},
    {0, WM8960_INPUT_VOL_MAX - WM8960_INPUT_VOL_MIN, -1725, 3000, 100, {0}},
};

/**
 * Writes data into external audio codec over i2c data bus.
 *
 * @param mixer Mixer context.
 * @param reg WM8960 register address.
 * @param val Value to be written.
 *
 * @return Execution status.
 */
static int wm8960_write(MIXER_CONTEXT_T * mixer, uint32_t reg, uint32_t val)
{
    int status = EOK;
    uint32_t i = 0;

    struct {
        i2c_send_t hdr;
        uint16_t regval;
    } msg;

    msg.hdr.slave.addr = mixer->i2c_addr;
    msg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    msg.hdr.len = 2;
    msg.hdr.stop = 1;
    /* First 7 bits is the register address, the next 9 bits is the data value (byte swapped) */
    msg.regval = (0xff & ((reg << 1) | (0x1 & (val >> 8)))) | ((0xff & val) << 8);

    do {
        status = devctl(mixer->i2c_fd, DCMD_I2C_SEND, &msg, sizeof(msg), NULL);
    } while ((status == EAGAIN) && (i++ < 10) && (usleep(10000) == 0));

    if (status != EOK) {
        ado_error_fmt("I2C_WRITE failed");
    } else {
        /* Save written value */
        mixer->regs[reg] = val;
    }
    return status;
}

/**
 * Provides a non-destructive way of modifying register contents.
 *
 * @param mixer Mixer context.
 * @param reg WM8960 register address.
 * @param mask The mask code for the bits want to change.
 * @param val Value to be written.
 *
 * @return Execution status.
 */
static int wm8960_update(MIXER_CONTEXT_T * mixer, uint32_t reg, uint32_t mask, uint32_t val, uint32_t write_reg)
{
    /* WM8960 can't accept read commands so we read from regs buffer */
    uint32_t current_value = mixer->regs[reg];

    /* Always update the regs buffer */
    current_value &= ~mask;

    if(write_reg == WM8960_UPDATE_AND_WRITE_REG) { /* If a bit isn't sticky we avoid an extra codec access */
        return wm8960_write(mixer, reg, current_value | val);
    } else if(write_reg == WM8960_UPDATE_ONLY) {
        mixer->regs[reg] = current_value | val;
        return EOK;
    }

    return -1;
}

/**
 * Controls Speaker mute.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a mute will be set. When false mute state will be got.
 * @param val Mute value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case mute has been changed.
 */
static int32_t wm8960_spk_mute_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                       uint32_t * val, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        altered = (wm8960->spk_mute != (val[0] & 0x3)) ? 1 : 0;
        if (altered) {
            if (val[0] & (1<<0)) {
                wm8960->spk_mute |= (1<<0);
            } else {
                wm8960->spk_mute &= ~(1<<0);
            }

            if (val[0] & (1<<1)) {
                wm8960->spk_mute |= (1<<1);
            } else {
                wm8960->spk_mute &= ~(1<<1);
            }

            /* Clear volume update bit */
            status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_UPDATE_MASK, 0,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("Clear volume update bit failed");
                return 0;
            }
            if (wm8960->spk_mute & (1<<0)) {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_L, WM8960_SPKOUT_VOL_CTRL_MASK,
                                       WM8960_SPKOUT_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_L, WM8960_SPKOUT_VOL_CTRL_MASK,
                                       wm8960->spk_volume[0], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            if (wm8960->spk_mute & (1<<1)) {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_CTRL_MASK,
                                       WM8960_SPKOUT_VOL_MUTE_VAL,
                                       WM8960_UPDATE_AND_WRITE_REG);
            } else {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_CTRL_MASK,
                                       wm8960->spk_volume[1],
                                       WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            /* Set volume update bit */
            status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_UPDATE_MASK,
                                   WM8960_SPKOUT_VOL_UPDATE_MASK,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("set volume update bit failed");
                return 0;
            }
        }
    } else {
        val[0] = wm8960->spk_mute;
    }

    return (altered);
}
/**
 * Controls Speaker volume.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new volume will be set. When false volume will be got.
 * @param vol Volume value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_spk_vol_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                      uint32_t * vol, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        // This offset is used since spk_range was shifted to start from 0
        vol[0] += WM8960_SPKOUT_VOL_MIN;
        vol[1] += WM8960_SPKOUT_VOL_MIN;

        altered = ((vol[0] != ((wm8960->spk_volume[0]) & WM8960_SPKOUT_VOL_CTRL_MASK)) ||
                   (vol[1] != ((wm8960->spk_volume[1]) & WM8960_SPKOUT_VOL_CTRL_MASK)));

        if (altered) {
            if ((wm8960->spk_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Clear volume update bit */
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_UPDATE_MASK, 0,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("Clear volume update bit failed");
                    return 0;
                }
            }
            if (!(wm8960->spk_mute & (1 << 0))) {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_L,
                                       WM8960_SPKOUT_VOL_CTRL_MASK, vol[0], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if (!(wm8960->spk_mute & (1 << 1))) {
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R,
                                       WM8960_SPKOUT_VOL_CTRL_MASK, vol[1], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if ((wm8960->spk_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Set volume update bit */
                status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_UPDATE_MASK,
                                       WM8960_SPKOUT_VOL_UPDATE_MASK,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("set volume update bit failed");
                    return 0;
                }
            }
            /* Save volume */
            wm8960->spk_volume[0] = vol[0];
            wm8960->spk_volume[1] = vol[1];
        }
    } else {
        /* Get saved volume */
        vol[0] = (wm8960->spk_volume[0] & WM8960_SPKOUT_VOL_CTRL_MASK) - WM8960_SPKOUT_VOL_MIN;
        vol[1] = (wm8960->spk_volume[1] & WM8960_SPKOUT_VOL_CTRL_MASK) - WM8960_SPKOUT_VOL_MIN;
    }

    return (altered);
}

/**
 * Controls Headphone mute.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new mute will be set. When false mute status will be got.
 * @param val Mute value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_hp_mute_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                      uint32_t * val, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        altered = (wm8960->hp_mute != (val[0] & 0x3)) ? 1 : 0;

        if (altered) {
            if (val[0] & (1<<0)) {
                wm8960->hp_mute |= (1<<0);
            } else {
                wm8960->hp_mute &= ~(1<<0);
            }

            if (val[0] & (1<<1)) {
                wm8960->hp_mute |= (1<<1);
            } else {
                wm8960->hp_mute &= ~(1<<1);
            }

            /* Clear volume update bit */
            status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_UPDATE_MASK, 0,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("clear volume update bit failed");
                return 0;
            }
            if (wm8960->hp_mute & (1<<0)) {
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_L, WM8960_HPOUT_VOL_CTRL_MASK,
                                       WM8960_HPOUT_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_L, WM8960_HPOUT_VOL_CTRL_MASK,
                                       wm8960->hp_volume[0], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            if (wm8960->hp_mute & (1<<1)) {
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_CTRL_MASK,
                                       WM8960_HPOUT_VOL_MUTE_VAL,
                                       WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_CTRL_MASK,
                                       wm8960->hp_volume[1],
                                       WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            /* Set volume update bit */
            status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_UPDATE_MASK,
                                   WM8960_HPOUT_VOL_UPDATE_MASK, WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("set volume update bit failed");
                return 0;
            }

        }
    } else {
        val[0] = wm8960->hp_mute;
    }

    return (altered);
}

/**
 * Controls Headphone volume.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new volume will be set. When false volume will be got.
 * @param vol Volume value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_hp_vol_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                     uint32_t * vol, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        // This offset is used since spk_range was shifted to start from 0
        vol[0] += WM8960_HPOUT_VOL_MIN;
        vol[1] += WM8960_HPOUT_VOL_MIN;

        altered = ((vol[0] != ((wm8960->hp_volume[0]) & WM8960_HPOUT_VOL_CTRL_MASK))
                   || (vol[1] != ((wm8960->hp_volume[1]) & WM8960_HPOUT_VOL_CTRL_MASK)));

        if (altered) {
            if ((wm8960->hp_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Clear volume update bit */
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_UPDATE_MASK, 0,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("clear volume update bit failed");
                    return 0;
                }
            }
            if (!(wm8960->hp_mute & (1 << 0))) {
                /* Update Left channel volume */
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_L,
                                       WM8960_HPOUT_VOL_CTRL_MASK, vol[0], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if (!(wm8960->hp_mute & (1 << 1))) {
                /* Update Right channel volume */
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R,
                                       WM8960_HPOUT_VOL_CTRL_MASK, vol[1], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if ((wm8960->hp_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Set volume update bit */
                status = wm8960_update(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_UPDATE_MASK, WM8960_HPOUT_VOL_UPDATE_MASK,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("set volume update bit failed");
                    return 0;
                }
            }
            /* Save volume */
            wm8960->hp_volume[0] = vol[0];
            wm8960->hp_volume[1] = vol[1];
        }
    } else {
        /* Get saved volume */
        vol[0] = (wm8960->hp_volume[0] & WM8960_HPOUT_VOL_CTRL_MASK) - WM8960_HPOUT_VOL_MIN;
        vol[1] = (wm8960->hp_volume[1] & WM8960_HPOUT_VOL_CTRL_MASK) - WM8960_HPOUT_VOL_MIN;
    }

    return (altered);
}

/**
 * Controls Master mute.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new mute will be set. When false mute state will be got.
 * @param val Mute value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case mute has been changed.
 */
static int32_t wm8960_dac_mute_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                       uint32_t * val, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        altered = (wm8960->dac_mute != (val[0] & 0x3)) ? 1 : 0;
        if (altered) {
            if (val[0] & (1<<0)) {
                wm8960->dac_mute |= (1<<0);
            } else {
                wm8960->dac_mute &= ~(1<<0);
            }

            if (val[0] & (1<<1)) {
                wm8960->dac_mute |= (1<<1);
            } else {
                wm8960->dac_mute &= ~(1<<1);
            }

            /* Clear volume update bit */
            status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK, 0,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt(" clear volume update bit failed");
                return 0;
            }
            /* Apply mute update */
            if (wm8960->dac_mute & (1<<0)) {
                status = wm8960_update(wm8960, WM8960_DAC_VOL_L, WM8960_DAC_VOL_CTRL_MASK,
                                       WM8960_DAC_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_DAC_VOL_L, WM8960_DAC_VOL_CTRL_MASK,
                                       wm8960->dac_volume[0], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            if (wm8960->dac_mute & (1<<1)) {
                status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_CTRL_MASK,
                                       WM8960_DAC_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_CTRL_MASK,
                                       wm8960->dac_volume[1], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }

            /* Set volume update bit */
            status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK, WM8960_DAC_VOL_UPDATE_MASK,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt(" set volume update bit failed");
                return 0;
            }
        }
    } else {
        val[0] = wm8960->dac_mute;
    }

    return (altered);
}

/**
 * Controls DAC volume.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new volume will be set. When false volume will be got.
 * @param vol Volume value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_dac_vol_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                      uint32_t * vol, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        // This offset is used since dac_range was shifted to start from 0
        vol[0] += WM8960_DAC_VOL_MIN;
        vol[1] += WM8960_DAC_VOL_MIN;

        altered = ((vol[0] != ((wm8960->dac_volume[0]) & WM8960_DAC_VOL_CTRL_MASK))
                   || (vol[1] != ((wm8960->dac_volume[1]) & WM8960_DAC_VOL_CTRL_MASK)));

        if (altered) {
            if ((wm8960->dac_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Clear volume update bit */
                status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK, 0,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("clear volume update bit failed");
                    return 0;
                }
            }
            if (!(wm8960->dac_mute & (1 << 0))) {
                /* Update Left channel volume */
                status = wm8960_update(wm8960, WM8960_DAC_VOL_L, WM8960_DAC_VOL_CTRL_MASK,
                                       vol[0], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if (!(wm8960->dac_mute & (1 << 1))) {
                /* Update Right channel volume */
                status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_CTRL_MASK,
                                       vol[1], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
            }
            if ((wm8960->dac_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                /* Set volume update bit */
                status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK, WM8960_DAC_VOL_UPDATE_MASK,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("set volume update bit failed");
                    return 0;
                }
            }
            wm8960->dac_volume[0] = vol[0];
            wm8960->dac_volume[1] = vol[1];
        }
    } else {
        /* Get saved volume */
        vol[0] = (wm8960->dac_volume[0] & WM8960_DAC_VOL_CTRL_MASK) - WM8960_DAC_VOL_MIN;
        vol[1] = (wm8960->dac_volume[1] & WM8960_DAC_VOL_CTRL_MASK) - WM8960_DAC_VOL_MIN;
    }

    return (altered);
}

/**
 * Controls ADC Mute.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new mute will be set. When false mute state will be got.
 * @param val Mute value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case mute has been changed.
 */
static int32_t wm8960_adc_mute_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                       uint32_t * val, void *instance_data)
{
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        /* If we are Jointly-mute then translate mute status across all channels */
        if (ado_mixer_get_element_type(element) == SND_MIXER_ETYPE_SWITCH2) {
            val[0] = (val[0] == 1) ? 0x3 : 0;
        }
        altered = (wm8960->adc_mute != (val[0] & 0x3)) ? 1 : 0;
        if (altered) {
            if (val[0] & (1<<0)) {
                wm8960->adc_mute |= (1<<0);
            } else {
                wm8960->adc_mute &= ~(1<<0);
            }

            if (val[0] & (1<<1)) {
                wm8960->adc_mute |= (1<<1);
            } else {
                wm8960->adc_mute &= ~(1<<1);
            }

            /* Clear volume update bit */
            status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_UPDATE_MASK, 0,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("clear volume update bit failed");
                return 0;
            }
            /* Apply mute update */
            if (wm8960->adc_mute & (1<<0)) {
                status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                       WM8960_ADC_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                       wm8960->adc_volume[0], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
            if (wm8960->adc_mute & (1<<1)) {
                status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                       WM8960_ADC_VOL_MUTE_VAL, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                // restore volume
                status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                       wm8960->adc_volume[1], WM8960_UPDATE_AND_WRITE_REG);
            }
            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }

            /* Set volume update bit */
            status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_UPDATE_MASK, WM8960_ADC_VOL_UPDATE_MASK,
                                   WM8960_UPDATE_AND_WRITE_REG);
            if (status != EOK) {
                ado_error_fmt("set volume update bit failed");
                return 0;
            }
        }
    } else {
        val[0] = wm8960->adc_mute;
    }

    return (altered);
}

/**
 * Controls ADC volume.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new volume will be set. When false volume will be got.
 * @param vol Volume value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_adc_vol_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                      uint32_t * vol, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        // This offset is used since dac_range was shifted to start from 0
        if (wm8960->mic_left2right) {
            vol[0] += WM8960_ADC_VOL_MIN;
            altered = (vol[0] != ((wm8960->adc_volume[0]) & WM8960_ADC_VOL_CTRL_MASK));

            if (altered) {
                if (!(wm8960->adc_mute & (1 << 0))) {
                    /* Clear volume update bit */
                    status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_UPDATE_MASK, 0, WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("clear volume update bit failed");
                        return 0;
                    }
                    /* Update left channel volume */
                    status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                           vol[0], WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("update left volume failed");
                        return 0;
                    }
                    /* Set volume update bit */
                    status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_UPDATE_MASK, WM8960_ADC_VOL_UPDATE_MASK, WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("set volume update bit failed");
                        return 0;
                    }
                }
                wm8960->adc_volume[0] = vol[0];
            }
        } else {
            // This offset is used since dac_range was shifted to start from 0
            vol[0] += WM8960_ADC_VOL_MIN;
            vol[1] += WM8960_ADC_VOL_MIN;

            altered = ((vol[0] != ((wm8960->adc_volume[0]) & WM8960_ADC_VOL_CTRL_MASK))
                       || (vol[1] != ((wm8960->adc_volume[1]) & WM8960_ADC_VOL_CTRL_MASK)));

            if (altered) {
                if ((wm8960->adc_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                    /* Clear volume update bit */
                    status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_UPDATE_MASK, 0, WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("clear volume update bit failed");
                        return 0;
                    }
                }
                if (!(wm8960->adc_mute & (1 << 0))) {
                    /* Update Left channel volume */
                    status = wm8960_update(wm8960, WM8960_ADC_L_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                           vol[0], WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("left volume update failed");
                        return 0;
                    }
                }
                if (!(wm8960->adc_mute & (1 << 1))) {
                    /* Update Right channel volume */
                    status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_CTRL_MASK,
                                           vol[1], WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("right volume update failed");
                        return 0;
                    }
                }
                if ((wm8960->adc_mute & 0x3) != 0x3) { // if either or both channels are un-muted
                    /* Set volume update bit */
                    status = wm8960_update(wm8960, WM8960_ADC_R_VOL, WM8960_ADC_VOL_UPDATE_MASK, WM8960_ADC_VOL_UPDATE_MASK, WM8960_UPDATE_AND_WRITE_REG);
                    if (status != EOK) {
                        ado_error_fmt("set volume update bit failed");
                        return 0;
                    }
                }
                wm8960->adc_volume[0] = vol[0];
                wm8960->adc_volume[1] = vol[1];
            }
        }
    } else {
        /* Get saved volume */
        if (wm8960->mic_left2right) {
            vol[0] = (wm8960->adc_volume[0] & WM8960_ADC_VOL_CTRL_MASK) - WM8960_ADC_VOL_MIN;
        } else {
            vol[0] = (wm8960->adc_volume[0] & WM8960_ADC_VOL_CTRL_MASK) - WM8960_ADC_VOL_MIN;
            vol[1] = (wm8960->adc_volume[1] & WM8960_ADC_VOL_CTRL_MASK) - WM8960_ADC_VOL_MIN;
        }
    }

    return (altered);
}

/**
 * Controls Analog Input Mute.
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new mute will be set. When false mute state will be got.
 * @param val Mute value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_input_mute_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                       uint32_t * val, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        /* If we are Jointly-mute then translate mute status across all channels */
        if (ado_mixer_get_element_type(element) == SND_MIXER_ETYPE_SWITCH2) {
            val[0] = (val[0] == 1) ? 0x3 : 0;
        }
        altered = (wm8960->input_mute != (val[0] & 0x3)) ? 1 : 0;
        if (altered) {
            /* Input mute has specific mute control bit, don't need to set volume update bit*/
            if (val[0] & (1<<0)) {
                wm8960->input_mute |= (1<<0);
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_MUTE_MASK,
                                       WM8960_INPUT_VOL_MUTE_MASK, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                wm8960->input_mute &= ~(1<<0);
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_MUTE_MASK,
                                       0, WM8960_UPDATE_AND_WRITE_REG);
            }

            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }

            if (val[0] & (1<<1)) {
                wm8960->input_mute |= (1<<1);
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_MUTE_MASK,
                                       WM8960_INPUT_VOL_MUTE_MASK, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                wm8960->input_mute &= ~(1<<1);
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_MUTE_MASK,
                                       0, WM8960_UPDATE_AND_WRITE_REG);
            }

            if (status != EOK) {
                ado_error_fmt("mute update failed");
                return 0;
            }
        }
    } else {
        val[0] = wm8960->input_mute;
    }

    return (altered);
}

/**
 * Controls Analog Input volume.
 *
 * @param wm8960 Mixer context.
 * @param element Pointer to a mixer element.
 * @param set When true a new volume will be set. When false volume will be got.
 * @param vol Volume value.
 * @param instance_data Driver instance data. This pointer is not used and shall be NULL.
 *
 * @return Returns 1 in case volume has been changed.
 */
static int32_t wm8960_input_vol_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,
                                      uint32_t * vol, void *instance_data)
{
    (void)(element);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    int32_t altered = 0;
    int status;

    if (set) {
        if (wm8960->mic_left2right) {
            // This offset is used since spk_range was shifted to start from 0
            vol[0] += WM8960_INPUT_VOL_MIN;
            altered = (vol[0] != (wm8960->input_volume[0] & WM8960_INPUT_VOL_CTRL_MASK));

            if (altered) {
                /* Clear volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_UPDATE_MASK, 0,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("clear volume update bit failed");
                    return 0;
                }
                /* Update Left channel volume and set volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_CTRL_MASK, vol[0],
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
                /* Set volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_UPDATE_MASK, WM8960_INPUT_VOL_UPDATE_MASK,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("set volume update bit failed");
                    return 0;
                }
                wm8960->input_volume[0] = vol[0];
            }
        } else {
            // This offset is used since spk_range was shifted to start from 0
            vol[0] += WM8960_INPUT_VOL_MIN;
            vol[1] += WM8960_INPUT_VOL_MIN;

            altered = ((vol[0] != (wm8960->input_volume[0] & WM8960_INPUT_VOL_CTRL_MASK)) ||
                       (vol[1] != (wm8960->input_volume[1] & WM8960_INPUT_VOL_CTRL_MASK)));

            if (altered) {
                /* Clear volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_UPDATE_MASK, 0,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("clear volume update bit failed");
                    return 0;
                }
                /* Update Left and Right channel volume and set volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_CTRL_MASK,
                                       vol[0], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_CTRL_MASK,
                                       vol[1], WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("volume update failed");
                    return 0;
                }
                /* Set volume update bit */
                status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_UPDATE_MASK, WM8960_INPUT_VOL_UPDATE_MASK,
                                       WM8960_UPDATE_AND_WRITE_REG);
                if (status != EOK) {
                    ado_error_fmt("set volume update bit failed");
                    return 0;
                }
                wm8960->input_volume[0] = vol[0];
                wm8960->input_volume[1] = vol[1];
            }
        }
    } else {
        /* Get saved volume */
        if (wm8960->mic_left2right) {
            vol[0] = (wm8960->input_volume[0] & WM8960_INPUT_VOL_CTRL_MASK) - WM8960_INPUT_VOL_MIN;
        } else {
            vol[0] = (wm8960->input_volume[0] & WM8960_INPUT_VOL_CTRL_MASK) - WM8960_INPUT_VOL_MIN;
            vol[1] = (wm8960->input_volume[1] & WM8960_INPUT_VOL_CTRL_MASK) - WM8960_INPUT_VOL_MIN;
        }
    }

    return (altered);
}

static void
wm8960_set_input_routing (MIXER_CONTEXT_T * wm8960, wm8960_mux_t route )
{
    uint32_t boost = 0;
    uint32_t sig_path_val = 0; /* Initialize to mute the mic input to the boost mixer */

    wm8960->input_mux = route;
    /* All inputs go through the input boost mixer to get to the ADC, so we use the mutes
     * on the input boost mixer to only pass the signal from the selected input
     */
    switch (wm8960->input_mux)
    {
        default:
        case INPUT_MUX_MIC:
            /* Unmute the Mic input to the boost mixer
             * Note: The values are the same for the left and right channel registers,
             *       so we can use this setting for both register updates
             */
            sig_path_val = WM8960_ADC_L_LMIC2B_MASK;

            /* Power up the Mic Gain (while still muted) */
            if (wm8960->mic_left2right) {
                wm8960_update(wm8960, WM8960_PWR_MGMT_3, WM8960_PWR_MGMT_3_LMIC | WM8960_PWR_MGMT_3_RMIC,
                              WM8960_PWR_MGMT_3_LMIC, WM8960_UPDATE_AND_WRITE_REG);
            } else {
                wm8960_update(wm8960, WM8960_PWR_MGMT_3, WM8960_PWR_MGMT_3_LMIC | WM8960_PWR_MGMT_3_RMIC,
                              WM8960_PWR_MGMT_3_LMIC | WM8960_PWR_MGMT_3_RMIC, WM8960_UPDATE_AND_WRITE_REG);
            }

            /* Mute both the line inputs to the boost mixer */
            boost = WM8960_INPUT_L_BOOST_LIN2MUTE | WM8960_INPUT_L_BOOST_LIN3MUTE;
            break;
        case INPUT_MUX_LINE2:
            /* The line boost is -12dB -> 6dB in 3dB increments
             * Add 12 and divide by 3 to convert the dB value to the step level. Offset by 1 as 0 is mute.
             */
            boost = (wm8960->line2_boost + 12) / 3 + 1;
            /* Unmute the line2 input and mute the line3 input to the boost mixer */
            boost = WM8960_INPUT_L_BOOST_LIN2BOOST(boost);
            break;
        case INPUT_MUX_LINE3:
            /* The line boost is -12dB -> 6dB in 3dB increments
             * Add 12 and divide by 3 to convert the dB value to the step level. Offset by 1 as 0 is mute.
             */
            boost = (wm8960->line3_boost + 12) / 3 + 1;
            /* Unmute the line3 input and mute the line2 input to the boost mixer */
            boost = WM8960_INPUT_L_BOOST_LIN3BOOST(boost);
            break;
    }

    wm8960_update(wm8960, WM8960_ADC_L_SIG_PATH, WM8960_ADC_L_LMIC2B_MASK, sig_path_val, WM8960_UPDATE_AND_WRITE_REG);
    if (!wm8960->mic_left2right) {
        wm8960_update(wm8960, WM8960_ADC_R_SIG_PATH, WM8960_ADC_R_RMIC2B_MASK, sig_path_val, WM8960_UPDATE_AND_WRITE_REG);
    }

    if (sig_path_val == 0)
    {
        /* Power down the Mic Gain (after we have muted) */
        wm8960_update(wm8960, WM8960_PWR_MGMT_3, WM8960_PWR_MGMT_3_LMIC | WM8960_PWR_MGMT_3_RMIC, 0, WM8960_UPDATE_AND_WRITE_REG);
    }

    /* Apply the input line boost value to both left and right channels */
    wm8960_update(wm8960, WM8960_INPUT_L_BOOST_MIX, 0xff, boost, WM8960_UPDATE_AND_WRITE_REG);
    wm8960_update(wm8960, WM8960_INPUT_R_BOOST_MIX, 0xff, boost, WM8960_UPDATE_AND_WRITE_REG);
}

/* Input mux control */
static int32_t
wm8960_mux_control(MIXER_CONTEXT_T * wm8960, ado_mixer_delement_t * element, uint8_t set,ado_mixer_delement_t ** inelements, void *instance_data)
{
    int32_t altered = 0;
    wm8960_mux_t route;

    if (set)
    {
        if(inelements[0] == wm8960->mic)
            route = INPUT_MUX_MIC;
        else if (inelements[0] == wm8960->line2)
            route = INPUT_MUX_LINE2;
        else
            route = INPUT_MUX_LINE3;

        altered = wm8960->input_mux != route;

        if (altered)
        {
            wm8960_set_input_routing (wm8960, route );
        }
    }
    else
    {
        switch (wm8960->input_mux)
        {
            case INPUT_MUX_MIC:
                inelements[0] = inelements[1] = wm8960->mic;
                break;
            case INPUT_MUX_LINE2:
                inelements[0] = inelements[1] = wm8960->line2;
                break;
            case INPUT_MUX_LINE3:
                inelements[0] = inelements[1] = wm8960->line3;
                break;
            default:
                break;
        }
    }

    return (altered);
}

/**
 * Determine if the mic-bias is enabled.
 *
 * @param wm8960 Mixer context.
 * @param dswitch Opaque parameter.
 * @param cswitch Opaque parameter.
 * @param instance_data Optional data. By default NULL.
 *
 * @return Execution status.
 */
static int32_t wm8960_micbias_get(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t data;

    data = wm8960->regs[WM8960_PWR_MGMT_1] & WM8960_PWR_MGMT_1_MICB_MASK;
    cswitch->type = SND_SW_TYPE_BOOLEAN;
    cswitch->value.enable = data ? 1 : 0;

    return EOK;
}
/**
 * Set the micbias to a new value if that has been requested.
 *
 * @param wm8960 Mixer context.
 * @param dswitch Opaque parameter.
 * @param cswitch Opaque parameter.
 * @param instance_data Optional data. By default NULL.
 *
 * @return Returns 1 when altered.
 */
static int32_t wm8960_micbias_set(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t bias;
    int32_t altered = 0;
    int status;

    bias = wm8960->regs[WM8960_PWR_MGMT_1] & WM8960_PWR_MGMT_1_MICB_MASK;
    altered = (cswitch->value.enable != (bias ? 1 : 0));
    if (altered) {
        status = wm8960_update(wm8960, WM8960_PWR_MGMT_1, WM8960_PWR_MGMT_1_MICB_MASK,
                               cswitch->value.enable ? WM8960_PWR_MGMT_1_MICB_MASK : 0, WM8960_UPDATE_AND_WRITE_REG);

        if (status != EOK) {
            ado_error_fmt("micbias update failed");
            return 0;
        }
    }
    return (altered);
}

static int32_t wm8960_loopback_get(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t data;

    data = wm8960->regs[WM8960_AUDIO_INTERF_1] & WM8960_AUDIO_INTERF_1_LOOPBACK;
    cswitch->type = SND_SW_TYPE_BOOLEAN;
    cswitch->value.enable = data ? 1 : 0;

    return EOK;
}

static int32_t wm8960_loopback_set(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t loopback;
    int32_t altered = 0;
    int status;

    loopback = wm8960->regs[WM8960_AUDIO_INTERF_1] & WM8960_AUDIO_INTERF_1_LOOPBACK;
    altered = (cswitch->value.enable != (loopback ? 1 : 0));
    if (altered) {
        status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_1, WM8960_AUDIO_INTERF_1_LOOPBACK,
                               cswitch->value.enable ? WM8960_AUDIO_INTERF_1_LOOPBACK : 0, WM8960_UPDATE_AND_WRITE_REG);

        if (status != EOK) {
            ado_error_fmt("loopback update failed");
            return 0;
        }
    }
    return (altered);
}

static int32_t wm8960_analog_loopback_get(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t data;

    data = wm8960->regs[WM8960_BYPASS_L] & WM8960_BYPASS_LB2LO;
    cswitch->type = SND_SW_TYPE_BOOLEAN;
    cswitch->value.enable = data ? 1 : 0;

    return EOK;
}

static int32_t wm8960_analog_loopback_set(MIXER_CONTEXT_T * wm8960, ado_dswitch_t * dswitch, snd_switch_t * cswitch,
                                  void *instance_data)
{
    (void)(dswitch);  /* Currently not used */
    (void)(instance_data); /* Currently not used */
    uint32_t loopback;
    int32_t altered = 0;
    int status;

    loopback = wm8960->regs[WM8960_BYPASS_L] & WM8960_BYPASS_LB2LO;
    altered = (cswitch->value.enable != (loopback ? 1 : 0));
    if (altered) {
        status = wm8960_update(wm8960, WM8960_BYPASS_L, WM8960_BYPASS_LB2LO,
                               cswitch->value.enable ? WM8960_BYPASS_LB2LO : 0, WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            ado_error_fmt("Left loopback update failed");
            return 0;
        }
        status = wm8960_update(wm8960, WM8960_BYPASS_R, WM8960_BYPASS_RB2RO,
                               cswitch->value.enable ? WM8960_BYPASS_RB2RO : 0, WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            ado_error_fmt("Right loopback update failed");
            return 0;
        }
    }
    return (altered);
}

/**
 * Resets WM8960 codec
 *
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
static int wm8960_reset(MIXER_CONTEXT_T * wm8960)
{
    int status;
    /* Data written to reset registers isn't really important. */
    status = wm8960_write(wm8960, WM8960_SOFTWARE_RESET, 0);
    if (status != EOK) {
        ado_error_fmt("codec reset failed");
        return status;
    }
    /* Initialize register buffer to default reset values from RM */
    wm8960->regs[WM8960_INPUT_VOL_L] = WM8960_INPUT_VOL_DEFAULT; // default volume
    wm8960->regs[WM8960_INPUT_VOL_R] = WM8960_INPUT_VOL_DEFAULT; // default volume
    wm8960->input_volume[0] = WM8960_INPUT_VOL_DEFAULT;
    wm8960->input_volume[1] = WM8960_INPUT_VOL_DEFAULT;

    wm8960->regs[WM8960_HPOUT_VOL_L] = 0;
    wm8960->regs[WM8960_HPOUT_VOL_R] = 0;
    wm8960->hp_volume[0] = 0;
    wm8960->hp_volume[1] = 0;

    wm8960->regs[WM8960_CLOCKING_1] = 0;
    wm8960->regs[WM8960_ADCDAC_CTL_1] = WM8960_ADCDAC_CTL_1_DACMUTE_MASK;
    wm8960->regs[WM8960_ADCDAC_CTL_2] = 0;
    wm8960->regs[WM8960_AUDIO_INTERF_0] = WM8960_AUDIO_INTERF_0_WL_24 | WM8960_AUDIO_INTERF_0_FORMAT_I2S;
    wm8960->regs[WM8960_CLOCKING_2] = WM8960_CLOCKING_2_DCLKDIV_MASK;
    wm8960->regs[WM8960_AUDIO_INTERF_1] = 0;

    /* Mute DAC Output */
    if ((status = wm8960_write(wm8960, WM8960_DAC_VOL_L, 0)) != EOK) {
        return status;
    }
    if ((status = wm8960_write(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK | 0)) != EOK) {
        return status;
    }
    /* The VOL_UPDATE bit is not sticky, so clear it in our local copy */
    if ((status = wm8960_update(wm8960, WM8960_DAC_VOL_R, WM8960_DAC_VOL_UPDATE_MASK, 0, WM8960_UPDATE_ONLY)) != EOK) {
        return status;
    }

    wm8960->regs[WM8960_DAC_3D_CTL] = 0;
    wm8960->regs[WM8960_ALC_CTL_1] = WM8960_ALC_CTL_1_DEFAULT;
    wm8960->regs[WM8960_ALC_CTL_2] = 0;
    wm8960->regs[WM8960_ALC_CTL_3] = WM8960_ALC_CTL_3_DECAY_192MS | WM8960_ALC_CTL_3_ATTACK_24MS;
    wm8960->regs[WM8960_NOISE_GATE] = 0;

    wm8960->regs[WM8960_ADC_L_VOL] = WM8960_ADC_VOL_DEFAULT;  // default volume
    wm8960->regs[WM8960_ADC_R_VOL] = WM8960_ADC_VOL_DEFAULT;  //default volume
    wm8960->adc_volume[0] = WM8960_ADC_VOL_DEFAULT;
    wm8960->adc_volume[1] = WM8960_ADC_VOL_DEFAULT;

    wm8960->regs[WM8960_ADDITIONAL_CTRL_1] = WM8960_ADDITIONAL_CTRL_1_TSDEN_MASK | WM8960_ADDITIONAL_CTRL_1_VSEL_MASK;
    wm8960->regs[WM8960_ADDITIONAL_CTRL_2] = 0;
    wm8960->regs[WM8960_PWR_MGMT_1] = 0;
    wm8960->regs[WM8960_PWR_MGMT_2] = 0;
    wm8960->regs[WM8960_ADDITIONAL_CTRL_3] = 0;
    wm8960->regs[WM8960_ANTI_POP_1] = 0;
    wm8960->regs[WM8960_ANTI_POP_2] = 0;
    wm8960->regs[WM8960_ADC_L_SIG_PATH] = WM8960_ADC_L_SIG_PATH_LMN1_MASK;
    wm8960->regs[WM8960_ADC_R_SIG_PATH] = WM8960_ADC_R_SIG_PATH_RMN1_MASK;
    wm8960->regs[WM8960_OUT_L_MIX] = WM8960_OUT_MIX_DEFAULT; // default volume
    wm8960->regs[WM8960_OUT_R_MIX] = WM8960_OUT_MIX_DEFAULT; // default volume
    wm8960->regs[WM8960_MONO_OUT_MIX_1] = 0;
    wm8960->regs[WM8960_MONO_OUT_MIX_2] = 0;
    wm8960->regs[WM8960_SPKOUT_VOL_L] = 0;
    wm8960->regs[WM8960_SPKOUT_VOL_R] = 0;
    wm8960->spk_volume[0] = 0;
    wm8960->spk_volume[1] = 0;

    /* Mute SPK Output */
    if ((status = wm8960_write(wm8960, WM8960_SPKOUT_VOL_L, 0)) != EOK) {
        return status;
    }
    if ((status = wm8960_write(wm8960, WM8960_SPKOUT_VOL_L, WM8960_SPKOUT_VOL_UPDATE_MASK | 0)) != EOK) {
        return status;
    }
    /* The VOL_UPDATE bit is not sticky, so clear it in our local copy */
    if ((status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_L, WM8960_SPKOUT_VOL_UPDATE_MASK, 0, WM8960_UPDATE_ONLY)) != EOK) {
        return status;
    }

    wm8960->regs[WM8960_OUT_3_VOL] = WM8960_OUT_3_VOL_MOUTVOL_MASK;
    wm8960->regs[WM8960_INPUT_L_BOOST_MIX] = 0;               /* IN2 & IN3 Boost Muted */
    wm8960->regs[WM8960_INPUT_R_BOOST_MIX] = 0;               /* IN2 & IN3 Boost Muted */

    /* Initialize the Analog loopback volume to 0dB and disabled */
    if ((status = wm8960_write(wm8960, WM8960_BYPASS_L, WM8960_BYPASS_0DB)) != EOK) {
        return status;
    }
    if ((status = wm8960_write(wm8960, WM8960_BYPASS_R, WM8960_BYPASS_0DB)) != EOK) {
        return status;
    }

    wm8960->regs[WM8960_PWR_MGMT_3] = 0;
    wm8960->regs[WM8960_ADDITIONAL_CTRL_4] = WM8960_ADDITIONAL_CTRL_4_TSENSEN_MASK;
    wm8960->regs[WM8960_CLASSD_CTRL_1] = WM8960_CLASSD_CTRL_1_DEFAULT; // Reserved bits 5:0
    wm8960->regs[WM8960_CLASSD_CTRL_2] = WM8960_CLASSD_CTRL_2_DEFAULT; // Reserved bits 8:6
    wm8960->regs[WM8960_PLL_1] = WM8960_PLL_1_PLLN_DEFAULT;
    wm8960->regs[WM8960_PLL_2] = WM8960_PLL_2_PLLK_H_DEFAULT;
    wm8960->regs[WM8960_PLL_3] = WM8960_PLL_3_PLLK_L_DEFAULT;
    wm8960->regs[WM8960_PLL_4] = WM8960_PLL_4_PLLK_LL_DEFAULT;

    /* Set default codec sample. Driver will overwrite it later */
    wm8960->sample_rate = 48000;
    return (EOK);
}

/**
 * Initializes WM8960 subsystems like power, gains, volume, mute etc.
 *
 * @param mx Audio driver context.
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
static int wm8960_init_subsystems(MIXER_CONTEXT_T * wm8960)
{
    uint32_t vol[2];
    int status;
    uint32_t val;

    /* Master Clock enabled */
    val = 0;
    status = wm8960_write(wm8960, WM8960_PWR_MGMT_1, val);
    if (status != EOK) {
        return status;
    }
    delay(1); /* Ensure 1ms delay between enable of master clock and everything else */

    /* Set Class D amp AC and DC Gain */
#define WM8960_GAIN (WM8960_SPKVDD * 100) / WM8960_AVDD

#if WM8960_GAIN >= 180
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1P80 | WM8960_CLASSD_CTRL_2_DCGAIN_1P80
#elif WM8960_GAIN >= 167
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1P67 | WM8960_CLASSD_CTRL_2_DCGAIN_1P67
#elif WM8960_GAIN >= 152
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1P52 | WM8960_CLASSD_CTRL_2_DCGAIN_1P52
#elif WM8960_GAIN >= 140
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1P40 | WM8960_CLASSD_CTRL_2_DCGAIN_1P40
#elif WM8960_GAIN >= 127
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1P27 | WM8960_CLASSD_CTRL_2_DCGAIN_1P27
#else
#define WM8960_GAIN_VAL WM8960_CLASSD_CTRL_2_ACGAIN_1 | WM8960_CLASSD_CTRL_2_DCGAIN_1
#endif

    /* Anti-Pop */
    val = ANTI_POP_1_POBCTRL | ANTI_POP_1_BUFDCOPEN | ANTI_POP_1_BUFIOEN | ANTI_POP_1_SOFT_ST;
    status = wm8960_write(wm8960, WM8960_ANTI_POP_1, val);
    if (status != EOK) {
        return status;
    }

    /* Power on VMID */
    val = WM8960_PWR_MGMT_1_VMIDSEL_50k | WM8960_PWR_MGMT_1_AINL_MASK | WM8960_PWR_MGMT_1_AINR_MASK | WM8960_PWR_MGMT_1_ADCL_MASK | WM8960_PWR_MGMT_1_ADCR_MASK;
    if (wm8960->micbias) {
        val |= WM8960_PWR_MGMT_1_MICB_MASK;
    }
    status = wm8960_update(wm8960, WM8960_PWR_MGMT_1, val, val, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }
    /* Delay for VMID ramp (POP suppression) */
    delay(700);

    /* Enable VREF */
    val = WM8960_PWR_MGMT_1_VREF_MASK;
    status = wm8960_update(wm8960, WM8960_PWR_MGMT_1, val, val, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }

    status = wm8960_update(wm8960, WM8960_CLASSD_CTRL_2,
                           WM8960_CLASSD_CTRL_2_DCGAIN_MASK | WM8960_CLASSD_CTRL_2_ACGAIN_MASK, WM8960_GAIN_VAL,
                           WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }

    /* Optimize Analog bias */
#if WM8960_AVDD <= WM8960_ANALOG_BIAS_THRESH
    status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_1,
                  WM8960_ADDITIONAL_CTRL_1_VSEL_MASK,
                  WM8960_ADDITIONAL_CTRL_1_VSEL_HBIAS,
                  WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }
#endif

    /* Enable power for the Output Mixer */
    val = WM8960_PWR_MGMT_3_LOMIX | WM8960_PWR_MGMT_3_ROMIX;
    if (wm8960->input_enable & MIC_ENABLED) {
        /* Input PGA is only used for the Mic (input1), so only power
         * it up if the mic is enabled
         */
        val |= WM8960_PWR_MGMT_3_LMIC | WM8960_PWR_MGMT_3_RMIC;
    }
    if ((status = wm8960_write(wm8960, WM8960_PWR_MGMT_3, val)) != EOK) {
        return status;
    }

    /* Set routing on left and right output mix and default volume 0 dB */
    status = wm8960_write(wm8960, WM8960_OUT_L_MIX, WM8960_OUT_MIX_DAC2MIX);
    if (status != EOK) {
        return status;
    }
    status = wm8960_write(wm8960, WM8960_OUT_R_MIX, WM8960_OUT_MIX_DAC2MIX);
    if (status != EOK) {
        return status;
    }

    if (wm8960->output_enable & HEADPHONE_ENABLE) {
        /* Enable Zero crossing and unmute Headphone out, set 0 dB */
        status = wm8960_write(wm8960, WM8960_HPOUT_VOL_L, WM8960_HPOUT_VOL_ZERO_MASK);
        if (status != EOK) {
            return status;
        }
        status = wm8960_write(wm8960, WM8960_HPOUT_VOL_R, WM8960_HPOUT_VOL_ZERO_MASK);
        if (status != EOK) {
            return status;
        }
        wm8960->hp_mute = 0;

        /* Set volume to 0 dB. Step is 1 dB */
        vol[0] = hp_range[0].max - (hp_range[0].max_dB / 100);
        vol[1] = hp_range[1].max - (hp_range[1].max_dB / 100);
        wm8960_hp_vol_control(wm8960, NULL, 1, vol, NULL);
    }

    /* Set volume to 0 dB */
    vol[0] = WM8960_DAC_VOLUME_DEFAULT - WM8960_DAC_VOL_MIN;
    vol[1] = WM8960_DAC_VOLUME_DEFAULT - WM8960_DAC_VOL_MIN;
    wm8960_dac_vol_control(wm8960, NULL, 1, vol, NULL);

    /* Unmute DAC */
    status = wm8960_write(wm8960, WM8960_ADCDAC_CTL_1, 0);
    if (status != EOK) {
        return status;
    }
    wm8960->dac_mute = 0;

    /* Configure default input routing */
    if (wm8960->input_enable & MIC_ENABLED)
        wm8960_set_input_routing (wm8960, INPUT_MUX_MIC );
    else if (wm8960->input_enable & LINE2_ENABLED)
        wm8960_set_input_routing (wm8960, INPUT_MUX_LINE2 );
    else if (wm8960->input_enable & LINE3_ENABLED)
        wm8960_set_input_routing (wm8960, INPUT_MUX_LINE3 );

    /* Un-mute and enable Zero Cross, volume to 0dB */
    status = wm8960_update(wm8960, WM8960_INPUT_VOL_L, WM8960_INPUT_VOL_ZERO_MASK | WM8960_INPUT_VOL_MUTE_MASK,
                           WM8960_INPUT_VOL_ZERO_MASK | WM8960_INPUT_VOL_UPDATE_MASK, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }
    status = wm8960_update(wm8960, WM8960_INPUT_VOL_R, WM8960_INPUT_VOL_ZERO_MASK | WM8960_INPUT_VOL_MUTE_MASK,
                           WM8960_INPUT_VOL_ZERO_MASK | WM8960_INPUT_VOL_UPDATE_MASK, WM8960_UPDATE_AND_WRITE_REG);

    if (status != EOK) {
        return status;
    }
    wm8960->input_mute = 0;
    wm8960->adc_mute = 0;

    /* Enable slow timer for Zero cross timeout and Headphone detect debounce */
    status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_1, WM8960_ADDITIONAL_CTRL_1_TOEN_MASK,
                           WM8960_ADDITIONAL_CTRL_1_TOEN_MASK, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }

   /* Capless mode, OUT3 is used as VMID for headphones rather then as a mono (mix of L&R) output */
    status = wm8960_write(wm8960, WM8960_ADDITIONAL_CTRL_3, OUT3CAP);
    if (status != EOK) {
        return status;
    }

    /* Mic Bias 0.65 * AVDD */
    status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_4, WM8960_ADDITIONAL_CTRL_4_MBSEL,
                           WM8960_ADDITIONAL_CTRL_4_MBSEL, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
       return status;
    }

   if (wm8960->hpsel != JD_NC)
   {
      uint32_t mask = WM8960_ADDITIONAL_CTRL_4_HPSEL_MASK;

      switch (wm8960->hpsel)
      {
          case JD_GPIO1:
              /* When setting the headphone select to use GPIO1 as it input, we must also
               * set the GPIO function as Jack detect input
               */
              val = WM8960_ADDITIONAL_CTRL_4_HPSEL_GPIO1 | WM8960_ADDITIONAL_CTRL_4_GPIOSEL_JDI;
              mask |= WM8960_ADDITIONAL_CTRL_4_GPIOSEL_MASK;

              /* The ADCLRC pin is shared with the GPIO1 function. If using that pin for Jack detect
               * we must configure that pin to function as a GPIO.
               * NOTE: This means that the ADC will used the DACLRC as its clock source
               */
              status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_1, WM8960_AUDIO_INTERF_1_ALRCGPIO_MASK,
                                     WM8960_AUDIO_INTERF_1_ALRCGPIO, WM8960_UPDATE_AND_WRITE_REG);
              if (status != EOK) {
                  return status;
              }
              break;
         case JD_2:
              val = WM8960_ADDITIONAL_CTRL_4_HPSEL_JD2;
              break;
         case JD_3:
              val = WM8960_ADDITIONAL_CTRL_4_HPSEL_JD3;
              break;
         case JD_NC:
              /* Will never get here */
              break;
      }

      status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_4, mask, val, WM8960_UPDATE_AND_WRITE_REG);
       if (status != EOK) {
          return status;
       }

       /* HP detect pin is high with headphone jack connected */
       status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_2,
                              WM8960_ADDITIONAL_CTRL_2_HPSWEN_MASK | WM8960_ADDITIONAL_CTRL_2_HPSWPOL_MASK,
                              WM8960_ADDITIONAL_CTRL_2_HPSWEN_MASK, WM8960_UPDATE_AND_WRITE_REG);
       if (status != EOK) {
           return status;
       }
   }
   else
   {
       /* Ensure HPSEL is disabled */
       status = wm8960_update(wm8960, WM8960_ADDITIONAL_CTRL_2,
                              WM8960_ADDITIONAL_CTRL_2_HPSWEN_MASK | WM8960_ADDITIONAL_CTRL_2_HPSWPOL_MASK,
                              0,  WM8960_UPDATE_AND_WRITE_REG);
       if (status != EOK) {
           return status;
       }
   }

    if (wm8960->output_enable & SPEAKER_ENABLE) {
        /* Enable Class D amp */
        status = wm8960_update(wm8960, WM8960_CLASSD_CTRL_1, WM8960_CLASSD_CTRL_1_SPKEN_MASK,
                               WM8960_CLASSD_CTRL_1_SPKEN_MASK, WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            return status;
        }

        /* Enable Zero cross for Speakers */
        status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_L, WM8960_SPKOUT_VOL_ZERO_MASK, WM8960_SPKOUT_VOL_ZERO_MASK,
                               WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            return status;
        }
        status = wm8960_update(wm8960, WM8960_SPKOUT_VOL_R, WM8960_SPKOUT_VOL_ZERO_MASK, WM8960_SPKOUT_VOL_ZERO_MASK,
                               WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            return status;
        }
        /* Set volume to 0 dB. Step is 1 dB */
        vol[0] = spk_range[0].max - (spk_range[0].max_dB / 100);
        vol[1] = spk_range[1].max - (spk_range[1].max_dB / 100);
        /* Unmute Speakers */
        wm8960_spk_vol_control(wm8960, NULL, 1, vol, NULL);
        wm8960->spk_mute = 0;
    }

    /* Power on DACL, DACR */
    val = WM8960_PWR_MGMT_2_DACL | WM8960_PWR_MGMT_2_DACR;
    if (wm8960->output_enable & HEADPHONE_ENABLE) {
        /* Power on Headphone/Line out */
        val |= (WM8960_PWR_MGMT_2_LOUT1 | WM8960_PWR_MGMT_2_ROUT1);
    }
    if (wm8960->output_enable & SPEAKER_ENABLE) {
        /* Power on SPKL, SPKR */
        val |= (WM8960_PWR_MGMT_2_SPKL | WM8960_PWR_MGMT_2_SPKR);
    }
    status = wm8960_update(wm8960, WM8960_PWR_MGMT_2, val, val, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }

    /* Disable Anti-Pop */
    val = ANTI_POP_1_POBCTRL | ANTI_POP_1_BUFDCOPEN | ANTI_POP_1_SOFT_ST;
    status = wm8960_update(wm8960, WM8960_ANTI_POP_1, val, 0, WM8960_UPDATE_AND_WRITE_REG);
    if (status != EOK) {
        return status;
    }

    return EOK;
}

/**
 * Initializes WM8960 audio data I2S interface.
 *
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
static int wm8960_init_interface(MIXER_CONTEXT_T *wm8960)
{
    int status;
    /* Set interface format, configure as Master or Slave */
    status = wm8960_write(wm8960, WM8960_AUDIO_INTERF_0,
                          ((wm8960->format == WM8960_I2S) ? WM8960_AUDIO_INTERF_0_FORMAT_I2S : WM8960_AUDIO_INTERF_0_FORMAT_DSP) |
                          ((wm8960->clk_mode == WM8960_MASTER) ? WM8960_AUDIO_INTERF_0_MS_MASK : 0));
    if (status != EOK) {
        return status;
    }

    /* Set sample size */
    switch (wm8960->sample_size) {
        case 16:
            status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_0, WM8960_AUDIO_INTERF_0_WL_MASK,
                                   WM8960_AUDIO_INTERF_0_WL_16, WM8960_UPDATE_AND_WRITE_REG);
            break;
        case 24:
            status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_0, WM8960_AUDIO_INTERF_0_WL_MASK,
                                   WM8960_AUDIO_INTERF_0_WL_24, WM8960_UPDATE_AND_WRITE_REG);
            break;
        case 32:
        default:
            status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_0, WM8960_AUDIO_INTERF_0_WL_MASK,
                                   WM8960_AUDIO_INTERF_0_WL_32, WM8960_UPDATE_AND_WRITE_REG);
            break;
    }
    if (status != EOK) {
        return status;
    }

    /* Use DAC LRCK pin for ADC
     * This configuration will also be applied if Jack detection is configured to use GPIO1
     * since ADCLRC and GPIO1 share the same pin.
     */
    if (wm8960->use_dac_lrck) {
        status = wm8960_update(wm8960, WM8960_AUDIO_INTERF_1, WM8960_AUDIO_INTERF_1_ALRCGPIO_MASK,
                               WM8960_AUDIO_INTERF_1_ALRCGPIO, WM8960_UPDATE_AND_WRITE_REG);
        if (status != EOK) {
            return status;
        }
    }

    return EOK;
}

/**
 * Calculates WM8960 ADC and DAC divider according to required sample rate.
 *
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
int wm8960_set_rate(MIXER_CONTEXT_T * wm8960, uint32_t sample_rate)
{

    int err = 0;
    int best_err = 0;
    uint32_t best_div;
    uint32_t div;
    uint32_t begin;
    int status = EOK;
    uint32_t sysclk_div = 0;
    uint32_t bclk_sample_size;
    uint32_t target_sysclk;

    wm8960->sample_rate = sample_rate;

    if (wm8960->sample_rate == 0 || wm8960->mclk == 0) {
        return EINVAL;
    }

    /* sysclk should be as close as possible to wm8960->sample_rate * 256 */
    target_sysclk = wm8960->sample_rate * 256;
    if (abs(wm8960->mclk - target_sysclk) <= abs((wm8960->mclk/2) - target_sysclk)) {
        wm8960->sysclk = wm8960->mclk;
    } else {
        /* sysclk can divide only by 2 */
        wm8960->sysclk = wm8960->mclk / 2;
        sysclk_div = 2 << 1;
    }

    div = wm8960->sysclk / wm8960->sample_rate / 256;

    // calculate more accurately
    if (div) {
        div *= 10;
    } else {
        /* div can't be zero */
        ado_error_fmt("Wrong div %u", div);
        return EINVAL;
    }

    /* Start with smaller div */
    if (div > 10) {
        div -= 10;
    }

    begin = div;
    best_div = div;
    do {

        err = wm8960->sample_rate - ((wm8960->sysclk * 10) / (div * 256));

        if (abs(err) <= abs(best_err)) {
            best_err = err;
            best_div = div;
        }

        if ((div == 10) || (div == 40) || (div % 10)) {
            div += 5;
        } else {
            div += 10;
        }

    } while (div <= (begin + 20) && (div <= 60));

    ado_debug(DB_LVL_MIXER, "calculated div=%u.%u with err=%i", best_div / 10, best_div % 10, best_err);

    switch (best_div) {
        case 10:
            /* for div = 1.0 writes 0 to the register */
            best_div -= 10;
            /* fallthrough */
        case 20: /* div = 2.0 writes 2 */
        case 30: /* div = 3.0 writes 3 */
        case 40: /* div = 4.0 writes 4 */
        case 60: /* div = 6.0 writes 6 */
            if (best_div) {
                best_div /= 10;
            }
            status = wm8960_update(wm8960, WM8960_CLOCKING_1, WM8960_CLOCKING_1_ADCDIV_MASK | WM8960_CLOCKING_1_DACDIV_MASK | WM8960_CLOCKING_1_SYSCLKDIV_MASK,
                                   best_div << WM8960_CLOCKING_1_ADCDIV_SHIFT | best_div << WM8960_CLOCKING_1_DACDIV_SHIFT | sysclk_div,
                                   WM8960_UPDATE_AND_WRITE_REG);
            break;
        case 15: /* div = 1.5 writes 1 */
        case 55: /* div = 5.5 writes 5 */
            best_div -= 5;
            best_div /= 10;
            status = wm8960_update(wm8960, WM8960_CLOCKING_1, WM8960_CLOCKING_1_ADCDIV_MASK | WM8960_CLOCKING_1_DACDIV_MASK | WM8960_CLOCKING_1_SYSCLKDIV_MASK,
                                   best_div << WM8960_CLOCKING_1_ADCDIV_SHIFT | best_div << WM8960_CLOCKING_1_DACDIV_SHIFT | sysclk_div,
                                   WM8960_UPDATE_AND_WRITE_REG);
            break;
        default:
            ado_error_fmt("Wrong div %u.%u", best_div / 10, best_div % 10);
            return EINVAL;
            break;
    }

    if (wm8960->clk_mode == WM8960_MASTER) {
        /* BCLK for Master mode */
        bclk_sample_size = wm8960->sample_size;

        div = (wm8960->sysclk * 10) / (wm8960->sample_rate * bclk_sample_size * 2);
        switch (div) {
            case 0:
                break;
            case 15:
            case 20:
            case 30:
            case 40:
            case 60:
                div = div / 10;
                break;
            case 80:
                div = 7;
                break;
            case 110:
                div = 8;
                break;
            case 120:
                div = 9;
                break;
            case 160:
                div = 10;
                break;
            case 220:
                div = 11;
                break;
            case 240:
                div = 12;
                break;
            case 320:
                div = 0xff;
                break;
            default:
                if (div > 40 && div < 60) {
                    div = 5;
                }
                break;
        }
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_BCLKDIV_MASK, div, WM8960_UPDATE_AND_WRITE_REG);
    }
    return status;
}

/**
 * Initializes WM8960 internal clocks.
 *
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
static int wm8960_init_clocks(MIXER_CONTEXT_T * wm8960)
{

    uint32_t div;
    int status;

    status = wm8960_set_rate(wm8960, wm8960->sample_rate);

    if (status != EOK) {
        return status;
    }

    /* Calculate Class D amp divider to divide SYSCLK to 700 - 800 kHz */
    div = wm8960->sysclk / 80000;

    if (div > 160) {
        ado_error_fmt("Wrong div %u", div);
        return EINVAL;
    } else if (div > 120) {
        /* set div to 16 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_16,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 16;
    } else if (div > 80) {
        /* set div to 12 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_12,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 12;
    } else if (div > 60) {
        /* set div to 8 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_8,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 8;
    } else if (div > 40) {
        /* set div to 6 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_6,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 6;
    } else if (div > 30) {
        /* set div to 4 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_4,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 4;
    } else if (div > 20) {
        /* set div to 3 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_3,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 3;
    } else {
        /* set div to 2 */
        status = wm8960_update(wm8960, WM8960_CLOCKING_2, WM8960_CLOCKING_2_DCLKDIV_MASK, WM8960_CLOCKING_2_DCLKDIV_2,
                           WM8960_UPDATE_AND_WRITE_REG);
        div = 2;
    }

    if (status != EOK) {
        return status;
    }

    /* Double check Class D amp. calculated clock */
    div = wm8960->sysclk / div;

    if ((div < 700000U) || (div > 800000U)) {
        ado_debug(DB_LVL_MIXER, "Warning Class D amp. clock is %u", div);
    }

    return EOK;
}

/**
 * Destroys WM8960 context and Power downs the codec.
 *
 * @param wm8960 Mixer context.
 *
 * @return Execution status.
 */
static int wm8960_destroy(MIXER_CONTEXT_T * wm8960)
{
    uint32_t val;
    int status;
    /* Anti-Pop */
    val = ANTI_POP_1_POBCTRL | ANTI_POP_1_BUFDCOPEN | ANTI_POP_1_SOFT_ST;
    status = wm8960_write(wm8960, WM8960_ANTI_POP_1, val);
    if (status) {
        ado_error_fmt("Setting anti pop failed");
    }
    /* Power down analog Outputs */
    status = wm8960_write(wm8960, WM8960_PWR_MGMT_2, 0);
    if (status) {
        ado_error_fmt("Power down analog outputs failed");
    }
    /* Disable VREF */
    status = wm8960_update(wm8960, WM8960_PWR_MGMT_1, WM8960_PWR_MGMT_1_VREF_MASK, 0, WM8960_UPDATE_AND_WRITE_REG);
    if (status) {
         ado_error_fmt("Power down vref failed");
    }
    delay(100);
    /* Disable Anti Pop */
    status = wm8960_write(wm8960, WM8960_ANTI_POP_1, ANTI_POP_1_BUFIOEN);
    if (status) {
         ado_error_fmt("Disabling anti pop failed");
    }
    /* Power down the rest of the codec */
    status = wm8960_write(wm8960, WM8960_PWR_MGMT_1, 0);
    if (status) {
         ado_error_fmt("Codec power down 1 failed");
    }
    status = wm8960_write(wm8960, WM8960_CLASSD_CTRL_1, 0);
    if (status) {
         ado_error_fmt("Class D power down failed");
    }
    status = wm8960_write(wm8960, WM8960_PWR_MGMT_3, 0);
    if (status) {
         ado_error_fmt("Codec power down 2 failed");
    }
    /* disconnect connect between SoC and codec */
    if (wm8960->i2c_fd != -1) {
        close(wm8960->i2c_fd);
    }

    ado_debug(DB_LVL_MIXER, "Destroying WM8960 mixer");
    ado_free(wm8960);

    return (0);
}

/**
 * Builds ado mixer according to WM8960 capabilities.
 *
 * @param wm8960 Mixer context.
 * @param mixer A pointer to the ado_mixer_t structure for the mixer.
 *
 * @return Execution status.
 */
static int32_t build_wm8960_mixer(MIXER_CONTEXT_T * wm8960, ado_mixer_t * mixer)
{
    /* DAC Master Output elements */
    ado_mixer_delement_t *master_mute = NULL, *master_vol = NULL, *master_io = NULL;

    /* Input elements */
    ado_mixer_delement_t *adc_mute = NULL, *adc_vol = NULL, *adc_pcm = NULL, *prev_elem = NULL, *input_mux = NULL;

    /* Channel Map */
    const snd_pcm_chmap_t *chmap = ado_pcm_get_default_chmap(2);

    /* ################# */
    /*  PLAYBACK GROUPS  */
    /* ################# */
    /* WM8960: Line Out Output */
    if (wm8960->output_enable & SPEAKER_ENABLE) {
        ado_mixer_delement_t *spk_mute = NULL, *spk_vol = NULL, *spk_io = NULL, *spk_pcm = NULL;

        if ((spk_pcm = ado_mixer_element_pcm1(mixer, SND_MIXER_ELEMENT_PLAYBACK, SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0])) == NULL) {
            return (-1);
        }

        if ((spk_mute = ado_mixer_element_sw1(mixer, "Speaker Mute", 2, wm8960_spk_mute_control, NULL, NULL)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, spk_pcm, spk_mute) != 0) {
            return (-1);
        }

        if ((spk_vol = ado_mixer_element_volume1(mixer, "Speaker Volume", 2, spk_range, wm8960_spk_vol_control, NULL, NULL)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, spk_mute, spk_vol) != 0) {
            return (-1);
        }

        if ((spk_io = ado_mixer_element_io(mixer, "Speaker Out", SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, spk_vol, spk_io) != 0) {
            return (-1);
        }

        if (ado_mixer_playback_group_create(mixer, SND_MIXER_FRONT_OUT, chmap, spk_vol, spk_mute) == NULL) {
            return (-1);
        }
    }

    /* WM8960: Headphone Output */
    if (wm8960->output_enable & HEADPHONE_ENABLE) {
        ado_mixer_delement_t *hp_mute = NULL, *hp_vol = NULL, *hp_io = NULL;

        if (ado_mixer_element_pcm1(mixer, SND_MIXER_ELEMENT_PLAYBACK, SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0]) == NULL) {
            return (-1);
        }

        if ((hp_mute = ado_mixer_element_sw1(mixer, "HP Mute", 2, wm8960_hp_mute_control, NULL, NULL)) == NULL) {
            return (-1);
        }

        if ((hp_vol = ado_mixer_element_volume1(mixer, "HP Volume", 2, hp_range, wm8960_hp_vol_control, NULL, NULL))
            == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, hp_mute, hp_vol) != 0) {
            return (-1);
        }

        if ((hp_io = ado_mixer_element_io(mixer, "HP OUT", SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, hp_vol, hp_io) != 0) {
            return (-1);
        }

        if (ado_mixer_playback_group_create(mixer, SND_MIXER_HEADPHONE_OUT, chmap, hp_vol, hp_mute) == NULL) {
            return (-1);
        }
    }

    /* WM8960: DAC master Output */
    if (ado_mixer_element_pcm1(mixer, SND_MIXER_ELEMENT_PLAYBACK, SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0]) == NULL) {
        return (-1);
    }

    if ((master_mute = ado_mixer_element_sw1(mixer, "Master Mute", 2, wm8960_dac_mute_control, NULL, NULL))
        == NULL) {
        return (-1);
    }

    if ((master_vol = ado_mixer_element_volume1(mixer, "Master Volume", 2, master_range, wm8960_dac_vol_control, NULL, NULL)) == NULL) {
        return (-1);
    }

    if (ado_mixer_element_route_add(mixer, master_mute, master_vol) != 0) {
        return (-1);
    }

    if ((master_io = ado_mixer_element_io(mixer, "Master OUT", SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL) {
        return (-1);
    }

    if (ado_mixer_element_route_add(mixer, master_vol, master_io) != 0) {
        return (-1);
    }

    if (ado_mixer_playback_group_create(mixer, SND_MIXER_MASTER_OUT, chmap, master_vol, master_mute) == NULL) {
        return (-1);
    }

    /* ################ */
    /*  CAPTURE GROUPS  */
    /* ################ */

    if (wm8960->mic_left2right) {
        /* Jointly-mute */
        if ((adc_mute = ado_mixer_element_sw2(mixer, "ADC Mute", wm8960_adc_mute_control, NULL, NULL)) == NULL) {
            return (-1);
        }
    } else {
        if ((adc_mute = ado_mixer_element_sw1(mixer, "ADC Mute", 2, wm8960_adc_mute_control, NULL, NULL)) == NULL) {
            return (-1);
        }
    }

    if (wm8960->mic_left2right) {
        /* Jointly-volume */
        if ((adc_vol = ado_mixer_element_volume1(mixer, "ADC Volume", 1, adc_range, wm8960_adc_vol_control, NULL, NULL)) == NULL) {
            return (-1);
        }
    }
    else {
        if ((adc_vol = ado_mixer_element_volume1(mixer, "ADC Volume", 2, adc_range, wm8960_adc_vol_control, NULL, NULL)) == NULL) {
            return (-1);
        }
    }

    if (ado_mixer_element_route_add(mixer, adc_vol, adc_mute) != 0) {
        return (-1);
    }

    if ((adc_pcm = ado_mixer_element_pcm1(mixer, SND_MIXER_ELEMENT_CAPTURE, SND_MIXER_ETYPE_CAPTURE1, 1, &pcm_devices[0])) == NULL) {
        return (-1);
    }

    if (ado_mixer_element_route_add(mixer, adc_mute, adc_pcm) != 0) {
        return (-1);
    }


    if (ado_mixer_capture_group_create(mixer, SND_MIXER_GRP_IGAIN, chmap, adc_vol, adc_mute, NULL, NULL) == NULL) {
        return (-1);
    }
    prev_elem = adc_mute;

    /* If there are multiple inputs then create an input mux */
    if ((wm8960->input_enable & INPUT_MASK) > 1)
    {
        /* use voices=1 to create jointly-capture mixer groups for the mux inputs */
        if((input_mux = ado_mixer_element_mux1(mixer, SND_MIXER_ELEMENT_INPUT_MUX, 0, 1, wm8960_mux_control, NULL, NULL)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, input_mux, prev_elem) != 0) {
            return (-1);
        }
        prev_elem = input_mux;
    }

    if (wm8960->input_enable & MIC_ENABLED)
    {
        ado_mixer_delement_t *mic_vol = NULL, *mic_mute = NULL, *mic_io = NULL;

        if (wm8960->mic_left2right) {
            /* Jointly-volume*/
            if ((mic_vol = ado_mixer_element_volume1(mixer, "Mic Volume", 1, input_range, wm8960_input_vol_control, NULL, NULL)) == NULL) {
                return (-1);
            }
        } else {
            if ((mic_vol = ado_mixer_element_volume1(mixer, "Mic Volume", 2, input_range, wm8960_input_vol_control, NULL, NULL)) == NULL) {
                return (-1);
            }
        }

        if (ado_mixer_element_route_add(mixer, mic_vol, prev_elem) != 0) {
            return (-1);
        }

        if (wm8960->mic_left2right) {
            /* Jointly-mute */
            if ((mic_mute = ado_mixer_element_sw2(mixer, "Mic Mute", wm8960_input_mute_control, NULL, NULL)) == NULL) {
                return (-1);
            }
        } else {
            if ((mic_mute = ado_mixer_element_sw1(mixer, "Mic Mute", 2, wm8960_input_mute_control, NULL, NULL)) == NULL) {
                return (-1);
            }
        }

        if (ado_mixer_element_route_add(mixer, mic_mute, mic_vol) != 0) {
            return (-1);
        }

        if ((mic_io = ado_mixer_element_io(mixer, "Mic" , SND_MIXER_ETYPE_INPUT, 0, chmap)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, mic_io, mic_mute) != 0) {
            return (-1);
        }

        wm8960->mic = mic_vol; /* The mic_vol is the element connected directly to the input_mux */
        if (ado_mixer_capture_group_create(mixer, SND_MIXER_MIC_IN, chmap, mic_vol, mic_mute, input_mux, input_mux ? wm8960->mic : NULL) == NULL) {
            return (-1);
        }
    }

    if (wm8960->input_enable & LINE2_ENABLED)
    {
        if ((wm8960->line2 = ado_mixer_element_io(mixer, "Line2", SND_MIXER_ETYPE_INPUT, 0, chmap)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, wm8960->line2, prev_elem) != 0) {
            return (-1);
        }

        if (ado_mixer_capture_group_create(mixer, SND_MIXER_LINE_IN, chmap, NULL, NULL, input_mux, input_mux ? wm8960->line2 : NULL) == NULL) {
            return (-1);
        }
    }

    if (wm8960->input_enable & LINE3_ENABLED)
    {
        if ((wm8960->line3 = ado_mixer_element_io(mixer, "Line3", SND_MIXER_ETYPE_INPUT, 0, chmap)) == NULL) {
            return (-1);
        }

        if (ado_mixer_element_route_add(mixer, wm8960->line3, prev_elem) != 0) {
            return (-1);
        }

        if (ado_mixer_capture_group_create(mixer, SND_MIXER_LINE_IN, chmap, NULL, NULL, input_mux, input_mux ? wm8960->line3 : NULL) == NULL) {
            return (-1);
        }
    }

    /* ####################### */
    /* SWITCHES                */
    /* ####################### */
    if (ado_mixer_switch_new(mixer, "MIC BIAS", SND_SW_TYPE_BYTE, 0, wm8960_micbias_get, wm8960_micbias_set, NULL, NULL) == NULL) {
        return (-1);
    }

    if (ado_mixer_switch_new(mixer, "Digital Loopback", SND_SW_TYPE_BYTE, 0,
                             wm8960_loopback_get, wm8960_loopback_set, NULL, NULL) == NULL) {
        return (-1);
    }

    if (ado_mixer_switch_new(mixer, "Analog Loopback", SND_SW_TYPE_BYTE, 0,
                             wm8960_analog_loopback_get, wm8960_analog_loopback_set, NULL, NULL) == NULL) {
        return (-1);
    }

    return (0);
}

/** WM8960 command line opts */
static const char *wm8960_opts[] = {
#define MCLK               0
    "wm8960_mclk",
#define ADCLRC             1
    "wm8960_adclrc",
#define I2CDEV             2
    "wm8960_i2cdev",
#define I2CADDR            3
    "wm8960_i2caddr",
#define MIC_LEFT2RIGHT     4
    "wm8960_mic_left2right",
#define MICBIAS            5
    "wm8960_micbias",
#define CLK_MODE           6
    "wm8960_clk_mode",
#define FORMAT             7
    "wm8960_format",
#define OUTPUT_ENABLE      8
    "wm8960_output_enable",
#define HPSEL              9
    "wm8960_hpsel",
#define MIC                10
   "wm8960_mic",
#define LINE2              11
   "wm8960_line2",
#define LINE3              12
   "wm8960_line3",
    NULL
};


#define N_OPTS ((sizeof(wm8960_opts)/sizeof(wm8960_opts[0])) - 1U)

static void dump_options(const char** keys, const char** vals, int nOptions)
{
    int cnt;
    for (cnt = 0; cnt < nOptions; cnt++) {
        if ( vals[cnt] )
            ado_error_fmt("%s = %s", keys[cnt], vals[cnt]);
    }
}

/* Parse mixer options */
static int32_t
wm8960_parse_options (MIXER_CONTEXT_T * wm8960)
{
    const ado_dict_t *dict = NULL;
    const char* optValues[N_OPTS] = {0};
    ado_card_t *card = ado_mixer_get_card(wm8960->mixer);

    dict = ado_get_card_dict( card );

    wm8960->i2c_num = WM8960_DEFAULT_I2C_NUM;
    wm8960->i2c_addr = WM8960_DEFAULT_I2C_ADDR;
    wm8960->mclk = 0;
    wm8960->use_dac_lrck = 0;
    wm8960->clk_mode = WM8960_SLAVE;
    wm8960->format = WM8960_I2S;
    wm8960->output_enable = SPEAKER_ENABLE | HEADPHONE_ENABLE;
    wm8960->hpsel = JD_NC; /* Not connected */
    wm8960->input_enable = MIC_ENABLED; /* Default Mic in single-ended mode */

    ado_config_load_key_values(dict, wm8960_opts, optValues, N_OPTS, 0, 1);

    if (optValues[MCLK])
    {
        wm8960->mclk = strtoul (optValues[MCLK], NULL, 0);
    }

    if (optValues[ADCLRC])
    {
        /* NOTE: This option can be overrided by the HPSEL option
         *       if configured to use GPIO1/ADCLRC
         */
        wm8960->use_dac_lrck = strtoul (optValues[ADCLRC], NULL, 0);
    }

    if (optValues[HPSEL])
    {
        if (strncasecmp(optValues[HPSEL], "nc", sizeof("nc")) == 0) {
            wm8960->hpsel = JD_NC;
            ado_debug(DB_LVL_MIXER, "WM8960 Headphone select not connected");
        } else if (strncasecmp(optValues[HPSEL], "gpio1", sizeof("gpio1")) == 0) {
            wm8960->hpsel = JD_GPIO1;
            ado_debug(DB_LVL_MIXER, "WM8960 Headphone select using GPIO1/ADCLRC pin");
        } else if (strncasecmp(optValues[HPSEL], "jd2", sizeof("jd2")) == 0) {
            wm8960->hpsel = JD_2;
            ado_debug(DB_LVL_MIXER, "WM8960 Headphone select using JD2/LINPUT3 pin");
        } else if (strncasecmp(optValues[HPSEL], "jd3", sizeof("jd3")) == 0) {
            wm8960->hpsel = JD_3;
            ado_debug(DB_LVL_MIXER, "WM8960 Headphone select using JD3/RINPUT3 pin");
        } else {
            ado_error_fmt ("WM8960: Invalid Headphone selection configuration: %s", optValues[HPSEL]);
            return (-1);
        }
    }

    if (optValues[MIC])
    {
        if (strncasecmp(optValues[MIC], "enable", strlen("enable")) == 0)
        {
            char *p = strpbrk(optValues[MIC], ":");
            if (p != NULL)
            {
                if (strncasecmp(p+1, "differential", strlen("differential")) == 0)
                {
                    p = strpbrk(p, ":");
                    if (p != NULL)
                    {
                        if (strncasecmp(p+1, "input2", strlen("input2")) == 0)
                            wm8960->input_enable |= DIFFERENTIAL_LINE2;
                        else if (strncasecmp(p+1, "input3", strlen("input3")) == 0)
                        {
                            if (wm8960->hpsel == JD_2 || wm8960->hpsel == JD_3)
                            {
                                ado_error_fmt("Invalid configuration - Mic differential using input3 cannot be enabled when also configured as headphone detect");
                                return (-1);
                            }
                            wm8960->input_enable |= DIFFERENTIAL_LINE3;
                        }
                        else
                        {
                            ado_error_fmt("Invalid non-inverting differential input value %s - %s=%s", p, wm8960_opts[MIC], optValues[MIC]);
                            return (-1);
                        }
                    }
                    else
                    {
                        ado_error_fmt("Must provide non-inverting input2 or input3 for differential input - %s=%s", wm8960_opts[MIC], optValues[MIC]);
                        return (-1);
                    }
                }
                else if (strncasecmp(p+1, "single-ended", strlen("single-ended")) == 0)
                {
                    wm8960->input_enable &= ~(DIFFERENTIAL_LINE2 | DIFFERENTIAL_LINE3); /* Default mode */
                }
                else
                {
                    ado_error_fmt("Invalid key value pair %s:%s", wm8960_opts[MIC], optValues[MIC]);
                    return (-1);
                }
            }
            ado_debug(DB_LVL_MIXER, "Enable Mic in %s mode", wm8960->input_enable & (DIFFERENTIAL_LINE2 | DIFFERENTIAL_LINE3) ? "single-ended" : "differential");
            wm8960->input_enable |= MIC_ENABLED;
        }
        else
        {
             ado_debug(DB_LVL_MIXER, "Disable Mic");
             wm8960->input_enable &= ~(MIC_ENABLED | DIFFERENTIAL_LINE2 | DIFFERENTIAL_LINE3);
        }
    }

    if (optValues[LINE2])
    {
        if (strncasecmp(optValues[LINE2], "enable", strlen("enable")) == 0)
        {
            char *p;

            if (wm8960->input_enable & DIFFERENTIAL_LINE2)
            {
                ado_error_fmt("Invalid configuration - Line2 cannot be eneabled when input2 is also configured as the non-inverting Mic differential input");
                return (-1);
            }

            p = strpbrk(optValues[LINE2], ":");
            if (p != NULL)
            {
                wm8960->line2_boost = strtoul (p+1, NULL, 0);
                switch (wm8960->line2_boost)
                {
                    case -12:
                    case -9:
                    case -6:
                    case -3:
                    case 0:
                    case 3:
                    case 6:
                        break;
                    default:
                        ado_error_fmt("Invalid line2 boost value %d for key value pair %s=%s", wm8960_opts[LINE2], optValues[LINE2]);
                        return (-1);
                    break;
                }
            }
            else
            {
                /* Default line2 boost value to 0dB */
                wm8960->line2_boost = 0;
            }
            ado_debug(DB_LVL_MIXER, "Line2 enabled, Gain Boost = %ddB", wm8960->line2_boost);
            wm8960->input_enable |= LINE2_ENABLED;
        }
        else
        {
             wm8960->input_enable &= ~(LINE2_ENABLED);
        }
    }

    if (optValues[LINE3])
    {
        if (strncasecmp(optValues[LINE3], "enable", strlen("enable")) == 0)
        {
            char *p;

            if (wm8960->input_enable & DIFFERENTIAL_LINE3)
            {
                ado_error_fmt("Invalid configuration - Line3 cannot be enabled when input3 is also configured as the non-inverting Mic differential input");
                return (-1);
            }
            if (wm8960->hpsel == JD_2 || wm8960->hpsel == JD_3)
            {
                ado_error_fmt("Invalid configuration - Input3/Line3 cannot be enabled when also configured as headphone detect");
                return (-1);
            }

            p = strpbrk(optValues[LINE3], ":");
            if (p != NULL)
            {
                wm8960->line3_boost = strtoul (p+1, NULL, 0);
                switch (wm8960->line3_boost)
                {
                    case -12:
                    case -9:
                    case -6:
                    case -3:
                    case 0:
                    case 3:
                    case 6:
                        break;
                    default:
                        ado_error_fmt("Invalid line3 boost value %d for key value pair %s=%s", wm8960_opts[LINE3], optValues[LINE3]);
                        return (-1);
                    break;
                }
            }
            else
            {
                /* Default line3 boost value to 0dB */
                wm8960->line3_boost = 0;
            }
            ado_debug(DB_LVL_MIXER, "Line3 enabled, Gain boost = %ddB", wm8960->line3_boost);
            wm8960->input_enable |= LINE3_ENABLED;
        }
        else
        {
             wm8960->input_enable &= ~(LINE3_ENABLED);
        }
    }

    if (optValues[I2CDEV])
    {
        wm8960->i2c_num = strtoul (optValues[I2CDEV], NULL, 0);
    }

    if (optValues[I2CADDR])
    {
        wm8960->i2c_addr = strtoul (optValues[I2CADDR], NULL, 0);
    }

    if (optValues[MIC_LEFT2RIGHT])
    {
        unsigned int val = strtoul (optValues[MIC_LEFT2RIGHT], NULL, 0);
        wm8960->mic_left2right = (val == 0) ? false : true;
    }

    if (optValues[MICBIAS])
    {
        unsigned int val = strtoul (optValues[I2CADDR], NULL, 0);
        wm8960->micbias = (val == 0) ? false : true;
    }
    if (optValues[CLK_MODE])
    {
        if (strncasecmp(optValues[CLK_MODE], "master", sizeof("master")) == 0) {
            wm8960->clk_mode = WM8960_MASTER;
            ado_debug(DB_LVL_MIXER, "WM8960 clock mode = Master");
        } else if (strncasecmp(optValues[CLK_MODE], "slave", sizeof("slave")) == 0) {
            wm8960->clk_mode = WM8960_SLAVE;
            ado_debug(DB_LVL_MIXER, "WM8960 clock mode = Slave");
        } else {
            ado_error_fmt ("WM8960: Invalid clk_mode: %s", optValues[CLK_MODE]);
            return (-1);
        }
    }
    if (optValues[FORMAT])
    {
        if (strncasecmp(optValues[FORMAT], "I2S", sizeof("I2S")) == 0) {
            wm8960->format = WM8960_I2S;
            ado_debug(DB_LVL_MIXER, "WM8960 format = I2S");
        } else if (strncasecmp(optValues[FORMAT], "PCM", sizeof("PCM")) == 0) {
            wm8960->format = WM8960_PCM;
            ado_debug(DB_LVL_MIXER, "WM8960 format = PCM");
        } else {
            ado_error_fmt ("WM8960: Invalid format: %s", optValues[FORMAT]);
            return (-1);
        }
    }

    if (optValues[OUTPUT_ENABLE])
    {
        if (strncasecmp(optValues[OUTPUT_ENABLE], "speaker", sizeof("speaker")) == 0) {
            wm8960->output_enable = SPEAKER_ENABLE;
        } else if (strncasecmp(optValues[OUTPUT_ENABLE], "headphone", sizeof("headphone")) == 0) {
            wm8960->output_enable = HEADPHONE_ENABLE;
        } else if (strncasecmp(optValues[OUTPUT_ENABLE], "all", sizeof("all")) == 0) {
            wm8960->output_enable = (SPEAKER_ENABLE | HEADPHONE_ENABLE);
        } else {
            ado_error_fmt ("wm8960: Invalid output path: %s", optValues[OUTPUT_ENABLE]);
            return (-1);
        }
    }


    dump_options(wm8960_opts, optValues, N_OPTS);

    return (EOK);
}

/**
 * Called by audio controller to associate a mixer element and group with a PCM device.
 *
 * @param pcm     Pointer to PCM data
 * @param mixer   Pointer to Mixer data
 * @param channel Channel type
 * @param index   Element index
 */
static void codec_set_default_group(ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index)
{
    /* setup mixer controls for playback and capture */
    switch (channel) {
        case ADO_PCM_CHANNEL_PLAYBACK:
            ado_pcm_chn_mixer(pcm, ADO_PCM_CHANNEL_PLAYBACK, mixer,
                              ado_mixer_find_element(mixer, SND_MIXER_ETYPE_PLAYBACK1, SND_MIXER_ELEMENT_PLAYBACK, index),
                              ado_mixer_find_group(mixer, SND_MIXER_MASTER_OUT, index));
            break;
        case ADO_PCM_CHANNEL_CAPTURE:
            ado_pcm_chn_mixer(pcm, ADO_PCM_CHANNEL_CAPTURE, mixer,
                              ado_mixer_find_element(mixer, SND_MIXER_ETYPE_CAPTURE1, SND_MIXER_ELEMENT_CAPTURE, index),
                              ado_mixer_find_group(mixer, SND_MIXER_GRP_IGAIN, index));
            break;
        default:
            break;
    }
}

ado_mixer_dll_init_t mixer_dll_init;
int
mixer_dll_init (MIXER_CONTEXT_T ** context, ado_mixer_t * mixer, void *params, void *raw_callbacks, int version)
{
    (void)(version);  /* Currently not used */
    wm8960_context_t *wm8960;
    char i2c_port[MAX_I2C_PORT];
    ado_mixer_dll_codec_callbacks_t *callbacks = raw_callbacks;

    ado_debug(DB_LVL_MIXER, "Initializing WM8960 Codec");

    if ((wm8960 = (wm8960_context_t *)
        ado_calloc (1, sizeof (wm8960_context_t))) == NULL)
    {
        ado_error_fmt ("WM8960: Failed to allocate device structure - %s", strerror (errno));
        return (-1);
    }

    *context = wm8960;
    ado_mixer_set_name (mixer, "WM8960");

    wm8960->mixer = mixer;
    memcpy (&wm8960->params, params, sizeof (wm8960->params));
    wm8960->sample_rate = wm8960->params.tx_sample_rate;
    wm8960->sample_size = wm8960->params.tx_sample_size;

    if (wm8960_parse_options (wm8960)!=EOK)
    {
        ado_error_fmt ("WM8960: Failed to parse mixer options");
        ado_free (wm8960);
        return (-1);
    }

    if (callbacks)
    {
        callbacks->codec_set_default_group = codec_set_default_group;
    }

    if (wm8960->params.codec_open)
    {
        if (wm8960->params.codec_open(wm8960->params.hw_context) != EOK)
        {
            ado_error_fmt("WM8960: codec open failed");
            ado_free(wm8960);
            return (-1);
        }
    }
    else
    {
        snprintf(i2c_port, MAX_I2C_PORT - 1, "/dev/i2c%d", wm8960->i2c_num);
        if ((wm8960->i2c_fd = open(i2c_port, O_RDWR)) < 0)
        {
            ado_error_fmt("WM8960: could not open i2c device %s - %s", i2c_port, strerror(errno));
            ado_free(wm8960);
            return (-1);
        }
    }

    if (build_wm8960_mixer (wm8960, wm8960->mixer))
    {
        ado_error_fmt ("WM8960: Failed to build mixer");
        if (wm8960->params.codec_close)
        {
            wm8960->params.codec_close(wm8960->params.hw_context);
        }
        else
        {
            close (wm8960->i2c_fd);
        }
        ado_free (wm8960);
        return (-1);
    }

    wm8960_reset(wm8960);
    wm8960_init_clocks(wm8960);
    wm8960_init_subsystems(wm8960);
    wm8960_init_interface(wm8960);

    ado_mixer_set_reset_func (wm8960->mixer, wm8960_reset);
    ado_mixer_set_destroy_func (wm8960->mixer, wm8960_destroy);
    return (0);
}

ado_dll_version_t version;
void
version (int *major, int *minor, const char **date)
{
    *major = ADO_MAJOR_VERSION;
    *minor = I2S_CODEC_MINOR_VERSION;
    *date = __DATE__;
    ado_debug(DB_LVL_DRIVER, "Date = %s", *date);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/mixer/wm8960/wm8960.c $ $Rev: 934572 $")
#endif
