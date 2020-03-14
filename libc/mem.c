#include "../include/mem.h"
void memcpy (char *source, char *dest, int nbytes) 
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
