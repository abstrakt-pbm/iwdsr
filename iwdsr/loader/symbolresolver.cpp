#include "symbolresolver.hpp"

SR::Symbol::Symbol(std::string name, int8_t* rawSymbol = nullptr) {
    this->name = name;    
    this->rawSymbol = rawSymbol;
    rawSymbol == nullptr ? isLoaded = false : isLoaded = true;
}

bool SR::Symbol::getIsLoaded() {
    return this->isLoaded;
}

int8_t* SR::Symbol::getRaw() {
    return this->rawSymbol;
}
