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
extern struct tcb* head;
uint32_t count;


void task_function ()
{
  kprint ("this is a new task: \n");
  print_task(0);


	// TESTS
	kprint ("pre sleep\n");
	if (current_task->pid != 1) {
		int i;
		for (i = 0; i < 1e9; i++);
	}
	if (current_task->pid != 1)
		sleep (3);
	else {
		int i;
		for (i = 0; i < 1e8; i++);
	}
	kprint ("post sleep\n");
  kprint ("\n--------------\n");
	if (current_task->pid != 1) {
		int i;
		for (i = 0; i < 1e8; i++);
	}
	// END TESTS

  lock_irq ();
  scheduler ();
  unlock_irq ();

  task_function ();
}

uint8_t startup ()
{
  unlock_irq ();
  task_function ();
  return 0;
}


extern uint32_t function (struct tcb *s);

void entry ()
{
  clear_screen ();
  kprint ("Welcome to the James OS!\n\0");
  isr_install (); 
  asm volatile ("sti");

  pit_init (0);

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


  create_kernel_task (0x120000, startup, "bonito");
  create_kernel_task (0x120000 - 0x1000, startup, "eh");
  create_kernel_task (0x120000 - 0x2000, startup, "joao");

  task_function ();
}
