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

void print_esp_eip_c (uint32_t esp, uint32_t eip)
{
  kprintf ("eip: %d (%x)\nesp: %d (%x)\n", 4, eip, eip, esp, esp);
}

void print_stack_c (uint32_t address, uint32_t value)
{
  kprintf ("stack->%x: %x\n", 2,address, value);
}

void test_c (uint32_t v, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g)
{
  print_stack_asm (17, 0x1c);
  while (1);
}
void print (uint32_t v)
{
  kprintf ("print: %x\n", 1, v);
}
