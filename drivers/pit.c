#include "../include/pit.h"

uint32_t tick = 0;

static void pit_callback (registers_t *regs)
{
    tick++;
    kprintf ("Tick: %d\n", 1, tick);
}

void pit_init (uint32_t freq) {

    register_interrupt_handler(IRQ0, pit_callback);

    
    /*
    uint32_t divisor = PIT_INPUT_FREQUENCY / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    
    port_byte_out (PIT_CTRL_PORT, 0x36); 
    port_byte_out (PIT_DATA_PORT_CH0, low);
    port_byte_out (PIT_DATA_PORT_CH0, high);
    */
}

