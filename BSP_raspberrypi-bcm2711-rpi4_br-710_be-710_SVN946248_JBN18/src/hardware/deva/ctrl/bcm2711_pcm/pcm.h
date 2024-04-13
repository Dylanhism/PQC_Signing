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

#ifndef __BCM2711PCM_H__
#define __BCM2711PCM_H__

typedef struct bcm2711pcm_subchn bcm2711pcm_subchn_t;
typedef struct bcm2711pcm_context bcm2711pcm_context_t;

#define PCM_SUBCHN_CONTEXT_T bcm2711pcm_subchn_t
#define MIXER_CONTEXT_T bcm2711pcm_context_t
#define HW_CONTEXT_T bcm2711pcm_context_t

#define PCM_CHANNELS 2

#define PCM_DREQ_PANIC_MAX 127
#define PCM_DREQ_REQ_MAX   127

#include "dma.h"
#include <audio_driver.h>
#include <mixer/i2s_codec_dll.h>

/* BCM2711 PCM register map */

/* PCM CS_A Register - This register contains the main control and status bits for the PCM.
 * The bottom 3 bits of this register can be written to whilst the PCM is running. The remaining bits cannot.
 */
#define PCM_CS_STBY                 (1<<25) /* RW, RAM Standby */
    /* This bit is used to control the PCM Rams standby mode. By default this bit is 0 causing RAMs to start initially
     * in standby mode. Rams should be released from standby prior to any transmit/receive operation. Allow for at least
     * 4 PCM clock cycles to take effect. This may or may not be implemented, depending upon the RAM libraries being used.
     */
#define PCM_CS_SYNC                 (1<<24) /* RW, PCM Clock sync helper. */
    /* This bit provides a software synchronisation mechanism to allow the software to detect when 2 PCM clocks have occurred.
     * It takes 2 PCM clocks before the value written to this bit will be echoed back in the read value.
     */
#define PCM_CS_RXSEX                (1<<23) /* RW, RX Sign Extend */
    /* 0 = No sign extension. 1 = Sign extend the RX data.
     * When set, the MSB of the received data channel (as set by the CHxWID parameter) is repeated in all the higher data bits
     * up to the full 32 bit data width.
     */
#define PCM_CS_RXF                  (1<<22) /* RO, RX FIFO Full */
    /* 0 = RX FIFO can accept more data.
     * 1 = RX FIFO is full and will overflow if more data is received.
     */
#define PCM_CS_TXE                  (1<<21) /* RO, TX FIFO Empty */
    /* 0 = TX FIFO is not empty.
     * 1 = TX FIFO is empty and underflow will take place if no more data is written.
     */
#define PCM_CS_RXD                  (1<<20) /* RO, Indicates that the RX FIFO contains data */
    /* 0 = RX FIFO is empty. 1 = RX FIFO contains at least 1 sample. */
#define PCM_CS_TXD                  (1<<19) /* RO, Indicates that the TX FIFO can accept data */
    /* 0 = TX FIFO is full and so cannot accept more data.
     * 1 = TX FIFO has space for at least 1 sample.
     */
#define PCM_CS_RXR                  (1<<18) /* RO, Indicates that the RX FIFO needs reading */
    /* 0 = RX FIFO is less than RXTHR full. 1 = RX FIFO is RXTHR or more full.
     * This is cleared by reading sufficient data from the RX FIFO.
     */
#define PCM_CS_TXW                  (1<<17) /* RO, Indicates that the TX FIFO needs Writing */
    /* 0 = TX FIFO is at least TXTHR full. 1 = TX FIFO is less then TXTHR full.
     * This is cleared by writing sufficient data to the TX FIFO.
     */
#define PCM_CS_RXERR                (1<<16) /* RW, RX FIFO Error */
    /* 0 = FIFO has had no errors. 1 = FIFO has had an under or overflow error.
     * This flag is cleared by writing a 1.
     */
#define PCM_CS_TXERR                (1<<15) /* RW, TX FIFO Error */
    /* 0 = FIFO has had no errors. 1 = FIFO has had an under or overflow error.
     * This flag is cleared by writing a 1.
     */
#define PCM_CS_RXSYNC               (1<<14) /* RO, RX FIFO Sync */
    /* 0 = FIFO is out of sync. The amount of data left in the FIFO is not a multiple of
     * that required for a frame. This takes into account if we are halfway through the frame.
     * 1 = FIFO is in sync.
     */
#define PCM_CS_TXSYNC               (1<<13) /* RO, TX FIFO Sync */
    /* 0 = FIFO is out of sync. The amount of data left in the FIFO is not a multiple of
     * that required for a frame. This takes into account if we are halfway through the frame.
     * 1 = FIFO is in sync.
     */
#define PCM_CS_DMAEN                (1<<9) /* RW, DMA DREQ Enable */
    /* 0 = Don't generate DMA DREQ requests.
     * 1 = Generates a TX DMA DREQ requests whenever the TX FIFO level is lower than TXREQ or
     * generates a RX DMA DREQ when the RX FIFO level is higher than RXREQ.
     */
#define PCM_CS_RXTHR(n)             ((n)<<7) /* RW, Sets the RX FIFO threshold at which point the RXR flag is set */
#define PCM_CS_RXTHR_MSK            PCM_CS_RXTHR(3)
    /* 00 = set when we have a single sample in the RX FIFO
     * 01 = set when the RX FIFO is at least full
     * 10 = set when the RX FIFO is at least
     * 11 = set when the RX FIFO is full
     */
#define PCM_CS_TXTHR(n)             ((n)<<5) /* RW, Sets the TX FIFO threshold at which point the TXW flag is set */
#define PCM_CS_TXTHR_MSK            PCM_CS_TXTHR(3)
    /* 00 = set when the TX FIFO is empty
     * 01 = set when the TX FIFO is less than 1/4 full
     * 10 = set when the TX FIFO is less than 3/4 full
     * 11 = set when the TX FIFO is full but for one sample
     */
#define PCM_CS_RXCLR                (1<<4) /* WO Clear the RX FIFO. */
    /* Assert to clear RX FIFO. This bit is self clearing and is always read as clear
     * Note that it will take 2 PCM clocks for the FIFO to be physically cleared.
     */
#define PCM_CS_TXCLR                (1<<3) /* WO Clear the TX FIFO */
    /* Assert to clear TX FIFO. This bit is self clearing and is always read as clear.
     * Note that it will take 2 PCM clocks for the FIFO to be physically cleared.
     */
#define PCM_CS_TXON                 (1<<2) /* RW Enable transmission */
    /* 0 = Stop transmission. This will stop immediately if possible or else at the end of the next frame.
     * The TX FIFO can still be written to to preload data.
     * 1 = Start transmission. This will start transmitting at the start of the next frame. Once enabled,
     * the first data read from the TX FIFO will be placed in the first channel of the frame, thus ensuring
     * proper channel synchronisation.
     * The frame counter will be started whenever TXON or RXON are set.
     * This bit can be written whilst the interface is running.
     */
#define PCM_CS_RXON                 (1<<1) /* RW Enable reception. */
    /* 0 = Disable reception. This will stop on the next available frame end. RX FIFO data can still be read.
     * 1 = Enable reception. This will be start receiving at the start of the next frame. The first channel
     * to be received will be the first word written to the RX FIFO.
     * This bit can be written whilst the interface is running.
     */
#define PCM_CS_EN                   (1<<0) /* RW Enable the PCM Audio Interface */
    /* 0 = The PCM interface is disabled and most logic is gated off to save power.
     * 1 = The PCM Interface is enabled.
     * This bit can be written whilst the interface is running.
     */

/* PCM MODE_A Register - This register defines the basic PCM Operating Mode.
 * It is used to configure the frame size and format and whether the PCM is in
 * master or slave modes for its frame sync or clock. This register cannot be
 * changed whilst the PCM is running.
 */
#define PCM_MODE_CLK_DIS            (1<<28) /* RW PCM Clock Disable */
    /* 1 = Disable the PCM Clock. This cleanly disables the PCM clock. This enables glitch free clock switching
     * between an internal and an uncontrollable external clock. The PCM clock can be disabled, and then the clock
     * source switched, and then the clock re-enabled.
     * 0 = Enable the PCM clock.
     */
#define PCM_MODE_PDMN               (1<<27) /* RW PDM Decimation Factor (N) */
    /* 0 = Decimation factor 16. 1 = Decimation factor 32.
     * Sets the decimation factor of the CIC decimation filter.
     */
#define PCM_MODE_PDME               (1<<26) /* RW PDM Input Mode Enable */
    /* 0 = Disable PDM (classic PCM input).
     * 1 = Enable PDM input filter. Enable CIC filter on input pin for PDM inputs. In order to receive data RXON must also be set.
     */
#define PCM_MODE_FRXP               (1<<25) /* RW Receive Frame Packed Mode */
    /* 0 = The data from each channel is written into the RX FIFO.
     * 1 = The data from both RX channels is merged (1st channel is in the LS half) and then written to the RX FIFO
     * as a single 2x16 bit packed mode word. First received channel in the frame goes into the LS half word.
     * If the received data is larger than 16 bits, the upper bits are truncated. The maximum channel size is 16 bits.
     */
#define PCM_MODE_FTXP               (1<<24) /* RW Transmit Frame Packed Mode */
    /* 0 = Each TX FIFO word is written into a single channel.
     * 1 = Each TX FIFO word is split into 2 16 bit words and used to fill both data channels in the same frame.
     * The maximum channel size is 16 bits. The LS half of the word is used in the first channel of the frame.
     */
#define PCM_MODE_CLKM               (1<<23) /* RW PCM Clock Mode */
    /* 0 = Master mode. The PCM CLK is an output and drives at the MCLK rate.
     * 1 = Slave mode. The PCM CLK is an input.
     */
#define PCM_MODE_CLKI               (1<<22) /* RW Clock Invert this logically inverts the PCM_CLK signal. */
    /* 0 = Outputs change on rising edge of clock, inputs are sampled on falling edge.
     * 1 = Outputs change on falling edge of clock, inputs are sampled on rising edge.
     */
#define PCM_MODE_FSM                (1<<21) /* RW Frame Sync Mode */
    /* 0 = Master mode. The PCM_FS is an output and we generate the frame sync.
     * 1 = Slave mode. The PCM_FS is an input and we lock onto the incoming frame sync signal.
     */
#define PCM_MODE_FSI                (1<<20) /* RW Frame Sync Invert This logically inverts the frame sync signal. */
    /* 0 = In master mode, FS is normally low and goes high to indicate frame sync. In slave mode, the frame starts
     * with the clock where FS is a 1 after being a 0.
     * 1 = In master mode, FS is normally high and goes low to indicate frame sync. In slave mode, the frame starts
     * with the clock where FS is a 0 after being a 1.
     */
#define PCM_MODE_FLEN(n)            ((n)<<10) /* RW Frame Length */
#define PCM_MODE_FLEN_MSK           PCM_MODE_FLEN(1023)
    /* Sets the frame length to (FLEN+1) clocks. Used only when FSM == 0.
     * 1 = frame length of 2 clocks. 2 = frame length of 3 clocks. etc
     */
#define PCM_MODE_FSLEN(n)           ((n)<<0) /* RW Frame Sync Length */
#define PCM_MODE_FSLEN_MSK          PCM_MODE_FSLEN(1023)
    /* Sets the frame sync length to (FSLEN) clocks. This is only used when FSM == 0.
     * PCM_FS will remain permanently active if FSLEN >= FLEN.
     * 0 = frame sync pulse is off. 1 = frame sync pulse is 1 clock wide. etc
     */

/* PCM RXC_A Register - PCM Receive Configuration
 * Sets the Channel configurations for Receiving. This sets the position and width of the 2 receive channels within the
 * frame. The two channels cannot overlap, however they channel 1 can come after channel zero, although the first data
 * will always be from the first channel in the frame. Channels can also straddle the frame begin end boundary as that is
 * set by the frame sync position. This register cannot be changed whilst the PCM is running.
 */
#define PCM_RXC_CH1WEX(n)           (n<<31) /* RW Channel 1 Width Extension Bit */
    /* This is the MSB of the channel 1 width (CH1WID). It allows widths greater than 24 bits to be
     * programmed and is added here to keep backwards compatibility with older versions of the PCM
     */
#define PCM_RXC_CH1EN               (1<<30) /* RW Channel 1 Enable */
    /* 0 = Channel 1 disabled and no data is received from channel 1 and written to the RX FIFO.
     * 1 = Channel 1 enabled.
     */
#define PCM_RXC_CH1POS(n)           ((n)<<20) /* RW Channel 1 Position */
#define PCM_RXC_CH1POS_MSK          PCM_RXC_CH1POS(1023)
    /* This sets the bit clock at which the first bit (MS bit) of channel 1 data occurs in the frame.
     * 0 indicates the first clock of frame.
     */
#define PCM_RXC_CH1WID(n)           ((n)<<16) /* RW Channel 1 Width */
#define PCM_RXC_CH1WID_MSK          PCM_RXC_CH1WID(15)
    /* This sets the width of channel 1 in bit clocks. This field has been extended with the CH1WEX bit
     * giving a total width of (CH1WEX * 16) + CH1WID + 8. The Maximum supported width is 32 bits.
     * 0 = 8 bits wide. 1 = 9 bits wide
     */
#define PCM_RXC_CH2WEX(n)           (n<<15) /* RW Channel 2 Width Extension Bit */
    /* This is the MSB of the channel 2 width (CH2WID). It allows widths greater than 24 bits to be
     * programmed and is added here to keep backwards compatibility with older versions of the PCM
     */
#define PCM_RXC_CH2EN               (1<<14) /* RW Channel 2 Enable */
    /* 0 = Channel 2 disabled and no data is received from channel 2 and written to the RX FIFO.
     * 1 = Channel 2 enabled.
     */
#define PCM_RXC_CH2POS(n)           ((n)<<4) /* RW Channel 2 Position */
#define PCM_RXC_CH2POS_MSK          PCM_RXC_CH2POS(1023)
    /* This sets the bit clock at which the first bit (MS bit) of channel 2 data occurs in the frame.
     * 0 indicates the first clock of frame.
     */
#define PCM_RXC_CH2WID(n)           ((n)<<0) /* RW Channel 2 Width */
#define PCM_RXC_CH2WID_MSK          PCM_RXC_CH2WID(15)
    /* This sets the width of channel 2 in bit clocks. This field has been extended with the CH2WEX bit
     * giving a total width of (CH2WEX * 16) + CH2WID + 8. The Maximum supported width is 32 bits.
     * 0 = 8 bits wide. 1 = 9 bits wide
     */

/* PCM TXC_A Register - PCM Transmit Configuration
 * Sets the Channel configurations for Transmitting. This sets the position and width of the 2 transmit channels within
 * the frame. The two channels cannot overlap, however they channel 1 can come after channel zero, although the first data
 * will always be used in the first channel in the frame. Channels can also straddle the frame begin end boundary as that
 * is set by the frame sync position. This register cannot be changed whilst the PCM is running.
 */
#define PCM_TXC_CH1WEX(n)           (n<<31) /* RW Channel 1 Width Extension Bit */
    /* This is the MSB of the channel 1 width (CH1WID). It allows widths greater than 24 bits to be
     * programmed and is added here to keep backwards compatibility with older versions of the PCM
     */
#define PCM_TXC_CH1EN               (1<<30) /* RW Channel 1 Enable */
    /* 0 = Channel 1 disabled and no data is taken from the TX FIFO and transmitted on channel 1.
     * 1 = Channel 1 enabled.
     */
#define PCM_TXC_CH1POS(n)           ((n)<<20) /* RW Channel 1 Position */
#define PCM_TXC_CH1POS_MSK          PCM_TXC_CH1POS(1023)
    /* This sets the bit clock at which the first bit (MS bit) of channel 1 data occurs in the frame.
     * 0 indicates the first clock of frame.
     */
#define PCM_TXC_CH1WID(n)           ((n)<<16) /* RW Channel 1 Width */
#define PCM_TXC_CH1WID_MSK          PCM_TXC_CH1WID(15)
    /* This sets the width of channel 1 in bit clocks. This field has been extended with the CH1WEX bit
     * giving a total width of (CH1WEX * 16) + CH1WID + 8. The Maximum supported width is 32 bits.
     * 0 = 8 bits wide. 1 = 9 bits wide
     */
#define PCM_TXC_CH2WEX(n)           (n<<15) /* RW Channel 2 Width Extension Bit */
    /* This is the MSB of the channel 2 width (CH2WID). It allows widths greater than 24 bits to be
     * programmed and is added here to keep backwards compatibility with older versions of the PCM
     */
#define PCM_TXC_CH2EN               (1<<14) /* RW Channel 2 Enable */
    /* 0 = Channel 2 disabled and no data is taken from the TX FIFO and transmitted on channel 2.
     * 1 = Channel 2 enabled.
     */
#define PCM_TXC_CH2POS(n)           ((n)<<4)    /* RW Channel 2 Position */
#define PCM_TXC_CH2POS_MSK          PCM_TXC_CH2POS(1023)
    /* This sets the bit clock at which the first bit (MS bit) of channel 2 data occurs in the frame.
     * 0 indicates the first clock of frame.
     */
#define PCM_TXC_CH2WID(n)           ((n)<<0)    /* RW Channel 2 Width */
#define PCM_TXC_CH2WID_MSK          PCM_TXC_CH2WID(15)
    /* This sets the width of channel 2 in bit clocks. This field has been extended with the CH2WEX bit
     * giving a total width of (CH2WEX * 16) + CH2WID + 8. The Maximum supported width is 32 bits.
     * 0 = 8 bits wide. 1 = 9 bits wide
     */

/* PCM DREQ_A Register - PCM DMA Request Level
 * Set the DMA DREQ and Panic thresholds. The PCM drives 2 DMA controls back to the DMA, one for the TX channel and one
 * for the RX channel. DMA DREQ is used to request the DMA to perform another transfer, and DMA Panic is used to tell the
 * DMA to use its panic level of priority when requesting thins on the AXI bus. This register cannot be changed whilst the
 * PCM is running.
 */
#define PCM_DREQ_TX_PANIC(n)        ((n)<<24)    /* RW TX Panic Level */
#define PCM_DREQ_TX_PANIC_MSK       PCM_DREQ_TX_PANIC(127)
    /* This sets the TX FIFO Panic level. When the level is below this the PCM will assert its TX DMA Panic signal. */
#define PCM_DREQ_RX_PANIC(n)        ((n)<<16)    /* RW RX Panic Level */
#define PCM_DREQ_RX_PANIC_MSK       PCM_DREQ_RX_PANIC(127)
    /* This sets the RX FIFO Panic level. When the level is above this the PCM will assert its RX DMA Panic signal. */
#define PCM_DREQ_TX_REQ(n)          ((n)<<8)    /* RW TX Request Level */
#define PCM_DREQ_TX_REQ_MSK         PCM_DREQ_TX(127)
    /* This sets the TX FIFO DREQ level. When the level is below this the PCM will assert its DMA DREQ signal to
     * request more data is written to the TX FIFO.
     */
#define PCM_DREQ_RX_REQ(n)          ((n)<<0)    /* RW RX Request Level */
#define PCM_DREQ_RX_REQ_MSK         PCM_DREQ_RX(127)
    /* This sets the RX FIFO DREQ level. When the level is above this the PCM will assert its DMA DREQ signal to
     * request that some more data is read out of the RX FIFO.
     */

/* PCM INTEN_A Register - PCM Interrupt Enable
 * Set the reasons for generating an Interrupt. This register cannot be changed whilst the PCM is running.
 */
#define PCM_INTEN_RXERR             (1<<3)    /* RW RX Error Interrupt */
    /* Setting this bit enables interrupts from PCM block when RX FIFO error occurs. */
#define PCM_INTEN_TXERR             (1<<2)    /* RW TX Error Interrupt */
    /* Setting this bit enables interrupts from PCM block when TX FIFO error occurs. */
#define PCM_INTEN_RXR               (1<<1)    /* RW RX Read Interrupt Enable */
    /* Setting this bit enables interrupts from PCM block when RX FIFO level is greater equal to the specified RXTHR level. */
#define PCM_INTEN_TXW               (1<<0)    /* RW TX Write Interrupt Enable */
    /* Setting this bit enables interrupts from PCM block when TX FIFO level is less than the specified TXTHR level. */

/* PCM INTSTC_A Register - PCM Interrupt Status & Clear
 * This register is used to read and clear the PCM interrupt status.
 * Writing a 1 to the asserted bit clears the bit.  Writing a 0 has no effect.
 */
#define PCM_INTSTC_RXERR            (1<<3)    /* RW RX Error Interrupt Status / Clear */
    /* This bit indicates an interrupt occurred on RX FIFO Error.
     * Writing 1 to this bit clears it. Writing 0 has no effect.
     */
#define PCM_INTSTC_TXERR            (1<<2)    /* RW TX Error Interrupt Status / Clear */
    /* This bit indicates an interrupt occurred on TX FIFO Error.
     * Writing 1 to this bit clears it. Writing 0 has no effect.
     */
#define PCM_INTSTC_RXR              (1<<1)    /* RW RX Read Interrupt Status / Clear */
    /* This bit indicates an interrupt occurred on RX Read.
     * Writing 1 to this bit clears it. Writing 0 has no effect.
     */
#define PCM_INTSTC_TXW              (1<<0)    /* RW TX Write Interrupt Status / Clear */
    /* This bit indicates an interrupt occurred on TX Write.
     * Writing 1 to this bit clears it. Writing 0 has no effect.
     */

/* PCM GRAY Register - Gray Mode Control
 * This register is used to control the gray mode generation. This is used to put the PCM into a special data/strobe mode.
 * This mode is under 'best effort' contract.
 */
#define PCM_GRAY_RXFIFOLEVEL(n)     ((n)<<16) /* RO The Current level of the RXFIFO */
#define PCM_GRAY_RXFIFOLEVEL_MSK    PCM_GRAY_RXFIFOLEVEL(63)
    /* This indicates how many words are currently in the RXFIFO. */
#define PCM_GRAY_FLUSHED(n)         ((n)<<10) /* RO The Number of bits that were flushed into the RXFIFO */
#define PCM_GRAY_FLUSHED_MSK        PCM_GRAY_FLUSHED(63)
    /* This indicates how many bits were valid when the flush operation was performed.
     * The valid bits are from bit 0 upwards. Non-valid bits are set to zero.
     */
#define PCM_GRAY_RXLEVEL(n)         ((n)<<4)  /* RO The Current fill level of the RX Buffer */
#define PCM_GRAY_RXLEVEL_MSK        PCM_GRAY_RXLEVEL(63)
    /* This indicates how many GRAY coded bits have been received. When 32 bits are received, they are written
     * out into the RXFIFO.
     */
#define PCM_GRAY_FLUSH              (1<<2)    /* RW Flush the RX Buffer into the RX FIFO */
    /* This forces the RX Buffer to do an early write. This is necessary if we have reached the end of the message
     * and we have bits left in the RX Buffer. Flushing will write these bits as a single 32 bit word, starting at
     * bit zero. Empty bits will be packed with zeros. The number of bits written will be recorded in the FLUSHED Field.
     * This bit is written as a 1 to initiate a flush. It will read back as a zero until the flush operation has completed
     * (as the PCM Clock may be very slow).
     */
#define PCM_GRAY_CLR                (1<<1)    /* RW Clear the GRAY Mode Logic */
    /* This Bit will reset all the GRAY mode logic, and flush the RX buffer. It is not self clearing. */
#define PCM_GRAY_EN                 (1<<0)    /* RW Enable GRAY Mode */
    /* Setting this bit will put the PCM into GRAY mode. In gray mode the data is received on the data in and
     * the frame sync pins. The data is expected to be in data/strobe format.
     */

typedef struct bcm2711pcm_reg {
    uint32_t cs_a;
    uint32_t fifo_a;
    uint32_t mode_a;
    uint32_t rxc_a;
    uint32_t txc_a;
    uint32_t dreq_a;
    uint32_t inten_a;
    uint32_t intstc_a;
    uint32_t gray;
} bcm2711pcm_reg_t;

typedef enum bcm2711clk_mode {
    CLK_MODE_MASTER,
    CLK_MODE_SLAVE
} bcm2711clk_mode_t;

typedef struct bcm2711dma_context {
    const struct dma_config *cfg;
    void *hw;                   /* dma hardware */
    void *cb;                   /* dma control-block */
    off64_t cb_phys;            /* dma control-block legacy physical address */
    uint32_t cb_ti;
    uint32_t chan;
    uint32_t dreq;
    uint32_t thresh_dreq;       /* DMA Threshold for DREQ signal */
    uint32_t thresh_panic;      /* DMA Threshold for PANIC signal */
} bcm2711dma_context_t;

struct bcm2711pcm_subchn {
    ado_pcm_subchn_t *subchn;   /* Pointer to subchannel structure */
    ado_pcm_config_t *config;   /* Data structure that describes the configuration of a PCM subchannel */
    ado_pcm_cap_t caps;         /* Data structure of capabilities of a PCM device */
    ado_pcm_hw_t funcs;         /* Data structure of callbacks for PCM devices */
    bcm2711dma_context_t dma;
    uint32_t buffer_pos;
    int32_t sample_rate;        /* sample rate in Hz */
    bool configured;
    bool active;
    unsigned fifo_err_count;
    unsigned dma_err_count;
};

struct bcm2711pcm_context {
    pthread_mutex_t lock;
    ado_card_t *card;
    ado_pcm_t *pcm;
    const snd_pcm_chmap_t *chmap;
    ado_mixer_t *mixer;
    ado_mixer_dll_codec_callbacks_t codec_mixer_callbacks;
    bcm2711pcm_subchn_t capture;
    bcm2711pcm_subchn_t playback;
    volatile bcm2711pcm_reg_t *pcm_reg;  /* per-card pcm controller */
    uint32_t sample_rate_min;
    uint32_t sample_rate_max;
    uint32_t sample_rate;
    uint32_t sample_format;
    uint32_t sys_clk;             /* System clock frequency in Hz., i.e., mclk */
    uint32_t clk_mode;
    uint32_t clk_freq;            /* clock source frequency */
    uint32_t clk_src;             /* clock source */
    uint32_t clk_mash;
    uint32_t bit_delay;           /* bit delay in bit clock cycles */
    uint32_t clk_pol;             /* bit clock polarity */
    uint32_t fs_pol;              /* FS clock polarity */
};

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/bcm2711_pcm/pcm.h $ $Rev: 934572 $")
#endif
