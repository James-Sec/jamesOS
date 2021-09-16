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
#include <udp.h>
#include <l2_interface.h>
#include <l3_interface.h>
#include <l4_interface.h>
#include <l5_interface.h>
#include <network_utils.h>


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

  serial_init ();

  asm volatile ("cli");
  multitask_init ();
  asm volatile ("sti");


  /*
     kprint ("creating carolina\n");
     uint8_t* b = kmalloc_u(4);
   *b = 37;
   struct tcb* carolina = create_task (general_task_function, "CAROLINA", READY_TO_RUN, 4, b);

   kprint ("creating anderson\n");
   uint8_t* argp = kmalloc_u(4);
   *argp = 0x7;
   struct tcb* anderson = create_task (general_task_function, "ANDERSON", READY_TO_RUN, 4, argp);

  //kprintf ("carolina argp: %x, anderson argp: %x\n", 2, );
  asm volatile ("sti");

  uint8_t mac_dest_addr [] = {0xa4,0x63,0xa1,0x53,0x9d,0x6a};
  uint8_t *james = "JAMES";
  l3_upper_interface (0x05060708, mac_dest_addr, james, 5, L3_PROTOCOL_IPv4, IPv4_DSCP_DF, 0, 6);
  //l2_upper_interface (mac_dest_addr, james, 5, L2_PROTOCOL_ETHERNET2, ETHER_TYPE_IPv4);

  */

  /*
  uint8_t mac_dest_addr [] = {0x0a,0x0f,0x70,0xf4,0xa9,0x8b};
  uint8_t *james = "JAMES";
  l3_upper_interface (0x1e1e1e1e, mac_dest_addr, james, 5, L3_PROTOCOL_IPv4, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_UDP);
  uint8_t h[8] = {0x08, 0x00, 0x00, 0x00, 0x24, 0xcb, 0x00, 0x01};
  uint8_t d[56] = {0x73, 0x8c, 0xc2, 0x60, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x64, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};
  uint16_t y = internet_checksum(h, 8, d, 56);
  kprintf ("CHECKSUM: %x\n", 1, y);

  uint8_t mac_dest_addr [] = {0x0a,0x0f,0x70,0xf4,0xa9,0x8b};
  uint8_t *j = "jamesjamesjamesjamesjamesjamesjamesjamesjamesjame";
  uint16_t port = 5555;
  uint32_t length = 50;
  htons (&port);
  l4_upper_interface (port, 0x1e1e1e1e, mac_dest_addr, j, length, L4_PROTOCOL_UDP, port);

  uint8_t mac_dest_addr [] = {0x0a,0x0f,0x70,0xf4,0xa9,0x8b};
  uint8_t *james = "JAMES";
  l3_upper_interface (0x1e1e1e1e, mac_dest_addr, james, 5, L3_PROTOCOL_ICMP4, 0, 8, 1);
  */
  
  /*
  uint16_t size =  UDP_HEADER_SIZE + 50;
  htons (&port);
  htons (&size);
  build_udp_segment (udp, port, port, size, j, 50);


  uint8_t *array = udp_to_array (udp, 50);


  l3_upper_interface (0x1e1e1e1e, mac_dest_addr, array, UDP_HEADER_SIZE + 50, L3_PROTOCOL_IPv4, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_UDP);

  serial_send_string("james\n\x00");
  */

  /*
  uint8_t* data = kmalloc_u (100);
  struct net_address_set* dest_addresses;
  int32_t receive_port = udp_port_bind(5555, data, &dest_addresses);
  htons(&receive_port);
  while(1)
  {
    task_receive_udp ();
    kprintf("udp segment content: %s\n", 1, data);
    uint8_t* mac = dest_addresses->mac;
    uint32_t ip = dest_addresses->ip;
    uint16_t port = dest_addresses->port;
    l4_upper_interface (port, ip, mac, data, 100, L4_PROTOCOL_UDP, receive_port);
  }
  udp_port_unbind (receive_port);
  kfree (data, 100);
  */

  uint32_t ip = 0x1e1e1e1e;
  uint16_t port = 5555;
  htons (&port);
  uint8_t mac[6] = {0x9a, 0xed, 0x4d, 0x50, 0xd3, 0x8f};
  uint8_t* data = "james\n";
  l5_upper_interface (port, ip, mac, data, 6, L5_PROTOCOL_JNP, L4_PROTOCOL_UDP);
  jnp_recv_message (5555);


  task_termination (0, 0);
}
