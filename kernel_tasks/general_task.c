#include "../include/kernel_tasks.h"
extern struct tcb* current_task;

void task_function ()
{

  kprintf ("%d\n", 1, current_task->pid);
  sleep (current_task->pid * 5);

  lock_irq ();
  scheduler ();
  unlock_irq ();

  task_function ();
}
