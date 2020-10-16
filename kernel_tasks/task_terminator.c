#include "../include/task_terminator.h"

extern struct tcb* head;

void task_terminator ()
{
  unlock_irq();

  struct tcb* tmp;
  struct tcb* prev;
  while (1)
  {
    uint8_t flag = 0;
    tmp = head;
    while (tmp)
    {
      if (tmp->state == TERMINATED)
      {
        flag = 1;
        if (tmp == head)
          head = tmp->next_task;
        else
          prev->next_task = tmp->next_task;
        kfree (0x1000, tmp->initial_addr);
        break;
      }
      prev = tmp;
      tmp = tmp->next_task;
    }
    struct tcb* i;
    for (i = head; i!= 0; i = i->next_task)
     //kprintf ("************%d, %d************\n", 2, i->pid, i->state);
    if (!flag)
      sleep (5);
  }
}
