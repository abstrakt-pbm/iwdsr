#include "posixmem.hpp"
#include <iostream>
#include <format>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>

enum PosixSysCalls {
    MMAP = 9
};

uint64_t PosixMem::allocate(uint64_t baseAddress, uint64_t byteCount, P_SIZE pageSize) {
    struct user_regs_struct procRegs;
    struct user_regs_struct regsBeforeStop = procRegs;
    
    ptrace(
        PTRACE_ATTACH,
        pid,
        NULL,
        NULL
    );
    waitpid(pid, NULL, 0);
    ptrace(PTRACE_GETREGS, pid, NULL, &procRegs);

    procRegs.rax = PosixSysCalls::MMAP;
    procRegs.rdi = baseAddress;
    procRegs.rsi = byteCount;
    procRegs.rdx = PROT_READ | PROT_WRITE;
    procRegs.r10 = MAP_PRIVATE | MAP_ANONYMOUS;
    procRegs.r8 = -1;
    procRegs.r9 = 0;

    ptrace(PTRACE_SETREGS, pid, NULL, &procRegs);
    ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    waitpid(pid, NULL, 0);
    ptrace(PTRACE_GETREGS, pid, NULL, &procRegs);

    uint64_t allocAddr = procRegs.rax;
    
    if ( (void*)allocAddr == MAP_FAILED ) {
       std::cerr << std::format("Allocation error {}", baseAddress) << std::endl; 
    } else {
        std::cout << "Allocation success: " << std::hex << allocAddr << std::endl;
    }

    ptrace(PTRACE_SETREGS, pid, NULL, &regsBeforeStop);
    return allocAddr;
};

void PosixMem::dealloc(uint64_t addr) {
    
}

void PosixMem::writeMem(uint64_t baseAddress, int8_t* payload, uint64_t byteCount) {

}

int8_t* PosixMem::readMem(uint64_t baseAddress, uint64_t byteCount) {
    return 0;
}

PosixMem::PosixMem(pid_t pid) {
    this->pid = pid;
}
