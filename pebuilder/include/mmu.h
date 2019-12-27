#pragma once

#include "util.h"

void *mmu_wince_phys2virt(void *phys);
int mmu_is_enabled(void);
void *mmu_virt2phys(void *vaddr);
void mmu_dumptables(void);
void mmu_sectionmap(uint32_t vaddr, uint32_t paddr, uint32_t pagecount, uint32_t flags);
