#ifndef MULTITASK_H
#define MULTITASK_H

#include "paging.h"
#include "vga.h"
#include "mem.h"

struct tcb
{
  uint8_t *esp;
  struct page_directory_t* page_dir;
  //struct tcb* next_task;
  uint32_t pid;
  char pname [32];
}__attribute__ ((packed));


// TCB LINKED LIST MANAGER
struct tcb_manager
{
  struct tcb* ready_to_run [128];
	struct tcb* not_ready [128];

  uint8_t top_nr;
  uint8_t current_nr;

  uint8_t top_rtr;
  uint8_t current_rtr;
}__attribute__ ((packed));

void multitask_init ();
void print_task (struct tcb*);
struct tcb* create_kernel_task (uint32_t* esp, uint8_t (*func)(void), char *pname);
void schedule ();
void change_state ();
void to_nr ();

#endif
