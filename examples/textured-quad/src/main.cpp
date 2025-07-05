#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>
#include <glm/glm.hpp>
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
    struct Vertex {
        glm::vec3 pos;
        glm::vec2 uv;
    };
    Vertex vertices[] = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
    };
    uint32_t indices[] {
        0, 2, 1,
    	0, 3, 2
    };

    auto cmd = device->createCommandList();
    cmd->begin();
    cmd->writeBuffer(index_buffer, indices, sizeof(indices), 0);
    cmd->writeBuffer(vertex_buffer, vertices, sizeof(vertices), 0);
    cmd->end();
    device->execute(cmd);
    
    const char* vertex_source = R"(
        #version 440 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aUv;

        out vec2 uv;
        void main()
        {
            uv = aUv;
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    const char* fragment_source = R"(
        #version 440 core
        layout (location = 0) out vec4 fragColor;
        
        layout(binding = 0) uniform sampler2D tex;
        in vec2 uv;
        void main()
        {
            fragColor = texture(tex, uv);
        }
    )";

    auto vs = device->createShader(alina::ShaderType::VERTEX, vertex_source, std::strlen(vertex_source));
    auto fs = device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source));

    auto pipeline = device->createGraphicsPipeline(
        alina::GraphicsPipelineDesc()
            .setInputLayout(
                device->createInputLayout({
                    alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(3).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, pos)),
                    alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(2).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, uv)),
                })
            )
            .setVertexShader(vs)
            .setFragmentShader(fs)
    );
    auto tex = device->createTexture(alina::TextureDesc().setWidth(8).setHeight(8));
    struct Color {
        float r,g,b;
    };
    Color texture_data[8][8];
    Color baseColor = {0.2f, 0.6f, 0.9f};
    Color altColor = {1.0f - baseColor.r, 1.0f - baseColor.g, 1.0f - baseColor.b};

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bool is_white = (x + y) % 2 == 0;
            texture_data[y][x] = is_white ? baseColor : altColor;
        }
    }
    auto sampler = device->createSampler(alina::SamplerDesc().setFilter(alina::TextureFilter::Point));
    auto shaderResouces = alina::ShaderResources().setTexBindings({alina::TextureBinding().setTexture(tex).setSampler(sampler)});
    cmd->begin();
    cmd->writeTexture(tex, texture_data, alina::TextureFormat::RGB32_Float);
    cmd->end();
    device->execute(cmd);

    cmd->begin();
    cmd->bindGraphicsPipeline(pipeline);
    cmd->bindShaderResources(shaderResouces);
    cmd->bindVertexBuffers({
        alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(Vertex)),
    });
    cmd->bindIndexBuffer(index_buffer);
    cmd->drawIndexed(alina::DrawArguments().setVertexCount(6));
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