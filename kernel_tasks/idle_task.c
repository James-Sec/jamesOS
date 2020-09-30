#include "../include/kernel_tasks.h"
extern void unlock_irq ();
uint32_t esp;
uint32_t ebp;
uint32_t *ptr;

uint32_t idle_task_function ()
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

