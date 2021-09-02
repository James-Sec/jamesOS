#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>
#include <ethernet.h>
#include <icmp.h>

struct net_address_set
{
  uint8_t mac[6];
  uint32_t ip;
  uint16_t port;
};

void htonb (uint8_t* byte, uint8_t bits);
void ntohb (uint8_t* byte, uint8_t bits);

void htons (uint16_t* shrt);
void ntohs (uint16_t* shrt);

void htonl (uint32_t* lng);
void ntohl (uint32_t* lng);

void set_bits_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint32_t value);
uint32_t get_bits_attr_value (uint8_t* attr, uint32_t offset, uint32_t size);

void get_bytes_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* ret);
void set_bytes_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* value);

uint16_t internet_checksum (uint8_t* header, uint32_t header_size, uint8_t *data, uint32_t data_size);

#endif
