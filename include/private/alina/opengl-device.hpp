#pragma once
#include <alina/opengl.hpp>
#include <alina/opengl-commandlist.hpp>
#include <alina/opengl-graphics-pipeline.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    class GlDevice : public IGlDevice, std::enable_shared_from_this<GlDevice> {
    public:
        GlDevice(glLoadFunction fn);
        GladGLContext getContext() override {return context;}
        Texture createUnmanagedTexture(uint32_t id) override;
        Framebuffer createUnmanagedFramebuffer(uint32_t id) override;

        bool beginFrame() override;
        void endFrame() override;
        Buffer createBuffer(const BufferDesc& desc) override;
        CommandList createCommandList() override;
        InputLayout createInputLayout(const VertexAttributeDesc* attrs, size_t size) override;
        inline InputLayout createInputLayout(const std::vector<VertexAttributeDesc>& attrs) {return createInputLayout(attrs.data(), attrs.size());}
        Shader createShader(ShaderType type, const void* data, size_t size, const std::string& entry_point = "main") override;
        GraphicsPipeline createGraphicsPipeline(const GraphicsPipelineDesc& desc) override;
        Texture createTexture(const TextureDesc& desc) override;
        Sampler createSampler(const SamplerDesc& desc) override;
        Framebuffer createFramebuffer(const FramebufferDesc& desc) override;
        void execute(const Commands::BindGraphicsPipeline& command);
        void execute(const Commands::BindShaderResources& command);
        void execute(const Commands::BindVertexBuffers& command);
        void execute(const Commands::BindIndexBuffer& command);
        void execute(const Commands::Draw& command);
        void execute(const Commands::DrawIndexed& command);
        void execute(const Commands::WriteBuffer& command);
        void execute(const Commands::ClearBuffer& command);
        void execute(const Commands::BlitTextures& command);
        void execute(const Commands::GenerateMipMaps& command);
        void execute(const Commands::WriteTexture& command);
        void execute(const Commands::BeginRenderPass& command);
        void execute(const Commands::BeginSubPass& command);
        void execute(const Commands::EndRenderPass& command);
        void execute(CommandList cmd) override;


        RenderPassDesc currentRenderPass;
        GlGraphicsPipeline* currentPipeline;
        GladGLContext context;
    };
}