#include <l3_interface.h>

void l3_upper_interface (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol)
{
  switch (protocol)
  {
    case L3_PROTOCOL_IPv4:
      send_ipv4_packet (ip, mac, data, data_size);
      break;
  }
}

void l3_lower_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol)
{
  kprint ("l3 lower\n");
  switch (protocol)
  {
    case L3_PROTOCOL_IPv4:
      recv_ipv4_packet (mac, data, data_size);
      break;
  }
}
