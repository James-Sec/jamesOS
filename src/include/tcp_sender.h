#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include <stdint.h>
#include <tcp.h>
#include <mem.h>
#include <multitask.h>

#define TCP_SENDER_MAX_QUEUE 1024


struct tcp_sender_queue_entry
{
  struct tcp_segment *segment;
  uint32_t data_size;
  uint32_t ip;
  uint8_t mac[6];
  uint32_t tick;
};

struct tcp_sender_queue_entry segment_queue [TCP_SENDER_MAX_QUEUE];
uint32_t tcp_sender_pid;

void tcp_sender_init ();
void tcp_sender_enqueue (struct tcp_segment *segment, uint32_t data_size, uint32_t ip, uint8_t mac[6]);
#endif
