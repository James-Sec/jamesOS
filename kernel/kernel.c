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
#include "../include/ip.h"

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


	struct ip_packet* ip = build_ipv4_packet ("jaaames", 7);

	kprint ("ip header: \n");
	int i = 0;
	for (; i < 20; i++)
		kprintf ("%x ", 1, *((uint8_t*)ip + i));
	kprint ("\n");

  struct ether_frame* frame = build_ether_frame (dest_addr, type, (uint8_t*) ip, 200);
  rtl8139_send_frame (frame);
}
