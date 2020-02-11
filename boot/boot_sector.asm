;the boot sector
[org 0x7c00] ; address offset for the labels
KERNEL_OFFSET equ 0x1000 ; the memory offset where we will load our kernel

mov [BOOT_DRIVE], dl ; drive that the kernel is (stored by bios)

;organizing the stack
mov bp, 0x9000
mov sp, bp

;printing 16-bit real mode debug message
mov bx, MSG_REAL_MODE
call print_string_16b

;loading the kernel
call load_kernel

;switching to protected mode. We will not return to here anymore
call switch_to_pm

jmp $

%include "boot/print_string_16b.asm"
%include "boot/print_string_pm.asm"
%include "boot/switch_to_protected_mode.asm"
%include "boot/disk_load.asm"
%include "boot/print_hex.asm"

[bits 16] ;16 bits instructions
load_kernel:
  ;printing load kernel debug message
  mov bx, MSG_LOAD_KERNEL
  call print_string_16b

  ;now, we will read the kernel from the HD
  mov bx, 0
  mov es, bx ;segment
  mov bx, KERNEL_OFFSET ;offset
  mov dh,31 ;number of sectors
  mov dl, [BOOT_DRIVE] ;set the driver to read
  call disk_load
  ret

[bits 32]
BEGIN_PM:
;printing 32-bit protected mode message debug
mov ebx, MSG_PROT_MODE
call print_string_pm

;starting kernel. we will not return here anymore
call KERNEL_OFFSET
jmp $





BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfuly landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
DISK_ERROR_MSG_1 db "Disk load error in jc", 0
DISK_ERROR_MSG_2 db "Disk load error in cmp", 0

times 510-($-$$) db 0
dw 0xaa55
