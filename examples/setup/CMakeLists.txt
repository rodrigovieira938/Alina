file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_setup ${src})
target_link_libraries(example_setup alina glfw)