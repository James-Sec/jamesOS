#include <tcp.h>

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, struct tcp_header_bit_field *header, uint8_t* options, uint8_t* data, uint32_t data_size, struct pseudo_ip* pseudo)
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
  set_bits_attr_value (tcp->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE, header->urgent_pointer);
  set_bytes_attr_value (tcp->header, TCP_OPTIONS_OFFSET, (header->data_offset * 4) - TCP_HEADER_MIN_SIZE, options);

  uint16_t checksum = 0;
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);
  checksum = internet_checksum (tcp->header, (header->data_offset * 4), &pseudo->header, PSEUDO_HEADER_SIZE);
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);

  memcpy(data, tcp->data, data_size);
}

void tcp_send_segment (struct tcp_header_bit_field *tcp_header, uint32_t ip, uint8_t mac[6], uint8_t *options, uint8_t *data, uint32_t data_size, struct pseudo_ip* pseudo)
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  
  tcp_build_segment (segment, tcp_header, options, data, data_size, pseudo);

  uint8_t *tcp_array = tcp_to_array (segment, data_size);
  l3_upper_interface (ip, mac, tcp_array, data_size + (tcp_header->data_offset * 4), L3_PROTOCOL_IPv4, 0, 0, IPv4_PROTOCOL_TCP);
}

void tcp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size) 
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  array_to_tcp (segment, data, data_size);
  uint32_t i;
  uint8_t data_offset = get_bits_attr_value (segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  uint8_t syn = get_bits_attr_value (segment->header, TCP_SYN_OFFSET, TCP_SYN_SIZE);
  if (ip == 0x1e1e1e1e && syn)
  {
    kprint ("TCP SYN RECEIVED\n");
    struct tcp_header_bit_field tcp_header;
    tcp_header.source_port = get_bits_attr_value (segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
    tcp_header.destination_port = get_bits_attr_value (segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE);
    tcp_header.sequence_number = get_bits_attr_value (segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE);
    tcp_header.ack_number = tcp_header.sequence_number + 1;
    tcp_header.data_offset = data_offset;
    tcp_header.reserved = 0;
    tcp_header.ecn = 0;
    tcp_header.cwr = 0;
    tcp_header.ece = 0;
    tcp_header.urg = 0;
    tcp_header.ack = 1;
    tcp_header.psh = 0;
    tcp_header.rst = 0;
    tcp_header.syn = 1;
    tcp_header.fin = 0;
    tcp_header.window_size = get_bits_attr_value (segment->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE);
    tcp_header.checksum = 0x0;
    tcp_header.urgent_pointer = 0;
    uint32_t options_size = (data_offset * 4) - TCP_HEADER_MIN_SIZE;
    uint8_t *options = kmalloc_u (options_size);
    get_bytes_attr_value (segment->header, TCP_OPTIONS_OFFSET, options_size, options);
    struct pseudo_ip pseudo;
    uint32_t src_ip = 0x01020304;
    uint32_t dst_ip = 0x1e1e1e1e;
    uint8_t fxd_ip = 0x0;
    uint8_t prt_ip = 0x6;
    uint16_t sgl_ip = 0x28;
    pseudo_header_calculator(src_ip, dst_ip, fxd_ip, prt_ip, sgl_ip, &pseudo);
    kprintf("printing pseudo header: %x, %x, %x, %x, %x\n", 5, src_ip, dst_ip, fxd_ip, prt_ip, sgl_ip);
    for (uint8_t i = 0; i < PSEUDO_HEADER_SIZE; i++)
      kprintf("%x ", 1, pseudo.header[i]);
    kprint("\n");
    tcp_send_segment (&tcp_header, ip, mac, options, 0, 0, &pseudo);
  }
  kprint ("TCP RECEIVED\n");
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

