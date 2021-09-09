#include <l5_interface.h>

void l5_upper_interface(uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...)
{
  va_list ap;
  switch (protocol)
  {
    case L5_PROTOCOL_JNP:
      va_start (ap, 1);
      uint8_t l4_protocol = va_arg (ap, int);
      jnp_send_message (port, ip, mac, data, data_size, l4_protocol);
      break;
  }
}

void l5_lower_interface(uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol)
{
}
