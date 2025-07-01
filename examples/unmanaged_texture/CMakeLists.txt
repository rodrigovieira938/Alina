file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_unamanged_texture ${src})
target_link_libraries(example_unamanged_texture alina glfw)