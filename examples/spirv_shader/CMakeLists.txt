file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_spirvshader ${src})
target_link_libraries(example_spirvshader alina glfw stb glm)
