; I HATE WRITING ASSEMBLY SO THIS IS AI GENERATED
; PLEASE FOR THE LOVE OF GOD TEST THIS THOROUGHLY BEFORE USING

global isr_stub_table
extern exception_handler

section .text

; Macro for ISRs without error codes
%macro ISR_NOERR 1
isr_stub_%+%1:
    push 0              ; Push dummy error code
    push %1             ; Push interrupt vector
    jmp isr_common
%endmacro

; Macro for ISRs with error codes
%macro ISR_ERR 1
isr_stub_%+%1:
    push %1             ; Push interrupt vector (error code already pushed)
    jmp isr_common
%endmacro

; Common ISR handler
isr_common:
    ; Save registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    ; Call C exception handler
    ; Args: vector (rdi), error_code (rsi), rip (rdx)
    mov rdi, [rsp + 120]  ; Vector number
    mov rsi, [rsp + 128]  ; Error code
    mov rdx, [rsp + 136]  ; RIP
    call exception_handler
    
    ; Restore registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    ; Clean up error code and vector
    add rsp, 16
    
    iretq

; Generate ISR stubs for all 32 exceptions
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR   21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR   29
ISR_ERR   30
ISR_NOERR 31

; ISR stub table (array of pointers)
section .rodata
isr_stub_table:
%assign i 0
%rep 32
    dq isr_stub_%+i
%assign i i+1
%endrep
