file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_texture ${src})
target_link_libraries(example_texture alina glfw)