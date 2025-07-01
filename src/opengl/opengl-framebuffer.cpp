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
    }
}