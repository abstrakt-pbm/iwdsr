cmake_minimum_required(VERSION 3.20)

project( CORE )

set( CMAKE_CXX_STANDARD 20 )

set ( CORE_HEADERS
        "file-formats/elf64.hpp"
        "loader/loader.hpp"
        "loader/memblock.hpp"
        "loader/symbolresolver.hpp"
        "memory/virtual-memory.hpp"
        "data-structures/rbtree/rbtree.hpp"
        "proc/proc.hpp"
        "proc/procmem.hpp"
        "proc/mock/mockproc.hpp"
        "proc/mock/mockmem.hpp"
        #"proc/windows/winproc.hpp"
        #"proc/windows/winmem.hpp"
        #"proc/posix/posixproc.hpp"
        #"proc/posix/posixmem.hpp"
)

set ( CORE_SRC
        "file-formats/elf64.cpp"
        "loader/loader.cpp"
        "loader/memblock.cpp"
        "loader/symbolresolver.cpp"
        "proc/proc.cpp"
        "proc/procmem.cpp"
        "proc/mock/mockproc.cpp"
        "proc/mock/mockmem.cpp"
        #"proc/windows/winproc.cpp"
        #"proc/windows/winmem.cpp"
        #"proc/posix/posixproc.cpp"
        #"proc/posix/posixmem.cpp"
)

add_library(CORE STATIC ${CORE_HEDERS} ${CORE_SRC})