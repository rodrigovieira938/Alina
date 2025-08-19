#pragma once
#include <alina/alina.hpp>
#include <variant>
#include <vector>

namespace alina::opengl {
    class GlDevice;
    namespace Commands {
        struct Draw {
            DrawArguments args;
        };
        struct DrawIndexed {
            DrawArguments args;
        };
        struct WriteBuffer {
            Buffer buffer; 
            const void* data;
            size_t size;
            size_t offset;
        };
        struct ClearBuffer {
            Buffer buffer;
            uint32_t clearValue;
        };
        struct BindGraphicsPipeline {
            alina::GraphicsPipeline pipeline;
        };
        struct BindVertexBuffers {
            std::vector<BindVertexBuffer> buffers;
        };
        struct BindIndexBuffer {
            Buffer buffer;
        };
        struct BindShaderResources {
            ShaderResources shaderResources;
        };
        struct BlitTextures {
            Texture src, dest;
        };
        struct GenerateMipMaps {
            Texture tex;
        };
        struct WriteTexture {
            Texture tex;
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
    class GlCommandList : public ICommandList {
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
        GlCommandList(GlDevice* device);
        void begin() override;
        void bindGraphicsPipeline(alina::GraphicsPipeline pipeline) override;
        void bindShaderResources(const ShaderResources& shaderResouces) override;
        void bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) override;
        void bindIndexBuffer(Buffer buffer) override;
        void draw(const DrawArguments& drawArgs) override;
        void drawIndexed(const DrawArguments& drawArgs) override;
        void writeBuffer(Buffer buffer, const void* data, size_t size, size_t offset) override;
        void clearBuffer(Buffer buffer, uint32_t clearValue) override;
        void blitTexture(Texture src, Texture dest) override;
        void generateMipMaps(Texture tex) override;
        void writeTexture(Texture tex, const void* data, TextureFormat dataFormat) override;
        void beginRenderPass(const RenderPassDesc& desc) override;
        void beginSubPass(const SubPassDesc& desc) override;
        void endRenderPass() override;
        void end() override;
        IDevice* getDevice() override;
        std::string getName() override {return mName;}
        void setName(const std::string& name) override {mName = name;}
        std::string mName;
        std::vector<Command> commands = {};
        GlDevice* device;
        bool doneRecording = false;
    };
}