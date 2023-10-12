#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_SCANCODE 0x60

#include <stdint.h>
#include <ports.h>
#include <isr.h>
#include <string.h>
#include <vga.h>

void keyboard_init ();
#endif
