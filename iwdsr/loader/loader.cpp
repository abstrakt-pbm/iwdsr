#include "loader.hpp"
#include <iostream>
#include <format>

Loader::Loader(Process* proc) {
    this->proc = proc;
    proc->getMemory()->setBaseOffset(0x10000);
    this->symbolResolver = SR::SymbolResolver(proc->getMemory(), &memMap); 
    memMap.setMinimalAddress(0x10000);
    memMap.setMaximumAddress(0x00007FFFFFFFFFFF);
    memMap.createNewRootBlk();
}

void Loader::loadElf(ELF_PARSER::ELF* elfFile) {
    MemBlock* elfMemBlk = memMap.reserve( 0x10000, elfFile->getMemImageSize(), MemBlkPageSize::KB4 );

    if ( elfMemBlk == nullptr) {
        std::cout << "Failed to reserve memory block for executable" << std::endl;
        return;
    }

    loadElfInMemBlk( elfFile, elfMemBlk );

    memMap.makeElfMemoryImage( BASE_ELF_MEMORY_IMAGE, elfMemBlk, elfFile );

    for ( auto lib : elfFile->getLibDependencies() ) {
        loadDLL(lib, libPool[lib]);
    }

    symbolResolver.resolveImage( BASE_ELF_MEMORY_IMAGE );
}

void Loader::loadDLL( std::string libName, ELF_PARSER::ELF* dynamicLib) {
    if ( dynamicLib == nullptr) {
        std::cout << std::format("DLL not found: {}", libName) << std::endl;
        return;
    }
    
    std::cout << std::format("Start loading DLL: {}", libName) << std::endl;

    MemBlock* dllMemBlk = memMap.reserve(dynamicLib->getMemImageSize(), MemBlkPageSize::KB4);
    if ( dllMemBlk == nullptr ) {
        std::cout << std::format("Failed to reserve memory for DLL: {}", libName) << std::endl;
        return;
    }

    ProcessMemory* procMem = proc->getMemory();

    loadElfInMemBlk( dynamicLib, dllMemBlk );
    memMap.makeElfMemoryImage( libName, dllMemBlk, dynamicLib );
    for ( auto lib : dynamicLib->getLibDependencies() ) {
        loadDLL(lib, libPool[lib]);
    }

    symbolResolver.resolveImage( libName );
}

void Loader::loadElfInMemBlk(ELF_PARSER::ELF* elf, MemBlock* blk) {
    ProcessMemory* procMem = proc->getMemory();
    procMem->reserve( blk->getStartAddr(), blk->getLenght(), P_SIZE::KB_4);
    std::vector<ELF_PARSER::ProgramHeader*> ptDynamicSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_DYNAMIC);
    std::vector<ELF_PARSER::ProgramHeader*> ptTLSSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_TLS);
    std::vector<ELF_PARSER::ProgramHeader*> ptRELROSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_GNU_RELRO);
    std::vector<ELF_PARSER::ProgramHeader*> ptGnuStackSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_GNU_STACK);
    std::vector<ELF_PARSER::ProgramHeader*> ptEhFrameSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_GNU_EH_FRAME);
    std::vector<ELF_PARSER::ProgramHeader*> ptPropertySections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_GNU_PROPERTY);

    std::vector<ELF_PARSER::ProgramHeader*> loadableSections = elf->getProgramHeadersByPType(ELF_PARSER::P_TYPE::PT_LOAD);

    loadableSections.insert(
        loadableSections.end(),
        ptDynamicSections.begin(),
        ptDynamicSections.end()
    );

    loadableSections.insert(
        loadableSections.end(),
        ptTLSSections.begin(),
        ptTLSSections.end()
    );

    loadableSections.insert(
        loadableSections.end(),
        ptRELROSections.begin(),
        ptRELROSections.end()
    );

    loadableSections.insert(
        loadableSections.end(),
        ptGnuStackSections.begin(),
        ptGnuStackSections.end()
    );

    loadableSections.insert(
        loadableSections.end(),
        ptEhFrameSections.begin(),
        ptEhFrameSections.end()
    );

    loadableSections.insert(
        loadableSections.end(),
        ptPropertySections.begin(),
        ptPropertySections.end()
    );

    for ( auto loadableSection : loadableSections ) {
        int8_t* rawSection = elf->rawRead(
            loadableSection->p_offset,
            loadableSection->p_memsz
        );

        memMap.allocate(
            blk->getStartAddr() + loadableSection->p_offset,
            loadableSection->p_memsz,
            allignmentToPageSize(loadableSection->p_align),
            MemBlkPermissions::RWE
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