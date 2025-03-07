#include "memblock.hpp"
#include <format>
#include <iostream>
#include <cmath>

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
    this->rootBlk = new MemBlock( minimumAddr, maximumAddr, MemBlkState::FREE, MemBlkPermissions::NOACCESS);
}

ElfMemoryImage* MemoryMap::getImage( std::string imageName) {
    if ( images.contains(imageName) ){ 
        return this->images[imageName]; 
    }
    return nullptr;
}

MemBlock* MemoryMap::allocate(uint64_t baseAddr, uint64_t lenght, MemBlkPageSize psize, MemBlkPermissions permission) {
    if ( baseAddr > this->maximumAddr || baseAddr < minimalAddr ) {
        std::cout << std::format("Allocation error | Address out of range: {:X}", baseAddr) << std::endl;
        return nullptr;
    }

    uint64_t allignedLenght = calculateAllignLeft(lenght); 
    if ( baseAddr + allignedLenght > maximumAddr ) {
        std::cout << std::format("Allocation error | overflow");
        return nullptr;
    }

    MemBlock* targetBlk = getBlkContainingAddr( baseAddr );
    if ( targetBlk->getState() == MemBlkState::FREE ) {
        std::cout << std::format("Allocation error | Memory Unreserved: {:X}", baseAddr) << std::endl;
        return nullptr;
    }

    uint64_t calcLeftLim = calculateAllignLeft( baseAddr );
    uint64_t calcLenght = calculateAllignedLenght( baseAddr - calcLeftLim + lenght, psize );

    MemBlock* allocatedBlk = new MemBlock(
         calcLeftLim,
         calcLenght,
         MemBlkState::ALLOCATED,
         permission
    );

    insertMemBlk( allocatedBlk, targetBlk->getChild());

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
        if ( addr >= currentBlk->getStartAddr() && addr < currentBlk->getFinishAddr() ) {
            break;
        }
        currentBlk = currentBlk->getRightBlk();
    }
    return currentBlk;
}

std::vector<ElfMemoryImage*> MemoryMap::getImages() {
    std::vector<ElfMemoryImage*> imgs;
    for ( auto img : images ) {
        if (img.second != nullptr) {
            imgs.push_back(img.second);
        }
    }
    return imgs;
}

void MemoryMap::setMinimalAddress(uint64_t addr) {
    this->minimalAddr = addr;
}

void MemoryMap::setMaximumAddress(uint64_t addr) {
    this->maximumAddr = addr;
}

void MemoryMap::createNewRootBlk() {
    this->rootBlk = new MemBlock(
        minimalAddr, 
        maximumAddr - minimalAddr,
        MemBlkState::FREE,
        MemBlkPermissions::NOACCESS
    );
}

MemBlock* MemoryMap::reserve( uint64_t baseAddr, uint64_t lenght, MemBlkPageSize psize ) {
    uint64_t allignedLeftLim = calculateAllignLeft( baseAddr );
    uint64_t allignedLenght = calculateAllignedLenght( baseAddr - allignedLeftLim + lenght, psize );

    if ( allignedLeftLim < minimalAddr || allignedLeftLim + allignedLenght - 1 > maximumAddr ) {
        std::cout << "Reservation error | Out of range" << std::endl;
    }

    MemBlock* targetBlk = getBlkContainingAddr( baseAddr );
    if ( targetBlk == nullptr ) {
        std::cout << std::format( "Failed to find blk to reserve" ) << std::endl;
        return nullptr;
    }

    if ( targetBlk->getState() == MemBlkState::RESERVED ) {
        std::cout << std::format("Address already resrved") << std::endl;
        return nullptr;
    }
    
    MemBlock* reservedBlkChild = new MemBlock(
        allignedLeftLim,
        allignedLenght,
        MemBlkState::FREE,
        MemBlkPermissions::NOACCESS
    );
    
    MemBlock* reservedBlk = new MemBlock(
        allignedLeftLim,
        allignedLenght,
        MemBlkState::RESERVED,
        MemBlkPermissions::NOACCESS,
        nullptr,
        reservedBlkChild
    );
    reservedBlkChild->setFather(reservedBlk);

    insertMemBlk( reservedBlk, targetBlk );

    return reservedBlk;
}

MemBlock* MemoryMap::reserve( uint64_t lenght, MemBlkPageSize psize ) {

    MemBlock* freeBlk = getFreeBlock( lenght );
    uint64_t allignedLenght = calculateAllignedLenght( lenght, psize );
    
    MemBlock* reservedBlkChild = new MemBlock(
        freeBlk->getStartAddr(),
        allignedLenght,
        MemBlkState::FREE,
        MemBlkPermissions::NOACCESS
    );
    
    MemBlock* reservedBlk = new MemBlock(
        freeBlk->getStartAddr(),
        allignedLenght,
        MemBlkState::RESERVED,
        MemBlkPermissions::NOACCESS,
        reservedBlkChild
    );
    reservedBlkChild->setFather(reservedBlk);

    insertMemBlk( reservedBlk, freeBlk );
    return reservedBlk;
}

void MemoryMap::insertMemBlk( MemBlock* blkToInsert, MemBlock* blkWhereInsert ) {
    uint64_t allignedLeftLim = blkToInsert->getStartAddr();
    uint64_t allignedLenght = blkToInsert->getLenght();

    blkWhereInsert->setLenght( std::abs((int64_t)(blkWhereInsert->getLenght() - allignedLenght)) );

    // случий когда алоцированый блок равен длинне резервации


    if ( allignedLeftLim + allignedLenght - 1 == maximumAddr ) {
        blkWhereInsert->setRightBlk(blkToInsert);
        blkToInsert->setLeftBlk(blkWhereInsert);
    }
    
    if ( allignedLeftLim == minimalAddr && blkWhereInsert->getFather() == nullptr ) {
        blkWhereInsert->setStartAddr(allignedLeftLim + allignedLenght);
        blkWhereInsert->setLeftBlk(blkToInsert);
        blkToInsert->setRightBlk(blkWhereInsert);
        rootBlk = blkToInsert;
    }

    if ( blkWhereInsert->getFather() != nullptr && blkWhereInsert->getFather()->getStartAddr() == allignedLeftLim ) {
        blkWhereInsert->setStartAddr(allignedLeftLim + allignedLenght);
        blkWhereInsert->setLeftBlk(blkToInsert);
        blkToInsert->setRightBlk(blkWhereInsert);
        blkWhereInsert->getFather()->setChild(blkToInsert); 
    }


    if ( allignedLeftLim > minimalAddr && allignedLeftLim + allignedLenght - 1 < maximumAddr ) {
        MemBlock* blkLeftFromBlkToInsert = new MemBlock(
           blkWhereInsert->getStartAddr(),
           blkToInsert->getStartAddr() - blkWhereInsert->getStartAddr(),
           MemBlkState::FREE,
           MemBlkPermissions::NOACCESS,
           blkWhereInsert->getLeftBlk(),
           blkToInsert
        );

        blkToInsert->setLeftBlk( blkLeftFromBlkToInsert );
        blkToInsert->setRightBlk( blkWhereInsert );
        blkWhereInsert->setStartAddr( allignedLeftLim + allignedLenght );
        blkWhereInsert->setLeftBlk( blkToInsert );
    }

    if (blkToInsert->getRightBlk()->getLenght() == 0) {
        delete blkToInsert->getRightBlk();
        blkToInsert->setRightBlk(nullptr);
    }
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
    if ( originElf->isExportSymbolByName( symbolName ) ){
        ELF_PARSER::DynamicSymbol* symbol = originElf->getDynSymbolByName(symbolName);
        addr = blk->getStartAddr() + symbol->getBaseAddr();
    }
    return addr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemBlock::MemBlock( uint64_t startAddr, uint64_t lenght, MemBlkState state, MemBlkPermissions permission, MemBlock* father , MemBlock* childBlk, MemBlock* lBlk, MemBlock* rBlk ) {
    this->state = state;
    this->permission = permission;
    this->startAddr = startAddr;
    this->lenght = lenght;
    this->leftBlk = lBlk;
    this->rightBlk = rBlk;
    this->childBlk = childBlk;
    this->father = father;
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

MemBlock* MemBlock::getChild() {
    return this->childBlk;
}

MemBlock* MemBlock::getFather() {
    return this->father;
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

void MemBlock::setChild( MemBlock* childBlk ) {
    this->childBlk = childBlk;
}

void MemBlock::setFather( MemBlock* father ) {
    this->father = father;
}

void MemBlock::setStartAddr( uint64_t startAddr ) {
    this->startAddr = startAddr;
}

uint64_t MemoryMap::calculateAllignLeft( uint64_t startAddr) {
    return previousPow2(startAddr);
}

uint64_t MemoryMap::calculateAllignedLenght( uint64_t baseLenght, MemBlkPageSize psize ) {
    uint64_t pages = baseLenght / psize;
    if ( baseLenght % psize > 0 ) {
        pages += 1;
    }

    return pages * psize;
}

MemBlkPageSize allignmentToPageSize( uint64_t allignment ) {
    return static_cast<MemBlkPageSize>(allignment);
}

uint64_t previousPow2( uint64_t n ) {
    if (n == 0) return 0;
    while (n & (n - 1)) {
        n &= n - 1;
    }
    return n;
}