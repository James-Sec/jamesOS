#include <ethernet.h>

struct ether_frame* build_ether_frame (struct ether_frame *frame, uint8_t dest [6], uint16_t type, uint8_t *data, uint32_t data_size)
{
  memcpy (dest, frame->destination_addr,  6);
  memcpy (rtl8139_device->mac_addr, frame->source_addr,  6);
  memcpy (&type, &frame->ether_type, sizeof (uint16_t));
  memcpy (data, frame->data, data_size);

  return frame;
}

void send_ethernet_frame (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint16_t type)
{
  struct ether_frame* ether = kmalloc_u (sizeof (struct ether_frame));
  ether = build_ether_frame (ether, mac, type, data, data_size);
  rtl8139_send_frame (ethernet_to_array (ether, data_size), data_size + ETHER_HEADER_SIZE);

}

void recv_ethernet_frame (uint8_t *data, uint32_t size)
{
  struct ether_frame *frame = kmalloc_u (sizeof (struct ether_frame));
  frame = array_to_ethernet (frame, data, size);
  switch (frame->ether_type)
  {
    case ETHER_TYPE_IPv4:
      kprint ("calling ipv4 handler\n");
      l3_lower_interface (frame->source_addr, frame->data, size - ETHER_HEADER_SIZE, L3_PROTOCOL_IPv4);
      break;
    case ETHER_TYPE_ARP:
      kprint ("calling arp handler\n");
      recv_arp_handler ((struct arp_t*) frame->data);
      break;
    default:
      kprint ("undefined protocol\n");
      break;
  }
}
uint8_t* ethernet_to_array (struct ether_frame *frame, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (data_size + ETHER_HEADER_SIZE);
  memcpy (frame, array,ETHER_HEADER_SIZE);
  memcpy (frame->data, array + ETHER_HEADER_SIZE, data_size);
  
  return array;
}

struct ether_frame* array_to_ethernet (struct ether_frame* frame, uint8_t* array, uint32_t size)
{
  memcpy (array, frame, ETHER_HEADER_SIZE);
  frame->data = kmalloc_u (size - ETHER_HEADER_SIZE);
  memcpy (array + ETHER_HEADER_SIZE, frame->data, size - ETHER_HEADER_SIZE);

  return frame;
}
