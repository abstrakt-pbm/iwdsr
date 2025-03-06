#pragma once

#include <cstdint>
#include <string>
#include "../file-formats/elf64.hpp"


enum MemBlkPermissions {
    NOACCESS,
    R,
    W,
    E,
    RW,
    RE,
    WE,
    RWE
};

enum MemBlkPageSize : uint64_t {
    KB4 = 4096,
    KB16 = 16384,
    KB64 = 65536,
    MB2 = 1048576,
    GB1 = 1073741824
};

enum MemBlkState {
    FREE,
    ALLOCATED,
    RESERVED
};

class MemBlock {
    private:
    MemBlkState state;
    MemBlkPermissions permission;
    uint64_t startAddr;
    MemBlock* leftBlk;
    MemBlock* rightBlk;
    MemBlock* childBlk;

    uint64_t lenght;

    public:
    MemBlock( uint64_t startAddr, uint64_t lenght, MemBlkState state, MemBlkPermissions permission, MemBlock* lBlk = nullptr, MemBlock* rBlk = nullptr );
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

MemBlkPageSize allignmentToPageSize( uint64_t allignment );

class MemoryMap{
    private: 

    uint64_t minimalAddr;
    uint64_t maximumAddr;

    std::unordered_map<std::string, ElfMemoryImage*> images;
    MemBlock* rootBlk;

    uint64_t calculateAllignedLenght( uint64_t baseLenght, MemBlkPageSize psize );

    public:
    MemoryMap() = default;
    MemoryMap( uint64_t minimalAddr, uint64_t maximumAddr );

    void makeElfMemoryImage( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf);
    ElfMemoryImage* getImage( std::string );
    std::vector<ElfMemoryImage*> getImages();

    bool isImageExists( std::string imageName);
    MemBlock* getBlkContainingAddr( uint64_t addr );
    MemBlock* getFreeBlock(uint64_t size);

    MemBlock* allocate(uint64_t baseAddr, uint64_t lenght, MemBlkPageSize psize);
    void unalloc( MemBlock* blk );
    MemBlock* reserve( uint64_t baseAddr, uint64_t lenght, MemBlkPageSize psize );
    MemBlock* reserve( uint64_t lenght, MemBlkPageSize psize );

    void setMinimalAddress( uint64_t addr );
    void setMaximumAddress( uint64_t addr );
    void createNewRootBlk();
    
};