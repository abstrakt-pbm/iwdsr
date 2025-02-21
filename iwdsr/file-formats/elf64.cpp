#include "elf64.hpp"
#include <iostream>
#include <string>
#include <span>

using namespace ELF_PARSER;

template<typename Type>
Type changeEndian(Type value) {
    Type changedEndian;
    for( int i = sizeof(Type) - 1 ; i >= 0 ; i-- ) {
        ((uint8_t*)&changedEndian)[i] = ((uint8_t*)&value)[sizeof(Type) - i - 1];
    }
    return changedEndian;
};

ELF::ELF(std::filesystem::path pathToELF) {
    elfFile = new std::fstream(pathToELF, std::ios::in | std::ios::binary );
    if ( !elfFile->is_open() ) {
        std::cout << "ELF file is not open" << std::endl;
        return;
    }
    elfHeader = parseELFHeader();
    sectionHeaders = parseSectionHeaders();
    programHeaders = parseProgramHeaders();
    std::unordered_map<uint64_t, std::string> shstrTab = parseShStrTable();
    for ( auto i = 0 ; i < sectionHeaders.size() ; i++ ) {
        sections[shstrTab[sectionHeaders[i]->sh_name]] = new Section(shstrTab[sectionHeaders[i]->sh_name], sectionHeaders[i]);
    }
    
    symbols = parseSymbolTable();
    relHeaders = parseRelTables();
    relaHeaders = parseRelaTables();
    gotPointers = parseGotTable();
    dynSymbols = parseDynSymbolTable();
    dynamicTable = parseDynamicTable();
    libDependencies = parseLibDependedcise();

};

ELF::~ELF() {

}

ELF_Header ELF::parseELFHeader(){
    char rawElfHeader[64];
    elfFile->read(rawElfHeader, 64);
    
    std::span<char> rawElfMagic(rawElfHeader,4);
    if (std::string(rawElfMagic.begin(),rawElfMagic.end()).compare(std::string({0x7F, 'E', 'L', 'F'})) != 0) {
        std::cout << "Bad ELF magic symbol" << std::endl;
        return ELF_Header();
    } 
    ELF_Header elfHdr;
    elfHdr.ei_class = static_cast<EI_CLASS>(rawElfHeader[4]);
    elfHdr.ei_data = static_cast<EI_DATA>(rawElfHeader[5]);
    elfHdr.ei_version = rawElfHeader[6];
    elfHdr.ei_osabi = static_cast<EI_OSABI>(rawElfHeader[7]);
    elfHdr.ei_abiversion = *((int8_t*)(rawElfHeader + 8));
    elfHdr.e_type = static_cast<E_TYPE>(*((uint8_t*)(rawElfHeader + 16)));
    elfHdr.e_entry = *((uint64_t*)(rawElfHeader + 24));
    elfHdr.e_phoff = *((uint64_t*)(rawElfHeader + 32));
    elfHdr.e_shoff = *((uint64_t*)(rawElfHeader + 40));
    elfHdr.e_flags = *((int32_t*)(rawElfHeader + 48));
    elfHdr.e_ehsize = *((int8_t*)(rawElfHeader + 52));
    elfHdr.e_phentsize = *((int8_t*)(rawElfHeader + 54));
    elfHdr.e_phnum = *((int8_t*)(rawElfHeader + 56));
    elfHdr.e_shentsize = *((int8_t*)(rawElfHeader + 58));
    elfHdr.e_shnum = *((int8_t*)(rawElfHeader + 60));
    elfHdr.e_shstrndx = *((int8_t*)(rawElfHeader + 62));

    return elfHdr;
};

std::vector<SectionHeader*> ELF::parseSectionHeaders() {
    std::vector<SectionHeader*> sectionHdrs (elfHeader.e_shnum);
    if ( elfHeader.e_shoff == 0) {
        return sectionHdrs;
    }
    char* rawSectionTable = new char[elfHeader.e_shnum * elfHeader.e_shentsize];

    elfFile->seekg(elfHeader.e_shoff, std::ios::beg);
    elfFile->read(rawSectionTable ,elfHeader.e_shnum * elfHeader.e_shentsize);
    

    for ( auto i = 0; i < elfHeader.e_shnum ; i++ ) {
        SectionHeader* sectionHdr = new SectionHeader();
        sectionHdr->sh_name = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize);
        sectionHdr->sh_type = static_cast<SH_TYPE>( rawSectionTable[ i * elfHeader.e_shentsize + 4] );
        sectionHdr->sh_flags = static_cast<SH_FLAGS>( rawSectionTable[ i * elfHeader.e_shentsize + 8] );
        sectionHdr->sh_addr =  *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 16);
        sectionHdr->sh_offset = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 24);
        sectionHdr->sh_size = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 32);
        sectionHdr->sh_link = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize + 40);
        sectionHdr->sh_info = *(int32_t*)(rawSectionTable + i * elfHeader.e_shentsize + 44);
        sectionHdr->sh_addralign = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 48);
        sectionHdr->sh_entsize = *(int64_t*)(rawSectionTable + i * elfHeader.e_shentsize + 56);
        sectionHdrs[i] = sectionHdr;
    }

    delete [] rawSectionTable;
    return sectionHdrs;
}

std::vector<ProgramHeader*> ELF::parseProgramHeaders() {
    std::vector<ProgramHeader*> programHdrs(elfHeader.e_phnum);
    char rawProgramHeaderTable[ elfHeader.e_phentsize * elfHeader.e_phnum ];
    elfFile->seekg(elfHeader.e_phoff, std::ios::beg);
    elfFile->read(rawProgramHeaderTable, elfHeader.e_phentsize * elfHeader.e_phnum);

    for( auto i = 0 ; i < elfHeader.e_phnum ; i++ ) {
        ProgramHeader* programHdr = new ProgramHeader();
        programHdr->p_type = static_cast<P_TYPE>(*(uint32_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize ));
        programHdr->p_flag = static_cast<P_FLAGS>(*(uint32_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 4));
        programHdr->p_offset = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 8);
        programHdr->p_vaddr  = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 16);
        programHdr->p_paddr = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 24);
        programHdr->p_filesz = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 32);
        programHdr->p_memsz = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 40);
        programHdr->p_align = *(uint64_t*)(rawProgramHeaderTable + i * elfHeader.e_phentsize + 48);
        programHdrs[i] = programHdr;
    } 
    return programHdrs;
}

std::unordered_map<uint64_t, std::string> ELF::parseShStrTable(){ 
    SectionHeader* strTabSecHeader = sectionHeaders[elfHeader.e_shstrndx];

    char rawShStrTab[strTabSecHeader->sh_size];
    elfFile->seekg(strTabSecHeader->sh_offset, std::ios::beg);
    elfFile->read(rawShStrTab, strTabSecHeader->sh_size);
    
    return separateASCIIZeroes(rawShStrTab, strTabSecHeader->sh_size);
}

std::unordered_map<uint64_t, std::string> ELF::parseStrTable() {
    Section* strTabSec = getSectionByName(".strtab");
    SectionHeader* strTabHeader = strTabSec->getHeader();
    char rawStrTab[strTabHeader->sh_size];

    elfFile->seekg(strTabHeader->sh_offset, std::ios::beg);
    elfFile->read(rawStrTab, strTabHeader->sh_size);

    return separateASCIIZeroes(rawStrTab, strTabHeader->sh_size); 
}

std::vector<Rel*> ELF::parseRelTables() {
    std::vector<Rel*> relsTable;
    std::vector<Section*> relTypeSections = getSectionsByShType(SHT_REL);
    for (auto section : relTypeSections ) {
        auto relsFromCurrentSection = parseRelTable(section);
        relsTable.reserve(relsTable.size() + relsFromCurrentSection.size());
        relsTable.insert(relsTable.end(), relsTable.begin(), relsTable.end());
        relsTable.insert(relsTable.end(), relsFromCurrentSection.begin(), relsFromCurrentSection.end());
    }

    return relsTable;
}

std::vector<Rel*> ELF::parseRelTable(Section* relTypeSection) {
    SectionHeader* rtsHeader = relTypeSection->getHeader();
    std::vector<Rel*> rels;
    if ( rtsHeader->sh_type != SH_TYPE::SHT_REL) {
        return rels;
    }

    char rawTable[rtsHeader->sh_size];

    elfFile->seekg(rtsHeader->sh_offset, std::ios::beg);
    elfFile->read(rawTable, rtsHeader->sh_offset);

    uint64_t recordCount = rtsHeader->sh_size / rtsHeader->sh_entsize;

    for ( auto i = 0 ; i < recordCount ; i++ ) {
        Rel* currentRel = new Rel;
        currentRel->r_offset = *(uint64_t*)(rawTable + i * rtsHeader->sh_entsize);
        currentRel->r_info = *(uint64_t*)(rawTable + i * rtsHeader->sh_entsize + 8); 
        rels.push_back(currentRel);
    }

    return rels;
}

std::vector<Rela*> ELF::parseRelaTables() {
    std::vector<Rela*> relasTable;
    std::vector<Section*> relaTypeSections = getSectionsByShType(SHT_RELA);
    for (auto section : relaTypeSections ) {
        auto relasFromCurrentSection = parseRelaTable(section);
        relasTable.reserve(relasTable.size() + relasFromCurrentSection.size());
        relasTable.insert(relasTable.end(), relasTable.begin(), relasTable.end());
        relasTable.insert(relasTable.end(), relasFromCurrentSection.begin(), relasFromCurrentSection.end());
    }
    return relasTable;
}

std::vector<Rela*> ELF::parseRelaTable(Section* relaTypeSection) {
    SectionHeader* relatsHeader = relaTypeSection->getHeader(); 
    std::vector<Rela*> relas;

    if (relatsHeader->sh_type != SH_TYPE::SHT_RELA) {
        return relas;
    }
    char rawRelaTable[relatsHeader->sh_size];
    elfFile->seekg(relatsHeader->sh_offset, std::ios::beg);
    elfFile->read(rawRelaTable, relatsHeader->sh_size);

    for ( auto ch : rawRelaTable) {
        std::cout << std::hex << (int)ch <<  std::endl;
    }
   
    
    uint64_t recordCount = relatsHeader->sh_size / relatsHeader->sh_entsize;
    for ( auto i = 0 ; i < recordCount ; i++ ) {
        Rela* currentRela = new Rela;
        currentRela->r_offset = *(uint64_t*)(rawRelaTable + i * relatsHeader->sh_entsize);
        currentRela->r_info = *(uint64_t*)(rawRelaTable + i * relatsHeader->sh_entsize + 8);
        currentRela->r_addend = *(int64_t*)(rawRelaTable + i * relatsHeader->sh_entsize + 16);
        relas.push_back(currentRela);
    }

   return relas;
}

std::unordered_map<uint64_t, std::string> ELF::separateASCIIZeroes(char* rawWords, uint64_t charsCount){
    std::unordered_map<uint64_t, std::string> wordsMap;
    if ( rawWords == nullptr ) {
        return wordsMap;
    }

    bool consumingWord = false;
    int prev = 0;
    auto i = 0;
    while ( i < charsCount){
        if ( consumingWord == false && rawWords[i] == 0x00) {
            consumingWord = true;
            prev = i+1;
            i++;
        } else if (consumingWord == true && rawWords[i] == 0x00) {
            consumingWord = false;
            std::span<char> rawWord(rawWords + prev, i - prev);
            wordsMap[prev] = std::string(rawWord.begin(), rawWord.end());
        } else {
            i++;
        }
    }
    return wordsMap;
}

std::unordered_map<std::string, Symbol*> ELF::parseSymbolTable() {
    std::unordered_map<std::string, Symbol*> symbols;
    Section* symTab = getSectionByName(".symtab");
    SectionHeader* symTabHeader = symTab->getHeader();
    std::unordered_map<uint64_t, std::string> strTab = parseStrTable();

    char rawSymTab[symTabHeader->sh_size];
    elfFile->seekg(symTabHeader->sh_offset, std::ios::beg);
    elfFile->read(rawSymTab, symTabHeader->sh_size);

    uint64_t recordCount = symTabHeader->sh_size / symTabHeader->sh_entsize;

    for ( auto i = 0 ; i < recordCount ; i++ ) {
        uint64_t st_name = *(uint32_t*)(rawSymTab + i * symTabHeader->sh_entsize);
        uint8_t st_info = *(uint8_t*)(rawSymTab + i * symTabHeader->sh_entsize + 4);
        uint8_t st_other = *(uint8_t*)(rawSymTab + i * symTabHeader->sh_entsize + 5);
        uint16_t st_shndx = *(uint16_t*)(rawSymTab + i * symTabHeader->sh_entsize + 6);
        uint64_t st_value = *(uint64_t*)(rawSymTab + i * symTabHeader->sh_entsize + 8);
        uint64_t st_size = *(uint64_t*)(rawSymTab + i * symTabHeader->sh_entsize + 16);

        symbols[strTab[st_name]] = new Symbol(strTab[st_name], st_info, st_other, st_shndx, st_value, st_size);
    }

    return symbols;
}

std::unordered_map<std::string, Symbol*> ELF::parseDynSymbolTable() {
    std::unordered_map<std::string, Symbol*> dynSymbols;
    Section* dynSymbolSection = getSectionByName(".dynsym");
    SectionHeader* dynSymSecHeader = dynSymbolSection->getHeader();
    dynStrs = parseDynStrTable();
    uint64_t recordCount = dynSymSecHeader->sh_size / dynSymSecHeader->sh_entsize;

    char rawDynSymTab[dynSymSecHeader->sh_size];
    elfFile->seekg(dynSymSecHeader->sh_offset, std::ios::beg);
    elfFile->read(rawDynSymTab, dynSymSecHeader->sh_size);

    for( auto i = 0 ; i < recordCount ; i++ ) {
        uint64_t st_name = *(uint32_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize);
        uint8_t st_info = *(uint8_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize + 4);
        uint8_t st_other = *(uint8_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize + 5);
        uint16_t st_shndx = *(uint16_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize + 6);
        uint64_t st_value = *(uint64_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize + 8);
        uint64_t st_size = *(uint64_t*)(rawDynSymTab + i * dynSymSecHeader->sh_entsize + 16);

        dynSymbols[dynStrs[st_name]] = new Symbol(dynStrs[st_name], st_info, st_other, st_shndx, st_value, st_size);
    }

    return dynSymbols;
}

std::unordered_map<uint64_t, std::string> ELF::parseDynStrTable() {
    std::unordered_map<uint64_t, std::string> dynSymStrTable;      
    Section* dynStr = getSectionByName(".dynstr");
    SectionHeader* dynStrHeaders = dynStr->getHeader();

    char rawDynSymStr[dynStrHeaders->sh_size];
    elfFile->seekg(dynStrHeaders->sh_offset, std::ios::beg);
    elfFile->read(rawDynSymStr, dynStrHeaders->sh_size);

    return separateASCIIZeroes(rawDynSymStr, dynStrHeaders->sh_size);
}

std::vector<ELF_DYN*> ELF::parseDynamicTable() {
    std::vector<ELF_DYN*> dynamicTable;
    Section* dynamicTableSec = getSectionByName(".dynamic");
    SectionHeader* dynTabHeaders = dynamicTableSec->getHeader();
    uint64_t recordCount = dynTabHeaders->sh_size / dynTabHeaders->sh_entsize;
    char rawDynamicTable[dynTabHeaders->sh_size];
    elfFile->seekg(dynTabHeaders->sh_offset, std::ios::beg);
    elfFile->read(rawDynamicTable, dynTabHeaders->sh_size);

    for ( auto i = 0 ; i < recordCount ; i++ ) {
        ELF_DYN* dyn = new ELF_DYN;
        dyn->type = static_cast<DT_TAG>(*(uint64_t*)(rawDynamicTable + i*dynTabHeaders->sh_entsize));
        dyn->d_un.d_val = *(uint64_t*)(rawDynamicTable + i*dynTabHeaders->sh_entsize + 8);
        dynamicTable.push_back(dyn);
    }
    return dynamicTable;
}

std::vector<std::string> ELF::parseLibDependedcise() {
    std::vector<std::string> libsStr;
    std::vector<ELF_DYN*> elfDyn = getDynamicRecordsByDtTag(DT_TAG::DT_NEEDED);
    for ( auto dyn : elfDyn ) {
        libsStr.push_back(dynStrs[dyn->d_un.d_val]);
    }
    return libsStr;
}

std::vector<ELF_DYN*> ELF::getDynamicRecordsByDtTag(DT_TAG dtTag) {
    std::vector<ELF_DYN*> dynRecs;
    for ( auto record : dynamicTable) {
        if ( record->type == dtTag ) {
            dynRecs.push_back(record);
        }
    }
    return dynRecs;
}

std::vector<uint64_t> ELF::parseGotTable() {
    std::vector<uint64_t> funcPointers;
    Section* gotSec = getSectionByName(".got");
    SectionHeader* gotSecHeader = gotSec->getHeader();

    char rawGotTable[gotSecHeader->sh_size];
    elfFile->seekg(gotSecHeader->sh_offset, std::ios::beg);
    elfFile->read(rawGotTable, gotSecHeader->sh_size);

    uint64_t recordCount = gotSecHeader->sh_size / gotSecHeader->sh_entsize;
    for ( auto i = 0 ; i < recordCount ; i++ ) {
       uint64_t currentFuncPointer =  *(uint64_t*)(rawGotTable + i * gotSecHeader->sh_entsize);
       funcPointers.push_back(currentFuncPointer);
    }

    return funcPointers;
}

Section* ELF::getSectionByName(std::string sectionName) {
    Section* foundSection = nullptr;
    if ( sections.contains(sectionName) ) {
        foundSection = sections[sectionName]; 
    }
    return foundSection;
}

std::vector<Section*> ELF::getSectionsByShType(SH_TYPE type) {
    std::vector<Section*> secs;
    for( auto section : sections) {
        SectionHeader* secHeader = section.second->getHeader();
        if (secHeader->sh_type == type) {
            secs.push_back(section.second);
        }
    }    
    return secs;
}

std::vector<ProgramHeader*> ELF::getProgramHeadersByPType(P_TYPE pType) const {
   std::vector<ProgramHeader*> suitablePheaders;
   for ( auto header : programHeaders) {
        if( header->p_type == pType) {
            suitablePheaders.push_back(header);
        }
    }
    return suitablePheaders; 
}

int8_t* ELF::rawRead( uint64_t offset, uint64_t byteCount ) {
    char* rawInput = new char[byteCount];
    elfFile->seekg(offset, std::ios::beg);
    elfFile->read(rawInput, byteCount);
    return (int8_t*)(rawInput);
}

std::vector<std::string> ELF::getLibDependencies() {
    return this->libDependencies;
}

std::unordered_map<std::string, Symbol*> ELF::getSymbols() {
    return this->symbols;
}

std::vector<Rel*> ELF::getRels() {
    return this->relHeaders;
}

std::vector<Rela*> ELF::getRelas() {
    return this->relaHeaders;
}

Symbol* ELF::getSymbolById(uint16_t id) {
    Symbol* res = nullptr;
    for ( auto symb : symbols ) {
        if ( symb.second->getId() == id) {
            res = symb.second;
        }
    } 
    return res;
}

int8_t* ELF::fetchRawSymbolByName( std::string symbolName) {
    Symbol* symb = symbols[symbolName];

    char* rawSymbol = new char[symb->getSize()];
    elfFile->seekg( symb->getBaseAddr(), std::ios::beg );
    elfFile->read(rawSymbol, symb->getSize());
    return (int8_t*)rawSymbol;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Section::Section(std::string title, SectionHeader *segHdr) {
    this->title = title;
    this->headers = segHdr;
}

SectionHeader* Section::getHeader() {
    return this->headers;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Symbol::Symbol(std::string name, uint8_t st_info, uint8_t st_other, uint16_t st_shndx, uint64_t st_value, uint64_t st_size) {
    this->name = name;
    this->st_info = st_info;
    this->st_other = st_other;
    this->st_shndx = st_shndx;
    this->st_value = st_value;
    this->st_size = st_size;
}

std::string Symbol::getName() {

    return this->name;
}

bool Symbol::addrInSymbol( uint64_t addr ) {
    return addr >= st_value && addr < (st_value + st_size + 1);
}

uint16_t Symbol::getId() {
    return this->st_shndx;
}

uint64_t Symbol::getBaseAddr() {
    return this->st_value;
}

uint64_t Symbol::getSize() {
    return this->st_size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t Rel::getSymbolId(){
    return r_info >> 32;
}


RelocationType Rel::getRelocationType() {
    return static_cast<RelocationType>(r_info & 0xFFFFFFFF );
}

