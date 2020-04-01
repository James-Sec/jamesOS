#ifndef KMALLOC_H
#define KMALLOC_H
#include "types.h"
#include "mem.h"
#include "string.h"
#include "screen.h"

#define BITSET_BASE 0x10c800
#define BITSET_LIMIT 0x100000

#define HEAP_BASE 0x171000


void kheap_init ();
uint32_t kheap_malloc (uint32_t size);
void fill (uint32_t begin, uint32_t size, uint8_t value);
void kfree (uint32_t size, uint32_t addr);
uint32_t kmalloc_int (uint32_t size, uint32_t aling, uint32_t *phys);
uint32_t kmalloc_a (uint32_t sz);
uint32_t kmalloc_p (uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap (uint32_t sz, uint32_t *phys);
uint32_t kmalloc (uint32_t sz);

void print_bit_map (uint32_t size) ;

#endif
