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

//should be 0x170004
uint32_t idle_task_function ()
{
  asm volatile ("mov %%esp, %0" : "=r"(esp));
  kprintf ("inline esp: %x\n", 1, esp);

  //print_stack_asm (1, 0x1c);
  func_test ();
  
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


  //asm volatile ("sti");
  //unlock_irq ();
  while (1)
  {
    kprint ("-----------idle-----------\n");
    //print_esp_eip_asm ();
    kprint ("-----------idle-----------\n");
    int i;
    for (i = 0; i < 1e9; i++);
    asm volatile ("hlt");
    //kprint ("idle\n");
  }
}

