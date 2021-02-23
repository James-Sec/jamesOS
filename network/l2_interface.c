#include <l2_interface.h>

void l2_upper_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...)
{
  va_list ap;
  switch (protocol)
  {
    case L2_PROTOCOL_ETHERNET2:
      va_start (ap, 1);
      send_ethernet_frame (mac, data, data_size, va_arg (ap, int));
      break;
    case L2_PROTOCOL_ARP:
      va_start (ap, 1);
      kprint("entering l2_interface\n");
      send_arp_request (va_arg (ap, int));
      break;
  }
}

void l2_lower_interface (uint8_t *data, uint32_t size, uint8_t protocol)
{
  switch (protocol)
  {
    case L2_PROTOCOL_ETHERNET2:
      recv_ethernet_frame (data, size);
      break;
  }
}
