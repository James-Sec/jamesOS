#include "../include/kernel_tasks.h"
uint8_t task_entry ()
{
  unlock_irq ();
  return task_function ();
}
