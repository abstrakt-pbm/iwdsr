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

    if ( targetBlk == nullptr ) {
        std::cout << std::format("Failed to find blk containing addr: {:#x}", baseAddr) << std::endl;
    }

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

    MemBlock* blkWhereInsert = targetBlk->getChild();

    while( blkWhereInsert != nullptr ) {
        if ( blkWhereInsert->getState() == MemBlkState::FREE ){
            break;
        }
        blkWhereInsert = blkWhereInsert->getRightBlk();
    }

    if ( blkWhereInsert == nullptr ) {
        std::cout << std::format("Failed to find free area") << std::endl;
        return nullptr;
    }
    allocatedBlk->setFather(targetBlk);
    insertMemBlk( allocatedBlk, blkWhereInsert);

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
        if ( addr >= currentBlk->getStartAddr() && addr <= currentBlk->getFinishAddr() ) {
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
    std::cout << std::format("Reserve memory: {:#x} - {:#x}", allignedLeftLim, allignedLeftLim + allignedLenght ) << std::endl;

    if ( allignedLeftLim < minimalAddr || allignedLeftLim + allignedLenght - 1 > maximumAddr ) {
        std::cout << "Reservation error | Out of range" << std::endl;
    }

    MemBlock* targetBlk = getBlkContainingAddr( baseAddr );
    if ( targetBlk == nullptr ) {
        std::cout << std::format( "Failed to find blk to reserve" ) << std::endl;
        return nullptr;
    }

    if ( targetBlk->getState() == MemBlkState::RESERVED ) {
        std::cout << std::format("Address already reserved") << std::endl;
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
    if ( freeBlk == nullptr ) {
        std::cout << std::format("Not enough memory") << std::endl;
        return nullptr;
    }

    uint64_t allignedLenght = calculateAllignedLenght( lenght, psize );
    
    std::cout << std::format("Reserve memory: {:#x} - {:#x}", freeBlk->getStartAddr(), freeBlk->getStartAddr() + allignedLenght ) << std::endl;

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
        nullptr,
        reservedBlkChild
    );
    reservedBlkChild->setFather(reservedBlk);

    insertMemBlk( reservedBlk, freeBlk );
    return reservedBlk;
}

void MemoryMap::insertMemBlk( MemBlock* blkToInsert, MemBlock* blkWhereInsert ) { 
    uint64_t startAddr = blkToInsert->getStartAddr();
    uint64_t lenght = blkToInsert->getLenght();

    uint64_t bwiInitialStartAddr = blkWhereInsert->getStartAddr();
    MemBlock* blkLeftFromBWI = blkWhereInsert->getLeftBlk();
    MemBlock* blkRightFromBWI = blkWhereInsert->getRightBlk();

    if ( blkWhereInsert->getStartAddr() == startAddr && blkWhereInsert->getLenght() == lenght ) {
        blkWhereInsert->setState(blkToInsert->getState());
        return;
    }

    if ( blkWhereInsert->getStartAddr() == startAddr ) {
        blkWhereInsert->setLenght( blkWhereInsert->getLenght() - lenght );
        blkWhereInsert->setStartAddr(startAddr + lenght);
        blkWhereInsert->setLeftBlk(blkToInsert);
        blkToInsert->setRightBlk(blkWhereInsert);
        blkToInsert->setLeftBlk(blkLeftFromBWI);
        if ( blkLeftFromBWI != nullptr ){
            blkLeftFromBWI->setRightBlk(blkToInsert);
        }

        if ( blkWhereInsert->getFather() == nullptr && minimalAddr == startAddr ) {
            rootBlk = blkToInsert;
        } else if ( blkWhereInsert->getFather() != nullptr && bwiInitialStartAddr == startAddr ){
            blkWhereInsert->getFather()->setChild(blkToInsert);
        }


    }

    if ( blkToInsert->getFinishAddr() == blkWhereInsert->getFinishAddr() ) {
        blkWhereInsert->setLenght( blkWhereInsert->getLenght() - lenght );
        blkWhereInsert->setRightBlk(blkToInsert);
        blkToInsert->setLeftBlk(blkWhereInsert);
        blkToInsert->setRightBlk(blkRightFromBWI);
        if ( blkRightFromBWI != nullptr ) {
            blkRightFromBWI->setLeftBlk(blkToInsert); 
        }
    }

    if ( startAddr > blkWhereInsert->getStartAddr() && blkToInsert->getFinishAddr() < blkWhereInsert->getFinishAddr() ) {
        MemBlock* splitBlk = new MemBlock(
            blkToInsert->getFinishAddr() + 1,
            blkWhereInsert->getLenght() - blkToInsert->getFinishAddr() + 1,
            blkWhereInsert->getState(),
            blkWhereInsert->getPermissions(),
            nullptr,
            nullptr
        );
        
        blkWhereInsert->setLenght( blkWhereInsert->getLenght() - lenght );
        blkWhereInsert->setRightBlk(blkToInsert);
        blkToInsert->setLeftBlk(blkWhereInsert);
        splitBlk->setLeftBlk(blkToInsert);
        blkToInsert->setRightBlk(splitBlk);
        splitBlk->setRightBlk(blkRightFromBWI);
        if ( blkRightFromBWI != nullptr ) {
            blkRightFromBWI->setLeftBlk(splitBlk);
        }
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

uint64_t ElfMemoryImage::getDynSymAddressById( uint16_t id ) {
    uint64_t addr = 0;
    ELF_PARSER::DynamicSymbol* dynSymb = originElf->getDynSymbolById(id);
    if ( dynSymb != nullptr) {
        addr = blk->getStartAddr() + dynSymb->getBaseAddr();
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
    return this->startAddr + lenght - 1;
}

MemBlock* MemBlock::getChild() {
    return this->childBlk;
}

MemBlock* MemBlock::getFather() {
    return this->father;
}

MemBlkPermissions MemBlock::getPermissions() {
    return this->permission;
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

void MemBlock::setState( MemBlkState state ) {
    this->state = state;
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