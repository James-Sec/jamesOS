#include <general_task.h>

uint8_t task_function ()
{
  while (1) {
    kprintf ("-----------%d-----------\n", 1, current_task->pid);
    task_termination ();
  }
}
