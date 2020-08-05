global function

struc tcb
  .f resb 1
  .s resw 1
  .t resd 1
endstruc

function:
mov ebx, esp
add ebx, 4
mov ebx, [ebx]

mov eax, [ebx+tcb.t]

ret
