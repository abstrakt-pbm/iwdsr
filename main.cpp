#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "iwdsr/proc/windows/winproc.hpp"
#include "iwdsr/loader/loader.hpp"

int main() {
    Process* proc = new WinProc();
    //ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    //Loader loader;
    //loader.loadElf(elf, prog);
    //ProcessMemory* emm = proc.getMemory();
    //emm->allocate(0x10000, 0x1000, PAGE_SIZE::KB_4);
}