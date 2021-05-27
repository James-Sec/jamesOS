#include <kheap.h>

static uint32_t _kmalloc (uint32_t size, uint32_t align); 
static void fill (uint32_t end, uint32_t size, uint8_t value);

uint8_t *bitset_base = (uint8_t*) BITSET_BASE;
uint8_t *bitset_limit = (uint8_t*) BITSET_LIMIT;
uint8_t *heap_base = (uint8_t*) HEAP_BASE;
uint32_t kheap_enable = 0;

uint32_t kmalloc_u (uint32_t sz)
{
  return _kmalloc (sz, 0);
}

uint32_t kmalloc (uint32_t sz)
{
  return _kmalloc (sz, 1);
}

void kheap_init ()
{
  kheap_enable = 1;
  memset ((uint8_t*)bitset_base, 0 ,bitset_limit - bitset_base);
}
 
static uint32_t _kmalloc (uint32_t size, uint32_t align)
{
  uint8_t *i;
  uint32_t count = 0;
  uint32_t end = 0;
  for (i = bitset_base; i < bitset_limit ; i++)
  {
    uint32_t num_bits_from_base = (((uint32_t)i) - BITSET_BASE) * 8;
    if (align && !count && (num_bits_from_base % 0x1000) && num_bits_from_base){
      end += 8;
      continue;
    }
    uint32_t j;
    for (j = 0; j < 8; j++)
    {
      end++;
      if ((*i >> j) & 1)
        count = 0;
      else
        count++;
      if (count == size)
      {
        fill (end - size, size, 1);
        return (uint32_t)((((((uint32_t)i) - BITSET_BASE) * 8) + (j + 1)) + HEAP_BASE) - size;
      }
    }
  }
  return 0;
}

static void fill (uint32_t begin, uint32_t size, uint8_t value)
{
  uint32_t i;
  for (i = begin; i < begin + size; i++) 
  {
    uint32_t byte = i / 8;
    uint32_t offset = i % 8;

    if (value)
      *(bitset_base + byte) |= (1 << (offset)); 
    else
      *(bitset_base + byte) &= ~(1 << (offset)); 
  }

}

void kfree (uint32_t addr, uint32_t size)
{
  uint32_t byte = (uint32_t)addr;
  uint32_t tmp = (byte - HEAP_BASE);
  fill (tmp, size, 0);
}

void print_bit_map (uint32_t size)
{
  size = (size / 8) + (size % 8 != 0);
  uint8_t *i = bitset_limit;
  for (i = bitset_base ; i < (bitset_base + size); i++)
  {
    int8_t j;
    for (j = 0; j < 8; j++)
    {
      if ((*i >> j) & 1)
        kprint_debug ("1", CYAN);
      else
        kprint_debug ("0", CYAN);
    }
  }
}
