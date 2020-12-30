#ifndef ARP_H
#define ARP_H

#define ARP_HTYPE 0
#define ARP_PTYPE 2
#define ARP_HLEN 4
#define ARP_PLEN 5
#define ARP_OPER 6
#define ARP_SHA 8
#define ARP_SPA 14
#define ARP_THA 18
#define ARP_TPA 24

#define ARP_SIZE 28

#define ARP_TABLE_SIZE 256

#include <stdint.h>
#include <rtl8139.h>

struct arp_t
{
  uint8_t header [28];
};

void recv_arp_handler (struct arp_t* arp);
void send_arp_request (uint8_t* ip);

struct arp_table_entry {
	uint32_t ip_addr;
	uint8_t mac_addr [6];
};

struct arp_table_entry arp_table [ARP_TABLE_SIZE];

uint8_t arp_table_size = 0;


struct arp_t* build_arp_packet (uint16_t htype, uint16_t ptype, uint8_t hlen, uint8_t plen, uint16_t oper, uint8_t sha [6], uint8_t* spa, uint8_t tha[6], uint8_t* tpa);
#endif
