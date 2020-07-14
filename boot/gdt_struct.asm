; Intel flat model
gdt_start:

gdt_null:
  dd 0x0 ; dd -> double word -> 16bits + 16bits 
  dd 0x0 

gdt_code:
  dw 0xffff     ; limit(0-15)
  dw 0x0        ; base (0-15)
  db 0x0        ; base (16-23)
  db 10011010b  ; set 8 flags
  db 11001111b  ; limit (16-19) and set 4 flags
  db 0x0        ; base (24-31)

gdt_data:
  dw 0xffff     ; limit(0-15)
  dw 0x0        ; base (0-15)
  db 0x0        ; base (16-23)
  db 10010010b  ; set 8 flags
  db 11001111b  ; limit (16-19) and set 4 flags
  db 0x0        ; base (24-31)

gdt_end:
gdt_descriptor:
  dw gdt_end - gdt_start - 1 ; gdt size
  dd gdt_start               ; gdt address


; define address of each sement descriptor
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
