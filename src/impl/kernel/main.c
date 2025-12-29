#include "print.h"
#include "idt.h"
#include "syscall.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    print_str("Welcome to the Microkernel!\n");
    
    idt_init();
    syscall_init();
    
    print_str("Kernel initialized successfully\n");
    
    print_str("Testing syscall...\n");
    
    const char* test_msg = "Hello from syscall!";
    uint64_t result;
    
    asm volatile(
        "mov $0, %%rax;"      
        "mov %1, %%rdi;"      
        "syscall;"
        "mov %%rax, %0;"     
        : "=r"(result)
        : "r"(test_msg)
        : "rax", "rdi", "rcx", "r11", "memory"
    );
    
    print_str("\nSyscall test complete\n");
    
    // Test sys_getpid
    asm volatile(
        "mov $2, %%rax;"      
        "syscall;"
        "mov %%rax, %0;"
        : "=r"(result)
        :
        : "rax", "rcx", "r11", "memory"
    );
    
    print_str("PID returned: ");
    if (result == 1) {
        print_str("1 (correct!)\n");
    } else {
        print_str("ERROR\n");
    }
    
    while(1) {
        asm volatile("hlt");
    }
}
