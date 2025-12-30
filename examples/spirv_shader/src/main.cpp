#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <fstream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Alina Setup Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    auto device = alina::opengl::CreateDevice(glfwGetProcAddress);

    constexpr const char* filepath = "./examples/spirv_shader/shader.spv";
    if(!std::filesystem::exists(filepath)) {
        std::cerr << "Should be run with the project root as the current working directory!\n";
        exit(1);
    }
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Failed to open shader\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto vs = device->createShader(alina::ShaderType::VERTEX, content.data(), content.size());
    auto fs = device->createShader(alina::ShaderType::FRAGMENT, content.data(), content.size());

    //The rest is just to draw the triangle

    auto vertex_buffer = device->createBuffer(
        alina::BufferDesc()
            .setDebugName("VertexBuffer1")
            .setType(alina::BufferType::VERTEX)
    );
    auto index_buffer = device->createBuffer(
        alina::BufferDesc()
            .setDebugName("IndexBuffer1")
            .setType(alina::BufferType::INDEX)
    );

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    uint32_t indices[] {
        0,1,2
    };

    auto cmd = device->createCommandList();
    cmd->begin();
    cmd->writeBuffer(index_buffer, indices, sizeof(indices), 0);
    cmd->writeBuffer(vertex_buffer, vertices, sizeof(vertices), 0);
    cmd->end();
    device->execute(cmd);

    auto pipeline = device->createGraphicsPipeline(
        alina::GraphicsPipelineDesc()
            .setInputLayout(
                device->createInputLayout({
                    alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(3).setStride(sizeof(float) * 3),
                    alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(4).setStride(sizeof(float) * 4).setBufferIndex(1).setInstanced(true)
                })
            )
            .setVertexShader(vs)
            .setFragmentShader(fs)
    );

    cmd->begin();
    cmd->bindGraphicsPipeline(pipeline);
    cmd->bindVertexBuffers({
        alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(float) * 3),
    });
    cmd->bindIndexBuffer(index_buffer);
    cmd->drawIndexed(alina::DrawArguments().setVertexCount(3));
    cmd->end();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(device->beginFrame()) {
            device->execute(cmd);
            device->endFrame();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}