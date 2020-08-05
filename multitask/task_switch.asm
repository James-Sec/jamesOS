extern current_task
extern task_function

struc TCB
  .ESP resd 1
  .PAGE_DIR resd 1
  .NEXT_TASK resd 1
  .PID resd 1
  .PNAME resb 32
endstruc

global task_switch

task_switch:
 
    push ebx
    push esi
    push edi
    push ebp
 
    mov edi,[current_task]    ;edi = address of the previous task's "thread control block"
    mov [edi+TCB.ESP],esp         ;Save ESP for previous task's kernel stack in the thread's TCB
 
    ;Load next task's state
 
    mov esi,[esp+(4+1)*4]         ;esi = address of the next task's "thread control block" (parameter passed on stack)
    mov [current_task],esi    ;Current task's TCB is the next task TCB
 
    mov esp,[esi+TCB.ESP]         ;Load ESP for next task's kernel stack from the thread's TCB
    mov eax,[esi+TCB.PAGE_DIR]         ;eax = address of page directory for next task
    mov ecx,cr3                   ;ecx = previous task's virtual address space
 
    cmp eax,ecx                   ;Does the virtual address space need to being changed?
    je .doneVAS                   ; no, virtual address space is the same, so don't reload it and cause TLB flushes
    mov cr3,eax                   ; yes, load the next task's virtual address space
.doneVAS:

    pop ebp
    pop edi
    pop esi
    pop ebx

    ret                           ;Load next task's EIP from its kernel stack

