#include "alina/alina.hpp"
#include <array>
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
#include <alina/opengl-framebuffer.hpp>
#include <vector>

namespace alina::opengl {
    GlDevice::GlDevice(glLoadFunction fn) {
        gladLoadGLContext(&context, fn);
        context.Enable(GL_CULL_FACE);
    }
    Texture GlDevice::createUnmanagedTexture(uint32_t id) {
        return std::make_shared<GlTexture>(id, this);
    }
    Framebuffer GlDevice::createUnmanagedFramebuffer(uint32_t id) {
        return std::make_shared<GlFramebuffer>(id, this);
    }
    bool GlDevice::beginFrame() {
        return true;
    }
    void GlDevice::endFrame() {
        
    }
    Buffer GlDevice::createBuffer(const BufferDesc& desc) {
        return std::make_shared<GlBuffer>(desc, this);
    }
    CommandList GlDevice::createCommandList() {
        return std::make_shared<GlCommandList>(this);
    }
    Shader GlDevice::createShader(ShaderType type, const void* data, size_t size) {
        return GlShader::createShader(this, type, data, size);
    }
    InputLayout GlDevice::createInputLayout(const VertexAttributeDesc* attrs, size_t size) {
        return std::make_shared<GlInputLayout>(this, attrs, size);
    }
    GraphicsPipeline GlDevice::createGraphicsPipeline(const GraphicsPipelineDesc& desc) {
        return std::make_shared<GlGraphicsPipeline>(desc, this);
    }
    Texture GlDevice::createTexture(const TextureDesc& desc) {
        return std::make_shared<GlTexture>(desc, this);
    }
    Sampler GlDevice::createSampler(const SamplerDesc& desc) {
        return std::make_shared<GlSampler>(desc, this);
    }
    Framebuffer GlDevice::createFramebuffer(const FramebufferDesc& desc) {
        return std::make_shared<GlFramebuffer>(desc, this);
    }
    void GlDevice::execute(const Commands::BindGraphicsPipeline& command) {
        currentPipeline = (GlGraphicsPipeline*)command.pipeline.get();
        currentPipeline->bind();
    }
    void GlDevice::execute(const Commands::BindShaderResources& command) {
        for(auto& ubo : command.shaderResources.uboBinding) {
            auto buffer = ((GlBuffer*)ubo.buffer.get());
            //TODO: deal with bindings
            context.BindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, buffer->mID);
        }
        int i = 0;
        for(auto& bindTex : command.shaderResources.texBinding) {
            auto buffer = ((GlTexture*)bindTex.texture.get());
            //TODO: deal with bindings
            context.ActiveTexture(GL_TEXTURE0+i);
            context.BindTexture(GL_TEXTURE_2D, buffer->id);
            if(bindTex.sampler) {
                auto sampler = ((GlSampler*)bindTex.sampler.get());
                context.BindSampler(i, sampler->id);
            }
//            context.Uniform1i(bindTex.binding, i++);
        }
    }

    void GlDevice::execute(const Commands::BindVertexBuffers& command) {
        for(int i = 0; i < command.buffers.size(); i++) {
            alina::BindVertexBuffer bindBuffer = command.buffers[i]; 
            GlBuffer* buffer = (GlBuffer*)bindBuffer.buffer.get();
            //context.BindVertexBuffer(i, buffer->mID, bindBuffer.offset, bindBuffer.offset);
            context.VertexArrayVertexBuffer(currentPipeline->vertexArray, i, buffer->mID, bindBuffer.offset, bindBuffer.stride);
        }
    }
    void GlDevice::execute(const Commands::BindIndexBuffer& command) {
        GlBuffer* buffer = (GlBuffer*)command.buffer.get();
        context.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->mID);
    }
    void GlDevice::execute(const Commands::Draw& command) {
        //TODO: change primitive
        context.DrawArraysInstanced(GL_TRIANGLES, command.args.offset, command.args.vertexCount, command.args.instanceCount);
    }
    void GlDevice::execute(const Commands::DrawIndexed& command) {
        //TODO: change primitive
        context.DrawElementsInstancedBaseVertex(GL_TRIANGLES, command.args.vertexCount, GL_UNSIGNED_INT, nullptr, command.args.instanceCount, command.args.offset);
    }
    void GlDevice::execute(const Commands::WriteBuffer& command) {
        GlBuffer* buffer = (GlBuffer*)command.buffer.get();
        GLenum bufferType = bufferTypeToGl(buffer->mType); 
        context.BindBuffer(bufferType, buffer->mID);
        if(command.size > buffer->size) {
            context.BufferData(bufferType, command.size, command.data, GL_DYNAMIC_DRAW);
            buffer->size = command.size;
        }
        else
            context.BufferSubData(bufferType, command.offset, command.size, command.data);
        context.BindBuffer(bufferType, 0);
    }
    void GlDevice::execute(const Commands::ClearBuffer& command) {
        GlBuffer* buffer = (GlBuffer*)command.buffer.get();
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

    void GlDevice::execute(const Commands::BlitTextures& command) {
        auto src = ((GlTexture*)command.src.get());
        auto dest = ((GlTexture*)command.dest.get());
        
        context.CopyImageSubData(src->id, src->desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0, 0, 0, 0, dest->id, dest->desc.depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0, 0, 0, 0, src->desc.width, src->desc.height, src->desc.depth);
    }
    void GlDevice::execute(const Commands::GenerateMipMaps& command) {
        context.GenerateTextureMipmap(((GlTexture*)command.tex.get())->id);
    }
    //TODO: add support for conversion from unsupported formats
    void GlDevice::execute(const Commands::WriteTexture& command) {
        auto tex = ((GlTexture*)command.tex.get());
        context.TextureSubImage2D(tex->id, 0, 0, 0, tex->desc.width, tex->desc.height, textureFormatsToGlChannels(command.dataFormat), textureFormatsToGlType(command.dataFormat), command.data);
    }
    void GlDevice::execute(const Commands::BeginRenderPass& command) {
        //TODO: validate renderpass
        //TODO: replace this makeshift support of default framebuffer
        auto fb = (GlFramebuffer*)command.desc.fb.get();
        context.BindFramebuffer(GL_FRAMEBUFFER, fb->id);
        int count = 1;
        if(fb->id == 0) {
            count = 1;
        } else {
            for(count = 0; fb->desc.colorAttachments[count].texture != nullptr; count++);
        }

        for(int i = 0; i < count; i++) {
            if(command.desc.loadOps[i] == RenderPassLoadOp::CLEAR) {
                context.ClearBufferfv(GL_COLOR, i, command.desc.clearColors[i].data());
            }
        }
        currentRenderPass = command.desc;
    }
    void GlDevice::execute(const Commands::BeginSubPass& command) {
        //TODO: validate subpass
        //TODO: replace this makeshift support of default framebuffer
        std::array<GLenum, 8> drawBuffers;
        int count = 0;
        int it = 0;
        GlFramebuffer* fb = (GlFramebuffer*)currentRenderPass.fb.get();
        if(fb->id != 0) {
            for(int i = 0; fb->desc.colorAttachments[i].texture.get() != nullptr;i++) {
                if(command.desc.attachments[i] == SubPassAttachment::COLOR) {
                    drawBuffers[it] = GL_COLOR_ATTACHMENT0 + i;
                    it++;
                    count++;
                }
            }
        } else {
            if(command.desc.attachments[0] == SubPassAttachment::COLOR) {
                drawBuffers[0] = GL_COLOR_ATTACHMENT0;
                count++;
            }
        }

        context.DrawBuffers(count, drawBuffers.data());
    }

    void GlDevice::execute(const Commands::EndRenderPass& command) {
        context.BindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void GlDevice::execute(CommandList cmd) {
        for (auto& command : ((GlCommandList*)cmd.get())->commands) {
            std::visit([this](auto&& arg){
                this->execute(arg);
            }, command);
        }
    }
}
