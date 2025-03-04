#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <unordered_map>

namespace ELF_PARSER {

enum EI_CLASS : char {
  ELFCLASSNONE,
  ELFCLASS32,
  ELFCLASS64
};

enum EI_DATA : char {
  ELFDATANONE,
  ELFDATA2LSB,
  ELFDATA2MSB
};

enum EI_OSABI : char {
  ELFOSABI_NONE,
  ELFOSABI_HPUX,
  ELFOSABI_NETBSD,
  ELFOSABI_GNU,
  ELFOSABI_SOLARIS,
  ELFOSABI_AIX,
  ELFOSABI_IRIX,
  ELFOSABI_FREEBSD,
  ELFOSABI_TRU64,
  ELFOSABI_MODESTO,
  ELFOSABI_OPENBSD,
  ELFOSABI_OPENVMS,
  ELFOSABI_NSK,
  ELFOSABI_AROS,
  ELFOSABI_FENIXOS,
  ELFOSABI_CLOUDABI,
  ELFOSABI_OPENVOS,
};

enum E_TYPE : std::int8_t {
  ET_NONE,
  ET_REL,
  ET_EXEC,
  ET_DYN,
  ET_CORE,
};

typedef struct {
  EI_CLASS ei_class;
  EI_DATA ei_data;
  unsigned int ei_version;
  EI_OSABI ei_osabi;
  unsigned int ei_abiversion;
  E_TYPE e_type;
  uint64_t e_entry;
  uint64_t e_phoff;
  uint64_t e_shoff;
  int8_t e_flags;
  int8_t e_ehsize;
  int8_t e_phentsize;
  int8_t e_phnum;
  int8_t e_shentsize;
  int8_t e_shnum;
  int8_t e_shstrndx;
} ELF_Header;

enum P_TYPE : std::uint32_t {
  PT_NULL = 0x00000000,
  PT_LOAD = 0x00000001,
  PT_DYNAMIC = 0x00000002,
  PT_INTERP = 0x00000003,
  PT_NOTE = 0x00000004,
  PT_SHLIB = 0x00000005,
  PT_PHDR = 0x00000006,
  PT_TLS = 0x00000007,
  PT_LOOS = 0x60000000,
  PT_HIOS = 0x6FFFFFFF,
  PT_LOPROC = 0x70000000,
  PT_HIPROC = 0x7FFFFFFF
};

enum P_FLAGS : std::uint32_t {
  PF_X = 0x1,
  PF_W = 0x2,
  PF_R = 0x4,
  PF_MASKOS = 0x0ff00000,
  PF_MASKPROC = 0xf0000000
};

typedef struct {
  P_TYPE p_type;
  P_FLAGS p_flag;
  uint64_t p_offset;
  uint64_t p_vaddr;
  uint64_t p_paddr;
  uint64_t p_filesz;
  uint64_t p_memsz;
  uint32_t p_flags;
  uint64_t p_align;

} ProgramHeader;

enum SH_TYPE : std::int32_t{
  SHT_NULL = 0x0,
  SHT_PROGBITS = 0x1,
  SHT_SYMTAB = 0x2,
  SHT_STRTAB = 0x3,
  SHT_RELA = 0x4,
  SHT_HASH = 0x5,
  SHT_DYNAMIC = 0x6,
  SHT_NOTE = 0x7,
  SHT_NOBITS = 0x8,
  SHT_REL = 0x9,
  SHT_SHLIB = 0x0A,
  SHT_DYNSYM = 0x0B,
  SHT_INIT_ARRAY = 0x0E,
  SHT_FINI_ARRAY = 0x0F,
  SHT_PREINIT_ARRAY = 0x10,
  SHT_GROUP = 0x11,
  SHT_SYMTAB_SHNDX = 0x12,
  SHT_NUM = 0x13,
  SHT_LOOS = 0x60000000
};

enum SH_FLAGS : std::int64_t {
  SHF_WRITE = 0x1,
  SHF_ALLOC = 0x2,
  SHF_EXECINSTR = 0x4,
  SHF_MERGE = 0x10,
  SHF_STRINGS = 0x20,
  SHF_INFO_LINK = 0x40,
  SHF_LINK_ORDER = 0x80,
  SHF_OS_NONCONFORMING = 0x100,
  SHF_GROUP	= 0x200,
  SHF_TLS = 0x400,
  SHF_MASKOS = 0x0FF00000,
  SHF_MASKPROC = 0xF0000000,
  SHF_ORDERED = 0x4000000,
  SHF_EXCLUDE = 0x8000000
};

typedef struct {
  int32_t sh_name;
  SH_TYPE sh_type;
  SH_FLAGS sh_flags;
  int64_t sh_addr;
  int64_t sh_offset;
  int64_t sh_size;
  int32_t sh_link;
  int32_t sh_info;
  int64_t sh_addralign;
  int64_t sh_entsize;

} SectionHeader;

enum RelocationType {
  R_X86_64_NONE,
  R_X86_64_64,
  R_X86_64_PC32,
  R_X86_64_GOT32,
  R_X86_64_PLT32,
  R_X86_64_COPY,
  R_X86_64_GLOB_DAT,
  R_X86_64_JUMP_SLOT,
  R_X86_64_RELATIVE,
  R_X86_64_GOTPCREL,
  R_X86_64_32,
  R_X86_64_32S,
  R_X86_64_16,
  R_X86_64_PC16,
  R_X86_64_8,
  R_X86_64_PC8

};

class Rel {
  public:
  uint64_t r_offset;
  uint64_t r_info;
  uint64_t getSymbolId();
  RelocationType getRelocationType();
};

class Rela : public Rel {
  public:
  int64_t r_addend;
};

class Symbol {
  protected:
  std::string name;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
  uint64_t st_value;
  uint64_t st_size;

  public:
  Symbol() = default;
  Symbol(std::string name, uint8_t st_info, uint8_t st_other, uint16_t st_shndx, uint64_t st_value, uint64_t st_size);

  std::string getName();
  bool addrInSymbol(uint64_t addr);
  uint16_t getSectionId();
  uint64_t getBaseAddr();
  uint64_t getSize();
};

class DynamicSymbol : public Symbol {
  protected:
  uint16_t id; 
  public:
  DynamicSymbol(std::string name, uint8_t st_info, uint8_t st_other, uint16_t st_shndx, uint64_t st_value, uint64_t st_size, uint64_t id);
  uint16_t getId();
};

enum DT_TAG {
  DT_NULL,
  DT_NEEDED,
  DT_PLTRELSZ,
  DT_PLTGOT,
  DT_HASH,
  DT_STRTAB,
  DT_SYMTAB,
  DT_RELA,
  DT_RELASZ,
  DT_RELAENT,
  DT_STRSZ,
  DT_SYMENT,
  DT_INIT,
  DT_FINI,
  DT_SONAME,
  DT_RPATH,
  DT_SYMBOLIC,
  DT_REL,
  DT_RELSZ,
  DT_RELENT,
  DT_PLTREL,
  DT_DEBUG,
  DT_TEXTREL,
  DT_JMPREL,
  DT_BIND_NOW,
  DT_INIT_ARRAY,
  DT_FINI_ARRAY,
  DT_INIT_ARRAYSZ,
  DT_FINI_ARRAYSZ,
  DT_RUNPATH,
  DT_FLAGS,
  DT_ENCODING,
  DT_PREINIT_ARRAY,
  DT_PREINIT_ARRAYSZ,
  DT_LOOS,
  DT_SUNW_RTLDINF,
  DT_HIOS,
  DT_VALRNGLO,
  DT_CHECKSUM,
  DT_PLTPADSZ,
  DT_MOVEENT,
  DT_MOVESZ,
  DT_FEATURE_1,
  DT_POSFLAG_1,
  DT_SYMINSZ,
  DT_SYMINENT,
  DT_VALRNGHI,
  DT_ADDRRNGLO,
  DT_CONFIG,
  DT_DEPAUDIT,
  DT_AUDIT,
  DT_PLTPAD,
  DT_MOVETAB,
  DT_SYMINFO,
  DT_ADDRRNGHI,
  DT_RELACOUNT,
  DT_RELCOUNT,
  DT_FLAGS_1,
  DT_VERDEF,
  DT_VERDEFNUM,
  DT_VERNEED,
  DT_VERNEEDNUM,
  DT_LOPROC,
  DT_SPARC_REGISTER,
  DT_AUXILIARY,
  DT_USED,
  DT_FILTER ,
  DT_HIPROC = 0x7fffffff
};

typedef struct {
  DT_TAG type;
  union {
    uint64_t d_val;
    uint64_t d_ptr;
  } d_un;
} ELF_DYN;

class Section {
  private:
  bool isLoad;
  std::string title; 
  SectionHeader* headers;
  uint8_t* payload;


  public:
  Section(std::string, SectionHeader *seg);
  ~Section();

  SectionHeader* getHeader();
};

class ELF {
  private:
  std::fstream* elfFile;
  ELF_Header elfHeader;

  std::vector<std::string> libDependencies;
  std::vector<ELF_DYN*> dynamicTable;

  std::vector<ProgramHeader*> programHeaders;
  std::vector<SectionHeader*> sectionHeaders;
  std::vector<Rel*> relHeaders;
  std::vector<Rela*> relaHeaders;
  
  std::unordered_map<uint64_t, std::string> dynStrs;
  std::unordered_map<uint64_t, std::string> shstrTab;

  std::unordered_map<std::string, Section*> sections;
  std::unordered_map<std::string, Symbol*> symbols;
  std::unordered_map<std::string, DynamicSymbol*> dynSymbols;

  std::vector<ProgramHeader*> parseProgramHeaders();
  std::vector<SectionHeader*> parseSectionHeaders();
  std::unordered_map<std::string, Symbol*> parseSymbolTable();
  std::unordered_map<std::string, DynamicSymbol*> parseDynSymbolTable();
  std::unordered_map<uint64_t, std::string> parseShStrTable();
  std::unordered_map<uint64_t, std::string> parseStrTable();
  std::unordered_map<uint64_t, std::string> parseDynStrTable();
  std::vector<Rel*> parseRelTable(Section* secRelType);
  std::vector<Rel*> parseRelTables();
  std::vector<Rela*> parseRelaTable(Section* secRelType);
  std::vector<Rela*> parseRelaTables();
  ELF_Header parseELFHeader();
  std::vector<ELF_DYN*> parseDynamicTable();
  std::vector<std::string> parseLibDependedcise();
  std::unordered_map<uint64_t, std::string> separateASCIIZeroes(char* rawWords, uint64_t charsCount);

  uint64_t getGOTbaseAddr();

  bool isSectionExistsByName( std::string sectionName);

  public:
  ELF(std::filesystem::path pathToELF);
  ~ELF();

  Section* getSectionByName(std::string sectionName);
  std::vector<Section*> getSectionsByShType(SH_TYPE type);
  std::vector<ELF_DYN*> getDynamicRecordsByDtTag(DT_TAG dtTag);
  std::vector<ProgramHeader*> getProgramHeadersByPType(P_TYPE pType) const;
  int8_t* rawRead( uint64_t offset, uint64_t byteCount);
  std::vector<std::string> getLibDependencies();
  std::unordered_map<std::string, Symbol*> getSymbols(); 
  std::vector<std::string> getSymbolNames();
  Symbol* getSymbolById( uint16_t id );
  Symbol* getSymbolByName( std::string );
  bool isExportSymbolByName( std::string );
  DynamicSymbol* getDynSymbolById( uint16_t id );
  DynamicSymbol* getDynSymbolByName( std::string dynamiSymbolName );
  int8_t* fetchRawSymbolByName( std::string symbolName);

  std::vector<Rel*> getRels();
  std::vector<Rela*> getRelas();

  uint64_t getGOTSize();
  uint64_t getMemImageSize();
};

}