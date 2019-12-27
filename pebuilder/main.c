#include <stdint.h>
#include <arm_acle.h>

#include "uart.h"
#include "mmu.h"
#include "string.h"

extern void *_start, *zimage, *end_zimage;

static char* zImage_vmem_base;
static char* zImage_cmem_base; // c stands for contiguous
static uint32_t zImage_size;

static uint32_t pagemap[0x4000]; // enough for all 64MB of ram

const uint32_t stage2_off = 0x86000000;
const uint32_t sdram_base = 0xA0000000;

void reloc_main(void);

/* Stop all the things */
void deinit_hw(void)
{
	return;
}

/*

Problem: the Linux zImage needs to be called with the MMU disabled.
We're currently loaded in virtual memory which is not necessarily mapped
to contiguous physical memory.

Solution:
- Make a table of where all our pages are in physical memory
- move any blocks out of the way of where our code needs relocating to
- copy our code there
- jump to relocated code
- relocate zimage



Disable all interrupts and DMA devices

Identity map SDRAM

relocate image to contiguous SDRAM, careful not to clobber own code in the process

disable MMU and caches

jump to zImage
*/


int main()
{
	uart_init((void*)0xac100000);

	uart_puts("\n\n******** kexec.exe - main() ********\n\n");

	deinit_hw(); // we're about to clobber kernel memory, kill as much as we can

	zImage_vmem_base = (char*)&zimage;
	zImage_cmem_base = zImage_vmem_base + stage2_off;
	zImage_size = (uint32_t)&end_zimage - (uint32_t)&zimage;

	uart_printf("checksumming...\n");

	uint32_t checksum = 0;
	for (unsigned char *ptr=(unsigned char*)&_start; ptr<(unsigned char*)&end_zimage; ptr++) {
		checksum += *ptr;
	}
	uart_printf("checksum: %p\n", checksum);

	for (uint32_t vaddr=(uint32_t)&_start; vaddr < (uint32_t)&end_zimage; vaddr += 0x1000) {
		uint32_t sdram_index = (uint32_t)mmu_virt2phys((void*)vaddr)-sdram_base;
		uint32_t pagemap_index = sdram_index/0x1000;
		if (pagemap_index > sizeof(pagemap)/sizeof(pagemap[0])) {
			uart_printf("pagemap index out of range??? %p -> %p\n", vaddr, mmu_virt2phys((void*)vaddr));
			for(;;);
		} else {
			uart_printf("pagemap[%p] = %p\n", pagemap_index, vaddr);
			pagemap[pagemap_index] = vaddr;
		}
	}

	uart_printf("done!\n");

	for(;;);

	// we relocate our own code to the start of the top half of sdram
	// everything is compiled PIC/PIE so it shouldn't cause any issues
	// XXX: assumption: this memcpy does not clobber our code or zimage
	memcpy((void*)((uint32_t)&_start+stage2_off), &_start, (uint32_t)&zimage-(uint32_t)&_start);
	void (*stage2)(void) = (void*)((uint32_t)reloc_main+stage2_off);
	stage2();

	return 0;
}

void reloc_main(void)
{
	uart_puts("\n\n******** kexec.exe - reloc_main() ********\n\n");

	uart_printf("reloc_main: %p => %p\n", reloc_main, mmu_virt2phys(reloc_main));

	uint32_t pid = __arm_mrc(15, 0, 13, 0, 0) >> 25;
	uart_printf("pid: %p\n", pid);

	uart_puts("MMU state:\n");
	//for (uint32_t i=0; i<0x1000; i++) mmu_virt2phys((void*)(i<<20));
	//uart_printf("test: %p\n", mmu_virt2phys((void*)0x84300000));
	//mmu_dumptables();
	uart_puts("done!\n");
	for (;;); // TODO: jump into zImage
}
