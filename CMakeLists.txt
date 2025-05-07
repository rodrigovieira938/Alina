cmake_minimum_required(VERSION 3.28)
project(alina)
include(FetchContent)

option(ALINA_BUILD_EXAMPLES "Build Examples" ON)

file(GLOB_RECURSE alina_src "src/*.cpp" "src/*.hpp")
add_library(alina ${alina_src})

if(${ALINA_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()