#ifndef ETHERNET_H
#define ETHERNET_H

#include "rtl8139.h"

typedef struct ethernet_frame 
{
  uint8_t dst_mac_addr[6];
  uint8_t src_mac_addr[6];
  uint16_t type;
  uint8_t data[];
}__attribute__((packed)) ethernet_frame_t;

uint32_t ethernet_send_packet (uint8_t *dst_mac_addr, uint8_t *data, uint32_t len, uint16_t protocol);
void ethernet_handle_packet (ethernet_frame_t *packet, uint32_t len);

#endif
