#ifndef MULTITASK_H
#define MULTITASK_H

#include "paging.h"
#include "vga.h"
#include "mem.h"

#define BLOCKED 0
#define RUNNING 1
#define READY_TO_RUN 2

struct tcb
{
  uint8_t *esp;
  struct page_directory_t* page_dir;
  struct tcb* next_task;
  uint32_t pid;
  char pname [32];
  uint8_t state;
}__attribute__ ((packed));

void multitask_init ();
void print_task (struct tcb*);
struct tcb* create_kernel_task (uint32_t* esp, uint8_t (*func)(void), char *pname);
struct tcb* search_task (uint32_t pid);

#endif