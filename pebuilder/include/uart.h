#pragma once

#include "util.h"

struct uart_regs {
	union{
		struct {
			volatile uint32_t FFTHR;
			volatile uint32_t FFIER;
		} DLAB0;
		struct {
			volatile uint32_t FFDLL;
			volatile uint32_t FFDLH;
		} DLAB1;
	};
	volatile uint32_t FFFCR;
	volatile uint32_t FFLCR;
	volatile uint32_t FFMCR;
	volatile uint32_t FFLSR;
};

DECL_REG( GAFR1_L, 0xace0005c ); // XXX put in gpio.h

void uart_init(void *reg_base);
void uart_putchar(const char c);
void uart_puts(const char* str);
void uart_printf(const char* format, ...);
