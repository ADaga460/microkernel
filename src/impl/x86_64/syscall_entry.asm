; AI WRITTEN - PLEASE TEST THOROUGHLY BEFORE USING

global syscall_entry
extern syscall_handler

section .data
user_rsp: dq 0
kernel_rsp: dq kernel_stack_top

section .bss
align 16
kernel_stack_bottom:
    resb 16384      ; 16KB kernel stack for syscalls
kernel_stack_top:

section .text
bits 64

syscall_entry:
    ; Save userspace stack pointer
    mov [rel user_rsp], rsp
    
    ; Switch to kernel stack
    mov rsp, [rel kernel_rsp]
    
    ; Push userspace context
    push qword [rel user_rsp]  ; User RSP
    push r11                    ; User RFLAGS
    push rcx                    ; User RIP (return address)
    
    ; Save callee-saved registers
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
    
    ; Syscall arguments: RAX = syscall_num, RDI = arg1, RSI = arg2, RDX = arg3
    ; syscall_handler expects: (uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3)
    ; So move: RAX->RDI, RDI->RSI, RSI->RDX, RDX->RCX
    
    mov rcx, rdx    ; arg3
    mov rdx, rsi    ; arg2
    mov rsi, rdi    ; arg1
    mov rdi, rax    ; syscall_num
    
    ; Call C handler
    call syscall_handler
    
    ; Return value is already in RAX
    
    ; Restore callee-saved registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    
    ; Restore userspace context
    pop rcx         ; User RIP
    pop r11         ; User RFLAGS
    pop rsp         ; User RSP
    
    ; Return to userspace
    ; sysretq uses RCX as return address and R11 as RFLAGS
    sysretq
