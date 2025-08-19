#include <alina/opengl-commandlist.hpp>
#include <alina/opengl-device.hpp>

namespace alina::opengl {
    GlCommandList::GlCommandList(GlDevice* device) {
        this->device = device;
    };
    IDevice* GlCommandList::getDevice() {return device;}
    void GlCommandList::begin() {
        doneRecording = false;
        commands.clear();
    }
    void GlCommandList::draw(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::Draw {
                drawArgs
            }
        );
    }
    void GlCommandList::bindGraphicsPipeline(alina::GraphicsPipeline pipeline) {
        commands.emplace_back(
            Commands::BindGraphicsPipeline {
                pipeline
            }
        );
    }
    void GlCommandList::bindShaderResources(const ShaderResources& shaderResouces) {
        commands.emplace_back(
            Commands::BindShaderResources {
                shaderResouces
            }
        );
    }
    void GlCommandList::bindVertexBuffers(const std::vector<BindVertexBuffer>& buffers) {
        commands.emplace_back(
            Commands::BindVertexBuffers {
                buffers
            }
        );
    }
    void GlCommandList::bindIndexBuffer(Buffer buffer) {
        commands.emplace_back(
            Commands::BindIndexBuffer {
                buffer
            }
        );
    }
    void GlCommandList::drawIndexed(const DrawArguments& drawArgs) {
        commands.emplace_back(
            Commands::DrawIndexed {
                drawArgs
            }
        );
    }
    void GlCommandList::writeBuffer(Buffer buffer, const void* data, size_t size, size_t offset) {
        commands.emplace_back(
            Commands::WriteBuffer {
                buffer, data, size, offset
            }
        );
    }
    void GlCommandList::clearBuffer(Buffer buffer, uint32_t clearValue) {
        commands.emplace_back(
            Commands::ClearBuffer {
                buffer,clearValue
            }
        );
    }
    void GlCommandList::blitTexture(Texture src, Texture dest){
        commands.emplace_back(
            Commands::BlitTextures {
                src, dest
            }
        );
    }
    void GlCommandList::generateMipMaps(Texture tex){
        commands.emplace_back(
            Commands::GenerateMipMaps {
                tex
            }
        );
    }
    void GlCommandList::writeTexture(Texture tex, const void* data, TextureFormat dataFormat){
        commands.emplace_back(
            Commands::WriteTexture {
                tex,data,dataFormat
            }
        );
    }
    void GlCommandList::beginRenderPass(const RenderPassDesc& desc) {
        commands.emplace_back(
            Commands::BeginRenderPass {desc}
        );
    }
    void GlCommandList::beginSubPass(const SubPassDesc& desc) {
        commands.emplace_back(
            Commands::BeginSubPass {desc}
        );
    }
    void GlCommandList::endRenderPass() {
        commands.emplace_back(
            Commands::EndRenderPass {}
        );
    }
    void GlCommandList::end() {
        doneRecording = true;
    }
}