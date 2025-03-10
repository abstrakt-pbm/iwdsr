#include "mockproc.hpp"
#include "mockmem.hpp"

MockProcess::MockProcess() {
    this->mem = new MockMemory();
}

void MockProcess::createMainThread() {

}

void MockProcess::start() {
   for(;;); 
}

void MockProcess::stop() {
    
}

