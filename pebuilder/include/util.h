#pragma once

#include <stdint.h>

#define DECL_REG(name, addr) __attribute__ ((unused)) static volatile uint32_t *name = (void*)addr
