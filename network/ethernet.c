#include <ethernet.h>

struct ether_frame* build_ether_frame (uint8_t dest [6], uint16_t type, uint8_t *data, uint32_t size)
{
  struct ether_frame* frame = (struct ether_frame*) kmalloc (sizeof (struct ether_frame));
  memcpy (dest, frame->destination_addr,  6);
  memcpy (rtl8139_device->mac_addr, frame->source_addr,  6);
  frame->ether_type = type;
  memcpy (data, frame->data, size);
  frame->data_size  = size;
  return frame;
}

void send_ether_frame (uint8_t dest[6], uint16_t type, uint8_t *data, uint32_t size)
{
  struct ether_frame* frame = build_ether_frame (dest, type, data, size);
  rtl8139_send_frame (frame);
}

void recv_ether_frame (struct ether_frame* frame)
{
  kprintf ("destination_addr: %x:%x:%x:%x:%x:%x\n", 6, frame->destination_addr[0], frame->destination_addr[1],frame->destination_addr[2],frame->destination_addr[3],frame->destination_addr[4],frame->destination_addr[5]);

  kprintf ("source_addr: %x:%x:%x:%x:%x:%x\n", 6, frame->source_addr[0], frame->source_addr[1],frame->source_addr[2],frame->source_addr[3],frame->source_addr[4],frame->source_addr[5]);

  //ntohs (&(frame->ether_type));
  kprintf ("ether_type: %x\n", 1, frame->ether_type);

  switch (frame->ether_type)
  {
    case ETHER_TYPE_IPV4:
      kprint ("calling ipv4 handler");
      // ipv4 handler
      break;
    default:
      kprint ("undefined protocol");
      break;
  }
}
