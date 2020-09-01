#include "../include/kernel_tasks.h"
extern void unlock_irq ();

uint8_t idle_task_function ()
{
  asm volatile ("sti");
  //unlock_irq ();
  while (1)
  {
    asm volatile ("hlt");
    //kprint ("idle\n");
  }
}
