#include <network_handler_task.h>

struct nht_queue_entry nht_recv_queue [NHT_RECV_QUEUE_SIZE];
struct nht_queue_entry nht_send_queue [NHT_SEND_QUEUE_SIZE];
int32_t nht_recv_push = 0;
int32_t nht_recv_read = 0;
int32_t nht_send_push = 0;
int32_t nht_send_read = 0;
struct tcb* nht_recv_tcb;
struct tcb* nht_send_tcb;

void nht_enqueue_recv_frame (uint8_t *frame, uint32_t frame_size)
{
  uint32_t queue_pos = (nht_recv_push + 1) % NHT_RECV_QUEUE_SIZE; 

  if (queue_pos == nht_recv_read) {
    kfree (frame, frame_size);
    return;
  }

  nht_recv_queue [nht_recv_push].frame = kmalloc_u (frame_size);
  memcpy(frame, nht_recv_queue [nht_recv_push].frame, frame_size);
  nht_recv_queue [nht_recv_push].frame_size = frame_size;
  nht_recv_push = queue_pos;
  soft_unblock_task (nht_recv_tcb->pid);
}

void nht_enqueue_send_frame (uint8_t *frame, uint32_t frame_size)
{
  uint32_t queue_pos = (nht_send_push + 1) % NHT_SEND_QUEUE_SIZE; 

  if (queue_pos == nht_send_read) {
    kfree (frame, frame_size);
    return;
  }

  nht_send_queue [nht_send_push].frame = kmalloc_u (frame_size);
  memcpy(frame, nht_send_queue [nht_send_push].frame, frame_size);
  nht_send_queue [nht_send_push].frame_size = frame_size;
  nht_send_push = queue_pos;
  soft_unblock_task (nht_send_tcb->pid);
}

void nht_recv_handler (uint32_t argc, uint8_t *argp)
{
  task_entry();

  while (1) {
    lock_irq();
    if (nht_recv_read == nht_recv_push){
      block_task (BLOCKED, 0);
      continue;
    }
    unlock_irq();

    l2_lower_interface (nht_recv_queue[nht_recv_read].frame,
        nht_recv_queue[nht_recv_read].frame_size, L2_PROTOCOL_ETHERNET2);
    kfree (nht_recv_queue[nht_recv_read].frame,
          nht_recv_queue[nht_recv_read].frame_size);
    nht_recv_read = (nht_recv_read + 1) % NHT_RECV_QUEUE_SIZE;
  }
  
  task_termination(argc, argp);
}

void nht_send_handler (uint32_t argc, uint8_t *argp)
{
  task_entry();

  while (1) {
    lock_irq();
    if (nht_send_read == nht_send_push){
      block_task (BLOCKED, 0);
      continue;
    }
    unlock_irq();

    l1_upper_interface (nht_send_queue[nht_send_read].frame,
        nht_send_queue[nht_send_read].frame_size, L1_RTL8139_ID);
    kfree (nht_send_queue[nht_send_read].frame,
          nht_send_queue[nht_send_read].frame_size);
    nht_send_read = (nht_send_read + 1) % NHT_SEND_QUEUE_SIZE;
  }

  task_termination(argc, argp);
}

void nht_init ()
{
   kprint ("creating nht_recv_task\n");
   nht_recv_tcb = create_task (nht_recv_handler,
       "nht_recv_hander", READY_TO_RUN, 0, 0);

   kprint ("creating nht_send_task\n");
   nht_send_tcb = create_task (nht_send_handler,
       "nht_send_hander", READY_TO_RUN, 0, 0);
}
