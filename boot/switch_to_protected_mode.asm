[bits 16] 
%include "boot/gdt_struct.asm" ; the GDT table
switch_to_pm:
 ; disabling interruptions
  cli

  ; loading the GDT table
  lgdt [gdt_descriptor] 

  ; switching
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  ; cleaning pipeline with a far jump
  jmp CODE_SEG:init_pm

[bits 32] 
init_pm:
  ; initialise registers and the stack once in pm
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; realocating stack
  mov ebp, 0x90000
  mov esp, ebp

  call begin_pm
