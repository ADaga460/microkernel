#include "idt.h"
#include "print.h"

// IDT entry structure
struct idt_entry {
    uint16_t offset_low;    // Lower 16 bits of handler address
    uint16_t selector;      // Kernel code segment selector (0x08)
    uint8_t  ist;          // Interrupt Stack Table offset (0 for now)
    uint8_t  type_attr;    // Type and attributes
    uint16_t offset_mid;   // Middle 16 bits of handler address
    uint32_t offset_high;  // Higher 32 bits of handler address
    uint32_t zero;         // Reserved
} __attribute__((packed));

// IDT structure
struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// 256 entries for the IDT
static struct idt_entry idt[256];
static struct idt_ptr idtr;

// Exception messages
static const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

// exception handler
void exception_handler(uint64_t vector, uint64_t error_code, uint64_t rip) {
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_RED);
    print_str("KERNEL PANIC!\n");
    
    if (vector < 32) {
        print_str(exception_messages[vector]);
        print_str("\n");
    } else {
        print_str("Unknown Exception\n");
    }
    
    print_str("System Halted");
    
    while(1) {
        asm volatile("hlt");
    }
}


static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    uint64_t handler_addr = (uint64_t)isr;
    // set idt entry fields
    idt[vector].offset_low = handler_addr & 0xFFFF;
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].type_attr = flags;
    idt[vector].offset_mid = (handler_addr >> 16) & 0xFFFF;
    idt[vector].offset_high = (handler_addr >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0;
}
// defined in assembly
extern void* isr_stub_table[];

void idt_init(void) {
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t)&idt;
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }
    
    asm volatile("lidt %0" : : "m"(idtr));
    
    print_str("IDT initialized\n");
}
