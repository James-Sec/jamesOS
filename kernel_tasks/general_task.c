#include <general_task.h>

uint8_t general_task_function (uint32_t x)
{
  task_entry ();
  uint8_t i;
  kprintf("PARAMETER: %x\n", 1, x);
  /*
  for (i = 0 ; i < 3; i++) {
    kprintf ("[%s] executing...\n", 1, current_task->pname);
    sleep (3);
  }
  */
  while (1);
  task_termination ();

}
