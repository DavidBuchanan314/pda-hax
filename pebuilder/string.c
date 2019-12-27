#include <stddef.h>

#include "string.h"

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	for (size_t i=0; i<n; i++) d[i] = s[i];
	return dest;
}

void memswap(void *a, void *b, size_t n)
{
	char *ac = a;
	char *bc = b;
	char tmp;
	for (size_t i=0; i<n; i++) {
		tmp = ac[i];
		ac[i] = bc[i];
		bc[i] = tmp;
	}
}
