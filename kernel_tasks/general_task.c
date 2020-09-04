#include "../include/kernel_tasks.h"
extern struct tcb* current_task;
extern struct tcb* idle_task;
extern uint32_t lock_irq_counter;

void task_function ()
{
  kprintf ("-----------%d-----------\n", 1, current_task->pid);
  print_task (idle_task);
  kprintf ("-----------%d-----------\n", 1, current_task->pid);
  sleep (current_task->pid * 5);

  lock_irq ();
  scheduler ();
  unlock_irq ();

  task_function ();
}
