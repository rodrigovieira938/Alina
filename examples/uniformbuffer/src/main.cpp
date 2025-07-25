#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>

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
    auto uniform_buffer = device->createBuffer(
        alina::BufferDesc()
            .setDebugName("UniformBuffer1")
            .setType(alina::BufferType::UNIFORM)
    );

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    uint32_t indices[] {
        0,1,2
    };
    float size[4] = {0.5f, /*std140 Padding*/0, 0, 0};


    auto cmd = device->createCommandList();
    cmd->begin();
    cmd->writeBuffer(index_buffer, indices, sizeof(indices), 0);
    cmd->writeBuffer(vertex_buffer, vertices, sizeof(vertices), 0);
    cmd->writeBuffer(uniform_buffer, size, sizeof(size), 0);
    cmd->end();
    device->execute(cmd);
    
    const char* vertex_source = R"(
        #version 440 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;

        layout (std140, binding = 1) uniform ubo
        {
            float size;
        };

        void main()
        {
            gl_Position = vec4(aPos * size, 1.0);
        }
    )";
    const char* fragment_source = R"(
        #version 440 core
        layout (location = 0) out vec4 fragColor;
        
        void main()
        {
            fragColor = vec4(1,1,0,1);
        }
    )";

    auto vs = device->createShader(alina::ShaderType::VERTEX, vertex_source, std::strlen(vertex_source));
    auto fs = device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source));
    auto shaderResources = alina::ShaderResources().setUboBindings({
        alina::UniformBufferBinding().setBuffer(uniform_buffer).setBinding(1)
    });

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
    cmd->bindShaderResources(shaderResources);
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