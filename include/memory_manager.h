#ifndef KMALLOC_H
#define KMALLOC_H
#include "types.h"
#include "mem.h"
#include "string.h"
#include "screen.h"

#define BITSET_BASE 0x10c800
#define BITSET_LIMIT 0x100000

#define MEMORY_MAPPED_BASE 0x171000


void memory_manager_init ();
uint8_t* kmalloc (uint32_t size);
void kfree (uint32_t size, uint32_t addr);
void fill (uint32_t begin, uint32_t size, uint8_t value);
void print_bit_map (uint32_t size);



#endif
