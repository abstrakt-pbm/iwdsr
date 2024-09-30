#pragma once

#include <filesystem>
#include <vector>

enum P_TYPE {
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

enum P_FLAGS {
  PF_X = 0x1,
  PF_W = 0x2,
  PF_R = 0x4
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

enum SH_TYPE {
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

enum SH_FLAGS {
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
  int sh_name;
  SH_TYPE sh_type;
  SH_FLAGS sh_flags;
  int sh_addr;
  int sh_offset;
  int sh_size;
  int sh_link;
  int sh_info;
  int sh_addralign;
  int sh_entsize;

} SectionHeader;

class ELF {
  private:
  std::vector<ProgramHeader> programHeaders;
  std::vector<SectionHeader> sectionHeaders;
  public:
  ELF(std::filesystem::path pathToELF);
};