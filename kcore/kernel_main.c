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
#include <l2_interface.h>
#include <l3_interface.h>

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

  uint8_t *james = "JAMES";
  l3_upper_interface (0x05060708, mac_dest_addr, james, 5, L3_PROTOCOL_IPv4);
  */

  uint8_t mac_dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
  uint8_t *james = "JAMES";
  l3_upper_interface (0x10101010, mac_dest_addr, james, 5, L3_PROTOCOL_ICMP4, ICMP4_ECHO_REQUEST_TYPE, ICMP4_ECHO_REQUEST_CODE, 0);
  task_termination ();
}
