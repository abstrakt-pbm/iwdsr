#pragma once

#include <cstdint>
#include <string>
#include "../file-formats/elf64.hpp"

class MemBlock {
    public:
    MemBlock( uint64_t leftLim, uint64_t rightLim );
    uint64_t getStartAddr(); 
};

class GlobalOffsetTable {
    private:

    public:
    void changePtr(uint64_t entryId, uint64_t value);
};

class ElfMemoryImage {
    private:
    std::string imageName;
    ELF_PARSER::ELF* originElf;
    MemBlock* blk;
    GlobalOffsetTable* globalOffsetTable; 

    public:
    ElfMemoryImage( std::string imageName, ELF_PARSER::ELF* originElf, MemBlock* blk );
    GlobalOffsetTable* getGlobalOffsetTable();
    uint64_t getSymbolAddressByName( std::string symbolName);
    
    ELF_PARSER::ELF* getOriginElf();

};

class MemoryMap{
    private: 
    std::unordered_map<std::string, ElfMemoryImage*> images;
    MemBlock* rootBlk;

    public:
    MemoryMap(uint64_t leftLim, uint64_t rightLim);

    void makeElfMemoryImage( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf);
    ElfMemoryImage* getImage( std::string );
    bool isImageExists( std::string imageName);
    MemBlock* getFreeBlock(uint64_t size);
    MemBlock* allocate(uint64_t baseAddr, uint64_t lenght);
    void unalloc( MemBlock* blk);
    
};