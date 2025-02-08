#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "iwdsr/proc/windows/winproc.hpp"
#include "iwdsr/loader/loader.hpp"

int main() {
    Process* proc = new WinProc();
    ProcessMemory* m = proc->getMemory();
    ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    Loader loader;
    loader.loadElf(elf, *proc);
    for(;;);
}