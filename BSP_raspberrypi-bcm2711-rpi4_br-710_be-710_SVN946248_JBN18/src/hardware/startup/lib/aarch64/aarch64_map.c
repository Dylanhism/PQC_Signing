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

#include "startup.h"
#include <aarch64/mmu.h>

/*
 * Page table manipulation
 */
#define	PTP_PADDR(ptp)		(((ptp) & 0x0000fffffffff000ULL) & ~(paddr_t)(__PAGESIZE -1))
#define	PTP_PTR(ptp)		((ptp_t *)PTP_PADDR(ptp))

#define	CONTIG_PAGES		16
#define	CONTIG_PAGES_64K	32
#define	USE_CONTIG(v, p, s, c) \
		((s) >= (c) && (((v) | (p)) & ((c) -1)) == 0)

#define USE_LARGE_PAGE() (__PAGESIZE > 0x1000)
#define NPTE_PTBL		(__PAGESIZE/sizeof(pte_t))

_Uint64t		L0_vaddr;
static ptp_t	*L1_ttbr0;
static ptp_t	*L1_ttbr1;
static uintptr_t	free_vaddr;

static pte_t
set_pte(paddr_t paddr, int flags)
{
	pte_t	pte = paddr | AARCH64_PTE_VALID | AARCH64_PTE_AF;

	/*
	 * Set access permissions
	 */
	if (flags & PROT_WRITE) {
		pte |= (flags & PROT_USER) ? AARCH64_PTE_URW : AARCH64_PTE_KRW;
	} else {
		pte |= (flags & PROT_USER) ? AARCH64_PTE_URO : AARCH64_PTE_KRO;
	}
	if ((flags & PROT_EXEC) == 0) {
		pte |= AARCH64_PTE_UXN | AARCH64_PTE_PXN;
	}

	/*
	 * Set mapping attributes.
	 * If SMP, set inner shareability for normal memory.
	 * (shareability is ignored for device attributes)
	 */
	if (flags & PROT_DEVICE) {
		pte |= (flags & PROT_NOCACHE) ? AARCH64_PTE_SODEV : AARCH64_PTE_DEV;
	} else {
		pte |= (flags & PROT_NOCACHE) ? AARCH64_PTE_NC : AARCH64_PTE_WBWA;
		if (lsp.syspage.p->num_cpu > 1) {
			pte |= AARCH64_PTE_ISH;
		}
	}
	if(meltdown_active > 0) {
		// can't use global descriptors with the meltdown workaround
		pte |= AARCH64_PTE_nG;
	}
	return pte;
}

static ptp_t
pt_alloc(void)
{
	paddr_t	pa = calloc_ram(__PAGESIZE, __PAGESIZE);

	if (pa == NULL_PADDR) {
		crash("pt_alloc: calloc_ram failed");
	}
	return set_pte(pa, PROT_READ|PROT_WRITE);
}

/*
 * Allocate initial page tables required for startup mappings:
 *
 * L1_ttbr1 is used to map 39/42 bits of system address space.
 * This is used to map bootstrap executables and syspage/callouts etc.
 *
 * L1_ttbr0 is used to map the low 39/42 bits of address space.
 * This is used to to create an identity mapping for _vstart.
 */
void
aarch64_map_init(void)
{
	unsigned	i;
	paddr_t		L0_paddr;
	ptp_t		L1pte;
	ptp_t		L2pte;
	ptp_t		*L0;
	ptp_t		*L2;

	/*
	 * Allocate TTBR0 L1 table used to map the startup code so vstart() has
	 * an identity mapping for the code to enable the MMU.
	 *
	 * we set the TTBR0 register to point at the L1 table using ASID 0.
	 */
	ttbr0 = calloc_ram(__PAGESIZE, __PAGESIZE);
	L1_ttbr0 = PTP_PTR(ttbr0);

	/*
	 * Allocate TTBR1 L1 table uses to map the system address space.
	 * Map the L1 table within itself to create recursive L2/L3 tables.
	 */
	L1pte = pt_alloc();
	L1_ttbr1 = PTP_PTR(L1pte);
	L1_ttbr1[__L1_L1_IDX] = L1pte;

       if(!USE_LARGE_PAGE()) {
		/*
		 * default 4K page size.
		 * Allocate L2 table for startup mappings (syspage, callouts etc.)
		 * Map the L2 within itself to create recursive L3 tables.
		 */
		L2pte = pt_alloc();
		L1_ttbr1[__L1_L2_IDX] = L2pte;
		L2 = PTP_PTR(L2pte);
		L2[__L2_L2_IDX] = L1pte;
		free_vaddr = AARCH64_STARTUP_BASE;
	} else {
		/*
		 * 64K page size, there are only 3 levels of page tables, we name the L1 as L2,
		 */
		free_vaddr = AARCH64_STARTUP_BASE_64K;
	}

	/*
	 * Allocate L1 tables used to map each cpu's system address space.
	 * T1SZ, bits [21:16], The region size is 2^(64-T1SZ) bytes, 39/42 bits per slot.
	 * the last slot (nxfer_slots - 1) is for kernel slots.
	 * slot 0 to nxfer_slots -2 is for message passsing.
	 */
	size_t l0size = nxfer_slots * sizeof(pte_t) * PROCESSORS_MAX;
	l0size = ROUNDPG(l0size);
	L0_paddr = calloc_ram(l0size, __PAGESIZE);
	L0_vaddr = aarch64_map(~0, L0_paddr, l0size, PROT_READ|PROT_WRITE);
	for (L0 = (ptp_t *)L0_paddr, i = 0; i < PROCESSORS_MAX; i++) {
		ttbr1[i] = (unsigned long)L0;
		L0[nxfer_slots -1] = L1pte;
		L0 += nxfer_slots;
	}
}

/*
 * Map [paddr, paddr+size) with protection/attributes in flags.
 * If vaddr is ~0, we assign the address, otherwise, the mapping
 * will be made at the specified virtual address.
 */
paddr_t
aarch64_map(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	ptp_t		*L1;
	pte_t		pte;
	uintptr_t	va;
	uintptr_t	end;
	unsigned	L1i;
	paddr_t		off;

	if (!(shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL)) {
		return paddr;
	}

	off = paddr & (__PAGESIZE-1);
	paddr &= ~(__PAGESIZE-1);
	size = ROUNDPG(size + off);
	if (vaddr == ~(uintptr_t)0) {
		vaddr = free_vaddr;
		free_vaddr += size;
	} else {
		if ((vaddr & (__PAGESIZE-1)) != off) {
			crash("aarch64_map: bad alignment va=%v pa=%v\n", vaddr, paddr+off);
		}
	}

	va = TRUNCPG(vaddr);
	end = va + size;
	if (end < va) {
		crash("aarch64_map: %v < %v, vaddr=%v, size=%d\n", end, va, vaddr, size);
	}

	/*
	 * Figure out which L1 table to use, kernel or startup.
	 */
	if (va < AARCH64_USER_END) {
		if (end >= AARCH64_USER_END) {
			crash("aarch64_map: %v >= %v\n", end, AARCH64_USER_END);
		}
		L1 = L1_ttbr0;
	} else {
		uintptr_t base = (USE_LARGE_PAGE()) ? AARCH64_STARTUP_BASE_64K : AARCH64_STARTUP_BASE;
		if (va < base) {
			crash("aarch64_map: %v < %v\n", end, base);
		}
		L1 = L1_ttbr1;
	}

	/*
	 * Set initial pte descriptor
	 */
	pte = set_pte(paddr, flags);

	const int contig_pages = (USE_LARGE_PAGE()) ? CONTIG_PAGES_64K : CONTIG_PAGES;

	L1i = (USE_LARGE_PAGE()) ? 0 : __L1_IDX(va);
	for (; L1i < NPTE_PTBL && va < end; L1i++) {
		ptp_t		*L2;
		unsigned	L2i;

		/*
		 * Map everything covered by this L2 table
		 */
		if (USE_LARGE_PAGE()) {
			/*
			 * for 64K page size, L2 is actually L1.
			 */
			L2 = L1;
			L2i = __L2_IDX_64K(va);
		} else {
			if (L1[L1i] == 0) {
				L1[L1i] = pt_alloc();
			}
			L2 = PTP_PTR(L1[L1i]);
			L2i = __L2_IDX(va);
		}
		for ( ; (L2i < NPTE_PTBL) && (va < end); L2i++) {
	 		ptp_t		*L3;
			unsigned	L3i;

			/*
			 * Map everything covered by this L3 table
			 */
			if (L2[L2i] == 0) {
				L2[L2i] = pt_alloc();
			}
			L3 = PTP_PTR(L2[L2i]);
			L3i = (USE_LARGE_PAGE()) ? __L3_IDX_64K(va) : __L3_IDX(va);
			while ((L3i < NPTE_PTBL) && (va < end)) {
				int		i;
				int		n;

				if (USE_CONTIG(va, TRUNCPG(pte), end - va, ((paddr_t)contig_pages * __PAGESIZE))) {
					pte |= AARCH64_PTE_CONTIG;
					n = contig_pages;
				} else {
					n = 1;
				}
				for (i = 0; i < n; i++) {
					L3[L3i++] = pte;
					va += __PAGESIZE;
					pte += __PAGESIZE;
				}
				if (n == contig_pages) {
					pte &= ~AARCH64_PTE_CONTIG;
				}
			}
		}
	}

	return vaddr + off;
}

paddr_t
elf_vaddr_to_paddr(uintptr_t vaddr)
{
	ptp_t	*L1;
	ptp_t	*L2;
	ptp_t	*L3;
	paddr_t	pa;

	/*
	 * Figure out which L1 table to use
	 */
	if (vaddr < AARCH64_USER_END) {
		L1 = L1_ttbr0;
	} else {
		uintptr_t base = (USE_LARGE_PAGE()) ? AARCH64_STARTUP_BASE_64K : AARCH64_STARTUP_BASE;
		if (vaddr < base) {
			crash("elf_vaddr_to_paddr: bad address %v\n", vaddr);
		}
		L1 = L1_ttbr1;
	}

	if(USE_LARGE_PAGE()) {
		/*
		 * for 64K page size, L2 is actually L1.
		 */
		L2 = L1;
	} else {
		/*
		 * Get L2 table address or block paddr from L1 entry
		 */
		pa = L1[__L1_IDX(vaddr)];
		if ((pa & AARCH64_PTP_VALID) == 0) {
			return NULL_PADDR;
		}
		if (AARCH64_PTP_TYPE(pa) == AARCH64_PTP_BLOCK) {
			return PTP_PADDR(pa) | __L1_VOFF(vaddr);
		}
		L2 = PTP_PTR(pa);
	}

	/*
	 * Get L3 table address or block paddr from L2 entry
	 */
	unsigned idx = (USE_LARGE_PAGE()) ? __L2_IDX_64K(vaddr) : __L2_IDX(vaddr);
	pa = L2[idx];
	if ((pa & AARCH64_PTP_VALID) == 0) {
		return NULL_PADDR;
	}
	if (AARCH64_PTP_TYPE(pa) == AARCH64_PTP_BLOCK) {
		const uintptr_t l2off = (USE_LARGE_PAGE()) ? __L2_VOFF_64K(vaddr) : __L2_VOFF(vaddr);
		return (PTP_PADDR(pa) | l2off);
	}
	L3 = PTP_PTR(pa);

	/*
	 * Get physical address from L3 entry
	 */
	idx = (USE_LARGE_PAGE()) ? __L3_IDX_64K(vaddr) : __L3_IDX(vaddr);
	pa = L3[idx];
	if ((pa & AARCH64_PTE_VALID) != AARCH64_PTE_VALID) {
		return NULL_PADDR;
	}
	const uintptr_t l3off = (USE_LARGE_PAGE()) ? __L3_VOFF_64K(vaddr) : __L3_VOFF(vaddr);
	return (PTP_PADDR(pa) | l3off);
};



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/aarch64/aarch64_map.c $ $Rev: 931478 $")
#endif
