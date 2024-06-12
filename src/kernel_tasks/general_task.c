#include <general_task.h>

uint8_t general_task_function (uint32_t argc, uint8_t *argp)
{
  task_entry ();
  while (1){
        //sleep (1);
      //kprintf("argc: %d, argp: %x\n", 2, argc, argp);
  }
  task_termination (argc, argp);
}
