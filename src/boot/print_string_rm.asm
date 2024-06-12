; print message in 16-bit real mode
; bx stores the string address
; ah stores the print char function number
; al stores the current character to write

print_string_rm: 
  ; pushing all general purpose registers: AX, CX, DX, BX, SP, BP, SI, DI
  pusha 

  ; video teletype output value
  mov ah, 0x0e 

  print_string_rm_loop:
    mov al, [bx]
    cmp al, 0 
    je end
    int 0x10
    add bx, 1
    jmp print_string_rm_loop

  end:
    ; poping all general purpose registers: DI, SI, BP, SP, BX, DX, CX, AX
    popa 
    ret
