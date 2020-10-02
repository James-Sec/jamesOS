#include "../include/idle_task.h"

uint8_t idle_task_function ()
{
  asm volatile ("sti");
  while (1)
  {
    kprint ("-----------idle-----------\n");
//    print_stack_asm (1, 0x1c);
//    kprint ("-----------idle-----------\n");

    asm volatile ("hlt");
    //kprint ("idle\n");
    int i;
    for (i = 0; i < 1e8; i++);
  }
}

