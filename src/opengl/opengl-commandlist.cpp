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
    void CommandList::bindGraphicsPipeline(alina::GraphicsPipeline* pipeline) {
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
    void CommandList::bindIndexBuffer(Buffer* buffer) {
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
    void CommandList::writeBuffer(Buffer* buffer, const void* data, size_t size, size_t offset) {
        commands.emplace_back(
            Commands::WriteBuffer {
                buffer, data, size, offset
            }
        );
    }
    void CommandList::clearBuffer(Buffer* buffer, uint32_t clearValue) {
        commands.emplace_back(
            Commands::ClearBuffer {
                buffer,clearValue
            }
        );
    }
    void CommandList::end() {
        doneRecording = true;
    }
}