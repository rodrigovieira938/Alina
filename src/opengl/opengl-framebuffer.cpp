#include "alina/alina.hpp"
#include "glad/gl.h"
#include <alina/opengl-framebuffer.hpp>
#include <alina/opengl-device.hpp>
#include <alina/opengl-texture.hpp>
namespace alina::opengl {
    Framebuffer::Framebuffer(const FramebufferDesc& desc, IDevice* device) {
        auto context = ((Device*)device)->context;
        context.CreateFramebuffers(1, &id);
        uint32_t index = GL_COLOR_ATTACHMENT0;
        for(auto& attachment : desc.colorAttachments) {
            if(attachment.texture == nullptr) continue;
            context.NamedFramebufferTexture(id, index, ((Texture*)attachment.texture)->id, 0);
            index++;
        }
        if(desc.depthAttachment.texture != nullptr) {
            context.NamedFramebufferTexture(id, index, ((Texture*)desc.depthAttachment.texture)->id, 0);
        }
        this->desc = desc;
        context.ObjectLabel(GL_FRAMEBUFFER, id, desc.name.size(), desc.name.data());
    }
    Framebuffer::Framebuffer(uint32_t id, IDevice* device) {
        auto glDevice = (Device*)device;
        auto context = glDevice->context;
        this->id = id;
        if(id == 0) {
            //TODO: deal with default framebuffer
            return;
        }
        
        GLint type;
        uint32_t i = 0;
        while (true) {
            context.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
            desc.colorAttachments[i] = FramebufferAttachment().setTexture(glDevice->createUnmanagedTexture(i));
            i++;
            if(type == GL_RENDERBUFFER) //Cannot be used as a texture
                continue;

            if(type != GL_TEXTURE)
                break;
        }
        //TODO: add support for depth and stencil when texture supports it 
    }
}