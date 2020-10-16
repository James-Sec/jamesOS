#include "../include/ethernet.h"

struct ether_frame* build_ether_frame (uint8_t dest [6], uint8_t type [2], uint8_t *data, uint32_t size)
{
  struct ether_frame* frame = (struct ether_frame*) kmalloc (sizeof (struct ether_frame));
  memcpy (dest, frame->destination_addr,  6);
  memcpy (rtl8139_device->mac_addr, frame->source_addr,  6);
  memcpy (type, frame->ether_type,  2);
  memcpy (data, frame->data, size);
  frame->data_size  = size;
  return frame;
}
