#include "virtual-memory.hpp"


VMBlock::VMBlock(VAddr start, VAddr end, uint64_t* rawSpace) {
    this->start = start;
    this->end = end;
    this->rawSpace = rawSpace;
}

uint64_t* VirtualMemory::alloc(VAddr start, uint64_t lenght) {
    bool isBlockAlreadyExist = false;
    VMBlock* foundedBlock;
    for ( auto vmblock : memoryContainer) {
        if ( vmblock->isAddrInBlock(start) ){
            isBlockAlreadyExist = true;
            foundedBlock = vmblock;
            break;
        }
    }
    uint64_t* resultPtr;

    if ( isBlockAlreadyExist ) {
        resultPtr = foundedBlock->getRawSpace(); 
    } else {
        uint64_t* createdRawSpace = reinterpret_cast<uint64_t*>(new uint8_t(1000));
        VMBlock* createdVMBlock = new VMBlock(start, start + lenght, createdRawSpace);
        resultPtr = createdRawSpace;
    }    

    return resultPtr;
}

 int VirtualMemory::unWrap( MemoryMap processMemoryMap ) {
    return 0;   
}



uint64_t* VirtualMemory::map(uint64_t start, char payload[], uint64_t lenght) {
   
 }