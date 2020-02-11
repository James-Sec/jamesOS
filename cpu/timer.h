#ifndef TIMER_H
#define TIMER_H
#include "types.h"
#include <stdint.h>
#include "../libc/string.h"
#include "../drivers/ports.h"
#include "../drivers/screen.h"
#include "isr.h"

void init_timer(uint32_t frequency);
#endif
