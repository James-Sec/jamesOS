#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/isr.h"
#include "../include/types.h"
#include "../include/timer.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/pci.h"
#include "../include/kheap.h"
#include "../include/paging.h"
#include "../include/rtl8139.h"

void entry ()
{
  clear_screen();
  kprint ("Welcome to my OS!\n");
  kprint ("Welcome to JAMES\n");
  isr_install();
  asm volatile ("sti");
  init_keyboard();
  kheap_init();
  page_init();
  rtl8139_init();
  ethernet_send_packet (0xffff, "ola",3);
}

