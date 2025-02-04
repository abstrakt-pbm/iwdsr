#include "program.hpp"
#include "windows.h"

ProgramMemory* Program::getMemory() {
    return mem;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinProgram::WinProgram() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );

    si.cb = sizeof(si);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = (LPSTR)("Game Core");
    

    bool isProcessCreated = CreateProcessA(
        (LPCSTR)("Game Core"),
        NULL,
        NULL,
        NULL,
        FALSE,
        HIGH_PRIORITY_CLASS,
        NULL,
        NULL,
        &si,
        &pi
    );
    mem = new WinProcMemory();
}


int64_t WinProgram::readMemory() {
    return 19;
}

bool WinProgram::writeMemory() {
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////