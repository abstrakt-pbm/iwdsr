#include "proc.hpp"
#include "windows.h"
#include <iostream>

ProcessMemory* Process::getMemory() {
    return mem;
}