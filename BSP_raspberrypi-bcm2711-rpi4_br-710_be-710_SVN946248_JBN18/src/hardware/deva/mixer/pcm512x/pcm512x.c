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

struct pcm512x_context;
#define MIXER_CONTEXT_T struct pcm512x_context

#include <hw/i2c.h>
#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>
#include "pcm512x.h"

#define MUTE_ALL   0x3

static int32_t pcm_devices[1] = { 0 };

/* Volume range of the PCM512X DAC */
static struct snd_mixer_element_volume1_range pcm512x_volume_range[2] = {
    {0, 255, -1030, 240, 10},    // min, max, min_dB, max_dB, dB_scale_factor
    {0, 255, -1030, 240, 10}     // min, max, min_dB, max_dB, dB_scale_factor
};

static int pcm512x_open_i2c_fd ( pcm512x_context_t *pcm512x )
{
    int status = EOK;

    if ( pcm512x ) {
        if ( pcm512x->params.codec_open ) {
            status = pcm512x->params.codec_open( pcm512x->params.hw_context );
            if ( status != EOK ) {
                ado_error_fmt ( "PCM512X codec open failed" );
            }
        } else {
            char i2c_devname[20];
            sprintf ( i2c_devname, "/dev/i2c%d", pcm512x->params.i2c_dev );
            if( pcm512x->i2c_fd < 0 ) {
                pcm512x->i2c_fd = open(i2c_devname, O_RDWR);
                if (pcm512x->i2c_fd < 0) {
                    status = ENODEV;
                }
            } else {
                status = EALREADY;
            }
        }
    } else {
        ado_error_fmt( "PCM512X codec is not init" );
        status = EINVAL;
    }
    return status;
}

static int pcm512x_close_i2c_fd ( pcm512x_context_t *pcm512x )
{
    int status = EOK;

    if ( pcm512x ) {
        if ( pcm512x->params.codec_close ) {
            pcm512x->params.codec_close( pcm512x->params.hw_context );
        } else {
            if ( pcm512x->i2c_fd < 0 ) {
                status = EBADF;
            } else {
                close(pcm512x->i2c_fd);
                pcm512x->i2c_fd = -1;
            }
        }
    } else {
        ado_error_fmt( "PCM512X codec is not init" );
        status = EINVAL;
    }

    return status;
}

static int pcm512x_i2c_write ( pcm512x_context_t *pcm512x, uint8_t reg, uint8_t val, int index )
{
    int status = EOK;

    if ( pcm512x ) {
        if ( pcm512x->params.codec_write ) {
            status = pcm512x->params.codec_write( pcm512x->params.hw_context, reg, val, index );
        } else {
            struct OMSG {
                i2c_send_t  hdr;        /* Message header */
                uint8_t     bytes[8];   /* Transmit buffer with (optional) extra space */
            };

            struct OMSG omsg;

            if( pcm512x->i2c_fd < 0 ) {
                return EBADF;
            }

            omsg.hdr.slave.addr = pcm512x->params.i2c_addr + index;
            omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
            omsg.hdr.len = 2;
            omsg.hdr.stop = 1;
            omsg.bytes[0] = reg;
            omsg.bytes[1] = val;

            status = devctl(pcm512x->i2c_fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);
        }
        if ( status != EOK ) {
            ado_error_fmt ( "PCM512X i2c write failed" );
        }
    } else {
        ado_error_fmt("PCM512X codec is not init");
        status = EINVAL;
    }

    return status;
}

static int pcm512x_i2c_read ( pcm512x_context_t *pcm512x, uint8_t reg, uint8_t* val, int index )
{
    int status = EOK;
    uint32_t reg_val;

    if ( pcm512x && val ) {
        if ( pcm512x->params.codec_read ) {
            status = pcm512x->params.codec_read( pcm512x->params.hw_context, reg, &reg_val, index );
            if ( status == EOK ) {
                *val = reg_val;
            }
        } else {
            int             status;
            iov_t           siov[2], riov[2];
            unsigned char   bdata;
            i2c_sendrecv_t  hdr;

            if( pcm512x->i2c_fd < 0 ) {
                return EBADF;
            }

            hdr.slave.addr = pcm512x->params.i2c_addr + index;
            hdr.slave.fmt = I2C_ADDRFMT_7BIT;
            hdr.stop = 1;

            hdr.send_len = 1;
            hdr.recv_len = 1;

            SETIOV(&siov[0], &hdr, sizeof(hdr) );
            SETIOV(&siov[1], &reg, 1 );

            SETIOV(&riov[0], &hdr, sizeof(hdr) );
            SETIOV(&riov[1], &bdata, 1 );
            status = devctlv(pcm512x->i2c_fd, DCMD_I2C_SENDRECV, 2, 2, siov, riov, NULL);

            if( status == EOK ) {
                *val = bdata;
            }
        }
        if ( status != EOK ) {
            ado_error_fmt ( "PCM512X i2c read failed" );
        }
    } else {
        ado_error_fmt("PCM512X codec is not init or val is NULL");
        status = EINVAL;
    }

    return status;
}

int pcm512x_vol_get( pcm512x_context_t* pcm512x, uint8_t* left_vol, uint8_t* right_vol )
{
    int status = EOK;
    uint8_t vol_data;

    if ( left_vol == NULL || right_vol == NULL ) {
        return EINVAL;
    }

    /* read the left channel volume */
    status = pcm512x_i2c_read ( pcm512x, PCM512X_DIGITAL_VOLUME_2, &vol_data, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading register PCM512X_DIGITAL_VOLUME_2");
        return status;
    }
    *left_vol = 255 - vol_data;

    if (pcm512x->joint_vol) {
        *right_vol = *left_vol;
    } else {
        /* read the right channel volume */
        status = pcm512x_i2c_read ( pcm512x, PCM512X_DIGITAL_VOLUME_3, &vol_data, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed reading register PCM512X_DIGITAL_VOLUME_3");
            return status;
        }
        *right_vol = 255 - vol_data;
    }

    return EOK;
}

int pcm512x_vol_set( pcm512x_context_t* pcm512x, uint8_t left_vol, uint8_t right_vol )
{
    int status = EOK;

    /* write the left channel volume */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_DIGITAL_VOLUME_2, 255 - left_vol, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing register PCM512X_DIGITAL_VOLUME_2");
        return status;
    }

    if (!pcm512x->joint_vol) {
        /* write the right channel volume */
        status = pcm512x_i2c_write ( pcm512x, PCM512X_DIGITAL_VOLUME_3, 255 - right_vol, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing register PCM512X_DIGITAL_VOLUME_3");
            return status;
        }
    }

    return EOK;
}

int pcm512x_analog_gain_get( pcm512x_context_t* pcm512x, uint8_t* left_gain, uint8_t* right_gain )
{
    int status = EOK;
    uint8_t gain_data, gain_boost_data;

    if ( left_gain == NULL || right_gain == NULL ) {
        return EINVAL;
    }

    /* Select page 1 */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 1, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed switching to page 1");
        return status;
    }
    status = pcm512x_i2c_read ( pcm512x, PCM512X_ANALOG_GAIN_CTRL, &gain_data, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_ANALOG_GAIN_CTRL register");
    } else {
        status = pcm512x_i2c_read ( pcm512x, PCM512X_ANALOG_GAIN_BOOST, &gain_boost_data, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed reading PCM512X_ANALOG_GAIN_CTRL register");
        }
    }

    /* Select page 0 */
    if (pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 0, 0 ) != EOK) {
        ado_error_fmt("Failed switching to page 0");
    }

    if ( status != EOK ) {
        return status;
    }

    if (gain_data & PCM512X_LAGN) {
        if (gain_boost_data & PCM512X_AGBL) {
            *left_gain = PCM512X_ANALOG_GAIN_MINUS5PT2_DB;
        } else {
            *left_gain = PCM512X_ANALOG_GAIN_MINUS6_DB;
        }
    } else {
        if (gain_boost_data & PCM512X_AGBL) {
            *left_gain = PCM512X_ANALOG_GAIN_0PT8_DB;
        } else {
            *left_gain = PCM512X_ANALOG_GAIN_0_DB;
        }
    }
    if (gain_data & PCM512X_RAGN) {
        if (gain_boost_data & PCM512X_AGBR) {
            *right_gain = PCM512X_ANALOG_GAIN_MINUS5PT2_DB;
        } else {
            *right_gain = PCM512X_ANALOG_GAIN_MINUS6_DB;
        }
    } else {
        if (gain_boost_data & PCM512X_AGBR) {
            *right_gain = PCM512X_ANALOG_GAIN_0PT8_DB;
        } else {
            *right_gain = PCM512X_ANALOG_GAIN_0_DB;
        }
    }

    return EOK;
}

int pcm512x_analog_gain_set( pcm512x_context_t* pcm512x, uint8_t left_gain, uint8_t right_gain )
{
    int status = EOK;
    uint8_t gain_data = 0;
    uint8_t gain_boost_data = 0;

    if (left_gain == PCM512X_ANALOG_GAIN_MINUS6_DB ||
        left_gain == PCM512X_ANALOG_GAIN_MINUS5PT2_DB) {
        gain_data |= PCM512X_LAGN;
    }
    if (left_gain == PCM512X_ANALOG_GAIN_0PT8_DB ||
        left_gain == PCM512X_ANALOG_GAIN_MINUS5PT2_DB) {
        gain_boost_data |= PCM512X_AGBL;
    }

    if (right_gain == PCM512X_ANALOG_GAIN_MINUS6_DB ||
        right_gain == PCM512X_ANALOG_GAIN_MINUS5PT2_DB) {
        gain_data |= PCM512X_RAGN;
    }
    if (right_gain == PCM512X_ANALOG_GAIN_0PT8_DB ||
        right_gain == PCM512X_ANALOG_GAIN_MINUS5PT2_DB) {
        gain_boost_data |= PCM512X_AGBR;
    }

    /* Select page 1 */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 1, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed switching to page 1");
        return status;
    }
    status = pcm512x_i2c_write ( pcm512x, PCM512X_ANALOG_GAIN_CTRL, gain_data, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_ANALOG_GAIN_CTRL register");
    } else {
        status = pcm512x_i2c_write ( pcm512x, PCM512X_ANALOG_GAIN_BOOST, gain_boost_data, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing PCM512X_ANALOG_GAIN_CTRL register");
        }
    }
    /* Select page 0 */
    if (pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 0, 0 ) != EOK) {
        ado_error_fmt("Failed switching to page 0");
    }

    return status;
}

int pcm512x_mute_set( pcm512x_context_t* pcm512x, uint8_t mute )
{
    uint8_t mute_data = 0;

    /* set mute status for left channel */
    if (mute & 0x1) {
        mute_data |= PCM512X_RQML;
    }
    /* set mute status for right channel */
    if (mute & 0x2) {
        mute_data |= PCM512X_RQMR;
    }

    return pcm512x_i2c_write ( pcm512x, PCM512X_MUTE, mute_data, 0 );
}

int pcm512x_mute_get( pcm512x_context_t* pcm512x, uint8_t* mute )
{
    int status = EOK;
    uint8_t mute_data;

    if ( mute ) {
        *mute = 0;
        status = pcm512x_i2c_read ( pcm512x, PCM512X_MUTE, &mute_data, 0 );
        if ( status == EOK ) {
            /* get mute status for left channel */
            if (mute_data & PCM512X_RQML) {
                *mute |= 0x1;
            }
            /* get mute status for right channel */
            if (mute_data & PCM512X_RQMR) {
                *mute |= 0x2;
            }
        }
    }

    return status;
}

int pcm512x_init ( pcm512x_context_t* pcm512x )
{
    int status;
    int gpio_idx;
    uint8_t gpio_en = 0;
    uint8_t gpio_ctl = 0;
    uint8_t gpio_inv = 0;
    uint8_t i2s_data = 0;
    uint8_t analog_gain = 0;

    ado_debug ( DB_LVL_DRIVER, "" );

    if ( pcm512x == NULL ) {
        ado_error_fmt ( "pcm512x is NULL" );
        return EINVAL;
    }

    if ( !pcm512x->params.tx_sample_rate ) {
        ado_error_fmt ( "tx sample rate must be provided" );
        return ENOTSUP;
    }

    switch (pcm512x->params.tx_sample_size) {
        case 0: /* format not specified, pick 16 bit format by default */
        case 16:
            i2s_data = PCM512X_ALEN_16;
            break;
        case 24:
            i2s_data = PCM512X_ALEN_24;
            break;
        case 32:
            i2s_data = PCM512X_ALEN_32;
            break;
        default:
            ado_error_fmt ( "sample size %d not supported", pcm512x->params.tx_sample_size );
            return ENOTSUP;
    }

    if ( pcm512x->params.tx_voices != 2 ) {
        ado_error_fmt ( "tx voices %d not supported", pcm512x->params.tx_voices );
        return ENOTSUP;
    }

    status = pcm512x_open_i2c_fd( pcm512x );
    if ( status != EOK ) {
        ado_error_fmt("Failed opening i2c device");
        return status;
    }

    /* Select page 0 */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 0, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed switching to page 0");
        return status;
    }
    /* Reset the DAC by toggling the RSTR and RSTM bits */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_RESET, PCM512X_RSTR | PCM512X_RSTM, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_RESET register");
        return status;
    }
    status = pcm512x_i2c_write ( pcm512x, PCM512X_RESET, 0, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_RESET register");
        return status;
    }

    /* Put the DAC in standby mode */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_POWER, PCM512X_RQST, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_POWER register");
        return status;
    }

    /* Set the PLL reference to BCK input (slave mode) */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PLL_REF, PCM512X_SREF_BCK, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_PLL_REF register");
        return status;
    }

    /* Ignore SCK clock halt detection (slave mode, SCK not used) */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_ERROR_DETECT, PCM512X_IDCH, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_ERROR_DETECT register");
        return status;
    }

    /* Set bus format to I2S and configured sample width */
    i2s_data |= PCM512X_AFMT_I2S;
    status = pcm512x_i2c_write ( pcm512x, PCM512X_I2S_1, i2s_data, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_I2S_1 register");
        return status;
    }

    if (pcm512x->joint_vol) {
        /* if joint volume enabled, configure right channel volume to follow left channel */
        status = pcm512x_i2c_write ( pcm512x, PCM512X_DIGITAL_VOLUME_1, 0x1, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing PCM512X_DIGITAL_VOLUME_1 register");
            return status;
        }
    }

    /* Set the sample rate */
    status = pcm512x_rate_setting ( pcm512x, pcm512x->params.tx_sample_rate );
    if ( status != EOK ) {
        ado_error_fmt("Failed setting the sample rate");
        return status;
    }

    /* Set the default volume (0dB) */
    status = pcm512x_vol_set ( pcm512x, pcm512x->left_vol, pcm512x->right_vol );
    if ( status != EOK ) {
        ado_error_fmt("Failed setting the default volume");
        return status;
    }

    /* Set de default mute status (unmuted) */
    status = pcm512x_mute_set ( pcm512x, pcm512x->mute );
    if ( status != EOK ) {
        ado_error_fmt("Failed setting the mute status");
        return status;
    }

    /* Set the analog gain */
    analog_gain = pcm512x->switch_value.value.string_11.selection;
    status = pcm512x_analog_gain_set (pcm512x, analog_gain, analog_gain);
    if ( status != EOK ) {
        ado_error_fmt("Failed setting the analog gain status");
        return status;
    }

    /* GPIO control - This codec has GPIO pins that can be used for interfacing to
     * external devices (e.g. in the case of the JustBoom DAC HAT, GPIOs 3, 6 can be
     * used to interact with the JustBoom AMP HAT). By default all GPIOs are input.
     * Via configuration options they can be specified as outputs.
     * Configure the GPIOs based on configuration options.
     */
    for (gpio_idx = 0; gpio_idx < 6; gpio_idx++) {
        if (pcm512x->gpio_sel[gpio_idx] != PCM512X_GXSL_OFF) {
            gpio_en |= (1 << gpio_idx);
            if (pcm512x->gpio_sel[gpio_idx] == PCM512X_GXSL_REG) {
                if (pcm512x->gpio_val[gpio_idx]) {
                    gpio_ctl |= (1 << gpio_idx);
                }
            } else {
                if (pcm512x->gpio_val[gpio_idx]) {
                    gpio_inv |= (1 << gpio_idx);
                }
            }
            status = pcm512x_i2c_write ( pcm512x, PCM512X_GPIO_OUTPUT_1 + gpio_idx, pcm512x->gpio_sel[gpio_idx], 0 );
            if ( status != EOK ) {
                ado_error_fmt("Failed writing PCM512X_GPIO_OUTPUT_%d register", gpio_idx + 1);
                return status;
            }
        }
    }
    if (gpio_en) {
        status = pcm512x_i2c_write ( pcm512x, PCM512X_GPIO_EN, gpio_en, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing PCM512X_GPIO_EN register");
            return status;
        }
    }
    if (gpio_ctl) {
        status = pcm512x_i2c_write ( pcm512x, PCM512X_GPIO_CONTROL_1, gpio_ctl, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing PCM512X_GPIO_CONTROL_1 register");
            return status;
        }
    }
    if (gpio_inv) {
        status = pcm512x_i2c_write ( pcm512x, PCM512X_GPIO_CONTROL_2, gpio_inv, 0 );
        if ( status != EOK ) {
            ado_error_fmt("Failed writing PCM512X_GPIO_CONTROL_2 register");
            return status;
        }
    }

    /* Put the DAC out of standby mode */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_POWER, 0, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed writing PCM512X_POWER register");
        return status;
    }

    if ( pcm512x->debug ) {
        pcm512x_register_dump ( pcm512x );
    }

    return EOK;
}

int pcm512x_deinit( pcm512x_context_t* pcm512x )
{
    /* Put the DAC in standby mode (or reset?) */
    pcm512x_i2c_write ( pcm512x, PCM512X_POWER, PCM512X_RQST, 0 );

    if ( pcm512x->debug ) {
        pcm512x_register_dump ( pcm512x );
    }

    pcm512x_close_i2c_fd ( pcm512x );

    return EOK;
};


/* Mixer functionality */

static int32_t
pcm512x_vol_control (MIXER_CONTEXT_T * pcm512x, ado_mixer_delement_t * element, uint8_t set,
    uint32_t * vol, void *instance_data)
{
    uint8_t left_vol = pcm512x->left_vol;
    uint8_t right_vol = pcm512x->right_vol;
    int32_t altered = 0;
    (void)element;
    (void)instance_data;

    if ( set ) {
        if (vol[0] != pcm512x->left_vol) {
            altered = 1;
            left_vol = vol[0];
        }
        if ( !pcm512x->joint_vol ) {
            if (vol[1] != pcm512x->right_vol) {
                altered = 1;
                right_vol = vol[1];
            }
        } else {
            right_vol = left_vol;
        }
        /* set volume to hardware */
        if ( altered ) {
            if ( pcm512x_vol_set( pcm512x, left_vol, right_vol ) == EOK ) {
                pcm512x->left_vol = left_vol;
                pcm512x->right_vol = right_vol;
            } else {
                altered = 0;
            }
        }
    } else {
        vol[0] = pcm512x->left_vol;
        vol[1] = pcm512x->right_vol;
    }

    return altered;
}

static int32_t
pcm512x_mute_control (MIXER_CONTEXT_T * pcm512x, ado_mixer_delement_t * element, uint8_t set,
    uint32_t * mute, void *instance_data)
{
    int32_t altered = 0;
    (void)element;
    (void)instance_data;

    if (set) {
        altered = pcm512x->mute != *mute;
        if (altered) {
            if (pcm512x->codec_on) {
                if ( pcm512x_mute_set(pcm512x, *mute) == EOK ) {
                    pcm512x->mute = *mute;
                } else {
                    altered = 0;
                }
            } else {
                pcm512x->mute = *mute;
            }
        }
    } else {
        *mute = pcm512x->mute;
    }

    return altered;
}

static int32_t pcm512x_switch_get (MIXER_CONTEXT_T * pcm512x,
    ado_dswitch_t * dswitch, snd_switch_t * cswitch, void *instance_data)
{
    (void)dswitch;
    (void)instance_data;

    memcpy (&cswitch->value, &pcm512x->switch_value.value, sizeof (cswitch->value));
    return EOK;
}

static int32_t pcm512x_switch_set (MIXER_CONTEXT_T * pcm512x,
    ado_dswitch_t * dswitch, snd_switch_t * cswitch, void *instance_data)
{
    int32_t altered = 0;
    (void)dswitch;
    (void)instance_data;

    if (cswitch->value.string_11.selection != pcm512x->switch_value.value.string_11.selection) {
        if (pcm512x_analog_gain_set (pcm512x, cswitch->value.string_11.selection, cswitch->value.string_11.selection) == EOK) {
            altered = 1;
            pcm512x->switch_value.value.string_11.selection = cswitch->value.string_11.selection;
        } else {
            ado_error_fmt("Failed setting the analog gain status");
            altered = -1;
        }
    }

    return altered;
}

/*
 *  build_pcm512x_mixer
 */
static int32_t
build_pcm512x_mixer(MIXER_CONTEXT_T * pcm512x)
{
    static const char* string_11_strings[] = {"-6.0dB", "-5.2dB", "0.0dB", "0.8dB"};
    ado_mixer_delement_t *pre_elem, *vol_elem, *mute_elem, *elem = NULL;
    const snd_pcm_chmap_t *chmap = ado_pcm_get_default_chmap(2);
    uint32_t vol_voices = pcm512x->joint_vol ? 1 : 2;
    int32_t error = 0;

    ado_debug (DB_LVL_DRIVER, "pcm512x");

    /* ################# */
    /* the OUTPUT GROUPS */
    /* ################# */
    if (!error && (elem = ado_mixer_element_pcm1 (pcm512x->mixer, SND_MIXER_ELEMENT_PLAYBACK,
                SND_MIXER_ETYPE_PLAYBACK1, 1, &pcm_devices[0])) == NULL)
        error++;

    pre_elem = elem;

    if (!error && (vol_elem = ado_mixer_element_volume1 (pcm512x->mixer, "Digital Volume", vol_voices,
                pcm512x_volume_range, pcm512x_vol_control, NULL, NULL)) == NULL)
        error++;

    /* route pcm to volume */
    if (!error && ado_mixer_element_route_add(pcm512x->mixer, pre_elem, vol_elem) != 0)
        error++;

    pre_elem = vol_elem;

    if (!error && (mute_elem = ado_mixer_element_sw1 (pcm512x->mixer, "Digital Mute", 2,
                pcm512x_mute_control, NULL, NULL)) == NULL)
        error++;

    /* route volume to mute */
    if (!error && ado_mixer_element_route_add(pcm512x->mixer, pre_elem, mute_elem) != 0)
        error++;

    pre_elem = mute_elem;

    if (!error && (elem = ado_mixer_element_io (pcm512x->mixer, "DAC Output",
                SND_MIXER_ETYPE_OUTPUT, 0, chmap)) == NULL)
        error++;

    if (!error && ado_mixer_element_route_add (pcm512x->mixer, pre_elem, elem) != 0)
        error++;

    if (!error && ado_mixer_playback_group_create(pcm512x->mixer, SND_MIXER_MASTER_OUT,
                chmap, vol_elem, mute_elem) == NULL)
        error++;

    /* enumerated mixer switch (4 options) for analog gain */
    if (!error) {
        pcm512x->switch_value.value.string_11.strings_cnt = 4;
        for (int i = 0; i < pcm512x->switch_value.value.string_11.strings_cnt; i++) {
            strlcpy(pcm512x->switch_value.value.string_11.strings[i], string_11_strings[i],
                sizeof(pcm512x->switch_value.value.string_11.strings[i]));
        }
        if (ado_mixer_switch_new (pcm512x->mixer, "Analog Gain", SND_SW_TYPE_STRING_11, 0,
                                  pcm512x_switch_get, pcm512x_switch_set, pcm512x, NULL) == NULL) {
            error++;
        }
    }

    if (!error) {
        return EOK;
    }

    return ENOMEM;
}

static void
pcm512x_codec_set_default_group( ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index )
{
    switch (channel)
    {
        case ADO_PCM_CHANNEL_PLAYBACK:
            ado_pcm_chn_mixer (pcm, ADO_PCM_CHANNEL_PLAYBACK, mixer,
                ado_mixer_find_element (mixer, SND_MIXER_ETYPE_PLAYBACK1,
                    SND_MIXER_ELEMENT_PLAYBACK, index), ado_mixer_find_group (mixer,
                    SND_MIXER_MASTER_OUT, index));
            break;
        case ADO_PCM_CHANNEL_CAPTURE: /* no input group */
        default:
            break;
    }
}

static void pcm512x_codec_set_rate (ado_mixer_t *mixer, uint32_t sample_rate, int channel)
{
    pcm512x_context_t *pcm512x = ado_mixer_get_context(mixer);

    ado_debug ( DB_LVL_DRIVER, "Setting pcm512x sample rate to %d", sample_rate );

    pcm512x_rate_setting ( pcm512x, sample_rate );
}

static void pcm512x_codec_capabilities (ado_mixer_t *mixer, ado_mixer_dll_codec_capabilities_t *cap, int channel)
{
    (void) mixer;

    if (cap == NULL) {
        return;
    }

    memset (cap, 0, sizeof(*cap));
    /* rate value of 384000 also supported by PCM512X DAC, but not currently supported in QNX */
    cap->rates = SND_PCM_RATE_8000 | SND_PCM_RATE_11025 | SND_PCM_RATE_16000 | SND_PCM_RATE_22050 |
        SND_PCM_RATE_32000 | SND_PCM_RATE_44100 | SND_PCM_RATE_48000 | SND_PCM_RATE_64000 |
        SND_PCM_RATE_88200 | SND_PCM_RATE_96000 | SND_PCM_RATE_176400 | SND_PCM_RATE_192000;
    cap->formats = SND_PCM_FMT_S32 | SND_PCM_FMT_S24_4 | SND_PCM_FMT_S16;
    cap->min_voices = 2;
    cap->max_voices = 2;
}

static void pcm512x_codec_on(ado_mixer_t *mixer, int channel)
{
    (void)channel;
    pcm512x_context_t *pcm512x = ado_mixer_get_context(mixer);
    if (!pcm512x->codec_on) {
        pcm512x->codec_on = 1;

        if (pcm512x->mute != MUTE_ALL) {
            pcm512x_mute_set( pcm512x, pcm512x->mute );
        }
    }
}

static void pcm512x_codec_off(ado_mixer_t *mixer, int channel)
{
    (void)channel;
    pcm512x_context_t *pcm512x = ado_mixer_get_context(mixer);
    if (pcm512x->codec_on) {
        pcm512x->codec_on = 0;

        if (pcm512x->mute != MUTE_ALL) {
            pcm512x_mute_set( pcm512x, MUTE_ALL );
        }
    }
}

/* Option names are prefixed with codec name to prevent name collisions
 * with other options handled at higher levels in io-audio;
 */
static const char* opts[] = {
#define I2C_DEV              0
    "pcm512x_i2c_dev",
#define I2C_ADDR             1
    "pcm512x_i2c_addr",
#define JOINT_VOL            2
    "pcm512x_joint_vol",
#define ANALOG_GAIN          3
    "pcm512x_analog_gain",
#define GPIO_OUT1            4
    "pcm512x_gpio_out1",
#define GPIO_OUT2            5
    "pcm512x_gpio_out2",
#define GPIO_OUT3            6
    "pcm512x_gpio_out3",
#define GPIO_OUT4            7
    "pcm512x_gpio_out4",
#define GPIO_OUT5            8
    "pcm512x_gpio_out5",
#define GPIO_OUT6            9
    "pcm512x_gpio_out6",
#define DEBUG                10
    "pcm512x_debug",
    NULL
};

#define N_OPTS ((sizeof(opts)/sizeof(opts[0])) - 1U)
#define N_REQD_OPTS (I2C_ADDR+1)

static int
parse_config_opts ( MIXER_CONTEXT_T *pcm512x )
{
    const ado_dict_t *dict = NULL;
    const char* opt_str[N_OPTS] = {NULL};
    uint32_t opt_idx;
    long value;
    ado_card_t *card = ado_mixer_get_card ( pcm512x->mixer );

    dict = ado_get_card_dict (card);

    if (ado_config_load_key_values(dict, opts, opt_str, N_OPTS, N_REQD_OPTS, -1) != EOK) {
        ado_error_fmt( "Missing required keys" );
        return EINVAL;
    }

    if ( opt_str[I2C_DEV] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_DEV], NULL, 0 ) ) >= 0 ) {
            pcm512x->params.i2c_dev = value;
        } else {
            ado_error_fmt( "Incorrect value for option %s: %s", opts[I2C_DEV], opt_str[I2C_DEV] );
            return EINVAL;
        }
    }

    if ( opt_str[I2C_ADDR] != NULL ) {
        if ( ( value = strtol ( opt_str[I2C_ADDR], NULL, 0 ) ) != 0 ) {
            pcm512x->params.i2c_addr = value;
        } else {
            ado_error_fmt( "Incorrect value for option %s: %s", opts[I2C_ADDR], opt_str[I2C_ADDR] );
            return EINVAL;
        }
    }

    if (opt_str[JOINT_VOL] != NULL ) {
        if ( !strncasecmp ( opt_str[JOINT_VOL], "enable", strlen("enable") ) ) {
            pcm512x->joint_vol = true;
        }
    }

    if (opt_str[ANALOG_GAIN] != NULL ) {
        /* Read the float value, multiply by 10 and convert to int, to be able to compare
         * it with the possible analog gain values expressed as integers in 0.1 dB units;
         * we do this because the comparison of the float value read against float constants
         * is not reliable
         */
        int32_t gain = (int32_t) ( 10 * strtof ( opt_str[ANALOG_GAIN], NULL ) );
        if (gain == -62) {
            pcm512x->switch_value.value.string_11.selection = PCM512X_ANALOG_GAIN_MINUS6_DB;
        } else if (gain == -52) {
            pcm512x->switch_value.value.string_11.selection = PCM512X_ANALOG_GAIN_MINUS5PT2_DB;
        } else if (gain == 0) {
            pcm512x->switch_value.value.string_11.selection = PCM512X_ANALOG_GAIN_0_DB;
        } else if (gain == 8) {
            pcm512x->switch_value.value.string_11.selection = PCM512X_ANALOG_GAIN_0PT8_DB;
        } else {
            ado_error_fmt ( "Incorrect value for option %s: %s", opts[ANALOG_GAIN], opt_str[ANALOG_GAIN] );
            return EINVAL;
        }
    }

    for (opt_idx = GPIO_OUT1; opt_idx <= GPIO_OUT6; opt_idx++) {
        if (opt_str[opt_idx] != NULL ) {
            if ( !strncasecmp ( opt_str[opt_idx], "dsp", strlen("dsp") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_DSP;
            } else if ( !strncasecmp ( opt_str[opt_idx], "high", strlen("high") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_REG;
                pcm512x->gpio_val[opt_idx - GPIO_OUT1] = 1;
            } else if ( !strncasecmp ( opt_str[opt_idx], "low", strlen("low") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_REG;
            } else if ( !strncasecmp ( opt_str[opt_idx], "automute_b", strlen("automute_b") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_AMUTB;
            } else if ( !strncasecmp ( opt_str[opt_idx], "automute_l", strlen("automute_l") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_AMUTL;
            } else if ( !strncasecmp ( opt_str[opt_idx], "automute_r", strlen("automute_r") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_AMUTR;
            } else if ( !strncasecmp ( opt_str[opt_idx], "clkinval", strlen("clkinval") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_CLKI;
            } else if ( !strncasecmp ( opt_str[opt_idx], "sdout", strlen("sdout") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_SDOUT;
            } else if ( !strncasecmp ( opt_str[opt_idx], "anamute_l", strlen("anamute_l") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_ANMUL;
            } else if ( !strncasecmp ( opt_str[opt_idx], "anamute_r", strlen("anamute_r") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_ANMUR;
            } else if ( !strncasecmp ( opt_str[opt_idx], "plllock", strlen("plllock") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_PLLLK;
            } else if ( !strncasecmp ( opt_str[opt_idx], "cpclk", strlen("cpclk") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_CPCLK;
            } else if ( !strncasecmp ( opt_str[opt_idx], "undervolt0_7", strlen("undervolt0_7") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_UV0_7;
            } else if ( !strncasecmp ( opt_str[opt_idx], "undervolt0_3", strlen("undervolt0_3") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_UV0_3;
            } else if ( !strncasecmp ( opt_str[opt_idx], "pllout", strlen("pllout") ) ) {
                pcm512x->gpio_sel[opt_idx - GPIO_OUT1] = PCM512X_GXSL_PLLCK;
            } else {
                ado_error_fmt ( "Incorrect value for option %s: %s", opts[opt_idx], opt_str[opt_idx] );
                return EINVAL;
            }
            if ( strstr ( opt_str[opt_idx], "_inv" ) ) {
                pcm512x->gpio_val[opt_idx - GPIO_OUT1] = 1;
            }
        }
    }

    if (opt_str[DEBUG] != NULL) {
        if ( !strncasecmp ( opt_str[DEBUG], "enable", strlen("enable") ) ) {
            pcm512x->debug = true;
        }
    }

    /* do a basic validation of the options: i2c_dev and i2c_addr must be >= 0 */
    if ( pcm512x->params.i2c_dev < 0 || pcm512x->params.i2c_addr < 0 ) {
        ado_error_fmt ( "Incorrect I2C params: i2c_dev %d, i2c_addr %d",
                        pcm512x->params.i2c_dev, pcm512x->params.i2c_addr );
        return EINVAL;
    }

    return EOK;
}

ado_mixer_dll_init_t mixer_dll_init;
int mixer_dll_init (MIXER_CONTEXT_T ** context, ado_mixer_t * mixer, void *params, void *raw_callbacks, int version)
{
    pcm512x_context_t *pcm512x;
    ado_mixer_dll_codec_callbacks_t *callbacks = raw_callbacks;

    ado_debug(DB_LVL_MIXER, "Initializing pcm512x Codec");

    if ( (pcm512x = (pcm512x_context_t *) ado_calloc(1, sizeof (pcm512x_context_t))) == NULL ) {
        ado_error_fmt( "pcm512x: Failed to allocate device structure - %s", strerror(errno) );
        return (-1);
    }
    *context = pcm512x;
    ado_mixer_set_name ( mixer, "pcm512x" );

    pcm512x->mixer = mixer;
    memcpy ( &pcm512x->params, params, sizeof (pcm512x->params) );

    /* Initialize the hardware context */
    pcm512x->i2c_fd = -1;
    pcm512x->joint_vol = false;
    pcm512x->codec_on = 1;
    pcm512x->debug = false;
    for (int i = 0; i < PCM512X_GPIOS; i++) {
        pcm512x->gpio_sel[i] = PCM512X_GXSL_OFF;
        pcm512x->gpio_val[i] = 0;
    }

    /* Initialize the cache value for analog gain, digital volume and mute status to
     * 0dB and unmuted; the initial analog gain can be overridden via driver options
     */
    pcm512x->switch_value.value.string_11.selection = PCM512X_ANALOG_GAIN_0_DB;
    pcm512x->left_vol = 255 - PCM512X_DIGITAL_VOLUME_0_DB;
    pcm512x->right_vol = 255 - PCM512X_DIGITAL_VOLUME_0_DB;
    pcm512x->mute = 0;

    if ( pcm512x->params.is_active == NULL ) {
        ado_error_fmt ( "pcm512x: is_active routine missing" );
        ado_free( pcm512x );
        return (-1);
    }

    if ( parse_config_opts(pcm512x) != EOK ) {
        ado_error_fmt("Failed parsing pcm512x options");
        ado_free(pcm512x);
        return -1;
    }

    if (callbacks) {
        callbacks->codec_set_default_group = pcm512x_codec_set_default_group;
        callbacks->codec_set_rate = pcm512x_codec_set_rate;
        callbacks->codec_capabilities = pcm512x_codec_capabilities;
        callbacks->codec_on = pcm512x_codec_on;
        callbacks->codec_off = pcm512x_codec_off;
    }

    if ( build_pcm512x_mixer(pcm512x) != EOK ) {
        ado_error_fmt("Failed building pcm512x mixer");
        ado_free(pcm512x);
        return -1;
    }

    if ( pcm512x_init(pcm512x) != EOK ) {
        ado_error_fmt("Failed to init pcm512x");
        ado_free(pcm512x);
        return -1;
    }

    ado_mixer_set_reset_func ( pcm512x->mixer, pcm512x_init );
    ado_mixer_set_destroy_func ( pcm512x->mixer, pcm512x_deinit );

    return 0;
}

ado_dll_version_t version;
void
version (int *major, int *minor, const char **date)
{
	*major = ADO_MAJOR_VERSION;
	*minor = I2S_CODEC_MINOR_VERSION;
	*date = __DATE__;
}


int pcm512x_rate_setting( pcm512x_context_t* pcm512x, uint32_t sample_rate )
{
    (void)pcm512x;
    (void)sample_rate;

    /* Nothing to do as long as codec in master clock mode not supported,
     * add PLL programming if/when master clock mode support is required
     */

    return EOK;
}

void pcm512x_register_dump( pcm512x_context_t* pcm512x )
{
    int status = EOK;
    uint8_t reg_val = 0;

    status = pcm512x_i2c_read ( pcm512x, PCM512X_RESET, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_RESET register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_RESET - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_POWER, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_POWER register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_POWER - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_MUTE, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_MUTE register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_MUTE - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_PLL_REF, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_PLL_REF register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_PLL_REF - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_I2S_1, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_I2S_1 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_I2S_1 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_ERROR_DETECT, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_ERROR_DETECT register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_ERROR_DETECT - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_DIGITAL_VOLUME_1, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_DIGITAL_VOLUME_1 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_DIGITAL_VOLUME_1 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_DIGITAL_VOLUME_2, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_DIGITAL_VOLUME_2 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_DIGITAL_VOLUME_2 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_DIGITAL_VOLUME_3, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_DIGITAL_VOLUME_3 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_DIGITAL_VOLUME_3 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_GPIO_EN, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_GPIO_EN register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_GPIO_EN - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_GPIO_OUTPUT_4, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_GPIO_OUTPUT_4 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_GPIO_OUTPUT_4 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_GPIO_CONTROL_1, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_GPIO_CONTROL_1 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_GPIO_CONTROL_1 - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_GPIO_CONTROL_2, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_GPIO_CONTROL_2 register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_GPIO_CONTROL_2 - 0x%02x", reg_val );

    /* Select page 1 */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 1, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed setting page 1");
        return;
    }

    status = pcm512x_i2c_read ( pcm512x, PCM512X_ANALOG_GAIN_CTRL, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_ANALOG_GAIN_CTRL register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_ANALOG_GAIN_CTRL - 0x%02x", reg_val );

    status = pcm512x_i2c_read ( pcm512x, PCM512X_ANALOG_GAIN_BOOST, &reg_val, 0 );
    if ( status != EOK ) {
        ado_error_fmt("Failed reading PCM512X_ANALOG_GAIN_BOOST register");
        return;
    }
    ado_debug ( DB_LVL_DRIVER, "PCM512X_ANALOG_GAIN_BOOST - 0x%02x", reg_val );

    /* Select page 0 */
    status = pcm512x_i2c_write ( pcm512x, PCM512X_PAGE, 0, 0 );
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/mixer/pcm512x/pcm512x.c $ $Rev: 927535 $")
#endif
