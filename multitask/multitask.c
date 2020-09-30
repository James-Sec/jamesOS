#include "../include/multitask.h"

struct tcb *head;
struct tcb *current_task;
struct tcb *idle_task;
uint32_t last_pid = 1;
extern struct page_directory_t *current_directory;
extern struct page_directory_t *kernel_directory;
uint32_t lock_irq_counter = 0;
uint32_t multitasking_on;
uint32_t ready_to_run_counter;
extern uint32_t tick;
extern uint8_t idle_task_function ();


struct tcb* _create_task (struct page_directory_t *page_dir, struct tcb *next_task, uint32_t pid, uint8_t state, char *pname, uint8_t (*func) (void))
{
  struct tcb *new_task = (struct tcb*) kmalloc (0x1000);
  new_task->initial_addr = new_task;
  new_task->ebp = ((uint32_t)new_task + 0x1000) - sizeof (struct tcb);
  new_task->esp = new_task->ebp - 4;
  new_task->page_dir = page_dir;
  new_task->next_task = next_task;
  new_task->pid = pid;
  new_task->state = state;
  memcpy (pname, new_task->pname, 32);
  
  //organizing the stack
  *new_task->esp = new_task->ebp; //ebp
  *(new_task->esp + 1) = 0; //edi
  *(new_task->esp + 2) = 0; //esi
  *(new_task->esp + 3) = 0; //ebx
  *(new_task->esp + 4) = func; //ret
  
  return new_task;
}
  
struct tcb* create_task (uint8_t (*func) (void), char *pname, uint8_t state)
{
  if (state == IDLE)
    return idle_task =  _create_task (kernel_directory, 0, 0, state, pname, func);
  ++ready_to_run_counter;
  return head = _create_task (kernel_directory, head, ++last_pid, state, pname, func);

}

struct tcb* create_idle_task (uint8_t (*func) (void), char *pname)
{
  idle_task = (struct tcb*) kmalloc (0x1000);
  idle_task->initial_addr = idle_task;
  idle_task->ebp = ((uint32_t)idle_task + 0x1000) - sizeof (struct tcb);
  idle_task->esp = idle_task->ebp - 4;
  idle_task->page_dir = current_directory;
  idle_task->next_task = 0;
  idle_task->pid = 0;
  idle_task->state = IDLE;
  memcpy (pname, idle_task->pname, 32);
  
  
  //organizing the stack
  *idle_task->esp = idle_task->ebp; //ebp
  *(idle_task->esp + 1) = 9; //edi
  *(idle_task->esp + 2) = 3; //esi
  *(idle_task->esp + 3) = 7; //ebx
  *(idle_task->esp + 4) = func; //ret
  
  return idle_task;
}

void multitask_init () 
{
  head = (struct tcb *) kmalloc (1024);
  current_task = head;

  asm volatile ("mov %%esp, %0" : "=r" (head->esp));
  head->page_dir = current_directory;
  head->next_task = 0;
  head->pid = 1;
  head->state = RUNNING;
  char *s = "JAMES";
  memcpy (s, head->pname, 4);

  //create_idle_task (idle_task_function, "IDLE");
  create_task (idle_task_function, "IDLE", IDLE);
	multitasking_on = 1;
  ++ready_to_run_counter;
}


/*
struct tcb* create_kernel_task (uint8_t (*func) (void), char *pname)
{
  struct tcb *new_task = (struct tcb*) kmalloc (0x1000);
  new_task->initial_addr = new_task;
  new_task->ebp = ((uint32_t)new_task + 0x1000) - sizeof (struct tcb);
  new_task->esp = new_task->ebp - 4;
  new_task->page_dir = current_directory;
  new_task->next_task = head;
  new_task->pid = ++last_pid;
  new_task->state = READY_TO_RUN;
  memcpy (pname, new_task->pname, 32);
  
  head = new_task;
  
  //organizing the stack
  *new_task->esp = new_task->ebp; //ebp
  *(new_task->esp + 1) = 9; //edi
  *(new_task->esp + 2) = 3; //esi
  *(new_task->esp + 3) = 7; //ebx
  *(new_task->esp + 4) = func; //ret
  
  ++ready_to_run_counter;
  return new_task;
}
*/


void block_task (uint8_t reason)
{
  lock_irq ();
  --ready_to_run_counter;
  current_task->state = reason;
  scheduler ();
  unlock_irq ();
}

void unblock_task (uint32_t pid)
{
  lock_irq ();
  ++ready_to_run_counter;
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

	// current_task->state = SLEEPING;
	// the task call scheduler
	block_task (SLEEPING);
}

void scheduler () 
{
  struct tcb* next;

  if (!ready_to_run_counter)
    task_switch (idle_task);

	// if the running task wasnt blocked os "sleeped"
	// set to the READY_TO_RUN list
  if (current_task->state != BLOCKED && current_task->state != SLEEPING && current_task->state != TERMINATED)
    current_task->state = READY_TO_RUN;
	// search for next available task
  next = current_task->next_task;
  while (1)
  {
    if (!next)
      next = head;
    if (next->state == BLOCKED || next->state == SLEEPING || next->state == TERMINATED)
      next = next->next_task;
    else 
      break;
  }
  next->state = RUNNING;
  task_switch (next);
}

void task_termination ()
{
  kprintf ("TERMINATING %d...\n", 1, current_task->pid);
  asm volatile ("cli");
  --ready_to_run_counter;
  current_task->state = TERMINATED;
  scheduler ();
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
  kprintf ("\nesp: %x", 1, tcb->esp);
  kprint ("\nnext_task: ");
  itoa ((uint32_t)tcb->next_task, s);
  kprint (s);
  kprint ("\npage_directory: ");
  itoa ((uint32_t)tcb->page_dir, s);
  kprint (s);
  kprintf ("\ncurrent state: %d", 1, tcb->state);
}

struct tcb* search_task (uint32_t pid)
{
  struct tcb* it;
  for (it = head; it != 0; it = it->next_task)
    if (it->pid == pid)
      return it;
  return 0;
}
