#pragma once
#include <alina/opengl.hpp>
#include <alina/opengl-commandlist.hpp>
#include <alina/opengl-graphics-pipeline.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    class Device : public IGlDevice {
    public:
        Device(glLoadFunction fn);
        GladGLContext getContext() override {return context;}
        ITexture* createUnmanagedTexture(uint32_t id) override;
        IFramebuffer* createUnmanagedFramebuffer(uint32_t id) override;

        bool beginFrame() override;
        void endFrame() override;
        IBuffer* createBuffer(const BufferDesc& desc) override;
        ICommandList* createCommandList() override;
        IInputLayout* createInputLayout(const VertexAttributeDesc* attrs, size_t size) override;
        inline IInputLayout* createInputLayout(const std::vector<VertexAttributeDesc>& attrs) {return createInputLayout(attrs.data(), attrs.size());}
        IShader* createShader(ShaderType type, const void* data, size_t size) override;
        IGraphicsPipeline* createGraphicsPipeline(const GraphicsPipelineDesc& desc) override;
        ITexture* createTexture(const TextureDesc& desc) override;
        ISampler* createSampler(const SamplerDesc& desc) override;
        IFramebuffer* createFramebuffer(const FramebufferDesc& desc) override;
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
        void execute(ICommandList* cmd) override;


        RenderPassDesc currentRenderPass;
        GraphicsPipeline* currentPipeline;
        GladGLContext context;
    };
}