#include <kheap.h>

static uint32_t kmalloc_int (uint32_t size, uint32_t align); 
static uint32_t kheap_malloc (uint32_t size);
static void fill (uint32_t begin, uint32_t size, uint8_t value);

uint8_t *bitset_base = (uint8_t*) BITSET_BASE;
uint8_t *bitset_limit = (uint8_t*) BITSET_LIMIT;
uint8_t *heap_base = (uint8_t*) HEAP_BASE;
uint32_t placement_address = 0x5c00;
uint32_t kheap_enable = 0;

uint32_t kmalloc_u (uint32_t sz)
{
  return kmalloc_int (sz, 0);
}

uint32_t kmalloc (uint32_t sz)
{
  return kmalloc_int (sz, 1);
}

void kheap_init ()
{
  kheap_enable = 1;
  memset ((uint8_t*)BITSET_LIMIT, 0 ,BITSET_BASE - BITSET_LIMIT);
}
 
static uint32_t kmalloc_int (uint32_t size, uint32_t align)
{
  if (!kheap_enable)
  {
    if (align == 1 && (placement_address & 0x00000fff))
    {
      placement_address &= 0xfffff000;
      placement_address += 0x1000;
    }
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
  }
  else
  {
    if (align && (size & 0x00000fff))
    {
      size += 0x1000;
      size &= 0xfffff000;
    }
    uint32_t address  = kheap_malloc (size);
    return address;
  }
}

static uint32_t kheap_malloc (uint32_t size)
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
        return (uint32_t)(heap_base - (begin - size) - size);
      }
    }
  }
  return 0;
}

static void fill (uint32_t begin, uint32_t size, uint8_t value)
{
  int i;
  for (i = begin; i < (size + begin); i++) 
  {
    uint32_t byte = i / 8;
    uint32_t offset = i % 8;

    if (value)
      *(bitset_base - byte) |= (1 << (7 - offset)); 
    else
      *(bitset_base - byte) &= ((1 << 8) - 1) - (1 << (7 - offset)); 
  }

}

void kfree (uint32_t size, uint32_t addr)
{
  uint32_t byte = (uint32_t) HEAP_BASE - addr;
  fill (byte, size, 0);
}

void print_bit_map (uint32_t size)
{
  size = (size / 8) + (size % 8 != 0);
  uint8_t *i = bitset_limit;
  for (i = bitset_base ; i > (bitset_base - size); i --)

  {
    int8_t j;
    for (j = 7 ; j >= 0 ; j--)
    {
      if ((*i >> j) & 1)
        kprint_debug ("1", CYAN);
      else
        kprint_debug ("0", CYAN);
    }
  }
}
