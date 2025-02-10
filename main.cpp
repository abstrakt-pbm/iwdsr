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
    m->allocate(0x0, 0x1000 , PAGE_SIZE::KB_4 );
    int8_t wr = 10;
    m->writeMem(0, &wr, 1);
    char reader =  *(m->readMem(0,1));
    std::cout << "Readed: " << (int)reader << std::endl;
    //ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    //Loader loader;
    //loader.loadElf(elf, *proc);
    //for(;;);
}