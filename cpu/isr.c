#include "../include/isr.h"

isr_t interrupt_handlers[256]; 

void isr_install() 
{
  set_idt_gate (0, (uint32_t)isr0);
  set_idt_gate (1, (uint32_t)isr1);
  set_idt_gate (2, (uint32_t)isr2);
  set_idt_gate (3, (uint32_t)isr3);
  set_idt_gate (4, (uint32_t)isr4);
  set_idt_gate (5, (uint32_t)isr5);
  set_idt_gate (6, (uint32_t)isr6);
  set_idt_gate (7, (uint32_t)isr7);
  set_idt_gate (8, (uint32_t)isr8);
  set_idt_gate (9, (uint32_t)isr9);
  set_idt_gate (10, (uint32_t)isr10);
  set_idt_gate (11, (uint32_t)isr11);
  set_idt_gate (12, (uint32_t)isr12);
  set_idt_gate (13, (uint32_t)isr13);
  set_idt_gate (14, (uint32_t)isr14);
  set_idt_gate (15, (uint32_t)isr15);
  set_idt_gate (16, (uint32_t)isr16);
  set_idt_gate (17, (uint32_t)isr17);
  set_idt_gate (18, (uint32_t)isr18);
  set_idt_gate (19, (uint32_t)isr19);
  set_idt_gate (20, (uint32_t)isr20);
  set_idt_gate (21, (uint32_t)isr21);
  set_idt_gate (22, (uint32_t)isr22);
  set_idt_gate (23, (uint32_t)isr23);
  set_idt_gate (24, (uint32_t)isr24);
  set_idt_gate (25, (uint32_t)isr25);
  set_idt_gate (26, (uint32_t)isr26);
  set_idt_gate (27, (uint32_t)isr27);
  set_idt_gate (28, (uint32_t)isr28);
  set_idt_gate (29, (uint32_t)isr29);
  set_idt_gate (30, (uint32_t)isr30);
  set_idt_gate (31, (uint32_t)isr31);

  // PIC is numbered as [0x8-0xf] and [0x70-0x77]
  // remap the PIC
  //            port  data
  // 0x11 command means initialize command (wait for 3 initialize words
  // on the data port)
  // write 0x11 at master command port
  port_byte_out(0x20, 0x11);
  // write 0x11 at slave command port
  port_byte_out(0xA0, 0x11);

  // first word is the vector offset this means that
  // the master port will begin at 0x20(32) and the
  // slave will begin at 0x28(40)
  // write 0x20 at master data port
  port_byte_out(0x21, 0x20);
  // write 0x28 at slave data port
  port_byte_out(0xA1, 0x28);

  // second word tells how the devices are wired to the pic
  // 0x04 tells the master that there is a slave at position 2
  // 0x04 (0000 0100)
  // write 0x04 at master data port
  port_byte_out(0x21, 0x04);
  // 0x02 tells the slave that it is chained to master (cascade)
  // 0x02 (0000 0010)
  // write 0x28 at slave data port
  port_byte_out(0xA1, 0x02);

  // third word gives information about the environment
  // 0x01 means 8086 mode
  // write 0x01 at master data port
  port_byte_out(0x21, 0x01);
  // write 0x01 at slave data port
  port_byte_out(0xA1, 0x01);

  // this is to restore the saved masks
  // masks are used to ignore a certain interrupt, it is a 8bit number
  // that if the 3 bit is set, it will ignore that request
  // 0000 0001
  // so all bits are set to 0 right here
  // write 0x00 at master data port
  port_byte_out(0x21, 0x0);
  // write 0x00 at slave data port
  port_byte_out(0xA1, 0x0);

  set_idt_gate (32, (uint32_t)irq0);
  set_idt_gate (33, (uint32_t)irq1);
  set_idt_gate (34, (uint32_t)irq2);
  set_idt_gate (35, (uint32_t)irq3);
  set_idt_gate (36, (uint32_t)irq4);
  set_idt_gate (37, (uint32_t)irq5);
  set_idt_gate (38, (uint32_t)irq6);
  set_idt_gate (39, (uint32_t)irq7);
  set_idt_gate (40, (uint32_t)irq8);
  set_idt_gate (41, (uint32_t)irq9);
  set_idt_gate (42, (uint32_t)irq10);
  set_idt_gate (43, (uint32_t)irq11);
  set_idt_gate (44, (uint32_t)irq12);
  set_idt_gate (45, (uint32_t)irq13);
  set_idt_gate (46, (uint32_t)irq14);
  set_idt_gate (47, (uint32_t)irq15);

  load_idt(); // Load with ASM 
}

char *exception_messages[] = { 
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",

  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",

  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",

  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

void isr_handler (registers_t *r) 
{
  kprint_debug ("interrupt received: ", LIGHT_RED);
  char s[3];
  itoa (r->int_no, s);
  kprint_debug (s, LIGHT_RED);
  kprint ("\n");
  kprint_debug (exception_messages[r->int_no], LIGHT_RED);
  kprint ("\n");
  if (interrupt_handlers [r->int_no] != 0)
  {
    isr_t handler = interrupt_handlers [r->int_no];
    // calling the interrupt handler
    handler (r);
  }
}

void register_interrupt_handler (uint8_t n, isr_t handler) 
{
  interrupt_handlers [n] = handler;
}

void irq_handler (registers_t *r)  
{
  // send a EOI to the PIC
  if (r->int_no >= 40) 
    port_byte_out (0xa0, 0x20);
  port_byte_out (0x20, 0x20); 

  if (interrupt_handlers [r->int_no] != 0)
  {
    isr_t handler = interrupt_handlers [r->int_no];
    // calling the handler
    handler (r);
  }
}
