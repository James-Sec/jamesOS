#include <udp.h>

struct udp_segment* build_udp_segment (struct udp_segment *udp, uint16_t source_port, uint16_t destination_port, uint16_t length, uint8_t* data, uint32_t data_size)
{
  set_bytes_attr_value (udp->header, UDP_SOURCE_PORT_OFFSET, UDP_SOURCE_PORT_SIZE, &source_port);
  set_bytes_attr_value (udp->header, UDP_DESTINATION_PORT_OFFSET, UDP_DESTINATION_PORT_SIZE, &destination_port);
  set_bytes_attr_value (udp->header, UDP_LENGTH_OFFSET, UDP_LENGTH_SIZE, &length);
  uint16_t checksum = 0;
  set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);
   //checksum = internet_checksum (udp->header, UDP_HEADER_SIZE, data, data_size);
  //set_bytes_attr_value (udp->header, UDP_CHECKSUM_OFFSET, UDP_CHECKSUM_SIZE, &checksum);

  memcpy (data, udp->data, data_size);
}

uint8_t* udp_to_array (struct udp_segment *udp, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (UDP_HEADER_SIZE + data_size);
  memcpy (udp, array,UDP_HEADER_SIZE);
  memcpy (udp->data, array + UDP_HEADER_SIZE, data_size);

  return array;
}
