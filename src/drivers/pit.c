#include <pit.h>

static void pit_callback (registers_t *regs);

static void pit_callback (registers_t *regs)
{
  tick++;
  if (multitasking_on)
  {
    struct tcb* x = head;
    uint32_t flag = 0;
    do
    {
      if (x->state == SLEEPING && x->sleep_until < tick)
        unblock_task (x->pid);
      if (x->state == READY_TO_RUN)
        ++flag;
      x = x->next_task;
    }
    while (x);

    if (!(tick % 1) && flag) {
      lock_irq ();
      scheduler ();
      unlock_irq ();
    }
  }
}

void pit_init (uint32_t freq) {

  register_interrupt_handler(IRQ0, pit_callback);

  uint32_t divisor = PIT_INPUT_FREQUENCY;
  uint8_t low  = (uint8_t)(divisor & 0xFF);
  uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

  port_byte_out (PIT_CTRL_PORT, 0x36); 
  port_byte_out (PIT_DATA_PORT_CH0, low);
  port_byte_out (PIT_DATA_PORT_CH0, high);
}
