#include <general_task.h>

uint8_t general_task_function ()
{
  task_entry ();
  uint8_t i;
  for (i = 0 ; i < 3; i++) {
    kprintf ("[%s] executing...\n", 1, current_task->pname);
    sleep (3);
  }
  task_termination ();

}
