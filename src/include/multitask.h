#ifndef MULTITASK_H
#define MULTITASK_H

#include <stdint.h>
#include <paging.h>
#include <vga.h>
#include <mem.h>
#include <pit.h>
#include <idle_task.h>
#include <task_terminator.h>

#define BLOCKED 0
#define RUNNING 1
#define READY_TO_RUN 2
#define SLEEPING 3
#define IDLE 4
#define TERMINATED 5

#define INITIAL_PROCESS_STACK_SIZE 6

struct tcb
{
  uint32_t *esp;
  uint32_t *ebp;
  struct page_directory_t* page_dir;
  struct tcb* next_task;
  uint32_t pid;
  char pname [32];
  uint8_t state;
	uint32_t sleep_until;

}__attribute__ ((packed));

// global functions
struct tcb* create_task (uint8_t (*func) (void), char *pname, uint8_t state, uint32_t argc, uint8_t *argp);
struct tcb* search_task (uint32_t pid);
void multitask_init ();
void print_task (struct tcb*);
void unblock_task (uint32_t pid);
void soft_unblock_task (uint32_t pid);
void block_task (uint8_t reason, uint8_t lock);
void sleep (uint32_t seconds);
void sleep_until (uint32_t ticks);
void task_entry ();
void task_termination (uint32_t argc, uint8_t *argp);
void scheduler () ;
void lock_irq ();
void unlock_irq ();

//void add_parameter(struct tcb* task, uint8_t *arg, uint32_t size);

// global variables
struct tcb* current_task;
struct tcb* idle_task;
struct tcb* head;

uint32_t multitasking_on;
uint32_t ready_to_run_counter;

#endif
