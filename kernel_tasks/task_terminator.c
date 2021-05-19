#include <task_terminator.h>

void task_terminator (uint32_t argc, uint8_t *argp)
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
        //kprintf ("[%s] exterminating %s...\n",2, current_task->pname, tmp->pname);
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
    if (!flag)
      sleep (10);
  }
}
