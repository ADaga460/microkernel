#include "syscall.h"
#include "print.h"

// MSR addresses
#define MSR_STAR  0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_SFMASK 0xC0000084

// Helper functions to write MSRs
static inline void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

// External assembly function (we'll write this next)
extern void syscall_entry(void);

// Syscall handlers
static uint64_t sys_write(const char* str) {
    print_str((char*)str);
    return 0;
}

static uint64_t sys_exit(int code) {
    print_str("\nProcess exited with code: ");
    // TODO: print the code
    while(1) asm volatile("hlt");
    return 0;
}

static uint64_t sys_getpid(void) {
    return 1; // Hardcoded for now
}

// Syscall dispatcher (called from assembly)
uint64_t syscall_handler(uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    switch(syscall_num) {
        case SYSCALL_WRITE:
            return sys_write((const char*)arg1);
        case SYSCALL_EXIT:
            return sys_exit((int)arg1);
        case SYSCALL_GETPID:
            return sys_getpid();
        default:
            print_str("Unknown syscall\n");
            return -1;
    }
}

void syscall_init(void) {
    // Set STAR MSR
    uint64_t star = ((uint64_t)0x08 << 32) | ((uint64_t)0x08 << 48);
    
    wrmsr(MSR_STAR, star);
    wrmsr(MSR_LSTAR, (uint64_t)syscall_entry);
    wrmsr(MSR_SFMASK, 0x200);

    print_str("Syscall interface initialized\n");
}