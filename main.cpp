#include <iostream>
#include "iwdsr/loader/loader.hpp"
#include "iwdsr/proc/mock/mockproc.hpp"

int main() {
    MockProcess proc;
    ELF_PARSER::ELF elf("C:\\making\\iwdsr\\a.out");

    Loader ld(&proc);
    ld.fillLibPool("libsss");
    ld.loadElf(&elf);
    
    proc.createMainThread();
}

// has big problem with separate askii symbols and making dynsymb tab with names