global print_esp_eip_asm
extern print_esp_eip_c

print_esp_eip_asm:

; getting eip 
mov eax, [esp]
push eax

; getting esp
mov eax, esp
add eax, 8
push eax

call print_esp_eip_c
add esp, 8
ret
