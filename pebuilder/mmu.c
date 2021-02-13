#include <stddef.h>
#include <arm_acle.h>

#include "mmu.h"
#include "uart.h"

/* not ideal... */
#define PTR_INVALID ((void*)-1)

enum l1_type {
	L1_TYPE_UNMAPPED  = 0b00,
	L1_TYPE_COARSE_L2 = 0b01,
	L1_TYPE_SECTION   = 0b10,
	L1_TYPE_FINE_L2   = 0b11
};

enum l2_type {
	L2_TYPE_UNMAPPED  = 0b00,
	L2_TYPE_LARGE     = 0b01,
	L2_TYPE_SMALL     = 0b10,
	L2_TYPE_TINY      = 0b11
};

static int mmu_enabled_flag = 1;

void *mmu_wince_phys2virt(void *phys)
{
	/* Assumes Dell Axim X3 WinCE memory map */
	uint32_t p = (uint32_t)phys;

	// sdram
	if (p >= 0xA0000000 && p < 0xA8000000) return (void*)(p - 0x1c000000);

	return PTR_INVALID;
}

int mmu_is_enabled(void)
{
	return mmu_enabled_flag;
}

void *mmu_virt2phys(void *vaddr)
{
	uint32_t vaddri = (uint32_t)vaddr;

	uint32_t TTBR = __arm_mrc(15, 0, 2, 0, 0);
	uint32_t *l1 = mmu_wince_phys2virt((void*)(TTBR&0xffffc000));

	uint32_t pid = __arm_mrc(15, 0, 13, 0, 0) >> 25;

	//art_printf("pid: %p\n", pid);

	if (vaddri < 0x02000000) {
		vaddri += 0x02000000 * pid;
	}

	//uart_printf("vaddri: %p\n", vaddri);

	uint32_t l1_desc = l1[vaddri>>20];

	//uart_printf("l1_desc: %p\n", l1_desc);

	uint32_t *l2;
	uint32_t l2_desc;

	switch (l1_desc & 0b11) {
	case L1_TYPE_UNMAPPED:
		return PTR_INVALID;
	case L1_TYPE_COARSE_L2:
		l2 = mmu_wince_phys2virt((void*)(l1_desc&0xfffffc00));
		l2_desc = l2[(vaddri>>12)&0x000000ff];
		//uart_printf("l2: %p[%p] -> %p\n", l2, (vaddri>>12)&0x000000ff, l2_desc);
		switch (l2_desc & 0b11) {
		case L2_TYPE_UNMAPPED:
			return PTR_INVALID;
		case L2_TYPE_LARGE:
			uart_printf("L2_TYPE_LARGE unimplemented\n");
			break;
		case L2_TYPE_SMALL:
			return (void*)((l2_desc&0xfffff000) | (vaddri&0x00000fff));
			break;
		case L2_TYPE_TINY:
			uart_printf("L2_TYPE_TINY unimplemented\n");
			break;
		}
		break;
	case L1_TYPE_SECTION:
		return (void*)((l1_desc&0xfff00000) | (vaddri&0x000fffff));
	case L1_TYPE_FINE_L2:
		uart_printf("L1_TYPE_FINE_L2 unimplemented\n");
		break;
	}

	// we shouldn't get here
	return PTR_INVALID;
}

void mmu_dumptables(void)
{
	uint32_t mapping_pstart = (uint32_t)mmu_virt2phys(0);
	uint32_t mapping_size = 0x1000;
	for (uint32_t vaddr=0x1000; vaddr; vaddr += 0x1000) { // iterate 4k pages
		uint32_t this_paddr = (uint32_t)mmu_virt2phys((void*)vaddr);
		if (this_paddr == mapping_pstart+mapping_size) {
			mapping_size += 0x1000;
		} else {
			if (mapping_pstart != (uint32_t)PTR_INVALID)
				uart_printf("  %p - %p => %p\n", vaddr-mapping_size, vaddr-1, mapping_pstart);
			mapping_pstart = this_paddr;
			mapping_size = 0x1000;
		}
	}
	if (mapping_pstart != (uint32_t)PTR_INVALID)
		uart_printf("  %p - %p => %p\n", -mapping_size, 0xffffffff, mapping_pstart);
}

void mmu_sectionmap(uint32_t vaddr, uint32_t paddr, uint32_t pagecount, uint32_t flags)
{
	uint32_t TTBR = __arm_mrc(15, 0, 2, 0, 0);
	uint32_t *l1 = mmu_wince_phys2virt((void*)(TTBR&0xffffc000));
	
	for (uint32_t i=0; i<pagecount; i++) {
		l1[(vaddr>>20)+i] = ((paddr+(i<<20))&0xfff00000) | flags;
	}
	
	// TODO flush caches?
}
