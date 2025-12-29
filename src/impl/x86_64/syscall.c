#include "syscall.h"
#include "print.h"

#define MSR_STAR  0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_SFMASK 0xC0000084

static inline void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

extern void syscall_entry(void);

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
    // dummy
    return 1;
}

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

static inline uint64_t rdmsr(uint32_t msr) {
    uint32_t low, high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uint64_t)high << 32) | low;
}

void syscall_init(void) {
    uint64_t efer = rdmsr(0xC0000080);  
    efer |= 1;                           
    wrmsr(0xC0000080, efer);             
    
    uint64_t star = ((uint64_t)0x08 << 32) | ((uint64_t)0x08 << 48);
    wrmsr(MSR_STAR, star);

    wrmsr(MSR_LSTAR, (uint64_t)syscall_entry);
    
    wrmsr(MSR_SFMASK, 0x200);
    
    print_str("Syscalls initialized\n");
}
