#pragma once
#include <alina/opengl.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    class Device : public ::alina::Device {
    public:
        Device(glLoadFunction fn);
        bool beginFrame() override;
        void endFrame() override;
        Buffer* createBuffer(const BufferDesc& desc) override;
        GladGLContext context;
    };
}