#include "print.h"
#include "idt.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    print_str("Welcome to the Microkernel!\n");
    
    idt_init();
    
    print_str("Kernel initialized successfully\n");
    
    // (remove this after testing)
    // asm volatile("int $0x0"); // Division by zero
    
    while(1) {
        asm volatile("hlt");
    }
}
