/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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

//
// This code is hardware independant and should not have to be
// changed by end users.
//

static unsigned			fixed_size;
static unsigned 		callouts_size;

PADDR_T				syspage_paddr;
struct local_syspage	lsp;

struct unknown;
typedef SYSPAGE_TYPED_SECTION(struct unknown, __attribute__((__may_alias__)) local_section);

#define SECTION_START	((local_section *)(void *)&lsp)
#define SECTION_END		((local_section *)(void *)(&lsp+1))

// We keep the maximum size we're allowed to grow things in
// the lsp.syspage->p.size field - we'll set it to it's proper value
// once we're done, that'll also disallow any further growing since
// it's going to be less than the total_size.

void *
grow_syspage_section(void *p, unsigned add) {
	local_section	*sect = p;
	local_section	*s2;
	uint8_t			*bottom;
	unsigned		new_size;
	unsigned		max_size;
	unsigned		len;
	uint8_t			*new;

	mdriver_check();
	if(add > 0) {
		add = ROUND(add, sizeof(uint64_t));
		new_size = lsp.syspage.p->total_size + add;
		max_size = lsp.syspage.p->size;
		if(new_size > max_size) {
			crash("Syspage memory request of %d exceeds maximum of %d bytes.\n",
				new_size, max_size);
		}
		bottom = (uint8_t *)sect->p + sect->size;
		for(s2 = SECTION_START; s2 < SECTION_END; ++s2) {
			if((s2 != sect) && ((void *)s2->p >= (void *)bottom)) {
				s2->p = (void *)((uint8_t *)s2->p + add);
			}
		}
		new = bottom + add;
		len = lsp.syspage.p->total_size - PTR_DIFF(bottom, lsp.syspage.p);
		memmove(new, bottom, len);
		callout_reloc_data(bottom, len, (ptrdiff_t)add);
		memset(bottom, 0, add);
		sect->size += add;
		lsp.syspage.p->total_size = new_size;
	}
	return(sect->p);
}

void *
set_syspage_section(void *p, unsigned size) {
	local_section	*sect = p;

	size = ROUND(size,sizeof(uint64_t));
	sect->p = (void *)((uint8_t *)lsp.syspage.p + fixed_size);
	fixed_size += size;
	return(grow_syspage_section(p, size));
}
	
void
init_syspage_memory(void *base, unsigned max_size) {
	local_section	*sect;
	unsigned		spsize = ROUND(sizeof(*lsp.syspage.p),sizeof(uint64_t));

	for(sect = SECTION_START; sect < SECTION_END; ++sect) {
		sect->p = (void *)((uint8_t *)base + spsize);
		sect->size = 0;
	}
	lsp.syspage.p = base;
	lsp.syspage.size = spsize;
	memset(base, 0, spsize);
	fixed_size = spsize;
	lsp.syspage.p->total_size = spsize;
	lsp.syspage.p->size = max_size;
	lsp.syspage.p->asinfo.element_size = sizeof(struct asinfo_entry);
	lsp.syspage.p->cpuinfo.element_size = sizeof(struct cpuinfo_entry);
	lsp.syspage.p->cacheattr.element_size = sizeof(struct cacheattr_entry);
	lsp.syspage.p->intrinfo.element_size = sizeof(struct intrinfo_entry);
	lsp.syspage.p->mdriver.element_size = sizeof(struct mdriver_entry);
	set_syspage_section(&lsp.callout, sizeof(*lsp.callout.p));
	set_syspage_section(&lsp.callin, sizeof(*lsp.callin.p));
//	grow_syspage_section(&lsp.meminfo, sizeof(*lsp.meminfo.p));
	grow_syspage_section(&lsp.typed_strings, sizeof(uint32_t));
	grow_syspage_section(&lsp.strings, sizeof(char));
	grow_syspage_section(&lsp.hwinfo, sizeof(struct hwi_prefix));

	lsp.syspage.p->num_cpu = 1;

	cpu_init_syspage_memory();

	hwi_default();
}

void
reloc_syspage_memory(void *base, unsigned max_size) {
	local_section	*sect;
	ptrdiff_t		diff;

	memmove(base, lsp.syspage.p, lsp.syspage.p->total_size);
	diff = PTR_DIFF(base, lsp.syspage.p);
	callout_reloc_data(lsp.syspage.p, lsp.syspage.p->total_size, diff);
	for(sect = SECTION_START; sect < SECTION_END; ++sect) {
		sect->p = (void *)((uint8_t *)sect->p + diff);
	}
	lsp.syspage.p->size = max_size;
}

void
alloc_syspage_memory() {
	PADDR_T					cpupage_paddr;
	unsigned				i;
	struct cpupage_entry	cpu;

	struct syspage_entry	*sp = lsp.syspage.p;

	//figure out backware compatabilty section sizes
	unsigned num_mdriver = lsp.mdriver.size / sizeof(struct mdriver_entry);
	sp->old_mdriver.entry_size = num_mdriver * sizeof(struct old_mdriver_entry);

	unsigned num_intrinfo = lsp.intrinfo.size / sizeof(struct intrinfo_entry);
	sp->old_intrinfo.entry_size = num_intrinfo * sizeof(struct old_intrinfo_entry);

	//figure out size of callouts and it's rw_size.
	unsigned rwsize;
	callouts_size = output_callouts(&rwsize);

	//Figure out total size required for the system page. It is then followed by
	//callout rw data, callout code, cpupage_entry for each cpu.
	//Allow for four more asinfo_entry's in case the syspage allocation(s)
	//splits the address range(s).
	// in order to make callout code to be readonly, we need to make sure the callout code
	// is page aligned, so we need ROUNDPG before and after callout code.
	unsigned spsize = ROUNDPG(sp->total_size
			+ sp->old_mdriver.entry_size
			+ sp->old_intrinfo.entry_size
			+ rwsize
			+ 4*sizeof(struct asinfo_entry));
	spsize += ROUNDPG(callouts_size); // this make sure cpupage_entries are in a separate page

	_syspage_ptr = cpu_alloc_syspage_memory(&cpupage_paddr, &syspage_paddr, spsize);

	#define INIT_ENTRY(field)	\
			sp->field.entry_size = lsp.field.size;	\
			sp->field.entry_off = PTR_DIFF(lsp.field.p, sp)

	sp->size = sizeof(*lsp.syspage.p); // disallows further growing
	sp->type = CPU_SYSPAGE_TYPE;

	// Set the offsets for the backwards compatability sections
	// Can't do it earlier since the cpu_alloc_syspage_memory() call
	// might have to grow the asinfo section
	sp->old_mdriver.entry_off = sp->total_size;
	sp->total_size += sp->old_mdriver.entry_size;
	sp->old_intrinfo.entry_off = sp->total_size;
	sp->total_size += sp->old_intrinfo.entry_size;

	INIT_ENTRY(system_private);
	INIT_ENTRY(meminfo);
	INIT_ENTRY(asinfo);
	INIT_ENTRY(hwinfo);
	INIT_ENTRY(cpuinfo);
	INIT_ENTRY(cacheattr);
	INIT_ENTRY(qtime);
	INIT_ENTRY(callout);
	INIT_ENTRY(callin);
	INIT_ENTRY(typed_strings);
	INIT_ENTRY(strings);
	INIT_ENTRY(intrinfo);
	INIT_ENTRY(smp);
	INIT_ENTRY(pminfo);
	INIT_ENTRY(mdriver);

	memset(&cpu, 0, sizeof(cpu));
	cpu.syspage = lsp.system_private.p->user_syspageptr;
	for(i = 0; i < sp->num_cpu; ++i) {
		struct cpuinfo_entry	*cp;

		cp = startup_memory_map(sizeof(cpu), cpupage_paddr, PROT_READ|PROT_WRITE);
		cpu.cpu = i;
	   	memmove(cp, &cpu, sizeof(cpu));
		startup_memory_unmap(cp);
		cpupage_paddr += lsp.system_private.p->cpupage_spacing;
	}

	//write the callouts to the syspage.
	output_callouts(NULL);

	//make the callouts read only execute.
	uintptr_t callout_off = ROUNDPG(sp->total_size + rwsize);
	cpu_syspage_remap((uintptr_t)lsp.system_private.p->kern_syspageptr+callout_off, syspage_paddr+callout_off, callouts_size, PF_R|PF_X);

	// Initialize backwards compatability for array sections
	sp->old_asinfo.entry_off     = sp->asinfo.entry_off;
	sp->old_asinfo.entry_size    = sp->asinfo.entry_size;
	sp->old_cpuinfo.entry_off    = sp->cpuinfo.entry_off;
	sp->old_cpuinfo.entry_size   = sp->cpuinfo.entry_size;
	sp->old_cacheattr.entry_off  = sp->cacheattr.entry_off;
	sp->old_cacheattr.entry_size = sp->cacheattr.entry_size;
	struct old_mdriver_entry *const old_mdriver = _SYSPAGE_ENTRY(sp, old_mdriver);
	for(i = 0; i < num_mdriver; ++i) {
		old_mdriver[i].intr = lsp.mdriver.p[i].intr;
		old_mdriver[i].handler32 = (uintptr_t)lsp.mdriver.p[i].handler;
		old_mdriver[i].data32 = (uintptr_t)lsp.mdriver.p[i].data;
		old_mdriver[i].data_paddr = lsp.mdriver.p[i].data_paddr;
		old_mdriver[i].data_size = lsp.mdriver.p[i].data_size;
		old_mdriver[i].name = lsp.mdriver.p[i].name;
		old_mdriver[i].internal = lsp.mdriver.p[i].internal;
	}
	struct old_intrinfo_entry *const old_intrinfo = _SYSPAGE_ENTRY(sp, old_intrinfo);
	for(i = 0; i < num_intrinfo; ++i) {
		old_intrinfo[i].vector_base = lsp.intrinfo.p[i].vector_base;
		old_intrinfo[i].num_vectors = lsp.intrinfo.p[i].num_vectors;
		old_intrinfo[i].cascade_vector = lsp.intrinfo.p[i].cascade_vector;
		old_intrinfo[i].cpu_intr_base = lsp.intrinfo.p[i].cpu_intr_base;
		old_intrinfo[i].cpu_intr_stride = lsp.intrinfo.p[i].cpu_intr_stride;
		old_intrinfo[i].local_stride = lsp.intrinfo.p[i].local_stride;
		old_intrinfo[i].flags = lsp.intrinfo.p[i].flags;
		old_intrinfo[i].id.genflags = lsp.intrinfo.p[i].id.genflags;
		old_intrinfo[i].id.size = lsp.intrinfo.p[i].id.size;
		old_intrinfo[i].id.rtn32 = (uintptr_t)lsp.intrinfo.p[i].id.rtn;
		old_intrinfo[i].eoi.genflags = lsp.intrinfo.p[i].eoi.genflags;
		old_intrinfo[i].eoi.size = lsp.intrinfo.p[i].eoi.size;
		old_intrinfo[i].eoi.rtn32 = (uintptr_t)lsp.intrinfo.p[i].eoi.rtn;
		old_intrinfo[i].mask32 = (uintptr_t)lsp.intrinfo.p[i].mask;
		old_intrinfo[i].unmask32 = (uintptr_t)lsp.intrinfo.p[i].unmask;
		old_intrinfo[i].config32 = (uintptr_t)lsp.intrinfo.p[i].config;
	}
}

void
write_syspage_memory() {
	unsigned	total_size = lsp.syspage.p->total_size;

	memmove(_syspage_ptr, lsp.syspage.p, total_size);
	cpu_write_syspage_memory(syspage_paddr, total_size, callouts_size);
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/startup/lib/syspage_memory.c $ $Rev: 911753 $")
#endif
