#ifndef UDP_H
#define UDP_H

#define UDP_HEADER_SIZE 8

#define UDP_SOURCE_PORT_OFFSET 0
#define UDP_DESTINATION_PORT_OFFSET 2
#define UDP_LENGTH_OFFSET 4
#define UDP_CHECKSUM_OFFSET 6

#define UDP_SOURCE_PORT_SIZE 2
#define UDP_DESTINATION_PORT_SIZE 2
#define UDP_LENGTH_SIZE 2
#define UDP_CHECKSUM_SIZE 2

#include <stdint.h>
#include <network_utils.h>
#include <l3_interface.h>
#include <ip.h>

struct udp_segment
{
  uint8_t header[UDP_HEADER_SIZE];
  uint8_t *data;
};

struct udp_segment* build_udp_segment (struct udp_segment *udp, uint16_t source_port, uint16_t destination_port, uint16_t length, uint8_t* data, uint32_t data_size);
void send_udp_segment (uint16_t source_port, uint16_t destination_port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size);

void recv_udp_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size);

struct udp_segment* array_to_udp (struct udp_segment *segment, uint8_t *array, uint32_t size);
uint8_t* udp_to_array (struct udp_segment*, uint32_t data_size);


#endif
