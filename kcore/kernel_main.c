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
#include <arp.h>

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

  asm volatile ("cli");
  multitask_init ();

  //create_task (task_entry, "ANDERSON", READY_TO_RUN);
  //create_task (task_entry, "CAROLINA", READY_TO_RUN);
  asm volatile ("sti");

  //task_function ();

	// sending a ipv4 packet
  /*
	char* data = "jaaames";
	struct ip_packet* ip = build_ipv4_packet (0x01020304, data, 7);
  uint8_t mac_dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
	send_ipv4_packet (ip, mac_dest_addr);
  */

  // sending arp request
  uint8_t ipp[4] = "\x01\x02\x03\x04";
  send_arp_request (ipp);
}
