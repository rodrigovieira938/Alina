file(GLOB_RECURSE src "src/*.cpp" "src/*.hpp")
add_executable(example_uniformbuffer ${src})
target_link_libraries(example_uniformbuffer alina glfw)