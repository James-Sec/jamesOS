#include "../include/ports.h"

// read a byte from a port
uint8_t port_byte_in (uint16_t port)
{
  uint8_t result;
  asm volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

// write a byte in a port
void port_byte_out (uint16_t port, uint8_t data)
{
  asm volatile ("out %%al, %%dx" : : "a" (data) , "d" (port));
}

// read a word from a port
uint16_t port_word_in (uint16_t port)
{
  uint16_t result;
  asm volatile ("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

// write a word in a port
void port_word_out (uint16_t port, uint16_t data)
{
  asm volatile ("out %%ax, %%dx" : : "a" (data), "d" (port));
}

// read a double word from a port
uint32_t port_dword_in (uint16_t port)
{
  uint32_t result;
  asm volatile ("in %%dx, %%eax" : "=a" (result) : "d" (port));
  return result;
}

// write a double word in a port
void port_dword_out (uint16_t port, uint32_t data)
{
  asm volatile ("out %%eax, %%dx" : : "a" (data), "d" (port));
}
