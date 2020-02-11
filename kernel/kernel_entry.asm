global _start
[bits 32]
_start:
[extern entry] ; instruction for the linker
call entry
jmp $
