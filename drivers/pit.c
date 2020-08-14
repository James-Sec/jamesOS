#include "../include/pit.h"

uint32_t tick = 0;

//TCB MANAGER
extern struct tcb_manager mtmg;
extern uint8_t multitask_on;
extern struct tcb* current_task;

static void pit_callback (registers_t *regs)
{
    tick++;
    //TCB MANAGER
    if (multitask_on) {
			if (!(tick % 10))
				if (mtmg.current_rtr != mtmg.top_rtr)
					schedule ();
    }
    //END
    


    //kprintf ("Tick: %d\n", 1, tick);
    /*
    kprint("Tick: ");
    
    char tick_ascii[256];
    itoa (tick, tick_ascii);
    kprint (tick_ascii);
    kprint ("\n");
    */
}

void pit_init (uint32_t freq) {

    register_interrupt_handler(IRQ0, pit_callback);

    
    uint32_t divisor = PIT_INPUT_FREQUENCY / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    
    port_byte_out (PIT_CTRL_PORT, 0x36); 
    port_byte_out (PIT_DATA_PORT_CH0, low);
    port_byte_out (PIT_DATA_PORT_CH0, high);
}

