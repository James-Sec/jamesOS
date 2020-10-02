#ifndef PIT_H
#define PIT_H

#define PIT_INPUT_FREQUENCY 1193180
#define PIT_DATA_PORT_CH0 0x40
#define PIT_DATA_PORT_CH1 0x41
#define PIT_DATA_PORT_CH2 0x42
#define PIT_CTRL_PORT 0x43

#include <stdint.h>
#include "string.h"
#include "ports.h"
#include "vga.h"
#include "isr.h"
#include "multitask.h"
#include "mem.h"

// global functions
void pit_init (uint32_t frequency);

// global variables
uint32_t tick = 0;

#endif
