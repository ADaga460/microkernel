#pragma once

#include <stdint.h>

// Syscall numbers
#define SYSCALL_WRITE  0
#define SYSCALL_EXIT   1
#define SYSCALL_GETPID 2

void syscall_init(void);
uint64_t syscall_handler(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3);