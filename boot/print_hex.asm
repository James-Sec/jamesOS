print_hex:
pusha

mov ax, 2
mov cl, 12
push dx

beginning:
shr dx, cl
and dx, 0x000f
mov bx,dx
mov dx, [MAP+bx]
mov bx,ax
mov [MSG+bx], dl
pop dx
add ax, 1
cmp ax, 6
je end_hex
sub cl, 4
push dx
jmp beginning

end_hex:
mov bx, MSG ; preparing to print_string
call print_string_16b

popa
ret

MAP:
db '0123456789abcdef'

MSG:
db '0x????', 0
