#include <stdint.h>
#include "../include/vga.h"
#include "../include/isr.h"
#include "../include/keyboard.h"
#include "../include/kheap.h"
#include "../include/paging.h"
#include "../include/rtl8139.h"
#include "../include/multitask.h"
#include "../include/pit.h"

extern struct tcb* current_task;
extern uint32_t task_switch (struct tcb* next_task);
uint32_t count;


void task_function ()
{
  kprint ("this is a new task: \n");
  print_task(0);
  int i;
  for (i = 0; i < 1e8; i++);

  task_switch (current_task->next_task);
  task_function ();
}


extern uint32_t function (struct tcb *s);

void entry ()
{
  clear_screen ();
  kprint ("Welcome to the James OS!\n\0");
  isr_install (); 
  asm volatile ("sti");

  keyboard_init();

  kheap_init ();  

  paging_init ();

  rtl8139_init ();
  char *msg = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  rtl8139_send_frame ((uint8_t*) msg, 64);

  kprint ("\n\n\n\n\n\n\n");

  //---------------------------------------------------

  asm volatile ("cli");
  multitask_init ();
  asm volatile ("sti");

  print_task (0);

  uint32_t *stack = 0x170000;
  struct tcb *nt = create_kernel_task (stack, task_function, "ola");

  task_switch (current_task->next_task);

  task_function ();


}
