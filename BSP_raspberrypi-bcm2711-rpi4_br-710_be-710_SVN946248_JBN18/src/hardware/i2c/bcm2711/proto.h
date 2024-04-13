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

#ifndef __PROTO_H_INCLUDED
#define __PROTO_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <hw/i2c.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <aarch64/bcm2711.h>

//#define GPIO_CONFIG
#define BCM2711_I2C_IDLE    0
#define BCM2711_I2C_TX      1
#define BCM2711_I2C_RX      2

typedef struct _bcm2711_dev {

    uint8_t             *buf;
    unsigned            buflen;
    unsigned            reglen;
    uintptr_t           regbase;
    unsigned            physbase;
    int                 intr;
    int                 iid;
    struct sigevent     intrevent;
    unsigned            txrx;
    unsigned            status;
    unsigned            cur_len;
    unsigned            tot_len;
    unsigned            stop;
    unsigned            slave_addr;
    unsigned            clk;
    i2c_addrfmt_t       slave_addr_fmt;
    unsigned            restart;
    unsigned            verbose;
    unsigned            speed;
#ifdef GPIO_CONFIG
    uint8_t             gpio;
#endif /* GPIO_CONFIG */
} bcm2711_dev_t;

#define BCM2711_DEFAULT_VERBOSE        _SLOG_INFO
#define INVALID_ADDR                   0x80

#define BCM2711_I2C_CTRREG_OFF          0x00
#define CTRREG_I2CEN                    (1 << 15)
#define CTRREG_INTR                     (1 << 10)
#define CTRREG_INTT                     (1 << 9)
#define CTRREG_INTD                     (1 << 8)
#define CTRREG_START                    (1 << 7)
#define CTRREG_CLEAR                    (3 << 4)
#define CTRREG_READ                     (1 << 0)

#define BCM2711_I2C_STATREG_OFF         0x04
#define STATREG_MASK                    0x000001FF
#define STATREG_CLKT                    (1 << 9)
#define STATREG_ERR                     (1 << 8)
#define STATREG_RXF                     (1 << 7)
#define STATREG_TXE                     (1 << 6)
#define STATREG_RXD                     (1 << 5)
#define STATREG_TXD                     (1 << 4)
#define STATREG_RXR                     (1 << 3)
#define STATREG_TXW                     (1 << 2)
#define STATREG_DONE                    (1 << 1)
#define STATREG_TA                      (1 << 0)

#define BCM2711_I2C_DLENREG_OFF         0x08
#define DLENREG_BIT_MSK                 0x0000FFFF

#define BCM2711_I2C_SLAVEADDRREG_OFF    0x0C
#define SLAVEADDRREG_BIT_MSK            0x0000007F

#define BCM2711_I2C_FIFOREG_OFF         0x10
#define FIFOREG_BIT_MSK                 0x000000FF

#define BCM2711_I2C_CLKDIVREG_OFF       0x14
#define CLKDIVREG_BIT_MSK               0x0000FFFF

#define BCM2711_I2C_DELAYREG_OFF        0x18
#define DELAYREG_WRITE_SHIFT_BITS       16
#define DELAYREG_WRITE_BIT_MSK          0xFFFF0000
#define DELAYREG_READ_SHIFT_BITS        0
#define DELAYREG_READ_BIT_MSK           0x0000FFFF

#define BCM2711_I2C_CLKTREG_OFF         0x1C
#define CLKTREG_BIT_MSK                 0x0000FFFF

#define BCM2711_I2C_REGLEN              0x200

#define bcm2711_i2c_slogf(dev, level, fmt, ...)                                \
    do {                                                                    \
        if ((level <= _SLOG_ERROR) || (dev->verbose & level)) {      \
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), level, fmt, ##__VA_ARGS__);    \
        }                                                                       \
    } while (0)

void *bcm2711_init(int argc, char *argv[]);
void bcm2711_fini(void *hdl);
int bcm2711_options(bcm2711_dev_t *dev, int argc, char *argv[]);

int bcm2711_set_slave_addr(void *hdl, unsigned int addr, i2c_addrfmt_t fmt);
int bcm2711_set_bus_speed(void *hdl, unsigned int speed, unsigned int *ospeed);
int bcm2711_version_info(i2c_libversion_t *version);
int bcm2711_driver_info(void *hdl, i2c_driver_info_t *info);
i2c_status_t bcm2711_recv(void *hdl, void *buf, unsigned int len, unsigned int stop);
i2c_status_t bcm2711_send(void *hdl, void *buf, unsigned int len, unsigned int stop);
i2c_status_t bcm2711_wait_complete(bcm2711_dev_t *dev);

int bcm2711_attach_intr(bcm2711_dev_t *dev);
void i2c_reset(bcm2711_dev_t *dev);
void push_data(bcm2711_dev_t *dev);
void read_data(bcm2711_dev_t *dev);
void disable_interrupt(bcm2711_dev_t *dev);

#endif  /*  __PROTO_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/i2c/bcm2711/proto.h $ $Rev: 919272 $")
#endif
