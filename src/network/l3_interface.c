#include <l3_interface.h>

void l3_upper_interface (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...)
{
  va_list ap;
  switch (protocol)
  {
    case L3_PROTOCOL_IPv4:
      va_start (ap, 3);
      uint32_t dscp = va_arg (ap, int);
      uint32_t ecn = va_arg (ap, int);
      uint8_t protocol = va_arg (ap, int);
      send_ipv4_packet (ip, mac, data, data_size, dscp, ecn, protocol);
      break;
    case L3_PROTOCOL_ICMP4:
      va_start (ap, 3);
      uint32_t type = va_arg (ap, int);
      uint32_t code = va_arg (ap, int);
      uint32_t rest = va_arg (ap, int);
      send_icmp4_packet (ip, mac, type, code, rest, data, data_size);
      break;
  }
}

void l3_lower_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol)
{
  switch (protocol)
  {
    case L3_PROTOCOL_IPv4:
      recv_ipv4_packet (mac, data, data_size);
      break;
  }
}
