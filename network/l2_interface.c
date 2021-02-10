#include <l2_interface.h>

void l2_upper_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...)
{
  va_list ap;
  switch (protocol)
  {
    case 0:
      va_start (ap, 1);
      struct ether_frame* ether = kmalloc_u (sizeof (struct ether_frame));
      uint16_t type = va_arg (ap, int);
      ether = build_ether_frame (ether, mac, type, data, data_size);
      rtl8139_send_frame (ethernet_to_array (ether, data_size), data_size + ETHER_HEADER_SIZE);
      break;
  }
}

void l2_lower_interface (uint8_t *data, uint32_t size)
{
  struct ether_frame *ether = kmalloc_u (sizeof (struct ether_frame));
  ether = array_to_ethernet (ether, data, size);

  switch (ether->ether_type)
  {
    case ETHER_TYPE_IPV4:
      //l3_lower_interface (ether->source_addr, ether->data, size - ETHER_HEADER_SIZE, L3_PROTOCOL_IPv4);
      break;
  }
}
