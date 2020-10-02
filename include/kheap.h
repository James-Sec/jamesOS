#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>
#include "mem.h"
#include "string.h"
#include "vga.h"

#define BITSET_BASE 0x10c800
#define BITSET_LIMIT 0x100000

#define HEAP_BASE 0x171000

void kheap_init ();
void kfree (uint32_t size, uint32_t addr);
uint32_t kmalloc (uint32_t sz);
uint32_t kmalloc_a (uint32_t sz);

void print_bit_map (uint32_t size) ;

#endif
