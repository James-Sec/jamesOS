#ifndef IP_H
#define IP_H

#include <mem.h>
#include <network_utils.h>
#include <vga.h>
#include <ethernet.h>
#include <l2_interface.h>

#define IPv4_DATA_MAX_LENGTH 65515
#define IPv4_HEADER_SIZE 20
#define IPv6_HEADER_SIZE 60

#define IPv4_VERSION_OFFSET 0
#define IPv4_IHL_OFFSET 4
#define IPv4_DSCP_OFFSET 8
#define IPv4_ECN_OFFSET 14
#define IPv4_TOTAL_LENGTH_OFFSET 16
#define IPv4_IDENTIFICATION_OFFSET 32
#define IPv4_FLAGS_OFFSET 48
#define IPv4_FRAGMENT_OFFSET_OFFSET 51
#define IPv4_TIME_TO_LIVE_OFFSET 64
#define IPv4_PROTOCOL_OFFSET 72
#define IPv4_HEADER_CHECKSUM_OFFSET 80
#define IPv4_SOURCE_IP_ADDRESS_OFFSET 96
#define IPv4_DESTINATION_IP_ADDRESS_OFFSET 128

#define IPv4_VERSION_SIZE 4
#define IPv4_IHL_SIZE 4
#define IPv4_DSCP_SIZE 6
#define IPv4_ECN_SIZE 2
#define IPv4_TOTAL_LENGTH_SIZE 16
#define IPv4_IDENTIFICATION_SIZE 16
#define IPv4_FLAGS_SIZE 3
#define IPv4_FRAGMENT_OFFSET_SIZE 13
#define IPv4_TIME_TO_LIVE_SIZE 8
#define IPv4_PROTOCOL_SIZE 8
#define IPv4_HEADER_CHECKSUM_SIZE 16
#define IPv4_SOURCE_IP_ADDRESS_SIZE 32
#define IPv4_DESTINATION_IP_ADDRESS_SIZE 32

#define IPv4_PROTOCOL_ICMP4 1
#define IPv4_PROTOCOL_TCP 6
#define IPv4_PROTOCOL_UDP 17

#define IPv4_VERSION 4
#define IPv4_IHL 5
#define IPv4_DSCP_DF 0
#define IPv4_TTL 64 //see why to use 127

struct ipv6_packet
{
  uint8_t header[IPv6_HEADER_SIZE];
  uint8_t *data;
};

struct ipv4_packet 
{
  uint8_t header[IPv4_HEADER_SIZE];
  uint8_t *data;
};

struct ipv4_packet* build_ipv4_packet (struct ipv4_packet *ip, uint8_t version, uint8_t ihl, uint8_t dscp, uint8_t ecn, uint16_t total_length, uint16_t identification, uint8_t flags, uint16_t fragment_offset, uint8_t time_to_live, uint8_t protocol, uint32_t source_ip, uint32_t destination_ip, uint8_t* data, uint32_t data_size);

void print_bit_ipv4 (uint8_t* header);
void recv_ipv4_packet (uint8_t mac[6], uint8_t *data, uint32_t size);
void send_ipv4_packet (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t dscp, uint8_t ecn, uint8_t protocol);

uint8_t* ipv4_to_array (struct ipv4_packet *ip_packet, uint32_t data_size);

struct ipv4_packet* array_to_ipv4 (struct ipv4_packet* ip_packet, uint8_t* array, uint32_t size);


uint32_t ip_get_attr_value (uint8_t* attr, uint32_t offset, uint32_t size);
void ip_set_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint32_t value);

#endif
