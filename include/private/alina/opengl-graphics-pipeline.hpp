#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class GlDevice;
    class GlGraphicsPipeline : public IGraphicsPipeline {
    public:
        GlGraphicsPipeline(GraphicsPipelineDesc desc, GlDevice* device);
        void bind();

        uint32_t vertexArray, program = 0;
        GraphicsPipelineDesc desc;
        GlDevice* device;
    };
}