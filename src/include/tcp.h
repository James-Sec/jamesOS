#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <network_utils.h>
#include <bitmap.h>
#include <minmax.h>

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

#define TCP_TOTAL_PORTS 65535

#define TCP_EPHEMERAL_PORT_BEGIN 32768
#define TCP_EPHEMERAL_PORT_END 60999

#define TCP_STATE_WAITING_THREEWAY_SYN 0
#define TCP_STATE_WAITING_THREEWAY_SYN_ACK 1
#define TCP_STATE_WAITING_THREEWAY_ACK 2
#define TCP_STATE_CONNECTED 3
#define TCP_STATE_FIN_WAIT_1 4
#define TCP_STATE_FIN_WAIT_2 5
#define TCP_STATE_CLOSING 6
#define TCP_STATE_TIME_WAIT 7
#define TCP_STATE_CLOSE_WAIT 8
#define TCP_STATE_LAST_ACK 9
#define TCP_STATE_CLOSED 10

#define TCP_SLIDING_WINDOW_BITMAP_SIZE 128
#define TCP_SLIDING_WINDOW_SIZE 1024

struct tcp_segment
{
  uint8_t header[TCP_HEADER_MAX_SIZE];
  uint8_t *data;
};

struct tcp_send_sliding_window
{
  uint32_t last_acked_byte;
  uint32_t window_size;
};

struct tcp_recv_sliding_window
{
  uint32_t last_acked_byte;
  uint32_t first_window_byte;
  uint8_t bitmap[TCP_SLIDING_WINDOW_BITMAP_SIZE];
  uint8_t buffer[TCP_SLIDING_WINDOW_SIZE];
};

struct tcp_port_table_entry
{
  uint32_t pid;
  uint8_t state;
  uint32_t current_byte_tx;
  uint8_t connection_stablished_or_reseted;

  struct net_address_set net_addresses;

  struct tcp_recv_sliding_window *recv_window;
  struct tcp_send_sliding_window *send_window;
};

struct tcp_port_table_entry tcp_port_table [TCP_TOTAL_PORTS];
//struct tcp_port_table_entry *tcp_port_table;

struct tcp_flags
{
  uint32_t ecn : 1;
  uint32_t cwr : 1;
  uint32_t ece : 1;
  uint32_t urg : 1;
  uint32_t ack : 1;
  uint32_t psh : 1;
  uint32_t rst : 1;
  uint32_t syn : 1;
  uint32_t fin : 1;
};

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, uint16_t source_port, uint16_t destination_port, uint32_t sequence_number, uint32_t ack_number, uint8_t data_offset, uint8_t reserved, uint8_t ecn, uint8_t cwr, uint8_t ece, uint8_t urg, uint8_t ack, uint8_t psh, uint8_t rst, uint8_t syn, uint8_t fin, uint16_t window_size, uint16_t urgent_pointer, uint8_t* options, uint8_t* data, uint32_t data_size, uint32_t ip);


uint32_t tcp_connect (uint16_t src_port, uint16_t dest_port, uint32_t ip, uint8_t mac[6]);

int32_t tcp_bind (uint16_t port);
uint32_t tcp_read (uint16_t port, uint8_t *buffer, uint32_t size);

void tcp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size);
void tcp_send_segment (struct tcp_segment *segment,uint32_t data_size, uint32_t ip, uint8_t mac[6]);

struct tcp_segment* array_to_tcp (struct tcp_segment *segment, uint8_t *array, uint32_t size);
uint8_t* tcp_to_array (struct tcp_segment *tcp, uint32_t data_size);

struct tcp_flags* tcp_get_flags (struct tcp_segment *segment);
uint8_t tcp_recv_threeway_syn (struct tcp_segment *segment);
uint8_t tcp_recv_threeway_syn_ack (struct tcp_segment *segment);
uint8_t tcp_recv_threeway_ack (struct tcp_segment *segment);
uint8_t tcp_recv_psh_ack (struct tcp_segment *segment);
uint8_t tcp_recv_fin_ack (struct tcp_segment *segment);

uint8_t tcp_state_threeway_syn_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size);
uint8_t tcp_state_threeway_syn_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size);
uint8_t tcp_state_threeway_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size);
uint8_t tcp_state_psh_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint32_t sequence_number, uint8_t* data, uint32_t data_size);
uint8_t tcp_state_fin_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment);

uint8_t tcp_send_syn_ack(uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size);
uint8_t tcp_send_ack(uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size, uint32_t ack_number);
uint8_t tcp_send_fin_ack (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size);

#endif
