#include "../include/multitask.h"

struct tcb *init;
struct tcb *current_task;
uint32_t last_pid = 1;
extern struct page_directory_t *current_directory;
uint32_t count = 0;


void multitask_init () 
{
  init = (struct tcb *) kmalloc (1024);
  current_task = init;

  asm volatile ("mov %%esp, %0" : "=r" (init->esp));
  init->page_dir = current_directory;
  init->next_task = init;
  init->pid = 1;
  char *s = "init";
  memcpy (s, init->pname, 4);
}

struct tcb* create_kernel_task (uint32_t* esp, uint8_t (*func) (void), char *pname)
{
  struct tcb *new_task = (struct tcb*) kmalloc (1024);
  new_task->esp = esp;
  new_task->page_dir = current_directory;
  new_task->next_task = init;
  new_task->pid = ++last_pid;
  memcpy (pname, new_task->pname, 32);
  current_task->next_task = new_task;
  
  //organizing the stack

  *esp = esp + 4; //ebp
  *(esp + 1) = 0; //edi
  *(esp + 2) = 0; //esi
  *(esp + 3) = 0; //ebx
  *(esp + 4) = func; //ret
  

  return new_task;
}

void print_task (struct tcb* tcb)
{
  if (!tcb)
    tcb = current_task;
  kprint ("task info: \nname: ");
  kprint (tcb->pname);
  kprint ("\npid: ");
  char s [10];
  itoa ((uint32_t)tcb->pid, s);
  kprint (s);
  kprint ("\nesp: ");
  itoa ((uint32_t)tcb->esp, s);
  kprint (s);
  kprint ("\nnext_task: ");
  itoa ((uint32_t)tcb->next_task, s);
  kprint (s);
  kprint ("\npage_directory: ");
  itoa ((uint32_t)tcb->page_dir, s);
  kprint (s);
  kprint ("\n--------------\n");
}
