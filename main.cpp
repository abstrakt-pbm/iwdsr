#include <iostream>
#include "iwdsr/loader/loader.hpp"
//#include "iwdsr/proc/windows/winproc.hpp"
#include "iwdsr/proc/mock/mockproc.hpp"

int main() {
    //WinProc proc;
    //ProcessMemory* m = proc.getMemory();
    //m->allocate(0x0000, P_SIZE::GB_1, P_SIZE::KB_4);
    MockProcess proc;
    ELF_PARSER::ELF elf("/mnt/nvme0n1p4/development/iwdsr/testElf.out");
    Loader ld(&proc);
    ld.fillLibPool("libsss");
    ld.loadElf(&elf);
    //for(;;);
}