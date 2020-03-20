#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/isr.h"
#include "../include/types.h"
#include "../include/timer.h"
#include "../include/memory_manager.h"
#include "../include/string.h"
#include "../include/pci.h"
#include "../include/paging.h"

void entry ()
{
  clear_screen();
  kprint ("Welcome to my OS!\n");
  kprint ("Welcome to JAMES\n");
  isr_install();
  asm volatile ("sti");
  init_keyboard();

  memory_manager_init();
  page_init();
  uint8_t* ptr = (uint8_t)0x120000;
  uint32_t a = *ptr;
  char s[10];
  itoa (a,s);
  kprint ("Testing virtual memory: ");
  kprint (s);
  kprint ("\n");
  //asm volatile ("int $14");
}

