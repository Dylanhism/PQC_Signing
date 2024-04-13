/*
 * $QNXLicenseC:
 * Copyright 2018-2019 NXP
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

#ifndef IMX_MU_H_
#define IMX_MU_H_

/*
 * MU Registers, offset from base address
 */
#define IMX_MU_xTR0                    0x00
#define IMX_MU_xTR1                    0x04
#define IMX_MU_xTR2                    0x08
#define IMX_MU_xTR3                    0x0C
#define IMX_MU_xRR0                    0x10
#define IMX_MU_xRR1                    0x14
#define IMX_MU_xRR2                    0x18
#define IMX_MU_xRR3                    0x1C
#define IMX_MU_xSR                     0x20
#define IMX_MU_xCR                     0x24
/* Define also side A and side B for backward compatibility */
#define IMX_MU_ATR0                    IMX_MU_xTR0
#define IMX_MU_ATR1                    IMX_MU_xTR1
#define IMX_MU_ATR2                    IMX_MU_xTR2
#define IMX_MU_ATR3                    IMX_MU_xTR3
#define IMX_MU_ARR0                    IMX_MU_xRR0
#define IMX_MU_ARR1                    IMX_MU_xRR1
#define IMX_MU_ARR2                    IMX_MU_xRR2
#define IMX_MU_ARR3                    IMX_MU_xRR3
#define IMX_MU_ASR                     IMX_MU_xSR
#define IMX_MU_ACR                     IMX_MU_xCR
#define IMX_MU_BTR0                    IMX_MU_xTR0
#define IMX_MU_BTR1                    IMX_MU_xTR1
#define IMX_MU_BTR2                    IMX_MU_xTR2
#define IMX_MU_BTR3                    IMX_MU_xTR3
#define IMX_MU_BRR0                    IMX_MU_xRR0
#define IMX_MU_BRR1                    IMX_MU_xRR1
#define IMX_MU_BRR2                    IMX_MU_xRR2
#define IMX_MU_BRR3                    IMX_MU_xRR3
#define IMX_MU_BSR                     IMX_MU_xSR
#define IMX_MU_BCR                     IMX_MU_xCR


/* ASR/BSR bit fields for both A, B sides */
#define IMX_MU_xSR_GIP_SHIFT           28
#define IMX_MU_xSR_GIP_MASK            (0x0F << IMX_MU_xSR_GIP_SHIFT)
#define IMX_MU_xSR_RF_SHIFT            24
#define IMX_MU_xSR_RF_MASK             (0x0F << IMX_MU_xSR_RF_SHIFT)
#define IMX_MU_xSR_TE_SHIFT            20
#define IMX_MU_xSR_TE_MASK             (0x0F << IMX_MU_xSR_TE_SHIFT)
#define IMX_MU_xSR_FUP_SHIFT           8
#define IMX_MU_xSR_FUP_MASK            (0x01 << IMX_MU_xSR_FUP_SHIFT)
#define IMX_MU_xSR_xRS_SHIFT           7
#define IMX_MU_xSR_xRS_MASK            (0x01 << IMX_MU_xSR_xRS_SHIFT)
#define IMX_MU_xSR_EP_SHIFT            4
#define IMX_MU_xSR_EP_MASK             (0x01 << IMX_MU_xSR_EP_SHIFT)
#define IMX_MU_xSR_F_SHIFT             0
#define IMX_MU_xSR_F_MASK              (0x07 << IMX_MU_xSR_F_SHIFT)
/* This bit field is MU A-side specific */
#define IMX_MU_ASR_BRDIP_SHIFT         9
#define IMX_MU_ASR_BRDIP_MASK          (0x01 << IMX_MU_ASR_BRDIP_SHIFT)
/* This bit field is MU B-side specific */
#define IMX_MU_BSR_APM_SHIFT           5
#define IMX_MU_BSR_APM_MASK            (0x03 << IMX_MU_BSR_APM_SHIFT)

/* ACR/BCR bit fields for both A, B sides */
#define IMX_MU_xCR_GIE_SHIFT           28
#define IMX_MU_xCR_GIE_MASK            (0x0F << IMX_MU_xCR_GIE_SHIFT)
#define IMX_MU_xCR_RIE_SHIFT           24
#define IMX_MU_xCR_RIE_MASK            (0x0F << IMX_MU_xCR_RIE_SHIFT)
#define IMX_MU_xCR_TIE_SHIFT           20
#define IMX_MU_xCR_TIE_MASK            (0x0F << IMX_MU_xCR_TIE_SHIFT)
#define IMX_MU_xCR_GIR_SHIFT           16
#define IMX_MU_xCR_GIR_MASK            (0x0F << IMX_MU_xCR_GIR_SHIFT)
#define IMX_MU_xCR_F_SHIFT             0
#define IMX_MU_xCR_F_MASK              (1 << IMX_MU_xCR_F_SHIFT)
/* These bit fields are MU A-side specific */
#define IMX_MU_ACR_BRDIE_SHIFT         6
#define IMX_MU_ACR_BRDIE_MASK          (1 << IMX_MU_ACR_BRDIE_SHIFT)
#define IMX_MU_ACR_MUR_SHIFT           5
#define IMX_MU_ACR_MUR_MASK            (1 << IMX_MU_ACR_MUR_SHIFT)
#define IMX_MU_ACR_BHR_SHIFT           4
#define IMX_MU_ACR_BHR_MASK            (1 << IMX_MU_ACR_BHR_SHIFT)
/* This bit field is MU B-side specific */
#define IMX_MU_BCR_HRM_SHIFT           4
#define IMX_MU_BCR_HRM_MASK            (1 << IMX_MU_BCR_HRM_SHIFT)

/* Define ASR bit fields for backward compatibility */
#define IMX_MU_ASR_GIP_SHIFT           IMX_MU_xSR_GIP_SHIFT
#define IMX_MU_ASR_GIP_MASK            IMX_MU_xSR_GIP_MASK
#define IMX_MU_ASR_RF_SHIFT            IMX_MU_xSR_RF_SHIFT
#define IMX_MU_ASR_RF_MASK             IMX_MU_xSR_RF_MASK
#define IMX_MU_ASR_TE_SHIFT            IMX_MU_xSR_TE_SHIFT
#define IMX_MU_ASR_TE_MASK             IMX_MU_xSR_TE_MASK
#define IMX_MU_ASR_FUP_SHIFT           IMX_MU_xSR_FUP_SHIFT
#define IMX_MU_ASR_FUP_MASK            IMX_MU_xSR_FUP_MASK
#define IMX_MU_ASR_BRS_SHIFT           IMX_MU_xSR_xRS_SHIFT
#define IMX_MU_ASR_BRS_MASK            IMX_MU_xSR_xRS_MASK
#define IMX_MU_ASR_EP_SHIFT            IMX_MU_xSR_EP_SHIFT
#define IMX_MU_ASR_EP_MASK             IMX_MU_xSR_EP_MASK
#define IMX_MU_ASR_F_SHIFT             IMX_MU_xSR_F_SHIFT
#define IMX_MU_ASR_F_MASK              IMX_MU_xSR_F_MASK

/* Define ACR bit fields for backward compatibility */
#define IMX_MU_ACR_GIE_SHIFT           IMX_MU_xCR_GIE_SHIFT
#define IMX_MU_ACR_GIE_MASK            IMX_MU_xCR_GIE_MASK
#define IMX_MU_ACR_RIE_SHIFT           IMX_MU_xCR_RIE_SHIFT
#define IMX_MU_ACR_RIE_MASK            IMX_MU_xCR_RIE_MASK
#define IMX_MU_ACR_TIE_SHIFT           IMX_MU_xCR_TIE_SHIFT
#define IMX_MU_ACR_TIE_MASK            IMX_MU_xCR_TIE_MASK
#define IMX_MU_ACR_GIR_SHIFT           IMX_MU_xCR_GIR_SHIFT
#define IMX_MU_ACR_GIR_MASK            IMX_MU_xCR_GIR_MASK
#define IMX_MU_ACR_F_SHIFT             IMX_MU_xCR_F_SHIFT
#define IMX_MU_aCR_F_MASK              IMX_MU_xCR_F_MASK

#endif /* IMX_MU_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/imx8_common/imx_mu.h $ $Rev: 881905 $")
#endif
