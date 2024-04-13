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

#ifndef __CM_H__
#define __CM_H__

#include <stdint.h> 

/* Clock Manager Clock CTL Register (CM_GP0CTL, CM_GP1CTL, CM_GP2CTL, CM_PCMCTL, CM_PWMCTL, etc) */
#define CM_CTL_PASSWD(n)    ((n)<<24)   /* Clock Manager password "5a" */
#define CM_CTL_PASSWD_MSK   CM_CTL_PASSWD(255)
#define CM_CTL_MASH(n)      ((n)<<9)    /* MASH control */
#define CM_CTL_MASH_MSK     CM_CTL_MASH(3)
    /* 0 = integer division
     * 1 = 1-stage MASH (equivalent to non-MASH dividers)
     * 2 = 2-stage MASH
     * 3 = 3-stage MASH
     * To avoid lock-ups and glitches do not change this control while
     * BUSY=1 and do not change this control at the same time as asserting ENAB
     */
#define CM_CTL_FLIP         (1<<8)      /* Invert the clock generator output */
    /* This is intended for use in test/debug only. Switching this control
     * will generate an edge on the clock generator output. To avoid
     * output glitches do not switch this control while BUSY=1
     */
#define CM_CTL_BUSY         (1<<7)      /* Clock generator is running (read-only) */
    /* Indicates the clock generator is running.  To avoid glitches and
     * lock-ups, clock sources and setups must not be changed while this
     * flag is set
     */
#define CM_CTL_RSVD6        (1<<6)      /* Reserved */
#define CM_CTL_KILL         (1<<5)      /* Kill the clock generator */
    /* 0 = no action
     * 1 = stop and reset the clock generator
     * This is intended for test/debug only. Using this control may cause
     * a glitch on the clock generator output
     */
#define CM_CTL_ENAB         (1<<4)      /* Enable the clock generator */
    /* This requests the clock to start or stop without glitches. The output
     * clock will not stop immediately because the cycle must be allowed
     * to complete to avoid glitches. The BUSY flag will go low when
     * the final cycle is completed
     */
#define CM_CTL_SRC(n)       ((n)<<0)    /* Clock source */
#define CM_CTL_SRC_MSK      CM_CTL_SRC(15)
#define CM_CTL_SRC_GND      CM_CTL_SRC(0)  /* 0 = GND */
#define CM_CTL_SRC_OSC      CM_CTL_SRC(1)  /* 1 = oscillator */
#define CM_CTL_SRC_TESTDBG0 CM_CTL_SRC(2)  /* 2 = testdebug0 */
#define CM_CTL_SRC_TESTDBG1 CM_CTL_SRC(3)  /* 3 = testdebug1 */
#define CM_CTL_SRC_PLLA_PER CM_CTL_SRC(4)  /* 4 = PLLA per */
#define CM_CTL_SRC_PLLC_PER CM_CTL_SRC(5)  /* 5 = PLLC per */
#define CM_CTL_SRC_PLLD_PER CM_CTL_SRC(6)  /* 6 = PLLD per */
#define CM_CTL_SRC_HDMI_AUX CM_CTL_SRC(7)  /* 7 = HDMI auxiliary */
    /* To avoid lock-ups and glitches do not change this control while
     * BUSY=1 and do not change this control at the same time as asserting ENAB
     */

/* Clock Manager Clock DIV register (CM_GP0DIV, CM_GP1DIV, CM_GP2DIV, CM_PCMDIV, CM_PWMDIV, etc) */
#define CM_DIV_PASSWD(n)    ((n)<<24)   /* Clock Manager password "5a" */
#define CM_DIV_PASSWD_MSK   CM_DIV_PASSWD(255)
#define CM_DIV_INT(n)       ((n)<<12)   /* Integer part of divisor */
#define CM_DIV_INT_MSK      CM_DIV_INT(4095)
    /* This value has a minimum limit determined by the MASH setting
     * See text for details. To avoid lock-ups and glitches do not change
     * this control while BUSY=1
     */
#define CM_DIV_FRAC(n)      ((n)<<0)    /* Fractional part of divisor */
#define CM_DIV_FRAC_MSK     CM_DIV_FRAC(4095)
    /* To avoid lock-ups and glitches do not change this control while BUSY=1 */

/* max value for CM mash */
#define CM_MASH_MAX         3

/* Clock Manager Clock Register Map - the Clock Manager register map consists of a number of instances of the
 * register map for individual clocks (GP0, GP1, GP2, PCM, PWM, etc), located contiguously in memory; the base
 * address (BCM2711_CM_BASE) and size (BCM2711_CM_SIZE) of the Clock Manager register map is defined in
 * bcm2711.h, as are the offsets for each clock instance (BCM2711_CM_GP0, BCM2711_CM_GP1, BCM2711_CM_GP2,
 * BCM2711_CM_PCM, BCM2711_CM_PWM)
 */
typedef struct cm_clk_reg {
    uint32_t ctl;                  /* Clock Manager Clock CTL Register */
    uint32_t div;                  /* Clock Manager Clock DIV register */
} cm_clk_reg_t;

/* Configure a specific clock unit (e.g. BCM2711_CM_GP0, BCM2711_CM_GP1, BCM2711_CM_GP2,
 * BCM2711_CM_PCM, BCM2711_CM_PWM) for frequency freq, starting from source clock src of
 * frequency clk, using a specified mash value
 */
void cm_config(unsigned unit, unsigned freq, unsigned clk, unsigned src, unsigned mash);

#endif /* __CM_H__ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pwm/cm.h $ $Rev: 924476 $")
#endif
