cmake_minimum_required(VERSION 3.28)
project(alina)
include(FetchContent)

option(ALINA_BUILD_EXAMPLES "Build Examples" ON)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        658f48e72aee3c6582e80b05ac0f8787a64fe6bb #v2.0.6
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glad)

glad_add_library(glad STATIC REPRODUCIBLE MX LOADER API gl:core=4.4)

file(GLOB_RECURSE alina_src "src/*.cpp" "src/*.hpp")
add_library(alina ${alina_src})
target_link_libraries(alina PUBLIC glad)
target_include_directories(alina PUBLIC include/public)
target_include_directories(alina PRIVATE include/private)

if(${ALINA_BUILD_EXAMPLES})
  FetchContent_MakeAvailable(glfw glm stb)
  add_subdirectory(examples)
  add_library(stb INTERFACE)
  target_include_directories(stb INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/_deps/stb-src)
endif()