#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>
#include <ethernet.h>
#include <icmp.h>

#define PSEUDO_HEADER_SIZE 12

#define PSEUDO_SOURCE_IP_OFFSET 0
#define PSEUDO_DESTINATION_IP_OFFSET 32
#define PSEUDO_FIXED_OFFSET 64
#define PSEUDO_PROTOCOL_OFFSET 72
#define PSEUDO_SEGMENT_LENGTH_OFFSET 80

#define PSEUDO_SOURCE_IP_SIZE 32
#define PSEUDO_DESTINATION_IP_SIZE 32
#define PSEUDO_FIXED_SIZE 8
#define PSEUDO_PROTOCOL_SIZE 8
#define PSEUDO_SEGMENT_LENGTH_SIZE 16

struct net_address_set
{
  uint8_t mac[6];
  uint32_t ip;
  uint16_t port;
};

struct pseudo_ip
{
  uint8_t header[PSEUDO_HEADER_SIZE];
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

uint16_t internet_checksum (uint8_t* header, uint32_t header_size, uint8_t *data, uint32_t data_size, uint8_t *pseudo, uint32_t pseudo_size);

void pseudo_header_build (uint32_t source_ip, uint32_t destination_ip, uint8_t fixed, uint8_t protocol, uint16_t length, struct pseudo_ip* pseudo);

#endif
