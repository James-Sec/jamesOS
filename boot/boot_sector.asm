; the BIOS loads the boot sector at address 0x7c00 [0x7c00..0x7e00]
[org 0x7c00]

; the memory offset where we will load our kernel
KERNEL_OFFSET equ 0x1000

; the BIOS loads the current drive identifier in dl register
mov [BOOT_DRIVE], dl

; organizing the stack
mov bp, 0x9000
mov sp, bp

; printing 16-bit real mode debug message
mov bx, REAL_MODE_MSG
call print_string_rm

; loading the kernel
call load_kernel 

; switching to protected mode
call switch_to_pm

jmp $

%include "boot/print_string_rm.asm"
%include "boot/print_string_pm.asm"
%include "boot/print_hex.asm"
%include "boot/switch_to_protected_mode.asm"
%include "boot/disk_load.asm"

[bits 16] ; 16 bits instructions
load_kernel:
  ; printing load kernel debug message
  mov bx, KERNEL_LOAD_MSG
  call print_string_rm

  ; reading the kernel from the storage device
  mov bx, 0
  mov es, bx ;segment
  mov bx, KERNEL_OFFSET ;offset
  mov dh, 48 ;number of sectors
  mov dl, [BOOT_DRIVE] ;set the driver to read
  call disk_load
  ret

[bits 32] ; 32 bits instructions
begin_pm:
  ; printing 32-bit protected mode debug message
  mov ebx, PROT_MODE_MSG
  call print_string_pm

  ; starting kernel
  call KERNEL_OFFSET
  jmp $

BOOT_DRIVE db 0 ; reserved byte for the drive number
REAL_MODE_MSG db "Started in 16-bit Real Mode", 0
PROT_MODE_MSG db "Successfuly landed in 32-bit Protected Mode", 0
KERNEL_LOAD_MSG db "Loading kernel into memory", 0
DISK_ERROR_MSG_1 db "Disk load error in jc", 0
DISK_ERROR_MSG_2 db "Disk load error in cmp", 0

times 510-($-$$) db 0

dw 0xaa55
