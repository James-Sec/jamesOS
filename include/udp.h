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

#define UDP_TOTAL_PORTS 65535

#define UDP_EPHEMERAL_PORT_BEGIN 32768
#define UDP_EPHEMERAL_PORT_END 60999

#include <stdint.h>
#include <network_utils.h>
#include <l3_interface.h>
#include <ip.h>
#include <multitask.h>

struct udp_segment
{
  uint8_t header[UDP_HEADER_SIZE];
  uint8_t *data;
};

struct udp_port_table_entry
{
  uint32_t pid;
  uint8_t* data;
  struct net_address_set net_addresses;
};

struct udp_port_table_entry udp_port_table [UDP_TOTAL_PORTS];

struct udp_segment* build_udp_segment (struct udp_segment *udp, uint16_t source_port, uint16_t destination_port, uint16_t length, uint8_t* data, uint32_t data_size);
void udp_send_segment (uint16_t source_port, uint16_t destination_port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size);

void udp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size);

struct udp_segment* array_to_udp (struct udp_segment *segment, uint8_t *array, uint32_t size);
uint8_t* udp_to_array (struct udp_segment*, uint32_t data_size);

int32_t udp_port_bind (uint16_t port, uint8_t* data, struct net_address_set** address);
int32_t udp_port_unbind (uint16_t port);

void udp_forward_segment_to_process (uint16_t port, uint8_t* data, uint32_t data_size, uint16_t source_port, uint32_t source_ip, int8_t* source_mac);

void udp_receive ();

#endif
