#include <tcp.h>

struct tcp_segment* tcp_build_segment (struct tcp_segment *tcp, uint16_t source_port, uint16_t destination_port, uint32_t sequence_number, uint32_t ack_number, uint8_t data_offset, uint8_t reserved, uint8_t ecn, uint8_t cwr, uint8_t ece, uint8_t urg, uint8_t ack, uint8_t psh, uint8_t rst, uint8_t syn, uint8_t fin, uint16_t window_size, uint16_t urgent_pointer, uint8_t* options, uint8_t* data, uint32_t data_size, uint32_t ip)
{
  set_bits_attr_value (tcp->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE, source_port);
  set_bits_attr_value (tcp->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE, destination_port);
  set_bits_attr_value (tcp->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE, sequence_number);
  set_bits_attr_value (tcp->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE, ack_number);
  set_bits_attr_value (tcp->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE, data_offset);
  set_bits_attr_value (tcp->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE, reserved);
  set_bits_attr_value (tcp->header, TCP_ECN_OFFSET, TCP_ECN_SIZE, ecn);
  set_bits_attr_value (tcp->header, TCP_CWR_OFFSET, TCP_CWR_SIZE, cwr);
  set_bits_attr_value (tcp->header, TCP_ECE_OFFSET, TCP_ECE_SIZE, ece);
  set_bits_attr_value (tcp->header, TCP_URG_OFFSET, TCP_URG_SIZE, urg);
  set_bits_attr_value (tcp->header, TCP_ACK_OFFSET, TCP_ACK_SIZE, ack);
  set_bits_attr_value (tcp->header, TCP_PSH_OFFSET, TCP_PSH_SIZE, psh);
  set_bits_attr_value (tcp->header, TCP_RST_OFFSET, TCP_RST_SIZE, rst);
  set_bits_attr_value (tcp->header, TCP_SYN_OFFSET, TCP_SYN_SIZE, syn);
  set_bits_attr_value (tcp->header, TCP_FIN_OFFSET, TCP_FIN_SIZE, fin);
  set_bits_attr_value (tcp->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE, window_size);
  set_bits_attr_value (tcp->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE, urgent_pointer);
  set_bytes_attr_value (tcp->header, TCP_OPTIONS_OFFSET, (data_offset * 4) - TCP_HEADER_MIN_SIZE, options);
  struct pseudo_ip pseudo;
  pseudo_header_build (rtl8139_device->ip_addr, ip, 0x0, IPv4_PROTOCOL_TCP, data_size + (data_offset * 4), &pseudo);
  uint16_t checksum = 0;
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);
  checksum = internet_checksum (tcp->header, (data_offset * 4), data, data_size, pseudo.header, PSEUDO_HEADER_SIZE);
  set_bits_attr_value (tcp->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);
  memcpy(data, tcp->data, data_size);
}

int32_t tcp_bind (uint16_t port, uint8_t* data, struct net_address_set** address)
{
  if (!port)
  {
    for (uint16_t i = TCP_EPHEMERAL_PORT_BEGIN; i <= TCP_EPHEMERAL_PORT_END; i++)
    {
      if (!tcp_port_table [i].pid)
      {
        port = i;
        break;
      }
    }
    if (!port)
      return -1;
  }
  else if (tcp_port_table [port].pid)
    return -1;

  *address = &tcp_port_table[port].net_addresses;
  tcp_port_table [port].data = data;
  tcp_port_table [port].pid = current_task->pid;
  return port;
}

uint32_t tcp_connect (uint16_t src_port, uint16_t dest_port, uint32_t ip, uint8_t mac[6])
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  tcp_build_segment (segment, src_port, dest_port, 0, 0,5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 65535, 0, 0, 0, 0, ip);
  tcp_send_segment (segment, 0, ip, mac);
  tcp_port_table[src_port].state = TCP_STATE_WAITING_THREEWAY_SYN_ACK;
  block_task (BLOCKED);
}

void tcp_send_segment (struct tcp_segment *segment, uint32_t data_size, uint32_t ip, uint8_t mac[6])
{
  uint8_t *tcp_array = tcp_to_array (segment, data_size);
  uint16_t data_offset = get_bits_attr_value (segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  kprintf ("data_offset: %d\n", 1, data_offset);
  l3_upper_interface (ip, mac, tcp_array, data_size + (data_offset * 4), L3_PROTOCOL_IPv4, 0, 0, IPv4_PROTOCOL_TCP);
}

void tcp_threeway_syn_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  kprint ("TCP SYN RECEIVED\n");
  struct tcp_segment* send_segment = kmalloc_u(sizeof(struct tcp_segment));
  uint32_t data_offset = get_bits_attr_value(recv_segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  uint32_t options_size = (data_offset * 4) - TCP_HEADER_MIN_SIZE;
  uint8_t* options = kmalloc_u(options_size);
  get_bytes_attr_value (recv_segment->header, TCP_OPTIONS_OFFSET, options_size, options);
  tcp_build_segment (send_segment,
    get_bits_attr_value(recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE),
    get_bits_attr_value(recv_segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE),
    0,
    get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE) + 1,
    data_offset,
    get_bits_attr_value(recv_segment->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE),
    0, 0, 0, 0, 1, 0, 0, 1, 0,
    get_bits_attr_value(recv_segment->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE),
    get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
    options, 0, 0, ip);
  tcp_send_segment (send_segment, 0, ip, mac);
  kfree(options, options_size);
}

void tcp_threeway_synack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  kprint ("TCP SYNACK RECEIVED\n");
  uint16_t port = get_bits_attr_value (recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
  if (tcp_port_table[port].state == TCP_STATE_WAITING_THREEWAY_SYN_ACK)
  {
    struct tcp_segment* send_segment = kmalloc_u(sizeof(struct tcp_segment));
    uint32_t data_offset = get_bits_attr_value(recv_segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
    uint32_t options_size = (data_offset * 4) - TCP_HEADER_MIN_SIZE;
    uint8_t* options = kmalloc_u(options_size);
    get_bytes_attr_value (recv_segment->header, TCP_OPTIONS_OFFSET, options_size, options);
    tcp_build_segment (send_segment,
      get_bits_attr_value(recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE) + 1,
      data_offset,
      get_bits_attr_value(recv_segment->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE),
      0, 0, 0, 0, 1, 0, 0, 0, 0,
      get_bits_attr_value(recv_segment->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
      options, 0, 0, ip);
    tcp_send_segment (send_segment, 0, ip, mac);
    tcp_port_table[port].state = TCP_STATE_WAITING_THREEWAY_ACK;
    kfree(options, options_size);
  }
}

void tcp_threeway_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  kprint ("TCP ACK RECEIVED\n");
  uint16_t port = get_bits_attr_value (recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
  htons (&port);
  if (tcp_port_table[port].state == TCP_STATE_WAITING_THREEWAY_ACK)
  {
    struct tcp_segment* send_segment = kmalloc_u(sizeof(struct tcp_segment));
    uint32_t data_offset = get_bits_attr_value(recv_segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
    uint32_t options_size = (data_offset * 4) - TCP_HEADER_MIN_SIZE;
    uint8_t* options = kmalloc_u(options_size);
    get_bytes_attr_value (recv_segment->header, TCP_OPTIONS_OFFSET, options_size, options);
    tcp_build_segment (send_segment,
      get_bits_attr_value(recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE),
      data_offset,
      get_bits_attr_value(recv_segment->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE),
      0, 0, 0, 0, 1, 0, 0, 0, 0,
      get_bits_attr_value(recv_segment->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
      options, 0, 0, ip);
    tcp_port_table[port].state = TCP_STATE_CONNECTED;
    tcp_send_segment (send_segment, 0, ip, mac);
    kfree(options, options_size);
  }
}

void tcp_threeway_psh_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint32_t sequence_number, uint8_t* data, uint32_t data_size)
{
  kprint ("TCP PSH ACK RECEIVED\n");
  struct tcp_segment* send_segment = kmalloc_u(sizeof(struct tcp_segment));
  uint32_t data_offset = get_bits_attr_value(recv_segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  uint32_t options_size = (data_offset * 4) - TCP_HEADER_MIN_SIZE;
  uint8_t* options = kmalloc_u(options_size);
  get_bytes_attr_value (recv_segment->header, TCP_OPTIONS_OFFSET, options_size, options);
  tcp_build_segment (send_segment,
    get_bits_attr_value(recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE),
    get_bits_attr_value(recv_segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE),
    sequence_number,
    get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE) + data_size,
    data_offset,
    get_bits_attr_value(recv_segment->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE),
    0, 0, 0, 0, 1, 0, 0, 0, 0,
    get_bits_attr_value(recv_segment->header, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE),
    get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
    options, 0, 0, ip);
  tcp_send_segment (send_segment, 0, ip, mac);
  kfree(options, options_size);
}

void tcp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size) 
{
  kprint ("TCP RECEIVED\n");
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  array_to_tcp (segment, data, data_size);

  if (tcp_recv_threeway_syn (segment))
    tcp_threeway_syn_handler (ip, mac, segment, 0, 0);

  if (tcp_recv_threeway_syn_ack (segment))
    tcp_threeway_synack_handler (ip, mac, segment, 0, 0);

  if (tcp_recv_threeway_ack (segment))
    tcp_threeway_ack_handler (ip, mac, segment, 0, 0);

  if (tcp_recv_psh_ack (segment) && data_size > 0) {
    uint8_t data_offset = get_bits_attr_value (data, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
    tcp_threeway_psh_ack_handler (ip, mac, segment, 0, 0, data_size - (data_offset * 4));
  }
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

struct tcp_flags* tcp_get_flags (struct tcp_segment *segment)
{
  struct tcp_flags *flags = kmalloc_u (sizeof (struct tcp_flags));
  flags->ecn = get_bits_attr_value (segment->header, TCP_ECN_OFFSET, TCP_ECN_SIZE);
  flags->cwr = get_bits_attr_value (segment->header, TCP_CWR_OFFSET, TCP_CWR_SIZE);
  flags->ece = get_bits_attr_value (segment->header, TCP_ECE_OFFSET, TCP_ECE_SIZE);
  flags->urg = get_bits_attr_value (segment->header, TCP_URG_OFFSET, TCP_URG_SIZE);
  flags->ack = get_bits_attr_value (segment->header, TCP_ACK_OFFSET, TCP_ACK_SIZE);
  flags->psh = get_bits_attr_value (segment->header, TCP_PSH_OFFSET, TCP_PSH_SIZE);
  flags->rst = get_bits_attr_value (segment->header, TCP_RST_OFFSET, TCP_RST_SIZE);
  flags->syn = get_bits_attr_value (segment->header, TCP_SYN_OFFSET, TCP_SYN_SIZE);
  flags->fin = get_bits_attr_value (segment->header, TCP_FIN_OFFSET, TCP_FIN_SIZE);
  return flags;
}

uint8_t tcp_recv_threeway_syn (struct tcp_segment *segment)
{
  struct tcp_flags *flags = tcp_get_flags (segment);
  uint8_t ret = !flags->ecn &&
                !flags->cwr &&
                !flags->ece &&
                !flags->urg &&
                !flags->ack &&
                !flags->psh &&
                !flags->rst &&
                flags->syn && 
                !flags->fin;
  kfree (flags, sizeof (struct tcp_flags));
  return ret;
}

uint8_t tcp_recv_threeway_syn_ack (struct tcp_segment *segment)
{
  struct tcp_flags *flags = tcp_get_flags (segment);
  uint8_t ret = !flags->ecn &&
                !flags->cwr &&
                !flags->ece &&
                !flags->urg &&
                flags->ack &&
                !flags->psh &&
                !flags->rst &&
                flags->syn && 
                !flags->fin;
  kfree (flags, sizeof (struct tcp_flags));
  return ret;
}

uint8_t tcp_recv_threeway_ack (struct tcp_segment *segment)
{
  struct tcp_flags *flags = tcp_get_flags (segment);
  uint8_t ret = !flags->ecn &&
                !flags->cwr &&
                !flags->ece &&
                !flags->urg &&
                flags->ack &&
                !flags->psh &&
                !flags->rst &&
                !flags->syn && 
                !flags->fin;
  kfree (flags, sizeof (struct tcp_flags));
  return ret;
}

uint8_t tcp_recv_psh_ack (struct tcp_segment *segment)
{
  struct tcp_flags *flags = tcp_get_flags (segment);
  uint8_t ret = !flags->ecn &&
                !flags->cwr &&
                !flags->ece &&
                !flags->urg &&
                flags->ack &&
                flags->psh &&
                !flags->rst &&
                !flags->syn && 
                !flags->fin;
  kfree (flags, sizeof (struct tcp_flags));
  return ret;
}
