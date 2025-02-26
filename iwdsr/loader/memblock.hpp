#pragma once

#include <cstdint>
#include <string>
#include "../file-formats/elf64.hpp"

class MemBlock {
    public:
    uint64_t getStartAddr(); 
};

class GlobalOffsetTable {
    public:
    void changePtr(uint64_t entryId, uint64_t value);
};

class NamedImage {
    private:
    std::string imageName;
    ELF_PARSER::ELF* originElf;
    MemBlock* bkl;
    GlobalOffsetTable* globalOffsetTable; 

    public:
    GlobalOffsetTable* getGlobalOffsetTable();
    uint64_t getSymbolAddressByName( std::string symbolName);
    
    ELF_PARSER::ELF* getOriginElf();

};

class MemoryMap{
    public:
    void makeNamedMapping( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf);
    NamedImage* getImage( std::string );
    bool isImageExists( std::string imageName);
    MemBlock* getFreeBlock(uint64_t size);
    MemBlock* allocate(uint64_t baseAddr, uint64_t lenght);
    void unalloc( MemBlock* blk);
    
};