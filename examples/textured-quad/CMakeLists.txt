file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_textured-quad ${src})
target_link_libraries(example_textured-quad alina glfw glm)