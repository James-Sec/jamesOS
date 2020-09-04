#ifndef MEM_H
#define MEM_H

#include <stdint.h>

void memcpy (uint8_t *source, uint8_t *dest, uint32_t nbytes);
void memset (uint8_t *dest, uint8_t val, uint32_t len);
void memmov (uint8_t *source, uint8_t *dest, uint32_t len);
int8_t memcmp (uint8_t *source, uint8_t *targ, uint32_t len);
void print_esp_eip_c (uint32_t esp, uint32_t eip);
void print_stack_c (uint32_t address, uint32_t value);

#endif
