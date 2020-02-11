#ifndef KMALLOC_H
#define KMALLOC_H
#include "../cpu/types.h"
#include "mem.h"
#include <stdint.h>
#include "../libc/string.h"
#include "../drivers/screen.h"

#define BITSET_BASE 0x10c800
#define BITSET_LIMIT 0x100000

#define MEMORY_MAPPED_BASE 0x170800


void memory_manager_init ();
uint8_t* kmalloc (uint32_t size);
void kfree (uint32_t size, uint32_t addr);
void fill (uint32_t begin, uint32_t size, uint8_t value);



#endif
