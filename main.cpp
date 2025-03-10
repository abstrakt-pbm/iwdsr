#include <iostream>
#include "iwdsr/loader/loader.hpp"
#include "iwdsr/proc/mock/mockproc.hpp"

int main() {
    MockProcess proc;
    ELF_PARSER::ELF elf("/mnt/nvme0n1p4/development/iwdsr/testElf.out");

    Loader ld(&proc);
    ld.fillLibPool("libsss");
    ld.loadElf(&elf);
    
    proc.createMainThread();
    //proc.start();
}

// has big problem with separate askii symbols and making dynsymb tab with names