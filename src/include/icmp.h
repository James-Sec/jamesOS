#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <ip.h>
#include <mem.h>
#include <network_utils.h>

#define ICMP4_ECHO_REPLY_TYPE 0
#define ICMP4_ECHO_REQUEST_TYPE 8
#define ICMP4_ECHO_REPLY_CODE 0
#define ICMP4_ECHO_REQUEST_CODE 0

#define ICMP4_TYPE_SIZE 1
#define ICMP4_CODE_SIZE 1
#define ICMP4_CHECKSUM_SIZE 2
#define ICMP4_REST_OF_HEADER_SIZE 4

#define ICMP4_TYPE_OFFSET 0
#define ICMP4_CODE_OFFSET 1
#define ICMP4_CHECKSUM_OFFSET 2
#define ICMP4_REST_OF_HEADER_OFFSET 4

#define ICMP4_HEADER_SIZE 8

struct icmp4 {
  uint8_t header[ICMP4_HEADER_SIZE];
  uint8_t* data;
};

struct icmp4* build_icmp4_packet (struct icmp4* icmp, uint8_t type, uint8_t code, uint32_t rest_of_header);
void send_icmp4_packet (uint32_t ip, uint8_t mac[6], uint8_t type, uint8_t code, uint32_t rest_of_header, uint8_t *data, uint32_t data_size);
uint8_t* icmp4_to_array (struct icmp4 *icmp, uint32_t data_size);
struct icmp4* array_to_icmp4 (struct icmp4* icmp, uint8_t* array, uint32_t size);

#endif
