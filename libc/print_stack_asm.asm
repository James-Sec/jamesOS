global print_stack_asm
global func_test
extern print_stack_c
extern test_c
extern print

func_test:
push 0x14
push 1
call print_stack_asm
add esp, 8
push 0xf8
call test_c
ret

print_stack_asm:
mov edx, [esp+8]; if the caller is a C code, it must be 0x1c. 
                ; if the caller is a assembly code, it must be 0x14.
mov eax, [esp+4]

push eax
push dword 0

print_stack_loop:
mov edx, [esp+0x10]
pop ecx
pop eax
cmp ecx, eax
je print_stack_end_loop

push eax
push ecx
add ecx, edx
push dword[esp+ecx]
add ecx, esp
add ecx, 4
push ecx
call print_stack_c

add esp, 8
pop ecx
add ecx, 1
push ecx
jmp print_stack_loop

print_stack_end_loop:
ret
