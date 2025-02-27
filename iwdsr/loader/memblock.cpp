#include "memblock.hpp"



MemBlock* MemoryMap::allocate(uint64_t baseAddr, uint64_t lenght) { 
    
}

void MemoryMap::makeElfMemoryImage( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf) {
    if ( !images.contains( name ) ) {
        images[name] = new ElfMemoryImage( name, originElf, blk );
    }
}


bool MemoryMap::isImageExists( std::string imageName) {
    return images.contains( imageName );
}

MemoryMap::MemoryMap( uint64_t leftLim, uint64_t rightLim ) {
    this->rootBlk = new MemBlock( leftLim, rightLim );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ElfMemoryImage::ElfMemoryImage( std::string imageName, ELF_PARSER::ELF* originElf, MemBlock* blk ) {
    this->imageName = imageName;
    this->originElf = originElf;
    this->blk = blk;
}