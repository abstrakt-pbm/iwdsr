#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "iwdsr/loader/loader.hpp"
#include "iwdsr/proc/posix/posixproc.hpp"

int main() {
    PosixProc proc;
    ProcessMemory* m = proc.getMemory();
    m->allocate(0x10000, P_SIZE::GB_1, P_SIZE::KB_4);
    //ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    //Loader loader;
    //loader.loadElf(elf, *proc);
    
    for(;;);
}