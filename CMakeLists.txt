cmake_minimum_required(VERSION 3.20)

project( IWDSR )

set( CMAKE_C_COMPILER_WORKS 0 )
set( CMAKE_CXX_STANDARD 20 )

add_executable( iwdsR main.cpp)
add_subdirectory(iwdsr)

target_link_libraries(iwdsR CORE)