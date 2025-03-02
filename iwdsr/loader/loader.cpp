#include "loader.hpp"
#include <iostream>
#include <format>

Loader::Loader(Process* proc) {
    this->proc = proc;
    this->symbolResolver = SR::SymbolResolver(proc->getMemory(), &memMap); 
    memMap.setMinimalAddress(0x10000);
    memMap.setMaximumAddress(0x00007FFFFFFFFFFF);
    memMap.createNewRootBlk();
}

void Loader::loadElf(ELF_PARSER::ELF* elfFile) {
    MemBlock* elfMemBlk = memMap.allocate( 0x10000, elfFile->getMemImageSize());

    if ( elfMemBlk == nullptr) {
        std::cout << "Failed to get memory block for executable" << std::endl;
        return;
    }

    loadElfInMemBlk( elfFile, elfMemBlk ); 

    memMap.makeElfMemoryImage( BASE_ELF_MEMORY_IMAGE, elfMemBlk, elfFile );
    for ( auto lib : elfFile->getLibDependencies() ) {
        loadDLL(lib, nullptr);
    }

    symbolResolver.resolveImage( BASE_ELF_MEMORY_IMAGE );
}

void Loader::loadDLL( std::string libName, ELF_PARSER::ELF* dynamicLib) {
    if ( dynamicLib == nullptr) {
        std::cout << std::format("DLL not found: {}", libName) << std::endl;
        return;
    }

    MemBlock* dllMemBlk = memMap.getFreeBlock(dynamicLib->getMemImageSize());
    ProcessMemory* procMem = proc->getMemory();

    if ( dllMemBlk == nullptr ) {
        std::cout << std::format("Failed to get memory block for: {}", libName) << std::endl;
        return;
    }

    loadElfInMemBlk( dynamicLib, dllMemBlk );
    memMap.makeElfMemoryImage( libName, dllMemBlk, dynamicLib );
    for ( auto lib : dynamicLib->getLibDependencies() ) {
        loadDLL(lib, nullptr);
    }

    symbolResolver.resolveImage( libName );
}

void Loader::loadElfInMemBlk(ELF_PARSER::ELF* elf, MemBlock* blk) {
    ProcessMemory* procMem = proc->getMemory();
    for ( auto loadableSection : elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_LOAD)) {
        int8_t* rawSection = elf->rawRead(
            loadableSection->p_offset,
            loadableSection->p_memsz
        );
        procMem->writeMem(
            loadableSection->p_vaddr + blk->getStartAddr(),
            rawSection,
            loadableSection->p_memsz
        );
    }
}

bool Loader::fillLibPool( std::filesystem::path pathToLibDir ) {
    std::filesystem::path absPath = std::filesystem::current_path() / pathToLibDir;
    if ( !std::filesystem::exists(absPath) ){
        std::cout << std::format("Path to lib not exists: {}", absPath.string()) << std::endl;
        return false;
    }
    if (  !std::filesystem::is_directory(absPath) ) {
        std::cout << std::format("Is not a directory: {}", absPath.string()) << std::endl;
        return false;
    }

    for ( const auto& file : std::filesystem::directory_iterator(absPath)) {
        if ( std::filesystem::is_regular_file(file.path()) ){
            std::cout << std::format( "Found lib: {}", file.path().filename().string()) << std::endl;
            this->libPool[file.path().filename().string()] = new ELF_PARSER::ELF(file);
        }
    }
    return true;
}