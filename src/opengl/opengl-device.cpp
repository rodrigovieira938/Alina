#include "alina/alina.hpp"
#include <cmath>
#include <glad/gl.h>
#include <alina/opengl-device.hpp>
#include <alina/opengl-buffer.hpp>
#include <alina/opengl-commandlist.hpp>
#include <alina/opengl-conversions.hpp>
#include <alina/opengl-graphics-pipeline.hpp>
#include <alina/opengl-inputlayout.hpp>
#include <alina/opengl-shader.hpp>
#include <vector>

namespace alina::opengl {
    Device::Device(glLoadFunction fn) {
        gladLoadGLContext(&context, fn);
        context.Enable(GL_CULL_FACE);
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
    ::alina::Shader* Device::createShader(ShaderType type, const void* data, size_t size) {
        return Shader::createShader(this, type, data, size);
    }
    ::alina::InputLayout* Device::createInputLayout(const VertexAttributeDesc* attrs, size_t size) {
        return new InputLayout(attrs, size);
    }
    ::alina::GraphicsPipeline* Device::createGraphicsPipeline(const GraphicsPipelineDesc& desc) {
        return new GraphicsPipeline(desc, this);
    }

    void Device::execute(const Commands::BindGraphicsPipeline& command) {
        currentPipeline = (GraphicsPipeline*)command.pipeline;
        currentPipeline->bind();
    }
    void Device::execute(const Commands::BindVertexBuffers& command) {
        for(int i = 0; i < command.buffers.size(); i++) {
            alina::BindVertexBuffer bindBuffer = command.buffers[i]; 
            Buffer* buffer = (Buffer*)bindBuffer.buffer;
            //context.BindVertexBuffer(i, buffer->mID, bindBuffer.offset, bindBuffer.offset);
            context.VertexArrayVertexBuffer(currentPipeline->vertexArray, i, buffer->mID, bindBuffer.offset, bindBuffer.stride);
        }
    }
    void Device::execute(const Commands::BindIndexBuffer& command) {
        Buffer* buffer = (Buffer*)command.buffer;
        context.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->mID);
    }
    void Device::execute(const Commands::Draw& command) {
        //TODO: change primitive
        context.DrawArraysInstanced(GL_TRIANGLES, command.args.offset, command.args.vertexCount, command.args.instanceCount);
    }
    void Device::execute(const Commands::DrawIndexed& command) {
        //TODO: change primitive
        context.DrawElementsInstancedBaseVertex(GL_TRIANGLES, command.args.vertexCount, GL_UNSIGNED_INT, nullptr, command.args.instanceCount, command.args.offset);
    }
    void Device::execute(const Commands::WriteBuffer& command) {
        Buffer* buffer = (Buffer*)command.buffer;
        GLenum bufferType = bufferTypeToGl(buffer->mType); 
        context.BindBuffer(bufferType, buffer->mID);
        if(command.size > buffer->size)
            context.BufferData(bufferType, command.size, command.data, GL_DYNAMIC_DRAW);
        else
            context.BufferSubData(bufferType, command.offset, command.size, command.data);
        buffer->size = command.size;
        context.BindBuffer(bufferType, 0);
    }
    void Device::execute(const Commands::ClearBuffer& command) {
        Buffer* buffer = (Buffer*)command.buffer;
        std::vector<uint32_t> data;
        //Simple round up
        size_t size = (buffer->size % sizeof(uint32_t) == 0) ? buffer->size / sizeof(uint32_t) : buffer->size / sizeof(uint32_t) + sizeof(uint32_t);
        data.reserve(size);
        for(int i = 0; i < buffer->size; i++) {
            data.push_back(command.clearValue);
        }
        if(buffer->size > 0) {
            GLenum bufferType = bufferTypeToGl(buffer->mType); 
            context.BindBuffer(bufferType, buffer->mID);
            context.BufferSubData(bufferType, 0, buffer->size, data.data());
            context.BindBuffer(bufferType, 0);
        }
    }

    void Device::execute(::alina::CommandList* cmd) {
        for (auto& command : ((CommandList*)cmd)->commands) {
            std::visit([this](auto&& arg){
                this->execute(arg);
            }, command);
        }
    }
}
