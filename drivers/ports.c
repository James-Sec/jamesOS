#include "../include/ports.h"

//Read a byte from a port
uint8_t port_byte_in (uint16_t port)
{
  uint8_t result;
  asm("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}


//Write a byte in a port
void port_byte_out (uint16_t port, uint8_t data)
{
  asm("out %%al, %%dx" : : "a" (data) , "d" (port));
}


//Read a work from a port
uint16_t port_word_in (uint16_t port)
{
  uint16_t result;
  asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}


//Write a word in a port
void port_word_out (uint16_t port, uint16_t data)
{
  asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

// parameter is a 16bit address
uint32_t port_dword_in (uint16_t port)
{
  uint16_t result;
  __asm__ ("in %%dx, %%eax" : "=a" (result) : "d" (port));
  return result;
}

// parameter is the 16bit port address and the data that will
// be written to
void port_dword_out (uint16_t port, uint32_t data)
{
  // write value in al to port address stored in dx
  __asm__ __volatile__ ("out %%eax, %%dx" : : "a" (data), "d" (port));
}
