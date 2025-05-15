file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_instacing ${src})
target_link_libraries(example_instacing alina glfw)