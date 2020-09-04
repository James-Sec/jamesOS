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
  //func_test (1,2);
  asm volatile ("mov %%esp, %0" : "=r"(esp));//0x16fff8
  kprintf ("inline esp: %x\n", 1, esp);

  print_stack_asm (1, 0x1c);
  
  //C - ASM 0 parametros ->   8 (4 eip)
  //C - ASM 1-4 parametros -> 8(4 eip)  + 16 (parametros)
  //C - C 0 parametros -> 16
  //C - C 1-4 parametros -> 32
  //func_test (1);
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

