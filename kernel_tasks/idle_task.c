#include "../include/kernel_tasks.h"

uint8_t idle_task_function ()
{
  asm volatile ("sti");
  while (1)
    asm volatile ("hlt");
}
