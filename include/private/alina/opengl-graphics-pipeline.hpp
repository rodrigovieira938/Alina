#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class Device;
    class GraphicsPipeline : public ::alina::GraphicsPipeline {
    public:
        GraphicsPipeline(GraphicsPipelineDesc desc, Device* device);
        void bind();

        GraphicsPipelineDesc desc;
        Device* device;
    };
}