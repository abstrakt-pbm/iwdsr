cmake_minimum_required(VERSION 3.20)

project( CORE )

set( CMAKE_CXX_STANDARD 20 )

set ( CORE_HEADERS
        "file-formats/elf64.hpp"
        "loader/loader.hpp"
        "memory/virtual-memory.hpp"
        "data-structures/rbtree/rbtree.hpp"
        "program/program.hpp"
        "program/mem.hpp"
)

set ( CORE_SRC
        "file-formats/elf64.cpp"
        "loader/loader.cpp"
        "program/program.cpp"
        "program/mem.cpp"
)

add_library(CORE STATIC ${CORE_HEDERS} ${CORE_SRC})