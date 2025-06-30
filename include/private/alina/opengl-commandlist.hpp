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
            Buffer* buffer; 
            const void* data;
            size_t size;
            size_t offset;
        };
        struct ClearBuffer {
            Buffer* buffer;
            uint32_t clearValue;
        };
        struct BindGraphicsPipeline {
            alina::GraphicsPipeline* pipeline;
        };
        struct BindVertexBuffers {
            std::vector<BindVertexBuffer> buffers;
        };
        struct BindIndexBuffer {
            Buffer* buffer;
        };
        struct BindShaderResources {
            const ShaderResources& shaderResources;
        };
        struct BlitTextures {
            Texture *src, *dest;
        };
        struct GenerateMipMaps {
            Texture* tex;
        };
        struct WriteTexture {
            Texture* tex;
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
    class CommandList : public ::alina::CommandList {
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
        void bindGraphicsPipeline(alina::GraphicsPipeline* pipeline) override;
        void bindShaderResources(const ShaderResources& shaderResouces) override;
        void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) override;
        void bindIndexBuffer(Buffer* buffer) override;
        void draw(const DrawArguments& drawArgs) override;
        void drawIndexed(const DrawArguments& drawArgs) override;
        void writeBuffer(Buffer* buffer, const void* data, size_t size, size_t offset) override;
        void clearBuffer(Buffer* buffer, uint32_t clearValue) override;
        void blitTexture(::alina::Texture* src, ::alina::Texture* dest) override;
        void generateMipMaps(::alina::Texture* tex) override;
        void writeTexture(::alina::Texture* tex, const void* data, TextureFormat dataFormat) override;
        void beginRenderPass(const RenderPassDesc& desc) override;
        void beginSubPass(const SubPassDesc& desc) override;
        void endRenderPass() override;
        void end() override;
        std::vector<Command> commands = {};
        bool doneRecording = false;
    };
}