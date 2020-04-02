#include "../include/ethernet.h"

uint32_t ethernet_send_packet (uint8_t *dst_mac_addr, uint8_t *data, uint32_t len, uint16_t protocol)
{
  ethernet_frame_t *frame = (ethernet_frame_t*) kmalloc (sizeof(ethernet_frame_t) + 3);
  frame -> dst_mac_addr[0] = 0x8a;
  frame -> dst_mac_addr[1] = 0x4e;
  frame -> dst_mac_addr[2] = 0xd8;
  frame -> dst_mac_addr[3] = 0xef;
  frame -> dst_mac_addr[4] = 0xb2;
  frame -> dst_mac_addr[5] = 0x94;

  frame -> src_mac_addr[0] = 0x52;
  frame -> src_mac_addr[1] = 0x55;
  frame -> src_mac_addr[2] = 0x00;
  frame -> src_mac_addr[3] = 0xd1;
  frame -> src_mac_addr[4] = 0x55;
  frame -> src_mac_addr[5] = 0x01;

  void * frame_data = (void*)frame + sizeof(ethernet_frame_t);
  memcpy(frame_data, data, len);

  frame -> type = htons (0x0800);


  rtl8139_send_packet((uint8_t*)frame, sizeof(ethernet_frame_t) + len);


  /*
  uint8_t src_mac_addr[6];
  src_mac_addr[0] = 0x52;
  src_mac_addr[1] = 0x55;
  src_mac_addr[2] = 0x00;
  src_mac_addr[3] = 0xd1;
  src_mac_addr[4] = 0x55;
  src_mac_addr[5] = 0x01;
  ethernet_frame_t * frame = kmalloc(sizeof(ethernet_frame_t) + len);
  void * frame_data = (void*)frame + sizeof(ethernet_frame_t);

  // Fill in source and destination mac address
  memcpy(frame->src_mac_addr, src_mac_addr, 6);
  memcpy(frame->dst_mac_addr, dst_mac_addr, 6);

  // Fill in data
  memcpy(frame_data, data, len);

  // Fill in type
  frame->type = htons(protocol);

  // Send packet
  rtl8139_send_packet(frame, sizeof(ethernet_frame_t) + len);
  kfree(frame);
  */

  return len;
}
