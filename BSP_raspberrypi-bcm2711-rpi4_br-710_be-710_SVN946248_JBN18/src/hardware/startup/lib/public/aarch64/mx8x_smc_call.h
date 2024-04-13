/*
 * $QNXLicenseC:
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

#ifndef IMX8_SMC_CALL_H_
#define IMX8_SMC_CALL_H_

#include "aarch64/psci.h"

typedef uint64_t imx_smc_status_t;

static inline imx_smc_status_t imx_sec_firmware_psci(
    uint64_t regX0, uint64_t regX1, uint64_t regX2, uint64_t regX3, uint64_t regX4)
{
    __asm__ __volatile__(
        "ldr x0, %0\n"
        "ldr x1, %1\n"
        "ldr x2, %2\n"
        "ldr x3, %3\n"
        "ldr x4, %4\n"
        "smc    #0\n"
        "str x0, %0\n"
        : "+m"(regX0), "+m"(regX1),
        "+m"(regX2), "+m"(regX3), "+m"(regX4)

        : "m"(regX0)
        : "x0", "x1", "x2", "x3", "x4"
    );
    return regX0;
}

#define IMX_FSL_SIP_GPC                     0xC2000000
#define IMX_FSL_SIP_CONFIG_GPC_MASK         0x00
#define IMX_FSL_SIP_CONFIG_GPC_UNMASK       0x01
#define IMX_FSL_SIP_CONFIG_GPC_SET_WAKE     0x02
#define IMX_FSL_SIP_CONFIG_GPC_PM_DOMAIN    0x03
#define IMX_FSL_SIP_CONFIG_GPC_SET_AFF      0x04

#define IMX_FSL_SIP_CPUFREQ                 0xC2000001
#define IMX_FSL_SIP_SET_CPUFREQ             0x00

#define IMX_FSL_SIP_SRTC                    0xC2000002
#define IMX_FSL_SIP_SRTC_SET_TIME           0x00
#define IMX_FSL_SIP_SRTC_START_WDOG         0x01
#define IMX_FSL_SIP_SRTC_STOP_WDOG          0x02
#define IMX_FSL_SIP_SRTC_SET_WDOG_ACT       0x03
#define IMX_FSL_SIP_SRTC_PING_WDOG          0x04
#define IMX_FSL_SIP_SRTC_SET_TIMEOUT_WDOG   0x05
#define IMX_FSL_SIP_SRTC_GET_WDOG_STAT      0x06
#define IMX_FSL_SIP_SRTC_SET_PRETIME_WDOG   0x07

#define IMX_FSL_SIP_SRTC_YEAR_SHIFT         16
#define IMX_FSL_SIP_SRTC_YEAR_MASK          0xFFFF0000
#define IMX_FSL_SIP_SRTC_MON_SHIFT          0
#define IMX_FSL_SIP_SRTC_MON_MASK           0x000000FF
#define IMX_FSL_SIP_SRTC_DAY_SHIFT          16
#define IMX_FSL_SIP_SRTC_DAY_MASK           0x00FF0000
#define IMX_FSL_SIP_SRTC_HOUR_SHIFT         0
#define IMX_FSL_SIP_SRTC_HOUR_MASK          0x000000FF
#define IMX_FSL_SIP_SRTC_MIN_SHIFT          16
#define IMX_FSL_SIP_SRTC_MIN_MASK           0x00FF0000
#define IMX_FSL_SIP_SRTC_SEC_SHIFT          0
#define IMX_FSL_SIP_SRTC_SEC_MASK           0x000000FF

#define IMX_FSL_SIP_BUILDINFO               0xC2000003
#define IMX_FSL_SIP_BUILDINFO_GET_COMMITHASH 0x00

#define IMX_FSL_SIP_DDR_DVFS                0xC2000004

#define IMX_FSL_SIP_SRC                     0xC2000005
#define IMX_FSL_SIP_SRC_M4_START            0x00
#define IMX_FSL_SIP_SRC_M4_STARTED          0x01

#define IMX_FSL_SIP_GET_SOC_INFO            0xC2000006

#define IMX_FSL_SIP_HAB                     0xC2000007
#define IMX_FSL_SIP_HAB_AUTHENTICATE        0x00
#define IMX_FSL_SIP_HAB_ENTRY               0x01
#define IMX_FSL_SIP_HAB_EXIT                0x02
#define IMX_FSL_SIP_HAB_REPORT_EVENT        0x03
#define IMX_FSL_SIP_HAB_REPORT_STATUS       0x04

#define IMX_FSL_SIP_NOC                     0xC2000008
#define IMX_FSL_SIP_NOC_LCDIF               0x00

#define IMX_FSL_SIP_MISC_SET_TEMP           0xC200000C

/* PSCI runtime services function */
#define IMX_PSCI_VERSION                    0x84000000
#define IMX_PSCI_CPU_SUSPEND_AARCH32        0x84000001
#define IMX_PSCI_CPU_SUSPEND_AARCH64        0xC4000001
#define IMX_PSCI_CPU_OFF                    0x84000002
#define IMX_PSCI_CPU_ON_AARCH32             0x84000003
#define IMX_PSCI_CPU_ON_AARCH64             0xC4000003
#define IMX_PSCI_AFFINITY_INFO_AARCH32      0x84000004
#define IMX_PSCI_AFFINITY_INFO_AARCH64      0xC4000004
#define IMX_PSCI_MIG_AARCH32                0x84000005
#define IMX_PSCI_MIG_AARCH64                0xC4000005
#define IMX_PSCI_MIG_INFO_TYPE              0x84000006
#define IMX_PSCI_MIG_INFO_UP_CPU_AARCH32    0x84000007
#define IMX_PSCI_MIG_INFO_UP_CPU_AARCH64    0xC4000007
#define IMX_PSCI_SYSTEM_OFF                 0x84000008
#define IMX_PSCI_SYSTEM_RESET               0x84000009
#define IMX_PSCI_FEATURES                   0x8400000A
#define IMX_PSCI_NODE_HW_STATE_AARCH32      0x8400000D
#define IMX_PSCI_NODE_HW_STATE_AARCH64      0xC400000D
#define IMX_PSCI_SYSTEM_SUSPEND_AARCH32     0x8400000E
#define IMX_PSCI_SYSTEM_SUSPEND_AARCH64     0xC400000E
#define IMX_PSCI_STAT_RESIDENCY_AARCH32     0x84000010
#define IMX_PSCI_STAT_RESIDENCY_AARCH64     0xC4000010
#define IMX_PSCI_STAT_COUNT_AARCH32         0x84000011
#define IMX_PSCI_STAT_COUNT_AARCH64         0xC4000011
#define IMX_PSCI_SYSTEM_RESET2_AARCH32      0x84000012
#define IMX_PSCI_SYSTEM_RESET2_AARCH64      0xC4000012
#define IMX_PSCI_MEM_PROTECT                0x84000013
#define IMX_PSCI_MEM_CHK_RANGE_AARCH32      0x84000014
#define IMX_PSCI_MEM_CHK_RANGE_AARCH64      0xC4000014

/* PSCI version 1.1 */
#define IMX_PSCI_MAJOR_VER                  (0x01 << 16)
#define IMX_PSCI_MINOR_VER                  0x01

/* PSCI error codes */
#define IMX_PSCI_SUCCESS                    0
#define IMX_PSCI_NOT_SUPPORTED              -1
#define IMX_PSCI_INVALID_PARAMS             -2
#define IMX_PSCI_DENIED                     -3
#define IMX_PSCI_ALREADY_ON                 -4
#define IMX_PSCI_ON_PENDING                 -5
#define IMX_PSCI_INTERN_FAIL                -6
#define IMX_PSCI_NOT_PRESENT                -7
#define IMX_PSCI_DISABLED                   -8
#define IMX_PSCI_INVALID_ADDRESS            -9

/* SYSTEM_RESET2 macros */
#define IMX_PSCI_RESET2_TYPE_VENDOR_SHIFT   31
#define IMX_PSCI_RESET2_TYPE_VENDOR         (0x01 << PSCI_RESET2_TYPE_VENDOR_SHIFT)
#define IMX_PSCI_RESET2_TYPE_ARCH           (0x00 << PSCI_RESET2_TYPE_VENDOR_SHIFT)
#define IMX_PSCI_RESET2_SYSTEM_WARM_RESET   (PSCI_RESET2_TYPE_ARCH | 0x00)
#define IMX_PSCI_RESET2_SYSTEM_COLD_RESET   (PSCI_RESET2_TYPE_VENDOR | 0x01)
#define IMX_PSCI_RESET2_SYSTEM_BOARD_RESET  (PSCI_RESET2_TYPE_VENDOR | 0x02)

#endif    /* IMX8_SMC_CALL_H_ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/mx8x_smc_call.h $ $Rev: 904597 $")
#endif
