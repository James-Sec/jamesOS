#ifndef MEM_H
#define MEM_H
#include "types.h"

void memcpy (char *source, char *dest, int nbytes) ;
void memset (uint8_t *dest, uint8_t val, uint32_t len);


uint16_t htons (uint16_t host_value);
uint32_t htonl (uint32_t host_value);
uint16_t ntohs (uint16_t network_value);
uint32_t ntohl (uint32_t network_value);
#endif
