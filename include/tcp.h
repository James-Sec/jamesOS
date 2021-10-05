#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <network_utils.h>

#define TCP_HEADER_MIN_SIZE 20
#define TCP_HEADER_MAX_SIZE 64
#define TCP_OPTIONS_OFFSET 20 // BYTE_LIKE

#define TCP_SOURCE_PORT_OFFSET 0
#define TCP_DESTINATION_PORT_OFFSET 16
#define TCP_SEQUENCE_NUMBER_OFFSET 32
#define TCP_ACK_NUMBER_OFFSET 64
#define TCP_DATA_OFFSET_OFFSET 96
#define TCP_RESERVED_OFFSET 100
#define TCP_ECN_OFFSET 103
#define TCP_CWR_OFFSET 104
#define TCP_ECE_OFFSET 105
#define TCP_URG_OFFSET 106
#define TCP_ACK_OFFSET 107
#define TCP_PSH_OFFSET 108
#define TCP_RST_OFFSET 109
#define TCP_SYN_OFFSET 110
#define TCP_FIN_OFFSET 111
#define TCP_WINDOW_SIZE_OFFSET 112
#define TCP_CHECKSUM_OFFSET 128
#define TCP_URGENT_POINTER_OFFSET 144

#define TCP_SOURCE_PORT_SIZE 16
#define TCP_DESTINATION_PORT_SIZE 16
#define TCP_SEQUENCE_NUMBER_SIZE 32
#define TCP_ACK_NUMBER_SIZE 32
#define TCP_DATA_OFFSET_SIZE 4
#define TCP_RESERVED_SIZE 3
#define TCP_ECN_SIZE 1
#define TCP_CWR_SIZE 1
#define TCP_ECE_SIZE 1
#define TCP_URG_SIZE 1
#define TCP_ACK_SIZE 1
#define TCP_PSH_SIZE 1
#define TCP_RST_SIZE 1
#define TCP_SYN_SIZE 1
#define TCP_FIN_SIZE 1
#define TCP_WINDOW_SIZE_SIZE 16
#define TCP_CHECKSUM_SIZE 16
#define TCP_URGENT_POINTER_SIZE 16

struct tcp_segment
{
  uint8_t header[TCP_HEADER_MAX_SIZE];
  uint8_t *data;
};

struct tcp_port_table_entry
{
  uint32_t pid;
  uint8_t* data;
  struct net_address_set net_addresses;
};

struct tcp_header_bit_field
{
  uint32_t source_port : 16;
  uint32_t destination_port : 16;
  uint32_t sequence_number : 32;
  uint32_t ack_number : 32;
  uint32_t data_offset : 4;
  uint32_t reserved : 3;
  uint32_t ecn : 1;
  uint32_t cwr : 1;
  uint32_t ece : 1;
  uint32_t urg : 1;
  uint32_t ack : 1;
  uint32_t psh : 1;
  uint32_t rst : 1;
  uint32_t syn : 1;
  uint32_t fin : 1;
  uint32_t window_size : 16;
  uint32_t checksum : 16;
  uint32_t urgent_pointer : 16;
};

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, struct tcp_header_bit_field *header, uint8_t* options, uint8_t* data, uint32_t data_size);

void tcp_recv_segment();
void tcp_send_segment (struct tcp_header_bit_field *tcp_header, uint32_t ip, uint8_t mac[6], uint8_t *options, uint8_t *data, uint32_t data_size);
struct tcp_segment* array_to_tcp (struct tcp_segment *segment, uint8_t *array, uint32_t size);
uint8_t* tcp_to_array (struct tcp_segment *tcp, uint32_t data_size);

#endif
