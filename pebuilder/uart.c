#include <stdint.h>
#include <stdarg.h>

#include "uart.h"

static struct uart_regs *regs;

void uart_init(void *reg_base)
{
	regs = reg_base;
	
	/*
	 baud = 14.7456MHz/(16 * divisor)
	*/
	
	regs->FFLCR = 0; // DLAB = 0
	regs->DLAB0.FFIER = 0; // disable interrupts
	
	regs->FFLCR = 1<<7 ; // DLAB = 1
	regs->DLAB1.FFDLL = 8; // freq div low (115200 baud)
	regs->DLAB1.FFDLH = 0; // freq div hi
	
	regs->FFLCR = 0; // DLAB=0
	regs->FFLCR = 0b11; // WLS=0b11 (8-bit words)
	regs->FFFCR = 0b111; // clear and enable fifos
	regs->DLAB0.FFIER = 0; // disable interrupts (again?)
	regs->FFMCR = 0; // set rts/dtr
	
	regs->DLAB0.FFIER |= 0x40; // enable uart
	
	// set gpio alternate mode
	*GAFR1_L &=~ (0b11 << 14);
	*GAFR1_L |= 0b10 << 14;
}

void uart_putchar(const char c)
{
	if (c == '\n') uart_putchar('\r');
	while ((regs->FFLSR & 0x40) == 0); // TEMT
	regs->DLAB0.FFTHR = c;
}

void uart_puts(const char* str)
{
	while (*str) uart_putchar(*str++);
}

void uart_printf(const char* fmt, ...)
{
	va_list args;
	uint32_t arg32;
	char * argstr;
	char hexmap[] = "0123456789abcdef";
	
	va_start(args, fmt);
	
	while (*fmt) { if (*fmt == '%') {
		switch(*++fmt) {
		case 'p':
			uart_puts("0x");
			/* fallthru */
		case 'x':
			arg32 = va_arg(args, uint32_t);
			for (int i = 32-4; i>=0; i-= 4) {
				uart_putchar(hexmap[(arg32>>i) & 0xF]);
			}
			break;
		case 's':
			argstr = va_arg(args, char *);
			uart_puts(argstr);
			break;
		case '%':
			uart_putchar('%');
			break;
		default:
			uart_putchar('%');
			uart_putchar(*fmt);
		}
		fmt++;
	} else {
		uart_putchar(*fmt++);
	}}
	
	va_end(args);
}
