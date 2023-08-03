#include <minmax.h>

uint32_t max (uint32_t i, uint32_t j)
{
  if (i > j)
    return i;
  return j;
}

uint32_t min (uint32_t i, uint32_t j)
{
  if (i < j)
    return i;
  return j;
}
