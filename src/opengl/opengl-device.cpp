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
#include <alina/opengl-texture.hpp>
#include <alina/opengl-sampler.hpp>
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
    ::alina::Texture* Device::createTexture(const TextureDesc& desc) {
        return new Texture(desc, this);
    }
    ::alina::Sampler* Device::createSampler(const SamplerDesc& desc) {
        return new Sampler(desc, this);
    }
    void Device::execute(const Commands::BindGraphicsPipeline& command) {
        currentPipeline = (GraphicsPipeline*)command.pipeline;
        currentPipeline->bind();
    }
    void Device::execute(const Commands::BindShaderResources& command) {
        for(auto& ubo : command.shaderResources.uboBinding) {
            auto buffer = ((Buffer*)ubo.buffer);
            //TODO: deal with bindings
            context.BindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, buffer->mID);
        }
        for(auto& bindTex : command.shaderResources.texBinding) {
            auto buffer = ((Texture*)bindTex.texture);
            //TODO: deal with bindings & multiple textures
            context.ActiveTexture(GL_TEXTURE0);
            context.BindTexture(GL_TEXTURE_2D, buffer->id);
            if(bindTex.sampler) {
                auto sampler = ((Sampler*)bindTex.sampler);
                context.BindSampler(0, sampler->id);
            }
            context.Uniform1i(bindTex.binding, 0);
        }
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

    void Device::execute(const Commands::BlitTextures& command) {
        auto src = ((Texture*)command.src);
        auto dest = ((Texture*)command.dest);
        
        context.CopyImageSubData(src->id, src->desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0, 0, 0, 0, dest->id, dest->desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0, 0, 0, 0, src->desc.width, src->desc.height, src->desc.depth);
    }
    void Device::execute(const Commands::GenerateMipMaps& command) {
        context.GenerateTextureMipmap(((Texture*)command.tex)->id);
    }
    //TODO: add support for conversion from unsupported formats
    void Device::execute(const Commands::WriteTexture& command) {
        auto tex = ((Texture*)command.tex);
        context.TextureSubImage2D(tex->id, 0, 0, 0, tex->desc.width, tex->desc.height, textureFormatsToGlChannels(command.dataFormat), textureFormatsToGlType(command.dataFormat), command.data);
    }

    void Device::execute(::alina::CommandList* cmd) {
        for (auto& command : ((CommandList*)cmd)->commands) {
            std::visit([this](auto&& arg){
                this->execute(arg);
            }, command);
        }
    }
}
