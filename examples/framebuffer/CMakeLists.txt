file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_framebuffer ${src})
target_link_libraries(example_framebuffer alina glfw glm)