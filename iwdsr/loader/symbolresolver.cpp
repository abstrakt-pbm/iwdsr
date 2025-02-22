#include "symbolresolver.hpp"

SR::SymbolResolver::SymbolResolver( ELF_PARSER::ELF& rootElf ) { // Бегаю по ELF и узнаю от кого какой символ зависит
    for ( auto elfSymb : rootElf.getSymbols()) { 
        rootSymbols[elfSymb.first] = new SR::Symbol(
            elfSymb.first,
            rootElf.fetchRawSymbolByName(elfSymb.first),
            searchSymbolDependencies(rootElf, *elfSymb.second)
        );
    }
}

std::unordered_map<std::string, SR::Symbol*> SR::SymbolResolver::searchSymbolDependencies( ELF_PARSER::ELF& searchingFrom, ELF_PARSER::Symbol& symbol ) {
    std::unordered_map<std::string, SR::Symbol*> dependendSymbols;
    for ( auto rel : searchingFrom.getRels() ) {
        if ( symbol.addrInSymbol(rel->r_offset) ) {
           ELF_PARSER::Symbol* dependentSymb = searchingFrom.getSymbolById(rel->getSymbolId());
           int8_t* rawDependentSymb = searchingFrom.fetchRawSymbolByName( dependentSymb->getName() );
           dependendSymbols[dependentSymb->getName()] = new Symbol(
                dependentSymb->getName(),
                rawDependentSymb,
                std::unordered_map<std::string, SR::Symbol*>()
            );
        } 
    }
    for ( auto rela : searchingFrom.getRelas() ) {
        if ( symbol.addrInSymbol(rela->r_offset) ) {
           ELF_PARSER::Symbol* dependentSymb = searchingFrom.getSymbolById(rela->getSymbolId());
           int8_t* rawDependentSymb = searchingFrom.fetchRawSymbolByName( dependentSymb->getName() );
           dependendSymbols[dependentSymb->getName()] = new Symbol(
                dependentSymb->getName(),
                rawDependentSymb,
                std::unordered_map<std::string, SR::Symbol*>()
            );
        } 
    }
    return dependendSymbols;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SR::Symbol::Symbol(std::string name, int8_t* rawSymbol, std::unordered_map<std::string, SR::Symbol*> dependendSymbols) {
    this->name = name;
    this->rawSymbol = rawSymbol;
    this->dependendSymbols = dependendSymbols;
}
