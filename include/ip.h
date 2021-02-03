#ifndef IP_H
#define IP_H

#include <mem.h>
#include <network_utils.h>
#include <vga.h>
#include <ethernet.h>

#define IPv4_DATA_MAX_LENGTH 65515
#define IPv4_HEADER_SIZE 20

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

struct ipv4_header {
	uint8_t ipv4 [20];
	//uint8_t options [24];
} __attribute__ ((packed));

struct ipv6_header {
	uint8_t ipv6 [40];
} __attribute__ ((packed));

union ip_header {
	struct ipv4_header ipv4;
	struct ipv6_header ipv6;
};

struct ip_packet {
	union ip_header header;
	uint8_t *data;
};

struct ip_packet* build_ipv4_packet (uint32_t destination_ip, uint8_t* data, uint16_t len);
struct ip_packet* _build_ipv4_packet (uint8_t version, uint8_t ihl, uint8_t dscp, uint8_t ecn, uint16_t total_length, uint16_t identification, uint8_t flags, uint16_t fragment_offset, uint8_t time_to_live, uint8_t protocol, uint32_t source_ip, uint32_t destination_ip, uint8_t* data);

void print_bit_ipv4 (uint8_t* header);
void recv_ipv4_handler (struct ip_packet* ip);
void send_ipv4_packet (struct ip_packet* ip, uint8_t* mac_dest_addr);

uint32_t ip_get_attr_value (uint8_t* attr, uint32_t offset, uint32_t size);

#endif
