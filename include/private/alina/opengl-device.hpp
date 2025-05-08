#pragma once
#include <alina/opengl.hpp>
#include <alina/opengl-commandlist.hpp>
#include <glad/gl.h>

namespace alina::opengl {
    class Device : public ::alina::Device {
    public:
        Device(glLoadFunction fn);
        bool beginFrame() override;
        void endFrame() override;
        Buffer* createBuffer(const BufferDesc& desc) override;
        ::alina::CommandList* createCommandList() override;
        void execute(const Commands::Draw& drawCommand);
        void execute(const Commands::DrawIndexed& drawCommand);
        void execute(::alina::CommandList* cmd) override;

        GladGLContext context;
    };
}