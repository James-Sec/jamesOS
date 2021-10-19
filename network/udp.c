#include <udp.h>

struct udp_segment* build_udp_segment (struct udp_segment *udp, uint16_t source_port, uint16_t destination_port, uint16_t length, uint8_t* data, uint32_t data_size, uint8_t *pseudo)
{
  set_bytes_attr_value (udp->header, UDP_SOURCE_PORT_OFFSET, UDP_SOURCE_PORT_SIZE, &source_port);
  set_bytes_attr_value (udp->header, UDP_DESTINATION_PORT_OFFSET, UDP_DESTINATION_PORT_SIZE, &destination_port);
  htons (&length);
  set_bytes_attr_value (udp->header, UDP_LENGTH_OFFSET, UDP_LENGTH_SIZE, &length);
  uint16_t checksum = 0;
  set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);
  checksum = internet_checksum (udp->header, UDP_HEADER_SIZE, data, data_size, pseudo, PSEUDO_HEADER_SIZE);
  htons (&checksum);
  set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);
  memcpy (data, udp->data, data_size);
}

void udp_send_segment (uint16_t source_port, uint16_t destination_port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size)
{
  struct pseudo_ip pseudo;
  pseudo_header_build(rtl8139_device->ip_addr, ip, 0x0, IPv4_PROTOCOL_UDP, data_size + UDP_HEADER_SIZE, &pseudo);

  struct udp_segment *segment = kmalloc_u (sizeof (struct udp_segment));
  segment->data = kmalloc_u (data_size);
  build_udp_segment (segment, source_port, destination_port, UDP_HEADER_SIZE + data_size, data, data_size, pseudo.header);

  uint8_t *array = udp_to_array (segment, data_size);
  l3_upper_interface (ip, mac, array, UDP_HEADER_SIZE + data_size, L3_PROTOCOL_IPv4, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_UDP);

  kfree (segment->data, data_size);
  kfree (segment, sizeof (struct udp_segment));
  kfree (array, UDP_HEADER_SIZE + data_size);
}

void udp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size)
{
  kprint ("UDP RECEIVED\n");
  struct udp_segment *segment = kmalloc_u (sizeof (struct udp_segment));
  array_to_udp (segment, data, data_size);

  uint16_t destination_port;
  get_bytes_attr_value (segment->header, UDP_DESTINATION_PORT_OFFSET, UDP_DESTINATION_PORT_SIZE, &destination_port);
  ntohs(&destination_port);

  uint16_t source_port;
  get_bytes_attr_value (segment->header, UDP_SOURCE_PORT_OFFSET, UDP_SOURCE_PORT_SIZE, &source_port);
  udp_forward_segment_to_process (destination_port, segment->data, data_size - UDP_HEADER_SIZE, source_port, ip, mac);

  kfree (segment->data, data_size - UDP_HEADER_SIZE);
  kfree (segment, sizeof (struct udp_segment));
}

struct udp_segment* array_to_udp (struct udp_segment *segment, uint8_t *array, uint32_t size)
{
  memcpy (array, segment->header, UDP_HEADER_SIZE);
  segment->data = kmalloc_u (size - UDP_HEADER_SIZE);
  memcpy (array + UDP_HEADER_SIZE, segment->data, size - UDP_HEADER_SIZE);
  return segment;
}

uint8_t* udp_to_array (struct udp_segment *udp, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (UDP_HEADER_SIZE + data_size);
  memcpy (udp, array,UDP_HEADER_SIZE);
  memcpy (udp->data, array + UDP_HEADER_SIZE, data_size);

  return array;
}

void udp_forward_segment_to_process (uint16_t port, uint8_t* data, uint32_t data_size, uint16_t source_port, uint32_t source_ip, int8_t* source_mac)
{
  if (!udp_port_table [port].pid)
    return;
  memcpy(data, udp_port_table [port].data, data_size);
  memcpy(source_mac, udp_port_table [port].net_addresses.mac, 6);
  udp_port_table [port].net_addresses.ip = source_ip;
  udp_port_table [port].net_addresses.port = source_port;
  soft_unblock_task (udp_port_table [port].pid);
}

int32_t udp_port_bind (uint16_t port, uint8_t* data, struct net_address_set** address)
{
  if (!port)
  {
    for (uint16_t i = UDP_EPHEMERAL_PORT_BEGIN; i <= UDP_EPHEMERAL_PORT_END; i++)
    {
      if (!udp_port_table [i].pid)
      {
        udp_port_table [i].pid = current_task->pid;
        port = i;
        break;
      }
    }
  }
  if (udp_port_table [port].pid)
    return -1;
  *address = &udp_port_table[port].net_addresses;
  udp_port_table [port].data = data;
  udp_port_table [port].pid = current_task->pid;
  return port;
}

int32_t udp_port_unbind (uint16_t port)
{
  if (udp_port_table [port].pid != current_task->pid)
    return -1;
  udp_port_table [port].pid = 0;
  return 0;
}

void udp_receive ()
{
  block_task (BLOCKED);
}
