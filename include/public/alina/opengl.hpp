#pragma once
#include <alina/alina.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    typedef void (*GLfunction)(void);
    typedef GLfunction (*glLoadFunction)(const char *name);
    IDevice* CreateDevice(glLoadFunction fn);

    struct IGlDevice : public IDevice {
        virtual GladGLContext getContext() = 0;
        virtual ITexture* createUnmanagedTexture(uint32_t id) = 0;
    };
} 