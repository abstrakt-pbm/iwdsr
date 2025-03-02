#include <iostream>
#include "iwdsr/loader/loader.hpp"
#include "iwdsr/proc/windows/winproc.hpp"

int main() {
    WinProc proc;
    ProcessMemory* m = proc.getMemory();
    //m->allocate(0x0000, P_SIZE::GB_1, P_SIZE::KB_4);
    ELF_PARSER::ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    Loader ld(&proc);
    ld.fillLibPool("libsss");
    ld.loadElf(&elf); 
    //for(;;);
}