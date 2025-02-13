#include "posixproc.hpp"
#include "posixmem.hpp"
#include <iostream>
#include <format>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ptrace.h>


int clearAddressSpace(void *arg) {
    printf("[Child] PID: %d (PPID: %d)\n", getpid(), getppid());

    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        perror("fopen");
        return 1;
    }
    
    unsigned long start, end;
    while (fscanf(maps, "%lx-%lx ", &start, &end) != EOF) {
        munmap((void *)start, end - start);
    }

    fclose(maps);

    //void *new_memory = mmap((void *)0x400000, 0x1000,
    //                        PROT_READ | PROT_WRITE | PROT_EXEC,
    //                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    printf("[Child] Адресное пространство очищено!\n");
    return 0;
}


PosixProc::PosixProc() {
    int *procStack = new int[1024*1024];

    pid = clone(
        clearAddressSpace,        
        procStack + 1024*1024,
        0,
        NULL
    );

    if ( pid <= 0) {
        std::cout << "Error while creating process" << std::endl;
    } else {
        std::cout << std::format("Created process: {}", pid) << std::endl;
    }
    mem = new PosixMem(pid);
}

void PosixProc::start() {

}

void PosixProc::stop() {

}
