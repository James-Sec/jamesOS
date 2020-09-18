#include "../include/multitask.h"
#include "../include/paging.h"
extern struct tcb* head;

void task_terminator ()
{
  unlock_irq();
  
  struct tcb* tmp;
  struct tcb* prev;
  while (1)
  {
    tmp = head;
    while (tmp)
    {
      if (tmp->state == TERMINATED)
      {
        free_frame (get_page (tmp->esp, 0, tmp->page_dir));
        if (tmp == head)
          head = tmp->next_task;
        else
        {
          prev->next_task = tmp->next_task;
        }
        kfree (1024, tmp);
      }
      prev = tmp;
      tmp = tmp->next_task;
    }
    struct tcb* i;
    for (i = head; i!= 0; i = i->next_task)
     kprintf ("************%d, %d************\n", 2, i->pid, i->state);
    sleep (5);
  }

}
