/*
 * $QNXLicenseC:
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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

#ifndef WM8960_H_
#define WM8960_H_

#include <audio_driver.h>

#ifndef WM8960_SPKVDD
    /** WM8960 Speaker Power Supply voltage level in mV */
    #define WM8960_SPKVDD                       5050
#endif

/** WM8960 Analog Power Supply voltage level in mV */
#ifndef WM8960_AVDD
    #define WM8960_AVDD                         3300
#endif

/** Defines threshold in mV for analog bias increasing. Bias is increased
 * when Analog Power Supply voltage level is below or equal to this threshold.
 */
#ifndef WM8960_ANALOG_BIAS_THRESH
    #define WM8960_ANALOG_BIAS_THRESH           3000
#endif

#ifndef WM8960_DEFAULT_I2C_NUM
    #define WM8960_DEFAULT_I2C_NUM              10
#endif

#ifndef WM8960_DEFAULT_I2C_ADDR
    #define WM8960_DEFAULT_I2C_ADDR              0x34 >> 1
#endif

/** WM8960 I2C 7-bit slave address */
#define WM8960_SLAVE_ADDR                       (0x34 >> 1)

#define MAX_I2C_PORT                            20

#define SPEAKER_ENABLE                          0x1
#define HEADPHONE_ENABLE                        0x2

/*
 * WM8960 CODEC Registers
 */
/* Left and Right Input Volume */
#define WM8960_INPUT_VOL_L                      0x00 /* R0 */
#define WM8960_INPUT_VOL_MAX                    0x3F
#define WM8960_INPUT_VOL_MIN                    0x00
#define WM8960_INPUT_VOL_DEFAULT                0x17
#define WM8960_INPUT_VOL_R                      0x01 /* R1 */
#define WM8960_INPUT_VOL_UPDATE_MASK            (1 << 8)
#define WM8960_INPUT_VOL_MUTE_MASK              (1 << 7)
#define WM8960_INPUT_VOL_ZERO_MASK              (1 << 6)
#define WM8960_INPUT_VOL_CTRL_MASK              (0x3F)
/* Left and Right Headphone Volume */
#define WM8960_HPOUT_VOL_L                      0x02 /* R2 */
#define WM8960_HPOUT_VOL_R                      0x03 /* R3 */
#define WM8960_HPOUT_VOL_UPDATE_MASK            (1 << 8)
#define WM8960_HPOUT_VOL_ZERO_MASK              (1 << 7)
#define WM8960_HPOUT_VOL_CTRL_MASK              0x7F
#define WM8960_HPOUT_VOL_MUTE_VAL               0x00
#define WM8960_HPOUT_VOL_MIN                    0x30
#define WM8960_HPOUT_VOL_MAX                    WM8960_HPOUT_VOL_CTRL_MASK
/* Clock Control Register */
#define WM8960_CLOCKING_1                       0x04 /* R4 */
#define WM8960_CLOCKING_1_ADCDIV_SHIFT          6
#define WM8960_CLOCKING_1_ADCDIV_MASK           (0x7 << WM8960_CLOCKING_1_ADCDIV_SHIFT)
#define WM8960_CLOCKING_1_DACDIV_SHIFT          3
#define WM8960_CLOCKING_1_DACDIV_MASK           (0x7 << WM8960_CLOCKING_1_DACDIV_SHIFT)
#define WM8960_CLOCKING_1_SYSCLKDIV_MASK        (0x3 << 1)
#define WM8960_CLOCKING_1_CLKSEL_MASK           (1)
/* ADC and DAC Control Registers */
#define WM8960_ADCDAC_CTL_1                     0x05 /* R5 */
#define WM8960_ADCDAC_CTL_1_DACDIV2_MASK        (1 << 7)
#define WM8960_ADCDAC_CTL_1_ADCPOL_MASK         (0x3 << 5)
#define WM8960_ADCDAC_CTL_1_DACMUTE_MASK        (1 << 3)
#define WM8960_ADCDAC_CTL_1_DEEMPH_MASK         (0x3 << 1)
#define WM8960_ADCDAC_CTL_1_ADCHPD_MASK         (1)
#define WM8960_ADCDAC_CTL_2                     0x06 /* R6 */
#define WM8960_ADCDAC_CTL_2_DACPOL_MASK         (3 << 5)
#define WM8960_ADCDAC_CTL_2_DACSMM_MASK         (1 << 3)
#define WM8960_ADCDAC_CTL_2_DACMR_MASK          (1 << 2)
#define WM8960_ADCDAC_CTL_2_DACSLOPE_MASK       (1 << 1)
/* Audio Interface Register */
#define WM8960_AUDIO_INTERF_0                   0x07 /* R7 */
#define WM8960_AUDIO_INTERF_0_ALRSWAP_MASK      (1 << 8)
#define WM8960_AUDIO_INTERF_0_BCLKINV_MASK      (1 << 7)
#define WM8960_AUDIO_INTERF_0_MS_MASK           (1 << 6)
#define WM8960_AUDIO_INTERF_0_DLRSWAP_MASK      (1 << 5)
#define WM8960_AUDIO_INTERF_0_LRP_MASK          (1 << 4)
#define WM8960_AUDIO_INTERF_0_WL_MASK           (0x3 << 2)
#define WM8960_AUDIO_INTERF_0_WL_32             (0x3 << 2)
#define WM8960_AUDIO_INTERF_0_WL_24             (0x2 << 2)
#define WM8960_AUDIO_INTERF_0_WL_16             (0x0 << 2)
#define WM8960_AUDIO_INTERF_0_FORMAT_MASK       (0x3)
#define WM8960_AUDIO_INTERF_0_FORMAT_I2S        (0x2)
#define WM8960_AUDIO_INTERF_0_FORMAT_DSP        (0x3)
/* Clock Control Register */
#define WM8960_CLOCKING_2                       0x08 /* R8 */
#define WM8960_CLOCKING_2_DCLKDIV_SHIFT         6
#define WM8960_CLOCKING_2_DCLKDIV_MASK          (0x7 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_2             (1 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_3             (2 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_4             (3 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_6             (4 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_8             (5 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_12            (6 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_DCLKDIV_16            (7 << WM8960_CLOCKING_2_DCLKDIV_SHIFT)
#define WM8960_CLOCKING_2_BCLKDIV_MASK          (0xF)
/* Audio Interface Register */
#define WM8960_AUDIO_INTERF_1                   0x09 /* R9 */
#define WM8960_AUDIO_INTERF_1_LOOPBACK          (1 << 0)
#define WM8960_AUDIO_INTERF_1_ALRCGPIO_MASK     (1 << 6)
#define WM8960_AUDIO_INTERF_1_ALRCGPIO          (1 << 6)
/* DAC Volume Register */
#define WM8960_DAC_VOL_L                        0x0A /* R10 */
#define WM8960_DAC_VOLUME_DEFAULT               0xFF
#define WM8960_DAC_VOL_MAX                      0xFF
#define WM8960_DAC_VOL_MIN                      0x01
#define WM8960_DAC_VOL_CTRL_MASK                0x1FF
#define WM8960_DAC_VOL_MUTE_VAL                 0x00
#define WM8960_DAC_VOL_UPDATE_MASK              (1 << 8)
#define WM8960_DAC_VOL_R                        0x0B /* R11 */
/* Software Reset Register */
#define WM8960_SOFTWARE_RESET                   0x0F /* R15 */
/* 3D Control Register */
#define WM8960_DAC_3D_CTL                       0x10 /* R16 */
/* Automatic Level Control Registers */
#define WM8960_ALC_CTL_1                        0x11 /* R17 */
#define WM8960_ALC_CTL_1_DEFAULT                0x0B
#define WM8960_ALC_CTL_2                        0x12 /* R18 */
#define WM8960_ALC_CTL_3                        0x13 /* R19 */
#define WM8960_ALC_CTL_3_DECAY_192MS            (0x3 << 4)
#define WM8960_ALC_CTL_3_ATTACK_24MS            (0x2)
/* Automatic Level Control - Noise Gate function */
#define WM8960_NOISE_GATE                       0x14 /* R20 */
/* Left ADC Volume Register */
#define WM8960_ADC_L_VOL                        0x15 /* R21 */
#define WM8960_ADC_VOL_DEFAULT                  0xC3
#define WM8960_ADC_VOL_MAX                      0xFF
#define WM8960_ADC_VOL_MIN                      0x01
#define WM8960_ADC_VOL_CTRL_MASK                0xFF
#define WM8960_ADC_VOL_MUTE_VAL                 0x00
#define WM8960_ADC_VOL_UPDATE_MASK              (1 << 8)
/* Right ADC Volume Register */
#define WM8960_ADC_R_VOL                        0x16 /* R22 */
/* Additional Control Register 1 */
#define WM8960_ADDITIONAL_CTRL_1                0x17 /* R23 */
#define WM8960_ADDITIONAL_CTRL_1_TSDEN_MASK     (1 << 8)
#define WM8960_ADDITIONAL_CTRL_1_VSEL_MASK      (0x3 << 6)
#define WM8960_ADDITIONAL_CTRL_1_VSEL_HBIAS     (1 << 6)
#define WM8960_ADDITIONAL_CTRL_1_DMONOMIX_MASK  (1 << 4)
#define WM8960_ADDITIONAL_CTRL_1_DATSEL_MASK    (0x3 << 2)
#define WM8960_ADDITIONAL_CTRL_1_DATSEL_LEFT_RIGHT  (0x0 << 2)
#define WM8960_ADDITIONAL_CTRL_1_DATSEL_LEFT_LEFT   (0x1 << 2)
#define WM8960_ADDITIONAL_CTRL_1_DATSEL_RIGHT_RIGHT (0x2 << 2)
#define WM8960_ADDITIONAL_CTRL_1_TOCLKSEL_MASK  (1 << 1)
#define WM8960_ADDITIONAL_CTRL_1_TOEN_MASK      (1)
/* Additional Control Register 2 */
#define WM8960_ADDITIONAL_CTRL_2                0x18 /* R24 */
#define WM8960_ADDITIONAL_CTRL_2_HPSWEN_MASK    (1 << 6)
#define WM8960_ADDITIONAL_CTRL_2_HPSWPOL_MASK   (1 << 5)
#define WM8960_ADDITIONAL_CTRL_2_TRIS_MASK      (1 << 3)
#define WM8960_ADDITIONAL_CTRL_2_LRCM_MASK      (1 << 2)
/* Power Management Register 1 */
#define WM8960_PWR_MGMT_1                       0x19 /* R25 */
#define WM8960_PWR_MGMT_1_MICB_SHIFT            1
#define WM8960_PWR_MGMT_1_MICB_MASK             (1 << WM8960_PWR_MGMT_1_MICB_SHIFT)
#define WM8960_PWR_MGMT_1_VMIDSEL_MASK          (0x3 << 7)
#define WM8960_PWR_MGMT_1_VMIDSEL_250k          (1 << 8)
#define WM8960_PWR_MGMT_1_VMIDSEL_50k           (1 << 7)
#define WM8960_PWR_MGMT_1_VREF_MASK             (1 << 6)
#define WM8960_PWR_MGMT_1_AINL_MASK             (1 << 5)
#define WM8960_PWR_MGMT_1_AINR_MASK             (1 << 4)
#define WM8960_PWR_MGMT_1_ADCL_MASK             (1 << 3)
#define WM8960_PWR_MGMT_1_ADCR_MASK             (1 << 2)
/* Power Management Register 2 */
#define WM8960_PWR_MGMT_2                       0x1A /* R26 */
#define WM8960_PWR_MGMT_2_DACL                  (1 << 8)
#define WM8960_PWR_MGMT_2_DACR                  (1 << 7)
#define WM8960_PWR_MGMT_2_LOUT1                 (1 << 6)
#define WM8960_PWR_MGMT_2_ROUT1                 (1 << 5)
#define WM8960_PWR_MGMT_2_SPKL                  (1 << 4)
#define WM8960_PWR_MGMT_2_SPKR                  (1 << 3)
#define WM8960_PWR_MGMT_2_OUT3                  (1 << 1)
/* Additional Control Register 3 */
#define WM8960_ADDITIONAL_CTRL_3                0x1B /* R27 */
#define OUT3CAP                 (1<<3)
#define SAMPLE_MODE_MASK        (0x1<<4)
#define SAMPLE_INT_MODE         (1<<4)
#define SAMPLE_FRAC_MODE        (0<<4)
#define SAMPLE_RATE_MASK        (0x7<<0)
#define SAMPLE_RATE_32KHZ       (0x1) // Mode 1 (INT)
#define SAMPLE_RATE_44KHZ       (0x0) // Mode 0 (FRACTIONAL) (44.1Khz)
#define SAMPLE_RATE_48KHZ       (0x0) // Mode 1 (INT)
#define SAMPLE_RATE_96KHZ       (0x6) // Mode 1 (INT)

/* Anti-Pop Register */
#define WM8960_ANTI_POP_1                       0x1C /* R28 */
#define ANTI_POP_1_POBCTRL      (1<<7)
#define ANTI_POP_1_BUFDCOPEN    (1<<4)
#define ANTI_POP_1_BUFIOEN      (1<<3)
#define ANTI_POP_1_SOFT_ST      (1<<2)
#define ANTI_POP_1_HPSTBY       (1<<0)
/* Anti-Pop Register */
#define WM8960_ANTI_POP_2                       0x1D /* R29 */
/* ADCL Signal Path Register */
#define WM8960_ADC_L_SIG_PATH                   0x20 /* R32 */
#define WM8960_ADC_L_SIG_PATH_LMN1_MASK         (1 << 8)
#define WM8960_ADC_L_SIG_PATH_LMP3_MASK         (1 << 7)
#define WM8960_ADC_L_SIG_PATH_LMP2_MASK         (1 << 6)
#define WM8960_ADC_L_LMIC2B_MASK                (1 << 3)
/* ADCR Signal Path Register */
#define WM8960_ADC_R_SIG_PATH                   0x21 /* R32 */
#define WM8960_ADC_R_SIG_PATH_RMN1_MASK         (1 << 8)
#define WM8960_ADC_R_SIG_PATH_RMP3_MASK         (1 << 7)
#define WM8960_ADC_R_SIG_PATH_RMP2_MASK         (1 << 6)
#define WM8960_ADC_R_RMIC2B_MASK                (1 << 3)
/* Left Out Mix */
#define WM8960_OUT_L_MIX                        0x22 /* R34 */
#define WM8960_OUT_MIX_DEFAULT                  0x50
#define WM8960_OUT_MIX_DAC2MIX                  (1 << 8)
#define WM8960_OUT_MIX_LINE2MIX                 (1 << 7)
/* Right Out Mix */
#define WM8960_OUT_R_MIX                        0x25 /* R37 */
/* Mono Out Mix 1 */
#define WM8960_MONO_OUT_MIX_1                   0x26 /* R38 */
#define WM8960_MONO_OUT_MIX_1_L2MO              (1 << 8)
/* Mono Out Mix 2 */
#define WM8960_MONO_OUT_MIX_2                   0x27 /* R39 */
#define WM8960_MONO_OUT_MIX_2_R2MO              (1 << 8)
/* Left Speaker Volume */
#define WM8960_SPKOUT_VOL_L                     0x28 /* R40 */
#define WM8960_SPKOUT_VOL_MASK                  0x7F
#define WM8960_SPKOUT_VOL_MIN                   0x30
#define WM8960_SPKOUT_VOL_MAX                   WM8960_SPKOUT_VOL_MASK
#define WM8960_SPKOUT_VOL_UPDATE_MASK           (1 << 8)
#define WM8960_SPKOUT_VOL_ZERO_MASK             (1 << 7)
#define WM8960_SPKOUT_VOL_CTRL_MASK             (0x7F)
#define WM8960_SPKOUT_VOL_MUTE_VAL              0x00
/* Right Speaker Volume */
#define WM8960_SPKOUT_VOL_R                     0x29 /* R41 */
/* Out 3 Volume */
#define WM8960_OUT_3_VOL                        0x2A /* R42 */
#define WM8960_OUT_3_VOL_MOUTVOL                (1 << 6)
#define WM8960_OUT_3_VOL_MOUTVOL_MASK           (1 << 6)
/* Left Input Boost Mixer */
#define WM8960_INPUT_L_BOOST_MIX                0x2B /* R43 */
#define WM8960_INPUT_L_BOOST_LIN2BOOST(x)       (x<<1)
#define WM8960_INPUT_L_BOOST_LIN2MUTE           WM8960_INPUT_L_BOOST_LIN2BOOST(0)
#define WM8960_INPUT_L_BOOST_LIN3BOOST(x)       (x<<4)
#define WM8960_INPUT_L_BOOST_LIN3MUTE           WM8960_INPUT_L_BOOST_LIN3BOOST(0)
/* Right Input Boost Mixer */
#define WM8960_INPUT_R_BOOST_MIX                0x2C /* R44 */
#define WM8960_INPUT_R_BOOST_RIN2BOOST(x)       (x<<1)
#define WM8960_INPUT_R_BOOST_RIN2MUTE           WM8960_INPUT_R_BOOST_RIN2BOOST(0)
#define WM8960_INPUT_R_BOOST_RIN3BOOST(x)       (x<<4)
#define WM8960_INPUT_R_BOOST_RIN3MUTE           WM8960_INPUT_R_BOOST_RIN3BOOST(0)
/* Left Bypass */
#define WM8960_BYPASS_L                         0x2D /* R45 */
#define WM8960_BYPASS_0DB                       0x0
#define WM8960_BYPASS_LB2LO                     (1<<7) /* Analog Loopback */
/* Right Bypass */
#define WM8960_BYPASS_R                         0x2E /* R46 */
#define WM8960_BYPASS_RB2RO                     (1<<7) /* Analog Loopback */
/* Power Management Control Register 4 */
#define WM8960_PWR_MGMT_3                       0x2F /* R47 */
#define WM8960_PWR_MGMT_3_LMIC                  (1 << 5)
#define WM8960_PWR_MGMT_3_RMIC                  (1 << 4)
#define WM8960_PWR_MGMT_3_LOMIX                 (1 << 3)
#define WM8960_PWR_MGMT_3_ROMIX                 (1 << 2)
/* Additional Control Register 4 */
#define WM8960_ADDITIONAL_CTRL_4                0x30 /* R48 */
#define WM8960_ADDITIONAL_CTRL_4_GPIOSEL_MASK   (0x7 << 4)
#define WM8960_ADDITIONAL_CTRL_4_GPIOSEL_JDI    (0x0 << 4)  /* Jack detect input            */
#define WM8960_ADDITIONAL_CTRL_4_GPIOSEL_JDO    (0x3 << 4)  /* Debounced Jack detect output */
#define WM8960_ADDITIONAL_CTRL_4_HPSEL_MASK     (0x3 << 2)
#define WM8960_ADDITIONAL_CTRL_4_HPSEL_GPIO1    (0x0 << 2)
#define WM8960_ADDITIONAL_CTRL_4_HPSEL_JD2      (0x2 << 2)
#define WM8960_ADDITIONAL_CTRL_4_HPSEL_JD3      (0x3 << 2)
#define WM8960_ADDITIONAL_CTRL_4_TSENSEN_MASK   (1 << 1)
#define WM8960_ADDITIONAL_CTRL_4_MBSEL          (1<<0)   /* 0.65 * AVDD */
/* Class D Amp. Control Register */
#define WM8960_CLASSD_CTRL_1                    0x31 /* R49 */
#define WM8960_CLASSD_CTRL_1_SPKEN_MASK         (0x3 << 6)
#define WM8960_CLASSD_CTRL_1_DEFAULT            0x37
/* Class D Amp. Control Register */
#define WM8960_CLASSD_CTRL_2                    0x33 /* R51 */
#define WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET      3
#define WM8960_CLASSD_CTRL_2_DCGAIN_MASK        (0x7 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET      0
#define WM8960_CLASSD_CTRL_2_ACGAIN_MASK        (0x7 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DEFAULT            0x80
#define WM8960_CLASSD_CTRL_2_DCGAIN_1           (0 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DCGAIN_1P27        (1 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DCGAIN_1P40        (2 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DCGAIN_1P52        (3 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DCGAIN_1P67        (4 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_DCGAIN_1P80        (5 << WM8960_CLASSD_CTRL_2_DCGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1           (0 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1P27        (1 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1P40        (2 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1P52        (3 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1P67        (4 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
#define WM8960_CLASSD_CTRL_2_ACGAIN_1P80        (5 << WM8960_CLASSD_CTRL_2_ACGAIN_OFFSET)
/* Internal PLL Control Register 1 */
#define WM8960_PLL_1                            0x34 /* R52 */
#define WM8960_PLL_1_PLLN_MASK                  (0xF)
#define WM8960_PLL_1_PLLN_DEFAULT               (8)
/* Internal PLL Control Register 2 */
#define WM8960_PLL_2                            0x35 /* R53 */
#define WM8960_PLL_2_PLLK_H_MASK                0xFF
#define WM8960_PLL_2_PLLK_H_DEFAULT             0X31
/* Internal PLL Control Register 3 */
#define WM8960_PLL_3                            0x36 /* R54 */
#define WM8960_PLL_3_PLLK_L_MASK                0xFF
#define WM8960_PLL_3_PLLK_L_DEFAULT             0X26
/* Internal PLL Control Register 4 */
#define WM8960_PLL_4                            0x37 /* R55 */
#define WM8960_PLL_4_PLLK_LL_MASK               0xFF
#define WM8960_PLL_4_PLLK_LL_DEFAULT            0XE9

#define WM8960_REG_COUNT                        56   /* R0 - R55 */
/** WM8960 Clock Mode. */
typedef enum wm8960_format {
    WM8960_I2S,                         /**< WM8960 format I2S */
    WM8960_PCM,                         /**< WM8960 format PCM. */
} wm8960_format_t;

typedef enum wm8960_mux {
	INPUT_MUX_MIC,
	INPUT_MUX_LINE2,
	INPUT_MUX_LINE3,
} wm8960_mux_t;

typedef enum wm8960_hpsel {
    JD_NC = 0,
    JD_GPIO1,
    JD_2,
    JD_3,
} wm8960_hpsel_t;

/** WM8960 mixer context structure */
typedef struct wm8960_context {
    ado_mixer_t          *mixer;
    char i2c_num;                       /**< I2C bus number */
    int32_t i2c_addr;                   /**< I2C device address */
    int i2c_fd;                         /**< I2C device handle */
    int  adr0cs;                        /* wm8960 slave address select pin */
    uint8_t clk_mode;                   /**< Whether codec is master */
    uint32_t mclk;                      /**< External MCLK frequency */
    uint32_t sysclk;                    /**< Internal SYSCLK frequency */
    uint32_t sample_rate;               /**< Sample rate */
    uint8_t sample_size;                /**< Sample size */
    uint8_t use_dac_lrck;               /**< Set to 1 when ADC uses DAC LRCK pin */
    bool    micbias;                    /**< Enable micbias on driver init */
    bool    mic_left2right;             /**< Route analog left to ADC right */
    uint16_t dac_mute;                  /**< DAC mute state */
    uint16_t adc_mute;                  /**< ADC mute state */
    uint16_t hp_mute;                   /**< Headphone mute state */
    uint16_t spk_mute;                  /**< Speaker mute state */
    uint8_t spk_volume[2];              /**< Speaker volume */
    uint8_t hp_volume[2];               /**< Headphone volume */
    uint8_t dac_volume[2];              /**< DAC volume */
    uint8_t adc_volume[2];              /**< ADC volume */
    uint8_t input_volume[2];            /**< Analog input Volume */
    uint16_t input_mute;                /**< Input mute state */
    uint8_t mic_bias;                   /**< MIC bias enabled/disabled */
    uint8_t output_enable;              /**< Enable/disable output path bit mask: bit 0: speaker, bit 1: headphone */
    uint16_t regs[WM8960_REG_COUNT];    /**< Stores WM8960 register states */
    wm8960_format_t format;             /**< Format I2S , PCM */
    wm8960_hpsel_t hpsel;               /** Enable and configure auto headphone selection (jack detect) function */
    uint8_t input_enable;
#define MIC_ENABLED         (1<<0)
#define LINE2_ENABLED       (1<<1)
#define LINE3_ENABLED       (1<<2)
#define INPUT_MASK          (MIC_ENABLED | LINE2_ENABLED | LINE3_ENABLED)
#define DIFFERENTIAL_LINE2  (1<<3)    /** Use the Line2 pin as our positive mic input */
#define DIFFERENTIAL_LINE3  (1<<4)    /** Use the Line3 pin as our positive mic input */
    int8_t  line2_boost;
    int8_t  line3_boost;

    /* For input mux */
    wm8960_mux_t         input_mux;
    ado_mixer_delement_t *mic;
    ado_mixer_delement_t *line2;
    ado_mixer_delement_t *line3;
    ado_mixer_dll_codec_params_t   params;
} wm8960_context_t;

/** WM8960 Write Option. */
typedef enum wm8960_reg_mode {
    WM8960_UPDATE_AND_WRITE_REG,        /** Update local register buffer and write codec register */
    WM8960_UPDATE_ONLY,                 /** Update local register buffer only */
} wm8960_reg_mode_t;

/** WM8960 Clock Mode. */
typedef enum wm8960_clk_mode {
    WM8960_MASTER,                      /**< WM8960 device is master. */
    WM8960_SLAVE,                       /**< WM8960 device is slave. */
} wm8960_clk_mode_t;

int wm8960_set_rate(MIXER_CONTEXT_T * wm8960, uint32_t sample_rate);
int wm8960_mixer(ado_card_t * card, ado_mixer_t **mixer, char * mixeropts, ado_pcm_t *pcm, uint32_t sample_size);
void wm8960_set_default_group(ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index);

#endif /* WM8960_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/mixer/wm8960/wm8960.h $ $Rev: 934572 $")
#endif
