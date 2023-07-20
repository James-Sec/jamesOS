#include <network_handler_task.h>

struct nht_recv_queue_entry nht_recv_queue [NHT_RECV_QUEUE_SIZE];
int32_t nht_curr_recv = 0;
int32_t nht_curr_read = 0;
struct tcb* nht_recv_tcb;

void nht_enqueue_recv_frame (uint8_t *frame, uint32_t frame_size)
{
  uint32_t queue_pos = (nht_curr_recv + 1) % NHT_RECV_QUEUE_SIZE; 

  if (queue_pos == nht_curr_read) {
    kprint ("EU NAO POSSO ESTAR PASSANDO AQUI\n");
    kfree (frame, frame_size);
    return;
  }

  nht_recv_queue [nht_curr_recv].frame = frame;
  nht_recv_queue [nht_curr_recv].frame_size = frame_size;
  nht_curr_recv = queue_pos;
  kprint ("LALAL\n");
  soft_unblock_task (nht_recv_tcb->pid);
}



void nht_recv_handler (uint32_t argc, uint8_t *argp)
{
  task_entry();

  while (1) {
    lock_irq();
    if (nht_curr_read == nht_curr_recv){
      kprint ("to blocando aqui\n");
      block_task (BLOCKED, 0);
      continue;
    }
    unlock_irq();

    kprintf ("MESSAGE RECEIVED: %d\n", 1, nht_recv_queue[nht_curr_read].frame_size);
    l2_lower_interface (nht_recv_queue[nht_curr_read].frame,
        nht_recv_queue[nht_curr_read].frame_size, L2_PROTOCOL_ETHERNET2);
    kfree (nht_recv_queue[nht_curr_read].frame,
          nht_recv_queue[nht_curr_read].frame_size);
    nht_curr_read = (nht_curr_read + 1) % NHT_RECV_QUEUE_SIZE;
    kprint ("to saindo\n");
  }
  
  task_termination(argc, argp);
}


void nht_init ()
{
   kprint ("creating nht_recv_task\n");
   nht_recv_tcb = create_task (nht_recv_handler,
       "nht_recv_hander", READY_TO_RUN, 0, 0);
}
