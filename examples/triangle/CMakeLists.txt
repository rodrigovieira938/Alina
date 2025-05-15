file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_triangle ${src})
target_link_libraries(example_triangle alina glfw)