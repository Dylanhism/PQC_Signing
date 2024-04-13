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

#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>


#define MBOX_TAG_GET_FIRMWAREREV        0x00000001
#define MBOX_TAG_GET_FIRMWAREVAR        0x00000002
#define MBOX_TAG_GET_FIRMWAREHASH       0x00000003
#define MBOX_TAG_GET_BOARDMODEL         0x00010001
#define MBOX_TAG_GET_BOARDREVISION      0x00010002
#define MBOX_TAG_GET_MACADDRESS         0x00010003
#define MBOX_TAG_GET_BOARDSERIAL        0x00010004
#define MBOX_TAG_GET_ARMMEMORY          0x00010005
#define MBOX_TAG_GET_VCMEMORY           0x00010006
#define MBOX_TAG_GET_CLOCKS             0x00010007

#define MBOX_TAG_GET_POWERSTATE         0x00020001
#define MBOX_TAG_GET_POWERTIMING        0x00020002

#define MBOX_TAG_GET_CLOCKSTATE         0x00030001
#define MBOX_TAG_GET_CLOCKRATE          0x00030002
#define MBOX_TAG_GET_VOLTAGE            0x00030003
#define MBOX_TAG_GET_MAX_CLOCKRATE      0x00030004
#define MBOX_TAG_GET_MAX_VOLTAGE        0x00030005
#define MBOX_TAG_GET_TEMPERATURE        0x00030006
#define MBOX_TAG_GET_MIN_CLOCKRATE      0x00030007
#define MBOX_TAG_GET_MIN_VOLTAGE        0x00030008
#define MBOX_TAG_GET_TURBO              0x00030009
#define MBOX_TAG_GET_MAX_TEMPERATURE    0x0003000a
#define MBOX_TAG_GET_DOMAIN_STATE       0x00030030
#define MBOX_TAG_GET_GPIO_STATE         0x00030041
#define MBOX_TAG_GET_GPIO_CONFIG        0x00030043
#define MBOX_TAG_GET_THROTTLED          0x00030046
#define MBOX_TAG_GET_CLOCK_MEASURED     0x00030047
#define MBOX_TAG_NOTIFY_REBOOT          0x00030048

#define MBOX_TAG_GET_CMDLINE            0x00050001
#define MBOX_TAG_NOTIFY_XHCI_RESET      0x00030058

#define MBOX_TAG_GET_DMACHAN            0x00060001

static int mbox_msg(unsigned tag, void *buf, unsigned bufsize) {
    struct {
        uint32_t hdrsize, hdrcode;
#define MBOX_PROCESS_REQUEST 0
        uint32_t tag, tagsize, tagcode;
#define MBOX_TAG_REQUEST     (0 << 31)
#define MBOX_TAG_RESPONSE    (1 << 31)
#define	MBOX_TAG_NULL        0
        uint8_t buf[bufsize];
        uint32_t tagend;
    } *msg;
    static void *out;
    static uint32_t physaddr;
    static volatile struct {
        uint32_t rdwr, _x04,   _x08,   _x0c;
        uint32_t poll, send, status, config;
#define MBOX_STATUS_FULL    0x80000000
#define MBOX_STATUS_EMPTY   0x40000000
#define MBOX_SEND_CHANNEL   8
#define MBOX_SEND_CHANNEL_MASK	0xf
    } *mbox; // [0]:fromvc [1]:tovc

    if (!out) {
        int fd;
        off_t offset;
        fd = posix_typed_mem_open("/sysram&below1G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
        if (fd < 0) {
            printf("Failed to open typed memory(%s) \n", strerror( errno ));
            exit(EXIT_FAILURE);
        }
        out = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, fd, 0);
        if (out == MAP_FAILED) {
            printf("Failed to allocate buffer!\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (mem_offset(out, NOFD, 1, &offset, 0) != EOK) {
            munmap(out, 0x1000);
            close(fd);
            printf("Failed to obtain physical address\n");
            exit(EXIT_FAILURE);
        }
        physaddr = (uint32_t)offset;
    }

    msg = out;
    msg->hdrsize = sizeof *msg;
    msg->hdrcode = MBOX_PROCESS_REQUEST;
    msg->tag = tag;
    msg->tagsize = bufsize;
    msg->tagcode = MBOX_TAG_REQUEST;
    memcpy(msg->buf, buf, bufsize);
    msg->tagend = MBOX_TAG_NULL;

#define MBOX_REGS           0xfe00b880
    int wait_cnt = 0;
    mbox = mbox ? : mmap_device_memory(0, 0x80, PROT_NOCACHE|PROT_READ|PROT_WRITE, 0, MBOX_REGS);
    if (mbox == MAP_FAILED) {
        printf("Failed to map mbox registers\n");
        exit (EXIT_FAILURE);
    }

    // check for space on tovc channel to send with timeout
    for (;;) {
        if (!(mbox[1].status & MBOX_STATUS_FULL)) {
            break;
        }
        usleep(100);
        if (++wait_cnt > 5000) {
            printf("Wait for response timeout\n");
            return -1;
        }
    }

    // send
    mbox[1].rdwr = (physaddr & ~MBOX_SEND_CHANNEL_MASK) | MBOX_SEND_CHANNEL;

    for (;;) {
        wait_cnt = 0;
        // wait for response on fromvc channel to receive with timeout
        for (;;) {
            if (!(mbox[0].status & MBOX_STATUS_EMPTY)) {
                break;
            }
            usleep(100);
            if (++wait_cnt > 5000) {
                printf("Wait for response timeout\n");
                return -2;
            }
        }

        // is it the channel we're waiting for?
        uint32_t rd = mbox[0].rdwr;
        if ((rd & MBOX_SEND_CHANNEL_MASK) == MBOX_SEND_CHANNEL) {
            uint32_t tagcode = msg->tagcode;
            unsigned len = tagcode & ~MBOX_TAG_RESPONSE;

            if (tag != MBOX_TAG_GET_FIRMWAREHASH) {
                if (len > bufsize) {
                    // passed in bufsize is not big enough to hold return response
                    len = bufsize;
                }
                memcpy(buf, msg->buf, len);
                return len;
            }
            else {
                /* firmwware hash should return 20 bytes but return code indicates 8 only */
                memcpy(buf, msg->buf, bufsize);
                return bufsize;
            }
        }
    }
    /*NOTREACHED*/
}

static void fmt_d(void *buf, unsigned len) {
    for (unsigned i = 0; i < len / sizeof (int32_t); i++)
        printf("%d ", ((int32_t *) buf)[i]);
    printf("\n");
}

static void fmt_s(void *buf, unsigned len) {
    printf("%.*s\n", len, (char *) buf);
}

static void fmt_u(void *buf, unsigned len) {
    for (unsigned i = 0; i < len / sizeof (uint32_t); i++)
        printf("%u ", ((uint32_t *) buf)[i]);
    printf("\n");
}

static void fmt_x(void *buf, unsigned len) {
    /* mac address returns 6 byte, need to round up to get last two octets */
    len += (sizeof (uint32_t) - 1);
    for (unsigned i = 0; i < len / sizeof (uint32_t); i++)
        printf("%08x ", ((uint32_t *) buf)[i]);
    printf("\n");
}

static void fmt_x_str(void *buf, unsigned len) {
    for (unsigned i = 0; i < len / sizeof (uint32_t); i++)
        printf("%08x", ((uint32_t *) buf)[i]);
    printf("\n");
}

int main(int argc, char **argv) {
    static const struct {
        char *str;
        void (*fmt)(void *, unsigned);
        unsigned tag;
        unsigned len;
    } t[] = {
        { "clockrate",      fmt_u, MBOX_TAG_GET_CLOCKRATE,        2 },
        { "clocks",         fmt_d, MBOX_TAG_GET_CLOCKS,          80 },
        { "clockstate",     fmt_d, MBOX_TAG_GET_CLOCKSTATE,       2 },
        { "cmdline",        fmt_s, MBOX_TAG_GET_CMDLINE,        256 },
        { "dmachan",        fmt_x, MBOX_TAG_GET_DMACHAN,          2 },
        { "firmwarerev",    fmt_x, MBOX_TAG_GET_FIRMWAREREV,      2 },
        { "firmwarevar",    fmt_x, MBOX_TAG_GET_FIRMWAREVAR,      1 },
        { "firmwarehash",   fmt_x_str, MBOX_TAG_GET_FIRMWAREHASH,     5 },
        { "gpioconfig",     fmt_x, MBOX_TAG_GET_GPIO_CONFIG,      2 },
        { "gpiostate",      fmt_d, MBOX_TAG_GET_GPIO_STATE,       2 },
        { "macaddress",     fmt_x, MBOX_TAG_GET_MACADDRESS,       2 },
        { "maxclockrate",   fmt_u, MBOX_TAG_GET_MAX_CLOCKRATE,    2 },
        { "maxtemperature", fmt_d, MBOX_TAG_GET_MAX_TEMPERATURE,  2 },
        { "maxvoltage",     fmt_d, MBOX_TAG_GET_MAX_VOLTAGE,      2 },
        { "memory",         fmt_x, MBOX_TAG_GET_ARMMEMORY,        2 },
        { "minclockrate",   fmt_u, MBOX_TAG_GET_MIN_CLOCKRATE,    2 },
        { "minvoltage",     fmt_d, MBOX_TAG_GET_MIN_VOLTAGE,      2 },
        { "model",          fmt_d, MBOX_TAG_GET_BOARDMODEL,       2 },
        { "notifyxhcireset",fmt_x, MBOX_TAG_NOTIFY_XHCI_RESET,    2 },
        { "powerstate",     fmt_d, MBOX_TAG_GET_POWERSTATE,       2 },
        { "powertiming",    fmt_d, MBOX_TAG_GET_POWERTIMING,      2 },
        { "revision",       fmt_x, MBOX_TAG_GET_BOARDREVISION,    2 },
        { "serial",         fmt_x, MBOX_TAG_GET_BOARDSERIAL,      2 },
        { "temperature",    fmt_d, MBOX_TAG_GET_TEMPERATURE,      2 },
        { "turbo",          fmt_x, MBOX_TAG_GET_TURBO,            2 },
        { "vcmemory",       fmt_x, MBOX_TAG_GET_VCMEMORY,         2 },
        { "voltage",        fmt_d, MBOX_TAG_GET_VOLTAGE,          2 },
    };

    while (*++argv) {
        unsigned set = 0;
        char *opt, *str = *argv;
        uint32_t i, n = 0, w[256+1] = { };
        unsigned len = strcspn(str, ":=");

        switch (str[len]) {
            case '=':
                set = 1;
                /*FALLTHRU*/
            case ':':
                opt = str + len;
                *opt++ = '\0';
                while (n < sizeof w / sizeof w[0]) {
                    w[n++] = strtol(opt, &opt, 0);
                    if (*opt++ != ':') break;
                }
                break;
        }
        for (i = 0; i < sizeof t / sizeof t[0]; i++) {
            if (strcmp(str, t[i].str) == 0) {
                uint32_t tag = t[i].tag;
                uint32_t len = t[i].len;
                if (n > len) len = n;
                if (set && (tag != MBOX_TAG_NOTIFY_XHCI_RESET)) tag |= 0x8000;
                if ((len = mbox_msg(tag, w, len * sizeof w[0])) > 0)
                    t[i].fmt(w, len);
                break;
            } else {
                uint32_t tag = strtol(str, &opt, 0);
                uint32_t len = 2;
                if (tag != 0 || errno != EINVAL) {
                    if (n > len) len = n;
                    if (set) tag |= 0x8000;
                    if ((len = mbox_msg(tag, w, len * sizeof w[0])) > 0)
                        fmt_x(w, len);
                    break;
                }
            }
        }
        if (i >= sizeof t / sizeof t[0]) {
            printf("%s not found?\n", str);
            return (EXIT_FAILURE);
        }
    }
    return 0;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/support/bcm2711/mbox/mbox.c $ $Rev: 931169 $")
#endif
