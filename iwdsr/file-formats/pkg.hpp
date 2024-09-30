#pragma once

#include <filesystem>

typedef struct {

} PKG_HEADERS;

class PKG {
    private:
    PKG_HEADERS pkgHeaders;
    
    public:
    PKG(std::filesystem::path pathToPkg);
    void unpack(std::filesystem::path pathToPkg);
    ~PKG();
    
};