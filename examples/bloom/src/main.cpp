#include "alina/opengl.hpp"
#include <GLFW/glfw3.h>
#include <alina/alina.hpp>
#include <cstring>
#include <filesystem>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>

alina::ITexture* loadTexture(alina::IDevice* device, alina::ICommandList* cmd) {;
    constexpr const char* filepath = "./examples/bloom/texture.png";
    if(!std::filesystem::exists(filepath)) {
        std::cerr << "Should be run with the project root as the current working directory!\n";
        exit(1);
    }

    stbi_set_flip_vertically_on_load(true);
    int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(filepath, &widthImg, &heightImg, &numColCh, 0);
    alina::TextureFormat format = alina::TextureFormat::Unknown;
    if(numColCh == 4) {
        format = alina::TextureFormat::RGBA8_Uint;
    } else if(numColCh == 3) {
        format = alina::TextureFormat::RGB8_Uint;
    } else if(numColCh == 2) {
        format = alina::TextureFormat::RG8_Uint;
    } else if(numColCh == 1) {
        format = alina::TextureFormat::R8_Uint;
    }
    
    auto tex = device->createTexture(alina::TextureDesc().setFormat(alina::TextureFormat::RGB32_Float).setWidth(widthImg).setHeight(heightImg).setMipLevels(10).setDebugName("OriginalTexture"));
    cmd->begin();
    cmd->writeTexture(tex, bytes, format);
    cmd->generateMipMaps(tex);
    cmd->end();
    device->execute(cmd);
    return tex;
}
struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

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
    alina::IGraphicsPipeline* extractBrightnessPipeline;
    auto brightnessAlbedoTex = device->createTexture(alina::TextureDesc().setWidth(640).setHeight(480).setFormat(alina::TextureFormat::RGB32_Float).setUsage(alina::TextureUsage::ATTACHMENT).setDebugName("Brightness Pass Albedo"));
    auto brightnessTex = device->createTexture(alina::TextureDesc().setWidth(640).setHeight(480).setFormat(alina::TextureFormat::RGB32_Float).setUsage(alina::TextureUsage::ATTACHMENT).setDebugName("Brightness Pass Brightness"));
    auto brightnessfb = device->createFramebuffer(alina::FramebufferDesc().setColorAttachments({
        alina::FramebufferAttachment().setTexture(brightnessAlbedoTex),
        alina::FramebufferAttachment().setTexture(brightnessTex)
    }).setDebugName("Brightness Framebuffer"));
    {
        const char* fragment_source = R"(
            #version 440 core
            layout (location = 0) out vec4 fragColor;
            layout (location = 1) out vec4 brightColor;

            layout(binding = 0) uniform sampler2D tex;
            in vec2 uv;
            void main()
            {
                fragColor = texture(tex, uv);

                // check whether fragment output is higher than threshold, if so output as brightness color
                float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > 0.5)
                    brightColor = vec4(fragColor.rgb, 1.0);
                else
                    brightColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
        )";
        extractBrightnessPipeline = device->createGraphicsPipeline(
            alina::GraphicsPipelineDesc()
                .setInputLayout(
                    device->createInputLayout({
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(3).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, pos)),
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(2).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, uv)),
                    })
                )
                .setVertexShader(vs)
                .setFragmentShader(device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source)))
        );
    }
    alina::IGraphicsPipeline* blurPipeline;
    alina::IBuffer* blurUbo = device->createBuffer(
        alina::BufferDesc()
            .setDebugName("UniformBuffer")
            .setType(alina::BufferType::UNIFORM));
    alina::ITexture* blurTextures[] = {
        device->createTexture(alina::TextureDesc().setWidth(640).setHeight(480).setFormat(alina::TextureFormat::RGB32_Float).setUsage(alina::TextureUsage::ATTACHMENT).setDebugName("Blur Pass Texture1")),
        device->createTexture(alina::TextureDesc().setWidth(640).setHeight(480).setFormat(alina::TextureFormat::RGB32_Float).setUsage(alina::TextureUsage::ATTACHMENT).setDebugName("Blur Pass Texture2")),
    };
    alina::IFramebuffer* blurFbs[] = {
        device->createFramebuffer(alina::FramebufferDesc().setColorAttachments({blurTextures[0]}).setDebugName("BlurPass Framebuffer 1")),
        device->createFramebuffer(alina::FramebufferDesc().setColorAttachments({blurTextures[1]}).setDebugName("BlurPass Framebuffer 2")),
    };
    {
        const char* fragment_source = R"(
            #version 440 core
            layout (location = 0) out vec4 fragColor;
            layout (location = 1) out vec4 brightColor;

            layout(binding = 0) uniform sampler2D image;
            in vec2 uv;

            layout (std140, binding = 1) uniform ubo
            {
                int horizontal;
            };

            float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

            void main()
            {
                vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
                vec3 result = texture(image, uv).rgb * weight[0]; // current fragment's contribution
                if(horizontal == 1)
                {
                    for(int i = 1; i < 5; ++i)
                    {
                        result += texture(image, uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                        result += texture(image, uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                    }
                }
                else
                {
                    for(int i = 1; i < 5; ++i)
                    {
                        result += texture(image, uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                        result += texture(image, uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                    }
                }
                fragColor = vec4(result, 1.0);
            }
        )";
        blurPipeline = device->createGraphicsPipeline(
            alina::GraphicsPipelineDesc()
                .setInputLayout(
                    device->createInputLayout({
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(3).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, pos)),
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(2).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, uv)),
                    })
                )
                .setVertexShader(vs)
                .setFragmentShader(device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source)))
        );
    };
    alina::IGraphicsPipeline* blendingPipeline;
    {
        const char* fragment_source = R"(
            #version 440 core
            layout (location = 0) out vec4 fragColor;

            in vec2 uv;

            uniform sampler2D scene;
            uniform sampler2D bloomBlur;

            void main()
            {             
                const float gamma = 2.2;
                vec3 hdrColor = texture(scene, uv).rgb;      
                vec3 bloomColor = texture(bloomBlur, uv).rgb;
                hdrColor += bloomColor;
                //No tonemapping
                fragColor = vec4(hdrColor,1);
            }  
        )";
        blendingPipeline = device->createGraphicsPipeline(
            alina::GraphicsPipelineDesc()
                .setInputLayout(
                    device->createInputLayout({
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(3).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, pos)),
                        alina::VertexAttributeDesc().setFormat(alina::VertexAttributeFormat::Float).setArraySize(2).setStride(sizeof(Vertex)).setOffset(offsetof(Vertex, uv)),
                    })
                )
                .setVertexShader(vs)
                .setFragmentShader(device->createShader(alina::ShaderType::FRAGMENT, fragment_source, std::strlen(fragment_source)))
        );
    }

    auto tex = loadTexture(device, cmd);
    
    auto lavaTexture = alina::ShaderResources().setTexBindings({alina::TextureBinding().setTexture(tex)});

    int blurPasses = 2;

    alina::ITexture* blurFinalTex;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(device->beginFrame()) {
            //Extract brightness pass
            {
                cmd->begin();
                cmd->beginRenderPass(alina::RenderPassDesc().setFramebuffer(brightnessfb).setAttachmentsLoadOp({alina::RenderPassLoadOp::LOAD}));
                cmd->beginSubPass(alina::SubPassDesc().setAttachments({alina::SubPassAttachment::COLOR,alina::SubPassAttachment::COLOR}));
                cmd->bindGraphicsPipeline(extractBrightnessPipeline);
                cmd->bindShaderResources(lavaTexture);
                cmd->bindVertexBuffers({
                    alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(Vertex)),
                });
                cmd->bindIndexBuffer(index_buffer);
                cmd->drawIndexed(alina::DrawArguments().setVertexCount(6));
                cmd->endRenderPass();
                cmd->end();
                device->execute(cmd);
            }
            //Blur Pass
            {
                bool horizontal = true, first_it = true;
                int uboData[4] = {horizontal, 0, 0, 0};
                for(int i = 0; i < blurPasses; i++) {
                    cmd->begin();
                    cmd->bindGraphicsPipeline(blurPipeline);
                    cmd->beginRenderPass(alina::RenderPassDesc().setFramebuffer(blurFbs[horizontal]));
                    cmd->beginSubPass(alina::SubPassDesc().setAttachments({alina::SubPassAttachment::COLOR}));
                    
                    auto resources = alina::ShaderResources().setUboBindings({
                        alina::UniformBufferBinding().setBuffer(blurUbo).setBinding(1)
                    });


                    if(first_it) {
                        first_it = false;
                        resources.setTexBindings({alina::TextureBinding().setTexture(brightnessTex)});
                    } else {
                        resources.setTexBindings({alina::TextureBinding().setTexture(blurTextures[!horizontal])});
                    }

                    uboData[0] = horizontal;
                    cmd->writeBuffer(blurUbo, uboData, sizeof(uboData), 0);
                    cmd->bindShaderResources(resources);
                    cmd->bindVertexBuffers({
                        alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(Vertex)),
                    });
                    cmd->bindIndexBuffer(index_buffer);
                    cmd->drawIndexed(alina::DrawArguments().setVertexCount(6));
                    horizontal = !horizontal;
                    cmd->endRenderPass();
                    cmd->end();
                    device->execute(cmd);
                }
                blurFinalTex = blurTextures[!horizontal];
            }
            //Blending pass
            {
                cmd->begin();
                cmd->bindGraphicsPipeline(blendingPipeline);
                cmd->bindShaderResources(
                    alina::ShaderResources()
                        .setTexBindings({
                            alina::TextureBinding().setTexture(blurFinalTex),
                            alina::TextureBinding().setTexture(brightnessAlbedoTex)
                        })
                );
                cmd->bindVertexBuffers({
                    alina::BindVertexBuffer().setBuffer(vertex_buffer).setStride(sizeof(Vertex)),
                });
                cmd->bindIndexBuffer(index_buffer);
                cmd->drawIndexed(alina::DrawArguments().setVertexCount(6));
                cmd->end();
                device->execute(cmd);
            }
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