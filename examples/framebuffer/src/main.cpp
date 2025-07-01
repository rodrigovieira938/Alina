#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <glm/glm.hpp>
#include <cstring>

void renderToFb(alina::IDevice* device, alina::IFramebuffer* fb, alina::ICommandList* cmd) {
    static alina::IBuffer *vertex_buffer = nullptr, *index_buffer = nullptr;
    if(!vertex_buffer) {
        static float vertices[] = {
            0.0f,  0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        };
        static uint32_t indices[] {
            0,1,2
        };
        
        vertex_buffer = device->createBuffer(
            alina::BufferDesc()
                .setDebugName("VertexBuffer1")
                .setType(alina::BufferType::VERTEX)
        );
        index_buffer = device->createBuffer(
            alina::BufferDesc()
                .setDebugName("IndexBuffer1")
                .setType(alina::BufferType::INDEX)
        );
        cmd->begin();
        cmd->writeBuffer(index_buffer, indices, sizeof(indices), 0);
        cmd->writeBuffer(vertex_buffer, vertices, sizeof(vertices), 0);
        cmd->end();
        device->execute(cmd);
    }
    
    static alina::IShader *vs = nullptr, *fs = nullptr;
    if(vs == nullptr) {
        static const char* vertex_source = R"(
            #version 440 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aColor;
    
            void main()
            {
                gl_Position = vec4(aPos, 1.0);
            }
        )";
        static const char* fragment_source = R"(
            #version 440 core
            layout (location = 0) out vec4 fragColor;
            
            void main()
            {
                fragColor = vec4(0,0,0,1);
            }
        )";
        
        vs = device->createShader(alina::ShaderType::VERTEX, vertex_source, std::strlen(vertex_source));
        fs = device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source));
    }
    static alina::IGraphicsPipeline* pipeline = nullptr;
    if(!pipeline) {
        pipeline = device->createGraphicsPipeline(
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
    }

    cmd->begin();
    cmd->beginRenderPass(alina::RenderPassDesc().setFramebuffer(fb).setAttachmentsLoadOp({alina::RenderPassLoadOp::CLEAR}).setAttachmentsClearColors({{1,1,1,1}}));
    cmd->beginSubPass(alina::SubPassDesc().setAttachments({alina::SubPassAttachment::COLOR}));
    //Render the triangle
    {
        cmd->bindGraphicsPipeline(pipeline);
        cmd->bindVertexBuffers({
            alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(float) * 3),
        });
        cmd->bindIndexBuffer(index_buffer);
        cmd->drawIndexed(alina::DrawArguments().setVertexCount(3));
    }
    cmd->endRenderPass();
    cmd->end();
    device->execute(cmd);
}
void renderFb(alina::IDevice* device, alina::ITexture* tex, alina::ICommandList* cmd) {
    static alina::IGraphicsPipeline* pipeline = nullptr;
    static alina::IShader *vs = nullptr, *fs = nullptr;
    static alina::IBuffer *vertex_buffer = nullptr, *index_buffer = nullptr;
    struct Vertex {
        glm::vec3 pos;
        glm::vec2 uv;
    };
    if(pipeline == nullptr) {
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

        vertex_buffer = device->createBuffer(
            alina::BufferDesc()
                .setDebugName("VertexBuffer1")
                .setType(alina::BufferType::VERTEX)
        );
        index_buffer = device->createBuffer(
            alina::BufferDesc()
                .setDebugName("IndexBuffer1")
                .setType(alina::BufferType::INDEX)
        );
    
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
            
            layout(binding = 1) uniform sampler2D tex;
            in vec2 uv;
            void main()
            {
                fragColor = texture(tex, uv);
            }
        )";
    
        vs = device->createShader(alina::ShaderType::VERTEX, vertex_source, std::strlen(vertex_source));
        fs = device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source));
    
        pipeline = device->createGraphicsPipeline(
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
    }
    auto shaderResouces = alina::ShaderResources().setTexBindings({alina::TextureBinding().setTexture(tex)});
    cmd->begin();
    cmd->bindGraphicsPipeline(pipeline);
    cmd->bindShaderResources(shaderResouces);
    cmd->bindVertexBuffers({
        alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(Vertex)),
    });
    cmd->bindIndexBuffer(index_buffer);
    cmd->drawIndexed(alina::DrawArguments().setVertexCount(6));
    cmd->end();
    device->execute(cmd);
}

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
    auto tex = device->createTexture(alina::TextureDesc().setWidth(640).setHeight(480).setUsage(alina::TextureUsage::ATTACHMENT));
    auto fb = device->createFramebuffer(alina::FramebufferDesc().setColorAttachments({alina::FramebufferAttachment().setTexture(tex)}));
    auto cmd = device->createCommandList();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(device->beginFrame()) {
            renderToFb(device, fb, cmd);
            //TODO: replace this with blit
            renderFb(device, tex, cmd);
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