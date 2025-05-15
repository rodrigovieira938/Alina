file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_uniform-buffer ${src})
target_link_libraries(example_uniform-buffer alina glfw)