#pragma once
#include "alina/alina.hpp"

namespace alina::opengl {
    class Framebuffer : public ::alina::Framebuffer {
    public:
        Framebuffer(const FramebufferDesc& desc, ::alina::Device* device);

        uint32_t id;
        FramebufferDesc desc;
    };
}