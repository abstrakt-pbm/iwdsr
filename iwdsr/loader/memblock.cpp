#include "memblock.hpp"
#include <format>
#include <iostream>

void MemoryMap::makeElfMemoryImage( std::string name, MemBlock* blk, ELF_PARSER::ELF* originElf) {
    if ( !images.contains( name ) ) {
        images[name] = new ElfMemoryImage( name, originElf, blk );
    }
}

bool MemoryMap::isImageExists( std::string imageName) {
    return images.contains( imageName );
}

MemoryMap::MemoryMap( uint64_t minimumAddr, uint64_t maximumAddr ) {
    this->minimalAddr = minimumAddr;
    this->maximumAddr = maximumAddr;
    this->rootBlk = new MemBlock( minimumAddr, maximumAddr, MemBlkState::FREE );
}

ElfMemoryImage* MemoryMap::getImage( std::string imageName) {
    return this->images[imageName]; 
}

MemBlock* MemoryMap::allocate(uint64_t baseAddr, uint64_t lenght) {
    if (baseAddr > this->maximumAddr || baseAddr < minimalAddr) {
        std::cout << std::format("Allocation error | Address out of range: {:X}", baseAddr) << std::endl;
        return nullptr;
    }

    MemBlock* targetBlk = getBlkContainingAddr( baseAddr );
    if ( targetBlk->getState() == MemBlkState::ALLOCATED ) {
        std::cout << std::format("Allocation error | Address already allocated: {:X}", baseAddr) << std::endl; 
        return nullptr;
    }

    MemBlock* allocatedBlk = nullptr;
    MemBlock* leftFromBlk = nullptr;
    if ( baseAddr > targetBlk->getStartAddr() && baseAddr < targetBlk->getFinishAddr()) {
        MemBlock* leftFromBlk = new MemBlock(
            targetBlk->getStartAddr(),
            baseAddr - targetBlk->getStartAddr() - 1,
            MemBlkState::FREE,
            targetBlk->getLeftBlk()
        );
        
    } else if ( baseAddr == targetBlk->getStartAddr() ) {
        leftFromBlk = targetBlk->getLeftBlk();
    }

    allocatedBlk = new MemBlock(
            baseAddr,
            lenght,
            MemBlkState::ALLOCATED,
            leftFromBlk,
            targetBlk
    );

    leftFromBlk->setRightBlk(allocatedBlk);
    targetBlk->setLeftBlk(allocatedBlk);
    targetBlk->setStartAddr(baseAddr + lenght - 1);
    targetBlk->setLenght(targetBlk->getLenght() - lenght);

    return allocatedBlk;
}

MemBlock* MemoryMap::getFreeBlock( uint64_t size ) {
    MemBlock* currentMemBlk = this->rootBlk;
    while ( currentMemBlk != nullptr ) {
        if ( currentMemBlk->getState() == MemBlkState::FREE && currentMemBlk->getLenght() >= size ) {
            break;;
        }
        currentMemBlk = currentMemBlk->getRightBlk();
    }
    return currentMemBlk;
}


MemBlock* MemoryMap::getBlkContainingAddr( uint64_t addr ) {
    MemBlock* currentBlk = rootBlk;
    while( currentBlk != nullptr ) {
        if ( addr > currentBlk->getStartAddr() && addr < currentBlk->getFinishAddr() ) {
            break;
        }
        currentBlk = currentBlk->getRightBlk();
    }
    return currentBlk;
}

std::vector<ElfMemoryImage*> MemoryMap::getImages() {
    std::vector<ElfMemoryImage*> imgs(images.size());
    int i = 0;
    for ( auto img : images ) {
        imgs[i] = img.second;
        i++;
    }
    return imgs;
}

void MemoryMap::setMinimalAddress(uint64_t addr) {
    this->minimalAddr = addr;
}

void MemoryMap::setMaximumAddress(uint64_t addr) {
    this->maximumAddr = addr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ElfMemoryImage::ElfMemoryImage( std::string imageName, ELF_PARSER::ELF* originElf, MemBlock* blk ) {
    this->imageName = imageName;
    this->originElf = originElf;
    this->blk = blk;
}

ELF_PARSER::ELF* ElfMemoryImage::getOriginElf() {
    return this->originElf;
}

uint64_t ElfMemoryImage::getBaseAddr() {
    return this->blk->getStartAddr();
}

uint64_t ElfMemoryImage::getSymbolAddressByName( std::string symbolName) {
    uint64_t addr = 0;
    if ( originElf->containSymbolByName( symbolName ) ){
        ELF_PARSER::Symbol* symbol = originElf->getSymbolByName(symbolName);  
        addr = blk->getStartAddr() + symbol->getBaseAddr();
    }
    return addr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemBlock::MemBlock( uint64_t startAddr, uint64_t lenght, MemBlkState state, MemBlock* lBlk, MemBlock* rBlk ) {
    this->state = state;
    this->startAddr = startAddr;
    this->lenght = lenght;
    this->leftBlk = lBlk;
    this->rightBlk = rBlk;
}

uint64_t MemBlock::getStartAddr() {
    return this->startAddr;
} 

uint64_t MemBlock::getLenght() {
    return this->lenght;
}

MemBlock* MemBlock::getLeftBlk() {
    return this->leftBlk; 
}

MemBlock* MemBlock::getRightBlk() {
    return this->rightBlk;
}

MemBlkState MemBlock::getState() {
    return this->state;
}

uint64_t MemBlock::getFinishAddr() {
    return this->startAddr + lenght;
}

void MemBlock::setLeftBlk( MemBlock* blk) {
    this->leftBlk = blk;
}

void MemBlock::setRightBlk( MemBlock* blk ) {
    this->rightBlk = blk;
}

void MemBlock::setLenght( uint64_t lenght ) {
    this->lenght = lenght;
}

void MemBlock::setStartAddr( uint64_t startAddr ) {
    this->startAddr = startAddr;
}

