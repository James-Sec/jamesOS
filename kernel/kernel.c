#include <stdint.h>
#include "../include/vga.h"
#include "../include/isr.h"
#include "../include/keyboard.h"
#include "../include/kheap.h"
#include "../include/paging.h"
#include "../include/rtl8139.h"
#include "../include/multitask.h"
#include "../include/pit.h"
#include "../include/task_entry.h"
#include "../include/ethernet.h"

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


  asm volatile ("cli");
  multitask_init ();

  //create_task (task_entry, "ANDERSON", READY_TO_RUN);
  //create_task (task_entry, "CAROLINA", READY_TO_RUN);
  asm volatile ("sti");
  //task_function ();
  
  rtl8139_init ();
  uint8_t dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
  uint8_t type [] = {0x08, 0x00};
  rtl8139_send_frame (build_ether_frame (dest_addr, type, "JAMES", 5));
}
