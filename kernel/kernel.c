#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/isr.h"
#include "../include/types.h"
#include "../include/timer.h"
#include "../include/memory_manager.h"
#include "../include/string.h"
#include "../include/pci.h"

void entry ()
{
  clear_screen();
  kprint ("Welcome to my OS!\n");
  isr_install();
  asm volatile ("sti");
  //init_timer(100);
  init_keyboard();
  //asm volatile ("int $1");
  char s[10];
  itoa ((int) s, s);
  kprint (s);

  memory_manager_init();

  pci_brute ();

  /*
  uint8_t* ptr = kmalloc (160);
  kfree (7, ptr - 2);
  kfree (3, ptr - 80);
  kfree (1, ptr - 158);

  kprint ("\n");


  uint8_t *map = BITSET_BASE;
  int i;
  for (i = 0; i < 20; i++) {
    int j;
    for (j = 7; j >= 0; j--) {
      if ((*(map - i) >> j) & 1)
        kprint ("1");
      else
        kprint ("0");
    }
  }
  kprint ("\n");
  */


}

