file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_library(alina_tests_common INTERFACE ${src})
target_link_libraries(alina_tests_common INTERFACE alina glfw stb glm Catch2WithMain)
target_include_directories(alina_tests_common INTERFACE src)