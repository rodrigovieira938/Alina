file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_bloom ${src})
target_link_libraries(example_bloom alina glfw stb glm)
