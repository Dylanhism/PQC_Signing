/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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

#ifndef _BSTA1000_H
#define _BSTA1000_H

#define CONFIG_A55

/* -------------------------------------------------------------------------
 * Devices
 * -------------------------------------------------------------------------
 */
#define A1000_HWI_UART              "uart"
#define A1000_HWI_PSS_REF_CLK       "pss_ref_clk"

/* -------------------------------------
 * Clock && PLL
 * -------------------------------------
 */
#define A1000_PSS_REF_CLK_FREQ      25000000UL
#define A1000_PERIPH_CLOCK          25000000UL

/* -------------------------------------------------------------------------
 * SDRAM configuration
 * -------------------------------------------------------------------------
 */
#define A1000_SDRAM_BANK1_BASE      0x80000000
#define A1000_SDRAM_BANK2_BASE      0x180000000
#define A1000_SDRAM_BANK1_SIZE      MEG(2 * 1024)
#define A1000_SDRAM_BANK2_SIZE      MEG(2 * 1024)

/* -------------------------------------------------------------------------
 * System Domain Address Allocation
 * -------------------------------------------------------------------------
 */

/*
 * -------------------------------------------------------------------------
 ************ Flash  ************
 * -------------------------------------------------------------------------
 */
#define IRAM_BASE_ADDR      0x0

#define FLASE_GRP_BASE      0x10000000

#ifdef CONFIG_R5_SECURE
#define QSPI0_BASE_ADDR     0x10000000
#else
#define QSPI0_BASE_ADDR     0x00000000
#endif
#define QSPI1_BASE_ADDR     0x14000000

#define SRAM_BASE_ADDR      0x18000000

/*
 * -------------------------------------------------------------------------
 ************ LOW SPEED IO configure  ************
 * -------------------------------------------------------------------------
 */
#define LIO_GRP_BASE    (0x20000000)
#define LIO_IP_OFFSET   (0x1000)

#define I2C0_BASE_ADDR  (0x20000000)
#define I2C1_BASE_ADDR  (0x20001000)
#define I2C2_BASE_ADDR  (0x20002000)
#define I2C3_BASE_ADDR  (0x20003000)
#define I2C4_BASE_ADDR  (0x20004000)
#define I2C5_BASE_ADDR  (0x20005000)
#define I2C_IP_CNT      6

#define UART0_BASE_ADDR (0x20008000)
#define UART1_BASE_ADDR (0x2000A000)
#define UART2_BASE_ADDR (0x2000B000)
#define UART3_BASE_ADDR (0x20009000)
#define UART_SIZE       0x1000
/* -------------------------------------------------------------------------
 * UART ports registers
 * -------------------------------------------------------------------------
 */
#define UART_SRC_CLK          (25000000UL)

#define UART_REG_DLL          0x0
#define UART_REG_THR          0x0        /* UART Transmit Holding Register */
#define UART_REG_RBR          0x0
#define UART_REG_DLH          0x4
#define UART_REG_IER          0x4
#define UART_REG_IER_ETBEI    0x1
#define UART_REG_FCR          0x8
#define UART_REG_IIR          0x8
#define UART_REG_LCR          0xC
#define UART_REG_MCR          0x10
#define UART_REG_LSR          0x14       /* UART Line Status Register */
    #define UART_LSR_THRE        (1<<5)  /* Transmit Holding Register Empty */
    #define UART_LSR_RXDREADY    (1<<0)

#define UART_REG_MSR          0x18

#define UART_REG_DLF          0xC0
    #define DLF_SIZE             (6)


#define SPI0_BASE_ADDR  (0x2000C000)
#define SPI1_BASE_ADDR  (0x2000D000)
#define I2S0_BASE_ADDR  (0x2000E000)
#define I2S1_BASE_ADDR  (0x2000F000)

#define GPIO0_BASE_ADDR (0x20010000)
#define GPIO1_BASE_ADDR (0x20011000)
#define GPIO_ADDR_SIZE  (0x1000)
    #define GPIO_INTEN          (0x0030)
    #define GPIO_INTMASK        (0x0034)
    #define GPIO_INTTYPE_LEVEL  (0x0038)
    #define GPIO_INT_POLARITY   (0x003c)
    #define GPIO_INTSTATUS      (0x0040)
    #define GPIO_RAW_INTSTATUS  (0x0044)
    #define GPIO_PORTA_DEBOUNCE (0x0048)
    #define GPIO_PORTA_EOI      (0x004c)
#define GPIO0_IO_NUMS   (128)
#define GPIO1_IO_NUMS   (22)


#define TIMER0_PWM0_BASE_ADDR (0x20012000)
#define TIMER1_PWM1_BASE_ADDR (0x20013000)
#define TIMER2_PWM2_BASE_ADDR (0x20014000)
#define TIMER3_PWM3_BASE_ADDR (0x20015000)
#define TIMER_PWM_IP_CNT 4

#define CANFD0_BASE_ADDR  (0x20016000)
#define CANFD1_BASE_ADDR  (0x20017000)

#define WDT0_BASE_ADDR    (0x2001A000)
#define WDT1_BASE_ADDR    (0x2001B000)
#define WDT2_BASE_ADDR    (0x2001C000)
#define WDT3_BASE_ADDR    (0x2001D000)

#define LSP0_CRM_ADDR     (0x20020000)
#define LSP1_CRM_ADDR     (0x20021000)
#define QSPI0_XIP_ENABLE_REG (LSP0_CRM_ADDR + 0x10)
#define SAFETY_RELEASE_BIT   (28)
#define A55_RELEASE_BIT      (30)

/*
 * -------------------------------------------------------------------------
 ************ HIGH SPEED IO configure  ************
 * -------------------------------------------------------------------------
 */
#define GMAC0_BASE_ADDR    (0x30000000)
#define GMAC1_BASE_ADDR    (0x30100000)
#define GMAC_SIZE          0x100000

#define USB3_BASE_ADDR     (0x30200000)
#define USB2_BASE_ADDR     (0x30300000)

/* -------------------------------------------------------------------------
 * USB PHY
 * -------------------------------------------------------------------------
 */
#define A1000_USB3PHY_BASE      (0x30e00000)
#define A1000_USB3PHY_SIZE      (0x1000)

#define USB3PHY_REG_1           0x14
    #define FSEL_POS            0
    #define FSEL_MSK            (0x3f<<FSEL_POS)
    #define MPLLMULTIPLIER_POS  6
    #define MPLLMULTIPLIER_MSK  (0x7f<<MPLLMULTIPLIER_POS)
    #define SSCREFCLKSEL_POS    23
    #define SSCREFCLKSEL_MSK    (0x1ff<<SSCREFCLKSEL_POS)

#define USB3PHY_REG_2           0x18
    #define SSCEN               (1<<0)
    #define REFUSEPAD           (1<<5)
    #define REFSSPEN            (1<<6)

#define USB3PHY_REG_4           0x20
    #define REFCLKDIV2          (1<<2)
    #define REFCLKSEL_POS       0
    #define REFCLKSEL_MSK       (0x3<<REFCLKSEL_POS)

#define USB3PHY_LOCAL_RST       0x68
    #define PHYSWRSTN           (1<<0)
    #define CTRLSWRSTN          (1<<1)

#define A1000_USB2PHY_BASE      (0x30e01000)
#define A1000_USB2PHY_SIZE      (0x1000)

#define USB2PHY_PWR_CTRL        0x10
    #define COMMONONN           (1<<0)
    #define PHYRESET            (1<<1)

#define SD_EMMC0_BASE_ADDR (0x30400000)
#define SD_EMMC1_BASE_ADDR (0x30500000)

#define PCIE_4X_BASE_ADDR  (0x30600000)
#define PCIE_2X_BASE_ADDR  (0x30A00000)
    #define GEN2_CTRL_OFF       (0x80C)
        #define NUM_OF_LANES_MASK    (0x1F)
        #define NUM_OF_LANES_SHIFT   (8)

#define PCIE_APB_BASE_ADDR (0x30E02000)
#define PCIE_PHY_BASE_ADDR (0x30E02000)

/* -------------------------------------------------------------------------
 * PCIe APB/PHY registers
 * -------------------------------------------------------------------------
 */
#define PCIE_PHY_CTRL              (0x04)
    #define X2_APP_HOLD_PHY_RST       (1<<2)
    #define X4_APP_HOLD_PHY_RST       (1<<1)

#define PCIE_MODE                  (0x08)
    #define X4_IS_RC                  (1<<1)
    #define BIFURCATION_EN            (1<<0)
    #define X4_IS_RC_SHIFT            (1)
    #define BIFURCATION_EN_SHIFT      (0)

#define PCIE_CLK_CTRL              (0x0C)
    #define PHY1_REF_USE_PAD          (1<<8)
    #define PHY0_REF_USE_PAD          (1<<7)
    #define PHY0_REF_REPEAT_CLK_EN    (1<<6)

#define PCIE_LANE_FLIP_EN          (0x14)
    #define X2_RX_LANE_FLIP_SET       (1 << 3)
    #define X2_TX_LANE_FLIP_SET       (1 << 2)
    #define X4_RX_LANE_FLIP_SET       (1 << 1)
    #define X4_TX_LANE_FLIP_SET       (1 << 0)

#define PCIE_DEBUG_PIN_OUT         (0x1C)
#define PCIE_PHY_SRAM_CTRL         (0x20)

#define PCIE_PHY_SRAM_INIT_STATUS  (0x24)
    #define PHY1_SRAM_INIT_DONE    (1<<1)
    #define PHY0_SRAM_INIT_DONE    (1<<0)

#define PCIE_LOCAL_RST             (0x11C)
    #define X4_CTRL_RST_N             (1<<0)   /* X4 controller reset. Active low. */
    #define X2_CTRL_RST_N             (1<<1)   /* X2 controller reset. Active low. */
    #define PHY_RST_N                 (1<<2)   /* PHY reset. Active low. */

#define PCIE_X4_INT_MASK           (0x120)
#define PCIE_X4_INT_STATUS         (0x124)
#define PCIE_INT_CLR               (0x130)
#define PCIE_PHY_PLL_STATE         (0x134)

#define PCIE_PHY_SRC_SEL           (0x13C)
    #define PIPE_LANE0_PHY0           0
    #define PIPE_LANE1_PHY0           0
    #define PIPE_LANE2_PHY1           (1 << 4)
    #define PIPE_LANE3_PHY1           (1 << 6)

#define PCIE_LANEX_LINK_NUM        (0x140)
    #define PIPE_LANE0_LINK0          0
    #define PIPE_LANE1_LINK0          0
    #define PIPE_LANE2_LINK1          (1 << 8)
    #define PIPE_LANE3_LINK1          (1 << 12)
/*
 * -------------------------------------------------------------------------
 ************ CORE configure ************
 * -------------------------------------------------------------------------
 */
#define GIC_BASE_ADDR       (0x32000000)

#define A55_TIMER_BASE_ADDR (0x32008000)

/* A55 */
#define A55_WDT0_BASE_ADDR (0x32009000)
#define A55_WDT1_BASE_ADDR (0x3200A000)
#define A55_WDT2_BASE_ADDR (0x3200B000)
#define A55_WDT3_BASE_ADDR (0x3200C000)
#define A55_WDT4_BASE_ADDR (0x3200D000)
#define A55_WDT5_BASE_ADDR (0x3200E000)
#define A55_WDT6_BASE_ADDR (0x3200F000)
#define A55_WDT7_BASE_ADDR (0x32010000)

#define CPU_CSR_BASE_ADDR  (0x32011000)

/*
 * -------------------------------------------------------------------------
 * TOP System Control Module
 * -------------------------------------------------------------------------
 */
#define TOP_SYS_CTRL_BASE     (0x33000000)

#define CPU_BOOT_ADDR           (TOP_SYS_CTRL_BASE+0x2C)
#define SDEMMC_TIMER_DIV_REG    (TOP_SYS_CTRL_BASE+0x5C)
#define SDEMMC_ECC_DELAY_REG    (TOP_SYS_CTRL_BASE+0x60)
#define SDEMMC_CTRL_EN_CLR_REG  (TOP_SYS_CTRL_BASE+0x68)

/*
 * -------------------------------------------------------------------------
 * TOP System PAD Module
 * -------------------------------------------------------------------------
 */
#define TOP_SYS_PAD_BASE      (0x33001000)
#define TOP_SYS_PAD_SIZE      (0x1000)

#define IO_CFG_REG_0     (TOP_SYS_PAD_BASE+0xa0)
#define IO_CFG_REG_1     (TOP_SYS_PAD_BASE+0xa4)
#define IO_CFG_REG_2     (TOP_SYS_PAD_BASE+0xa8)
#define IO_CFG_REG_3     (TOP_SYS_PAD_BASE+0xac)
#define IO_CFG_REG_4     (TOP_SYS_PAD_BASE+0xb0)
#define IO_CFG_REG_5     (TOP_SYS_PAD_BASE+0xb4)
#define IO_CFG_REG_6     (TOP_SYS_PAD_BASE+0xb8)

#define TOP_PMM_REG_0    (TOP_SYS_PAD_BASE+0x00)
#define TOP_PMM_REG_1    (TOP_SYS_PAD_BASE+0x04)
#define TOP_PMM_REG_2    (TOP_SYS_PAD_BASE+0x08)

/*
 * -------------------------------------------------------------------------
 * System CRM Module
 * -------------------------------------------------------------------------
 */
#define A1000_TOPCRM_BASE           (0x33002000UL)
#define A1000_TOPCRM_SIZE           (0x1000)
#define SAFETY_CRM_BASE             (0x70035000)
#define SAFETY_CRM_SIZE             (0x1000)

#define CRM_BLOCK_SW_RST0           (0x04)
#define CRM_PLL_CPU_REG0            (0x08)
#define CRM_PLL_CPU_REG1            (0x0C)
#define CRM_PLL_CPU_REG2            (0x10)
#define CRM_PLL_CPU_REG3            (0x14)

#define PLL_CPU_FBDIV_SHIFT         (16)
#define PLL_CPU_FBDIV_MASK          (0xFFF)

#define PLL_CPU_DSMEN               (1<<13)

#define PLL_CPU_REFDIV_SHIFT        (24)
#define PLL_CPU_REFDIV_MASK         (0x3F)

#define PLL_CPU_FRAC_SHIFT          (0)
#define PLL_CPU_FRAC_MASK           (0xFFFFFF)

#define TOPCRM_CLK_GATE_EN          0xac
    #define USB3REFALTCLKGATEEN     (1<<29)

#define TOPCRM_BLOCK_SW_RST1        0xe8
    #define USB3SWRESETN            (1<<9)
    #define USB2SWRESETN            (1<<10)

#define CRM_BLOCK_HW_RST0           (0xC0)

#define CRM_BLOCK_SW_RST1           (0xE8)
#define LB_PCIE_SW_RESET_N          (1<<8)  /* PCIe block soft reset. Active Low */

#define IPC_BASE                    (0x33100000)
#define GDMA_BASE_ADDR              (0x33200000)
#define GPU_BASE_ADDR               (0x33300000)

/*
 * -------------------------------------------------------------------------
 ************ DDR ************
 * -------------------------------------------------------------------------
 */
#define DDRC0_CTRL_PHY_ADDR         (0x38000000)
#define DDRC1_CTRL_PHY_ADDR         (0x3C000000)

/* -------------------------------------------------------------------------
 ************ CV Control ************
 * -------------------------------------------------------------------------
 */
#define CV_SREG_ADDR    (0x51030000)
#define CV_SREG_CORE_RELEASE  (CV_SREG_ADDR + 0x0)
#define CV_SREG_CORE1_START   (CV_SREG_ADDR + 0x4)
#define CV_SREG_CORE2_START   (CV_SREG_ADDR + 0x8)
#define CV_SREG_CORE3_START   (CV_SREG_ADDR + 0xC)
#define CV_SREG_CORE4_START   (CV_SREG_ADDR + 0x10)

#define CV_GWARP_ADDR   (0x51040000)

#define ISP_BASE        (0x52000000)
#define GWARP_BASE      (0x53000000)
#define VOUT_BASE       (0x53080000)
#define VSP_BASE        (0x53090000)
#define BSMEM_BASE      (0x54000000)

/* -------------------------------------------------------------------------
 * SAFETY CRM Module
 * -------------------------------------------------------------------------
 */
#ifdef CONFIG_A55
#define SAFETY_CRM_ADDR (0x70035000)
#else
#define SAFETY_CRM_ADDR (0xC0035000)
#endif

#define SAFE_RESET_CTRL      0x08
#define SAFF_RESET_SEL       0x0C

#define STRAP_PIN_READ       0x100
#define PCIE_X4_IS_RC          (1<<8)
#define PCIE_BIFURC_EN         (1<<7)

#define SAFENOC_CFG          0x114

/* -------------------------------------------------------------------------
 * AON PAD PINMUX
 * -------------------------------------------------------------------------
 */
#ifdef CONFIG_A55
#define GPIO_PINMUX (0x70038000)
#else
#define GPIO_PINMUX (0xC0038000)
#endif

#define AON_PAD_BASE      (GPIO_PINMUX)
#define AON_PAD_SIZE      (0x1000)

#define AON_PMM_REG_0     (AON_PAD_BASE + 0x0)
#define AON_PMM_REG_1     (AON_PAD_BASE + 0x4)
#define AON_PMM_REG_2     (AON_PAD_BASE + 0x8)

#define AON_IO_IE_SW0     (AON_PAD_BASE + 0x200)
#define AON_IO_IE_SW1     (AON_PAD_BASE + 0x204
#define AON_IO_IE_SW2     (AON_PAD_BASE + 0x208)

#define AON_CFG_REG_0     (AON_PAD_BASE + 0x50)
#define AON_CFG_REG_1     (AON_PAD_BASE + 0x54)
#define AON_CFG_REG_2     (AON_PAD_BASE + 0x58)
#define AON_CFG_REG_3     (AON_PAD_BASE + 0x5c)
#define AON_CFG_REG_4     (AON_PAD_BASE + 0x60)
#define AON_CFG_REG_5     (AON_PAD_BASE + 0x64)
#define AON_CFG_REG_6     (AON_PAD_BASE + 0x68)

/* -------------------------------------------------------------------------
 * Secure IP address
 * -------------------------------------------------------------------------
 */
#define R5_CONFIG_ADDR    (0xf0000000)

#define SECURE_CTRL_ADDR  (0xf0001000)
#define SECURE_CLOCK_CTRL    (SECURE_CTRL_ADDR + 0x0)
#define SECURE_BLOCK_RELEASE (SECURE_CTRL_ADDR + 0x4)
#define SECURE_SRAM_FIREWALL (SECURE_CTRL_ADDR + 0x10)

#define OTP_BASE_ADDR     (0xf0002000)
#define SECURE_TIMER_ADDR (0xf0003000)
#define SECURE_WDT_ADDR   (0xf0004000)
#define TRNG_BASE_ADDR    (0xf0005000)
#define AES_BASE_ADDR     (0xf0006000)
#define MSHA_BASE_ADDR    (0xf0007000)
#define SM_BASE_ADDR      (0xf0008000)
#define CRC_BASE_ADDR     (0xf0009000)
#define SECURE_INTC_ADDR  (0xf000A000)
#define PKA_BASE_ADDR     (0xf0010000)

/* -------------------------------------------------------------------------
 * R5 safety
 * -------------------------------------------------------------------------
 */
#define SAFETY_GIC_BASE_ADDR (0xC0000000)

#define SDMA_BASE_ADDR       (0xC0020000)

#define SRAM_FIREWALL        (0xF0001010)

/* -------------------------------------------------------------------------
 * Interrupt number allocation
 * -------------------------------------------------------------------------
 */
#define GTIMER_NONS_PHY_INT     30
#define GTIMER_SECURE_PHY_INT   29
#define GTIMER_VIRTUAL_INT      27
#define GTIMER_HYPERVISOR_INT   26

#define FIQ_IRQ_A55_WDT0     99
#define FIQ_IRQ_A55_WDT1     100
#define FIQ_IRQ_A55_WDT2     101
#define FIQ_IRQ_A55_WDT3     102
#define FIQ_IRQ_A55_WDT4     103
#define FIQ_IRQ_A55_WDT5     104
#define FIQ_IRQ_A55_WDT6     105
#define FIQ_IRQ_A55_WDT7     106

#define FIQ_IRQ_A55_TIMER0 107
#define FIQ_IRQ_A55_TIMER1 108
#define FIQ_IRQ_A55_TIMER2 109
#define FIQ_IRQ_A55_TIMER3 110
#define FIQ_IRQ_A55_TIMER4 111
#define FIQ_IRQ_A55_TIMER5 112
#define FIQ_IRQ_A55_TIMER6 113
#define FIQ_IRQ_A55_TIMER7 114

#define FIQ_IRQ_AXI_SRAM_PTY_INTR   121
#define FIQ_IRQ_AXI_SRAM_ECC_INTR   122
#define FIQ_IRQ_R5_BUS_ECC_ERR      123
#define FIQ_IRQ_R5_LOCK_ERR         124

#define FIQ_IRQ_LSP0_WDT0     125
#define FIQ_IRQ_LSP0_WDT1     126
#define FIQ_IRQ_LSP1_WDT0     127
#define FIQ_IRQ_LSP1_WDT1     128

#define FIQ_IRQ_LSP0_TIMER0  129
#define FIQ_IRQ_LSP0_TIMER1  130
#define FIQ_IRQ_LSP1_TIMER0  131
#define FIQ_IRQ_LSP1_TIMER1  132

#define FIQ_IRQ_CRM_SOFT_INT 137

#define FIQ_IRQ_DMA_CMN_INTR 176
#define FIQ_IRQ_DMA_CH1_INTR 177
#define FIQ_IRQ_DMA_CH2_INTR 178
#define FIQ_IRQ_DMA_CH3_INTR 179
#define FIQ_IRQ_DMA_CH4_INTR 180
#define FIQ_IRQ_DMA_CH5_INTR 181
#define FIQ_IRQ_DMA_CH6_INTR 182
#define FIQ_IRQ_DMA_CH7_INTR 183
#define FIQ_IRQ_DMA_CH8_INTR 184

#define FIQ_IRQ_GMAC0        191
#define FIQ_IRQ_GMAC1        203

#define A55_IRQ_PCIE_X4_INT               223
#define A55_IRQ_PCIE_X4_CFG_SAFETY_CORR   224
#define A55_IRQ_PCIE_X4_CFG_SAFETY_UNCORR 225
#define A55_IRQ_PCIE_X2_INT               226
#define A55_IRQ_PCIE_X2_CFG_SAFETY_CORR   227
#define A55_IRQ_PCIE_X2_CFG_SAFETY_UNCORR 228
#define A55_IRQ_PCIE_X4_MSI_CTRL_INT      229
#define A55_IRQ_PCIE_X4_OTHER_INT         230
#define A55_IRQ_PCIE_X2_OTHER_INT         231

#define FIQ_IRQ_HSP_USB3  232

#define FIQ_IRQ_LSP0_CAN0 235
#define FIQ_IRQ_LSP1_CAN1 236

#define FIQ_IRQ_LSP0_I2C0_IRQ 239
#define FIQ_IRQ_LSP0_I2C1_IRQ 240
#define FIQ_IRQ_LSP0_I2C2_IRQ 241
#define FIQ_IRQ_LSP0_I2C3_IRQ 242
#define FIQ_IRQ_LSP0_I2C4_IRQ 243
#define FIQ_IRQ_LSP0_I2C5_IRQ 244

#define FIQ_IRQ_LSP0_UART0 245
#define FIQ_IRQ_LSP0_UART1 246
#define FIQ_IRQ_LSP1_UART0 247
#define FIQ_IRQ_LSP1_UART1 248

#define FIQ_IRQ_LSP0_QSPI0 249
#define FIQ_IRQ_LSP1_QSPI1 250

#define FIQ_IRQ_LSP0_GPIO0 255
#define FIQ_IRQ_LSP1_GPIO1 256

#endif /* _BSTA1000_H */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/public/aarch64/a1000.h $ $Rev: 944734 $")
#endif
