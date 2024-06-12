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
  if (data_size) {
    tcp->data = kmalloc_u (data_size);
    memcpy(data, tcp->data, data_size);
  }
}

int32_t tcp_bind (uint16_t port)
{
  if (!port) {
    for (uint16_t i = TCP_EPHEMERAL_PORT_BEGIN; i <= TCP_EPHEMERAL_PORT_END; i++) {
      if (!tcp_port_table [i].pid) {
        port = i;
        break;
      }
    }
    if (!port)
      return -1;
  }
  /*
  else if (tcp_port_table [port].pid)
    return -1;
    */

  tcp_port_table [port].pid = current_task->pid;
  return port;
}

uint32_t tcp_connect (uint16_t src_port, uint16_t dest_port, uint32_t ip, uint8_t mac[6])
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  tcp_build_segment (segment, src_port, dest_port, 0, 0,TCP_HEADER_MIN_SIZE / 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, TCP_SLIDING_WINDOW_SIZE, 0, 0, 0, 0, ip);
  tcp_port_table[src_port].state = TCP_STATE_WAITING_THREEWAY_SYN_ACK;
  while(!tcp_port_table[src_port].connection_stablished_or_reseted){
    tcp_send_segment (segment, 0, ip, mac);
    sleep(1);
  }
}

uint32_t tcp_listen (uint16_t port)
{
  tcp_port_table[port].state = TCP_STATE_WAITING_THREEWAY_SYN;
}

void tcp_send_segment (struct tcp_segment *segment, uint32_t data_size, uint32_t ip, uint8_t mac[6])
{
  if (!data_size) {
    uint8_t *tcp_array = tcp_to_array (segment, data_size);
    uint16_t data_offset = get_bits_attr_value (segment->header, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
    l3_upper_interface (ip, mac, tcp_array, data_size + (data_offset * 4), L3_PROTOCOL_IPv4, 0, 0, IPv4_PROTOCOL_TCP);
    kfree(segment, data_size);

    return;
  }

  tcp_sender_enqueue (segment, data_size, ip, mac);
}

uint8_t tcp_send_syn_ack(uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
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
      TCP_SLIDING_WINDOW_SIZE,
      get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
      options, 0, 0, ip);
  tcp_send_segment (send_segment, 0, ip, mac);
  kfree(options, options_size);
}

uint8_t tcp_send_ack(uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size, uint32_t ack_number)
{
  struct tcp_segment* send_segment = kmalloc_u(sizeof(struct tcp_segment));

  if(!data_size)
    data_size++;
  tcp_build_segment (send_segment,
      get_bits_attr_value(recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE),
      get_bits_attr_value(recv_segment->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE),
      ack_number,
      TCP_HEADER_MIN_SIZE / 4,
      get_bits_attr_value(recv_segment->header, TCP_RESERVED_OFFSET, TCP_RESERVED_SIZE),
      0, 0, 0, 0, 1, 0, 0, 0, 0,
      TCP_SLIDING_WINDOW_SIZE,
      get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
      0, 0, 0, ip);
  tcp_send_segment (send_segment, 0, ip, mac);

  kfree(send_segment, sizeof(struct tcp_segment));
}

uint8_t tcp_send_fin_ack (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
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
      0, 0, 0, 0, 1, 0, 0, 0, 1,
      TCP_SLIDING_WINDOW_SIZE,
      get_bits_attr_value(recv_segment->header, TCP_URGENT_POINTER_OFFSET, TCP_URGENT_POINTER_SIZE),
      options, 0, 0, ip);
  tcp_send_segment (send_segment, 0, ip, mac);

  kfree(send_segment, sizeof(struct tcp_segment));
  kfree(options, options_size);
}

uint8_t tcp_state_threeway_syn_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  if (tcp_recv_threeway_syn (recv_segment))
  {
    tcp_send_syn_ack(ip, mac, recv_segment, data, data_size);
    return 1;
  }
  return 0;
}

uint8_t tcp_state_threeway_syn_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  if (tcp_recv_threeway_syn_ack (recv_segment))
  {
    uint32_t ack_number = get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE) + data_size + 1;
    tcp_send_ack(ip, mac, recv_segment, data, data_size, ack_number);
    return 1;
  }
  return 0;
}

uint8_t tcp_state_threeway_ack_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  if (tcp_recv_threeway_ack (recv_segment))
    return 1;
  return 0;
}

uint32_t tcp_read (uint16_t port, uint8_t *buffer, uint32_t size)
{
  lock_irq ();
  struct tcp_recv_sliding_window *window = tcp_port_table[port].recv_window;

  uint32_t last_acked_byte = window->last_acked_byte;
  uint32_t first_byte = window->first_window_byte;
  uint32_t last_byte = min (first_byte + size -1, last_acked_byte);

  uint32_t bytes_read = 0;
  for (uint32_t curr_byte = first_byte; curr_byte < last_byte ; curr_byte ++) {
    buffer[bytes_read++] = window->buffer[curr_byte % TCP_SLIDING_WINDOW_SIZE];
    bitmap_clear(window->bitmap, curr_byte % TCP_SLIDING_WINDOW_SIZE);
    window->first_window_byte ++;
  }

  unlock_irq ();
  return bytes_read;

}

uint8_t tcp_state_connected_handler (uint32_t ip, uint8_t mac[6], struct tcp_segment *recv_segment, uint8_t* data, uint32_t data_size)
{
  if (tcp_recv_threeway_ack (recv_segment)) {
    uint16_t port = get_bits_attr_value (recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
    struct tcp_send_sliding_window *window = tcp_port_table[port].send_window;
    uint32_t ack_number = get_bits_attr_value (recv_segment->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE);
    window->last_acked_byte = ack_number;
    return 0;
  }

  if (tcp_recv_psh_ack (recv_segment)) {
    lock_irq ();
    // check if it fits in sliding window
    uint16_t port = get_bits_attr_value (recv_segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
    struct tcp_recv_sliding_window *window = tcp_port_table[port].recv_window;
    uint32_t sequence_number = get_bits_attr_value (recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE);

    uint32_t first_received_byte = get_bits_attr_value(recv_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE);
    uint32_t last_received_byte = first_received_byte + data_size - 1;
    uint32_t last_acked_byte = window->last_acked_byte;
    uint32_t first_possible_byte = window->first_window_byte;
    uint32_t last_possible_byte = first_possible_byte + TCP_SLIDING_WINDOW_SIZE - 1;

    if (first_received_byte > last_possible_byte)
      goto end_unlock;

    if (last_received_byte < last_acked_byte) {
      tcp_send_ack(ip, mac, recv_segment->header, data, data_size, last_acked_byte);
      goto end_unlock;
    }

    uint32_t curr_byte = max (last_acked_byte, first_received_byte);
    uint32_t last_byte = min (last_possible_byte, last_received_byte);

    while (curr_byte <= last_byte) {
      if (bitmap_read(window->bitmap, curr_byte % TCP_SLIDING_WINDOW_SIZE))
        goto end_while;

      bitmap_set(window->bitmap, curr_byte % TCP_SLIDING_WINDOW_SIZE);
      window->buffer[curr_byte % TCP_SLIDING_WINDOW_SIZE] = data[curr_byte - sequence_number];

end_while:
      curr_byte++;
    }

    // send the higher ack byte possible
    if (last_acked_byte < curr_byte) {
      last_acked_byte = curr_byte;
      tcp_send_ack(ip, mac, recv_segment->header, data, data_size, last_acked_byte);
      window->last_acked_byte = last_acked_byte;
    }

end_unlock:
    unlock_irq ();
    return 0;
  }

  if (tcp_recv_fin_ack (recv_segment))
  {
    tcp_send_fin_ack(ip, mac, recv_segment, data, data_size);
    return 1;
  }
  return 0;
}

void tcp_init_window (uint16_t port, uint32_t sequence_number, uint16_t window_size)
{
  struct tcp_port_table_entry *port_struct = &tcp_port_table[port];

  struct tcp_recv_sliding_window *recv_window = kmalloc_u (sizeof(struct tcp_recv_sliding_window));
  struct tcp_send_sliding_window *send_window = kmalloc_u (sizeof(struct tcp_send_sliding_window));

  port_struct->recv_window = recv_window;
  port_struct->send_window = send_window;

  //recv window
  recv_window->last_acked_byte = sequence_number + 1;
  recv_window->first_window_byte = sequence_number + 1;
  
  //send window
  send_window->last_acked_byte = 1;
  if (window_size < TCP_SLIDING_WINDOW_SIZE)
    send_window->window_size = window_size;
  else
    send_window->window_size = TCP_SLIDING_WINDOW_SIZE;
}

void tcp_recv_segment (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t segment_size)
{
  struct tcp_segment *segment = kmalloc_u (sizeof (struct tcp_segment));
  array_to_tcp (segment, data, segment_size);

  uint16_t port = get_bits_attr_value (segment->header, TCP_DESTINATION_PORT_OFFSET, TCP_DESTINATION_PORT_SIZE);
  uint8_t data_offset = get_bits_attr_value (data, TCP_DATA_OFFSET_OFFSET, TCP_DATA_OFFSET_SIZE);
  uint32_t data_size = segment_size - (data_offset * 4);

  uint32_t sequence_number = get_bits_attr_value (data, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE);
  uint16_t window_size = get_bits_attr_value (data, TCP_WINDOW_SIZE_OFFSET, TCP_WINDOW_SIZE_SIZE);

  switch (tcp_port_table[port].state)
  {
    case TCP_STATE_WAITING_THREEWAY_SYN:
      if (tcp_state_threeway_syn_handler (ip, mac, segment, segment->data, data_size)){
        tcp_port_table[port].state = TCP_STATE_WAITING_THREEWAY_ACK;
        tcp_init_window (port, sequence_number, window_size);
      }
      break;
    case TCP_STATE_WAITING_THREEWAY_SYN_ACK:
      if (tcp_state_threeway_syn_ack_handler (ip, mac, segment, segment->data, data_size)){
        tcp_port_table[port].state = TCP_STATE_CONNECTED;
        tcp_port_table[port].connection_stablished_or_reseted = 1;
        tcp_init_window (port, sequence_number, window_size);
      }
      break;
    case TCP_STATE_WAITING_THREEWAY_ACK:
      if(tcp_state_threeway_ack_handler (ip, mac, segment, segment->data, data_size))
        tcp_port_table[port].state = TCP_STATE_CONNECTED;
      break;
    case TCP_STATE_CONNECTED:
      if(tcp_state_connected_handler(ip, mac, segment, segment->data, data_size)) {
        tcp_port_table[port].state = TCP_STATE_CLOSE_WAIT;
      }
      break;
    case TCP_STATE_FIN_WAIT_1:
      tcp_port_table[port].state = TCP_STATE_FIN_WAIT_2;
      tcp_port_table[port].state = TCP_STATE_CLOSING;
      tcp_port_table[port].state = TCP_STATE_TIME_WAIT;
      break;
    case TCP_STATE_FIN_WAIT_2:
      tcp_port_table[port].state = TCP_STATE_TIME_WAIT;
      break;
    case TCP_STATE_CLOSING:
      tcp_port_table[port].state = TCP_STATE_TIME_WAIT;
      break;
    case TCP_STATE_TIME_WAIT:
      tcp_port_table[port].state = TCP_STATE_CLOSED;
      break;
    case TCP_STATE_CLOSE_WAIT:
      tcp_port_table[port].state = TCP_STATE_LAST_ACK;
      break;
    case TCP_STATE_LAST_ACK:
      tcp_port_table[port].state = TCP_STATE_CLOSED;
      break;
  }

  
  kfree (segment->data, segment_size - (data_offset * 4));
  kfree (segment, sizeof (struct tcp_segment));
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

uint8_t tcp_recv_fin_ack (struct tcp_segment *segment)
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
                flags->fin;
  kfree (flags, sizeof (struct tcp_flags));
  return ret;
}
