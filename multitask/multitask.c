#include "../include/multitask.h"

struct tcb *head;
struct tcb *current_task;
uint32_t last_pid = 1;
extern struct page_directory_t *current_directory;
uint32_t lock_irq_counter = 0;
uint32_t multitasking_on;
extern uint32_t tick;

void multitask_init () 
{
  head = (struct tcb *) kmalloc (1024);
  current_task = head;

  asm volatile ("mov %%esp, %0" : "=r" (head->esp));
  head->page_dir = current_directory;
  head->next_task = 0;
  head->pid = 1;
  head->state = RUNNING;
  char *s = "init";
  memcpy (s, head->pname, 4);

	multitasking_on = 1;
}

struct tcb* create_kernel_task (uint32_t* esp, uint8_t (*func) (void), char *pname)
{
  struct tcb *new_task = (struct tcb*) kmalloc (1024);
  new_task->esp = esp;
  new_task->page_dir = current_directory;
  new_task->next_task = head;
  new_task->pid = ++last_pid;
  new_task->state = READY_TO_RUN;
  memcpy (pname, new_task->pname, 32);
  
  head = new_task;
  
  //organizing the stack
  *esp = esp + 4; //ebp
  *(esp + 1) = 0; //edi
  *(esp + 2) = 0; //esi
  *(esp + 3) = 0; //ebx
  *(esp + 4) = func; //ret
  
  return new_task;
}

void block_task (uint8_t reason)
{
  lock_irq ();
  current_task->state = reason;
  scheduler ();
  unlock_irq ();
}

void unblock_task (uint32_t pid)
{
  lock_irq ();
  struct tcb *tmp;
  tmp = search_task (pid);
  current_task->state = READY_TO_RUN;
  tmp->state = RUNNING;
  task_switch (tmp);
  unlock_irq ();
}

void lock_irq ()
{
  asm volatile ("cli");
  ++lock_irq_counter;
}

void unlock_irq ()
{
  if (--lock_irq_counter == 0)
    asm volatile ("sti");
}


// ticks is in seconds
void sleep (uint32_t ticks)
{
	sleep_until ((ticks * 100) + tick);
}

void sleep_until (uint32_t ticks)
{

	// if time has already passed
	if (ticks < tick)
		return;

	// set the time that the task should wake up
	current_task->sleep_until = ticks;

	//current_task->state = SLEEPING;
	block_task (SLEEPING);
}

void scheduler () 
{
  struct tcb* next;
  if (current_task->state != BLOCKED && current_task->state != SLEEPING)
    current_task->state = READY_TO_RUN;
  next = current_task->next_task;
  while (1)
  {
    if (!next)
      next = head;
    if (next->state == BLOCKED || next->state == SLEEPING)
      next = next->next_task;
    else 
      break;
  }
  next->state = RUNNING;
  task_switch (next);
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
  kprintf ("\ncurrent state: %d", 1, tcb->state);

  struct tcb *it = current_task->next_task;
  kprintf ("\ncurrent task->next_task: %d\n", 1, head);
  //kprint ("\n--------------\n");
}

struct tcb* search_task (uint32_t pid)
{
  struct tcb* it;
  for (it = head; it != 0; it = it->next_task)
    if (it->pid == pid)
      return it;
  return 0;
}
