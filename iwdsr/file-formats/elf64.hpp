#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <unordered_map>

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
  int p_offset;
  int p_vaddr;
  int p_paddr;
  int p_filesz;
  int p_memsz;
  int p_flags;
  int p_align;

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
  int64_t r_addend;
};

class Symbol {
  private:
  std::string name;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
  uint64_t st_value;
  uint64_t st_size;

  public:
  Symbol(std::string name, uint8_t st_info, uint8_t st_other, uint16_t st_shndx, uint64_t st_value, uint64_t st_size);

  std::string getName();
};

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

  std::vector<ProgramHeader*> programHeaders;
  std::vector<SectionHeader*> sectionHeaders;
  std::vector<Rel*> relHeaders;
  std::vector<Rela*> relaHeaders;
  std::vector<uint64_t> gotPointers;

  std::unordered_map<std::string, Section*> sections;
  std::unordered_map<std::string, Symbol*> symbols;
  std::unordered_map<std::string, Symbol*> dynSymbols;

  std::vector<ProgramHeader*> parseProgramHeaders();
  std::vector<SectionHeader*> parseSectionHeaders();
  std::unordered_map<std::string, Symbol*> parseSymbolTable();
  std::unordered_map<std::string, Symbol*> parseDynSymbolTable();
  std::unordered_map<uint64_t, std::string> parseShStrTable();
  std::unordered_map<uint64_t, std::string> parseStrTable();
  std::unordered_map<uint64_t, std::string> parseDynStrTable();
  std::vector<Rel*> parseRelTable(Section* secRelType);
  std::vector<Rel*> parseRelTables();
  std::vector<Rela*> parseRelaTable(Section* secRelType);
  std::vector<Rela*> parseRelaTables();
  std::vector<uint64_t> parseGotTable();
  ELF_Header parseELFHeader();


  std::unordered_map<uint64_t, std::string> separateASCIIZeroes(char* rawWords, uint64_t charsCount);

  public:
  ELF(std::filesystem::path pathToELF);
  ~ELF();
  Section* getSectionByName(std::string sectionName);
  std::vector<Section*> getSectionsByShType(SH_TYPE type);
};