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

#ifndef _PCM512X_H
#define _PCM512X_H

#include <stdint.h>
#include <mixer/i2s_codec_dll.h>

/* Page register */
#define PCM512X_PAGE              0

/* Page 0 registers */
#define PCM512X_RESET             1
    #define PCM512X_RSTR (1 << 0)
    #define PCM512X_RSTM (1 << 4)
#define PCM512X_POWER             2
    #define PCM512X_RQPD       (1 << 0)
    #define PCM512X_RQST       (1 << 4)
#define PCM512X_MUTE              3
    #define PCM512X_RQMR (1 << 0)
    #define PCM512X_RQML (1 << 4)
#define PCM512X_PLL_EN            4
    #define PCM512X_PLLE       (1 << 0)
    #define PCM512X_PLCK       (1 << 4)
#define PCM512X_SPI_MISO_FUNCTION 6
#define PCM512X_DSP               7
    #define PCM512X_SDSL       (1 << 0)
    #define PCM512X_DEMP       (1 << 4)
#define PCM512X_GPIO_EN           8
    #define PCM512X_G1OE       (1 << 0)
    #define PCM512X_G2OE       (1 << 1)
    #define PCM512X_G3OE       (1 << 2)
    #define PCM512X_G4OE       (1 << 3)
    #define PCM512X_G5OE       (1 << 4)
    #define PCM512X_G6OE       (1 << 5)
#define PCM512X_BCLK_LRCLK_CFG    9
    #define PCM512X_LRKO       (1 << 0)
    #define PCM512X_BCKO       (1 << 4)
    #define PCM512X_BCKP       (1 << 5)
#define PCM512X_DSP_GPIO_INPUT    10
#define PCM512X_MASTER_MODE       12
    #define PCM512X_RLRK       (1 << 0)
    #define PCM512X_RBCK       (1 << 1)
#define PCM512X_PLL_REF           13
    #define PCM512X_SREF_MSK    (7 << 4)
    #define PCM512X_SREF_SCK    (0 << 4)
    #define PCM512X_SREF_BCK    (1 << 4)
    #define PCM512X_SREF_GPIO   (3 << 4)
#define PCM512X_DAC_REF           14
    #define PCM512X_SDAC_MASK   (7 << 4)
    #define PCM512X_SDAC_MCK    (0 << 4)
    #define PCM512X_SDAC_PLL    (1 << 4)
    #define PCM512X_SDAC_SCK    (3 << 4)
    #define PCM512X_SDAC_BCK    (4 << 4)
    #define PCM512X_SDAC_GPIO   (5 << 4)
#define PCM512X_GPIO_DACIN        16
#define PCM512X_GPIO_PLLIN        18
    /* Register 16-18, GPIO source for DAC, PLL */
    #define PCM512X_GREF_MASK   (7 << 0)
    #define PCM512X_GREF_GPIO1  (0 << 0)
    #define PCM512X_GREF_GPIO2  (1 << 0)
    #define PCM512X_GREF_GPIO3  (2 << 0)
    #define PCM512X_GREF_GPIO4  (3 << 0)
    #define PCM512X_GREF_GPIO5  (4 << 0)
    #define PCM512X_GREF_GPIO6  (5 << 0)
#define PCM512X_SYNCHRONIZE       19
    #define PCM512X_RQSY        (1 << 0)
    #define PCM512X_RQSY_RESUME (0 << 0)
    #define PCM512X_RQSY_HALT   (1 << 0)
#define PCM512X_PLL_COEFF_0       20
#define PCM512X_PLL_COEFF_1       21
#define PCM512X_PLL_COEFF_2       22
#define PCM512X_PLL_COEFF_3       23
#define PCM512X_PLL_COEFF_4       24
#define PCM512X_DSP_CLKDIV        27
#define PCM512X_DAC_CLKDIV        28
#define PCM512X_NCP_CLKDIV        29
#define PCM512X_OSR_CLKDIV        30
#define PCM512X_MASTER_CLKDIV_1   32
#define PCM512X_MASTER_CLKDIV_2   33
#define PCM512X_FS_SPEED_MODE     34
    #define PCM512X_FSSP_MSK    (3 << 0)
    #define PCM512X_FSSP_48KHZ  (0 << 0)
    #define PCM512X_FSSP_96KHZ  (1 << 0)
    #define PCM512X_FSSP_192KHZ (2 << 0)
    #define PCM512X_FSSP_384KHZ (3 << 0)
#define PCM512X_IDAC_1            35
#define PCM512X_IDAC_2            36
#define PCM512X_ERROR_DETECT      37
    #define PCM512X_IPLK (1 << 0)
    #define PCM512X_DCAS (1 << 1)
    #define PCM512X_IDCM (1 << 2)
    #define PCM512X_IDCH (1 << 3)
    #define PCM512X_IDSK (1 << 4)
    #define PCM512X_IDBK (1 << 5)
    #define PCM512X_IDFS (1 << 6)
#define PCM512X_I2S_1             40
    #define PCM512X_ALEN_MSK   (3 << 0)
    #define PCM512X_ALEN_16    (0 << 0)
    #define PCM512X_ALEN_20    (1 << 0)
    #define PCM512X_ALEN_24    (2 << 0)
    #define PCM512X_ALEN_32    (3 << 0)
    #define PCM512X_AFMT_MSK   (3 << 4)
    #define PCM512X_AFMT_I2S   (0 << 4)
    #define PCM512X_AFMT_DSP   (1 << 4)
    #define PCM512X_AFMT_RTJ   (2 << 4)
    #define PCM512X_AFMT_LTJ   (3 << 4)
#define PCM512X_I2S_2             41
#define PCM512X_DAC_ROUTING       42
    #define PCM512X_AUPR_MSK   (3 << 0)
    #define PCM512X_AUPL_MSK   (3 << 4)
#define PCM512X_DSP_PROGRAM       43
#define PCM512X_CLKDET            44
#define PCM512X_AUTO_MUTE         59
    #define PCM512X_ATMR_MSK   (7 << 0)
    #define PCM512X_ATML_MASK  (7 << 4)
#define PCM512X_DIGITAL_VOLUME_1  60
    #define PCM512X_PCTL_MASK  (3 << 0)
#define PCM512X_DIGITAL_VOLUME_2  61
#define PCM512X_DIGITAL_VOLUME_3  62
    #define PCM512X_DIGITAL_VOLUME_0_DB 0x30
#define PCM512X_DIGITAL_MUTE_1    63
    #define PCM512X_VNUS_MSK   (3 << 0)
    #define PCM512X_VNUF_MSK   (3 << 2)
    #define PCM512X_VNDS_MSK   (3 << 4)
    #define PCM512X_VNDF_MSK   (3 << 6)
#define PCM512X_DIGITAL_MUTE_2    64
    #define PCM512X_VEDS_MSK   (3 << 4)
    #define PCM512X_VEDF_MSK   (3 << 6)
#define PCM512X_DIGITAL_MUTE_3    65
    #define PCM512X_AMRE       (1 << 0)
    #define PCM512X_AMLE       (1 << 1)
    #define PCM512X_ACTL       (1 << 2)
#define PCM512X_GPIO_OUTPUT_1     80
#define PCM512X_GPIO_OUTPUT_2     81
#define PCM512X_GPIO_OUTPUT_3     82
#define PCM512X_GPIO_OUTPUT_4     83
#define PCM512X_GPIO_OUTPUT_5     84
#define PCM512X_GPIO_OUTPUT_6     85
    /* Register 80-85, GPIO output selection */
    #define PCM512X_GXSL_MSK   (31 << 0)
    #define PCM512X_GXSL_OFF   (0 << 0)
    #define PCM512X_GXSL_DSP   (1 << 0)
    #define PCM512X_GXSL_REG   (2 << 0)
    #define PCM512X_GXSL_AMUTB (3 << 0)
    #define PCM512X_GXSL_AMUTL (4 << 0)
    #define PCM512X_GXSL_AMUTR (5 << 0)
    #define PCM512X_GXSL_CLKI  (6 << 0)
    #define PCM512X_GXSL_SDOUT (7 << 0)
    #define PCM512X_GXSL_ANMUL (8 << 0)
    #define PCM512X_GXSL_ANMUR (9 << 0)
    #define PCM512X_GXSL_PLLLK (10 << 0)
    #define PCM512X_GXSL_CPCLK (11 << 0)
    #define PCM512X_GXSL_UV0_7 (14 << 0)
    #define PCM512X_GXSL_UV0_3 (15 << 0)
    #define PCM512X_GXSL_PLLCK (16 << 0)
#define PCM512X_GPIO_CONTROL_1    86
    #define PCM512X_GPIO_GOUT0 (1 << 0)
    #define PCM512X_GPIO_GOUT1 (1 << 1)
    #define PCM512X_GPIO_GOUT2 (1 << 2)
    #define PCM512X_GPIO_GOUT3 (1 << 3)
    #define PCM512X_GPIO_GOUT4 (1 << 4)
    #define PCM512X_GPIO_GOUT5 (1 << 5)
#define PCM512X_GPIO_CONTROL_2    87
#define PCM512X_OVERFLOW          90
#define PCM512X_RATE_DET_1        91
#define PCM512X_RATE_DET_2        92
#define PCM512X_RATE_DET_3        93
#define PCM512X_RATE_DET_4        94
#define PCM512X_CLOCK_STATUS      95
#define PCM512X_ANALOG_MUTE_DET   108
#define PCM512X_GPIN              119
#define PCM512X_DIGITAL_MUTE_DET  120

/* Page 1 registers */
#define PCM512X_OUTPUT_AMPLITUDE  1
#define PCM512X_ANALOG_GAIN_CTRL  2
    #define PCM512X_RAGN       (1 << 0)
    #define PCM512X_LAGN       (1 << 4)
#define PCM512X_UNDERVOLTAGE_PROT 5
#define PCM512X_ANALOG_MUTE_CTRL  6
#define PCM512X_ANALOG_GAIN_BOOST 7
    #define PCM512X_AGBR       (1 << 0)
    #define PCM512X_AGBL       (1 << 4)
#define PCM512X_VCOM_CTRL_1       8
#define PCM512X_VCOM_CTRL_2       9

/* Page 44 registers */
#define PCM512X_CRAM_CTRL         1

/* Page 253 registers */
#define PCM512X_FLEX_A            63
#define PCM512X_FLEX_B            64

/* Analog gain enumerated values */
#define PCM512X_ANALOG_GAIN_MINUS6_DB    0
#define PCM512X_ANALOG_GAIN_MINUS5PT2_DB 1
#define PCM512X_ANALOG_GAIN_0_DB         2
#define PCM512X_ANALOG_GAIN_0PT8_DB      3

/* Number of GPIOs */
#define PCM512X_GPIOS                    6

typedef struct pcm512x_context{
    ado_mixer_t                  *mixer;
    ado_mixer_dll_codec_params_t params;
    int32_t                      i2c_fd;
    bool                         joint_vol;
    uint8_t                      left_vol;
    uint8_t                      right_vol;
    uint8_t                      mute;
    uint8_t                      codec_on;
    snd_switch_t                 switch_value;
    uint8_t                      gpio_sel[PCM512X_GPIOS]; /* a PCM512X_GXSL_* value */
    uint8_t                      gpio_val[PCM512X_GPIOS];
    bool                         debug;
} pcm512x_context_t;

int pcm512x_init( pcm512x_context_t* pcm512x );
int pcm512x_deinit( pcm512x_context_t* pcm512x );
int pcm512x_vol_get( pcm512x_context_t* pcm512x, uint8_t* left_vol, uint8_t* right_vol );
int pcm512x_vol_set( pcm512x_context_t* pcm512x, uint8_t left_vol, uint8_t right_vol );
int pcm512x_analog_gain_get( pcm512x_context_t* pcm512x, uint8_t* left_gain, uint8_t* right_gain );
int pcm512x_analog_gain_set( pcm512x_context_t* pcm512x, uint8_t left_gain, uint8_t right_gain );
int pcm512x_mute_set( pcm512x_context_t* pcm512x, uint8_t mute );
int pcm512x_mute_get( pcm512x_context_t* pcm512x, uint8_t* mute );

int pcm512x_rate_setting( pcm512x_context_t* pcm512x, uint32_t sample_rate );
int pcm512x_capabilities( pcm512x_context_t* pcm512x, ado_mixer_dll_codec_capabilities_t *cap, int channel );
void pcm512x_register_dump( pcm512x_context_t* pcm512x );

#endif /* _PCM512X_H */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/mixer/pcm512x/pcm512x.h $ $Rev: 925119 $")
#endif
