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
    auto color_buffer = device->createBuffer(alina::BufferDesc()
            .setDebugName("VertexBuffer2")
            .setType(alina::BufferType::VERTEX));

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    uint32_t indices[] {
        0,1,2
    };
    float colors[] = {
        1.0,0.0,0.0,1.0,
        0.0,1.0,0.0,1.0,
        0.0,0.0,1.0,1.0,
    };


    auto cmd = device->createCommandList();
    cmd->begin();
    cmd->writeBuffer(index_buffer, indices, sizeof(indices), 0);
    cmd->writeBuffer(vertex_buffer, vertices, sizeof(vertices), 0);
    cmd->writeBuffer(color_buffer, colors, sizeof(colors), 0);
    cmd->end();
    device->execute(cmd);
    
    const char* vertex_source = R"(
        #version 440 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;

        out vec4 oColor;

        void main()
        {
            vec4 pos = vec4(aPos, 1.0);
            pos.x += 0.05 * (gl_InstanceID-1);
            gl_Position = pos;
            oColor = aColor;
        }
    )";
    const char* fragment_source = R"(
        #version 440 core
        layout (location = 0) out vec4 fragColor;
        
        in vec4 oColor;

        void main()
        {
            fragColor = oColor;
        }
    )";

    auto vs = device->createShader(alina::ShaderType::VERTEX, vertex_source, std::strlen(vertex_source));
    auto fs = device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source));
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
        alina::BindVertexBuffer().setBuffer(color_buffer).setStride(sizeof(float) * 4)
    });
    cmd->bindIndexBuffer(index_buffer);
    cmd->drawIndexed(alina::DrawArguments().setVertexCount(3).setInstanceCount(3));
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