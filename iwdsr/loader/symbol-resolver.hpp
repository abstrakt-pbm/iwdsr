#pragma once

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

class Symbol {
    private:
    public:
    Symbol();
};

class SymbolResolver{

};
}

