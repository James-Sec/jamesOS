#include <stdint.h>
#include <vga.h>
#include <isr.h>
#include <keyboard.h>
#include <kheap.h>
#include <paging.h>
#include <rtl8139.h>
#include <multitask.h>
#include <pit.h>
#include <task_entry.h>
#include <ethernet.h>
#include <ip.h>

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


	struct ip_packet* ip = build_ipv4_packet ("jaaames", 7);

	kprint ("ip header: \n");
	int i = 0;
	for (; i < 20; i++)
		kprintf ("%x ", 1, *((uint8_t*)ip + i));
	kprint ("\n");

  //struct ether_frame* frame = build_ether_frame (dest_addr, type, (uint8_t*) ip, 200);
  //rtl8139_send_frame (frame);

  send_ether_frame (dest_addr, ETHER_TYPE_IPV4, (uint8_t*) ip, 200);
}
