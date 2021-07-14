#include <udp.h>

struct udp_segment* build_udp_segment (struct udp_segment *udp, uint16_t source_port, uint16_t destination_port, uint16_t length, uint8_t* data, uint32_t data_size)
{
  set_bytes_attr_value (udp->header, UDP_SOURCE_PORT_OFFSET, UDP_SOURCE_PORT_SIZE, &source_port);
  set_bytes_attr_value (udp->header, UDP_DESTINATION_PORT_OFFSET, UDP_DESTINATION_PORT_SIZE, &destination_port);
  htons (&length);
  set_bytes_attr_value (udp->header, UDP_LENGTH_OFFSET, UDP_LENGTH_SIZE, &length);
  uint16_t checksum = 0;
  set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);
   //checksum = internet_checksum (udp->header, UDP_HEADER_SIZE, data, data_size);
  //set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);

  memcpy (data, udp->data, data_size);
}

void send_udp_segment (uint16_t source_port, uint16_t destination_port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size)
{
  struct udp_segment *segment = kmalloc_u (sizeof (struct udp_segment));
  segment->data = kmalloc_u (data_size);
  build_udp_segment (segment, source_port, destination_port, UDP_HEADER_SIZE + data_size, data, data_size);
  uint8_t *array = udp_to_array (segment, data_size);
  l3_upper_interface (ip, mac, array, UDP_HEADER_SIZE + data_size, L3_PROTOCOL_IPv4, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_UDP);
  kfree (segment->data, data_size);
  kfree (segment, sizeof (struct udp_segment));
  kfree (array, UDP_HEADER_SIZE + data_size);
}

void recv_udp_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size)
{
  struct udp_segment *segment = kmalloc_u (sizeof (struct udp_segment));
  array_to_udp (segment, data, data_size);

  kprintf ("UDP MESSAGE RECEIVED: %s\n", 1, segment->data);

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
