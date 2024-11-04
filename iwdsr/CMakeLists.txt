cmake_minimum_required(VERSION 3.20)

project( CORE )

set( CMAKE_CXX_STANDARD 20 )

set ( CORE_HEADERS
        "file-formats/elf64.hpp"
        "memory/memory-map.hpp"
        "memory/virtual-memory.hpp"
)

set ( CORE_SRC
        "file-formats/elf64.cpp"
        "memory/memory-map.cpp"
        "memory/virtual-memory.cpp"
)

add_library(CORE STATIC ${CORE_HEDERS} ${CORE_SRC})