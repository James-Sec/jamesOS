#ifndef ARP_H
#define ARP_H

#define ARP_HTYPE_OFFSET 0
#define ARP_PTYPE_OFFSET 2
#define ARP_HLEN_OFFSET 4
#define ARP_PLEN_OFFSET 5
#define ARP_OPER_OFFSET 6
#define ARP_SHA_OFFSET 8
#define ARP_SPA_OFFSET 14
#define ARP_THA_OFFSET 18
#define ARP_TPA_OFFSET 24

#define ARP_HTYPE_SIZE 2
#define ARP_PTYPE_SIZE 2
#define ARP_HLEN_SIZE 1
#define ARP_PLEN_SIZE 1
#define ARP_OPER_SIZE 2
#define ARP_SHA_SIZE 6
#define ARP_SPA_SIZE 4
#define ARP_THA_SIZE 6
#define ARP_TPA_SIZE 4

#define ARP_HEADER_SIZE 28

#define ARP_TABLE_SIZE 256

#include <stdint.h>
#include <rtl8139.h>

struct arp_t
{
  uint8_t header [28];
};

struct arp_table_entry {
	uint32_t ip_addr;
	uint8_t mac_addr [6];
};

struct arp_table_entry arp_table [ARP_TABLE_SIZE];

uint8_t arp_table_size = 0;


struct arp_t* build_arp_packet (struct arp_t* arp, uint16_t htype, uint16_t ptype, uint8_t hlen, uint8_t plen, uint16_t oper, uint8_t sha [6], uint8_t* spa, uint8_t tha[6], uint8_t* tpa);

void recv_arp_handler (struct arp_t* arp);

void send_arp_request (uint32_t ip);
#endif
