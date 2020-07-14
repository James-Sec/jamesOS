; print message in 32-bit protected mode
; ebx stores the string address

[bits 32]

; the VGA memory starts at 0x0xb8000
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
  ; pushing all general purpose registers: AX, CX, DX, BX, SP, BP, SI, DI
  pusha

  ; moving the video address to edx
  mov edx, VIDEO_MEMORY

  print_string_pm_loop:
    mov al, [ebx] 
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je print_string_pm_end
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp print_string_pm_loop

  print_string_pm_end:
    ; poping all general purpose registers: DI, SI, BP, SP, BX, DX, CX, AX
    popa
    ret
