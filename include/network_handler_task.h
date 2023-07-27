#ifndef NETWORK_HANDLER_TASK_h
#define NETWORK_HANDLER_TASK_h

#include <stdint.h>
#include <multitask.h>
#include <l2_interface.h>
#include <l1_interface.h>

#define NHT_RECV_QUEUE_SIZE 1024
#define NHT_SEND_QUEUE_SIZE 1024

struct nht_queue_entry {
  uint8_t *frame;
  uint32_t frame_size;
};

void nht_enqueue_recv_frame (uint8_t *frame, uint32_t frame_size);
void nht_enqueue_send_frame (uint8_t *frame, uint32_t frame_size);
void nht_init ();

#endif
