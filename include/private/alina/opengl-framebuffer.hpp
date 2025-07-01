#pragma once
#include "alina/alina.hpp"

namespace alina::opengl {
    class Framebuffer : public IFramebuffer {
    public:
        Framebuffer(const FramebufferDesc& desc, IDevice* device);
        Framebuffer(uint32_t id, IDevice* device);

        uint32_t id;
        FramebufferDesc desc;
    };
}