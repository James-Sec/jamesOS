#include "../include/mem.h"
extern uint32_t esp;
void memcpy (uint8_t *source, uint8_t *dest, uint32_t nbytes)
{
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

void memset (uint8_t *dest, uint8_t val, uint32_t len)
{
  for (;len != 0; len--) 
    *dest++ = val;
}

void memmov (uint8_t *source, uint8_t *dest, uint32_t len)
{
  if (source == dest)
    return;
  if (source < dest)
  {
    source += len;
    dest += len;
    uint32_t i;
    for (i = 0; i < len; i++)
      *(--dest) = *(--source);
  }
  else
  {
    uint32_t i;
    for (i = 0; i < len; i++)
      *(dest++) = *(source++);
  }
}

int8_t memcmp (uint8_t *source, uint8_t *targ, uint32_t len)
{
  uint32_t i;
  for (i = 0; i < len; i++)
  {
    if (*source < *targ)
      return -1;
    else if (*(source++) > *(targ++))
      return 1;
  }
  return 0;
}

void print_stack_c (uint32_t address, uint32_t value)
{
  kprintf ("stack->%x: %x\n", 2,address, value);
}

void print_single_value (uint32_t value) {
  kprintf ("single value: %x\n", 1, value);
}
