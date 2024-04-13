/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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

#ifndef __AM437X_PINMUX_H
#define __AM437X_PINMUX_H

#define WUEVT          (0x1 << 30) /* 0: No event; 1: Event occurred */
#define WUEN           (0x1 << 29) /* 0: wakeup disable; 1: enable */
#define DSPULL_EN      (0x1 << 28) /* DS0 mode 0:pulldown selected; 1: pullup selected*/
#define DSPPULLUDEN    (0x1 << 27) /* DS0 mode 0: Pullup/down disabled; 1: Pullup/down enabled */
#define DSOUTVAL       (0x1 << 26) /* DS0 mode output value 0: 0  1: 1 */
#define DSOUTEN        (0x0 << 25) /* DS0 mode output 0: Enabled; 1: disabled */
#define DSEN           (0x1 << 24) /* DS0 mode override control 0: retain  1: OFF */
#define SLEWCTRL       (0x1 << 19) /* 0: fast; 1: Slow */
#define RXACTIVE       (0x1 << 18) /* 0: Receive disabled; 1: Receive enabled */
#define PULLUP_EN      (0x1 << 17) /* 0: Pulldown selectd; 1: PullUP Selected */
#define PULLUDEN       (0x0 << 16) /* 0: Pullup/down enabled */
#define PULLUDDIS      (0x1 << 16) /* 1: Pullup/down disabled */
#define MODE(val)      val



#define conf_gpmc_ad0                 (AM437X_CTRL_BASE+0x0800)
#define conf_gpmc_ad1                 (AM437X_CTRL_BASE+0x0804)
#define conf_gpmc_ad2                 (AM437X_CTRL_BASE+0x0808)
#define conf_gpmc_ad3                 (AM437X_CTRL_BASE+0x080C)
#define conf_gpmc_ad4                 (AM437X_CTRL_BASE+0x0810)
#define conf_gpmc_ad5                 (AM437X_CTRL_BASE+0x0814)
#define conf_gpmc_ad6                 (AM437X_CTRL_BASE+0x0818)
#define conf_gpmc_ad7                 (AM437X_CTRL_BASE+0x081C)
#define conf_gpmc_ad8                 (AM437X_CTRL_BASE+0x0820)
#define conf_gpmc_ad9                 (AM437X_CTRL_BASE+0x0824)
#define conf_gpmc_ad10                (AM437X_CTRL_BASE+0x0828)
#define conf_gpmc_ad11                (AM437X_CTRL_BASE+0x082C)
#define conf_gpmc_ad12                (AM437X_CTRL_BASE+0x0830)
#define conf_gpmc_ad13                (AM437X_CTRL_BASE+0x0834)
#define conf_gpmc_ad14                (AM437X_CTRL_BASE+0x0838)
#define conf_gpmc_ad15                (AM437X_CTRL_BASE+0x083C)
#define conf_gpmc_a0                  (AM437X_CTRL_BASE+0x0840)
#define conf_gpmc_a1                  (AM437X_CTRL_BASE+0x0844)
#define conf_gpmc_a2                  (AM437X_CTRL_BASE+0x0848)
#define conf_gpmc_a3                  (AM437X_CTRL_BASE+0x084C)
#define conf_gpmc_a4                  (AM437X_CTRL_BASE+0x0850)
#define conf_gpmc_a5                  (AM437X_CTRL_BASE+0x0854)
#define conf_gpmc_a6                  (AM437X_CTRL_BASE+0x0858)
#define conf_gpmc_a7                  (AM437X_CTRL_BASE+0x085C)
#define conf_gpmc_a8                  (AM437X_CTRL_BASE+0x0860)
#define conf_gpmc_a9                  (AM437X_CTRL_BASE+0x0864)
#define conf_gpmc_a10                 (AM437X_CTRL_BASE+0x0868)
#define conf_gpmc_a11                 (AM437X_CTRL_BASE+0x086C)
#define conf_gpmc_wait0               (AM437X_CTRL_BASE+0x0870)
#define conf_gpmc_wpn                 (AM437X_CTRL_BASE+0x0874)
#define conf_gpmc_be1n                (AM437X_CTRL_BASE+0x0878)
#define conf_gpmc_csn0                (AM437X_CTRL_BASE+0x087C)
#define conf_gpmc_csn1                (AM437X_CTRL_BASE+0x0880)
#define conf_gpmc_csn2                (AM437X_CTRL_BASE+0x0884)
#define conf_gpmc_csn3                (AM437X_CTRL_BASE+0x0888)
#define conf_gpmc_clk                 (AM437X_CTRL_BASE+0x088C)
#define conf_gpmc_advn_ale            (AM437X_CTRL_BASE+0x0890)
#define conf_gpmc_oen_ren             (AM437X_CTRL_BASE+0x0894)
#define conf_gpmc_wen                 (AM437X_CTRL_BASE+0x0898)
#define conf_gpmc_be0n_cle            (AM437X_CTRL_BASE+0x089C)
#define conf_lcd_data0                (AM437X_CTRL_BASE+0x08A0)
#define conf_lcd_data1                (AM437X_CTRL_BASE+0x08A4)
#define conf_lcd_data2                (AM437X_CTRL_BASE+0x08A8)
#define conf_lcd_data3                (AM437X_CTRL_BASE+0x08AC)
#define conf_lcd_data4                (AM437X_CTRL_BASE+0x08B0)
#define conf_lcd_data5                (AM437X_CTRL_BASE+0x08B4)
#define conf_lcd_data6                (AM437X_CTRL_BASE+0x08B8)
#define conf_lcd_data7                (AM437X_CTRL_BASE+0x08BC)
#define conf_lcd_data8                (AM437X_CTRL_BASE+0x08C0)
#define conf_lcd_data9                (AM437X_CTRL_BASE+0x08C4)
#define conf_lcd_data10               (AM437X_CTRL_BASE+0x08C8)
#define conf_lcd_data11               (AM437X_CTRL_BASE+0x08CC)
#define conf_lcd_data12               (AM437X_CTRL_BASE+0x08D0)
#define conf_lcd_data13               (AM437X_CTRL_BASE+0x08D4)
#define conf_lcd_data14               (AM437X_CTRL_BASE+0x08D8)
#define conf_lcd_data15               (AM437X_CTRL_BASE+0x08DC)
#define conf_lcd_vsync                (AM437X_CTRL_BASE+0x08E0)
#define conf_lcd_hsync                (AM437X_CTRL_BASE+0x08E4)
#define conf_lcd_pclk                 (AM437X_CTRL_BASE+0x08E8)
#define conf_lcd_ac_bias_en           (AM437X_CTRL_BASE+0x08EC)
#define conf_mmc0_dat3                (AM437X_CTRL_BASE+0x08F0)
#define conf_mmc0_dat2                (AM437X_CTRL_BASE+0x08F4)
#define conf_mmc0_dat1                (AM437X_CTRL_BASE+0x08F8)
#define conf_mmc0_dat0                (AM437X_CTRL_BASE+0x08FC)
#define conf_mmc0_clk                 (AM437X_CTRL_BASE+0x0900)
#define conf_mmc0_cmd                 (AM437X_CTRL_BASE+0x0904)
#define conf_mii1_col                 (AM437X_CTRL_BASE+0x0908)
#define conf_mii1_crs                 (AM437X_CTRL_BASE+0x090C)
#define conf_mii1_rxerr               (AM437X_CTRL_BASE+0x0910)
#define conf_mii1_txen                (AM437X_CTRL_BASE+0x0914)
#define conf_mii1_rxdv                (AM437X_CTRL_BASE+0x0918)
#define conf_mii1_txd3                (AM437X_CTRL_BASE+0x091C)
#define conf_mii1_txd2                (AM437X_CTRL_BASE+0x0920)
#define conf_mii1_txd1                (AM437X_CTRL_BASE+0x0924)
#define conf_mii1_txd0                (AM437X_CTRL_BASE+0x0928)
#define conf_mii1_txclk               (AM437X_CTRL_BASE+0x092C)
#define conf_mii1_rxclk               (AM437X_CTRL_BASE+0x0930)
#define conf_mii1_rxd3                (AM437X_CTRL_BASE+0x0934)
#define conf_mii1_rxd2                (AM437X_CTRL_BASE+0x0938)
#define conf_mii1_rxd1                (AM437X_CTRL_BASE+0x093C)
#define conf_mii1_rxd0                (AM437X_CTRL_BASE+0x0940)
#define conf_rmii1_refclk             (AM437X_CTRL_BASE+0x0944)
#define conf_mdio_data                (AM437X_CTRL_BASE+0x0948)
#define conf_mdio_clk                 (AM437X_CTRL_BASE+0x094C)
#define conf_spi0_sclk                (AM437X_CTRL_BASE+0x0950)
#define conf_spi0_d0                  (AM437X_CTRL_BASE+0x0954)
#define conf_spi0_d1                  (AM437X_CTRL_BASE+0x0958)
#define conf_spi0_cs0                 (AM437X_CTRL_BASE+0x095C)
#define conf_spi0_cs1                 (AM437X_CTRL_BASE+0x0960)
#define conf_ecap0_in_pwm0_out        (AM437X_CTRL_BASE+0x0964)
#define conf_uart0_ctsn               (AM437X_CTRL_BASE+0x0968)
#define conf_uart0_rtsn               (AM437X_CTRL_BASE+0x096C)
#define conf_uart0_rxd                (AM437X_CTRL_BASE+0x0970)
#define conf_uart0_txd                (AM437X_CTRL_BASE+0x0974)
#define conf_uart1_ctsn               (AM437X_CTRL_BASE+0x0978)
#define conf_uart1_rtsn               (AM437X_CTRL_BASE+0x097C)
#define conf_uart1_rxd                (AM437X_CTRL_BASE+0x0980)
#define conf_uart1_txd                (AM437X_CTRL_BASE+0x0984)
#define conf_i2c0_sda                 (AM437X_CTRL_BASE+0x0988)
#define conf_i2c0_scl                 (AM437X_CTRL_BASE+0x098C)
#define conf_mcasp0_aclkx             (AM437X_CTRL_BASE+0x0990)
#define conf_mcasp0_fsx               (AM437X_CTRL_BASE+0x0994)
#define conf_mcasp0_axr0              (AM437X_CTRL_BASE+0x0998)
#define conf_mcasp0_ahclkr            (AM437X_CTRL_BASE+0x099C)
#define conf_mcasp0_aclkr             (AM437X_CTRL_BASE+0x09A0)
#define conf_mcasp0_fsr               (AM437X_CTRL_BASE+0x09A4)
#define conf_mcasp0_axr1              (AM437X_CTRL_BASE+0x09A8)
#define conf_mcasp0_ahclkx            (AM437X_CTRL_BASE+0x09AC)
#define conf_cam0_hd                  (AM437X_CTRL_BASE+0x09B0)
#define conf_cam0_vd                  (AM437X_CTRL_BASE+0x09B4)
#define conf_cam0_field               (AM437X_CTRL_BASE+0x09B8)
#define conf_cam0_wen                 (AM437X_CTRL_BASE+0x09BC)
#define conf_cam0_pclk                (AM437X_CTRL_BASE+0x09C0)
#define conf_cam0_data8               (AM437X_CTRL_BASE+0x09C4)
#define conf_cam0_data9               (AM437X_CTRL_BASE+0x09C8)
#define conf_cam1_data9               (AM437X_CTRL_BASE+0x09CC)
#define conf_cam1_data8               (AM437X_CTRL_BASE+0x09D0)
#define conf_cam1_hd                  (AM437X_CTRL_BASE+0x09D4)
#define conf_cam1_vd                  (AM437X_CTRL_BASE+0x09D8)
#define conf_cam1_pclk                (AM437X_CTRL_BASE+0x09DC)
#define conf_cam1_field               (AM437X_CTRL_BASE+0x09E0)
#define conf_cam1_wen                 (AM437X_CTRL_BASE+0x09E4)
#define conf_cam1_data0               (AM437X_CTRL_BASE+0x09E8)
#define conf_cam1_data1               (AM437X_CTRL_BASE+0x09EC)
#define conf_cam1_data2               (AM437X_CTRL_BASE+0x09F0)
#define conf_cam1_data3               (AM437X_CTRL_BASE+0x09F4)
#define conf_cam1_data4               (AM437X_CTRL_BASE+0x09F8)
#define conf_cam1_data5               (AM437X_CTRL_BASE+0x09FC)
#define conf_cam1_data6               (AM437X_CTRL_BASE+0x0A00)
#define conf_cam1_data7               (AM437X_CTRL_BASE+0x0A04)
#define conf_cam0_data0               (AM437X_CTRL_BASE+0x0A08)
#define conf_cam0_data1               (AM437X_CTRL_BASE+0x0A0C)
#define conf_cam0_data2               (AM437X_CTRL_BASE+0x0A10)
#define conf_cam0_data3               (AM437X_CTRL_BASE+0x0A14)
#define conf_cam0_data4               (AM437X_CTRL_BASE+0x0A18)
#define conf_cam0_data5               (AM437X_CTRL_BASE+0x0A1C)
#define conf_cam0_data6               (AM437X_CTRL_BASE+0x0A20)
#define conf_cam0_data7               (AM437X_CTRL_BASE+0x0A24)

#define conf_uart3_rxd                (AM437X_CTRL_BASE+0x0A28)
#define conf_uart3_txd                (AM437X_CTRL_BASE+0x0A2C)
#define conf_uart3_ctsn               (AM437X_CTRL_BASE+0x0A30)
#define conf_uart3_rtsn               (AM437X_CTRL_BASE+0x0A34)
#define conf_gpio5_8                  (AM437X_CTRL_BASE+0x0A38)
#define conf_gpio5_9                  (AM437X_CTRL_BASE+0x0A3C)
#define conf_gpio5_10                 (AM437X_CTRL_BASE+0x0A40)
#define conf_gpio5_11                 (AM437X_CTRL_BASE+0x0A44)
#define conf_gpio5_12                 (AM437X_CTRL_BASE+0x0A48)
#define conf_gpio5_13                 (AM437X_CTRL_BASE+0x0A4C)
#define conf_spi4_sclk                (AM437X_CTRL_BASE+0x0A50)
#define conf_spi4_d0                  (AM437X_CTRL_BASE+0x0A54)
#define conf_spi4_d1                  (AM437X_CTRL_BASE+0x0A58)
#define conf_spi4_cs0                 (AM437X_CTRL_BASE+0x0A5C)
#define conf_spi2_sclk                (AM437X_CTRL_BASE+0x0A60)
#define conf_spi2_d0                  (AM437X_CTRL_BASE+0x0A64)
#define conf_spi2_d1                  (AM437X_CTRL_BASE+0x0A68)
#define conf_spi2_cs0                 (AM437X_CTRL_BASE+0x0A6C)
#define conf_xdma_evt_intr0           (AM437X_CTRL_BASE+0x0A70)
#define conf_xdma_evt_intr1           (AM437X_CTRL_BASE+0x0A74)
#define conf_clkreq                   (AM437X_CTRL_BASE+0x0A78)
#define conf_nresetin_out             (AM437X_CTRL_BASE+0x0A7C)
#define conf_nnmi                     (AM437X_CTRL_BASE+0x0A84)
#define conf_tms                      (AM437X_CTRL_BASE+0x0A90)
#define conf_tdi                      (AM437X_CTRL_BASE+0x0A94)
#define conf_tdo                      (AM437X_CTRL_BASE+0x0A98)
#define conf_tck                      (AM437X_CTRL_BASE+0x0A9C)
#define conf_ntrst                    (AM437X_CTRL_BASE+0x0AA0)
#define conf_emu0                     (AM437X_CTRL_BASE+0x0AA4)
#define conf_emu1                     (AM437X_CTRL_BASE+0x0AA8)
#define conf_osc1_in                  (AM437X_CTRL_BASE+0x0AAC)
#define conf_osc1_out                 (AM437X_CTRL_BASE+0x0AB0)
#define conf_rtc_porz                 (AM437X_CTRL_BASE+0x0AB4)
#define conf_ext_wakeup0              (AM437X_CTRL_BASE+0x0AB8)
#define conf_pmic_power_en0           (AM437X_CTRL_BASE+0x0ABC)
#define conf_usb0_drvvbus             (AM437X_CTRL_BASE+0x0AC0)
#define conf_usb1_drvvbus             (AM437X_CTRL_BASE+0x0AC4)
#define cqdetect_status               (AM437X_CTRL_BASE+0x0E00)
#define ddr_io_ctrl                   (AM437X_CTRL_BASE+0x0E04)
#define vtp_ctrl                      (AM437X_CTRL_BASE+0x0E0C)
#define vref_ctrl                     (AM437X_CTRL_BASE+0x0E14)
#define tpcc_evt_mux_0_3              (AM437X_CTRL_BASE+0x0F90)
#define tpcc_evt_mux_4_7              (AM437X_CTRL_BASE+0x0F94)
#define tpcc_evt_mux_8_11             (AM437X_CTRL_BASE+0x0F98)
#define tpcc_evt_mux_12_15            (AM437X_CTRL_BASE+0x0F9C)
#define tpcc_evt_mux_16_19            (AM437X_CTRL_BASE+0x0FA0)
#define tpcc_evt_mux_20_23            (AM437X_CTRL_BASE+0x0FA4)
#define tpcc_evt_mux_24_27            (AM437X_CTRL_BASE+0x0FA8)
#define tpcc_evt_mux_28_31            (AM437X_CTRL_BASE+0x0FAC)
#define tpcc_evt_mux_32_35            (AM437X_CTRL_BASE+0x0FB0)
#define tpcc_evt_mux_36_39            (AM437X_CTRL_BASE+0x0FB4)
#define tpcc_evt_mux_40_43            (AM437X_CTRL_BASE+0x0FB8)
#define tpcc_evt_mux_44_47            (AM437X_CTRL_BASE+0x0FBC)
#define tpcc_evt_mux_48_51            (AM437X_CTRL_BASE+0x0FC0)
#define tpcc_evt_mux_52_55            (AM437X_CTRL_BASE+0x0FC4)
#define tpcc_evt_mux_56_59            (AM437X_CTRL_BASE+0x0FC8)
#define tpcc_evt_mux_60_63            (AM437X_CTRL_BASE+0x0FCC)
#define timer_evt_capt                (AM437X_CTRL_BASE+0x0FD0)
#define ecap_evt_capt                 (AM437X_CTRL_BASE+0x0FD4)
#define adc_evt_capt                  (AM437X_CTRL_BASE+0x0FD8)
#define reset_iso                     (AM437X_CTRL_BASE+0x1000)
#define ddr_cke_ctrl                  (AM437X_CTRL_BASE+0x131C)
#define vsldo                         (AM437X_CTRL_BASE+0x1320)
#define wakeproc_txev_e               (AM437X_CTRL_BASE+0x1324)
#define ipc_msg_reg0                  (AM437X_CTRL_BASE+0x1328)
#define ipc_msg_reg1                  (AM437X_CTRL_BASE+0x132C)
#define ipc_msg_reg2                  (AM437X_CTRL_BASE+0x1330)
#define ipc_msg_reg3                  (AM437X_CTRL_BASE+0x1334)
#define ipc_msg_reg4                  (AM437X_CTRL_BASE+0x1338)
#define ipc_msg_reg5                  (AM437X_CTRL_BASE+0x133C)
#define ipc_msg_reg6                  (AM437X_CTRL_BASE+0x1340)
#define ipc_msg_reg7                  (AM437X_CTRL_BASE+0x1344)
#define ipc_msg_reg8                  (AM437X_CTRL_BASE+0x1348)
#define ipc_msg_reg9                  (AM437X_CTRL_BASE+0x134C)
#define ipc_msg_reg10                 (AM437X_CTRL_BASE+0x1350)
#define ipc_msg_reg11                 (AM437X_CTRL_BASE+0x1354)
#define ipc_msg_reg12                 (AM437X_CTRL_BASE+0x1358)
#define ipc_msg_reg13                 (AM437X_CTRL_BASE+0x135C)
#define ipc_msg_reg14                 (AM437X_CTRL_BASE+0x1360)
#define ddr_addrctrl_ioctrl           (AM437X_CTRL_BASE+0x1404)
#define ddr_addrctrl_wd0_ioctrl       (AM437X_CTRL_BASE+0x1408)
#define ddr_addrctrl_wd1_ioctr        (AM437X_CTRL_BASE+0x140C)
#define ddr_data0_ioctrl              (AM437X_CTRL_BASE+0x1440)
#define ddr_data1_ioctrl              (AM437X_CTRL_BASE+0x1444)
#define ddr_data2_ioctrl              (AM437X_CTRL_BASE+0x1448)
#define ddr_data3_ioctrl              (AM437X_CTRL_BASE+0x144C)

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/arm/am437x_pinmux.h $ $Rev: 800110 $")
#endif
