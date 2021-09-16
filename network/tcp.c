#include <tcp.h>

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, struct tcp_header_bit_field *header, uint8_t* options, uint8_t* data, uint32_t data_size)
{
  set_bits_attr_value (tcp->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_ECN_OFFSET, TCP_ECN_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_CWR_OFFSET, TCP_CWR_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_ECE_OFFSET, TCP_ECE_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_URG_OFFSET, TCP_URG_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_ACK_OFFSET, TCP_ACK_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_PSH_OFFSET, TCP_PSH_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_RST_OFFSET, TCP_RST_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_SYN_OFFSET, TCP_SYN_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_FIN_OFFSET, TCP_FIN_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE, header->source_port);
  set_bits_attr_value (tcp->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE, header->source_port);
  set_bytes_attr_value (tcp->header, TCP_OPTIONS_OFFSET, header->data_offset, options);
  memcpy(data, tcp->data, data_size);
}
