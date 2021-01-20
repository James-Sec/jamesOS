#include <multitask.h>

static struct tcb* _create_task (struct page_directory_t *page_dir, struct tcb *next_task, uint32_t pid, uint8_t state, char *pname, uint8_t (*func) (void));
static void sleep_until (uint32_t ticks);

uint32_t last_pid = 1;
uint32_t lock_irq_counter = 0;

static struct tcb* _create_task (struct page_directory_t *page_dir, struct tcb *next_task, uint32_t pid, uint8_t state, char *pname, uint8_t (*func) (void))
{
  struct tcb *new_task = (struct tcb*) kmalloc_u (0x1000);
  new_task->ebp = ((uint32_t)new_task + 0x1000) - sizeof (struct tcb);
  new_task->esp = new_task->ebp - 4;
  new_task->page_dir = page_dir;
  new_task->next_task = next_task;
  new_task->pid = pid;
  new_task->state = state;
  if (state == READY_TO_RUN)
    ++ready_to_run_counter;
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
  return head = _create_task (kernel_directory, head, ++last_pid, state, pname, func);

}

void multitask_init () 
{
  head = (struct tcb *) kmalloc_u (0x1000);
  current_task = head;

  asm volatile ("mov %%esp, %0" : "=r" (head->esp));
  head->page_dir = current_directory;
  head->next_task = 0;
  head->pid = 1;
  head->state = RUNNING;
  char *s = "JAMES";
  memcpy (s, head->pname, 5);

  //create_idle_task (idle_task_function, "IDLE");
  create_task (idle_task_function, "IDLE", IDLE);
  create_task (task_terminator, "CHUAZNEGUER", READY_TO_RUN);
  ++ready_to_run_counter;
	multitasking_on = 1;
}

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
void sleep (uint32_t seconds)
{
	sleep_until ((seconds * 100) + tick);
}

static void sleep_until (uint32_t ticks)
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
  {
    task_switch (idle_task);
    return;
  }

	// if the running task wasnt blocked or "sleeped"
	// set to the READY_TO_RUN list
  if (current_task->state == RUNNING)
    current_task->state = READY_TO_RUN;
	// search for next available task
  next = current_task->next_task;
  while (1)
  {
    if (!next)
      next = head;
    if (next->state != READY_TO_RUN)
      next = next->next_task;
    else 
      break;
  }
  next->state = RUNNING;
  task_switch (next);
}

void task_entry ()
{
  kprintf ("[%s] started.\n", 1, current_task->pname);
  unlock_irq ();
}

void task_termination ()
{
  lock_irq ();
  kprintf ("[%s] terminated.\n", 1, current_task->pname);
  --ready_to_run_counter;
  current_task->state = TERMINATED;
  scheduler ();
}

void print_task (struct tcb* tcb)
{
  if (!tcb)
    tcb = current_task;
  kprint ("---task info---\n");
  kprintf ("pname: %s\n", 1, tcb->pname);
  kprintf ("pid: %d\n", 1, tcb->pid);
  kprintf ("esp: %x\n", 1, tcb->esp);
  kprintf ("next_task: %x\n", 1, tcb->next_task);
  kprintf ("page_directory: %x\n", 1, tcb->page_dir);
  kprintf ("current state: %d\n", 1, tcb->state);
}

struct tcb* search_task (uint32_t pid)
{
  struct tcb* it;
  for (it = head; it != 0; it = it->next_task)
    if (it->pid == pid)
      return it;
  return 0;
}
