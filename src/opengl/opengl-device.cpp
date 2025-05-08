#include "alina/alina.hpp"
#include <glad/gl.h>
#include <alina/opengl-device.hpp>
#include <alina/opengl-buffer.hpp>
#include <alina/opengl-commandlist.hpp>
#include <variant>

namespace alina::opengl {
    Device::Device(glLoadFunction fn) {
        gladLoadGLContext(&context, fn);
    }
    bool Device::beginFrame() {
        context.Clear(GL_COLOR_BUFFER_BIT);
        return true;
    }
    void Device::endFrame() {
        
    }
    ::alina::Buffer* Device::createBuffer(const BufferDesc& desc) {
        return new Buffer(desc, this);
    }
    ::alina::CommandList* Device::createCommandList() {
        return new CommandList();
    }

    void Device::execute(const Commands::Draw& drawCommand) {
        //TODO: change primitive
        context.DrawArrays(GL_TRIANGLES, drawCommand.args.offset, drawCommand.args.vertexCount);
    }
    void Device::execute(const Commands::DrawIndexed& drawCommand) {
        //TODO: change primitive
        context.DrawElementsBaseVertex(GL_TRIANGLES, drawCommand.args.vertexCount, GL_UNSIGNED_INT, nullptr, drawCommand.args.offset);
    }

    void Device::execute(::alina::CommandList* cmd) {
        for (auto& command : ((CommandList*)cmd)->commands) {
            std::visit([this](auto&& arg){
                this->execute(arg);
            }, command);
        }
    }
}
