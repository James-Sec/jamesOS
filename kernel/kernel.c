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
extern uint8_t task_entry ();
extern struct tcb* head;
extern uint32_t lock_irq_counter;
uint32_t count;

extern void task_terminator ();

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

  create_task (task_entry, "ANDERSON", READY_TO_RUN);
  create_task (task_entry, "CAROLINA", READY_TO_RUN);
  create_task (task_terminator, "CHUAZNEGUER", READY_TO_RUN);
  asm volatile ("sti");
  task_function ();
}
