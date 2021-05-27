#include <general_task.h>

uint8_t general_task_function (uint32_t argc, uint8_t *argp)
{
  task_entry ();
  for (int i = 0 ; i < 3; i++)
  {
    kprintf("argc: %d, argp: %x\n", 2, argc, argp);
    sleep (3);
  }
  /*
  for (i = 0 ; i < 3; i++) {
    kprintf ("[%s] executing...\n", 1, current_task->pname);
    sleep (3);
  }
  */
  task_termination (argc, argp);
}
