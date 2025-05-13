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
        ::alina::GraphicsPipeline* createGraphicsPipeline(const GraphicsPipelineDesc& desc) override; 
        void execute(const Commands::BindGraphicsPipeline& command);
        void execute(const Commands::Draw& command);
        void execute(const Commands::DrawIndexed& command);
        void execute(const Commands::WriteBuffer& command);
        void execute(const Commands::ClearBuffer& command);
        void execute(::alina::CommandList* cmd) override;

        GladGLContext context;
    };
}