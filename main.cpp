#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "iwdsr/loader/loader.hpp"
#include "iwdsr/proc/windows/winproc.hpp"

int main() {
    WinProc proc;
    ProcessMemory* m = proc.getMemory();
    m->allocate(0x0000, P_SIZE::GB_1, P_SIZE::KB_4);
    ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    Loader loader;
    loader.loadElf(elf, proc);
    
    for(;;);
}