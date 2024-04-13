/*
 * $QNXLicenseC: 
 * Copyright 2007 - 2018, QNX Software Systems.  
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




#include "proto.h"

int
_spi_iomsg_read(resmgr_context_t *ctp, io_msg_t *msg, spi_ocb_t *ocb)
{
	spi_msg_t	*spimsg = (spi_msg_t *)msg;
	uint8_t		*buf;
	int		nbytes;
	SPIDEV		*drvhdl = (SPIDEV *)ocb->hdr.attr;
	spi_dev_t	*dev = drvhdl->hdl;
	uint16_t	chip = spimsg->device & SPI_DEV_ID_MASK;
	int		typed_fd;

	/* check if message buffer is too short */
	nbytes = spimsg->xlen;

	if (nbytes <= 0) {
		_IO_SET_READ_NBYTES(ctp, 0);
		return _RESMGR_NPARTS(0);
	}

	if ((nbytes > ctp->msg_max_size)  || (dev->typed_mem)) {
		if (dev->buflen < nbytes) {
			dev->buflen = nbytes;
			if (dev->buf) {
				free(dev->buf);
				dev->buf = NULL;
			}
			if(dev->typed_mem == NULL){
				if ((dev->buf = valloc(dev->buflen)) == NULL) {
					dev->buflen = 0;
					return ENOMEM;
				}
			} else {
				if((typed_fd = posix_typed_mem_open(
						dev->typed_mem, O_RDWR,
						POSIX_TYPED_MEM_ALLOCATE_CONTIG)) == -1) {
					_spi_slogf("MEM_OPEN FAILED: %d\n", typed_fd);
					dev->buflen = 0;
					return ENOMEM;
				}
				if((dev->buf = mmap(0, dev->buflen, PROT_READ|PROT_WRITE,
									MAP_SHARED, typed_fd, 0)) == MAP_FAILED) {
					_spi_slogf("MAP FAILED: %p\n", dev->buf);
					close(typed_fd);
					dev->buflen = 0;
					dev->buf = NULL;
					return ENOMEM;
				}
			}
		}

		buf = dev->buf;
	}
	else
		buf = (uint8_t *)msg;

	if (chip == SPI_DEV_ID_NONE)
		chip = ocb->chip;

	buf = dev->funcs->xfer(drvhdl, _SPI_DEV_READ(chip), buf, &nbytes);

	if (nbytes == 0)
		return EAGAIN;

	if (nbytes > 0) {
		_IO_SET_READ_NBYTES(ctp, nbytes);
		return _RESMGR_PTR(ctp, buf, nbytes);
	}

	return EIO;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/spi/master/_spi_iomsg_read.c $ $Rev: 872319 $")
#endif
