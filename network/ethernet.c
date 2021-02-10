#include <ethernet.h>

struct ether_frame* build_ether_frame (struct ether_frame *frame, uint8_t dest [6], uint16_t type, uint8_t *data, uint32_t data_size)
{
  memcpy (dest, frame->destination_addr,  6);
  memcpy (rtl8139_device->mac_addr, frame->source_addr,  6);
  memcpy (&type, &frame->ether_type, sizeof (uint16_t));
  memcpy (data, frame->data, data_size);

  return frame;
}

void send_ether_frame (struct ether_frame* frame)
{
  uint8_t *ether = l1_interface_send_rtl8139 (frame);
  //rtl8139_send_frame (ether, frame->data_size + ETHER_HEADER_SIZE);
}

void recv_ether_frame (struct ether_frame* frame)
{
  //kprintf ("mac_destination_addr: %x:%x:%x:%x:%x:%x\n", 6, frame->destination_addr[0], frame->destination_addr[1],frame->destination_addr[2],frame->destination_addr[3],frame->destination_addr[4],frame->destination_addr[5]);

  //kprintf ("mac_source_addr: %x:%x:%x:%x:%x:%x\n", 6, frame->source_addr[0], frame->source_addr[1],frame->source_addr[2],frame->source_addr[3],frame->source_addr[4],frame->source_addr[5]);

  //ntohs (&(frame->ether_type));
  //kprintf ("ether_type: %x\n", 1, frame->ether_type);

  switch (frame->ether_type)
  {
    case ETHER_TYPE_IPV4:
      kprint ("calling ipv4 handler\n");
      struct ip_packet *ip = l3_interface_recv (frame->data);
      recv_ipv4_handler (ip);
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

struct ether_frame* array_to_ethernet (struct ether_frame* ether, uint8_t* array, uint32_t size)
{
  memcpy (array, ether, ETHER_HEADER_SIZE);
  ether->data = kmalloc_u (size - ETHER_HEADER_SIZE);
  memcpy (array + ETHER_HEADER_SIZE, ether->data, size - ETHER_HEADER_SIZE);

  return ether;
}
