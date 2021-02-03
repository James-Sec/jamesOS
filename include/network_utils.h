#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>
#include <ethernet.h>
#include <icmp.h>

void htonb (uint8_t* byte, uint8_t bits);
void ntohb (uint8_t* byte, uint8_t bits);

void htons (uint16_t* shrt);
void ntohs (uint16_t* shrt);

void htonl (uint32_t* lng);
void ntohl (uint32_t* lng);

uint8_t* l3_interface_send (uint8_t* header, uint32_t header_size, uint8_t* data, uint32_t data_size);
struct ip_packet* l3_interface_recv (uint8_t* packet);
struct icmp4* l3_interface_recv_icmp4 (uint8_t* packet, uint32_t size);
struct ether_frame* l2_interface_recv_ethernet2 (uint8_t* packet, uint32_t size);
struct ether_frame* l2_interface_send_ethernet2 (struct ip_packet* ip, uint8_t* mac_dest_addr);
uint8_t* l1_interface_send_rtl8139 (struct ether_frame* ether);

#endif
