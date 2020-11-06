#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>

void htonb (uint8_t* byte, uint8_t bits);
void ntohb (uint8_t* byte, uint8_t bits);

void htons (uint16_t* shrt);
void ntohs (uint16_t* shrt);

void htonl (uint32_t* lng);
void ntohl (uint32_t* lng);

#endif
