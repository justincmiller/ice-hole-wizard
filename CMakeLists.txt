cmake_minimum_required(VERSION 3.10)
project(ice-hole-wizard)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/engine.c
    ${CMAKE_SOURCE_DIR}/src/vterminal.c
    ${CMAKE_SOURCE_DIR}/src/display.c
    ${CMAKE_SOURCE_DIR}/src/map.c)

add_executable(ice-hole-wizard ${SOURCES})