#include "../include/kernel_tasks.h"
#include "../include/pit.h"
extern struct tcb* current_task;
extern struct tcb* idle_task;
extern uint32_t lock_irq_counter;

void task_function ()
{
  while (1) {
    kprintf ("-----------%d-----------\n", 1, current_task->pid);
    print_stack_asm (1, 0x1c);
    kprintf ("-----------%d-----------\n", 1, current_task->pid);
    task_termination ();
  }
}
