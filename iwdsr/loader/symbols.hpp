#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include "..\file-formats\elf64.hpp"

namespace SR {

enum SymbolType {
    STT_NOTYPE,
    STT_OBJECT,
    STT_FUNC,
    STT_SECTION,
    STT_FILE,
    STT_COMMON,
    STT_TLS,
    STT_LOOS,
    STT_HIOS,
    STT_LOPROC,
    STT_HIPROC
};

enum SymbolBind {
    STB_LOCAL,
    STB_GLOBAL,
    STB_WEAK,
    STB_LOOS = 10,
    STB_HIOS = 12,
    STB_LOPROC,
    STB_HIPROC = 15
};

enum SymbolVisibility {
    STV_DEFAULT,
    STV_INTERNAL,
    STV_HIDDEN,
    STV_PROTECTED
};


class Symbol {
    private:
    SymbolType symType;
    SymbolBind symBind;
    SymbolVisibility symVisibility;
    uint8_t* rawPayload;
    public:
    Symbol(SymbolType symType, SymbolBind symBind, SymbolVisibility symVisibility, uint8_t* rawPayload);
};

class SymbolTable {
    private:
    std::unordered_map<std::string, Symbol*> symbols;
    public:
    SymbolTable(const ELF& elf);  
};

} // SR 