#include <stdint.h>
#include "../include/vga.h"
#include "../include/isr.h"
#include "../include/keyboard.h"
#include "../include/kheap.h"
#include "../include/paging.h"
#include "../include/rtl8139.h"
#include "../include/multitask.h"
#include "../include/pit.h"

//extern struct tcb* current_task;
extern uint32_t task_switch (struct tcb* next_task);

//TCB MANAGER
extern struct tcb_manager mtmg;


extern uint32_t function (struct tcb *s);

uint8_t task_function () {
	while (1) {
		//kprintf ("%d\n", 1, mtmg.ready_to_run [mtmg.current_rtr]->pid);
	}

	return 0;
}

void entry ()
{
  clear_screen ();
  kprint ("Welcome to the James OS!\n\0");
  isr_install (); 
  asm volatile ("sti");

  pit_init (10);

  keyboard_init();

  kheap_init ();  

  paging_init ();

  rtl8139_init ();
  char *msg = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  rtl8139_send_frame ((uint8_t*) msg, 64);

  kprint ("\n");

  //---------------------------------------------------

  asm volatile ("cli");
  multitask_init ();

  //int i;
  //for (i = 0; i < 10; i++)
  create_kernel_task ((0x120000) + (0 * 0x1000), task_function, "aa");
  create_kernel_task ((0x120000) + (1 * 0x1000), task_function, "aa");
  create_kernel_task ((0x120000) + (2 * 0x1000), task_function, "aa");
  asm volatile ("sti");


	//schedule ();

  task_function ();
  //int i;
  //for (i = 0; i < 10; i++)
}
