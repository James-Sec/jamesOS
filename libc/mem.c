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

uint16_t flip_short (uint16_t value)
{
    uint32_t first_byte = *((uint8_t*)(&value));
    uint32_t second_byte = *((uint8_t*)(&value) + 1);
    return (first_byte << 8) | (second_byte);
}

uint32_t flip_long (uint32_t value)
{
    uint32_t first_byte = *((uint8_t*)(&value));
    uint32_t second_byte = *((uint8_t*)(&value) + 1);
    uint32_t third_byte = *((uint8_t*)(&value)  + 2);
    uint32_t fourth_byte = *((uint8_t*)(&value) + 3);
    return (first_byte << 24) | (second_byte << 16) | (third_byte << 8) | (fourth_byte);
}

//flip_byte -> its necessary when there are two fields in a bytes

uint16_t htons (uint16_t host_value)
{
    return flip_short(host_value);
}

uint32_t htonl (uint32_t host_value)
{
    return flip_long (host_value);
}

uint16_t ntohs (uint16_t network_value)
{
    return flip_short (network_value);
}

uint32_t ntohl (uint32_t network_value)
{
    return flip_long (network_value);
}
