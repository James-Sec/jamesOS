#include <tcp_sender.h>

void tcp_sender_enqueue (struct tcp_segment *segment, uint32_t data_size, uint32_t ip, uint8_t mac[6])
{
  lock_irq ();
  struct tcp_segment *segment_copy = kmalloc_u (sizeof (struct tcp_segment));
  memcpy (segment, segment_copy, sizeof (struct tcp_segment));

  segment_copy->data = kmalloc_u (data_size);
  memcpy (segment->data, segment_copy->data, data_size);

  for (uint32_t i = 0 ; i < TCP_SENDER_MAX_QUEUE ; i++){
    if (segment_queue[i].segment)
      continue;

    segment_queue[i].segment = segment_copy;
    segment_queue[i].data_size = data_size;
    segment_queue[i].ip = ip;
    memcpy (mac, segment_queue[i].mac, 6);
    segment_queue[i].tick = 0;
    soft_unblock_task (tcp_sender_pid);
    break;
  }
  unlock_irq();
}

uint8_t tcp_sender_task (uint32_t argc, uint8_t *argp)
{
  task_entry ();

  uint32_t min_tick;
  struct tcp_segment *segment;
  uint16_t port;
  uint32_t sequence_number;
  struct tcp_port_table_entry *port_struct;
  struct tcp_send_sliding_window * window;
  struct tcp_sender_queue_entry *curr_entry;
  uint32_t new_min_tick;
  uint32_t data_remainder = 0;
  uint32_t last_possible_byte;
  uint32_t last_data_byte;

  for (uint32_t i ; i < TCP_SENDER_MAX_QUEUE ; i++)
    segment_queue[i].segment = 0;

  while (1) {
    lock_irq ();
    min_tick = 0xffffffff;
    for (uint32_t i = 0 ; i < TCP_SENDER_MAX_QUEUE ; i++) {
      curr_entry = &segment_queue[i];
      segment = curr_entry->segment;

      if (!segment){
        continue;
      }

      if (curr_entry->tick >= tick)
        continue;


      port = get_bits_attr_value (segment->header, TCP_SOURCE_PORT_OFFSET, TCP_SOURCE_PORT_SIZE);
      sequence_number = get_bits_attr_value(segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE);

      port_struct = &tcp_port_table[port];
      window = port_struct->send_window;


      if (window->last_acked_byte >= sequence_number + curr_entry->data_size){
        kfree (segment->data, curr_entry->data_size);
        kfree (segment, sizeof(struct tcp_segment));
        curr_entry->segment = 0;
        continue;
      }

      last_possible_byte = window->last_acked_byte + window->window_size;
      last_data_byte = sequence_number + curr_entry->data_size;

      if (last_data_byte > last_possible_byte)
        data_remainder = last_data_byte - last_possible_byte;

      if (data_remainder){
        struct tcp_segment *remainder_segment = kmalloc_u (sizeof (struct tcp_segment));
        memcpy (segment, remainder_segment, sizeof (struct tcp_segment));

        remainder_segment->data = kmalloc_u (data_remainder);
        memcpy (segment->data+last_possible_byte, remainder_segment->data,data_remainder);

        set_bits_attr_value (remainder_segment->header, TCP_SEQUENCE_NUMBER_OFFSET, TCP_SEQUENCE_NUMBER_SIZE, sequence_number + last_possible_byte);

        tcp_sender_enqueue (remainder_segment, data_remainder, curr_entry->ip, curr_entry->mac);

      }

      uint32_t to_send_data_size = curr_entry->data_size - data_remainder;
      struct tcp_segment *to_send_segment = kmalloc_u (sizeof (struct tcp_segment));
      memcpy (segment, to_send_segment, sizeof (struct tcp_segment));

      to_send_segment->data = kmalloc_u (to_send_data_size);
      memcpy (segment->data, to_send_segment->data, to_send_data_size);

      set_bits_attr_value (to_send_segment->header, TCP_ACK_NUMBER_OFFSET, TCP_ACK_NUMBER_SIZE, port_struct->recv_window->last_acked_byte);

      struct pseudo_ip pseudo;
      pseudo_header_build (rtl8139_device->ip_addr, curr_entry->ip, 0x0, IPv4_PROTOCOL_TCP, to_send_data_size + TCP_HEADER_MIN_SIZE, &pseudo);
      uint16_t checksum = 0;
      set_bits_attr_value (to_send_segment->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);
      checksum = internet_checksum (to_send_segment->header, TCP_HEADER_MIN_SIZE, to_send_segment->data, to_send_data_size, pseudo.header, PSEUDO_HEADER_SIZE);
      set_bits_attr_value (to_send_segment->header, TCP_CHECKSUM_OFFSET, TCP_CHECKSUM_SIZE, checksum);

      kfree (curr_entry->segment->data, curr_entry->data_size);
      kfree (curr_entry->segment, sizeof(struct tcp_segment));

      curr_entry->segment = to_send_segment;
      curr_entry->data_size = to_send_data_size;
      curr_entry->tick = 100 + tick;

      uint8_t *tcp_array = tcp_to_array (to_send_segment, to_send_data_size);

      l3_upper_interface (curr_entry->ip, curr_entry->mac, tcp_array, to_send_data_size + TCP_HEADER_MIN_SIZE, L3_PROTOCOL_IPv4, 0, 0, IPv4_PROTOCOL_TCP);
      kfree (tcp_array, TCP_HEADER_MIN_SIZE + to_send_data_size);

      min_tick = min (min_tick, curr_entry->tick);
    }
    unlock_irq ();
    sleep_until (min_tick);

  }

  task_termination (argc, argp);
}

void tcp_sender_init ()
{
  struct tcb *tcp_sender = create_task (tcp_sender_task,
      "tcp_sender_task", READY_TO_RUN, 0, 0);
  tcp_sender_pid = tcp_sender->pid;
}
