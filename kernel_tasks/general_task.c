#include <general_task.h>

uint8_t general_task_function (uint32_t argc, uint8_t *argp)
{
  task_entry ();
    kprintf("argc: %d, argp: %x\n", 2, argc, argp);
  task_termination (argc, argp);
}
