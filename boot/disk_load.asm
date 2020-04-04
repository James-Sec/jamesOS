;load sectors from dh
disk_load:
  ;dh contains the numbers of sectors
  ;dl contains the drive
  ;es contains the segment
  ;bx contains the offset
  push dx
  mov ah, 0x02
  mov al, dh  ;number of sectors
  
  mov cl, 2   ;init sector
  mov ch, 0   ;cylinder

  mov dh, 0    ;Head

  int 0x13

  jc disk_error_1 ;if the error flag is set

  pop dx
  cmp al, dh
  jne disk_error_2
  ret




disk_error_1:
mov bx, DISK_ERROR_MSG_1
call print_string_rm
jmp $

disk_error_2:
mov bx, DISK_ERROR_MSG_2
call print_string_rm
;mov [0x7e00], al
;mov dx, [0x7e00]
;call print_hex
jmp $
