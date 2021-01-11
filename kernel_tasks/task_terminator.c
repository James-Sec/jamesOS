#include <task_terminator.h>

void task_terminator ()
{
  task_entry ();
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
        kprintf ("[%s] exterminating %s...\n",2, current_task->pname, tmp->pname);
        flag = 1;
        if (tmp == head)
          head = tmp->next_task;
        else
          prev->next_task = tmp->next_task;
        kfree (0x1000, tmp);
        kprintf ("[%s] done.\n", 1, current_task->pname);
        break;
      }
      prev = tmp;
      tmp = tmp->next_task;
    }
    if (!flag)
      sleep (10);
  }
}
