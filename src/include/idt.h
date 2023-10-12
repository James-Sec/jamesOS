#ifndef IDT_H
#define IDT_H

#define IDT_ENTRIES 256 

// the code segment (we can change it, reading the segment register)
#define KERNEL_CS 0x08 

// simple functions to get specific part of an address
#define low_16(address) (uint16_t)((address) & 0xFFFF) 
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF) 

#include <stdint.h>

// each interrupt has a entry
typedef struct 
{
  uint16_t low_offset; // low address of the function
  uint16_t sel; // code segment in GDT
  uint8_t always0; 
  uint8_t flags; //described above
  uint16_t high_offset; // high address of the function
}__attribute__((packed)) idt_gate_t;

// this struct defines the idt vector bellow
typedef struct 
{
  uint16_t limit;
  uint32_t base;
}__attribute__((packed)) idt_register_t;

idt_gate_t idt [IDT_ENTRIES]; 
idt_register_t idt_reg; 

void set_idt_gate (uint32_t n, uint32_t handler);
void load_idt ();
#endif
