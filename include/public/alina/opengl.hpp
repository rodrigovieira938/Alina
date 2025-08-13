#pragma once
#include <alina/alina.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    typedef void (*GLfunction)(void);
    typedef GLfunction (*glLoadFunction)(const char *name);
    Device CreateDevice(glLoadFunction fn);

    struct IGlDevice : public IDevice {
        virtual GladGLContext getContext() = 0;
        virtual Texture createUnmanagedTexture(uint32_t id) = 0;
        virtual Framebuffer createUnmanagedFramebuffer(uint32_t id) = 0;
    };
} 