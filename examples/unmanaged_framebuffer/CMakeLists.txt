file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_unmanaged_framebuffer ${src})
target_link_libraries(example_unmanaged_framebuffer alina glfw)