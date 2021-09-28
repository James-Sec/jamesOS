#include <tcp.h>

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, struct tcp_header_bit_field *header, uint8_t* options, uint8_t* data, uint32_t data_size)
{
  set_bits_attr_value (tcp->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE, header->destination_port);
  set_bits_attr_value (tcp->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE, header->sequence_number);
  set_bits_attr_value (tcp->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE, header->ack_number);
  set_bits_attr_value (tcp->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE, header->data_offset);
  set_bits_attr_value (tcp->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE, header->reserved);
  set_bits_attr_value (tcp->header, TCP_ECN_OFFSET, TCP_ECN_SIZE, header->ecn);
  set_bits_attr_value (tcp->header, TCP_CWR_OFFSET, TCP_CWR_SIZE, header->cwr);
  set_bits_attr_value (tcp->header, TCP_ECE_OFFSET, TCP_ECE_SIZE, header->ece);
  set_bits_attr_value (tcp->header, TCP_URG_OFFSET, TCP_URG_SIZE, header->urg);
  set_bits_attr_value (tcp->header, TCP_ACK_OFFSET, TCP_ACK_SIZE, header->ack);
  set_bits_attr_value (tcp->header, TCP_PSH_OFFSET, TCP_PSH_SIZE, header->psh);
  set_bits_attr_value (tcp->header, TCP_RST_OFFSET, TCP_RST_SIZE, header->rst);
  set_bits_attr_value (tcp->header, TCP_SYN_OFFSET, TCP_SYN_SIZE, header->syn);
  set_bits_attr_value (tcp->header, TCP_FIN_OFFSET, TCP_FIN_SIZE, header->fin);
  set_bits_attr_value (tcp->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE, header->window_size);
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, header->checksum);
  set_bits_attr_value (tcp->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE, header->urgent_pointer);
  set_bytes_attr_value (tcp->header, TCP_OPTIONS_OFFSET, header->data_offset, options);
  memcpy(data, tcp->data, data_size);
}

void tcp_send_segment (struct tcp_header_bit_field *tcp_header, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size)
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  
  tcp_build_segment (segment, tcp_header, 0, data, data_size);

  uint8_t *tcp_array = tcp_to_array (segment, data_size);

  l3_upper_interface (ip, mac, tcp_array, data_size + TCP_HEADER_MIN_SIZE, L3_PROTOCOL_IPv4, 0, 0, IPv4_PROTOCOL_TCP);


}


struct tcp_segment* array_to_tcp (struct tcp_segment *segment, uint8_t *array, uint32_t size)
{
  uint8_t data_offset = get_bits_attr_value (array, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  memcpy (array, segment->header, (data_offset * 4));
  segment->data = kmalloc_u (size - (data_offset * 4));
  memcpy (array + (data_offset * 4), segment->data, size - (data_offset * 4));
  return segment;
}

uint8_t* tcp_to_array (struct tcp_segment *tcp, uint32_t data_size)
{
  uint8_t data_offset = get_bits_attr_value (tcp->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  uint8_t *array = kmalloc_u ((data_offset * 4) + data_size);
  memcpy (tcp, array,(data_offset * 4));
  memcpy (tcp->data, array + (data_offset * 4), data_size);

  return array;
}

