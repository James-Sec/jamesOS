;print message in 16-bit real mode

print_string_rm:
pusha

mov ah, 0x0e
print_string_rm_loop:
mov al, [bx]
cmp al, 0
je end
int 0x10
add bx, 1
jmp print_string_rm_loop

end:
popa
ret
