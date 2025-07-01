#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class Device;
    class GraphicsPipeline : public IGraphicsPipeline {
    public:
        GraphicsPipeline(GraphicsPipelineDesc desc, Device* device);
        void bind();

        uint32_t vertexArray, program = 0;
        GraphicsPipelineDesc desc;
        Device* device;
    };
}