[bits 16] ; 16 bits instructions
; print a word size value in hexadeciaml (word = 16 bits)
; this function analyses 4 bits at a time, because each algarism in hex is equivalent to 4 algarism in binary
; dx contains the value that will be printed

print_hex:
  ; pushing all general purpose registers: AX, CX, DX, BX, SP, BP, SI, DI
  pusha

  ; the current digit (the digit 0 and 1 are '0x')
  mov ax, 2
  ; the shift number
  mov cl, 12

  print_hex_loop:
    ; pushing the original value
    push dx

    ; verifying the current 4 bits
    shr dx, cl 
    and dx, 0x000f
    mov bx,dx

    ; changing the MSG to the correct algarism
    mov dx, [MAP+bx]
    mov bx,ax
    mov [MSG+bx], dl

    ; restoring the original value
    pop dx

    ; virefying if it is the last 4 bits set
    add ax, 1
    cmp ax, 6
    je end_hex
    sub cl, 4
    jmp print_hex_loop

  end_hex:
    ; preparing to print_string
    mov bx, MSG
    call print_string_rm

    ; poping all general purpose registers: DI, SI, BP, SP, BX, DX, CX, AX
    popa
    ret

  ; contain the character that represents the value of its index
  MAP:
    db '0123456789abcdef'

  ; the string that will be printed
  MSG:
    db '0x????', 0
