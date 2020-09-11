#include "../include/kernel_tasks.h"
extern void unlock_irq ();
uint32_t esp;
uint32_t ebp;
uint32_t *ptr;

void test ()
{
  asm volatile ("mov %%esp, %0" : "=r"(ptr));//0x16fff8
  print_stack_c (ptr, *ptr);
}

//should be 0x170000
//but why the fuck it is 0x16fff8?
//seems like there is a fcking add esp, 8
uint32_t idle_task_function ()
{
  //C - ASM 0  ->   4 
  //C - ASM 1-4 -> 4  + 16 
  //C - C 0 -> 16
  //C - C 1-4 -> 16 + 16
  //
  //ASM - ASM 0 -> 4
  //ASM - ASM N -> 4 + N
  //ASM - C 0 -> 16
  //ASM - C N -> 16 + N
  //test ();


  asm volatile ("sti");
  while (1)
  {
    kprint ("-----------idle-----------\n");
    print_stack_asm (1, 0x1c);
    kprint ("-----------idle-----------\n");

    uint32_t i = 0;
    for (i = 0; i < 1e9; i++);

    asm volatile ("hlt");
    //kprint ("idle\n");
  }
}

