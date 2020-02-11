#ifndef MEM_H
#define MEM_H
#include "../cpu/types.h"
#include <stdint.h>
void memcpy (char *source, char *dest, int nbytes) ;
void memset (uint8_t *dest, uint8_t val, uint32_t len);
#endif
