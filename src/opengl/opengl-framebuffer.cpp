#include "alina/alina.hpp"
#include "glad/gl.h"
#include <alina/opengl-framebuffer.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-texture.hpp>
namespace alina::opengl {
    GlFramebuffer::GlFramebuffer(const FramebufferDesc& desc, GlDevice* device) {
        this->device = device;
        auto context = device->context;
        context.CreateFramebuffers(1, &id);
        uint32_t index = GL_COLOR_ATTACHMENT0;
        for(auto& attachment : desc.colorAttachments) {
            if(attachment.texture == nullptr) continue;
            context.NamedFramebufferTexture(id, index, ((GlTexture*)attachment.texture.get())->id, 0);
            index++;
        }
        if(desc.depthAttachment.texture != nullptr) {
            context.NamedFramebufferTexture(id, index, ((GlTexture*)desc.depthAttachment.texture.get())->id, 0);
        }
        this->desc = desc;
        context.ObjectLabel(GL_FRAMEBUFFER, id, desc.name.size(), desc.name.data());
    }
    GlFramebuffer::GlFramebuffer(uint32_t id, GlDevice* device) {
        this->device = device;
        auto context = device->context;
        this->id = id;
        if(id == 0) {
            //TODO: deal with default framebuffer
            return;
        }
        
        GLint type;
        uint32_t i = 0;
        while (true) {
            context.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
            desc.colorAttachments[i] = FramebufferAttachment().setTexture(device->createUnmanagedTexture(i));
            i++;
            if(type == GL_RENDERBUFFER) //Cannot be used as a texture
                continue;

            if(type != GL_TEXTURE)
                break;
        }
        //TODO: add support for depth and stencil when texture supports it 
    }
    std::string GlFramebuffer::getName() {
        GLsizei length;
        device->context.GetObjectLabel(GL_BUFFER, id, 0, &length, nullptr);
        std::string str;
        str.resize(length, '\0');
        device->context.GetObjectLabel(GL_BUFFER, id, length+1, &length, str.data());
        return str;
    }
    void GlFramebuffer::setName(const std::string& name) {
        device->context.ObjectLabel(GL_BUFFER, id, name.size(), name.data());
    }
}