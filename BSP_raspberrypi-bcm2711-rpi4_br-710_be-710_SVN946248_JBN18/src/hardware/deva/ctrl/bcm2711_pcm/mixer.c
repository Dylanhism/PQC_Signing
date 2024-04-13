/*
 * $QNXLicenseC:
 * Copyright 2020 QNX Software Systems.
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

#include "pcm.h"
#include "mixer.h"

/**- codec support -**/

/**
 * Callback invoked by the codec to find out whether the playback or capture
 * channel is active
 * @param  hwc  hardware context structure.
 * @param  channel ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE
 * @return true if channel is active, false otherwise
 */
static bool codec_is_active(HW_CONTEXT_T *hwc, int channel)
{
    if ((channel == ADO_PCM_CHANNEL_PLAYBACK && hwc->playback.active) ||
        (channel == ADO_PCM_CHANNEL_CAPTURE && hwc->capture.active)) {
        return true;
    }

    return false;
}

/**
 * Callback invoked by the codec to get the channel map for the playback or capture
 * channel
 * @param  hwc  hardware context structure.
 * @param  channel ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE
 * @index  unused
 * @return channel map pointer or NULL if channel not configured
 */
static const snd_pcm_chmap_t* codec_get_chmap(HW_CONTEXT_T *hwc, int channel, int index)
{
    if ((channel == ADO_PCM_CHANNEL_PLAYBACK && hwc->playback.configured) ||
        (channel == ADO_PCM_CHANNEL_CAPTURE && hwc->capture.configured)) {
        return hwc->chmap;
    }

    return NULL;
}

/**
 * Helper function used to load a codec mixer dll, if one is indicated in the config
 * file for this card; otherwise create a simple mixer
 * @param  card sound card structure.
 * @param  hwc  hardware context structure.
 * @return Execution status
 */
int32_t build_codec_mixer(ado_card_t *card, HW_CONTEXT_T *hwc)
{
    static bool built_codec_mixer = false;
    ado_mixer_dll_codec_params_t params = {0};

    if (built_codec_mixer) {
        return EALREADY;
    }

    memset (&hwc->codec_mixer_callbacks, 0, sizeof (hwc->codec_mixer_callbacks));

    params.hw_context = hwc;

    params.tx_sample_rate = hwc->sample_rate_max;
    params.rx_sample_rate = hwc->sample_rate_max;
    params.tx_sample_size = ado_pcm_format_bit_width(hwc->sample_format);
    params.rx_sample_size = ado_pcm_format_bit_width(hwc->sample_format);
    params.tx_voices = PCM_CHANNELS;
    params.rx_voices = PCM_CHANNELS;

    /* The codec clock mode will be the opposite of the Audio Controllers clock mode */
    if (hwc->clk_mode == CLK_MODE_MASTER) {
        params.clk_master = false;
    } else {
        params.clk_master = true;
    }

    /* set i2c_dev, i2c_addr, mclk to invalid values, these should be passed as config file codec params */
    params.i2c_dev = -1;
    params.i2c_addr = -1;
    params.mclk = 0;

    /* codec -> ctrl callbacks */
    params.is_active = codec_is_active;
    params.get_chmap = codec_get_chmap;

    /* If NULL is passed in as the DLL name, this will cause the ado_mixer_dll() call
     * to look in the audio configuration file for the dll name (mixer_dll=xxxx)
     */
    if (ado_mixer_dll(card, NULL, I2S_CODEC_MINOR_VERSION, &params,
            &hwc->codec_mixer_callbacks, &hwc->mixer) != EOK) {
        /* ado_mixer_dll returns -1 and sets errno on failure */
        if (errno == ENOENT) {
            ado_debug (DB_LVL_DRIVER, "ado_mixer_dll failed (%d) - build a simple mixer", errno);
            /* no codec mixer was found, create a simple mixer */
            if (ado_mixer_create(card, "mixer", &hwc->mixer, hwc) != EOK) {
                /* ado_mixer_create returns -1 and sets errno on failure */
                ado_error_fmt ("Failed building a simple mixer (%d)", errno);
                return errno;
            }
        } else {
            ado_error_fmt ("ado_mixer_dll failed (%d)", errno);
            return errno;
        }
    } else {
        if (hwc->codec_mixer_callbacks.codec_set_default_group == NULL) {
            ado_error_fmt ( "Mixer DLL missing codec_set_default_group callback" );
            ado_mixer_destroy(hwc->mixer);
            return ENOTSUP;
        }
    }

    built_codec_mixer = true;

    return EOK;
}

/**
 * Helper function used to invoke the codec's codec_set_rate callback if a codec
 * is in use and it has a codec_set_rate callback
 * @param  hwc         hardware context structure.
 * @param  sample_rate sample rate in Hz
 */
void codec_set_rate(HW_CONTEXT_T *hwc, uint32_t sample_rate)
{
    if ( hwc->codec_mixer_callbacks.codec_set_rate ) {
        hwc->codec_mixer_callbacks.codec_set_rate(hwc->mixer, sample_rate, ADO_PCM_CHANNEL_PLAYBACK);
    }
}

/**
 * Helper function used to invoke the codec's codec_capabilities callback and provide its
 * rates capability information, if a codec is in use and it has a codec_capabilities callback;
 * otherwise return a bitmask with all valid rates
 * @param  hwc    hardware context structure.
 * @param  rates  rates bitmask assigned to the bitmask of codec supported rates
 */
void codec_supported_rates(HW_CONTEXT_T* hwc, uint32_t* rates)
{
    *rates = SND_PCM_RATE_ALL;

    if (hwc->codec_mixer_callbacks.codec_capabilities) {
        ado_mixer_dll_codec_capabilities_t cap = {0};
        hwc->codec_mixer_callbacks.codec_capabilities(hwc->mixer, &cap, ADO_PCM_CHANNEL_PLAYBACK);
        /* Ignore non-standard rates for now; still need to free the memory for ratelist
         * if allocated by the codec_capabilities callback
         */
        if (cap.ratelist.list && cap.ratelist.list_free) {
            cap.ratelist.list_free(cap.ratelist.list);
        }
        *rates = cap.rates;
    }
}

/**
 * Helper function used to invoke the codec's codec_capabilities callback and provide its
 * formats capability information, if a codec is in use and it has a codec_capabilities callback;
 * otherwise return a bitmask with all formats that could be supported by this driver
 * @param  hwc     hardware context structure.
 * @param  formats formats bitmask assigned to the bitmask of codec supported formats
 */
void codec_supported_formats(HW_CONTEXT_T* hwc, uint32_t* formats)
{
    *formats = SND_PCM_FMT_S32 | SND_PCM_FMT_S24_4 | SND_PCM_FMT_S16;

    if (hwc->codec_mixer_callbacks.codec_capabilities) {
        ado_mixer_dll_codec_capabilities_t cap = {0};
        hwc->codec_mixer_callbacks.codec_capabilities(hwc->mixer, &cap, ADO_PCM_CHANNEL_PLAYBACK);
        *formats = cap.formats;
    }
}

/**
 * Helper function used to invoke the codec's codec_set_default_group callback, if a codec
 * is in use and it has a codec_set_default_group callback;
 * @param  hwc     hardware context structure.
 * @param  pcm     pointer to pcm device
 * @param  channel ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE
 * @param  index
 */
void codec_set_default_group(HW_CONTEXT_T* hwc, ado_pcm_t *pcm, int channel, int index)
{
    /* if a codec mixer dll is used, use its default group handling */
    if (hwc->codec_mixer_callbacks.codec_set_default_group) {
        hwc->codec_mixer_callbacks.codec_set_default_group (pcm, hwc->mixer, channel, index);
    }
}

/**
 * Helper function used to invoke the codec's codec_on callback, if a codec
 * is in use and it has a codec_on callback;
 * @param  hwc     hardware context structure.
 * @param  channel ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE
 */
void codec_on(HW_CONTEXT_T* hwc, int channel) {
    /* if a codec mixer dll is used, and it has a codec_on callback, invoke it */
    if (hwc->codec_mixer_callbacks.codec_on) {
        hwc->codec_mixer_callbacks.codec_on (hwc->mixer, channel);
    }
}

/**
 * Helper function used to invoke the codec's codec_off callback, if a codec
 * is in use and it has a codec_off callback;
 * @param  hwc     hardware context structure.
 * @param  channel ADO_PCM_CHANNEL_PLAYBACK or ADO_PCM_CHANNEL_CAPTURE
 */
void codec_off(HW_CONTEXT_T* hwc, int channel) {
    /* if a codec mixer dll is used, and it has a codec_off callback, invoke it */
    if (hwc->codec_mixer_callbacks.codec_off) {
        hwc->codec_mixer_callbacks.codec_off (hwc->mixer, channel);
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/mixer.c $ $Rev: 925119 $")
#endif
