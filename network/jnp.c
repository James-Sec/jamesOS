#include <jnp.h>

void jnp_send_message (uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t l4_protocol)
{
  l4_upper_interface (port, ip, mac, data, data_size, l4_protocol);
}

void jnp_recv_message (uint16_t port)
{
  uint8_t* data = kmalloc_u (100);
  struct net_address_set* dest_addresses;
  int32_t receive_port = udp_port_bind(port, data, &dest_addresses);
  task_receive_udp ();
  kprint(data);
  udp_port_unbind (receive_port);
  kfree (data, 100);
}
