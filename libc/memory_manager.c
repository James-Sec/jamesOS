#include "memory_manager.h"

uint8_t *bitset_base = (uint8_t*) BITSET_BASE;
uint8_t *bitset_limit = (uint8_t*) BITSET_LIMIT;
uint8_t *memory_mapped_base = (uint8_t*) MEMORY_MAPPED_BASE;

void memory_manager_init ()
{
  memset (bitset_limit, 0 ,bitset_base - bitset_limit);
}

//begin e size sao em bits
void fill (uint32_t begin, uint32_t size, uint8_t value)
{
  char str [10];

  itoa (begin, str);
  kprint ("(");
  kprint (str);
  kprint (")");

  itoa (size, str);
  kprint ("(");
  kprint (str);
  kprint (")");




  int i;
  for (i = begin; i < (size + begin); i++) {
    uint32_t byte = i / 8;
    uint32_t offset = i % 8;

    if (value)
      *(bitset_base - byte) |= (1 << (7 - offset)); 
    else
      *(bitset_base - byte) &= ((1 << 8) - 1) - (1 << (7 - offset)); 
  }

}

uint8_t* kmalloc (uint32_t size)
{
  uint8_t *i;
  int count = 0;
  int begin = 0;
  for (i = bitset_base; i > bitset_limit ; i--)
  {
    int j;
    for (j = 7; j >= 0; j--)
    {
      begin++;
      if ((*i >> j) & 1)
        count = 0;
      else
        count++;
      if (count == size)
      {
        fill (begin - size, size, 1);
        return memory_mapped_base - (begin - size);
      }
    }
  }
  return 0;
}

void kfree (uint32_t size, uint32_t addr)
{
  uint32_t byte = memory_mapped_base - addr;
  fill (byte, size, 0);

}

