#pragma once
#include "../file-formats/elf64.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>

namespace SR {

class Symbol {
    private:
    std::string name;
    bool isLoaded;
    int8_t* rawSymbol;
    
    public:
    Symbol(std::string name, int8_t* payload = nullptr);

    void fill(int8_t* payload);
    bool getIsLoaded();
    int8_t* getRaw();
};

class SymbolTree {
    private:
    public:
};

class SymbolResolver { //Ведёт учёт всех длл и символов
private:
    std::unordered_map<std::string, ELF*> loadedDlls;
public:
    SymbolResolver( ELF& rootElf);
    void attachDLL( ELF& dll);
    
};

}; //SR