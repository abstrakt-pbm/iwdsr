#pragma once

#include <cstdint>
#include <string>
#include "../file-formats/elf64.hpp"

enum MemBlkState {
    FREE,
    ALLOCATED
};

class MemBlock {
    private:
    MemBlkState state;

    uint64_t startAddr;
    MemBlock* leftBlk;
    MemBlock* rightBlk;
    uint64_t lenght;

    public:
    MemBlock( uint64_t startAddr, uint64_t lenght, MemBlkState state, MemBlock* lBlk = nullptr, MemBlock* rBlk = nullptr );
    uint64_t getStartAddr(); 
    uint64_t getFinishAddr();
    uint64_t getLenght();
    MemBlock* getLeftBlk();
    MemBlock* getRightBlk();
    MemBlkState getState();

    void setLeftBlk( MemBlock* );
    void setRightBlk( MemBlock* );
    void setStartAddr( uint64_t );
    void setLenght( uint64_t );

};


class ElfMemoryImage {
    private:
    std::string imageName;
    ELF_PARSER::ELF* originElf;
    MemBlock* blk;

    public:
    ElfMemoryImage( std::string imageName, ELF_PARSER::ELF* originElf, MemBlock* blk );
    uint64_t getSymbolAddressByName( std::string symbolName);
    uint64_t getDynSymAddressById( uint16_t id );

    uint64_t getGlobalOffsetTableBaseAddr();
    uint64_t getBaseAddr();
    ELF_PARSER::ELF* getOriginElf();

};

class MemoryMap{
    private: 

    uint64_t minimalAddr;
    uint64_t maximumAddr;

    std::unordered_map<std::string, ElfMemoryImage*> images;
    MemBlock* rootBlk;

    public:
    MemoryMap() = default;
    MemoryMap( uint64_t minimalAddr, uint64_t maximumAddr );

    void makeElfMemoryImage( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf);
    ElfMemoryImage* getImage( std::string );
    std::vector<ElfMemoryImage*> getImages();

    bool isImageExists( std::string imageName);
    MemBlock* getBlkContainingAddr( uint64_t addr );
    MemBlock* getFreeBlock(uint64_t size);
    MemBlock* allocate(uint64_t baseAddr, uint64_t lenght);
    void unalloc( MemBlock* blk);

    void setMinimalAddress(uint64_t addr);
    void setMaximumAddress(uint64_t addr);
    
};