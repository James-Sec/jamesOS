#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <ports.h>

#define COM1 0x3f8

void serial_init ();
void serial_send_byte (uint8_t ch);
void serial_send_string (uint8_t* msg);

#endif
