#include <iostream>
#include <fstream>
#include "iwdsr/file-formats/elf64.hpp"
int main() {
    ELF elf(std::filesystem::path("/mnt/nvme0n1p4/development/iwds/a.out"));
}