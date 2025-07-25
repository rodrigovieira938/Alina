#include <alina/opengl-commandlist.hpp>

namespace alina::opengl {
    void CommandList::begin() {
        doneRecording = false;
        commands.clear();
    }
    void CommandList::draw(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::Draw {
                drawArgs
            }
        );
    }
    void CommandList::bindGraphicsPipeline(alina::IGraphicsPipeline* pipeline) {
        commands.emplace_back(
            Commands::BindGraphicsPipeline {
                pipeline
            }
        );
    }
    void CommandList::bindShaderResources(const ShaderResources& shaderResouces) {
        commands.emplace_back(
            Commands::BindShaderResources {
                shaderResouces
            }
        );
    }
    void CommandList::bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) {
        commands.emplace_back(
            Commands::BindVertexBuffers {
                buffers
            }
        );
    }
    void CommandList::bindIndexBuffer(IBuffer* buffer) {
        commands.emplace_back(
            Commands::BindIndexBuffer {
                buffer
            }
        );
    }
    void CommandList::drawIndexed(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::DrawIndexed {
                drawArgs
            }
        );
    }
    void CommandList::writeBuffer(IBuffer* buffer, const void* data, size_t size, size_t offset) {
        commands.emplace_back(
            Commands::WriteBuffer {
                buffer, data, size, offset
            }
        );
    }
    void CommandList::clearBuffer(IBuffer* buffer, uint32_t clearValue) {
        commands.emplace_back(
            Commands::ClearBuffer {
                buffer,clearValue
            }
        );
    }
    void CommandList::blitTexture(ITexture* src, ITexture* dest){
        commands.emplace_back(
            Commands::BlitTextures {
                src, dest
            }
        );
    }
    void CommandList::generateMipMaps(ITexture* tex){
        commands.emplace_back(
            Commands::GenerateMipMaps {
                tex
            }
        );
    }
    void CommandList::writeTexture(ITexture* tex, const void* data, TextureFormat dataFormat){
        commands.emplace_back(
            Commands::WriteTexture {
                tex,data,dataFormat
            }
        );
    }
    void CommandList::beginRenderPass(const RenderPassDesc& desc) {
        commands.emplace_back(
            Commands::BeginRenderPass {desc}
        );
    }
    void CommandList::beginSubPass(const SubPassDesc& desc) {
        commands.emplace_back(
            Commands::BeginSubPass {desc}
        );
    }
    void CommandList::endRenderPass() {
        commands.emplace_back(
            Commands::EndRenderPass {}
        );
    }
    void CommandList::end() {
        doneRecording = true;
    }
}