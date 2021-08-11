#include <l4_interface.h>

void l4_upper_interface (uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...)
{
  va_list ap;
  switch (protocol)
  {
    case L4_PROTOCOL_UDP:
      va_start (ap, 1);
      uint16_t source_port = va_arg (ap, int);
      if (!source_port)
      {
        //select an available port
      }
      send_udp_segment (source_port, port, ip, mac, data, data_size);

      // get source_port
      break;
  }
}

void l4_lower_interface (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol)
{
  switch (protocol)
  {
    case L4_PROTOCOL_UDP:
      //recv_udp_segment (ip, mac, data, data_size);
      break;
  }
}
