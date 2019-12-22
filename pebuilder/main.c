#include <stdint.h>
//#include <arm_acle.h>

extern char zimage;

static char hello2[] = "Hello, world!2\r\n";

void puts(const char* str) {
	volatile uint32_t *FFTHR = (void*)0xac100000; // DLAB = 0
	volatile uint32_t *FFLSR = (void*)0xac100014;
	while (*str) {
		while ((*FFLSR & 0x40) == 0); // TEMT
		*FFTHR = *str++;
	}
}

int main()
{

	volatile uint32_t *FFTHR = (void*)0xac100000; // DLAB = 0
	volatile uint32_t *FFDLL = (void*)0xac100000; // DLAB = 1
	volatile uint32_t *FFIER = (void*)0xac100004; // DLAB = 0
	volatile uint32_t *FFDLH = (void*)0xac100004; // DLAB = 1
	volatile uint32_t *FFFCR = (void*)0xac100008;
	volatile uint32_t *FFLCR = (void*)0xac10000c;
	volatile uint32_t *FFMCR = (void*)0xac100010;
	volatile uint32_t *FFLSR = (void*)0xac100014;
	/*
	 baud = 14.7456MHz/(16 * divisor)
	*/
	
	
	*FFLCR = 0; // DLAB = 0
	*FFIER = 0; // disable interrupts
	
	*FFLCR = 1<<7 ; // DLAB = 1
	*FFDLL = 8; // freq div low (115200 baud)
	*FFDLH = 0; // freq div hi
	
	*FFLCR = 0; // DLAB=0
	*FFLCR = 0b11; // WLS=0b11 (8-bit words)
	*FFFCR = 0b111; // clear and enable fifos
	*FFIER = 0; // disable interrupts (again?)
	*FFMCR = 0; // set rts/dtr
	
	*FFIER |= 0x40; // enable uart
	
	uint32_t *foo = (void*)0xace0005c;
	*foo &=~ 0b11 << 14;
	*foo |= 0b10 << 14;

	puts("Hello, world!\r\n");
	puts(hello2);
	puts(&zimage);
	return 0;
}
