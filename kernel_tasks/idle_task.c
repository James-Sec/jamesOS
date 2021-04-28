#include <idle_task.h>

uint8_t idle_task_function ()
{
  task_entry ();
  while (1)
  {
    //kprintf ("[%s] executing...\n", 1, current_task->pname);
    asm volatile ("hlt");
  }
}
