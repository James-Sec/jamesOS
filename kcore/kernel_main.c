#include <stdint.h>
#include <vga.h>
#include <isr.h>
#include <keyboard.h>
#include <kheap.h>
#include <paging.h>
#include <rtl8139.h>
#include <multitask.h>
#include <general_task.h>
#include <pit.h>
#include <ethernet.h>
#include <ip.h>
#include <arp.h>
#include <icmp.h>

uint8_t var;
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

  create_task (general_task_function, "ANDERSON", READY_TO_RUN);
  create_task (general_task_function, "CAROLINA", READY_TO_RUN);
  asm volatile ("sti");

  /*
	// sending a ipv4 packet
	uint8_t* data = "jaaames";
	struct ip_packet* ip = build_ipv4_packet (0x01020304, data, 7);
  uint8_t mac_dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
	send_ipv4_packet (ip, mac_dest_addr);

  // sending arp request
  uint32_t ipp = 0x07060504;
  send_arp_request (ipp);

  //sending a icmp4 packet
  uint32_t ipp = 0x07060504;
	uint8_t* data = "jaaames";
  uint8_t mac_dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
  struct icmp4* icmp = build_icmp4_packet (ICMP4_ECHO_REQUEST_TYPE, 0, 0xfff7, 0);
  icmp->data = data;
  send_icmp4_packet (icmp, ICMP4_HEADER_SIZE + 7, ipp, mac_dest_addr);
  */
  task_termination ();
}
