[bits 16]
%include "boot/print_hex.asm"
; load sectors from a storage device
; dh contains the numbers of sectors
; dl contains the device
; es contains the segment
; bx contains the offset

disk_load:
  push dx
  mov ah, 0x02
  mov al, dh  ; number of sectors

  mov cl, 2   ; init sector  
  mov ch, 0   ; cylinder 
  mov dh, 0   ; Head 

  int 0x13

  jc disk_error_1 ; if the error flag is set

  pop dx
  cmp al, dh 
  jne disk_error_2
  ret

disk_error_1:
  mov bx, DISK_ERROR_MSG_1
  call print_string_rm

  mov ah, 0x1
  mov dl, 0x0
  int 0x13

  xor dx, dx
  mov dl, ah
  call print_hex

  jmp $

disk_error_2:
  mov bx, DISK_ERROR_MSG_2
  call print_string_rm
  jmp $
