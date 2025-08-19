#pragma once
#include <alina/alina.hpp>

namespace alina::opengl {
    class GlDevice;
    class GlGraphicsPipeline : public IGraphicsPipeline {
    public:
        GlGraphicsPipeline(GraphicsPipelineDesc desc, GlDevice* device);
        void bind();
        IDevice* getDevice() override;
        std::string getName() override {return mName;}
        void setName(const std::string& name) override {mName = name;}
        
        std::string mName;
        uint32_t vertexArray, program = 0;
        GraphicsPipelineDesc desc;
        GlDevice* device;
    };
}