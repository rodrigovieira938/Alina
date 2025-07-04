#pragma once
#include <alina/alina.hpp>
#include <variant>
#include <vector>

namespace alina::opengl {
    namespace Commands {
        struct Draw {
            DrawArguments args;
        };
        struct DrawIndexed {
            DrawArguments args;
        };
        struct WriteBuffer {
            IBuffer* buffer; 
            const void* data;
            size_t size;
            size_t offset;
        };
        struct ClearBuffer {
            IBuffer* buffer;
            uint32_t clearValue;
        };
        struct BindGraphicsPipeline {
            alina::IGraphicsPipeline* pipeline;
        };
        struct BindVertexBuffers {
            std::vector<BindVertexBuffer> buffers;
        };
        struct BindIndexBuffer {
            IBuffer* buffer;
        };
        struct BindShaderResources {
            ShaderResources shaderResources;
        };
        struct BlitTextures {
            ITexture *src, *dest;
        };
        struct GenerateMipMaps {
            ITexture* tex;
        };
        struct WriteTexture {
            ITexture* tex;
            const void* data;
            TextureFormat dataFormat;
        };
        struct BeginRenderPass {
            RenderPassDesc desc;
        };
        struct BeginSubPass {
            SubPassDesc desc;
        };
        struct EndRenderPass {};
    };
    class CommandList : public ICommandList {
    public:
        using Command = std::variant<
            Commands::Draw,
            Commands::DrawIndexed,
            Commands::WriteBuffer,
            Commands::ClearBuffer,
            Commands::BindGraphicsPipeline,
            Commands::BindVertexBuffers,
            Commands::BindIndexBuffer,
            Commands::BindShaderResources,
            Commands::BlitTextures,
            Commands::GenerateMipMaps,
            Commands::WriteTexture,
            Commands::BeginRenderPass,
            Commands::BeginSubPass,
            Commands::EndRenderPass
        >;
        void begin() override;
        void bindGraphicsPipeline(alina::IGraphicsPipeline* pipeline) override;
        void bindShaderResources(const ShaderResources& shaderResouces) override;
        void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) override;
        void bindIndexBuffer(IBuffer* buffer) override;
        void draw(const DrawArguments& drawArgs) override;
        void drawIndexed(const DrawArguments& drawArgs) override;
        void writeBuffer(IBuffer* buffer, const void* data, size_t size, size_t offset) override;
        void clearBuffer(IBuffer* buffer, uint32_t clearValue) override;
        void blitTexture(ITexture* src, ITexture* dest) override;
        void generateMipMaps(ITexture* tex) override;
        void writeTexture(ITexture* tex, const void* data, TextureFormat dataFormat) override;
        void beginRenderPass(const RenderPassDesc& desc) override;
        void beginSubPass(const SubPassDesc& desc) override;
        void endRenderPass() override;
        void end() override;
        std::vector<Command> commands = {};
        bool doneRecording = false;
    };
}