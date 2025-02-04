#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <chrono>
#include "iwdsr/program/program.hpp"
#include "iwdsr/loader/loader.hpp"

int main() {
    WinProgram prog;
    ELF elf("C:\\Users\\pyumi\\Downloads\\a.out");
    Loader loader;
    loader.loadElf(elf, prog);
}