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

#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdint.h>

int32_t build_codec_mixer(ado_card_t *card, HW_CONTEXT_T *hwc);
void codec_set_rate(HW_CONTEXT_T *hwc, uint32_t sample_rate);
void codec_supported_rates(HW_CONTEXT_T* hwc, uint32_t* rates);
void codec_supported_formats(HW_CONTEXT_T* hwc, uint32_t* formats);
void codec_set_default_group(HW_CONTEXT_T* hwc, ado_pcm_t *pcm, int channel, int index);
void codec_on(HW_CONTEXT_T* hwc, int channel);
void codec_off(HW_CONTEXT_T* hwc, int channel);

#endif /* __MIXER_H__ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/mixer.h $ $Rev: 925119 $")
#endif
