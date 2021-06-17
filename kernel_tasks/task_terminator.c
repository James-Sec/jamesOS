#include <task_terminator.h>

void task_terminator (uint32_t argc, uint8_t *argp)
{
  task_entry ();
  struct tcb* tmp;
  struct tcb* prev;
  uint8_t flag = 0;
  while (1)
  {
    flag = 0;
    tmp = head;
    lock_irq();
    while (tmp)
    {
      if (tmp->state == TERMINATED)
      {
        kprintf ("[%s (%x)] exterminating %s (%x)...\n", 4, current_task->pname, current_task, tmp->pname, tmp);
        flag = 1;
        if (tmp == head)
          head = tmp->next_task;
        else
          prev->next_task = tmp->next_task;
        kfree (tmp, 0x1000);
        //kprintf ("[%s] done.\n", 1, current_task->pname);
        break;
      }
      prev = tmp;
      tmp = tmp->next_task;
    }
    unlock_irq();
    if (!flag)
      sleep (5);
  }
}
